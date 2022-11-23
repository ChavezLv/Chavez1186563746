/**
 * @file de_aiengine_node.h
 * @brief AiEngine接口文件
 * @details 提供NN模型执行功能接口
 * @details AiEngine支持2种典型的工作方式，同步模式和Graph模式
 * @author DesdkTeam
 * @version 1.0.0
 * @date 2020/02/25
 * @copyright Intellif
 */

#ifndef DE_AIENGINE_H_
#define DE_AIENGINE_H_

#include <core/de_thread.h>
#include <core/de_serializer.h>

namespace de
{

/**@brief default task of AiEngine
*/
struct NNTask
{
  uint64_t id;
  std::vector<de::NDArray> tensors;
};

const constexpr int kBatchMaxNum = 32;
/**@brief NN layer execute info
*/
typedef struct{
  //uint32_t layer_cyctime;
  uint32_t layer_sched_cyctime; //层调度 
  uint32_t layer_start_cyctime; //nnp配置处理时间点或者dsp cnn算子计算开始 
  uint32_t layer_end_cyctime;   //nnp中断处理时间点或者dsp cnn算子计算完毕
}BlockTimeInfo;

typedef struct {
    uint32_t block_type;
    uint32_t dsp_layer_type;
}LayerType;


/**@brief NN model excute info
*/
typedef struct{
  uint32_t model_cyctime;
  float model_time;
  uint16_t batchNum;
  uint16_t blockNum;  //
  uint32_t model_perbatch_cyctime[de::kBatchMaxNum];//

  uint32_t model_start_cyctime;  //model 发起时间点，即开始处理消息时间点    
  uint32_t model_end_cyctime;    //model 结束时间点，即上报cfn时间点

  std::vector<BlockTimeInfo> block_cyctime;
  std::vector<LayerType> layer_type;
  /************************************************************
   *       block_cyctime[batchNum][blockNum];
   * ********************************************************/
}ModelTime;


namespace serializer
{
STRUCT_SERIALIZE_2(de::NNTask,
                   uint64_t, id,
                   std::vector<de::NDArray>, tensors);


STRUCT_SERIALIZE_9(de::ModelTime,
	   uint32_t, model_cyctime,
	   float, model_time,
	   uint16_t, batchNum,
	   uint16_t, blockNum,
	   uint32_t[de::kBatchMaxNum], model_perbatch_cyctime,
	   uint32_t, model_start_cyctime,
	   uint32_t, model_end_cyctime,
	   std::vector<de::BlockTimeInfo>, block_cyctime,
       std::vector<LayerType>,  layer_type);

} // namespace serializer




class DE_DLL AiEngine : public de::Thread
{
 public:
  AiEngine(bool stream_mode = true);
  ~AiEngine();
  using PreProcFunc = std::function< std::vector<de::NDArray >(void*, int32_t, void*)>;
  using PostProcFunc = std::function< void* (void* , int32_t, std::vector<de::NDArray>, void*)>;
  /**@brief 加载nn模型
  * @param[in]       model_path        模型路径
  * @param[in]       para_path         参数路径
  * @param[in]       batch           批处理参数，即同时有几个输入。1~8，默认为1
  * @param[in]       encrypt           模型是否加密
  * @return        加载结果
  *            - 0：成功
  *            - -1：失败
  */
  int LoadModel(const char * model_path,
                const char * para_path,
                uint32_t batch = 1,
                bool encrypt = false) ;


  /**@brief nn引擎卸载nn模型
  * @return  函数执行结果
  * - 0    卸载成功
  * - 1    卸载失败
  */
  int UnloadModel();


  /**@brief 参数更新生效
  * @param  无
  * @return   无
  */
  void UpdatePara(void);


  /**@brief 设置前处理函数，不设置默认输入是NNTask类型
  * @param [in] pre_func      前处理函数
  * @return     无
  */
  void SetPreProc(PreProcFunc pre_func);


  /**@brief 设置后处理函数，不设置默认输出NNTask类型
  * @param [in] pout_idx      输出索引
  * @param [in] post_func     后处理函数
  * @return     无
  */
  void SetPostProc(int pout_idx, PostProcFunc post_func);

  /**@brief graph模式任务处理函数，无需用户调用
  * @param [in] rx_task       任务指针
  * @param [in] task_type     任务类型
  * @return   无
  */
  virtual void Proc(void* rx_task, int32_t task_type, POutType pout_type = DEFAULT_POUT);

  /**@brief  nn引擎同步推理接口
  * @param[in] inputs           模型输入
  * @param[out] outputs         模型输出
  * @return  函数执行结果
  * - 0                 推理成功
  * - 1               推理失败
  */

//  int Predict(std::vector<de::NDArray>& inputs, std::vector<de::NDArray>& outputs);


 
  /**@brief  nn引擎同步推理接口-带前后处理
  * @param[in] rx_task          用户输入任务
  * @param[in] task_type        用户输入任务类型
  * @param[in] pout_idx			用户指定后处理索引，默认为0
  * @return  函数执行结果
  * - tx_task        	    	用户输出任务
  */
  void* Call(void* rx_task, int32_t task_type, int32_t pout_idx = 0);


  /**@brief 调试信息获取，无需用户调用 
  * @param      无
  * @return     
  *         调试信息指针
  */
  const ModelTime* GetModelTimeInfo();

  /**
  * @brief 重写Stop函数
	*/
  virtual void Stop(void) override;

 protected:
  class Impl_;
  std::shared_ptr<Impl_> impl_;
  bool stream_mode_ = true;
  std::string model_net_path_;
  std::string model_param_path_;
  PreProcFunc pre_func_ = nullptr;
  PostProcFunc post_funcs_[kMaxUserPoutNum] = {nullptr, };
  int period_ = 1; ///< aie thread process period
  int64_t count_ = 0;
  std::mutex call_mutex_;
};

}
#endif



