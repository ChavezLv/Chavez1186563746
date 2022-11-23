//
// Created by Admin on 2021/6/18.
//

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_LICENSERECOG_TRT_TRTLICENSERECOGIMPL_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_LICENSERECOG_TRT_TRTLICENSERECOGIMPL_H_
#include "src/common/arcterndefs_generated.h"
#include "src/common/arcterndefs_config_analyzer.h"
#include "src/common/tensor.h"
#include "src/common/imageConvert.h"
#include "src/common/trt/TrtAlgorithmUtil.h"

#include "src/Base/trt/ArcternAlgorithmEx.h"
#include "src/Base/arcternalgorithm.h"
#include "../../../Processors/NppPreprocessorEx.h"

#include "src/common/testUtil.h"

#include "src/Algorithms/licenseRecog/licenseRecog.h"

using namespace std;
namespace arctern{
class TrtLicenseRecogImpl: public ArcternAlgorithmEx{
 public:
  TrtLicenseRecogImpl();

  virtual ~TrtLicenseRecogImpl();



  virtual void InitPreprocessorRunParam(NppPreprocessorRunPt *runParm) override;

  virtual ErrorType GetModelsConfigInfo(const MxnetModels *models) override;

  virtual ErrorType LocalPostProcess(const RunParameter *p, int startId,
                                     int endId,vector<arctern::OutputTensor<float>> &outputs) override;

  virtual ArcternRect LocalGetROI(const RunParameter *p, int idx) override;

 private:
  std::string CalculateResult(const std::vector<float> &output);
  std::string CalculateResult(const std::vector<float> &output,std::vector<float> &score);

 protected:
  const std::string LicenseCharacters[70] = {
      "0","1","2","3","4","5","6","7","8","9",
      "A","B","C","D","E","F","G","H","J","K","L","M",
      "N","P","Q","R","S","T","U","V","W","X","Y","Z","^",
      "云","京","冀","吉","学","宁","川","新","晋","桂","沪","津","浙","渝","港","湘","琼",
      "甘","皖","粤","苏","蒙","藏","警","豫","贵","赣","辽","鄂","闽","陕","青","鲁","黑",
      "-"
  };
  int CORPUS_LEN = std::end(LicenseCharacters) - std::begin(LicenseCharacters);
  int INDEXFORSPACE = CORPUS_LEN - 1;
  std::string m_version;
};
}
#endif //ARCTERN_BASE_SRC_ALGORITHMS_FACEGLASS_TRT_TRTFACEGLASSIMPL_H_
