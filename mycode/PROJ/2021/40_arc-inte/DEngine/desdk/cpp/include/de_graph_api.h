/**
 * @file de_graph.h
 * @brief Class Graph interface
 * @author DeSdk Team
 * @version v1.0.0
 * @date 2020/03/13
 * @copyright Intellif
 */

#pragma once
#include <string>
#include <map>
#include <vector>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <core/de_c_type.h>
#include <core/de_thread.h>

namespace de
{
class PIN;
class Node;
class Bridge;
class GraphDBWrapper;
class JSONReader;
class JSONWriter;
class Prop;

class GraphDB
{
public:
    /**
     * @brief Read all data bytestream (serialize) and clear database
     *
     * @return read out data
     */
	std::string ReadAndClear(void);

	/**
	 * @brief Write into database
	 * @param 
	 */
	void Write(const std::string& str);

    /**
     * @brief Set Max Size
     *
     * @param size
     */
    void SetMaxSize(int size);

    /**
     * @brief Close database, disable write
     */
    void Close(void);
protected:
    std::mutex mutex_;
	std::condition_variable cv_;
	int max_size_{ 0 };
	std::string str_db_data_;
    bool exit_flag_ { false };
};

//如果定义为untion，析构函数隐式定义为“已删除”
struct NodeAttrValue {
	int ivalue;
	double dvalue;
	std::string strvalue;
};

struct NodeAttrInfo {
	std::string attr_name;
	std::string attr_type;	//"int", "double" or "string"
	NodeAttrValue attr_value;

	void Save(de::JSONWriter* write) const;
    void Load(de::JSONReader* read);
};
using GraphAttrInfo = NodeAttrInfo;

struct BridgeInfo {
    int dev_id;
    std::string url;
    int port;
    bool profile_enable;
    int profile_duration;

    void Save(de::JSONWriter* write) const;
    void Load(de::JSONReader* read);
};

struct ChanInfo {
	std::string direct;	//"H2D" or "D2H"
	int ch_id;
	std::string ch_name;
	std::map<std::string, int> attr;

	void Save(de::JSONWriter* write) const;
    void Load(de::JSONReader* read);
};

struct LinkNodeInfo {
	int pout_idx;
	std::string dst_node;
	int bridge_ch;

	void Save(de::JSONWriter* write) const;
    void Load(de::JSONReader* read);
};

struct InputNodeInfo {
	int pin_idx;
	std::string node_name;

	void Save(de::JSONWriter* write) const;
    void Load(de::JSONReader* read);
};

struct OutputNodeInfo {
	int pout_idx;
	std::string node_name;
	int node_pout_idx;

	void Save(de::JSONWriter* write) const;
    void Load(de::JSONReader* read);
};


struct NodeInfo {
	std::string location;	//HOST or DEV
	std::string class_name;
	std::string node_name;

	std::vector<NodeAttrInfo> node_attr;
	std::vector<LinkNodeInfo> link_info;

	void Save(de::JSONWriter* write) const;
    void Load(de::JSONReader* read);
};

struct DE_DLL GraphInfo {
    GraphInfo(void) = default;
    GraphInfo(const std::string& cfg_file);

    std::string graph_name;

    std::vector<BridgeInfo> bridges;
    std::vector<ChanInfo> chans;
    std::vector<std::string> host_node_libs;
    std::vector<std::string> dev_node_libs;
    std::vector<NodeInfo> nodes;
    std::vector<InputNodeInfo> inputs;
    std::vector<OutputNodeInfo> outputs;
    std::vector<GraphAttrInfo> graph_attrs;

    void FromFile(const std::string& file_name);
    void FromString(const std::string& str_config);
    void ToFile(const std::string& file_name);

    void GetNodeAttr(const std::string& node_name,    NodeAttrInfo* p_attr_out);
    void SetNodeAttr(const std::string& node_name,    const NodeAttrInfo& attr);
    
    void Save(de::JSONWriter* write) const;
    void Load(de::JSONReader* read);

