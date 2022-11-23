//
// Created by lzm on 2021/9/17.
//

#ifndef ARCTERN_BASE_SRC_ATLASLINUXALGORITHMS_LICENSERECOG_ATLASLICENSERECOGIMPL_H_
#define ARCTERN_BASE_SRC_ATLASLINUXALGORITHMS_LICENSERECOG_ATLASLICENSERECOGIMPL_H_

#include "src/Algorithms/licenseRecog/licenseRecog.h"
#include "src/Base/atlas/ArcternAlgorithmEx.h"
namespace arctern {
class AtlasLicenseRecogImpl : public ArcternAlgorithmEx {
 public:
  AtlasLicenseRecogImpl();
  ~AtlasLicenseRecogImpl() override ;
  
 public:
  virtual ErrorType GetModelsConfigInfo(const TrtModels* models) override;


  virtual ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                                      std::vector<std::vector<float>> &outputs) override;

  virtual ErrorType LocalPreProcess(std::vector<DvppDataInfo>& vInputInfo,
                                    std::vector<DvppDataInfo>& vOutputInfos,
                                    std::vector<cv::Rect>& vRects) override;

  virtual cv::Rect LocalGetROI(const RunParameter *p, int idx) override;

  private:
    std::string CalculateResult(const std::vector<float> &output);
    std::vector<float> doTopK(float* data, int size, int strip);
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


}

#endif //ARCTERN_BASE_SRC_ATLASLINUXALGORITHMS_LICENSERECOG_ATLASLICENSERECOGIMPL_H_
