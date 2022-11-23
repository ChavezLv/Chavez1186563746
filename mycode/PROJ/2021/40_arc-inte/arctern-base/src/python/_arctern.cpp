#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <opencv2/opencv.hpp>
#include <chrono>
#include <iostream>
#include <string>
#include "ndarray_converter.h"

#include "Algorithms/faceDetectTiny/faceDetectTiny.h"
#include "Algorithms/ageGender/ageGender.h"
#include "Algorithms/faceGlass/faceGlass.h"
#include "Algorithms/faceMask/faceMask.h"
#include "Algorithms/faceQuality/faceQuality.h"
#include "Algorithms/facePose/facePose.h"
#include "Algorithms/faceHat/faceHat.h"
#include "Algorithms/faceExpression/faceExpression.h"
#include "Algorithms/faceNoiseMotionblur/faceNoiseMotionblur.h"
#include "Algorithms/faceAbnomalLight/faceAbnomalLight.h"
#include "Algorithms/faceOcclusion/faceOcclusion.h"
#include "Algorithms/memoryDB/memoryDB.h"
#include "Algorithms/faceGluon/faceGluon.h"
#include "Algorithms/personFeat/personFeat.h"
#include "Algorithms/faceBasicMultiAttr/faceBasicMultiAttr.h"
#include "Algorithms/faceDetectRTN/faceDetectRTN.h"
#include "Algorithms/personQuality/personQuality.h"
#include "Algorithms/faceQualityPose/faceQualityPose.h"
#include "Algorithms/facialLandmark/facialLandmark.h"
#include "Algorithms/personCompleteness/personCompleteness.h"

namespace py = pybind11;
using namespace arctern;

const char PY_AES_KEY[] = "EB641ACB6D2C16E2493025AF4FE4CC47";

void show_image(cv::Mat image) {
  cv::imshow("image_from_Cpp", image);
  cv::waitKey(0);
}

cv::Mat read_image(std::string image_name) {
  cv::Mat image = cv::imread(image_name, CV_LOAD_IMAGE_COLOR);
  return image;
}

cv::Mat passthru(cv::Mat image) { return image; }

cv::Mat cloneimg(cv::Mat image) { return image.clone(); }

///
/// \author  Wu Tao
/// \date 2019.09.26
/// \brief 人脸属性，包括年龄，性别
typedef struct {

  /// 性别信息:
  ///   第一个字段表示类型, 0 代表 男性, 1 代表 女性 ;
  //    第二个字段表示可信度得分
  std::pair<bool, float> gender;

  /// 年龄信息
  ///     第一个字段表示年龄
  ///     第二个字段表示年龄可信度得分
  std::pair<int, float> age;
} FaceAttribute;

/// \brief 佩戴眼镜
enum Glass {
  NOGLASS = 0, ///< 没有佩戴眼镜
  TYPE1 = 1,   ///< 普通眼镜
  TYPE2 = 2,    ///< 墨镜
};

/// \brief 帽子
enum Hat {
  NOHAT = 0,     ///< 没带帽子
  DETECTED = 1,  ///< 检测到帽子
};

typedef struct {
  std::pair<int, float> age;
  std::pair<int, float> gender;
  std::pair<int, float> glasses;
  std::pair<int, float> mask;
  std::pair<int, float> headwear;
} FaceBasicMultiAttribute;

typedef struct {
  float conf_thresh;   ///< 可信度阈值。低于阈值的检测框会被舍弃
} FaceDetectorYolov3Parameter;

typedef struct {
  float conf_thresh;
  bool using_landmark;
} RetinafaceParameter;

/// \brief
typedef struct  {
    std::vector<float> truncation;  ///< 人体完整度
    std::vector<std::pair<bool, float>> visible_parts;   ///< 人体部分的可视信息，包括头、胸部、腹部、大腿、小腿以及它们的置信度
    std::vector<std::vector<std::pair<bool, float>>> soft_mask;  ///< 人体分成16*8个网格，描述其是否可视以及置信度
}PersonCompletenessInfo;

void convertMat2ArcternImage(const cv::Mat &image, ArcternImage &img) {
  img.width = image.cols;
  img.height = image.rows;
  img.imgdata.gdata = image.data;
  img.format = ARCTERN_IMAGE_FORMAT_BGR888;
}

class InitException : public std::exception {
 public:
  explicit InitException(const char *msg) : msg_(msg) {

  }

  virtual const char *what() {
    return msg_.c_str();
  }

  std::string msg_;
};

class RuntimeException : public std::exception {
 public:
  explicit RuntimeException(const char *msg) : msg_(msg) {
  }

  virtual const char *what() {
    return msg_.c_str();
  }

  std::string msg_;
};

class ArcternManager {
 public:
  static ArcternManager create() {
    return ArcternManager();
  }

  void set_face_tiny_det_model(const std::string &model_path,
                               const FaceDetectorYolov3Parameter &param = FaceDetectorYolov3Parameter()) {
    faceDetTiny_ = std::make_unique<arctern::FaceDetectTiny>(); //reset(new arctern::FaceDetectTiny);

    InitParameter parameter;
    parameter.batchNum_ = 1;
    parameter.model_path = model_path;
    parameter.thresh_ = param.conf_thresh;

    auto code = faceDetTiny_->Init(&parameter);
    if (code != ErrorType::ERR_SUCCESS) {
      throw InitException("facedettiny init error");
    }
  }

  std::vector<std::pair<std::vector<int>, float>> detect_face_tiny(
      const cv::Mat &image) {

    ArcternImage img;
    convertMat2ArcternImage(image, img);
    arctern::FaceDetectTinyRunParameter runParameter;
    runParameter.imageV_.emplace_back(img);

    arctern::FaceDetectTinyResult result;
    auto code = faceDetTiny_->Process(&runParameter, &result);


    if (code != ErrorType::ERR_SUCCESS || result.detInfos_.size() != 1) {
      throw RuntimeException("detect_face_tiny error");
    }

    int num = result.detInfos_[0].size();
    std::vector<std::pair<std::vector<int>, float>> results;
    for (int i = 0; i < num; i++) {
      auto &info = result.detInfos_[0][i];
      std::vector<int>
          vecr = {static_cast<int>(info.rect.x), static_cast<int>(info.rect.y), static_cast<int>(info.rect.width),
                  static_cast<int>(info.rect.height)};
      results.emplace_back(std::make_pair(vecr, info.score));
    }

    return results;
  }

  std::vector<std::vector<std::pair<std::vector<int>, float>>> detect_face_tiny_batch(
      const std::vector<cv::Mat> &images) {
   
    std::vector<std::vector<std::pair<std::vector<int>, float>>> resultss;
    for (auto &image : images) {
      resultss.push_back(detect_face_tiny(image));
    }

    return resultss;
  }

  void set_face_gender_age_model(const std::string &model_path) {
    ageGender_ = std::make_unique<arctern::AgeGender>();

    InitParameter parameter;
    parameter.batchNum_ = 1;
    parameter.model_path = model_path;

    auto code = ageGender_->Init(&parameter);
    if (code != ErrorType::ERR_SUCCESS) {
      throw InitException("ageGender init error");
    }
  }

  FaceAttribute predict_face_gender_age(
      const cv::Mat &image,
      const std::vector<int> &face) {
    ArcternImage img;
    convertMat2ArcternImage(image, img);
    arctern::AgeGenderRunParameter runParameter;
    runParameter.imageV_.emplace_back(img);
    ArcternRect rect{static_cast<float>(face[0]), static_cast<float>(face[1]), static_cast<float>(face[2]),
                     static_cast<float>(face[3])};
    runParameter.rectsV_.emplace_back(rect);

    arctern::AgeGenderResult result;
    auto code = ageGender_->Process(&runParameter, &result);

    if (code != ErrorType::ERR_SUCCESS || result.ageGenders_.size() != 1) {
      throw RuntimeException("predict_face_gender_age error");
    }

    FaceAttribute attribute;
    attribute.age = std::make_pair(result.ageGenders_[0].age, result.ageGenders_[0].ageScore);
    int gender = result.ageGenders_[0].gender == MALE ? 0 : 1;
    attribute.gender = std::make_pair(gender, result.ageGenders_[0].genderScore);
    return attribute;
  }