    void AddBridge(int dev_id, const std::string& url, int port);
    void SetBridgeProfile(bool enable, int duration);
    void AddChan(const std::string& direct, int chan_id, const std::string& name, const std::map<std::string, int>& attr);
    void AddHostNodeLib(const std::string& lib_path);
    void AddDevNodeLib(const std::string& lib_path);
    void AddNode(const std::string& location, const std::string& class_name, const std::string& node_name);
    void AddNodeAttr(const std::string& node_name, const std::string& attr_name, int val);
    void AddNodeAttr(const std::string& node_name, const std::string& attr_name, double val);
    void AddNodeAttr(const std::string& node_name, const std::string& attr_name, const std::string& val);
    void AddLinkNode(const std::string& src_node, int pout_idx, const std::string& dst_node, int bridge_ch);
    void AddInput(int idx, const std::string& node_name);
    void AddOutput(int idx, const std::string& node_name, int node_pout_idx);
    void AddGraphAttr(const std::string& attr_name, int val);
    void AddGraphAttr(const std::string& attr_name, double val);
    void AddGraphAttr(const std::string& attr_name, const std::string& val);
};



/**
 * @brief Class used to manage operator nodes on host/device
 */
class DE_DLL Graph
{
public:

    /*!
     * @brief Create empty graph
     */
    Graph(const std::string& name);
    Graph(const GraphInfo& cfg_info);
    ~Graph();

    /**
     * @brief move constructor
     */
    Graph(Graph&& rr) = default;

    /*!
     * @brief Create graph from config file (json format)
     */
    static Graph* FromFile(const std::string& config_file) ;

    /*!
     * @brief 从配置字符串创建Graph。字符串中是json格式的配置内容
     * @param config_str 以json格式存储的Graph配置
     * @return 在堆中创建的Graph对象指针 
     * @note 应在程序结束时delete掉返回的指针
     */
    static Graph* FromString(const std::string& config_str) ;

    /*!
     * @brief  write graph to config file (json format)
     */
    void ToFile(const std::string& config_file) ;

    /*!
     * @brief  Create socket bridge between host and device, bridge supports multiple channels providing communication.
     * @param  dev_id the index of device which bridge link to
     * @param  p2p_url p2p server ip address
     * @param  p2p_port p2p server port
     * @note  The function worked only  if macro USE_HOST  defined
     */
    void CreateBridge(int dev_id, const std::string& p2p_url = "", int p2p_port = -1);

    /*!
     * @brief  Create hsot to device bridge channel
     * @param  ch_id logic channel id, must be unique in graph
     * @param  ch_name chan name identify one physical channel
     * @param  attr channel config attributes
     * @note   one physical channel can bear multi logic channel,  The function worked only  if macro USE_HOST  defined
     */
    void AddBridgeH2DChan(int ch_id, const std::string& ch_name,  const std::map<std::string, int>& attr = std::map<std::string, int>()) ;

    /*!
     * @brief  Create device to host bridge channel
     * @param  ch_id logic channel id, must be unique in graph
     * @param  ch_name chan name identify one physical channel
     * @param  attr channel config attributes
     * @note   one physical channel can bear multi logic channel, The function worked only  if macro USE_HOST  defined
     */
    void AddBridgeD2HChan(int ch_id, const std::string& ch_name,  const std::map<std::string, int>& attr = std::map<std::string, int>());

    /**
     * @brief 加载Host端的动态库以创建Host端的Node
     * @param node_lib_path 包含node注册信息的动态库
     */
    void AddHostNodeLib(const std::string& node_lib_path);

    /**
     * @brief 加载Device端的动态库以创建Device端的Node
     * @param node_lib_path 包含node注册信息的动态库
     */
    void AddDevNodeLib(const std::string& node_lib_path);

    /*!
     * @brief  Create Host Node
     * @param  class_name   class name which already registered
     * @param  node_name    node name which already registered
     */
    void CreateHostNode(const std::string& class_name, const std::string& node_name) ;

    /*!
     * @brief  Create Dev Node
     * @param  class_name   class name which already registered
     * @param  node_name    node name which already registered
     */
    void CreateDevNode(const std::string& class_name, const std::string& node_name) ;

    /*!
     * @brief  link source node and destination node,  nodes in different sides(host/device) can been linked with bridge channel
     * @param  src_node node name which already registered
     * @param  pout_idx the idx of source node' pout
     * @param  dst_node node name which already registered
     * @param  bridge_ch logic channel which used to link source node and destination node
     */
    void LinkNode(const std::string& src_node, int pout_idx, const std::string& dst_node, int bridge_ch = -1);

    /*!
     * @brief Set graph input Node
     * @param graph_pin_idx the index of graph pin
     * @param node_name node name which already registered
     */
    void SetInputNode(int graph_pin_idx, const std::string& node_name);

    /*!
     * @brief  Set graph input task
     * @param  idx the index of graph pin
     * @param  task the input task
     * @param  task_type_id the task id which already set in graph pin
     */
    void SetInput(int idx, void* task, int task_type_id = 0);

