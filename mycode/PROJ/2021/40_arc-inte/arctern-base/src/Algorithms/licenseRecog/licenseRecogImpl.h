/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chenbin
 *  Last modified:  2020.12.01
 *  email:          chen.bin1@intellif.com
 ****************************************************************************/
#ifndef SRC_ALGORITHMS_LICENSERECOG_LICENSERECOGIMPL_H_
#define SRC_ALGORITHMS_LICENSERECOG_LICENSERECOGIMPL_H_
#include <utility>
#include <vector>
#include "src/Base/arcternalgorithm.h"
#include "licenseRecog.h"

namespace arctern {

class LicenseRecogImpl : public ArcternAlgorithm {
 public:
  LicenseRecogImpl();
  ~LicenseRecogImpl() override;

  ArcternRect LocalGetRoi(const RunParameter *p, int idx) override;
  void InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm) override;

  int GetConfigInfo(arctern::MxnetModel *model) override {return 0;}

 private:
  ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                             std::vector<arctern::Tensor<float> > &outputs) override;

  ErrorType CheckRunParam(const RunParameter *p) override;
  std::string calc_license(const std::vector<float> &output);

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

};

}  // namespace arctern
#endif  // SRC_ALGORITHMS_LICENSERECOG_LICENSERECOGIMPL_H_