  std::vector<std::vector<FaceAttribute>> predict_face_gender_age_batch(
      const std::vector<cv::Mat> &images,
      const std::vector<std::vector<std::vector<int>>> &faces) {
    
    int cnt = images.size();

    std::vector<std::vector<FaceAttribute>> attrss;
    for (int i = 0; i < cnt; i++) {
      std::vector<FaceAttribute> attrs;
      for (size_t k = 0; k < faces[i].size(); k++) {
        auto attr = predict_face_gender_age(images[i], faces[i][k]);
        attrs.push_back(attr);
      }
      attrss.push_back(attrs);
    }

    return attrss;
  }

  void set_face_glass_model(const std::string &model_path) {
    faceGlass_ = std::make_unique<arctern::FaceGlass>();
    //faceGlass_.reset(new arctern::FaceGlass);

    InitParameter parameter;
    parameter.batchNum_ = 1;
    parameter.model_path = model_path;

    auto code = faceGlass_->Init(&parameter);
    if (code != ErrorType::ERR_SUCCESS) {
      throw InitException("face glass init error");
    }
  }

  std::pair<Glass, float> predict_face_glass(const cv::Mat &image,
                                             const std::vector<int> &face) {
    ArcternImage img;
    convertMat2ArcternImage(image, img);
    arctern::FaceGlassRunParameter runParameter;
    runParameter.imageV_.emplace_back(img);
    ArcternRect rect{static_cast<float>(face[0]), static_cast<float>(face[1]), static_cast<float>(face[2]),
                     static_cast<float>(face[3])};
    runParameter.rectsV_.emplace_back(rect);

    arctern::FaceGlassResult result;
    auto code = faceGlass_->Process(&runParameter, &result);

    if (code != ErrorType::ERR_SUCCESS || result.FaceGlasss_.size() != 1) {
      throw RuntimeException("predict_face_glass error");
    }

    Glass glass = NOGLASS;
    switch (result.FaceGlasss_[0].glassesStatus) {
      case NOGLASSES : {
        glass = NOGLASS;
        break;
      }
      case GLASSES : {
        glass = TYPE1;
        break;
      }
      default:glass = TYPE2;
    }

    std::pair<Glass, float> ret(glass, result.FaceGlasss_[0].statusScore);
    return ret;
  }

  std::vector<std::vector<std::pair<int, float>>> predict_face_glass_batch(
      const std::vector<cv::Mat> &images,
      const std::vector<std::vector<std::vector<int>>> &faces) {
    arctern::RunParameter runParameter;
    int cnt = images.size();

    std::vector<std::vector<std::pair<int, float>>> glassesss;
    for (int i = 0; i < cnt; i++) {
      std::vector<std::pair<int, float>> glasses;
      for (int k = 0; k < faces[i].size(); k++) {
        auto glass = predict_face_glass(images[i], faces[i][k]);
        glasses.push_back(std::make_pair(glass.first, glass.second));
      }
      glassesss.push_back(glasses);
    }

    return glassesss;
  }

  void set_face_mask_model(const std::string &model_path) {
    faceMask_ = std::make_unique<arctern::FaceMask>();

    InitParameter parameter;
    parameter.batchNum_ = 1;
    parameter.model_path = model_path;

    auto code = faceMask_->Init(&parameter);
    if (code != ErrorType::ERR_SUCCESS) {
      throw InitException("face mask init error");
    }
  }

  std::pair<bool, float> predict_face_mask(const cv::Mat &image, const std::vector<int> &face) {
    ArcternImage img;
    convertMat2ArcternImage(image, img);
    arctern::FaceMaskRunParameter runParameter;
    runParameter.imageV_.emplace_back(img);
    ArcternRect rect{static_cast<float>(face[0]), static_cast<float>(face[1]), static_cast<float>(face[2]),
                     static_cast<float>(face[3])};
    runParameter.rectsV_.emplace_back(rect);
    arctern::FaceMaskResult result;
    auto code = faceMask_->Process(&runParameter, &result);

    if (code != ErrorType::ERR_SUCCESS || result.FaceMasks_.size() != 1) {
      throw RuntimeException("predict_face_mask error");
    }

    std::pair<bool, float> ret(result.FaceMasks_[0].maskStatus == MASK, result.FaceMasks_[0].statusScore);
    return ret;
  }

  std::vector<std::vector<std::pair<bool, float>>> predict_face_mask_batch(
      const std::vector<cv::Mat> &images,
      const std::vector<std::vector<std::vector<int>>> &faces) {
    int cnt = images.size();

    std::vector<std::vector<std::pair<bool, float>>> maskss;
    for (int i = 0; i < cnt; i++) {
      std::vector<std::pair<bool, float>> masks;
      for (size_t k = 0; k < faces[i].size(); k++) {
        auto mask = predict_face_mask(images[i], faces[i][k]);
        masks.push_back(mask);
      }
      maskss.push_back(masks);
    }

    return maskss;
  }

  void set_face_quality_model(const std::string &model_path) {
    faceQuality_ = std::make_unique<arctern::FaceQuality>();

    InitParameter parameter;
    parameter.batchNum_ = 1;
    parameter.model_path = model_path;

    auto code = faceQuality_->Init(&parameter);
    if (code != ErrorType::ERR_SUCCESS) {
      throw InitException("face quality init error");
    }
  }

  float predict_face_quality(const cv::Mat &image, const std::vector<int> &face) {
    ArcternImage img;
    convertMat2ArcternImage(image, img);
    arctern::FaceQualityRunParameter runParameter;
    runParameter.imageV_.emplace_back(img);
    ArcternRect rect{static_cast<float>(face[0]), static_cast<float>(face[1]), static_cast<float>(face[2]),
                     static_cast<float>(face[3])};
    runParameter.rectsV_.emplace_back(rect);

    arctern::FaceQualityResult result;
    auto code = faceQuality_->Process(&runParameter, &result);

    if (code != ErrorType::ERR_SUCCESS || result.faceQuality_.size() != 1) {
      throw RuntimeException("predict_face_quality error");
    }
    return result.faceQuality_[0];
  }

  std::vector<std::vector<float>> predict_face_quality_batch(
      const std::vector<cv::Mat> &images,
      const std::vector<std::vector<std::vector<int>>> &faces) {
    int cnt = images.size();

    std::vector<std::vector<float>> qualityss;
    for (int i = 0; i < cnt; i++) {
      std::vector<float> qualitys;
      for (size_t k = 0; k < faces[i].size(); k++) {
        auto score = predict_face_quality(images[i], faces[i][k]);
        qualitys.push_back(score);
      }
      qualityss.push_back(qualitys);
    }

    return qualityss;
  }

  void set_face_pose_cls_model(const std::string &model_path) {
    facePose_ = std::make_unique<arctern::FacePose>();

    InitParameter parameter;
    parameter.batchNum_ = 1;
    parameter.model_path = model_path;

    auto code = facePose_->Init(&parameter);
    if (code != ErrorType::ERR_SUCCESS) {
      throw InitException("face_pose_cls init error");
    }
  }

