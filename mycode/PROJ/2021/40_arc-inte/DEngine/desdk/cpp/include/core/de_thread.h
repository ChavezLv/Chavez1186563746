/**
 * @file de_thread.h
 * @brief Defines multithread utilities for DeSdk
 * @author DeSdk Team
 * @date 2019/12/15
 * @copyright Intellif
 */

#ifndef DE_THREAD_H
#define DE_THREAD_H
#include <string>
#include <condition_variable>
#include "de_ndarray.h"
#include "de_prop.h"
#include "de_packed_func.h"

#define VPOUT_CONVERCT_FUNC(idx, classname, funcname) \
    virpout_task_convert_func_[idx] = std::bind(&classname::funcname, this, \
    std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)

enum POutType
{
    DEFAULT_POUT,
    USER_POUT
};

namespace de
{

const constexpr int kL1CacheBytes = 64;

/**
 * @brief  General queue, supporting SISO(no lock) and MISO(with lock)
 */
class DE_DLL TaskQueue {
public:
	enum {
		SISO = 0, // Lock-free single-producer-single-consumer queue for each thread,
		MISO, // multiple-producer-single-consumer queue
	};

	/*! \brief The task entry */
	struct Task {
		void* task;
		// debug info
		int32_t task_id;
	};

	using Deleter = std::function<void(TaskQueue::Task)>;

	/*!
	 * \brief 设置模式缺省为SISO.
	 */
	void set_mode(int mode = SISO);

	/*!
	 * \brief 创建ring_sz的队列，模式缺省为SISO.
	 */
	TaskQueue(int ring_sz = 1, int mode = SISO);
	~TaskQueue();

	/*!
	 * \brief Push a task into the queue and notify the comsumer if it is on wait.
	 * \param input The task to be dequeued.
	 */
	void Push(void* user_task, int user_task_id);

	/*!
	 * \brief Pop a task out of the queue and condition wait if no tasks.
	 * \param output The pointer to the task to be dequeued.
	 * \param spin_count The number of iterations to spin before sleep.
	 * \return Whether pop is successful (true) or we need to exit now (false).
	 */
	bool Pop(Task* output, uint32_t spin_count = 0);

	/*!
	 * \brief Signal to terminate the worker.
	 */
	void SignalForKill(Deleter delter);

	template<typename T>
	void SignalForKill(void) {
		if (nullptr == task_deleter_) {
			std::lock_guard<std::mutex> lock(mutex_);
			task_deleter_ = [](Task job) {
				delete reinterpret_cast<T*>(job.task);
			};

			exit_now_.store(true, std::memory_order_release);
			cv_.notify_all();
		}
	}

	/*!
	 * \brief 队列中pending任务个数.
	 */
	uint32_t Size();

	/*!
	 * \brief 队列中任务容量，最多能缓存的任务个数.
	 */
	uint32_t Capacity();

protected:
	void Push(const Task& input);
	bool Enqueue(const Task& input);
	// the cache line paddings are used for avoid false sharing between atomic variables
	typedef char cache_line_pad_t[kL1CacheBytes];
	cache_line_pad_t pad0_;
	int ring_sz_ = 1;
	Task* const buffer_;

	cache_line_pad_t pad1_;
	// queue head, where one gets a task from the queue
	std::atomic<uint32_t> head_;

	cache_line_pad_t pad2_;
	// queue tail, when one puts a task to the queue
	std::atomic<uint32_t> tail_;

	cache_line_pad_t pad3_;
	// pending tasks in the queue
	std::atomic<int8_t> pending_{ 0 };

	cache_line_pad_t pad4_;
	// signal for exit now
	std::atomic<bool> exit_now_{ false };

	// internal mutex
	std::mutex mutex_;
	// cv for consumer
	std::condition_variable cv_;

	Deleter task_deleter_{ nullptr };

	int mode_ = SISO;
};
}	//end of namepsace de

#include "de_task_serialize.h"

namespace de{

using FTaskSer = std::function<NDArray(void* task)>;
using FTaskDeser = std::function<void*(NDArray)>;
using FTaskDeleter = std::function<void(TaskQueue::Task)>;

class Thread;		//forward declaration

/**
 * @brief Port in
 */
class DE_DLL PIN
{
public:
    PIN(Thread* pthread) ;
    ~PIN();

    static constexpr int kMaxTaskTypeNum = 16;

