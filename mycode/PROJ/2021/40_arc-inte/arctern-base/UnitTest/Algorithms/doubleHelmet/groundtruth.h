
#ifndef UNITTEST_ALGORITHMS_DOUBLEHELMET_GROUNDTRUTH_H_
#define UNITTEST_ALGORITHMS_DOUBLEHELMET_GROUNDTRUTH_H_

#include "src/Algorithms/doubleHelmet/doubleHelmet.h"
#include "UnitTest/UnitTestBase/baseGroundTruth.h"
#include "UnitTest/UnitTestBase/baseAlgrithm.h"
#include <unordered_map>

typedef struct {
  arctern::ArcternRect rect;
  arctern::DoubleHelmetClassify doubleHelmet;
} ONEPICTURE;

class DoubleHelmetData : public BaseGroundTruthData {
 public:
  DoubleHelmetData() = default;
  ~DoubleHelmetData() override = default;

  void addGroundTruth(YAML::Node &, std::string) override;

  void addInputParam(YAML::Node &, std::string) override;

 public:
  std::vector<ONEPICTURE> doubleHelmets_;
  
};

class DoubleHelmetUnittest : public BaseAlgrithmUnittest {
 public:
  DoubleHelmetUnittest(std::string ymlGroundTruth, std::string ymlInput, std::string imgDir, std::string modelpath, arctern::Result *p);

  ~DoubleHelmetUnittest() override ;

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
#endif  // UNITTEST_ALGORITHMS_DOUBLEHELMET_GROUNDTRUTH_H_

