#include <fstream>
#include <random>
#include "de_graph_api.h"
#include "de_node.h"
#include <de_video_in_node.h>
#include <de_video_decoder_node.h>
#include "de_aiengine_node.h"
#include "de_pic_decoder_node.h"
#include "de_pic_encoder_node.h"
#include "de_node_common.h"

#include <stdio.h>
#include <math.h>
#include <time.h>
#include<iostream>
#include<random>
#include<iomanip>

using namespace de;

namespace de{

namespace benchmark{

typedef struct {
  NDArray  array;
  int    streamid;
  uint64_t pts;
  std::vector<deRect> rects;
}DetectTask;

typedef struct {
	NDArray  array; 	///< data object
	int 	 id;	///< id
	uint64_t pts;		///< timestamp
  int end_flag = 1; ///< last packet flag

    inline de::NDArray Serialize() {
        int32_t *ptr = static_cast<int32_t*>(array.GetBearData());
        ptr[0] = id;
        ptr[1] = pts & 0xFFFFFFFF;
        ptr[2] = (pts >> 32) & 0xFFFFFFFF;
        ptr[3] = end_flag;
        std::vector<int64_t> shapes;
        array.Shrink(shapes, 4*sizeof(int32_t));
        return array;
    }

    inline void DeSerialize(de::NDArray data) {
        int32_t *ptr = static_cast<int32_t*>(data.GetBearData());
        id = ptr[0];
        pts = (((int64_t)ptr[2])<<32) | ((int64_t)ptr[1] & 0xFFFFFFFF);
        end_flag = ptr[3];
        array = data;
    }
    
}SenderTask;
}

namespace serializer {
STRUCT_SERIALIZE_4(benchmark::DetectTask,
  NDArray, array,
  int, streamid,
  uint64_t, pts,
  std::vector<deRect>, rects);
  
STRUCT_SERIALIZE_4(benchmark::SenderTask,
	NDArray, array,
	int, id,
	uint64_t, pts,
  int, end_flag);
};

namespace benchmark{

class DE_DLL AiEngineTest: public AiEngine{
  public:
    AiEngineTest(bool stream_mode=true);
    void UpdatePara(void);
    void Stop(void) override;
    ~AiEngineTest();
};


static std::vector<de::NDArray> TaskPreProc(void *rx_task, int32_t task_type, void *self){

	de::NDArray nd;
	std::vector<de::NDArray> v_nd;
	
	if (task_type==0){
		auto task = reinterpret_cast<de::MediaTask*>(rx_task);
		nd = task->array;
		v_nd.emplace_back(nd);
	}else if (task_type==1){
		auto task = reinterpret_cast<de::MediaTask*>(rx_task);
		nd = task->array;
		v_nd.emplace_back(nd);
	}else if (task_type==2){
		auto task = reinterpret_cast<DetectTask*>(rx_task);
		nd = task->array;
		v_nd.emplace_back(nd);
	}
  else if (task_type==3){
		auto task = reinterpret_cast<de::NNTask*>(rx_task);
		v_nd = task->tensors;
	}
	else{
		CHECK(0) << "TaskPreProc: unknown taskType.";
	}

	return v_nd;
}

static void * TaskPostProc(void *rx_task, int32_t task_type, std::vector<de::NDArray> outs, void *self){

   AiEngineTest* pThis = (AiEngineTest*)self;
   if (task_type==0){
      if (pThis->pouts_[0]->IsLinked()){
      auto tx_task = new DetectTask();
      auto task = reinterpret_cast<de::MediaTask*>(rx_task);
      tx_task->streamid = task->streamid;
      tx_task->pts = task->pts;
      tx_task->array = task->array;
      return (void *)tx_task;
      }
      if (pThis->pouts_[1]->IsLinked()){
      auto tx_task = new de::NNTask();
      auto task = reinterpret_cast<de::MediaTask*>(rx_task);
      // printf("**** AIEngineTest recv jpegdectask id %d\n",task->streamid);
      tx_task->id = task->streamid;
      tx_task->tensors = outs;
      return (void *)tx_task;
      }
   }else if (task_type==1){
      auto tx_task = new DetectTask();
      auto task = reinterpret_cast<de::MediaTask*>(rx_task);
      tx_task->streamid = task->streamid;
      tx_task->pts = task->pts;
      tx_task->array = task->array;
      return (void *)tx_task;
   }else if (task_type==2){
      auto tx_task = new DetectTask();
      auto task = reinterpret_cast<DetectTask*>(rx_task);
      tx_task->streamid = task->streamid;
      tx_task->pts = task->pts;
      tx_task->rects = task->rects; // outs contains rect info
      tx_task->array = task->array;
      return (void *)tx_task;
   }else if (task_type==3){
      auto tx_task = new de::NNTask();
      auto task = reinterpret_cast<de::NNTask*>(rx_task);
      tx_task->id = task->id;
      tx_task->tensors = outs;

      // std::string out = "out.bin";
      // FILE *file = fopen(out.c_str(), "w");
      // fwrite((char*)outs[0].GetTensorData(), outs[0].GetTensorDataSize(), 1, file);
      // fclose(file);

      return (void *)tx_task;
   }
   else{
      CHECK(0) << "AiEngineTest: TaskPostProc task_type error " << task_type;
      return nullptr;
   }
}

AiEngineTest::AiEngineTest(bool stream_mode): AiEngine(stream_mode){

   if (stream_mode_){
      pin.SetTypeInfo(0, "de::MediaTask", de::TaskDeSerializeCustom<de::MediaTask>, de::TaskDeleter<de::MediaTask>);
      pin.SetTypeInfo(1, "de::MediaTask", de::TaskDeSerializeCustom<de::MediaTask>, de::TaskDeleter<de::MediaTask>);
      pin.SetTypeInfo(2, "de::benchmark::DetectTask", de::TaskDeSerialize<de::benchmark::DetectTask>, de::TaskDeleter<de::benchmark::DetectTask>);
      pin.SetTypeInfo(3, "de::NNTask", de::TaskDeSerialize<de::NNTask>, de::TaskDeleter<de::NNTask>);
      pouts_[0]->SetTypeInfo("de::benchmark::DetectTask", de::TaskSerialize<de::benchmark::DetectTask>);
      pouts_[1]->SetTypeInfo("de::NNTask", de::TaskSerialize<de::NNTask>);

   }

	/* set preproc & postprc */
	SetPreProc(TaskPreProc);
  SetPostProc(0, TaskPostProc);
	SetPostProc(1, TaskPostProc);
}

void AiEngineTest::Stop(void){
    pin.DestroyQueue();
}

void AiEngineTest::UpdatePara(void){   
   AiEngine::UpdatePara();
}

AiEngineTest::~AiEngineTest(){

}

DE_CLASS_REGISTER("de::benchmark::AiEngineTest", AiEngineTest);

static std::chrono::time_point<std::chrono::steady_clock> g_base_ts = std::chrono::steady_clock::now();

//for benchmark zero copy
class DevReceiver : de::Thread {
  public:
  DevReceiver(){
    pin.SetTypeInfo(0, "de::benchmark::SenderTask", de::TaskDeSerializeCustom<SenderTask>, de::TaskDeleter<SenderTask>);
    pouts_[0]->SetTypeInfo("de::MediaTask", de::TaskSerializeCustom<de::MediaTask>);
    pouts_[1]->SetTypeInfo("de::MediaTask", de::TaskSerializeCustom<de::MediaTask>);
    pouts_[2]->SetTypeInfo("de::NNTask", de::TaskSerialize<de::NNTask>);
    attr_.SetDefault("chip_test_mode", 0, 0, 1);
    attr_.SetDefault("predict_num", 100);
    attr_.SetDefault("vector_btnum", 1, 1, 32);
    attr_.SetDefault("interval_type", 0, 0, 5);
    attr_.SetDefault("interval_time", 0, 0, 5000);
    SetName("DevReceiver");
  }
  ~DevReceiver(){
    pin.DestroyQueue();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  void Proc(void* rx_task, int32_t task_type, POutType pout_type) {
    auto task = reinterpret_cast<SenderTask*>(rx_task);
    
    int chip_test_mode = 0;
    int predict_num = 0;
    int vector_btnum = 1;
    int interval_type = 0;
    int interval_time = 0;
    attr_.Get("chip_test_mode", chip_test_mode);
    attr_.Get("predict_num", predict_num);
    attr_.Get("vector_btnum", vector_btnum);
    attr_.Get("interval_type", interval_type);
    attr_.Get("interval_time", interval_time);
	
	

    // printf("DevReceiver[mode %d num %d] recv id %d task->end_flag %d vector_btnum %d\n",chip_test_mode, predict_num,task->id, task->end_flag, vector_btnum);
    if (chip_test_mode){
      if (pouts_[0]->IsLinked()){
        for (int i = 0; i < predict_num; i++) {
          de::MediaTask* tx_task = new de::MediaTask();
          tx_task->array = task->array;
          //复用id做时间戳
          tx_task->streamid = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - g_base_ts).count();
          pouts_[0]->SendTask(tx_task);
        }  
      } 
      else if (pouts_[2]->IsLinked()){
        if (last_id_ != task->id) {
          nn_task_ = new de::NNTask();  
        }
        //复用id做时间戳
        nn_task_->id = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - g_base_ts).count();
        nn_task_->tensors.emplace_back(task->array);
        if (task->end_flag) {
		  std::minstd_rand engine(time(NULL));
          std::poisson_distribution<int> distribution(interval_time);
          for (int i = 0; i < predict_num; i++) {  
            de::NNTask* nn_task = new de::NNTask();
            nn_task->id = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - g_base_ts).count();
            for (int j = 0; j < vector_btnum; j++) {
              nn_task->tensors.insert(nn_task->tensors.end(),nn_task_->tensors.begin(),nn_task_->tensors.end());
            }
            pouts_[2]->SendTask(nn_task);
			if (interval_type == 1){
				std::this_thread::sleep_for(std::chrono::milliseconds(interval_time));
			}
			if (interval_type == 2){
				// printf("possion time calc\n");
				int time= distribution(engine);
				// printf("possion result %d\n",time);
				std::this_thread::sleep_for(std::chrono::milliseconds(time));
			}
            // printf("DevReceiver has sent idx %d tensor0size %d\n", i, nn_task->tensors[0].GetTensorDataSize());
          }
          // printf("DevReceiver has sent %d nntask\n", predict_num);
        }
      }
    }else{
      if (pouts_[0]->IsLinked()){
        de::MediaTask* tx_task = new de::MediaTask();
        tx_task->array = task->array;
        tx_task->streamid = task->id;
        tx_task->pts = task->pts;
        pouts_[0]->SendTask(tx_task);
          
      } else if (pouts_[1]->IsLinked()){
        de::MediaTask* tx_task = new de::MediaTask();
        tx_task->array = task->array;
        tx_task->streamid = task->id;
        tx_task->pts = task->pts;
        pouts_[1]->SendTask(tx_task);
      } else if (pouts_[2]->IsLinked()){
        if (last_id_ != task->id) {
          nn_task_ = new de::NNTask();  
        }
        nn_task_->id = task->id;
        nn_task_->tensors.emplace_back(task->array);
        if (task->end_flag) {
          // printf("DevReceiver send nntask id %d. size=%d task0size %d\n", task->id, nn_task_->tensors.size().nn_task_->tensors[0].GetTensorDataSize());
          pouts_[2]->SendTask(nn_task_);
        }
        last_id_ = task->id;
      }
    }
    pin.DelTask(rx_task, task_type);
  }
  void Stop(void){pin.DestroyQueue();}
  private:
  de::NNTask* nn_task_ = nullptr;
  int last_id_ = -1;
};

