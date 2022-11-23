 //
// Created by gh on 2020/12/8.
//

#ifndef ARCTERN_BASE_ATLASENGIN_H
#define ARCTERN_BASE_ATLASENGIN_H

#include <vector>
#include "./src/Base/algorithmbase.h"

#include "src/Engin/src/enginBase.h"
#include "src/common/arcterndefs_generated.h"
#include "src/common/tensor.h"

#include "cnnmodelAtlas.h"
#include "atlasAclInfer.h"

#include "src/Processors/atlasAclPreprocess.h"
using arctern::TensorShape;

namespace arctern {

class AtlasEngine : public EnginBase {
 public:
  AtlasEngine(int gpuid, int batchsize);

  ~AtlasEngine() override;

  bool init_acl(const FileBuffer &file_buffer,InitParameter *p);

  int forward(const std::vector<std::pair<const void *, size_t>> &input, std::vector<std::vector<float>> &outputs,int batchSize);

  const aclrtStream & getAclStream(){
    return aclStream_;
  }

  const aclrtContext & getAclContext(){
    assert(aclContext_ != nullptr);
    return *aclContext_;
  }

  int getDeviceId(){
    return gpuId_;
  }

  int getMaxBatchSize(){
    return maxAclBatchSize_;
  }

 private:
  bool initAclStreams(const int& deviceId, aclrtContext &context, aclrtStream &stream);

  bool destroyAclStreams(aclrtContext &context, aclrtStream &stream);

  int gpuId_ = 0;
  int maxAclBatchSize_ = 1;
  aclrtContext * aclContext_ = nullptr;
  aclrtStream aclStream_ = nullptr; /// Acl runtime stream
  std::shared_ptr<CnnModelAtlas> v_atlas_infer_handle_;
  
};
}  // namespace arctern
#endif //ARCTERN_BASE_ATLASENGIN_H