  std::vector<std::pair<int, float>> predict_face_pose_cls(const cv::Mat &image,
                                                           const std::vector<int> &face) {
    ArcternImage img;
    convertMat2ArcternImage(image, img);
    arctern::FacePoseRunParameter runParameter;
    runParameter.imageV_.emplace_back(img);
    ArcternRect rect{static_cast<float>(face[0]), static_cast<float>(face[1]), static_cast<float>(face[2]),
                     static_cast<float>(face[3])};
    runParameter.rectsV_.emplace_back(rect);

    arctern::FacePoseResult result;
    auto code = facePose_->Process(&runParameter, &result);

    if (code != ErrorType::ERR_SUCCESS || result.facePose_.size() != 1) {
      throw RuntimeException("predict_face_pose error");
    }
    std::vector<std::pair<int, float>> ret{{0, result.facePose_[0].pitch},
                                           {1, result.facePose_[0].yaw},
                                           {2, result.facePose_[0].roll}};

    return ret;
  }

  std::vector<std::vector<std::vector<std::pair<int, float>>>> predict_face_pose_cls_batch(
      const std::vector<cv::Mat> &images,
      const std::vector<std::vector<std::vector<int>>> &faces) {
    int cnt = images.size();

    std::vector<std::vector<std::vector<std::pair<int, float>>>> posess;
    for (int i = 0; i < cnt; i++) {
      std::vector<std::vector<std::pair<int, float>>> poses;
      for (size_t k = 0; k < faces[i].size(); k++) {
        auto pose = predict_face_pose_cls(images[i], faces[i][k]);
        poses.emplace_back(pose);
      }
      posess.emplace_back(poses);
    }

    return posess;
  }

  void set_face_hat_model(const std::string &model_path) {
    faceHat_ = std::make_unique<arctern::FaceHat>();

    InitParameter parameter;
    parameter.batchNum_ = 1;
    parameter.model_path = model_path;

    auto code = faceHat_->Init(&parameter);
    if (code != ErrorType::ERR_SUCCESS) {
      throw InitException("face_face_hat init error");
    }
  }

  std::pair<Hat, float> predict_face_hat(const cv::Mat &image,
                                         const std::vector<int> &face) {
    ArcternImage img;
    convertMat2ArcternImage(image, img);
    arctern::FaceHatRunParameter runParameter;
    runParameter.imageV_.emplace_back(img);
    ArcternRect rect{static_cast<float>(face[0]), static_cast<float>(face[1]), static_cast<float>(face[2]),
                     static_cast<float>(face[3])};
    runParameter.rectsV_.emplace_back(rect);

    arctern::FaceHatResult result;
    auto code = faceHat_->Process(&runParameter, &result);

    if (code != ErrorType::ERR_SUCCESS || result.FaceHats_.size() != 1) {
      throw RuntimeException("predict_face_hat error");
    }

    Hat hat = result.FaceHats_[0].hatStatus == HatStatus::Hat ? DETECTED : NOHAT;

    return std::make_pair(hat, result.FaceHats_[0].statusScore);
  }

  std::vector<std::vector<std::pair<int, float>>> predict_face_hat_batch(
      const std::vector<cv::Mat> &images,
      const std::vector<std::vector<std::vector<int>>> &faces) {
    int cnt = images.size();

    std::vector<std::vector<std::pair<int, float>>> hatss;
    for (int i = 0; i < cnt; i++) {
      std::vector<std::pair<int, float>> hats;
      for (size_t k = 0; k < faces[i].size(); k++) {
        auto hat = predict_face_hat(images[i], faces[i][k]);
        hats.emplace_back(hat);
      }
      hatss.emplace_back(hats);
    }

    return hatss;
  }

  void set_face_expression_recognization_model(const std::string &model_path) {
    faceExpression_ = std::make_unique<arctern::FaceExpression>();

    InitParameter parameter;
    parameter.batchNum_ = 1;
    parameter.model_path = model_path;

    auto code = faceExpression_->Init(&parameter);
    if (code != ErrorType::ERR_SUCCESS) {
      throw InitException("face_face_expression init error");
    }
  }

  std::pair<ExpressionType, float> predict_face_expression(const cv::Mat &image,
                                                           const std::vector<int> &face) {
    ArcternImage img;
    convertMat2ArcternImage(image, img);
    arctern::FaceExpressionRunParameter runParameter;
    runParameter.imageV_.emplace_back(img);
    ArcternRect rect{static_cast<float>(face[0]), static_cast<float>(face[1]), static_cast<float>(face[2]),
                     static_cast<float>(face[3])};
    runParameter.rectsV_.emplace_back(rect);
    arctern::FaceExpressionResult result;
    auto code = faceExpression_->Process(&runParameter, &result);


    if (code != ErrorType::ERR_SUCCESS || result.faceExpressions_.size() != 1) {
      throw RuntimeException("predict_face_expression error");
    }

    return std::make_pair(result.faceExpressions_[0].type, result.faceExpressions_[0].score);
  }

  std::vector<std::vector<std::pair<ExpressionType, float>>> predict_face_expression_batch(
      const std::vector<cv::Mat> &images,
      const std::vector<std::vector<std::vector<int>>> &faces) {
    int cnt = images.size();
    std::vector<std::vector<std::pair<ExpressionType, float>>> expressionss;
    for (int i = 0; i < cnt; i++) {
      std::vector<std::pair<ExpressionType, float>> expressions;
      for (size_t k = 0; k < faces[i].size(); k++) {
        auto expression = predict_face_expression(images[i], faces[i][k]);
        expressions.emplace_back(expression);
      }
      expressionss.emplace_back(expressions);
    }

    return expressionss;
  }

  void set_face_noise_motionblur_classifier_model(const std::string &model_path) {
    faceNoiseMB_ = std::make_unique<arctern::FaceNoiseMotionblur>();

    InitParameter parameter;
    parameter.batchNum_ = 1;
    parameter.model_path = model_path;

    auto code = faceNoiseMB_->Init(&parameter);
    if (code != ErrorType::ERR_SUCCESS) {
      throw InitException("face_face_noise_motionblur init error");
    }
  }

  std::pair<NoiseType, float> predict_face_noise_motionblur(const cv::Mat &image,
                                                            const std::vector<int> &face) {
    ArcternImage img;
    convertMat2ArcternImage(image, img);
    arctern::FaceNoiseMotionblurRunParameter runParameter;
    runParameter.imageV_.emplace_back(img);
    ArcternRect rect{static_cast<float>(face[0]), static_cast<float>(face[1]), static_cast<float>(face[2]),
                     static_cast<float>(face[3])};
    runParameter.rectsV_.emplace_back(rect);
    arctern::FaceNoiseMotionblurResult result;
    auto code = faceNoiseMB_->Process(&runParameter, &result);

    if (code != ErrorType::ERR_SUCCESS || result.faceNoiseMotionblurs_.size() != 1) {
      throw RuntimeException("predict_face_noise_motionblur error");
    }

    return std::make_pair(result.faceNoiseMotionblurs_[0].type, result.faceNoiseMotionblurs_[0].score);
  }

  std::vector<std::vector<std::pair<NoiseType, float>>> predict_face_noise_motionblur_batch(
      const std::vector<cv::Mat> &images,
      const std::vector<std::vector<std::vector<int>>> &faces) {
    int cnt = images.size();
    std::vector<std::vector<std::pair<NoiseType, float>>> nmbss;
    for (int i = 0; i < cnt; i++) {
      std::vector<std::pair<NoiseType, float>> nmbs;
      for (size_t k = 0; k < faces[i].size(); k++) {
        auto nmb = predict_face_noise_motionblur(images[i], faces[i][k]);
        nmbs.emplace_back(nmb);
      }
      nmbss.emplace_back(nmbs);
    }

    return nmbss;
  }

