/****************************************************************************
 *  Filename:       mxnetengin.h
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         juwenqi
 *  Last modified:  2020.02.21
 *  email:          ju.wenqi@intellif.com
 ****************************************************************************/
#ifndef SRC_ENGIN_SRC_MXNET_MXNETENGIN_H_
#define SRC_ENGIN_SRC_MXNET_MXNETENGIN_H_
#include <vector>
#include "src/Base/algorithmbase.h"
#include "src/Engin/src/enginBase.h"
#include "src/common/arcterndefs_generated.h"
#include "mxnet/c_predict_api.h"
#include "src/common/tensor.h"
#include "src/common/Tensor5.h"
using arctern::TensorShape;

namespace arctern {


class MxnetEngin : public EnginBase {
 public:
    MxnetEngin();
    ~MxnetEngin() override;

    bool loadModel(const MxnetModel * model, int batch);

    int forward(const Tensor<float> &input_tensor,
                 std::vector<arctern::Tensor<float>> & outputTensors) override;

    int forward(std::vector<arctern::Tensor<float>> &outputTensors);



 protected:
    bool input_shape_plain_to_piled(const int *plain_shapes, const size_t plain_shapes_num,
                                    const int *plain_idxes, const size_t plain_idxes_num,
                                    std::vector<TensorShape> &piled_shapes);

 private:
    int gpuId_ = -1;
    /// < mxnet predictor handle
    PredictorHandle netHandle_ = nullptr;
};

}  // namespace arctern
#endif  // SRC_ENGIN_SRC_MXNET_MXNETENGIN_H_
