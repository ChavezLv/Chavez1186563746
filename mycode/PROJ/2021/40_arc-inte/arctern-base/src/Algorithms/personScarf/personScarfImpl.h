
#ifndef SRC_ALGORITHMS_PERSONSCAEF_PERSONSCARFIMPL_H_
#define SRC_ALGORITHMS_PERSONSCAEF_PERSONSCARFIMPL_H_
#include "src/Base/arcternalgorithm.h"
#include <vector>

namespace arctern {
class PersonScarfImpl : public ArcternAlgorithm {
public:
  PersonScarfImpl();
  ~PersonScarfImpl();

  ArcternRect LocalGetRoi(const RunParameter *p, int idx) override;
  void InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) override;
protected:
  ErrorType
  LocalPostProcess(const RunParameter *p, int startId, int endId,
                   std::vector<arctern::Tensor<float>> &outputs) override;

  int GetConfigInfo(arctern::MxnetModel *model) { return 1; }

private:
  std::pair<int, float> calcType(const float *p_data, const size_t class_num);
  cv::Rect check_rect(const int width, const int height, const cv::Rect bbox);
};
} // namespace arctern
#endif