  void set_face_abnomal_light_classifier_model(const std::string &model_path) {
    faceAbnormallight_ = std::make_unique<arctern::FaceAbnomalLight>();

    InitParameter parameter;
    parameter.batchNum_ = 1;
    parameter.model_path = model_path;

    auto code = faceAbnormallight_->Init(&parameter);
    if (code != ErrorType::ERR_SUCCESS) {
      throw InitException("face_face_abnormal light init error");
    }
  }

  std::pair<AbnomalLightType, float> predict_face_abnomal_light(const cv::Mat &image,
                                                                const std::vector<int> &face) {
    ArcternImage img;
    convertMat2ArcternImage(image, img);
    arctern::FaceAbnomalLightRunParameter runParameter;
    runParameter.imageV_.emplace_back(img);
    ArcternRect rect{static_cast<float>(face[0]), static_cast<float>(face[1]), static_cast<float>(face[2]),
                     static_cast<float>(face[3])};
    runParameter.rectsV_.emplace_back(rect);

    arctern::FaceAbnomalLightResult result;
    auto code = faceAbnormallight_->Process(&runParameter, &result);


    if (code != ErrorType::ERR_SUCCESS || result.faceAbnomalLights_.size() != 1) {
      throw RuntimeException("predict_face_abnormal light error");
    }

    return std::make_pair(result.faceAbnomalLights_[0].type, result.faceAbnomalLights_[0].score);
  }

  std::vector<std::vector<std::pair<AbnomalLightType, float>>> predict_face_abnomal_light_batch(
      const std::vector<cv::Mat> &images,
      const std::vector<std::vector<std::vector<int>>> &faces) {
    int cnt = images.size();
    std::vector<std::vector<std::pair<AbnomalLightType, float>>> ablss;
    for (int i = 0; i < cnt; i++) {
      std::vector<std::pair<AbnomalLightType, float>> abls;
      for (size_t k = 0; k < faces[i].size(); k++) {
        auto abl = predict_face_abnomal_light(images[i], faces[i][k]);
        abls.emplace_back(abl);
      }
      ablss.emplace_back(abls);
    }

    return ablss;
  }

  void set_faceocc_attributes_model(const std::string &model_path) {
    faceOcclusion_ = std::make_unique<arctern::FaceOcclusion>();

    InitParameter parameter;
    parameter.batchNum_ = 1;
    parameter.model_path = model_path;

    auto code = faceOcclusion_->Init(&parameter);
    if (code != ErrorType::ERR_SUCCESS) {
      throw InitException("face_face_occlusion detection init error");
    }
  }

  std::vector<std::pair<std::string, std::pair<int, float>>> predict_faceocc_attributes(
      const cv::Mat &image, const std::vector<int> &face) {
    /// \brief 人脸遮挡任务
    const std::vector<std::string> faceocc_attributes_task =
        {"Lefteye_open", "Righteye_open", "Lefteye_occ", "Righteye_occ", "Leftcheek_occ", "Rightcheeck_occ",
         "mouth_occ", "nose_occ", "chincontour_occ"};

    ArcternImage img;
    convertMat2ArcternImage(image, img);
    arctern::FaceOcclusionRunParameter runParameter;
    runParameter.imageV_.emplace_back(img);
    ArcternRect rect{static_cast<float>(face[0]), static_cast<float>(face[1]), static_cast<float>(face[2]),
                     static_cast<float>(face[3])};
    runParameter.rectsV_.emplace_back(rect);
    arctern::FaceOcclusionResult result;
    auto code = faceOcclusion_->Process(&runParameter, &result);
    //auto *gResult = dynamic_cast<const arctern::FaceOcclusionResult *>(result);

    if (code != ErrorType::ERR_SUCCESS || result.faceOcclusion_.size() != 1) {
      throw RuntimeException("predict_faceocc_attributes  error");
    }

    auto &singleFaceOcc = result.faceOcclusion_[0];

    using faceocc_attribue = std::pair<std::string, std::pair<int, float>>;

    faceocc_attribue
        leo = {faceocc_attributes_task[0], {singleFaceOcc.leftEyeOpen.first, singleFaceOcc.leftEyeOpen.second}};
    faceocc_attribue
        reo = {faceocc_attributes_task[1], {singleFaceOcc.rightEyeOpen.first, singleFaceOcc.rightEyeOpen.second}};
    faceocc_attribue
        leocc = {faceocc_attributes_task[2], {singleFaceOcc.leftEyeOcc.first, singleFaceOcc.leftEyeOcc.second}};
    faceocc_attribue
        reocc = {faceocc_attributes_task[3], {singleFaceOcc.rightEyeOcc.first, singleFaceOcc.rightEyeOcc.second}};
    faceocc_attribue
        lco = {faceocc_attributes_task[4], {singleFaceOcc.leftCheekOcc.first, singleFaceOcc.leftCheekOcc.second}};
    faceocc_attribue
        rco = {faceocc_attributes_task[5], {singleFaceOcc.rightCheekOcc.first, singleFaceOcc.rightCheekOcc.second}};
    faceocc_attribue mo = {faceocc_attributes_task[6], {singleFaceOcc.mouthOcc.first, singleFaceOcc.mouthOcc.second}};
    faceocc_attribue no = {faceocc_attributes_task[7], {singleFaceOcc.noseOcc.first, singleFaceOcc.noseOcc.second}};
    faceocc_attribue
        co = {faceocc_attributes_task[8], {singleFaceOcc.chincontourOcc.first, singleFaceOcc.chincontourOcc.second}};

    std::vector<std::pair<std::string, std::pair<int, float>>> ret = {leo, reo, leocc, reocc, lco, rco, mo, no, co};

    return ret;
  }

  std::vector<std::vector<std::vector<std::pair<std::string, std::pair<int, float>>>>> predict_faceocc_attributes_batch(
      const std::vector<cv::Mat> &images,
      const std::vector<std::vector<std::vector<int>>> &faces) {
    int cnt = images.size();
    std::vector<std::vector<std::vector<std::pair<std::string, std::pair<int, float>>>>> faceoccss;
    for (int i = 0; i < cnt; i++) {
      std::vector<std::vector<std::pair<std::string, std::pair<int, float>>>> faceoccs;
      for (size_t k = 0; k < faces[i].size(); k++) {
        auto faceocc = predict_faceocc_attributes(images[i], faces[i][k]);
        faceoccs.emplace_back(faceocc);
      }
      faceoccss.emplace_back(faceoccs);
    }

    return faceoccss;
  }

  void set_face_feat_model(const std::string &model_path) {
    faceFeat_ = std::make_unique<arctern::FaceGluon>();

    InitParameter parameter;
    parameter.batchNum_ = 1;
    parameter.model_path = model_path;

    auto code = faceFeat_->Init(&parameter);
    if (code != ErrorType::ERR_SUCCESS) {
      throw InitException("face_feat  init error");
    }
  }

  std::vector<uint8> get_face_feat(const cv::Mat &image,
                                   const std::vector<int> &face,
                                   const std::vector<float> landmark = std::vector<float>()) {
    ArcternImage img;
    convertMat2ArcternImage(image, img);
    arctern::FaceGluonRunParameter runParameter;
    runParameter.imageV_.emplace_back(img);
    ArcternRect rect{static_cast<float>(face[0]), static_cast<float>(face[1]), static_cast<float>(face[2]),
                     static_cast<float>(face[3])};
    runParameter.rectsV_.emplace_back(rect);

    int len = landmark.size();
    int idx = 0;
    GluonLandmarks landmarks;
    while (idx < len) {
      ArcternPoint point = {landmark[idx], landmark[idx + 1]};
      landmarks.emplace_back(point);
      idx += 2;
    };

    runParameter.faceLandmarksV_.emplace_back(landmarks);

    arctern::FaceGluonResult result;
    auto code = faceFeat_->Process(&runParameter, &result);

    if (code != ErrorType::ERR_SUCCESS || result.features_.size() != 1) {
      throw RuntimeException("predict_face_featrue  error");
    }

    return result.features_[0];

  }

