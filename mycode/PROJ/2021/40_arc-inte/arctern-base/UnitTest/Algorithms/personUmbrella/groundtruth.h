
#ifndef UNITTEST_ALGORITHMS_PERSONUMBRELLA_GROUNDTRUTH_H_
#define UNITTEST_ALGORITHMS_PERSONUMBRELLA_GROUNDTRUTH_H_

#include "src/Algorithms/personUmbrella/personUmbrella.h"
#include "UnitTest/UnitTestBase/baseGroundTruth.h"
#include "UnitTest/UnitTestBase/baseAlgrithm.h"
#include <unordered_map>

typedef struct {
  arctern::ArcternRect rect;
  arctern::PersonUmbrellaClassify personUmbrella;
} ONEPICTURE;

class PersonUmbrellaData : public BaseGroundTruthData {
 public:
  PersonUmbrellaData() = default;
  ~PersonUmbrellaData() override = default;

  void addGroundTruth(YAML::Node &, std::string) override;

  void addInputParam(YAML::Node &, std::string) override;

 public:
  std::vector<ONEPICTURE> personUmbrellas_;
  
};

class PersonUmbrellaUnittest : public BaseAlgrithmUnittest {
 public:
  PersonUmbrellaUnittest(std::string ymlGroundTruth, std::string ymlInput, std::string imgDir, std::string modelpath, arctern::Result *p);

  ~PersonUmbrellaUnittest() override ;

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
#endif  // UNITTEST_ALGORITHMS_PERSONUMBRELLA_GROUNDTRUTH_H_