    /**
     * @brief Destroy queue
     */
    void DestroyQueue(void);

    /*!
     * @brief Get queue
     */
    uint64_t GetQueue(void)
    {
        return queue_;
    }

    /*!
     * @brief Get size of queue
     * @return Size of queue
     * @note Must make sure that queue is valid
     */
    int Size(void);

    /**
     * @brief get task from queue
     */
    void* RecvTask(int32_t& task_type);

    /**
     * @brief 设置type类型信息
     * @code
     *		// 假设NodeClass A能处理Class NodeClassX::TaskX和Class NodeClassY::TaskY的任务
     *		void NodeClassA::Proc(void* rx_task, int32_t task_type_id) {
     *			if(task_type_id == 0) {
     *			// 来自于NodeClass X
     *			auto task = (NodeClassX::TaskX) rx_task;
     *			}
     *			...
     *		}
     *
     *		NodeClassA::NodeClassX() {
     *			pin.SetTypeInfo(0, "NodeClassX", de::TaskDeSerialize<TaskX>);
     *			pin.SetTypeInfo(1, "NodeClassY", de::TaskDeSerialize<TaskY>);
     *		}
    	@endcode
     */
    void SetTypeInfo(int id, const std::string& task_type, FTaskDeser deser_func, FTaskDeleter task_deleter);

    /**
     * @brief Get task type id by type info
     */
    int GetTaskTypeId(const std::string& type_info);


    /**
     * @brief set pin name
     */
    void SetName(const std::string& node_name);

    /*!
     * @breif Get name
     */
    std::string GetName(void)
    {
        return name_;
    }

    /**
     * @brief delete task
     * @param task pointer to a task
	 * @param task_type_id task type
     * @note task deleter must be exist!
     */
    void DelTask(void* task, int task_type_id);

    /**
     * @brief set pin link attr, if flag is true, the data(task_type)  comes from remote,
     * else flag is false, the data(task_type)  comes from local.
     */
    void SetBridgeFlg(int task_type, bool flag);

private:
    std::string name_;
    uint64_t queue_ = 0;		///< pin always has a queue_
    bool bridge_[kMaxTaskTypeNum] { false, }; ///< task come from bridge
    std::string types_info_[kMaxTaskTypeNum];
    FTaskDeser types_deser_info_[kMaxTaskTypeNum] { nullptr };
    FTaskDeleter types_deleter_info_[kMaxTaskTypeNum] { nullptr };
    Thread* pthread_;

    friend class POUT;
    friend class Graph;
    friend class Thread;
	friend class Node;
};

/**
 * @brief virtual pout
 */
class DE_DLL POUTBase
{
public:
    virtual ~POUTBase(){}
    virtual void SendTask(void* task);

    //任务缓存
    static constexpr int MAX_CACHE_TASK_NUM = 8;
    int task_num = 0;
    void* cache_task[MAX_CACHE_TASK_NUM]{nullptr, };
};

/**
 * @brief Port out
 */
class DE_DLL POUT : public POUTBase
{
public:
    POUT(Thread* parent) ;
    virtual ~POUT() ;

    /**
     * @brief get pin name
     * @param idx    输出POUT的index，不超过8个
     */
    void SetName(const std::string& node_name);

    /**
     * @brief Create queue
     * @note a pout need to create queue only if it is a graph output
     */
    void CreateQueue(void);

    /**
     * @brief Destroy queue
     * @note Used to destroy a queue that handles only one type of task
     */
    template<class T>
    void DestroyQueue(void)
    {
		if (exist_) {
			reinterpret_cast<TaskQueue*>(queue_)->SignalForKill<T>();
		}
    }

    /**
     * @brief Set type infomation
     * @code
     *		NodeClassA {
     *			pout.SetTypeInfo(0, de::TaskSerialize<TaskA>);
     *			pout.SetTypeInfo(1, de::TaskSerialize<TaskB>);
     *		}
    	@endcode
     */
    void SetTypeInfo(const std::string& task_type, FTaskSer ser_func, FTaskDeleter deleter_func = nullptr);

    /*!
     * @brief Set queue
     */
    void SetQueue(uint64_t queue);

    /**
     * @brief Send Task
     */
    virtual void SendTask(void* task) override;

    /*!
     * @brief Get queue
     */
    uint64_t GetQueue(void)
    {
        return queue_;
    }