    /*!
     * @brief Set graph output Node
     * @param graph_pout_idx the idx of graph pout
     * @param node_name node name which already registered
     * @param node_pout_idx the index of node which use as graph' pout
     */
    void SetOutputNode(int graph_pout_idx, const std::string& node_name, int node_pout_idx);

    /*!
     * @brief  get graph output task
     * @param  idx the idx of graph' pout
     */
    void* GetOutput(int idx);

    /**
     * @brief 停止获取输出任务
     * @param idx Graph输出索引
     */
    template<class T>
    void StopOutput(int idx) {
        if (nullptr != graph_pout_[idx]) {
            static_cast<POUT*>(graph_pout_[idx])->DestroyQueue<T>();
        }
    }

    /**
     * @brief 设置输入端口的状态
     * @param pin_idx Graph输入索引
     * @param state 0-unlink, 1-link/run, 2-finish
     */
    void SetInputState(int32_t pin_idx, int32_t state);

    /**
     * @brief 设置输出端口的状态
     * @param pout_idx Graph输出索引
     * @param state 0-unlink, 1-link/run, 2-finish
     */
    void SetOutputState(int32_t pout_idx, int32_t state);

    /*!
     * @brief Start all nodes
     */
    void Start(void);

    /*!
     * @brief Set attribute for node
     * @param node_name name for which node to set attribute
     * @param attr_name attribute name
     * @param val attribute value
     */
    void SetNodeAttr(const std::string& node_name, const std::string& attr_name, const int& val);

    /*!
     * @brief Set attribute for node
     * @param node_name name for which node to set attribute
     * @param attr_name attribute name
     * @param val attribute value
     */
    void SetNodeAttr(const std::string& node_name, const std::string& attr_name, const double& val);
    
    /*!
     * @brief Set attribute for node
     * @param node_name name for which node to set attribute
     * @param attr_name attribute name
     * @param val attribute value
     */
    void SetNodeAttr(const std::string& node_name, const std::string& attr_name, const std::string& val);

    /**
     * @brief 获取Node的所有属性，放到一个Prop对象中
     * @param node_name Node名字
     * @param[out] prop 用于存储node的所有属性
     */
    void GetNodeAttrList(const std::string& node_name, Prop& prop);

    /*!
     * @brief Add sub graph
     * @param vec_sub_graph vector of sub graph
     * @note nodes of sub graph in the vector will be removed
     */
    void AddSubGraph(const std::vector<std::pair<std::string, Graph*>>& vec_sub_graph);

    /*!
     * @brief Link two sub graph
     * @param out_graph_name output sub graph name
     * @param pout_idx pout index of output sub graph
     * @param in_graph_name input sub graph name
     * @param pin_idx pin index of input sub graph
     */
    void LinkGraph(const std::string& out_graph_name, int pout_idx, const std::string& in_graph_name, int pin_idx);


    /*!
     * @brief Link node to a graph
     * @param node_name name of the node
     * @param pout_idx pout index of a node
     * @param graph_name name of a sub graph
     * @param pin_idx pin index of the sub graph
     */
    void LinkNodeToGraph(const std::string& node_name, int pout_idx, const std::string& graph_name, int pin_idx);

    /*!
     * @brief Link node from a graph
     * @param graph_name name of a sub graph
     * @param pout_idx pout index of a graph
     * @param node name name of a node
     */
    void LinkNodeFromGraph(const std::string& graph_name, int pout_idx, const std::string& node_name);

    /*!
     * @brief Set input node from sub graph
     * @param pin_idx pin index
     * @param graph_name name of sub graph
     * @param graph_pin_idx input index in sub graph
     */
    void SetInputGraph(int pin_idx, const std::string& graph_name, int graph_pin_idx);

    /*!
     * @brief Set input node from sub graph
     * @param pin_idx pin index
     * @param graph_name name of sub graph
     * @param graph_pin_idx input index in sub graph
     */
    void SetOutputGraph(int pin_idx, const std::string& graph_name, int graph_pin_idx);

    /**
     * @brief 
     *
     * @param db_name
     */
    void CreateHostDB(const std::string& db_class_name, const std::string& db_name, int max_size) ;

    /**
     * @brief 
     *
     * @param db_name
     */
    void CreateDevDB(const std::string& db_class_name, const std::string& db_name, int max_size);

    /**
     * @brief 
     *
     * @param node_name
     * @param db_name
     * @note Node and Database must be in same place 
     */
    void AssignNodeDB(const std::string& node_name, const std::string& db_name);

    /**
     * @brief 
     *
     * @param db_name
     *
     * @return
     * @note get device database content(by rpc) from host
     */
    std::string GetGraphDBStream(const std::string& db_name);

