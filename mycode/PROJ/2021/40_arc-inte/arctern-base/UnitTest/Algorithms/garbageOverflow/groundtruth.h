
#ifndef UNITTEST_ALGORITHMS_GARBAGEOVERFLOW_GROUNDTRUTH_H_
#define UNITTEST_ALGORITHMS_GARBAGEOVERFLOW_GROUNDTRUTH_H_

#include "src/Algorithms/garbageOverflow/garbageOverflow.h"
#include "UnitTest/UnitTestBase/baseGroundTruth.h"
#include "UnitTest/UnitTestBase/baseAlgrithm.h"

typedef struct {
  std::vector<arctern::ArcternRect> rects;
  std::vector<float> confidences;
  std::vector<int> types;
} ONEPICTURE;

class GarbageOverflowData : public BaseGroundTruthData {
 public:
  GarbageOverflowData() = default;
  ~GarbageOverflowData() override = default;
  void addGroundTruth(YAML::Node &, std::string) override;

 public:
  std::vector<ONEPICTURE> rects_;
};

class GarbageOverflowUnittest : public BaseAlgrithmUnittest {
 public:
  GarbageOverflowUnittest(std::string ymlPath, std::string imgDir, std::string modelpath,arctern::Result *p);
  ~GarbageOverflowUnittest() override;

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
#endif  // UNITTEST_ALGORITHMS_GARBAGEOVERFLOW_GROUNDTRUTH_H_