  std::vector<std::vector<std::vector<uint8>>> get_face_feat_batch(
      const std::vector<cv::Mat> &images,
      const std::vector<std::vector<std::vector<int>>> &faces,
      const std::vector<std::vector<std::vector<float>>> &landmarks) {
    int cnt = images.size();

    std::vector<std::vector<std::vector<uint8>>> featss;
    for (int i = 0; i < cnt; i++) {
      std::vector<std::vector<uint8>> feats;
      for (size_t k = 0; k < faces[i].size(); k++) {
        auto feat = landmarks.empty() ? get_face_feat(images[i], faces[i][k]) : get_face_feat(images[i],
                                                                                              faces[i][k],
                                                                                              landmarks[i][k]);
        feats.emplace_back(feat);
      }
      featss.emplace_back(feats);
    }

    return featss;
  }

  void set_person_feat_model(const std::string &model_path) {
    personFeat_ = std::make_unique<arctern::PersonFeat>();

    InitParameter parameter;
    parameter.batchNum_ = 1;
    parameter.model_path = model_path;

    auto code = personFeat_->Init(&parameter);
    if (code != ErrorType::ERR_SUCCESS) {
      throw InitException("set_person_feat init error");
    }
  }

  std::vector<uint8> get_pedestrian_feat(const cv::Mat &image,
                                         const std::vector<int> &person) {
    ArcternImage img;
    convertMat2ArcternImage(image, img);
    arctern::PersonFeatRunParameter runParameter;
    runParameter.imageV_.emplace_back(img);
    ArcternRect rect{static_cast<float>(person[0]), static_cast<float>(person[1]), static_cast<float>(person[2]),
                     static_cast<float>(person[3])};
    runParameter.rectsV_.emplace_back(rect);

    arctern::PersonFeatResult result;
    auto code = personFeat_->Process(&runParameter, &result);

    if (code != ErrorType::ERR_SUCCESS || result.features_.size() != 1) {
      throw RuntimeException("predict_person_featrue  error");
    }

    return result.features_[0];
  }

  std::vector<std::vector<std::vector<uint8>>> get_pedestrian_feat_batch(
      const std::vector<cv::Mat> &images,
      const std::vector<std::vector<std::vector<int>>> &persons) {
    int cnt = images.size();

    std::vector<std::vector<std::vector<uint8>>> featss;
    for (int i = 0; i < cnt; i++) {
      std::vector<std::vector<uint8>> feats;
      for (size_t k = 0; k < persons[i].size(); k++) {
        auto feat = get_pedestrian_feat(images[i], persons[i][k]);
        feats.emplace_back(feat);
      }
      featss.emplace_back(feats);
    }

    return featss;
  }

  void set_face_basicmultiattribute_model(const std::string &model_path) {
    faceBasicMultiAttr_ = std::make_unique<arctern::FaceBasicMultiAttr>();

    InitParameter parameter;
    parameter.batchNum_ = 1;
    parameter.model_path = model_path;

    auto code = faceBasicMultiAttr_->Init(&parameter);
    if (code != ErrorType::ERR_SUCCESS) {
      throw InitException("set_face_basicmultiAttr init error");
    }
  }

  FaceBasicMultiAttribute predict_facebasic_multiattributes(
      const cv::Mat &image,
      const std::vector<int> &face) {
    ArcternImage img;
    convertMat2ArcternImage(image, img);
    arctern::FaceBasicMultiAttrRunParameter runParameter;
    runParameter.imageV_.emplace_back(img);
    ArcternRect rect{static_cast<float>(face[0]), static_cast<float>(face[1]), static_cast<float>(face[2]),
                     static_cast<float>(face[3])};
    runParameter.rectsV_.emplace_back(rect);

    arctern::FaceBasicMultiAttrResult result;
    auto code = faceBasicMultiAttr_->Process(&runParameter, &result);

    if (code != ErrorType::ERR_SUCCESS || result.faceBasicMultiAttrs_.size() != 1) {
      throw RuntimeException("predict_facebasic_multiattributes  error");
    }

    auto &attr = result.faceBasicMultiAttrs_[0];
    FaceBasicMultiAttribute attribute;
    attribute.age = std::make_pair(attr.age, attr.ageScore);
    attribute.gender = std::make_pair(static_cast<int>(attr.gender), attr.genderScore);
    attribute.glasses = std::make_pair(static_cast<int>(attr.glasses), attr.glassesScore);
    attribute.mask = std::make_pair(static_cast<int>(attr.mask), attr.maskScore);
    attribute.headwear = std::make_pair(static_cast<int>(attr.headwear), attr.headwearScore);

    return attribute;
  }
  std::vector<std::vector<FaceBasicMultiAttribute>> predict_facebasic_multiattributes_batch(
      const std::vector<cv::Mat> &images,
      const std::vector<std::vector<std::vector<int>>> &faces) {
    int cnt = images.size();

    std::vector<std::vector<FaceBasicMultiAttribute>> attrss;
    for (int i = 0; i < cnt; i++) {
      std::vector<FaceBasicMultiAttribute> attrs;
      for (size_t k = 0; k < faces[i].size(); k++) {
        auto attr = predict_facebasic_multiattributes(images[i], faces[i][k]);
        attrs.emplace_back(attr);
      }
      attrss.emplace_back(attrs);
    }

    return attrss;
  }

  void set_retina_face_det_model(
      const std::string &model_path, const RetinafaceParameter &param = RetinafaceParameter()) {
    faceDetRTN_ = std::make_unique<arctern::FaceDetectRTN>();

    InitParameter parameter;
    parameter.batchNum_ = 1;
    parameter.model_path = model_path;
    parameter.thresh_ = param.conf_thresh;

    auto code = faceDetRTN_->Init(&parameter);
    if (code != ErrorType::ERR_SUCCESS) {
      throw InitException("retina face init error");
    }
  }

  std::vector<std::pair<std::vector<int>, float>> detect_retinaface(const cv::Mat &image) {
    ArcternImage img;
    convertMat2ArcternImage(image, img);
    arctern::FaceDetectRTNRunParameter runParameter;
    runParameter.imageV_.emplace_back(img);

    arctern::FaceDetectRTNResult result;
    auto code = faceDetRTN_->Process(&runParameter, &result);

    if (code != ErrorType::ERR_SUCCESS || result.detInfos_.size() != 1) {
      throw RuntimeException("detect_retinaface  error");
    }

    auto &detInfo = result.detInfos_[0];
    std::vector<std::pair<std::vector<int>, float>> ret;

    std::transform(detInfo.begin(),
                   detInfo.end(),
                   std::back_inserter(ret),
                   [](const arctern::FACE_DETECT_RTN_INFO &info) {
                     std::vector<int> vecr = {info.rect.x, info.rect.y, info.rect.width, info.rect.height};
                     return std::make_pair(vecr, info.score);
                   });

    return ret;
  }

  std::vector<std::vector<std::pair<std::vector<int>, float>>>
  detect_retinaface_batch(const std::vector<cv::Mat> &images) {
    int cnt = images.size();

    std::vector<std::vector<std::pair<std::vector<int>, float>>> detss;
    for (int i = 0; i < cnt; i++) {
      detss.emplace_back(detect_retinaface(images[i]));
    }

    return detss;
  }

  void set_face_pose_quality_model(const std::string &model_path) {
    faceQualityPose_ = std::make_unique<arctern::FaceQualityPose>();

    InitParameter parameter;
    parameter.batchNum_ = 1;
    parameter.model_path = model_path;

    auto code = faceQualityPose_->Init(&parameter);
    if (code != ErrorType::ERR_SUCCESS) {
      throw InitException("set_face_pose_quality_model init error");
    }
  }

