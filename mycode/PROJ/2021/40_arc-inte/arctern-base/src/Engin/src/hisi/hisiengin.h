//
// Created by gh on 2020/10/21.
//

/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.23
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#ifndef ARCTERN_HISIENGIN_H
#define ARCTERN_HISIENGIN_H

#include "../enginBase.h"
#include "nnie/sample_comm_nnie.h"
#include "hisienginUtil.h"
#include <mutex>
namespace arctern {
class HisiResult : public Result {
  void Release() override {}
  void resize(int n) override { ; }
};

class HisiEngin : public EnginBase {
 public:
  HisiEngin();
  ~HisiEngin() override;

  ErrorType Init(const InitParameter *p) override;
  void Release() override;
  ErrorType Process(const RunParameter *p,Result *r) override;


  bool loadModel(const MxnetModel *model, int batch);
  int forward(const Tensor<uint8> &input_tensor,
              std::vector<arctern::Tensor<float>> &outputTensors) override;


 protected:

  int NNIE_set_input(const Tensor<HI_U8> &input_tensor);

  void NNIE_ouput2tensor(std::vector<Tensor<float>> &outputs);

  HI_S32 SAMPLE_SVP_NNIE_Forward(SAMPLE_SVP_NNIE_PARAM_S *pstNnieParam,
                                 SAMPLE_SVP_NNIE_INPUT_DATA_INDEX_S *pstInputDataIdx,
                                 SAMPLE_SVP_NNIE_PROCESS_SEG_INDEX_S *pstProcSegIdx,
                                 HI_BOOL bInstant);

  bool input_shape_plain_to_piled(const int *plain_shapes,
                                  const size_t plain_shapes_num, const int *plain_idxes,
                                  const size_t plain_idxes_num,
                                  std::vector<TensorShape> &piled_shapes);
  int gpuId_ = -1;

  HisiResult result_;

 public:

  std::shared_ptr<NNIEModel> s_stNNIEModel;

  SAMPLE_SVP_NNIE_PARAM_S s_stCnnNnieParam;
  unsigned int u32MaxInputNum;

  SAMPLE_SVP_NNIE_CFG_S stNnieCfg = {0};
  std::mutex mtx_;
};
}

#endif //ARCTERN_HISIENGIN_H