    /*!
     * @brief Get queue
     */
    void SetP2pChan(void* p2p_chan);
    /*!
     * @breif Get type info
     */
    std::string GetTypeInfo(void)
    {
        return type_info_;
    }

    /*!
     * @breif Get name
     */
    std::string GetName(void)
    {
        return name_;
    }

    /*!
     * @breif Set task type id
     */
    void SetTaskTypeId(int task_type_id)
    {
        task_type_id_ = task_type_id;
        SetLinked(true);
    }

    /*!
     * @breif Get task type id
     */
    int GetTaskTypeId(void)
    {
        return task_type_id_;
    }

    /*!
     * @breif Set bridge
     */
    void SetBridge(void* p2p_chan)
    {
        p2p_chan_ = p2p_chan;
    }

    bool IsLinked(void)
    {
        return linked_flag_;
    }

    void SetLinked(bool flag)
    {
        linked_flag_ = flag;
    }

     /**
     * @brief 删除任务
     * @param task pointer to a task
     * @note deleter_func_必须不为空
     */
    void DelTask(void* task);

private:
    /**< This flag is true only if the pout is a graph output.
     * If a pout is linked to a pin, it will delete it's queue_ and uses the pin's queue_.
     * If a pout is a graph output, it will keep it's own queue.
     */
    bool exist_ = false;
    int queue_size_ = 0;
    Thread* pthread_ = nullptr;
    bool linked_flag_ = false;
    std::string name_;
    void* p2p_chan_ = nullptr; ///< if exist,  task send by p2p channel
    uint64_t queue_ = 0;  ///< if p2p_chan_ exist, queue_ is remote node pin  queue
    std::string type_info_;
    int task_type_id_ = 0;
    FTaskSer ser_func_ = nullptr;
    FTaskDeleter deleter_func_ = nullptr;
    friend class PIN;
    friend class Thread;
    friend class Graph;
	friend class Node;
};

using Resource_Handle = void*;

/**
 * @brief  通用资源管理类
 * 提供异步线程调度资源管理，举例：
 *  FaceDet类最多支持4个并发调度，FaceDet初始化得到void* face_det_handle[4];
 *  在FaceDet类中增加ASyncRes res对象，初始化res(4)设置handle资源
 *  在使用时候，调用res.get来获取handle资源（堵塞），使用完毕后调用res.put归还handle资源
 */
class DE_DLL ASyncRes {
public:
	ASyncRes(std::vector<Resource_Handle> handles) ;
	virtual ~ASyncRes() {};

	/**
	* @brief 等待（堵塞）资源归还
	* @note 用于资源回收释放处理
	*/
	void WaitResIdle();

	/**
	* @brief 获得资源句柄
	* @note 如果无资源，那么堵塞直到其他线程归还； 调用get和put线程不能为同一个线程, 否则容易出现死锁
	*/
	Resource_Handle Get();

	/**
	* @brief 归还资源句柄
	* @note 调用get和put线程不能为同一个线程, 否则容易出现死锁
	*/
	void Put(Resource_Handle handle);

protected:
	int handle_max_sz_ = 0;
	int batch_per_handle_;
	TaskQueue handle_queue_;
};

class POUT;     ///< class forward declaration

/**
 * @brief  通用异步多线程乱序管理类
 *  举例：线程队列收到A,B,C,D任务请求，线程配置/启动加速（nnp）A,B,C,D，但由于硬件异步调度原因，收到的结果顺序可能是
 * 乱序的， 比如收到B,C,A,D，而此时下一级线程要求顺序，因此需要乱序管理类进行重排。
 */
class ASyncDisorder {
public:
	/**
	 * @brief Task cache database for reordering output tasks
	 */
	struct Cache {
		void* rx_task = nullptr;
		void* tx_task = nullptr;
		int   task_id = 0;
		Cache(void* rx = nullptr, void* tx = nullptr, int id = 0) : rx_task(rx), tx_task(tx), task_id(id) {};

	};

	/**
	 * @brief
	 * @param _tx_q    发送队列
	 */
	ASyncDisorder(TaskQueue *_tx_q, int _task_type = 0) : tx_q_(_tx_q), task_type_(_task_type) {
		CHECK(_tx_q != nullptr) << " tx queue is nullptr!";
	};

	/**
	 * @brief Construct from a POUT pointer
	 * @param ppout pointer to a POUT ojbect
	 * @note ppout should be a pointer of member in a de::Thread object
	 */
	ASyncDisorder(POUT* ppout) : ppout_(ppout){}

