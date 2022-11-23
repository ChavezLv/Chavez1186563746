
#ifndef UNITTEST_ALGORITHMS_RIVERFLOAT_GROUNDTRUTH_H_
#define UNITTEST_ALGORITHMS_RIVERFLOAT_GROUNDTRUTH_H_

#include "src/Algorithms/riverFloat/riverFloat.h"
#include "UnitTest/UnitTestBase/baseGroundTruth.h"
#include "UnitTest/UnitTestBase/baseAlgrithm.h"

typedef struct {
  std::vector<arctern::ArcternRect> rects;
  std::vector<float> confidences;
} ONEPICTURE;

class RiverFloatData : public BaseGroundTruthData {
 public:
  RiverFloatData() = default;
  ~RiverFloatData() override = default;
  void addGroundTruth(YAML::Node &, std::string) override;

 public:
  std::vector<ONEPICTURE> rects_;
};

class RiverFloatUnittest : public BaseAlgrithmUnittest {
 public:
  RiverFloatUnittest(std::string ymlPath, std::string imgDir, std::string modelpath,arctern::Result *p);
  ~RiverFloatUnittest() override;

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
#endif  // UNITTEST_ALGORITHMS_RiverFloat_GROUNDTRUTH_H_