  std::pair<float, std::vector<float>> predict_face_quality_pose(
      const cv::Mat &image, const std::vector<int> &face) {
    ArcternImage img;
    convertMat2ArcternImage(image, img);
    arctern::FaceQualityPoseRunParameter runParameter;
    runParameter.imageV_.emplace_back(img);
    ArcternRect rect{static_cast<float>(face[0]), static_cast<float>(face[1]), static_cast<float>(face[2]),
                     static_cast<float>(face[3])};
    runParameter.rectsV_.emplace_back(rect);

    arctern::FaceQualityPoseResult result;
    auto code = faceQualityPose_->Process(&runParameter, &result);

    if (code != ErrorType::ERR_SUCCESS || result.faceQualityPose_.size() != 1) {
      throw RuntimeException("predict_face_quality_pose  error");
    }

    auto &attr = result.faceQualityPose_[0];
    std::vector<float> pose = {attr.pitch, attr.yaw, attr.roll};
    return std::make_pair(attr.quality, pose);
  }

  std::vector<std::vector<std::pair<float, std::vector<float>>>>
  predict_face_quality_pose_batch(const std::vector<cv::Mat> &images,
                                  const std::vector<std::vector<std::vector<int>>> &faces) {
    int cnt = images.size();

    std::vector<std::vector<std::pair<float, std::vector<float>>>> attrss;
    for (int i = 0; i < cnt; i++) {
      std::vector<std::pair<float, std::vector<float>>> attrs;
      for (size_t k = 0; k < faces[i].size(); k++) {
        auto attr = predict_face_quality_pose(images[i], faces[i][k]);
        attrs.emplace_back(attr);
      }
      attrss.emplace_back(attrs);
    }

    return attrss;
  }

  void set_person_quality_predictor(const std::string &model_path) {
    personQuality_ = std::make_unique<arctern::PersonQuality>();

    InitParameter parameter;
    parameter.batchNum_ = 1;
    parameter.model_path = model_path;

    auto code = personQuality_->Init(&parameter);
    if (code != ErrorType::ERR_SUCCESS) {
      throw InitException("set_face_pose_quality_model init error");
    }
  }

  float predict_person_quality(const cv::Mat &image, const std::vector<int> &person) {
    ArcternImage img;
    convertMat2ArcternImage(image, img);
    arctern::PersonQualityRunParameter runParameter;
    runParameter.imageV_.emplace_back(img);
    ArcternRect rect{static_cast<float>(person[0]), static_cast<float>(person[1]), static_cast<float>(person[2]),
                     static_cast<float>(person[3])};
    runParameter.rectsV_.emplace_back(rect);

    arctern::PersonQualityResult result;
    auto code = personQuality_->Process(&runParameter, &result);

    if (code != ErrorType::ERR_SUCCESS || result.personQuality_.size() != 1) {
      throw RuntimeException("predict_person_quality  error");
    }

    return result.personQuality_[0];
  }

  std::vector<std::vector<float>> predict_person_quality_batch(
      const std::vector<cv::Mat> &images,
      const std::vector<std::vector<std::vector<int>>> &person) {
    int cnt = images.size();

    std::vector<std::vector<float>> attrss;
    for (int i = 0; i < cnt; i++) {
      std::vector<float> attrs;
      for (size_t k = 0; k < person[i].size(); k++) {
        auto attr = predict_person_quality(images[i], person[i][k]);
        attrs.emplace_back(attr);
      }
      attrss.emplace_back(attrs);
    }

    return attrss;

  }

  void set_facial_landmark_model(const std::string &model_path) {
    facial_landmark_ = std::make_unique<arctern::FacialLandmark>();
    InitParameter parameter;
    parameter.batchNum_ = 1;
    parameter.model_path = model_path;
    auto code = facial_landmark_->Init(&parameter);

    if (code != ErrorType::ERR_SUCCESS) {
      throw InitException("set_face_pose_quality_model init error");
    }
  }

  std::vector<float> predict_facial_landmark(
      const cv::Mat &image, const std::vector<int> &face) {
    ArcternImage img;
    convertMat2ArcternImage(image, img);
    arctern::FacialLandmarkRunParameter run_parameter;
    run_parameter.imageV_.emplace_back(img);
    ArcternRect rect{static_cast<float>(face[0]), static_cast<float>(face[1]), static_cast<float>(face[2]),
                     static_cast<float>(face[3])};
    run_parameter.rectsV_.emplace_back(rect);
    arctern::FacialLandmarkResult result;
    auto code = facial_landmark_->Process(&run_parameter, &result);

    if (code != ErrorType::ERR_SUCCESS || result.landmarks_.size() != 1) {
      throw RuntimeException("predict_facial_landmark  error");
    }
    std::vector<float> landmarks;
    for (auto &p : result.landmarks_[0]) {
      landmarks.push_back(p.x);
      landmarks.push_back(p.y);
    }
    return landmarks;
  }

  std::vector<std::vector<std::vector<float>>>
  predict_facial_landmark_batch(const std::vector<cv::Mat> &images,
                                const std::vector<std::vector<std::vector<int>>> &faces) {
    int cnt = images.size();

    std::vector<std::vector<std::vector<float>>> landmarkss;
    for (int i = 0; i < cnt; i++) {
      std::vector<std::vector<float>> landmarks;
      for (size_t k = 0; k < faces[i].size(); k++) {
        auto landmark = predict_facial_landmark(images[i], faces[i][k]);
        landmarks.emplace_back(landmark);
      }
      landmarkss.emplace_back(landmarks);
    }

    return landmarkss;
  }

  void set_person_completeness_model(const std::string& model_path) {
    person_completeness_ = std::make_unique<arctern::PersonCompleteness>();
    InitParameter parameter;
    parameter.batchNum_ = 1;
    parameter.model_path = model_path;
    auto code = person_completeness_->Init(&parameter);

    if (code != ErrorType::ERR_SUCCESS) {
      throw InitException("set_face_pose_quality_model init error");
    }
  }

  PersonCompletenessInfo predict_person_completeness(const cv::Mat& image, const std::vector<int>& person) {
    ArcternImage img;
    convertMat2ArcternImage(image, img);
    arctern::PersonCompletenessRunParameter run_parameter;
    run_parameter.imageV_.emplace_back(img);
    ArcternRect rect{static_cast<float>(person[0]), static_cast<float>(person[1]), static_cast<float>(person[2]),
                     static_cast<float>(person[3])};
    run_parameter.rectsV_.emplace_back(rect);
    arctern::PersonCompletenessResult result;
    auto code = person_completeness_->Process(&run_parameter, &result);

    if (code != ErrorType::ERR_SUCCESS || result.personCompleteness_.size() != 1) {
      throw RuntimeException("predict_facial_landmark  error");
    }

//    typedef struct  {
//      std::vector<float> truncation;  ///< 人体完整度
//      std::vector<std::pair<bool, float>> visible_parts;   ///< 人体部分的可视信息，包括头、胸部、腹部、大腿、小腿以及它们的置信度
//      std::vector<std::vector<std::pair<bool, float>>> soft_mask;  ///< 人体分成16*8个网格，描述其是否可视以及置信度
//    }PersonCompletenessInfo;

    PersonCompletenessInfo info;
    info.truncation = result.personCompleteness_[0].truncation;
    int len = result.personCompleteness_[0].visibleParts.size();
    for(int i = 0 ; i < len; i++) {
      info.visible_parts.emplace_back(std::make_pair(result.personCompleteness_[0].visibleParts[i],
                                                     result.personCompleteness_[0].visiblePartsScore[i]));
    }

    len = result.personCompleteness_[0].softMasks.size();
    for(int i = 0 ; i < len; i++) {
      std::vector<std::pair<bool, float>> mask;
      int k_size = result.personCompleteness_[0].softMasks[i].size();
      for(int k = 0 ; k < k_size; k++ ) {
        mask.emplace_back(std::make_pair(result.personCompleteness_[0].softMasks[i][k],
                                         result.personCompleteness_[0].softMasksScore[i][k]));
      }
      info.soft_mask.emplace_back(mask);
    }
    return info;
  }