	~ASyncDisorder() { cache_db_.clear(); };

	/**
	* @brief 启动异步调度, 记录tag
	* @param rx_task 接收任务指针
	*/
	void SetStartTag(void* rx_task);

	/**
	* @brief 收到对应tx task的结果处理
	* @param rx_task 接收任务指针
	* @param tx_task 发送任务指针
	* @param task_id 发送任务id
	*/
	void ReorderTxTask(void* rx_task, void* tx_task);


private:
	std::map<int64_t, Cache> cache_db_;
	std::mutex mutex_;
	TaskQueue *tx_q_ = nullptr;
	int64_t alloc_seq_ = 0;
	int64_t cont_done_seq_ = 0;
	int task_type_ = 0;

	POUT* ppout_{ nullptr };
};

/**
 * @brief   前一个Thread对象的信息
 */
struct ThreadPrevPtr {
	Thread* thread = nullptr;
	int que_idx = 0;  ///<  前一个thread对象的tx queue 索引
};

/**
 * @brief   后一个Thread对象的信息
 */
struct ThreadNextPtr {
	Thread* thread = nullptr;
	int type = 0;  ///< 后一个thread对象的rx queue任务type定义
};

/**
 * @brief Common operator base class in DeSdk
 */
class DE_DLL Thread
{
public:
    // input from pin,  output to pouts
    static const int kMaxPoutNum = 16;
    static const int kMaxUserPoutNum = kMaxPoutNum - 1;
    PIN pin{ this };
    POUT* pouts_[kMaxPoutNum];
    POUTBase* vir_pouts_[kMaxPoutNum];

    Thread();
    virtual ~Thread();
    virtual void Start(void);
    virtual void Stop(void) = 0;

    std::string name_ = "AnonymousThread";

    /**
     * @brief 设置thread name
     * @_name thread name
     */
    void SetName(const std::string& _name)
    {
        name_ = _name;
    }


    void SetTxQue(int que_sz, int idx = 0, int _mode = TaskQueue::MISO);

    virtual bool SetTxQue(TaskQueue* que,int idx = 0){
		if(idx < kMaxUserPoutNum) {
			pouts_[idx]->queue_ = reinterpret_cast<uint64_t>(que);
			return true;
		}
		return false;
	}

    void SetRxQue(int que_sz = 32);

    virtual void SetRxQue(TaskQueue* que) {pin.queue_ = reinterpret_cast<uint64_t>(que);}

    bool GetOutput(TaskQueue::Task& job, int que_idx_ = 0);

    void SetInput(void* task, int type_ = 0);

    /**
     * @brief 设置线程需要的资源数据库
     * @handles resource database of current thread
     */
    void SetResouces(std::vector<Resource_Handle> handles);

    /**
     * @brief 获取资源
     * @return resource handle of current thread
     */
    Resource_Handle GetResource();

    /**
     * @brief 释放资源
     * @res   resource handler to be released
     */
    void PutResource(Resource_Handle res);


    /**
     * @brief 设置线程支持乱序排序功能数据库
     * @pout_idx index used to indicate which pout needs to support task reordering
     */
    void SetDisorderDb(int pout_idx = 0);

    /**
    * @brief 发送乱序排序功能，启动异步调度, 记录tag
    * @param rx_task 接收任务指针
    * @param idx index of reorder_db
    */
    void SetStartTag(void* rx_task, int idx);

    /**
    * @brief 发送乱序排序功能， 收到对应tx task的结果处理
    * @param rx_task 接收任务指针
    * @param tx_task 发送任务指针
    * @param idx index of reorder_db
    */
    void ReorderTxTask(void* rx_task, void* tx_task, int idx);

    /**
    * @brief set pout task serialize and deserialize
    * @param pout_idx index of pout
    * @param task_ser_func function used to serialize task before sending task
    * @param task_deser_func function used to de-serialize task after receving task
    */
    void SetPoutTaskSer(int pout_idx, FTaskSer task_ser_func, FTaskDeser task_deser_func);

    /**
     * @brief set attr (key, value)
     * @param key attribute key
     * @param val attribute value
     * @return true if set attribute succeed, false otherwise
     */
    template<class T>
    bool Set(const std::string& key, const T& val)
    {
        return attr_.Set(key, val);
    }

    /**
     * @brief update parameters
     */
    virtual void UpdatePara();

