
#ifndef UNITTEST_ALGORITHMS_LicenseLandmark_GROUNDTRUTH_H_
#define UNITTEST_ALGORITHMS_LicenseLandmark_GROUNDTRUTH_H_

#include "src/Algorithms/licenseLandmark/licenseLandmark.h"
#include "UnitTest/UnitTestBase/baseGroundTruth.h"
#include "UnitTest/UnitTestBase/baseAlgrithm.h"
#include <unordered_map>

typedef struct {
  std::vector<arctern::ArcternPoint> points;
} ONEPICTURE;

class LicenseLandmarkData : public BaseGroundTruthData {
 public:
  LicenseLandmarkData() = default;
  ~LicenseLandmarkData() override = default;

  void addGroundTruth(std::string);

 public:
  std::vector<ONEPICTURE> landmarks_;
};

class LicenseLandmarkUnittest : public BaseAlgrithmUnittest {
 public:
  LicenseLandmarkUnittest(std::string ymlGroundTruth, std::string ymlInput, std::string imgDir, std::string modelpath, arctern::Result *p);

  ~LicenseLandmarkUnittest() override ;

  std::shared_ptr<arctern::AlgorithmInterface> createAlgHandle(int batch) override;

  std::shared_ptr<arctern::RunParameter> buildRunParameter(std::shared_ptr<BaseGroundTruthData> gdata,
                                                           int startId,
                                                           int len) override;

  std::shared_ptr<BaseGroundTruthData> generateGroundTruth() override;

  void checkResult(std::shared_ptr<BaseGroundTruthData> gdata,
                   int startId,
                   int len,
                   const arctern::Result *p) override;
};
#endif  // UNITTEST_ALGORITHMS_LicenseLandmark_GROUNDTRUTH_H_