  std::vector<std::vector<PersonCompletenessInfo>> predict_person_completeness_batch(
      const std::vector<cv::Mat>& images,
      const std::vector<std::vector<std::vector<int>>>& person) {
    int cnt = images.size();

    std::vector<std::vector<PersonCompletenessInfo>> infoss;
    for (int i = 0; i < cnt; i++) {
      std::vector<PersonCompletenessInfo> infos;
      for (size_t k = 0; k < person[i].size(); k++) {
        auto info = predict_person_completeness(images[i], person[i][k]);
        infos.emplace_back(info);
      }
      infoss.emplace_back(infos);
    }
    return infoss;
  }

 private:
  ArcternManager() {}

 private:
  std::unique_ptr<arctern::FaceDetectTiny> faceDetTiny_;
  std::unique_ptr<arctern::AgeGender> ageGender_;
  std::unique_ptr<arctern::FaceGlass> faceGlass_;
  std::unique_ptr<arctern::FaceMask> faceMask_;
  std::unique_ptr<arctern::FaceQuality> faceQuality_;
  std::unique_ptr<arctern::FacePose> facePose_;
  std::unique_ptr<arctern::FaceHat> faceHat_;
  std::unique_ptr<arctern::FaceExpression> faceExpression_;
  std::unique_ptr<arctern::FaceNoiseMotionblur> faceNoiseMB_;
  std::unique_ptr<arctern::FaceAbnomalLight> faceAbnormallight_;
  std::unique_ptr<arctern::FaceOcclusion> faceOcclusion_;
  std::unique_ptr<arctern::FaceGluon> faceFeat_;
  std::unique_ptr<arctern::PersonFeat> personFeat_;
  std::unique_ptr<arctern::FaceBasicMultiAttr> faceBasicMultiAttr_;
  std::unique_ptr<arctern::FaceDetectRTN> faceDetRTN_;
  std::unique_ptr<arctern::PersonQuality> personQuality_;
  std::unique_ptr<arctern::FaceQualityPose> faceQualityPose_;
  std::unique_ptr<arctern::FacialLandmark> facial_landmark_;
  std::unique_ptr<arctern::PersonCompleteness> person_completeness_;
};