    /**
     * @brief Set database
     */
    void SetDatabase(void* pdb){
        pdb_ = pdb;
    }

    /**
     * @brief 获取处理线程是否active状态
     * @return 处理线程active状态
     */
    bool GetThreadActive(void) { return thread_active_; }

    /**
     * @brief 返回de_state_的值
     */
    int32_t GetDeState(void);

    Prop attr_;

    /**
     * @brief 获取cpu总使用时间
     * @return cpu总使用时间
     */
    static int64_t GetTotalCpuOccupy(void);

    /**
     * @brief 获取指定线程的cpu使用总时间
     * @param tid 线程标识符
     * @return 指定线程的cpu使用总时间
     */
    static int64_t GetThreadCpuOccupy(int tid);

    /**
     * @brief 获取指定线程的内存使用量
     * @param tid 线程标识符
     * @return 指定线程的内存使用量，单位: kB
     */
    static int GetThreadMemOccupy(int tid);

    /**
     * @brief Pout转换输出
     * @param rx_task 原始输入任务
     * @param task_type 输入任务类型
     * @note Thread类的派生类可以重写该函数，以定义输出任务的转换操作
     */
    void PoutConvert(void *rx_task, int32_t task_type, POUTBase** pout_base){}

    /**
     * @brief 根据Pout type选择正确的POUTBase指针
     * @param pout_type pout类型，DEFAULT_POUT或者USER_POUT
     * @return 正确的POUTBase指针
     */ 
    POUTBase** SelectPout(POutType pout_type);

protected:
    ASyncRes* resource_ = nullptr; // whether thread run with resource limit
    ASyncDisorder* reorder_db_[kMaxPoutNum] { nullptr, }; // async scheduled, need intermediated cache storage
    ThreadPrevPtr* prev_ = nullptr;
    ThreadNextPtr* next_[kMaxPoutNum] = { nullptr, };

    int dbg_pin_grab_ = 0;  ///< whether grab input task to file
    std::string dbg_pin_grab_file_;
    int dbg_pin_grab_max_size_ = 4; ///< unit: MB

    int profile_enable_ = 0;
    int profile_stat_period_ = 0;
    int debug_level_ = 0;  //LOG打印等级

    int64_t frame_no = 0;       //frame number
    int proc_average_time = 0;  //处理一条任务平均所需时间
    int proc_max_time = 0;      //处理一条任务最长所需时间

    void* pdb_ {nullptr};       // pointer into a GraphDB

    /**
     * @brief  用户线程的任务处理函数
     * @param  rx_task 接收任务指针
     * @param  task_type 接收任务type
     */
    virtual void Proc(void* rx_task, int32_t task_type, POutType pout_type = DEFAULT_POUT) = 0;

    virtual void SetOutput(void* task, int idx = 0);

    /**
     * @brief  通用的线程调度函数
     */
    static void ThreadProc(Thread* self);

    /**
     * @brief Write database
     * @param str data to be written into database
     */
    void WriteDatabase(const std::string& str);

    /**
     * @brief Read database
     * @return content in database
     */
    std::string ReadDatabase(void);

    /**
     * @brief 设置处理线程是否active
     * @param active 处理线程是否active, true or false
     */
    void SetThreadActive(bool active) { thread_active_ = active; }

    DeNodeState de_state_ { DE_NODE_INIT };
    
    friend class ASyncDisorder;

    //虚拟pout任务转换函数
    std::function<void(void*, int, void*, POUTBase**)> virpout_task_convert_func_[kMaxUserPoutNum] {nullptr, };

    //虚拟pout任务统一处理
    void VirPoutTaskProc(void* rx_task, int rx_task_type, POUTBase** pout_base);

private:
    int tx_task_num_{ 0 };
    int rx_task_num_{ 0 };
    //max size of pin.queue
    int max_rx_queue_size_{ 0 };

    //the following two variable are used to calcutate average size of pin.queue
    int rx_queue_stat_size_{ 0 };
    int rx_queue_stat_cnt_{ 0 };

    bool thread_active_ { false };

    PackedFunc filter_cb_func_;

    friend class PIN;
    friend class POUT;
};  //end of class Thread

DE_DLL void set_thread_name_by_handle(std::thread *thread, const char* threadName);
DE_DLL void set_thread_name_by_prctl(const char* threadName);
DE_DLL void get_thread_name_by_prctl(char* threadName);

}//end of namespace de

#endif




