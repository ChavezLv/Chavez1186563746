#ifndef UNITTEST_ALGORITHMS_MANHOLECOVERDET_GROUNDTRUTH_H_
#define UNITTEST_ALGORITHMS_MANHOLECOVERDET_GROUNDTRUTH_H_

#include "src/Algorithms/manHoleCoverDet/manHoleCoverDet.h"
#include "UnitTest/UnitTestBase/baseGroundTruth.h"
#include "UnitTest/UnitTestBase/baseAlgrithm.h"

typedef struct {
  std::vector<arctern::ArcternRect> rects;
  std::vector<float> confidences;
  std::vector<int> types;
} ONEPICTURE;

class ManHoleCoverDetData : public BaseGroundTruthData {
 public:
  ManHoleCoverDetData() = default;
  ~ManHoleCoverDetData() override = default;
  void addGroundTruth(YAML::Node &, std::string) override;
  void addGroundTruth(std::string);
 public:
  std::vector<ONEPICTURE> rects_;
};

class ManHoleCoverDetUnittest : public BaseAlgrithmUnittest {
 public:
  ManHoleCoverDetUnittest(std::string ymlPath, std::string imgDir, std::string modelpath,arctern::Result *p);
  ~ManHoleCoverDetUnittest() override;

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
#endif  // UNITTEST_ALGORITHMS_MANHOLECOVERDET_GROUNDTRUTH_H_
