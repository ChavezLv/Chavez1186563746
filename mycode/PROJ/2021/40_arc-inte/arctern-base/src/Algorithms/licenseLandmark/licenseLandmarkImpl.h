
#ifndef SRC_ALGORITHMS_LicenseLandmark_LicenseLandmarkIMPL_H_
#define SRC_ALGORITHMS_LicenseLandmark_LicenseLandmarkIMPL_H_
#include <utility>
#include <vector>
#include "src/Base/arcternalgorithm.h"
#include "licenseLandmark.h"

namespace arctern {

class LicenseLandmarkImpl : public ArcternAlgorithm {
 public:
  LicenseLandmarkImpl();
  ~LicenseLandmarkImpl() override;
  virtual void InitPreprocessorParam() override;
  void InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) override;

  int GetConfigInfo(arctern::MxnetModel *model) override {return 0;}

 private:
  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                             std::vector<arctern::Tensor<float> > &outputs) override;

  ErrorType CheckRunParam(const RunParameter *p) override;
};

}  // namespace arctern
#endif  // SRC_ALGORITHMS_LicenseLandmark_LicenseLandmarkIMPL_H_
