
#include "src/Algorithms/personScarf/personScarf.h"
#include <istream>
#include "opencv2/opencv.hpp"
#include <cassert>
#include <fstream>
/*Arctern Result
0 0.880479
0 0.88066
0 0.880448
0 0.880695
0 0.880435
0 0.880488
0 0.879206
0 0.880504
0 0.75056
0 0.880503
0 0.880643
0 0.880582
 * */
int main(int argc, char **argv) {
  arctern::PersonScarf recog;
  arctern::InitParameter initParam;
  initParam.batchNum_ = 1;
  initParam.model_path = "/data/leizhiming/arctern-models/develop/body/person-scarf-f32-d-0.0.0.bin";
  auto ret = recog.Init(&initParam);


  assert(ret == arctern::ErrorType::ERR_SUCCESS);

  cv::Mat image = cv::imread("/data/leizhiming/arctern-metadata/ModelsVerifyData/data/predict_scarf/test_person.jpg");

  ::arctern::ArcternImage img;
  img.width = image.cols;
  img.height = image.rows;
  img.imgdata.gdata = image.data;
  img.format = arctern::ARCTERN_IMAGE_FORMAT_BGR888;
  img.step = image.step;

  arctern::PersonScarfRunParameter runParameter;
  for(int i=0;i<12;i++){
    runParameter.imageV_.push_back(img);
  }
  runParameter.rectsV_.push_back({406, 178, 267, 781});
  runParameter.rectsV_.push_back({400,383,301,698});
  runParameter.rectsV_.push_back({384,24,229,645});
  runParameter.rectsV_.push_back({1236,613,307,468});
  runParameter.rectsV_.push_back({1522,694,394,385});
  runParameter.rectsV_.push_back({646,747,413,334});
  runParameter.rectsV_.push_back({741,6,205,444});
  runParameter.rectsV_.push_back({242,127,244,703});
  runParameter.rectsV_.push_back({1531,3,168,367});
  runParameter.rectsV_.push_back({5,150,157,709});
  runParameter.rectsV_.push_back({753,132,216,624});
  runParameter.rectsV_.push_back({1363,52,240,669});

  arctern::PersonScarfResult result;
  ret = recog.Process(&runParameter,&result);
  fstream fout("/data/leizhiming/arctern-metadata/ModelsVerifyData/results_arcterncpp/LINUX_MXNET_CPU/person-scarf-f32-d-0.0.0.rc.yml.txt",std::ios::binary| std::ios::out);
  fout << "PersonScarf:imageDir imageName scarf score"<<std::endl;

  if (ret == arctern::ErrorType::ERR_SUCCESS) {
    for (auto& ele : result.m_vClassifyResults)
    {
      fout << "/data/leizhiming/arctern-metadata/ModelsVerifyData/data/predict_scarf " << "test_person.jpg ";
      fout << ele.first <<" "<<ele.second<<std::endl;
      std::cout << ele.first << ' ' << ele.second << std::endl;
    }

  }
  fout.close();
  return 0;
}