    /**
     * @brief 
     *
     * @param db_name
     *
     * @return 
     * @note Only local pointer is valid, otherwise return nullptr
     */
    GraphDB* GetGraphDBPtr(const std::string& db_name);

    /**
     * @brief 查询Node的状态
     * @param node_name 要查询的node的名字
     * @return Node状态
     */
    DeNodeState GetNodeSt(const std::string& node_name);

    /**
     * @brief 输出Graph
     * @param os 输出流对象
     * @param g Graph对象
     * @return 输出流对象
     */
    DE_DLL friend std::ostream& operator<<(std::ostream& os, const Graph& g);

    /**
     * @brief 设置bridge profile参数
     * @param enable profile使能
     * @param duration profile时间间隔，单位为毫秒
     */
    void SetBridgeProfile(bool enable, int duration);

    /*!
     * @brief 设置Graph自身的属性
     * @param attr_name 属性名
     * @param val 属性值
     */
    template<typename T>
    void SetGraphAttr(const std::string& attr_name, const T& val) {
        attr_.Set(attr_name, val);
        save_info_.AddGraphAttr(attr_name, val);
    }

    /*!
     * @brief 更新Graph属性
     */
    void UpdateGraphAttr(void);

private:
    static constexpr int kGraphMaxPin = 8;
    void* graph_pin_[kGraphMaxPin] = {nullptr, };
    void* graph_pout_[kGraphMaxPin] = { nullptr, };

    ///< Graph输入端口状态。类中不能有atomic<int>成员，否则类的move constructor会被删除
    std::atomic<int32_t>* pin_state_ = nullptr;
    ///< Graph输出端口状态
    std::atomic<int32_t>* pout_state_ = nullptr;

    Bridge* bridge_ = nullptr;

	struct NodeMap {
		std::string name;
		Node* pnode;
	};
    std::vector<NodeMap> nodes_;

    struct NodeLibHandle {
        int dev_id;
        void* handle;
    };
    std::vector<NodeLibHandle> node_lib_handle_;

    std::map<std::string, Graph*> sub_graphs_;

    std::map<std::string, GraphDBWrapper*> dbs_;

    std::string name_;
    
    //Grpah的组织信息格式化为JSON字符串
    GraphInfo save_info_;

    //Graph本身的属性
    Prop attr_;

    /**
     * @delete default copy constructor
     */
    Graph(const Graph&) = delete;

    /**
     * @delete default copy assignment
     */
    Graph& operator=(const Graph&) = delete;

    /**
     * @brief 描述node和graph之间的依赖关系
     */
    struct Depdends
    {
        std::map<Node*, Node*> nodes_;
        std::map<Node*, Graph*> nodes_graphs_;
        std::map<Graph*, Node*> graphs_nodes_;
        std::map<Graph*, Graph*> graphs_;

        /**
         * @brief 检查Node是否和其他node以及graph有依赖
         *
         * @param name node's name
         *
         * @return
         */
        bool NodeDepends(const std::string& name) ;


        /**
         * @brief 删除Node
         *
         * @param name node's name
         *
         * @return
         */
        bool NodeDelete(const std::string& name);


        /**
         * @brief 检查Graph是否和其他Node以及Graph有依赖
         *
         * @param name graph's name
         *
         * @return
         */
        bool GraphDepends(const std::string& name);


        /**
         * @brief 删除Graph
         *
         * @param name
         *
         * @return
         */
        bool GraphDelete(const std::string& name);

    };

    /*!
     * @brief  Get Node by name
     * @param name node name
     * @return pointer to a node
     */
    Node* GetNode(const std::string& name);

    /*!
     * @brief  通过类名获取Node
     * @param class_name Node类名
     * @return 指向node的指针
     */
    Node* GetNodeByClass(const std::string& class_name);

	/*!
	 * @brief  Get GraphDBWrapper by name
	 * @param name db name
	 * @return pointer to a dbwrapper
	 */
	GraphDBWrapper* GetDBWrapper(const std::string& name);

    /*!
     * @brief  Get sub graph by name
     * @param name name of sub graph
     * @return pointer to a sub graph
     */
    Graph* GetSubGraph(const std::string& name);

    /*!
     * @brief  Link POUT and PIN
     * @param ppout pointer to a POUT
     * @param ppin pointer to a PIN
     * @note ppout must be an output node of a graph, and
     */
    Graph* LinkPout2Pin(POUT* ppout, PIN* ppin);

    /*!
     * @brief  Clear pin and pout info
     */
    void ClearInOut(void);

};	//end of class Graph

}		//end of namespace de
