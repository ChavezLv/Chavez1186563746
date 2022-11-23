

#ifndef UNITTEST_ALGORITHMS_FIREDET_GROUNDTRUTH_H_
#define UNITTEST_ALGORITHMS_FIREDET_GROUNDTRUTH_H_

#include "src/Algorithms/fireDet/fireDet.h"
#include "UnitTest/UnitTestBase/baseGroundTruth.h"
#include "UnitTest/UnitTestBase/baseAlgrithm.h"

typedef struct {
  std::vector<arctern::ArcternRect> rects;
  std::vector<float> confidences;
  std::vector<int> types;
} ONEPICTURE;

class FireDetData : public BaseGroundTruthData {
 public:
  FireDetData() = default;
  ~FireDetData() override = default;
  void addGroundTruth(YAML::Node &, std::string) override;
  void addGroundTruth(std::string);
 public:
  std::vector<ONEPICTURE> rects_;
};

class FireDetUnittest : public BaseAlgrithmUnittest {
 public:
  FireDetUnittest(std::string ymlPath, std::string imgDir, std::string modelpath,arctern::Result *p);
  ~FireDetUnittest() override;

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
#endif  // UNITTEST_ALGORITHMS_FIREDET_GROUNDTRUTH_H_
