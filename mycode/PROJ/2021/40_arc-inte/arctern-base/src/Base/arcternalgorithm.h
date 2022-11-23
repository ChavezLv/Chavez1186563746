/****************************************************************************
 *  Filename:       arcternalgorithm.h
 *  Copyright:      Copyright @ 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         juwenqi
 *  Last modified:  2020.04.20
 *  email:          ju.wenqi@intellif.com
 ****************************************************************************/
#ifndef SRC_BASE_ARCTERNALGORITHM_H_
#define SRC_BASE_ARCTERNALGORITHM_H_
#include <vector>
#include "src/Base/algorithmbase.h"
#include "src/Engin/src/enginmanager.h"
#include "src/common/tensor.h"
#include "src/Processors/opencvPreprocessor.h"
//#include "src/Processors/Algorithms/preprocesser.h"
#include "src/Processors/ImageFormatConverters/ImageFormatConverter.h"



namespace arctern {

enum class NetworkShape {
  NCHW = 0,   ///< NHWC
  NHWC = 1,   ///< NHWC
};

class ArcternAlgorithm : public AlgorithmBase {
 public:
  ArcternAlgorithm();
  ArcternAlgorithm(const ArcternAlgorithm &) = delete;
  ArcternAlgorithm(ArcternAlgorithm &&) = delete;
  ArcternAlgorithm &operator=(const ArcternAlgorithm &) = delete;
  ArcternAlgorithm &operator=(ArcternAlgorithm &&) = delete;
  ~ArcternAlgorithm() override;

  ErrorType Init(const InitParameter *p) override;
  void Release() override;
  ErrorType Process(const RunParameter *p,Result *r) override;

 public:
  virtual ErrorType LocalPostProcess(const RunParameter *p, int startId, int endId,
                                     std::vector<arctern::Tensor<float> > &outputs) = 0;
  virtual int GetConfigInfo(arctern::MxnetModel *model) = 0;

 public:
  virtual ErrorType CheckInitParam(const InitParameter *p);
  virtual ErrorType CheckRunParam(const RunParameter *p);
  virtual int GetModelsConfigInfo(arctern::MxnetModels *models) {return 0;};

  virtual ArcternRect LocalGetRoi( const RunParameter * p, int idx);

  virtual ErrorType CommonPreProcess(const RunParameter *p);
  virtual ErrorType CommonProProcess(const RunParameter *p);
  virtual ErrorType LocalPreProcess( const RunParameter *p, int idx);

 public:
  virtual void InitPreprocessorParam();
  virtual void InitPreprocessorRunParam(OpencvPreprocessorRunRt *runParm);

  virtual int GetModelHeight() { return 0;}
  virtual int GetModelWidth() { return 0; }

 protected:
  virtual int forward(const Tensor<float> &input_tensor, std::vector<arctern::Tensor<float> > &outputs) ;
  ///> \brief hisi use this function;
  virtual int forward(const Tensor<uint8> &input_tensor, std::vector<arctern::Tensor<float> > &outputs) ;

 public:
  void SetEngine(std::vector<std::shared_ptr<EnginBase>> & engines) { engines_ = std::move(engines);}


 protected:
  bool swapChannel();


  std::vector<std::shared_ptr<EnginBase>> engines_;
  std::vector<cv::Mat> preprocessed_imgs_;
  std::unique_ptr<arctern::ImageFormatConverter> imageFormatChanger_;
  std::unique_ptr<arctern::OpencvPreprocessor> preProcessor_;

  int input_h_;
  int input_w_;

  Result *result_ = nullptr;
  InitParameter initParam_;

  ///< network shape ,default nchw
  NetworkShape netShape_ = NetworkShape::NCHW;
  ArcternImageFormat dstImageFormatForForward_ = ARCTERN_IMAGE_FORMAT_RGB888;
};
}  // namespace arctern
#endif  // SRC_BASE_ARCTERNALGORITHM_H_
