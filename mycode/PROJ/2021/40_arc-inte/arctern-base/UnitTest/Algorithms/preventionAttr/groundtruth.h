
#ifndef UNITTEST_ALGORITHMS_PreventionAttr_GROUNDTRUTH_H_
#define UNITTEST_ALGORITHMS_PreventionAttr_GROUNDTRUTH_H_

#include "src/Algorithms/preventionAttr/preventionAttr.h"
#include "UnitTest/UnitTestBase/baseGroundTruth.h"
#include "UnitTest/UnitTestBase/baseAlgrithm.h"

typedef struct {
  arctern::ArcternRect rect;
  arctern::SinglePreventionAttr attrs;
} ONEPICTURE;

class PreventionAttrData : public BaseGroundTruthData {
 public:
  PreventionAttrData() = default;
  ~PreventionAttrData() override = default;

  void addGroundTruth( std::string) ;

  void addInputParam( std::string) ;

 public:
  std::vector<ONEPICTURE> PreventionAttr_;
};

class PreventionAttrUnittest : public BaseAlgrithmUnittest {
 public:
  PreventionAttrUnittest(std::string ymlGroundTruth, std::string ymlInput, std::string imgDir, std::string modelpath, arctern::Result *p);

  ~PreventionAttrUnittest() override ;

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

#endif  // UNITTEST_ALGORITHMS_PreventionAttr_GROUNDTRUTH_H_