PYBIND11_MODULE(arcternbase, m) {
  NDArrayConverter::init_numpy();

  m.def("read_image", &read_image, "A function that read an image",
        py::arg("image"));

  m.def("compute_similarity", &compute_similarity,
        "Computes the similarity of two encrypted features",
        py::arg("feat1"), py::arg("feat2"));

  py::class_<FaceAttribute>(m, "FaceAttribute")
      .def(py::init<>())
      .def_readwrite("gender", &FaceAttribute::gender)
      .def_readwrite("age", &FaceAttribute::age);

  py::class_<FaceBasicMultiAttribute>(m, "FaceBasicMultiAttribute")
      .def(py::init<>())
      .def_readwrite("gender", &FaceBasicMultiAttribute::gender)
      .def_readwrite("glasses", &FaceBasicMultiAttribute::glasses)
      .def_readwrite("mask", &FaceBasicMultiAttribute::mask)
      .def_readwrite("headwear", &FaceBasicMultiAttribute::headwear)
      .def_readwrite("age", &FaceBasicMultiAttribute::age);

  py::enum_<Glass>(m, "Glass")
      .value("NOGLASS", Glass::NOGLASS)
      .value("TYPE1", Glass::TYPE1)
      .value("TYPE2", Glass::TYPE2);

  py::enum_<Hat>(m, "Hat")
      .value("NOHAT", Hat::NOHAT)
      .value("DETECTED", Hat::DETECTED);

  py::enum_<NoiseType>(m, "NoiseType")
      .value("MOTIONBLUR", NoiseType::MOTIONBLUR)
      .value("NOISY", NoiseType::NOISY)
      .value("NORMAL", NoiseType::NORMAL);

  py::enum_<ExpressionType>(m, "ExpressionType")
      .value("ANGRY", ExpressionType::ANGRY)
      .value("DISGUST", ExpressionType::DISGUST)
      .value("FEAR", ExpressionType::FEAR)
      .value("HAPPY", ExpressionType::HAPPY)
      .value("SAD", ExpressionType::SAD)
      .value("SURPRISE", ExpressionType::SURPRISE)
      .value("NEUTRAL_EXP", ExpressionType::NEUTRAL_EXP);

  py::enum_<AbnomalLightType>(m, "AbnomalLightType")
      .value("BACK", AbnomalLightType::BACK)
      .value("FRONT", AbnomalLightType::FRONT)
      .value("DARK", AbnomalLightType::DARK)
      .value("SIDE", AbnomalLightType::SIDE)
      .value("NORMAL_LIGHT", AbnomalLightType::NORMAL_LIGHT);

  py::class_<FaceDetectorYolov3Parameter>(m, "FaceDetectorYolov3Parameter")
      .def(py::init<>())
      .def_readwrite("conf_thresh", &FaceDetectorYolov3Parameter::conf_thresh);

  py::class_<RetinafaceParameter>(m, "RetinafaceParameter")
      .def(py::init<>())
      .def_readwrite("conf_thresh", &RetinafaceParameter::conf_thresh)
      .def_readwrite("using_landmark", &RetinafaceParameter::using_landmark);

  py::class_<PersonCompletenessInfo>(m, "PersonCompletenessInfo")
      .def(py::init<>())
      .def_readwrite("truncation", &PersonCompletenessInfo::truncation)
      .def_readwrite("visible_parts", &PersonCompletenessInfo::visible_parts)
      .def_readwrite("soft_mask", &PersonCompletenessInfo::soft_mask);

  py::class_<ArcternManager>(m, "ArcternManager")
      .def(py::init(&ArcternManager::create))
      .def("set_face_tiny_det_model",
           &ArcternManager::set_face_tiny_det_model,
           "sets face tiny det model",
           py::arg("model_path"),
           py::arg("param"))
      .def("detect_face_tiny",
           &ArcternManager::detect_face_tiny, "A function to detect face using yolo tiny model", py::arg("image"))
      .def("detect_face_tiny_batch",
           &ArcternManager::detect_face_tiny_batch,
           "A function to detect face using yolo tiny model", py::arg("images"))
      .def("set_face_gender_age_model",
           &ArcternManager::set_face_gender_age_model,
           "Sets face gender-age model",
           py::arg("model_path"))
      .def("predict_face_gender_age",
           &ArcternManager::predict_face_gender_age,
           "A function to predict face gender age",
           py::arg("image"), py::arg("face"))
      .def("predict_face_gender_age_batch",
           &ArcternManager::predict_face_gender_age_batch,
           "A function to predict face gender age", py::arg("images"), py::arg("faces"))
      .def("set_face_glass_model", &ArcternManager::set_face_glass_model,
           "Sets face glass model",
           py::arg("model_path"))
      .def("predict_face_glass",
           &ArcternManager::predict_face_glass,
           "A function to predict face glass", py::arg("image"),
           py::arg("face"))
      .def("predict_face_glass_batch",
           &ArcternManager::predict_face_glass_batch,
           "A function to predict face glass", py::arg("images"), py::arg("faces"))
      .def("set_face_mask_model", &ArcternManager::set_face_mask_model,
           "Sets face mask model",
           py::arg("model_path"))
      .def("predict_face_mask",
           &ArcternManager::predict_face_mask,
           "A function to predict face mask", py::arg("image"), py::arg("face"))
      .def("predict_face_mask_batch",
           &ArcternManager::predict_face_mask_batch,
           "A function to predict face mask", py::arg("images"), py::arg("faces"))
      .def("set_face_quality_model",
           &ArcternManager::set_face_quality_model,
           "Sets face quality model",
           py::arg("model_path"))
      .def("predict_face_quality",
           &ArcternManager::predict_face_quality,
           "A function to predict face quality",
           py::arg("image"), py::arg("face"))
      .def("predict_face_quality_batch",
           &ArcternManager::predict_face_quality_batch,
           "A function to predict face quality", py::arg("images"), py::arg("faces"))
      .def("set_face_pose_cls_model",
           &ArcternManager::set_face_pose_cls_model,
           "Sets face pose classification model",
           py::arg("model_path"))
      .def("predict_face_pose_cls",
           &ArcternManager::predict_face_pose_cls,
           "A function to predict face pose classification",
           py::arg("image"), py::arg("face"))
      .def("predict_face_pose_cls_batch",
           &ArcternManager::predict_face_pose_cls_batch,
           "A function to predict face pose classification",
           py::arg("images"), py::arg("faces"))
      .def("set_face_hat_model", &ArcternManager::set_face_hat_model,
           "Sets face hat model",
           py::arg("model_path"))
      .def("predict_face_hat",
           &ArcternManager::predict_face_hat,
           "A function to predict face hat", py::arg("image"), py::arg("face"))
      .def("predict_face_hat_batch",
           &ArcternManager::predict_face_hat_batch,
           "A function to predict face hat", py::arg("images"), py::arg("faces"))
      .def("set_face_expression_recognization_model",
           &ArcternManager::set_face_expression_recognization_model,
           "Sets face expression recognization model",
           py::arg("model_path"))
      .def("predict_face_expression",
           &ArcternManager::predict_face_expression,
           "A function to predict face expression",
           py::arg("image"), py::arg("face"))
      .def("predict_face_expression_batch",
           &ArcternManager::predict_face_expression_batch,
           "A function to predict face expression", py::arg("images"), py::arg("faces"))
      .def("set_face_noise_motionblur_classifier_model",
           &ArcternManager::set_face_noise_motionblur_classifier_model,
           "Sets single face noise motionblur model",
           py::arg("model_path"))
      .def("predict_face_noise_motionblur",
           &ArcternManager::predict_face_noise_motionblur,
           "A function to predict face noise motionblur",
           py::arg("image"), py::arg("face"))
      .def("predict_face_noise_motionblur_batch",
           &ArcternManager::predict_face_noise_motionblur_batch,
           "A function to predict face noise motionblur", py::arg("images"), py::arg("faces"))
      .def("set_face_abnomal_light_classifier_model",
           &ArcternManager::set_face_abnomal_light_classifier_model,
           "Sets face abnormal light classifier model")
      .def("predict_face_abnomal_light",
           &ArcternManager::predict_face_abnomal_light,
           "A function to predict face abnormal light", py::arg("image"), py::arg("face"))
      .def("predict_face_abnomal_light_batch",
           &ArcternManager::predict_face_abnomal_light_batch,
           "A function to predict face abnormal light", py::arg("images"), py::arg("faces"))
      .def("set_faceocc_attributes_model",
           &ArcternManager::set_faceocc_attributes_model,
           "Sets faceocc attributes model",
           py::arg("model_path"))
      .def("predict_faceocc_attributes",
           &ArcternManager::predict_faceocc_attributes,
           "A function to predict faceocc attributes", py::arg("image"), py::arg("face"))
      .def("predict_faceocc_attributes_batch",
           &ArcternManager::predict_faceocc_attributes_batch,
           "A function to predict faceocc attributes", py::arg("images"), py::arg("faces"))
      .def("set_face_feat_model", &ArcternManager::set_face_feat_model,
           "Sets face features model",
           py::arg("model_path"))
      .def("get_face_feat",
           &ArcternManager::get_face_feat,
           "A function to get face feature",
           py::arg("image"), py::arg("face"), py::arg("landmark") = std::vector<float>())
      .def("get_face_feat_batch",
           &ArcternManager::get_face_feat_batch,
           "A function to get face feature",
           py::arg("images"), py::arg("faces"),
           py::arg("landmarks") = std::vector<std::vector<float>>())
      .def("set_person_feat_model", &ArcternManager::set_person_feat_model,
           "Sets person features model",
           py::arg("model_path"))
      .def("get_pedestrian_feat",
           &ArcternManager::get_pedestrian_feat,
           "A function to get pedestrian feature", py::arg("image"),
           py::arg("person"))
      .def("get_pedestrian_feat_batch",
           &ArcternManager::get_pedestrian_feat_batch,
           "A function to get pedestrian feature", py::arg("images"), py::arg("persons"))
      .def("set_face_basicmultiattribute_model", &ArcternManager::set_face_basicmultiattribute_model,
           "set face basicmultiattribute model",
           py::arg("model_path"))
      .def("predict_facebasic_multiattributes",
           &ArcternManager::predict_facebasic_multiattributes,
           "A function to predict facebasic multiattributes "
           "", py::arg("image"),
           py::arg("face"))
      .def("predict_facebasic_multiattributes_batch",
           &ArcternManager::predict_facebasic_multiattributes_batch,
           "A function to predict facebasic multiattributes", py::arg("images"), py::arg("faces"))
      .def("set_retina_face_det_model",
           &ArcternManager::set_retina_face_det_model,
           "Sets retinaface detect models",
           py::arg("model_path"), py::arg("param"))
      .def("detect_retinaface",
           &ArcternManager::detect_retinaface,
           "A function to retina detect face", py::arg("image"))
      .def("detect_retinaface_batch",
           &ArcternManager::detect_retinaface_batch,
           "A function to retina detect face batch", py::arg("images"))
      .def("set_face_pose_quality_model",
           &ArcternManager::set_face_pose_quality_model,
           "Sets face pose quality model",
           py::arg("model_path"))
      .def("predict_face_quality_pose",
           &ArcternManager::predict_face_quality_pose,
           "A function to predict face quality pose", py::arg("image"),
           py::arg("face"))
      .def("predict_face_quality_pose_batch",
           &ArcternManager::predict_face_quality_pose_batch,
           "A function to predict face quality pose", py::arg("images"), py::arg("faces"))
      .def("set_person_quality_predictor", &ArcternManager::set_person_quality_predictor,
           "Sets person quality predictor",
           py::arg("model_path"))
      .def("predict_person_quality",
           &ArcternManager::predict_person_quality,
           "A function to predict person quality", py::arg("image"), py::arg("person"))
      .def("predict_person_quality_batch",
           &ArcternManager::predict_person_quality_batch,
           "A function to predict person quality", py::arg("images"), py::arg("person"))
      .def("set_facial_landmark_model", &ArcternManager::set_facial_landmark_model,
           "Sets facial landmark model",
           py::arg("model_path"))
      .def("predict_facial_landmark",
               &ArcternManager::predict_facial_landmark,
           "A function to predict facial landmark",
           py::arg("image"), py::arg("face"))
      .def("predict_facial_landmark_batch",
               &ArcternManager::predict_facial_landmark_batch,
           "A function to predict facial landmark",
           py::arg("images"), py::arg("faces"))
      .def("set_person_completeness_model", &ArcternManager::set_person_completeness_model,
           "Sets person completeness model",
           py::arg("model_path"))
      .def( "predict_person_completeness",
            &ArcternManager::predict_person_completeness,
            "A function to predict person completeness", py::arg("image"), py::arg("person"))
      .def( "predict_person_completeness_batch",
        &ArcternManager::predict_person_completeness_batch,
            "A function to predict person completeness", py::arg("images"), py::arg("person"))
           ;
}