DE_CLASS_REGISTER("de::benchmark::DevReceiver", DevReceiver);

class DevSender : de::Thread {
  public:
  DevSender() {
    pin.SetTypeInfo(0, "de::MediaTask", de::TaskDeSerializeCustom<de::MediaTask>, de::TaskDeleter<de::MediaTask>);
    pin.SetTypeInfo(1, "de::MediaTask", de::TaskDeSerializeCustom<de::MediaTask>, de::TaskDeleter<de::MediaTask>);
    pin.SetTypeInfo(2, "de::NNTask", de::TaskDeSerialize<de::NNTask>, de::TaskDeleter<de::NNTask>);
    pouts_[0]->SetTypeInfo("de::benchmark::SenderTask", de::TaskSerializeCustom<SenderTask>);
    attr_.SetDefault("report_flag", 0, 0, 1);
    attr_.SetDefault("chip_test_mode", 0, 0, 1);
    attr_.SetDefault("predict_num", 100);
    attr_.SetDefault("vector_btnum", 1, 1, 32);
    SetName("DevSender");
  }
  ~DevSender() {
    pin.DestroyQueue();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  void Stop(void){pin.DestroyQueue();}
  void Proc(void* rx_task, int32_t task_type, POutType pout_type) {
    // LOG(INFO) << "***** enter DevSender proc";
    int report_flag = 0;
    attr_.Get("report_flag", report_flag);
    int chip_test_mode = 0;
    int predict_num = 0;
    int vector_btnum = 1;
    attr_.Get("chip_test_mode", chip_test_mode);
    attr_.Get("predict_num", predict_num);
    attr_.Get("vector_btnum", vector_btnum);

    // printf("DevSender[mode %d num %d cur_predict_num_ %d] \n",chip_test_mode, predict_num, cur_predict_num_);
    if (chip_test_mode) {
       if (task_type == 2) {
        auto task = reinterpret_cast<de::NNTask*>(rx_task);
        if (cur_predict_num_ == 0) {
          first_pkt_pts_ = task->id;
        }
        int64_t cur_pkt_pts = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - g_base_ts).count();
        total_delay_ +=  cur_pkt_pts - task->id;
        cur_predict_num_++;

        if (cur_predict_num_ >= predict_num){
          SenderTask* tx_task = new SenderTask();
          tx_task->array = NDArray::Empty(8);
          float* p = (float*)tx_task->array.GetTensorData();
          *p = (float)1000*predict_num*vector_btnum/(cur_pkt_pts - first_pkt_pts_);
          *(p+1) = (float)total_delay_/predict_num;
          tx_task->id = 12345;
          tx_task->pts = 54321;
          pouts_[0]->SendTask(tx_task);
        }
      }
    }
    else{
      if (task_type == 0) {
        SenderTask* tx_task = new SenderTask();
        auto task = reinterpret_cast<de::MediaTask*>(rx_task);
        tx_task->array = task->array;
        tx_task->id = task->streamid;
        tx_task->pts = task->pts;
        pouts_[0]->SendTask(tx_task);
      } else if (task_type == 1) {
        SenderTask* tx_task = new SenderTask();
        auto task = reinterpret_cast<de::MediaTask*>(rx_task);
        tx_task->array = task->array;
        tx_task->id = task->streamid;
        tx_task->pts = task->pts;
        pouts_[0]->SendTask(tx_task);
      } else if (task_type == 2) {
        auto task = reinterpret_cast<de::NNTask*>(rx_task);
        // printf( "***** send tensors size %d id = %d\n", task->tensors.size(), task->id);
        for (int i = 0; i < task->tensors.size(); i++) {
          SenderTask* tx_task = new SenderTask();
          if(report_flag){
            tx_task->array = task->tensors[i].Clone();
            tx_task->array.CopyFromBytes(task->tensors[i].GetTensorData(),task->tensors[i].GetTensorDataSize());
          }
          else
          {
            tx_task->array = NDArray::Empty(8);
          }
          tx_task->id = task->id;
          if (i == task->tensors.size() -1) 
            tx_task->end_flag = 1;
          else
            tx_task->end_flag = 0;
          // LOG(INFO) << "***** send nd len = " << tx_task->array.GetTensorDataSize();
          // printf("***** send nd len = %d\n" ,tx_task->array.GetTensorDataSize());
          pouts_[0]->SendTask(tx_task);
        }
      }
    }
    pin.DelTask(rx_task, task_type);
  }
  private:
  int cur_predict_num_ = 0;
  int64_t total_delay_ = 0;
  int64_t first_pkt_pts_;
};

DE_CLASS_REGISTER("de::benchmark::DevSender", DevSender);

void __attribute__((destructor)) x_fini(void)
{
    de::NodeClassRegistry::Remove("de::benchmark::DevSender");
    de::NodeClassRegistry::Remove("de::benchmark::DevReceiver");
    de::NodeClassRegistry::Remove("de::benchmark::AiEngineTest");
}

}
}