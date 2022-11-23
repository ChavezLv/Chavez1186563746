#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <arctern/aes_cryptor.h>
#include <arctern/arctern_manager.h>
#include <cryptopp/base64.h>
#include <chrono>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <string>
#include "ndarray_converter.h"
#include "utils.h"

namespace py = pybind11;

using arctern::AnalyzeResult;
using arctern::BikeDetection;
using arctern::DeviceType;
using arctern::ElectricBikeDetection;
using arctern::ErrorCode;
using arctern::FaceDetection;
using arctern::FaceDetectorParameter;
using arctern::FaceDetectorYolov3Parameter;
using arctern::MultiDetectorParameter;
using arctern::ConfigInfo;
using arctern::Gender;
using arctern::Glass;
using arctern::Hat;
using arctern::FaceAttribute;
using arctern::FaceAngle;
using arctern::FaceFeatParams;
using arctern::Parameter;
using arctern::ObjectTrackerParameter;
using arctern::FaceTrackerParameter;
using arctern::PedestrianTrackerParameter;
using arctern::TrackerState;
using arctern::Expression;
using arctern::ExpressionType;
using arctern::NoiseType;
using arctern::AbnomalLightType;
using arctern::AbnormalExposureType;
using arctern::PersonType;
using arctern::PersonCompletenessInfo;
using arctern::FaceInteractiveAction;
using arctern::FaceInteractiveLevel;
using arctern::FaceInteractiveParameter;
using arctern::FashionType;
using arctern::FashionStyleType;
using arctern::CameraOcclusionType;
using arctern::MultiCatType;
using arctern::MouthAction;
using arctern::SimilarlyMappingType;

const char PY_AES_KEY[] = "EB641ACB6D2C16E2493025AF4FE4CC47";

void show_image(cv::Mat image) {
  cv::imshow("image_from_Cpp", image);
  cv::waitKey(0);
}

cv::Mat read_image(std::string image_name) {
  cv::Mat image = cv::imread(image_name, CV_LOAD_IMAGE_COLOR);
  return image;
}

float compute_similarity(const std::vector<uint8>& feat1,
                         const std::vector<uint8>& feat2) {
  return arctern::ArcternManager::compute_similarity(feat1, feat2);
}

cv::Mat passthru(cv::Mat image) { return image; }

cv::Mat cloneimg(cv::Mat image) { return image.clone(); }

struct PersonDetection {
  std::vector<int> box;
  float confidence;
  float top_occluded_prob;
  float bottom_occluded_prob;
  PersonType type;
};

struct FashionDetection {
  std::vector<int> box;
  float confidence;
  FashionType type;
};

struct MultiCatDetection {
  std::vector<int> box;
  float confidence;
  MultiCatType type;
  float quality;
};

struct TrackerInfo {
  std::vector<int> rect;
  std::vector<std::vector<int>> rect_history;
  int id;
  bool from_det;
  TrackerState state;
  int frame_index;
};

struct TrackResult {
  int tracked_count;
  std::vector<TrackerInfo> tracked_infos;
  std::vector<TrackerInfo> tracking_infos;
};


class ArcternManager : public arctern::ArcternManager {
 public:
#ifdef WITH_LICENSE
  static ArcternManager create(const std::string& license_file) {
    return ArcternManager(license_file);
  }
#else
  static ArcternManager create() {
    return ArcternManager();
  }
#endif
  void set_face_det_model(
      const std::string& model_path,
      const FaceDetectorParameter& param = FaceDetectorParameter()) {
    arctern::ArcternManager::set_face_det_model(model_path, param);
  }

  void set_face_tiny_det_model(const std::string& model_path,
                               const FaceDetectorYolov3Parameter& param = FaceDetectorYolov3Parameter()) {
    arctern::ArcternManager::set_face_tiny_det_model(model_path, param);
  }

  void set_face_feat_model(const std::string& model_path,
                           const Parameter& param = Parameter()) {
    arctern::ArcternManager::set_face_feat_model(model_path, param);
  }

  void set_face_mask_model(const std::string& model_path,
                          const Parameter& param = Parameter()) {
    arctern::ArcternManager::set_face_mask_model(model_path, param);
  }


  void set_face_age_model(const std::string& model_path,
                          const Parameter& param = Parameter()) {
    arctern::ArcternManager::set_face_age_model(model_path, param);
  }

  void set_face_gender_model(const std::string& model_path,
                             const Parameter& param = Parameter()) {
    arctern::ArcternManager::set_face_gender_model(model_path, param);
  }

  void set_face_pose_quality_model(const std::string& model_path,
                                   const Parameter& param = Parameter()) {
    arctern::ArcternManager::set_face_pose_quality_model(model_path, param);
  }

  void set_face_glass_model(const std::string& model_path,
                            const Parameter& param = Parameter()) {
    arctern::ArcternManager::set_face_glass_model(model_path, param);
  }

  void set_face_expression_model(const std::string& model_path,
                                 const Parameter& param = Parameter()) {
    arctern::ArcternManager::set_face_expression_model(model_path, param);
  }

  void set_face_expression_recognization_model(
      const std::string& model_path, const Parameter& param = Parameter()) {
    arctern::ArcternManager::set_face_expression_recognization_model(
          model_path, param);
  }

  void set_face_hat_model(const std::string& model_path,
                          const Parameter& param = Parameter()) {
    arctern::ArcternManager::set_face_hat_model(model_path, param);
  }

  void set_face_quality_model(const std::string& model_path,
                              const Parameter& param = Parameter()) {
    arctern::ArcternManager::set_face_quality_model(model_path, param);
  }

  void set_face_landmark_model(const std::string& model_path,
                               const Parameter& param = Parameter()) {
    arctern::ArcternManager::set_face_landmark_model(model_path, param);
  }

  void set_facial_landmark_model(const std::string& model_path,
                                 const Parameter& param = Parameter()) {
    arctern::ArcternManager::set_facial_landmark_model(model_path,param);
  }

  void set_facial_landmark_tiny_7_model(const std::string& model_path,
                                        const Parameter& param = Parameter()) {
    arctern::ArcternManager::set_facial_landmark_tiny_7_model(model_path,param);
  }

  void set_facial_landmark_detect_model(const std::string& model_path,
                                 const Parameter& param = Parameter()) {
    arctern::ArcternManager::set_facial_landmark_detect_model(model_path,param);
  }

  void set_face_noise_motionblur_classifier_model(const std::string& model_path,
                                                  const Parameter& param = Parameter()) {
    arctern::ArcternManager::set_face_noise_motionblur_classifier_model(model_path, param);
  }

  void set_face_attributes_model(const std::string& model_path, 
                                 const Parameter& param = Parameter() ) {
    arctern::ArcternManager::set_face_attributes_model(model_path, param);
  }

  void set_face_gender_age_model(const std::string& model_path,
                                 const Parameter& param = Parameter()) {
    arctern::ArcternManager::set_face_gender_age_model(model_path, param);
  }
 
  void set_face_pose_cls_model(const std::string& model_path,
                               const Parameter& param = Parameter()) {
    arctern::ArcternManager::set_face_pose_cls_model(model_path, param);
  }

  void set_face_liveness_ir_model(const std::string& model_path,
                                  const Parameter& param = Parameter()) {
    arctern::ArcternManager::set_face_liveness_ir_model(model_path, param);
  }

  void set_face_image_color_model(const std::string& model_path,
                                  const Parameter& param = Parameter()) {
    arctern::ArcternManager::set_face_image_color_model(model_path, param);
  }

  void set_face_angle_model(const std::string& model_path,
                            const Parameter& param = Parameter()) {
    arctern::ArcternManager::set_face_angle_model(model_path, param);
  }

  void set_faceocc_attributes_model(const std::string& model_path,
                            const Parameter& param = Parameter()) {
    arctern::ArcternManager::set_faceocc_attributes_model(model_path, param);
  }

  void set_face_abnomal_light_classifier_model(const std::string& model_path,
                            const Parameter& param = Parameter()) {
    arctern::ArcternManager::set_face_abnomal_light_classifier_model(model_path, param);
  }

  void set_face_3dlandmark_pose_model(const std::string& model_path,
                            const Parameter& param = Parameter()) {
    arctern::ArcternManager::set_face_3dlandmark_pose_model(model_path, param);
  }

  void set_mouth_action_model(const std::string& model_path,
                            const Parameter& param = Parameter()) {
    arctern::ArcternManager::set_mouth_action_model(model_path, param);
  }

  void set_face_extendmultiattribute_model(const std::string& model_path,
                            const Parameter& param = Parameter()) {
    arctern::ArcternManager::set_face_extendmultiattribute_model(model_path, param);
  }

  void set_person_det_model(const std::string& model_path,
                            const Parameter& param = Parameter()) {
    arctern::ArcternManager::set_person_det_model(model_path, param);
  }

  void set_person_feat_model(const std::string& model_path,
                             const Parameter& param = Parameter()) {
    arctern::ArcternManager::set_person_feat_model(model_path, param);
  }

  void set_person_quality_predictor(const std::string& model_path,
                          const Parameter& param = Parameter()) {
    arctern::ArcternManager::set_person_quality_predictor(model_path, param);
  }

  void set_person_completeness_model(const std::string& model_path,
                                     const Parameter& param = Parameter()) {
    arctern::ArcternManager::set_person_completeness_model(model_path, param);
  }

  void set_person_attributes_model(const std::string& model_path,
                                   const Parameter& param = Parameter()) {
    arctern::ArcternManager::set_person_attributes_model(model_path, param);
  }

  void set_bike_det_model(const std::string& model_path,
                          const Parameter& param = Parameter()) {
    arctern::ArcternManager::set_bike_det_model(model_path, param);
  }

  void set_car_types_model(const std::string& model_path,
                           const Parameter& param = Parameter()) {
    arctern::ArcternManager::set_car_types_model(model_path, param);
  }

  void set_car_attributes_model(const std::string& model_path,
                                const Parameter& param = Parameter()) {
    arctern::ArcternManager::set_car_attributes_model(model_path, param);
  }

  void set_multi_cat_tiny_det_model(const std::string& model_path,
      const MultiDetectorParameter& param = MultiDetectorParameter()) {
    arctern::ArcternManager::set_multi_cat_tiny_det_model(model_path, param);
  }

  void set_multi_cat_det_center_model(const std::string& model_path,
      const MultiDetectorParameter& param = MultiDetectorParameter()) {
    arctern::ArcternManager::set_multi_cat_det_center_model(model_path, param);
  }

  void set_body_face_head_det_center_model(const std::string& model_path,
      const MultiDetectorParameter& param = MultiDetectorParameter()) {
    arctern::ArcternManager::set_body_face_head_det_center_model(model_path, param);
  }

  void set_fashion_det_model(const std::string& model_path,
                             const Parameter& param = Parameter()) {
    arctern::ArcternManager::set_fashion_det_model(model_path, param);
  }

  void set_fashionstyle_det_model(const std::string& model_path,
                                  const Parameter& param = Parameter()) {
    arctern::ArcternManager::set_fashionstyle_det_model(model_path, param);
  }

  void set_face_tracker_model(
      const std::string& det_model,
      const std::string& feat_model,
      const FaceTrackerParameter& param = FaceTrackerParameter()) {
    arctern::ArcternManager::set_face_tracker_model(det_model, feat_model, param);
  }

  void set_pedestrian_tracker_model(
      const std::string& det_model,
      const std::string& feat_model,
      const PedestrianTrackerParameter& param= PedestrianTrackerParameter()) {
    arctern::ArcternManager::set_pedestrian_tracker_model(det_model, feat_model, param);
  }

  void set_person_tiny_det_model(const std::string& model_path,
                                 const Parameter& param = Parameter()) {
    arctern::ArcternManager::set_person_tiny_det_model(model_path, param);
  }

  void set_face_interactive_model(const std::string& face_dect_model_path,
        const std::string& face_landmark_model_path, const FaceInteractiveParameter& param) {
    arctern::ArcternManager::set_face_interactive_model(face_dect_model_path, face_landmark_model_path, param);
  }

  void set_similarly_mapping(const std::vector<float>& threshold,
        SimilarlyMappingType type = SimilarlyMappingType::SIMAP_NORMAL) {
    arctern::ArcternManager::set_similarly_mapping(threshold, type);
  }

  float similarly_mapping(float raw_similarly) {
    return arctern::ArcternManager::similarly_mapping(raw_similarly);
  }

  std::vector<std::pair<std::vector<int>, float>> detect_face(
      const cv::Mat& image) {
    std::vector<std::pair<cv::Rect, float>> faces =
        arctern::ArcternManager::detect_face(image);
    std::vector<std::pair<std::vector<int>, float>> results;
    std::transform(faces.begin(), faces.end(), std::back_inserter(results),
                   [](const std::pair<cv::Rect, float>& face) {
                     const auto& r = face.first;
                     std::vector<int> vecr({r.x, r.y, r.width, r.height});
                     return std::make_pair(vecr, face.second);
                   });
    return results;
  }

  std::vector<std::vector<std::pair<std::vector<int>, float>>> detect_face_batch(
      const std::vector<cv::Mat>& images){
    auto faces = arctern::ArcternManager::detect_face(images);
    std::vector<std::vector<std::pair<std::vector<int>, float>>> results;
    for (auto& a : faces) {
      std::vector<std::pair<std::vector<int>, float>> res;
      std::transform(a.begin(), a.end(), std::back_inserter(res),
                     [](const std::pair<cv::Rect, float>& face) {
                       const auto& r = face.first;
                       std::vector<int> vecr({r.x, r.y, r.width, r.height});
                       return std::make_pair(vecr, face.second);
                     });
      results.emplace_back(res);
    }
    return results;
  }

  std::vector<std::pair<std::vector<int>, float>> detect_face_tiny(
      const cv::Mat& image) {
    std::vector<std::pair<cv::Rect, float>> faces =
        arctern::ArcternManager::detect_tiny_face(image);
    std::vector<std::pair<std::vector<int>, float>> results;
    std::transform(faces.begin(), faces.end(), std::back_inserter(results),
                   [](const std::pair<cv::Rect, float>& face) {
                     const auto& r = face.first;
                     std::vector<int> vecr({r.x, r.y, r.width, r.height});
                     return std::make_pair(vecr, face.second);
                   });
    return results;
  }

  std::vector<std::vector<std::pair<std::vector<int>, float>>> detect_face_tiny_batch(
      const std::vector<cv::Mat>& images){
    auto faces = arctern::ArcternManager::detect_tiny_face(images);
    std::vector<std::vector<std::pair<std::vector<int>, float>>> results;
    for (auto& a : faces) {
      std::vector<std::pair<std::vector<int>, float>> res;
      std::transform(a.begin(), a.end(), std::back_inserter(res),
                     [](const std::pair<cv::Rect, float>& face) {
                       const auto& r = face.first;
                       std::vector<int> vecr({r.x, r.y, r.width, r.height});
                       return std::make_pair(vecr, face.second);
                     });
      results.emplace_back(res);
    }
    return results;
  }

  std::pair<std::vector<float>, std::vector<float>> predict_face_landmark_pose(
      const cv::Mat& image, const std::vector<int>& face) {
    cv::Rect rect({face[0], face[1], face[2], face[3]});
    auto a = arctern::ArcternManager::predict_face_landmark_pose(image, rect);
    std::vector<float> pose({a.first[0], a.first[1], a.first[2]});
    return std::make_pair(pose, a.second);
  }

  std::vector<std::vector<std::pair<std::vector<float>, std::vector<float>>>>
      predict_face_landmark_pose_batch(const std::vector<cv::Mat>& images,
      const std::vector<std::vector<std::vector<int>>>& faces) {
    std::vector<std::vector<std::pair<std::vector<float>, std::vector<float>>>> results;
    std::vector<std::vector<cv::Rect>> rects;
    cv::Rect r;
    for (auto& a : faces) {
      std::vector<cv::Rect> rec;
      for (auto& b : a) {
        r = cv::Rect(b[0],b[1],b[2],b[3]);
        rec.emplace_back(r);
      }
      rects.emplace_back(rec);
    }
    auto poses = arctern::ArcternManager::predict_face_landmark_pose(images, rects);
    std::vector<float> pose;
    for (auto& a : poses) {
      std::vector<std::pair<std::vector<float>, std::vector<float>>> res; 
      for (auto& b : a) {
        pose = {b.first[0], b.first[1], b.first[2]};
        res.emplace_back(std::make_pair(pose,b.second));
      }
      results.emplace_back(res);
    }
    return results;
  }
 
 
  std::pair<bool, float>  predict_face_mask(const cv::Mat& image, const std::vector<int>& face) {
    cv::Rect rect({face[0], face[1], face[2], face[3]});
    return arctern::ArcternManager::predict_face_mask(image, rect);
  }

 std::vector<std::vector<std::pair<bool, float>>>  predict_face_mask_batch(
      const std::vector<cv::Mat>& images, 
      const std::vector<std::vector<std::vector<int>>>& faces) {
    std::vector<std::vector<cv::Rect>> rects;
    cv::Rect r;
    for (auto& a : faces) {
      std::vector<cv::Rect> rec;
      for (auto& b : a) {
        r = cv::Rect(b[0],b[1],b[2],b[3]);
        rec.emplace_back(r);
      }
      rects.emplace_back(rec);
    }
    return arctern::ArcternManager::predict_face_mask(images, rects);
  }


  float predict_face_age(const cv::Mat& image, const std::vector<int>& face) {
    cv::Rect rect({face[0], face[1], face[2], face[3]});
    return arctern::ArcternManager::predict_face_age(image, rect);
  }

  std::vector<std::vector<float>> predict_face_age_batch(
      const std::vector<cv::Mat>& images, 
      const std::vector<std::vector<std::vector<int>>>& faces) {
    std::vector<std::vector<cv::Rect>> rects;
    cv::Rect r;
    for (auto& a : faces) {
      std::vector<cv::Rect> rec;
      for (auto& b : a) {
        r = cv::Rect(b[0],b[1],b[2],b[3]);
        rec.emplace_back(r);
      }
      rects.emplace_back(rec);
    }
    return arctern::ArcternManager::predict_face_age(images, rects);
  }

  std::pair<float, std::vector<float>> predict_face_quality_pose(
      const cv::Mat& image, const std::vector<int>& face) {
    cv::Rect rect({face[0], face[1], face[2], face[3]});
    auto a = arctern::ArcternManager::predict_face_quality_pose(image, rect);
    std::vector<float> vec3f({a.second[0], a.second[1], a.second[2]});
    return std::make_pair(a.first, vec3f);
  }

  std::vector<std::vector<std::pair<float,std::vector<float>>>> 
      predict_face_quality_pose_batch(const std::vector<cv::Mat>& images,
      const std::vector<std::vector<std::vector<int>>>& faces) {
    std::vector<std::vector<std::pair<float, std::vector<float>>>> results;
    std::vector<std::vector<cv::Rect>> rects;
    cv::Rect r;
    for (auto& a : faces) {
      std::vector<cv::Rect> rec;
      for(auto& b : a){
        r = cv::Rect(b[0],b[1],b[2],b[3]);
        rec.emplace_back(r);
      }
      rects.emplace_back(rec);
    }
    std::vector<float> vec3f;
    auto poses = arctern::ArcternManager::predict_face_quality_pose(images, rects);
    for (auto& a : poses) {
      std::vector<std::pair<float, std::vector<float>>> res; 
      for (auto& b : a) {
        vec3f = {b.second[0], b.second[1], b.second[2]};
        res.emplace_back(std::make_pair(b.first, vec3f));
      }
      results.emplace_back(res);
    }
    return results;
  }

  std::pair<Gender, float> predict_face_gender(const cv::Mat& image,
                                               std::vector<int>& face) {
    cv::Rect rect = {face[0], face[1], face[2], face[3]};
    return arctern::ArcternManager::predict_face_gender(image, rect);
  }

  std::vector<std::vector<std::pair<int,float>>> predict_face_gender_batch(
      const std::vector<cv::Mat>& images,
      const std::vector<std::vector<std::vector<int>>>& faces){
    std::vector<std::vector<cv::Rect>> rects;
    cv::Rect r;
    for (auto& a : faces) {
      std::vector<cv::Rect> rec;
      for (auto& b : a) {
        r = cv::Rect(b[0],b[1],b[2],b[3]);
        rec.emplace_back(r);
      }
      rects.emplace_back(rec);
    }
    return arctern::ArcternManager::predict_face_gender(images, rects);
  } 
 
  std::pair<Glass, float> predict_face_glass(const cv::Mat& image,
                                             std::vector<int>& face) {
    cv::Rect rect = {face[0], face[1], face[2], face[3]};
    return arctern::ArcternManager::predict_face_glass(image, rect);
  }

  std::vector<std::vector<std::pair<int,float>>> predict_face_glass_batch(
      const std::vector<cv::Mat>& images,
      const std::vector<std::vector<std::vector<int>>>& faces){
    std::vector<std::vector<cv::Rect>> rects;
    cv::Rect r;
    for (auto& a : faces) {
      std::vector<cv::Rect> rec;
      for (auto& b : a) {
        r = cv::Rect(b[0],b[1],b[2],b[3]);
        rec.emplace_back(r);
      }
      rects.emplace_back(rec);
    }
    return arctern::ArcternManager::predict_face_glass(images, rects);
  }

  std::pair<Hat, float> predict_face_hat(const cv::Mat& image,
                                         std::vector<int>& face) {
    cv::Rect rect = {face[0], face[1], face[2], face[3]};
    return arctern::ArcternManager::predict_face_hat(image, rect);
  }

  std::vector<std::vector<std::pair<int,float>>> predict_face_hat_batch(
      const std::vector<cv::Mat>& images,
      const std::vector<std::vector<std::vector<int>>>& faces){
    std::vector<std::vector<cv::Rect>> rects;
    cv::Rect r;
    for (auto& a : faces) {
      std::vector<cv::Rect> rec;
      for (auto& b : a) {
        r = cv::Rect(b[0],b[1],b[2],b[3]);
        rec.emplace_back(r);
      }
      rects.emplace_back(rec);
    }
    return arctern::ArcternManager::predict_face_hat(images, rects);
  }

  std::pair<FaceAngle, float> predict_face_angle(const cv::Mat& image,
                                                 std::vector<int>& face) {
    cv::Rect rect = {face[0], face[1], face[2], face[3]};
    return arctern::ArcternManager::predict_face_angle(image, rect);
  }

  std::vector<std::vector<std::pair<int,float>>> predict_face_angle_batch(
      const std::vector<cv::Mat>& images,
      const std::vector<std::vector<std::vector<int>>>& faces){
    std::vector<std::vector<cv::Rect>> rects;
    cv::Rect r;
    for (auto& a : faces) {
      std::vector<cv::Rect> rec;
      for (auto& b : a) {
        r = cv::Rect(b[0],b[1],b[2],b[3]);
        rec.emplace_back(r);
      }
      rects.emplace_back(rec);
    }
    return arctern::ArcternManager::predict_face_angle(images, rects);
  }
  
  std::pair<Expression, float> predict_face_expression(const cv::Mat& image,
        const std::vector<int>& face,
        const std::vector<float>& landmarks) {
    cv::Rect rect({ face[0], face[1], face[2], face[3] });
    auto ret = arctern::ArcternManager::predict_face_expression(image, rect, landmarks);
    return ret;
  }

  std::vector<std::vector<std::pair<int,float>>> predict_face_expression_batch(
      const std::vector<cv::Mat>& images,
      const std::vector<std::vector<std::vector<int>>>& faces,
      const std::vector<std::vector<std::vector<float>>>& landmarks) {
    std::vector<std::vector<cv::Rect>> rects;
    cv::Rect r;
    for (auto& a : faces) {
      std::vector<cv::Rect> rec;
      for (auto& b : a) {
        r = cv::Rect(b[0],b[1],b[2],b[3]);
        rec.emplace_back(r);
      }
      rects.emplace_back(rec);
    }
    return arctern::ArcternManager::predict_face_expression(images, rects, landmarks);
  }

  std::pair<ExpressionType, float> predict_face_expression(const cv::Mat& image,
        const std::vector<int>& face) {
    cv::Rect rect({ face[0], face[1], face[2], face[3] });
    auto ret = arctern::ArcternManager::predict_face_expression(image, rect);
    return ret;
  }

  std::vector<std::vector<std::pair<ExpressionType,float>>> predict_face_expression_batch(
      const std::vector<cv::Mat>& images,
      const std::vector<std::vector<std::vector<int>>>& faces) {
    std::vector<std::vector<cv::Rect>> rects;
    cv::Rect r;
    for (auto& a : faces) {
      std::vector<cv::Rect> rec;
      for (auto& b : a) {
        r = cv::Rect(b[0],b[1],b[2],b[3]);
        rec.emplace_back(r);
      }
      rects.emplace_back(rec);
    }
    return arctern::ArcternManager::predict_face_expression(images, rects);
  }

  float predict_face_quality(const cv::Mat& image, const std::vector<int>& face) {
    cv::Rect rect({ face[0], face[1], face[2], face[3] });
    return arctern::ArcternManager::predict_face_quality(image, rect);
  }

  std::vector<std::vector<float>> predict_face_quality_batch(
      const std::vector<cv::Mat>& images,
      const std::vector<std::vector<std::vector<int>>>& faces) {
    std::vector<std::vector<cv::Rect>> rects;
    cv::Rect r;
    for (auto& a : faces) {
      std::vector<cv::Rect> rec;
      for (auto& b : a) {
        r = cv::Rect(b[0],b[1],b[2],b[3]);
        rec.emplace_back(r);
      }
      rects.emplace_back(rec);
    }
    return arctern::ArcternManager::predict_face_quality(images, rects);
  }

  std::vector<uint8> get_face_feat(const cv::Mat& image,
                                   std::vector<int>& face, 
                                   FaceFeatParams facefeatparams = FaceFeatParams()) {
    cv::Rect rect = {face[0], face[1], face[2], face[3]};
    if(facefeatparams.landmarks.size() == 0) {
      return arctern::ArcternManager::get_face_feat(image, rect);
    } else {
      ConfigInfo config = ConfigInfo();
      config.facefeatparams = &facefeatparams;
      return arctern::ArcternManager::get_face_feat(image, rect, &config);
    }
  }

  std::vector<std::vector<std::vector<uint8>>> get_face_feat_batch(
      const std::vector<cv::Mat>& images,
      const std::vector<std::vector<std::vector<int>>>& faces,
      FaceFeatParams facefeatparams = FaceFeatParams()) {
    std::vector<std::vector<cv::Rect>> rects;
    cv::Rect r;
    for (auto& a : faces) {
      std::vector<cv::Rect> rec;
      for (auto& b : a) {
        r = cv::Rect(b[0],b[1],b[2],b[3]);
        rec.emplace_back(r);
      }
      rects.emplace_back(rec);
    }
    if(facefeatparams.landmarks.size() == 0) {
      return arctern::ArcternManager::get_face_feat(images, rects);
    } else {
      ConfigInfo config = ConfigInfo();
      config.facefeatparams = &facefeatparams;
      return arctern::ArcternManager::get_face_feat(images, rects, &config);
    }
  }

  FaceAttribute predict_face_attributes(
      const cv::Mat& image,
      const std::vector<int>& face) {
    cv::Rect rect = cv::Rect(face[0], face[1], face[2],face[3]);
    return  arctern::ArcternManager::predict_face_attributes(image, rect); 
  }

  std::vector<std::vector<FaceAttribute>> predict_face_attributes_batch(
      const std::vector<cv::Mat>& images,
      const std::vector<std::vector<std::vector<int>>>& faces) {
    std::vector<std::vector<cv::Rect>> rects;
    cv::Rect r;
    for (auto& a : faces) {
      std::vector<cv::Rect> rec;
      for (auto& b : a) {
        r = cv::Rect(b[0],b[1],b[2],b[3]);
        rec.emplace_back(r);
      }
      rects.emplace_back(rec);
    }
    return arctern::ArcternManager::predict_face_attributes(images, rects);
  }

  FaceAttribute predict_face_gender_age(
      const cv::Mat& image,
      const std::vector<int>& face) {
    cv::Rect rect = cv::Rect(face[0], face[1], face[2],face[3]);
    return  arctern::ArcternManager::predict_face_gender_age(image, rect);
  }

  std::vector<std::vector<FaceAttribute>> predict_face_gender_age_batch(
      const std::vector<cv::Mat>& images,
      const std::vector<std::vector<std::vector<int>>>& faces) {
    std::vector<std::vector<cv::Rect>> rects;
    cv::Rect r;
    for (auto& a : faces) {
      std::vector<cv::Rect> rec;
      for (auto& b : a) {
        r = cv::Rect(b[0],b[1],b[2],b[3]);
        rec.emplace_back(r);
      }
      rects.emplace_back(rec);
    }
    return arctern::ArcternManager::predict_face_gender_age(images, rects);
  }

  std::pair<NoiseType, float> predict_face_noise_motionblur(const cv::Mat& image,
        const std::vector<int>& face) {
    cv::Rect rect({ face[0], face[1], face[2], face[3] });
    auto ret = arctern::ArcternManager::predict_face_noise_motionblur(image, rect);
    return ret;
  }

  std::vector<std::vector<std::pair<NoiseType,float>>> predict_face_noise_motionblur_batch(
      const std::vector<cv::Mat>& images,
      const std::vector<std::vector<std::vector<int>>>& faces) {
    std::vector<std::vector<cv::Rect>> rects;
    cv::Rect r;
    for (auto& a : faces) {
      std::vector<cv::Rect> rec;
      for (auto& b : a) {
        r = cv::Rect(b[0],b[1],b[2],b[3]);
        rec.emplace_back(r);
      }
      rects.emplace_back(rec);
    }
    return arctern::ArcternManager::predict_face_noise_motionblur(images, rects);
  }

  std::vector<float> predict_facial_landmark(
      const cv::Mat& image, const std::vector<int>& face) {
    cv::Rect rect({face[0], face[1], face[2], face[3]});
    std::vector<cv::Point> points = arctern::ArcternManager::predict_facial_landmark(image, rect);
    std::vector<float> landmarks;
    for (auto& p : points) {
      landmarks.push_back(p.x);
      landmarks.push_back(p.y);
    }
    return landmarks;
  }

  std::vector<std::vector<std::vector<float>>>
      predict_facial_landmark_batch(const std::vector<cv::Mat>& images,
      const std::vector<std::vector<std::vector<int>>>& faces) {
    std::vector<std::vector<std::vector<float>>> landmarks;
    std::vector<std::vector<cv::Rect>> rects;
    cv::Rect r;
    for (auto& a : faces) {
      std::vector<cv::Rect> rec;
      for (auto& b : a) {
        r = cv::Rect(b[0],b[1],b[2],b[3]);
        rec.emplace_back(r);
      }
      rects.emplace_back(rec);
    }
    auto points = arctern::ArcternManager::predict_facial_landmark(images, rects);
    for (auto& a : points) {
      std::vector<std::vector<float>> ldmks; 
      for (auto& b : a) {
        std::vector<float> ldmk; 
        for (auto& p : b) {
          ldmk.push_back(p.x);
          ldmk.push_back(p.y);
        }
        ldmks.push_back(ldmk);
      }
      landmarks.push_back(ldmks);
    }
    return landmarks;
  }

  std::vector<float> predict_facial_landmark_tiny_7(
      const cv::Mat& image, const std::vector<int>& face) {
    cv::Rect rect({face[0], face[1], face[2], face[3]});
    std::vector<cv::Point> points = arctern::ArcternManager::predict_facial_landmark_tiny_7(image, rect);
    std::vector<float> landmarks;
    for (auto& p : points) {
      landmarks.push_back(p.x);
      landmarks.push_back(p.y);
    }
    return landmarks;
  }

  std::vector<std::vector<std::vector<float>>>
      predict_facial_landmark_tiny_7_batch(const std::vector<cv::Mat>& images,
      const std::vector<std::vector<std::vector<int>>>& faces) {
    std::vector<std::vector<std::vector<float>>> landmarks;
    std::vector<std::vector<cv::Rect>> rects;
    cv::Rect r;
    for (auto& a : faces) {
      std::vector<cv::Rect> rec;
      for (auto& b : a) {
        r = cv::Rect(b[0],b[1],b[2],b[3]);
        rec.emplace_back(r);
      }
      rects.emplace_back(rec);
    }
    auto points = arctern::ArcternManager::predict_facial_landmark_tiny_7(images, rects);
    for (auto& a : points) {
      std::vector<std::vector<float>> ldmks; 
      for (auto& b : a) {
        std::vector<float> ldmk; 
        for (auto& p : b) {
          ldmk.push_back(p.x);
          ldmk.push_back(p.y);
        }
        ldmks.push_back(ldmk);
      }
      landmarks.push_back(ldmks);
    }
    return landmarks;
  }

  std::vector<float> predict_facial_landmark_detect(
      const cv::Mat& image, const std::vector<int>& face) {
    cv::Rect rect({face[0], face[1], face[2], face[3]});
    std::vector<cv::Point> points = arctern::ArcternManager::predict_facial_landmark_detect(image, rect);
    std::vector<float> landmarks;
    for (auto& p : points) {
      landmarks.push_back(p.x);
      landmarks.push_back(p.y);
    }
    return landmarks;
  }

  std::vector<std::vector<std::vector<float>>>
      predict_facial_landmark_detect_batch(const std::vector<cv::Mat>& images,
      const std::vector<std::vector<std::vector<int>>>& faces) {
    std::vector<std::vector<std::vector<float>>> landmarks;
    std::vector<std::vector<cv::Rect>> rects;
    cv::Rect r;
    for (auto& a : faces) {
      std::vector<cv::Rect> rec;
      for (auto& b : a) {
        r = cv::Rect(b[0],b[1],b[2],b[3]);
        rec.emplace_back(r);
      }
      rects.emplace_back(rec);
    }
    auto points = arctern::ArcternManager::predict_facial_landmark_detect(images, rects);
    for (auto& a : points) {
      std::vector<std::vector<float>> ldmks; 
      for (auto& b : a) {
        std::vector<float> ldmk; 
        for (auto& p : b) {
          ldmk.push_back(p.x);
          ldmk.push_back(p.y);
        }
        ldmks.push_back(ldmk);
      }
      landmarks.push_back(ldmks);
    }
    return landmarks;
  }

  std::vector<std::pair<int, float>> predict_face_pose_cls(const cv::Mat& image,
                                             std::vector<int>& face) {
    cv::Rect rect = {face[0], face[1], face[2], face[3]};
    return arctern::ArcternManager::predict_face_pose_cls(image, rect);
  }

  std::vector<std::vector<std::vector<std::pair<int,float>>>> predict_face_pose_cls_batch(
      const std::vector<cv::Mat>& images,
      const std::vector<std::vector<std::vector<int>>>& faces) {
    std::vector<std::vector<cv::Rect>> rects;
    cv::Rect r;
    for (auto& a : faces) {
      std::vector<cv::Rect> rec;
      for (auto& b : a) {
        r = cv::Rect(b[0],b[1],b[2],b[3]);
        rec.emplace_back(r);
      }
      rects.emplace_back(rec);
    }
    return arctern::ArcternManager::predict_face_pose_cls(images, rects);
  }

  float predict_face_liveness_ir(const cv::Mat& image, const std::vector<int>& face) {
    cv::Rect rect({face[0], face[1], face[2], face[3]});
    return arctern::ArcternManager::predict_face_liveness_ir(image, rect);
  }

  std::vector<std::vector<float>> predict_face_liveness_ir_batch(
      const std::vector<cv::Mat>& images, 
      const std::vector<std::vector<std::vector<int>>>& faces) {
    std::vector<std::vector<cv::Rect>> rects;
    cv::Rect r;
    for (auto& a : faces) {
      std::vector<cv::Rect> rec;
      for (auto& b : a) {
        r = cv::Rect(b[0],b[1],b[2],b[3]);
        rec.emplace_back(r);
      }
      rects.emplace_back(rec);
    }
    return arctern::ArcternManager::predict_face_liveness_ir(images, rects);
  }

  float predict_face_image_color(const cv::Mat& image, const std::vector<int>& face) {
    cv::Rect rect({face[0], face[1], face[2], face[3]});
    return arctern::ArcternManager::predict_face_image_color(image, rect);
  }

  std::vector<std::vector<float>> predict_face_image_color_batch(
      const std::vector<cv::Mat>& images, 
      const std::vector<std::vector<std::vector<int>>>& faces) {
    std::vector<std::vector<cv::Rect>> rects;
    cv::Rect r;
    for (auto& a : faces) {
      std::vector<cv::Rect> rec;
      for (auto& b : a) {
        r = cv::Rect(b[0],b[1],b[2],b[3]);
        rec.emplace_back(r);
      }
      rects.emplace_back(rec);
    }
    return arctern::ArcternManager::predict_face_image_color(images, rects);
  }

  std::vector<std::pair<std::string, std::pair<int, float>>> predict_faceocc_attributes(
          const cv::Mat &image, const std::vector<int> &face) {
    cv::Rect rect({face[0], face[1], face[2], face[3]});
    return arctern::ArcternManager::predict_faceocc_attributes(image, rect);
  }

  std::vector<std::vector<std::vector<std::pair<std::string, std::pair<int, float>>>>> predict_faceocc_attributes_batch(
          const std::vector<cv::Mat> &images,
          const std::vector<std::vector<std::vector<int>>> &faces) {
    std::vector<std::vector<cv::Rect>> rects;
    cv::Rect r;
    for (auto& a : faces) {
      std::vector<cv::Rect> rec;
      for (auto& b : a) {
        r = cv::Rect(b[0],b[1],b[2],b[3]);
        rec.emplace_back(r);
      }
      rects.emplace_back(rec);
    }
    return arctern::ArcternManager::predict_faceocc_attributes(images, rects);
  }

  std::pair<AbnomalLightType, float> predict_face_abnomal_light(const cv::Mat& image,
        const std::vector<int>& face) {
    cv::Rect rect({ face[0], face[1], face[2], face[3] });
    auto ret = arctern::ArcternManager::predict_face_abnomal_light(image, rect);
    return ret;
  }

  std::vector<std::vector<std::pair<AbnomalLightType,float>>> predict_face_abnomal_light_batch(
      const std::vector<cv::Mat>& images,
      const std::vector<std::vector<std::vector<int>>>& faces) {
    std::vector<std::vector<cv::Rect>> rects;
    cv::Rect r;
    for (auto& a : faces) {
      std::vector<cv::Rect> rec;
      for (auto& b : a) {
        r = cv::Rect(b[0],b[1],b[2],b[3]);
        rec.emplace_back(r);
      }
      rects.emplace_back(rec);
    }
    return arctern::ArcternManager::predict_face_abnomal_light(images, rects);
  }

  std::vector<float> predict_face_3dlandmark_pose(const cv::Mat& image,
        const std::vector<int>& face) {
    cv::Rect rect({ face[0], face[1], face[2], face[3]});
    auto ret = arctern::ArcternManager::predict_face_3dlandmark_pose(image, rect);
    return ret;
  }

  std::vector<std::vector<std::vector<float>>> predict_face_3dlandmark_pose_batch(
      const std::vector<cv::Mat>& images,
      const std::vector<std::vector<std::vector<int>>>& faces) {
    std::vector<std::vector<cv::Rect>> rects;
    cv::Rect r;
    for (auto& a : faces) {
      std::vector<cv::Rect> rec;
      for (auto& b : a) {
        r = cv::Rect(b[0],b[1],b[2],b[3]);
        rec.emplace_back(r);
      }
      rects.emplace_back(rec);
    }
    return arctern::ArcternManager::predict_face_3dlandmark_pose(images, rects);
  }

  std::pair<MouthAction, float> predict_mouth_action(const cv::Mat& image,
        const std::vector<int>& face) {
    cv::Rect rect({ face[0], face[1], face[2], face[3] });
    auto ret = arctern::ArcternManager::predict_mouth_action(image, rect);
    return ret;
  }

  std::map<std::string, std::pair<int, float>> predict_face_extendmultiattribute(const cv::Mat& image,
        const std::vector<int>& face) {
    cv::Rect rect({ face[0], face[1], face[2], face[3]});
    auto ret = arctern::ArcternManager::predict_face_extendmultiattribute(image, rect);
    return ret;
  }

  std::vector<std::vector<std::map<std::string, std::pair<int, float>>>> predict_face_extendmultiattribute_batch(
      const std::vector<cv::Mat>& images,
      const std::vector<std::vector<std::vector<int>>>& faces) {
    std::vector<std::vector<cv::Rect>> rects;
    cv::Rect r;
    for (auto& a : faces) {
      std::vector<cv::Rect> rec;
      for (auto& b : a) {
        r = cv::Rect(b[0],b[1],b[2],b[3]);
        rec.emplace_back(r);
      }
      rects.emplace_back(rec);
    }
    return arctern::ArcternManager::predict_face_extendmultiattribute(images, rects);
  }

  std::vector<std::pair<AbnormalExposureType, float>> ArcternManager::detect_image_abnormalexposure(
        const cv::Mat& image, const std::vector<int>& face, const std::vector<float>& config) {
    cv::Rect rect({face[0], face[1], face[2], face[3]});
    float cfg[9] = { config[0], config[1], config[2], config[3], 
        config[4], config[5], config[6], config[7], config[8]};
    return arctern::ArcternManager::detect_image_abnormalexposure(image, rect, cfg);
  }

  std::vector<std::vector<std::vector<std::pair<AbnormalExposureType, float>>>> ArcternManager::detect_image_abnormalexposure_batch(
      const std::vector<cv::Mat>& images, 
      const std::vector<std::vector<std::vector<int>>>& faces,
      const std::vector<float>& config) {
    std::vector<std::vector<cv::Rect>> rects;
    cv::Rect r;
    for (auto& a : faces) {
      std::vector<cv::Rect> rec;
      for (auto& b : a) {
        r = cv::Rect(b[0],b[1],b[2],b[3]);
        rec.emplace_back(r);
      }
      rects.emplace_back(rec);
    }
    float cfg[9] = { config[0], config[1], config[2], config[3], 
        config[4], config[5], config[6], config[7], config[8]};
    return arctern::ArcternManager::detect_image_abnormalexposure(images, rects, cfg);
  }
 
  std::vector<PersonDetection> detect_person(
      const cv::Mat& image) {
    auto a = arctern::ArcternManager::detect_person(image);
    std::vector<PersonDetection> results;
    std::transform(a.cbegin(), a.cend(), std::back_inserter(results),
                   [](const arctern::PersonDetection& ret) {
                     PersonDetection pd;
                     pd.box = { ret.box.x, ret.box.y,
                                ret.box.width, ret.box.height };
                     pd.confidence = ret.confidence;
                     pd.top_occluded_prob = ret.confidence;
                     pd.bottom_occluded_prob = ret.bottom_occluded_prob;
                     pd.type = ret.type;
                     return pd;
                   });
    return results;
  }

  std::vector<std::vector<PersonDetection>> detect_person_batch(
          const std::vector<cv::Mat>& images) {
    auto persons = arctern::ArcternManager::detect_person(images);
    std::vector<std::vector<PersonDetection>> results;
    for (auto& a : persons) {
      std::vector<PersonDetection> res;
      std::transform(a.cbegin(), a.cend(), std::back_inserter(res),
                     [](const arctern::PersonDetection& ret) {
                       PersonDetection pd;
                       pd.box = { ret.box.x, ret.box.y,
                                  ret.box.width, ret.box.height };
                       pd.confidence = ret.confidence;
                       pd.top_occluded_prob = ret.confidence;
                       pd.bottom_occluded_prob = ret.bottom_occluded_prob;
                       pd.type = ret.type;
                       return pd;
                     });
      results.emplace_back(res);
    }
    return results;
  }

  std::vector<uint8> get_pedestrian_feat(const cv::Mat& image,
                                         const std::vector<int>& person) {
    cv::Rect rect = {person[0], person[1], person[2], person[3]};
    return arctern::ArcternManager::get_pedestrian_feat(image, rect);
  }

  std::vector<std::vector<std::vector<uint8>>> get_pedestrian_feat_batch(
      const std::vector<cv::Mat>& images,
      const std::vector<std::vector<std::vector<int>>>& persons) {
    std::vector<std::vector<cv::Rect>> rects;
    cv::Rect r;
    for (auto& a : persons) {
      std::vector<cv::Rect> rec;
      for (auto& b : a) {
        r = cv::Rect(b[0],b[1],b[2],b[3]);
        rec.emplace_back(r);
      }
      rects.emplace_back(rec);
    }
    return arctern::ArcternManager::get_pedestrian_feat(images, rects);
  }

  float predict_person_quality(const cv::Mat& image, const std::vector<int>& person) {
    cv::Rect rect({person[0], person[1], person[2], person[3]});
    return arctern::ArcternManager::predict_person_quality(image, rect);
  }

  std::vector<std::vector<float>> predict_person_quality_batch(
      const std::vector<cv::Mat>& images, 
      const std::vector<std::vector<std::vector<int>>>& person) {
    std::vector<std::vector<cv::Rect>> rects;
    cv::Rect r;
    for (auto& a : person) {
      std::vector<cv::Rect> rec;
      for (auto& b : a) {
        r = cv::Rect(b[0],b[1],b[2],b[3]);
        rec.emplace_back(r);
      }
      rects.emplace_back(rec);
    }
    return arctern::ArcternManager::predict_person_quality(images, rects);
  }

  PersonCompletenessInfo predict_person_completeness(const cv::Mat& image, const std::vector<int>& person) {
    cv::Rect rect({person[0], person[1], person[2], person[3]});
    return arctern::ArcternManager::predict_person_completeness(image, rect);
  }

  std::vector<std::vector<PersonCompletenessInfo>> predict_person_completeness_batch(
      const std::vector<cv::Mat>& images, 
      const std::vector<std::vector<std::vector<int>>>& person) {
    std::vector<std::vector<cv::Rect>> rects;
    cv::Rect r;
    for (auto& a : person) {
      std::vector<cv::Rect> rec;
      for (auto& b : a) {
        r = cv::Rect(b[0],b[1],b[2],b[3]);
        rec.emplace_back(r);
      }
      rects.emplace_back(rec);
    }
    return arctern::ArcternManager::predict_person_completeness(images, rects);
  }

  std::vector<std::pair<std::string, std::pair<std::string, float>>> predict_person_attributes(const cv::Mat& image,
      const std::vector<int>& person, const std::vector<float> &pad_scale) {
    cv::Rect rect({person[0], person[1], person[2], person[3]});
    return arctern::ArcternManager::predict_person_attributes(image, rect, pad_scale);
  }

  std::vector<std::vector<std::vector<std::pair<std::string, std::pair<std::string, float>>> >> predict_person_attributes_batch(
      const std::vector<cv::Mat>& images, 
      const std::vector<std::vector<std::vector<int>>>& person, 
      const std::vector<std::vector<std::vector<float>>>& pad_scales) {
    std::vector<std::vector<cv::Rect>> rects;
    cv::Rect r;
    for (auto& a : person) {
      std::vector<cv::Rect> rec;
      for (auto& b : a) {
        r = cv::Rect(b[0],b[1],b[2],b[3]);
        rec.emplace_back(r);
      }
      rects.emplace_back(rec);
    }
    return arctern::ArcternManager::predict_person_attributes(images, rects, pad_scales);
  } 

  std::vector<PersonDetection> detect_person_tiny(
      const cv::Mat& image) {
    auto a = arctern::ArcternManager::detect_person_tiny(image);
    std::vector<PersonDetection> results;
    std::transform(a.cbegin(), a.cend(), std::back_inserter(results),
                   [](const arctern::PersonDetection& ret) {
                     PersonDetection pd;
                     pd.box = { ret.box.x, ret.box.y,
                                ret.box.width, ret.box.height };
                     pd.confidence = ret.confidence;
                     pd.top_occluded_prob = ret.confidence;
                     pd.bottom_occluded_prob = ret.bottom_occluded_prob;
                     pd.type = ret.type;
                     return pd;
                   });
    return results;
  }

  std::vector<std::vector<PersonDetection>> detect_person_tiny_batch(
          const std::vector<cv::Mat>& images) {
    auto persons = arctern::ArcternManager::detect_person_tiny(images);
    std::vector<std::vector<PersonDetection>> results;
    for (auto& a : persons) {
      std::vector<PersonDetection> res;
      std::transform(a.cbegin(), a.cend(), std::back_inserter(res),
                     [](const arctern::PersonDetection& ret) {
                       PersonDetection pd;
                       pd.box = { ret.box.x, ret.box.y,
                                  ret.box.width, ret.box.height };
                       pd.confidence = ret.confidence;
                       pd.top_occluded_prob = ret.confidence;
                       pd.bottom_occluded_prob = ret.bottom_occluded_prob;
                       pd.type = ret.type;
                       return pd;
                     });
      results.emplace_back(res);
    }
    return results;
  }

  std::vector<std::pair<std::vector<int>, float>> detect_bike(
      const cv::Mat& image) {
    auto a = arctern::ArcternManager::detect_bike(image);
    std::vector<std::pair<std::vector<int>, float>> results;
    std::transform(a.cbegin(), a.cend(), std::back_inserter(results),
                   [](const std::pair<cv::Rect, float>& face) {
                     const auto& r = face.first;
                     std::vector<int> vecr({r.x, r.y, r.width, r.height});
                     return std::make_pair(vecr, face.second);
                   });
    return results;
  }

  std::vector<std::vector<std::pair<std::vector<int>, float>>> detect_bike_batch(
      const std::vector<cv::Mat>& images) {
    auto bikes = arctern::ArcternManager::detect_bike(images);
    std::vector<std::vector<std::pair<std::vector<int>, float>>> results;
    for (auto& a : bikes) {
      std::vector<std::pair<std::vector<int>, float>> res;
      std::transform(a.cbegin(), a.cend(), std::back_inserter(res),
                     [](const std::pair<cv::Rect, float>& face) {
                       const auto& r = face.first;
                       std::vector<int> vecr({r.x, r.y, r.width, r.height});
                       return std::make_pair(vecr, face.second);
                     });
      results.emplace_back(res);
    }
    return results;
  }

  std::vector<std::pair<std::string, float>> predict_car_types(const cv::Mat& image,
                                             std::vector<int>& car_roi) {
    cv::Rect rect = {car_roi[0], car_roi[1], car_roi[2], car_roi[3]};
    return arctern::ArcternManager::predict_car_types(image, rect);
  }

  std::vector<std::vector<std::vector<std::pair<std::string,float>>>> predict_car_types_batch(
      const std::vector<cv::Mat>& images,
      const std::vector<std::vector<std::vector<int>>>& car_rois) {
    std::vector<std::vector<cv::Rect>> rects;
    cv::Rect r;
    for (auto& a : car_rois) {
      std::vector<cv::Rect> rec;
      for (auto& b : a) {
        r = cv::Rect(b[0],b[1],b[2],b[3]);
        rec.emplace_back(r);
      }
      rects.emplace_back(rec);
    }
    return arctern::ArcternManager::predict_car_types(images, rects);
  }

  std::vector<std::pair<std::string, std::pair<std::string, float>>> predict_car_attributes(const cv::Mat& image,
                                                                    std::vector<int>& car_roi) {
    cv::Rect rect = {car_roi[0], car_roi[1], car_roi[2], car_roi[3]};
    return arctern::ArcternManager::predict_car_attributes(image, rect);
  }

  std::vector<std::vector<std::vector<std::pair<std::string, std::pair<std::string,float>>>>> predict_car_attributes_batch(
      const std::vector<cv::Mat>& images,
      const std::vector<std::vector<std::vector<int>>>& car_rois) {
    std::vector<std::vector<cv::Rect>> rects;
    cv::Rect r;
    for (auto& a : car_rois) {
      std::vector<cv::Rect> rec;
      for (auto& b : a) {
        r = cv::Rect(b[0],b[1],b[2],b[3]);
        rec.emplace_back(r);
      }
      rects.emplace_back(rec);
    }
    return arctern::ArcternManager::predict_car_attributes(images, rects);
  }

  std::vector<FashionDetection> detect_fashion(
      const cv::Mat& image) {
    auto a = arctern::ArcternManager::detect_fashion(image);
    std::vector<FashionDetection> results;
    std::transform(a.cbegin(), a.cend(), std::back_inserter(results),
                   [](const arctern::FashionDetection& ret) {
                     FashionDetection fd;
                     fd.box = { ret.box.x, ret.box.y,
                                ret.box.width, ret.box.height };
                     fd.confidence = ret.confidence;
                     fd.type = ret.type;
                     return fd;
                   });
    return results;
  }

  std::vector<std::vector<FashionDetection>> detect_fashion_batch(
          const std::vector<cv::Mat>& images) {
    auto fashions = arctern::ArcternManager::detect_fashion(images);
    std::vector<std::vector<FashionDetection>> results;
    for (auto& a : fashions) {
      std::vector<FashionDetection> res;
      std::transform(a.cbegin(), a.cend(), std::back_inserter(res),
                     [](const arctern::FashionDetection& ret) {
                       FashionDetection fd;
                       fd.box = { ret.box.x, ret.box.y,
                                  ret.box.width, ret.box.height };
                       fd.confidence = ret.confidence;
                       fd.type = ret.type;
                       return fd;
                     });
      results.emplace_back(res);
    }
    return results;
  }

  std::pair<FashionStyleType, float> detect_fashionstyle(const cv::Mat& image) {
    return arctern::ArcternManager::detect_fashionstyle(image);
  }

  std::vector<std::pair<FashionStyleType, float>> detect_fashionstyle_batch(const std::vector<cv::Mat>& images) {
    return arctern::ArcternManager::detect_fashionstyle(images);
  }

  std::vector<MultiCatDetection> detect_multi_tiny_cat(
      const cv::Mat& image) {
    auto a = arctern::ArcternManager::detect_multi_tiny_cat(image);
    std::vector<MultiCatDetection> results;
    std::transform(a.cbegin(), a.cend(), std::back_inserter(results),
                   [](const arctern::MultiCatDetection& ret) {
                     MultiCatDetection md;
                     md.box = { ret.box.x, ret.box.y,
                                ret.box.width, ret.box.height };
                     md.confidence = ret.confidence;
                     md.type = ret.type;
                     md.quality = ret.quality;
                     return md;
                   });
    return results;
  }

  std::vector<std::vector<MultiCatDetection>> detect_multi_tiny_cat_batch(
          const std::vector<cv::Mat>& images) {
    auto cats = arctern::ArcternManager::detect_multi_tiny_cat(images);
    std::vector<std::vector<MultiCatDetection>> results;
    for (auto& a : cats) {
      std::vector<MultiCatDetection> res;
      std::transform(a.cbegin(), a.cend(), std::back_inserter(res),
                     [](const arctern::MultiCatDetection& ret) {
                       MultiCatDetection md;
                       md.box = { ret.box.x, ret.box.y,
                                  ret.box.width, ret.box.height };
                       md.confidence = ret.confidence;
                       md.type = ret.type;
                       md.quality = ret.quality;
                       return md;
                     });
      results.emplace_back(res);
    }
    return results;
  }

  std::vector<MultiCatDetection> detect_multi_cat_center(
      const cv::Mat& image) {
    auto a = arctern::ArcternManager::detect_multi_cat_center(image);
    std::vector<MultiCatDetection> results;
    std::transform(a.cbegin(), a.cend(), std::back_inserter(results),
                   [](const arctern::MultiCatDetection& ret) {
                     MultiCatDetection md;
                     md.box = { ret.box.x, ret.box.y,
                                ret.box.width, ret.box.height };
                     md.confidence = ret.confidence;
                     md.type = ret.type;
                     md.quality = ret.quality;
                     return md;
                   });
    return results;
  }

  std::vector<std::vector<MultiCatDetection>> detect_multi_cat_center_batch(
          const std::vector<cv::Mat>& images) {
    auto cats = arctern::ArcternManager::detect_multi_cat_center(images);
    std::vector<std::vector<MultiCatDetection>> results;
    for (auto& a : cats) {
      std::vector<MultiCatDetection> res;
      std::transform(a.cbegin(), a.cend(), std::back_inserter(res),
                     [](const arctern::MultiCatDetection& ret) {
                       MultiCatDetection md;
                       md.box = { ret.box.x, ret.box.y,
                                  ret.box.width, ret.box.height };
                       md.confidence = ret.confidence;
                       md.type = ret.type;
                       md.quality = ret.quality;
                       return md;
                     });
      results.emplace_back(res);
    }
    return results;
  }

  std::vector<MultiCatDetection> detect_body_face_head_center(
      const cv::Mat& image) {
    auto a = arctern::ArcternManager::detect_body_face_head_center(image);
    std::vector<MultiCatDetection> results;
    std::transform(a.cbegin(), a.cend(), std::back_inserter(results),
                   [](const arctern::MultiCatDetection& ret) {
                     MultiCatDetection md;
                     md.box = { ret.box.x, ret.box.y,
                                ret.box.width, ret.box.height };
                     md.confidence = ret.confidence;
                     md.type = ret.type;
                     md.quality = ret.quality;
                     return md;
                   });
    return results;
  }

  std::vector<std::vector<MultiCatDetection>> detect_body_face_head_center_batch(
          const std::vector<cv::Mat>& images) {
    auto cats = arctern::ArcternManager::detect_body_face_head_center(images);
    std::vector<std::vector<MultiCatDetection>> results;
    for (auto& a : cats) {
      std::vector<MultiCatDetection> res;
      std::transform(a.cbegin(), a.cend(), std::back_inserter(res),
                     [](const arctern::MultiCatDetection& ret) {
                       MultiCatDetection md;
                       md.box = { ret.box.x, ret.box.y,
                                  ret.box.width, ret.box.height };
                       md.confidence = ret.confidence;
                       md.type = ret.type;
                       md.quality = ret.quality;
                       return md;
                     });
      results.emplace_back(res);
    }
    return results;
  }

  bool predict_face_interactive(const std::vector<cv::Mat>& images,
        const std::vector<FaceInteractiveAction>& action,
        FaceInteractiveLevel lv = FaceInteractiveLevel::INTERRUPTED) {
    return arctern::ArcternManager::predict_face_interactive(images, action, lv);
  }

  std::vector<cv::Mat> crop_aligned_face(const cv::Mat& image) {
    std::vector<cv::Mat> cropped_faces;
    std::vector<std::pair<cv::Rect, float>> faces =
        arctern::ArcternManager::detect_face(image);
    for (const auto& face1 : faces) {
      const auto& face = face1.first;
      std::pair<cv::Vec3f, std::vector<float>> landmark_pose =
          arctern::ArcternManager::predict_face_landmark_pose(image, face);
      assert(landmark_pose.second.size() == NUM_LANDMARKS * 2);
      const auto& landmarks = landmark_pose.second;

      cv::Mat cropped =
          arctern::crop_aligned_face(image, landmarks, 0.375, 0.5, 128, 128);
      cropped_faces.push_back(cropped);
    }

    return cropped_faces;
  }
#ifdef WITH_TF
  //加载鉴黄模型
  void set_safety_model(const std::string& safety1_model_path,
                        const std::string& safety2_model_path,
                        const Parameter& param = Parameter()) {
     manager_->set_safety_model(safety1_model_path, safety2_model_path, param);
  }

  //使用模型预测图片
  std::vector<float> detect_safety(const cv::Mat& image) {
    std::vector<float> result = manager_->detect_safety(image);
    return result;
  }
#endif
  TrackResult track_face(const cv::Mat& image) {
    TrackResult outputs;
    auto results = arctern::ArcternManager::track_face(image);
	  outputs.tracked_count = results.tracked_count;
    std::transform(results.tracking_infos.begin(), results.tracking_infos.end(),
				   std::back_inserter(outputs.tracking_infos),
        [] (const arctern::TrackerInfo& r) {
          TrackerInfo tr;
          tr.id = r.id;
          tr.rect.resize(4);
          tr.rect[0] = r.rect.x;
          tr.rect[1] = r.rect.y;
          tr.rect[2] = r.rect.width;
          tr.rect[3] = r.rect.height;
          size_t sz = r.rect_history.size();
          tr.rect_history.resize(sz);
          for (size_t i = 0; i < sz; ++i) {
            tr.rect_history[i].resize(4);
            tr.rect_history[i][0] = r.rect_history[i].x;
            tr.rect_history[i][1] = r.rect_history[i].y;
            tr.rect_history[i][2] = r.rect_history[i].width;
            tr.rect_history[i][3] = r.rect_history[i].height;
          }
          tr.state = r.state;
          tr.from_det = r.from_det;
          tr.frame_index = r.frame_index;
          return tr;
        });
    std::transform(results.tracked_infos.begin(), results.tracked_infos.end(),
				   std::back_inserter(outputs.tracked_infos),
        [] (const arctern::TrackerInfo& r) {
          TrackerInfo tr;
          tr.id = r.id;
          tr.rect.resize(4);
          tr.rect[0] = r.rect.x;
          tr.rect[1] = r.rect.y;
          tr.rect[2] = r.rect.width;
          tr.rect[3] = r.rect.height;
          size_t sz = r.rect_history.size();
          tr.rect_history.resize(sz);
          for (size_t i = 0; i < sz; ++i) {
            tr.rect_history[i].resize(4);
            tr.rect_history[i][0] = r.rect_history[i].x;
            tr.rect_history[i][1] = r.rect_history[i].y;
            tr.rect_history[i][2] = r.rect_history[i].width;
            tr.rect_history[i][3] = r.rect_history[i].height;
          }
          tr.state = r.state;
          tr.from_det = r.from_det;
          tr.frame_index = r.frame_index;
          return tr;
        });

    return outputs;
  }

  TrackResult track_pedestrian(const cv::Mat& image) {
    TrackResult outputs;
    auto results = arctern::ArcternManager::track_pedestrian(image);
    outputs.tracked_count = results.tracked_count;
    std::transform(results.tracking_infos.begin(), results.tracking_infos.end(),
				   std::back_inserter(outputs.tracking_infos),
        [] (const arctern::TrackerInfo& r) {
          TrackerInfo tr;
          tr.id = r.id;
          tr.rect.resize(4);
          tr.rect[0] = r.rect.x;
          tr.rect[1] = r.rect.y;
          tr.rect[2] = r.rect.width;
          tr.rect[3] = r.rect.height;
          size_t sz = r.rect_history.size();
          tr.rect_history.resize(sz);
          for (size_t i = 0; i < sz; ++i) {
            tr.rect_history[i].resize(4);
            tr.rect_history[i][0] = r.rect_history[i].x;
            tr.rect_history[i][1] = r.rect_history[i].y;
            tr.rect_history[i][2] = r.rect_history[i].width;
            tr.rect_history[i][3] = r.rect_history[i].height;
          }
          tr.state = r.state;
          tr.from_det = r.from_det;
          tr.frame_index = r.frame_index;
          return tr;
        });
    std::transform(results.tracked_infos.begin(), results.tracked_infos.end(),
				   std::back_inserter(outputs.tracked_infos),
        [] (const arctern::TrackerInfo& r) {
          TrackerInfo tr;
          tr.id = r.id;
          tr.rect.resize(4);
          tr.rect[0] = r.rect.x;
          tr.rect[1] = r.rect.y;
          tr.rect[2] = r.rect.width;
          tr.rect[3] = r.rect.height;
          size_t sz = r.rect_history.size();
          tr.rect_history.resize(sz);
          for (size_t i = 0; i < sz; ++i) {
            tr.rect_history[i].resize(4);
            tr.rect_history[i][0] = r.rect_history[i].x;
            tr.rect_history[i][1] = r.rect_history[i].y;
            tr.rect_history[i][2] = r.rect_history[i].width;
            tr.rect_history[i][3] = r.rect_history[i].height;
          }
          tr.state = r.state;
          tr.from_det = r.from_det;
          tr.frame_index = r.frame_index;
          return tr;
        });

    return outputs;
  }

 private:
#ifdef WITH_LICENSE
  ArcternManager(const std::string& license_file) : arctern::ArcternManager(license_file) {
  }
#else
  ArcternManager() : arctern::ArcternManager() {}
#endif
};

PYBIND11_MODULE(_arctern, m) {
  NDArrayConverter::init_numpy();

  py::enum_<ErrorCode>(m, "ErrorCode")
      .value("SUCCEED", ErrorCode::SUCCEED)
      .value("MODLE_CORRUPTED", ErrorCode::MODLE_CORRUPTED)
      .value("INVALID_LICENSE", ErrorCode::INVALID_LICENSE);

  py::enum_<DeviceType>(m, "DeviceType")
      .value("CPU", DeviceType::DT_CPU)
      .value("GPU", DeviceType::DT_GPU);

  py::enum_<Gender>(m, "Gender")
      .value("FEMALE", Gender::FEMALE)
      .value("MALE", Gender::MALE)
      .value("UNKNOWN", Gender::GENDER_UNKNOWN);

  py::enum_<Hat>(m, "Hat")
      .value("NOHAT", Hat::NOHAT)
      .value("DETECTED", Hat::DETECTED);

  py::enum_<Glass>(m, "Glass")
      .value("NOGLASS", Glass::NOGLASS)
      .value("TYPE1", Glass::TYPE1)
      .value("TYPE2", Glass::TYPE2);

  py::enum_<FaceAngle>(m, "FaceAngle")
      .value("ANGLE_UP", FaceAngle::ANGLE_UP)
      .value("ANGLE_LEFT", FaceAngle::ANGLE_LEFT)
      .value("ANGLE_DOWN", FaceAngle::ANGLE_DOWN)
      .value("ANGLE_RIGHT", FaceAngle::ANGLE_RIGHT);

  py::enum_<TrackerState>(m, "TrackerState")
      .value("NEW", TrackerState::NEW)
      .value("TRACKED", TrackerState::TRACKED)
      .value("LOST", TrackerState::LOST)
      .value("REMOVED", TrackerState::REMOVED)
      .value("REPLACED", TrackerState::REPLACED);

  py::enum_<Expression>(m, "Expression")
    .value("NEUTRAL", Expression::NEUTRAL)
    .value("JOY", Expression::JOY);

  py::enum_<ExpressionType>(m, "ExpressionType")
    .value("ANGRY", ExpressionType::ANGRY)
    .value("DISGUST", ExpressionType::DISGUST)
    .value("FEAR", ExpressionType::FEAR)
    .value("HAPPY", ExpressionType::HAPPY)
    .value("SAD", ExpressionType::SAD)
    .value("SURPRISE", ExpressionType::SURPRISE)
    .value("NEUTRAL_EXP", ExpressionType::NEUTRAL_EXP);

  py::enum_<NoiseType>(m, "NoiseType")
    .value("MOTIONBLUR", NoiseType::MOTIONBLUR)
    .value("NOISY", NoiseType::NOISY)
    .value("NORMAL", NoiseType::NORMAL);

  py::enum_<AbnomalLightType>(m, "AbnomalLightType")
    .value("BACK", AbnomalLightType::BACK)
    .value("FRONT", AbnomalLightType::FRONT)
    .value("DARK", AbnomalLightType::DARK)
    .value("SIDE", AbnomalLightType::SIDE)
    .value("NORMAL_LIGHT", AbnomalLightType::NORMAL_LIGHT);

  py::enum_<AbnormalExposureType>(m, "AbnormalExposureType")
    .value("LIGHTNORMAL", AbnormalExposureType::LIGHTNORMAL)
    .value("OVEREXPOSED", AbnormalExposureType::OVEREXPOSED)
    .value("UNDEREXPOSED", AbnormalExposureType::UNDEREXPOSED);

  py::enum_<PersonType>(m, "PersonType")
    .value("UNKNOWN", PersonType::PT_UNKNOWN)
    .value("PERSON", PersonType::PT_PERSON);

  py::enum_<FaceInteractiveAction>(m, "FaceInteractiveAction")
    .value("MOUTH_OPEN", FaceInteractiveAction::MOUTH_OPEN)
    .value("FACE_LEFT", FaceInteractiveAction::FACE_LEFT)
    .value("FACE_RIGHT", FaceInteractiveAction::FACE_RIGHT);

  py::enum_<FaceInteractiveLevel>(m, "FaceInteractiveLevel")
    .value("INTERRUPTED", FaceInteractiveLevel::INTERRUPTED)
    .value("CONTINUOUS", FaceInteractiveLevel::CONTINUOUS)
    .value("COMPLETE", FaceInteractiveLevel::COMPLETE);

  py::enum_<FashionType>(m, "FashionType")
    .value("FT_UNKNOWN", FashionType::FT_UNKNOWN)
    .value("FT_BLOUSE", FashionType::FT_BLOUSE)
    .value("FT_CASUALPANTS", FashionType::FT_CASUALPANTS)
    .value("FT_CHAQUETA", FashionType::FT_CHAQUETA)
    .value("FT_DOWNJACKET", FashionType::FT_DOWNJACKET)
    .value("FT_DRESS", FashionType::FT_DRESS)
    .value("FT_JEANS", FashionType::FT_JEANS)
    .value("FT_OVERCOAT", FashionType::FT_OVERCOAT)
    .value("FT_TSHIRT", FashionType::FT_TSHIRT)
    .value("FT_BLAZER", FashionType::FT_BLAZER)
    .value("FT_SUITPANT", FashionType::FT_SUITPANT)
    .value("FT_WINDCOAT", FashionType::FT_WINDCOAT)
    .value("FT_POLO", FashionType::FT_POLO)
    .value("FT_JACKET", FashionType::FT_JACKET);

  py::enum_<FashionStyleType>(m, "FashionStyleType")
    .value("FST", FashionStyleType::FST)
    .value("CASUAL", FashionStyleType::CASUAL)
    .value("COMMUTING", FashionStyleType::COMMUTING)
    .value("KOREA", FashionStyleType::KOREA)
    .value("PUNK", FashionStyleType::PUNK)
    .value("SPORT", FashionStyleType::SPORT);

  py::enum_<CameraOcclusionType>(m, "CameraOcclusionType")
      .value("CAMERA_NORMAL", CameraOcclusionType::CAMERA_NORMAL)
      .value("CAMERA_OCCLUDED", CameraOcclusionType::CAMERA_OCCLUDED);

  py::enum_<MultiCatType>(m, "MultiCatType")
    .value("BIKE", MultiCatType::BIKE)
    .value("CAR", MultiCatType::CAR)
    .value("LICENSE", MultiCatType::LICENSE)
    .value("HUMAN", MultiCatType::HUMAN)
    .value("FACE", MultiCatType::FACE)
    .value("HEAD_HUMAN", MultiCatType::HEAD_HUMAN);

  py::enum_<MouthAction>(m, "MouthAction")
    .value("CLOSE", MouthAction::CLOSE)
    .value("OPEN", MouthAction::OPEN);

  py::enum_<SimilarlyMappingType>(m, "SimilarlyMappingType")
    .value("SIMAP_IDENTITY", SimilarlyMappingType::SIMAP_IDENTITY)
    .value("SIMAP_LINEAR", SimilarlyMappingType::SIMAP_LINEAR)
    .value("SIMAP_NORMAL", SimilarlyMappingType::SIMAP_NORMAL)
    .value("SIMAP_RADICAL", SimilarlyMappingType::SIMAP_RADICAL)
    .value("SIMAP_CONSERVATIVE", SimilarlyMappingType::SIMAP_CONSERVATIVE)
    .value("SIMAP_NORMAL_12POINTS", SimilarlyMappingType::SIMAP_NORMAL_12POINTS);


  py::class_<FaceAttribute> face_attribute(m, "FaceAttribute");
  face_attribute.def(py::init<>())
    .def_readwrite("gender", &FaceAttribute::gender)
    .def_readwrite("age", &FaceAttribute::age);

  py::class_<FaceFeatParams> facefeatparams(m, "FaceFeatParams");
  facefeatparams.def(py::init<>())
    .def_readwrite("trtContextId", &FaceFeatParams::trtContextId)
    .def_readwrite("withMask", &FaceFeatParams::withMask)
    .def_readwrite("maskWipeH", &FaceFeatParams::maskWipeH)
    .def_readwrite("landmarks", &FaceFeatParams::landmarks);

  py::class_<TrackerInfo> tracking_info(m, "TrackerInfo");
  tracking_info.def(py::init<>())
    .def_readwrite("rect", &TrackerInfo::rect)
    .def_readwrite("rect_history", &TrackerInfo::rect_history)
    .def_readwrite("id", &TrackerInfo::id)
    .def_readwrite("from_det", &TrackerInfo::from_det)
    .def_readwrite("state", &TrackerInfo::state)
    .def_readwrite("frame_index", &TrackerInfo::frame_index);

  py::class_<TrackResult> tracking_result(m, "TrackResult");
  tracking_result.def(py::init<>())
    .def_readwrite("tracked_count", &TrackResult::tracked_count)
    .def_readwrite("tracking_infos", &TrackResult::tracking_infos)
    .def_readwrite("tracked_infos", &TrackResult::tracked_infos);

  py::class_<FaceDetection> face_dect(m, "FaceDetection");
  face_dect.def(py::init<>())
      .def_readwrite("box", &FaceDetection::box)
      .def_readwrite("confidence", &FaceDetection::confidence)
      .def_readwrite("gender", &FaceDetection::gender)
      .def_readwrite("age", &FaceDetection::age)
      .def_readwrite("quality", &FaceDetection::quality)
      .def_readwrite("hat", &FaceDetection::hat)
      .def_readwrite("glass", &FaceDetection::glass)
      .def_readwrite("pose", &FaceDetection::pose)
      .def_readwrite("feat", &FaceDetection::feat)
      .def_readwrite("landmarks", &FaceDetection::landmarks);

  py::class_<PersonDetection> person_dect(m, "PersonDetection");
  person_dect.def(py::init<>())
      .def_readwrite("box", &PersonDetection::box)
      .def_readwrite("confidence", &PersonDetection::confidence)
      .def_readwrite("top_occluded_prob", &PersonDetection::top_occluded_prob)
      .def_readwrite("bottom_occluded_prob",
                     &PersonDetection::bottom_occluded_prob)
      .def_readwrite("type", &PersonDetection::type);

  py::class_<PersonCompletenessInfo> person_completenessinfo(m, "PersonCompletenessInfo");
  person_completenessinfo.def(py::init<>())
      .def_readwrite("truncation", &PersonCompletenessInfo::truncation)
      .def_readwrite("visible_parts", &PersonCompletenessInfo::visible_parts)
      .def_readwrite("soft_mask", &PersonCompletenessInfo::soft_mask);

  py::class_<BikeDetection> bike_dect(m, "BikeDetection");
  bike_dect.def(py::init<>())
      .def_readwrite("box", &BikeDetection::box)
      .def_readwrite("confidence", &BikeDetection::confidence);

  py::class_<ElectricBikeDetection> ebike_dect(m, "ElectricBikeDetection");
  ebike_dect.def(py::init<>())
      .def_readwrite("box", &ElectricBikeDetection::box)
      .def_readwrite("confidence", &ElectricBikeDetection::confidence);

  py::class_<FashionDetection> fashion_detection(m, "FashionDetection");
  fashion_detection.def(py::init<>())
      .def_readwrite("box", &FashionDetection::box)
      .def_readwrite("confidence", &FashionDetection::confidence)
      .def_readwrite("type", &FashionDetection::type);

  py::class_<MultiCatDetection> multicat_detection(m, "MultiCatDetection");
  multicat_detection.def(py::init<>())
      .def_readwrite("box", &MultiCatDetection::box)
      .def_readwrite("confidence", &MultiCatDetection::confidence)
      .def_readwrite("type", &MultiCatDetection::type)
      .def_readwrite("quality", &MultiCatDetection::quality);

  py::class_<AnalyzeResult> analyze_result(m, "AnalyzeResult");
  analyze_result.def(py::init<>())
      .def_readwrite("faces", &AnalyzeResult::faces)
      .def_readwrite("persons", &AnalyzeResult::persons)
      .def_readwrite("bikes", &AnalyzeResult::bikes)
      .def_readwrite("electric_bikes", &AnalyzeResult::electric_bikes);

  py::class_<Parameter> parameter(m, "Parameter");
  parameter.def(py::init<>())
      .def_readwrite("device_type", &Parameter::device_type)
      .def_readwrite("device_id", &Parameter::device_id)
      .def_readwrite("batch_size", &Parameter::batch_size);

  py::class_<FaceDetectorParameter, Parameter> face_dect_param(
      m, "FaceDetectorParameter");
  face_dect_param.def(py::init<>())
      .def_readwrite("min_size", &FaceDetectorParameter::min_size)
      .def_readwrite("pnet_threshold", &FaceDetectorParameter::pnet_threshold)
      .def_readwrite("rnet_threshold", &FaceDetectorParameter::rnet_threshold)
      .def_readwrite("onet_threshold", &FaceDetectorParameter::onet_threshold)
      .def_readwrite("pyramid_scale", &FaceDetectorParameter::pyramid_scale)
      .def_readwrite("init_scale", &FaceDetectorParameter::init_scale);

  py::class_<FaceDetectorYolov3Parameter, Parameter> face_dect_yolov3_param(
      m, "FaceDetectorYolov3Parameter");
  face_dect_yolov3_param.def(py::init<>())
      .def_readwrite("conf_thresh", &FaceDetectorYolov3Parameter::conf_thresh);

  py::class_<MultiDetectorParameter, Parameter> multi_cat_detector_param(
      m, "MultiDetectorParameter");
  multi_cat_detector_param.def(py::init<>())
      .def_readwrite("CONF_THRESH_PARAM_", &MultiDetectorParameter::CONF_THRESH_PARAM_);

  py::class_<ObjectTrackerParameter> objectrackerparameter(
          m, "ObjectTrackerParameter");
      objectrackerparameter.def(py::init<>())
          .def_readwrite("detect_period", &ObjectTrackerParameter::detect_period)
          .def_readwrite("detect_id", &ObjectTrackerParameter::detect_id)
          .def_readwrite("min_ap_dist", &ObjectTrackerParameter::min_ap_dist)
          .def_readwrite("min_approved_overlap_ratio",
                  &ObjectTrackerParameter::min_approved_overlap_ratio)
          .def_readwrite("max_time_lost", &ObjectTrackerParameter::max_time_lost)
          .def_readwrite("use_tracking", &ObjectTrackerParameter::use_tracking)
          .def_readwrite("single_tracker_type",
                  &ObjectTrackerParameter::single_tracker_type)
          .def_readwrite("use_refind", &ObjectTrackerParameter::use_refind)
          .def_readwrite("gating_thres", &ObjectTrackerParameter::gating_thres)
          .def_readwrite("choose_size", &ObjectTrackerParameter::choose_size);

  py::class_<FaceTrackerParameter, ObjectTrackerParameter> face_track_param(
     m, "FaceTrackerParameter");
  face_track_param.def(py::init<>())
      .def_readwrite("face_det_param", &FaceTrackerParameter::face_det_param);

  py::class_<PedestrianTrackerParameter, ObjectTrackerParameter> pedestrian_track_param(
     m, "PedestrianTrackerParameter");
  pedestrian_track_param.def(py::init<>())
      .def_readwrite("pedestrian_det_param",
              &PedestrianTrackerParameter::pedestrian_det_param);
  
  py::class_<FaceInteractiveParameter, Parameter> face_interactive_predctor_param(
      m, "FaceInteractiveParameter");
  face_interactive_predctor_param.def(py::init<>())
      .def_readwrite("num_frame", &FaceInteractiveParameter::num_frame)
      .def_readwrite("min_face", &FaceInteractiveParameter::min_face)
      .def_readwrite("face_angle_threshold", &FaceInteractiveParameter::face_angle_threshold)
      .def_readwrite("mouth_angle_threshold", &FaceInteractiveParameter::mouth_angle_threshold)
      .def_readwrite("mouth_open_threshold", &FaceInteractiveParameter::mouth_open_threshold);

  m.def("read_image", &read_image, "A function that read an image",
        py::arg("image"));
  m.def("show_image", &show_image, "A function that show an image",
        py::arg("image"));
  m.def("passthru", &passthru, "Passthru function", py::arg("image"));

  m.def("clone", &cloneimg, "Clone function", py::arg("image"));

  m.def("compute_similarity", &compute_similarity,
        "Computes the similarity of two encrypted features",
        py::arg("feat1"), py::arg("feat2"));

  py::class_<ArcternManager> arctern_manager(m, "ArcternManager");
  arctern_manager
#ifdef WITH_LICENSE
      .def(py::init(&ArcternManager::create), py::arg("license_file"))
#else
      .def(py::init(&ArcternManager::create))
#endif
      .def("set_face_det_model", &ArcternManager::set_face_det_model,
           "Sets face detection model",
           py::arg("model_path"), py::arg("param") = FaceDetectorParameter())
      .def("set_face_tiny_det_model", &ArcternManager::set_face_tiny_det_model,
           "Sets face tiny detection model",
           py::arg("model_path"), py::arg("param") = FaceDetectorYolov3Parameter())
      .def("set_face_feat_model", &ArcternManager::set_face_feat_model,
           "Sets face features model",
           py::arg("model_path"), py::arg("param") = Parameter())
      .def("set_face_expression_model",
           &ArcternManager::set_face_expression_model,
           "Sets face expression model",
           py::arg("model_path"), py::arg("param") = Parameter())
      .def("set_face_expression_recognization_model",
           &ArcternManager::set_face_expression_recognization_model,
           "Sets face expression recognization model",
           py::arg("model_path"), py::arg("param") = Parameter())
      .def("set_face_mask_model", &ArcternManager::set_face_mask_model,
           "Sets face mask model",
           py::arg("model_path"), py::arg("param") = Parameter())
      .def("set_face_age_model", &ArcternManager::set_face_age_model,
           "Sets face age model",
           py::arg("model_path"), py::arg("param") = Parameter())
      .def("set_face_gender_model", &ArcternManager::set_face_gender_model,
           "Sets face gender model",
           py::arg("model_path"), py::arg("param") = Parameter())
      .def("set_face_pose_quality_model",
           &ArcternManager::set_face_pose_quality_model,
           "Sets face pose quality model",
           py::arg("model_path"), py::arg("param") = Parameter())
      .def("set_face_glass_model", &ArcternManager::set_face_glass_model,
           "Sets face glass model",
           py::arg("model_path"), py::arg("param") = Parameter())
      .def("set_face_hat_model", &ArcternManager::set_face_hat_model,
           "Sets face hat model",
           py::arg("model_path"), py::arg("param") = Parameter())
      .def("set_face_landmark_model", &ArcternManager::set_face_landmark_model,
           "Sets face landmark model",
           py::arg("model_path"), py::arg("param") = Parameter())
      .def("set_facial_landmark_model", &ArcternManager::set_facial_landmark_model,
           "Sets facial landmark model",
           py::arg("model_path"), py::arg("param") = Parameter())
      .def("set_facial_landmark_tiny_7_model", 
           &ArcternManager::set_facial_landmark_tiny_7_model,
           "Sets facial landmark tiny 7 model",
           py::arg("model_path"), py::arg("param") = Parameter())
      .def("set_facial_landmark_detect_model",
            &ArcternManager::set_facial_landmark_detect_model,
           "Sets facial landmark detect model",
           py::arg("model_path"), py::arg("param") = Parameter())
      .def("set_face_quality_model",
          &ArcternManager::set_face_quality_model,
          "Sets face quality model",
           py::arg("model_path"), py::arg("param") = Parameter())
      .def("set_face_interactive_model",
           &ArcternManager::set_face_interactive_model,
           "Sets face interactive predictor model", 
           py::arg("face_dect_model"), py::arg("face_landmark_model"),
           py::arg("param") = FaceInteractiveParameter())
      .def("set_face_attributes_model",
           &ArcternManager::set_face_attributes_model,
           "Sets single face attributes model",
           py::arg("model_path"), py::arg("param") = Parameter())
      .def("set_face_gender_age_model",
           &ArcternManager::set_face_gender_age_model,
           "Sets face gender-age model",
           py::arg("model_path"), py::arg("param") = Parameter())
      .def("set_face_noise_motionblur_classifier_model",
           &ArcternManager::set_face_noise_motionblur_classifier_model,
           "Sets single face noise motionblur model",
           py::arg("model_path"), py::arg("param") = Parameter())
      .def("set_face_pose_cls_model",
          &ArcternManager::set_face_pose_cls_model,
          "Sets face pose classification model",
           py::arg("model_path"), py::arg("param") = Parameter())
      .def("set_face_liveness_ir_model",
          &ArcternManager::set_face_liveness_ir_model,
          "Sets face liveness ir model",
           py::arg("model_path"), py::arg("param") = Parameter())
      .def("set_face_image_color_model",
          &ArcternManager::set_face_image_color_model,
          "Sets face image color model",
           py::arg("model_path"), py::arg("param") = Parameter())
      .def("set_face_angle_model",
          &ArcternManager::set_face_angle_model,
          "Sets face angle model",
           py::arg("model_path"), py::arg("param") = Parameter())
      .def("set_faceocc_attributes_model",
          &ArcternManager::set_faceocc_attributes_model,
          "Sets faceocc attributes model",
           py::arg("model_path"), py::arg("param") = Parameter())
      .def("set_face_abnomal_light_classifier_model",
          &ArcternManager::set_face_abnomal_light_classifier_model,
          "Sets face abnormal light classifier model",
           py::arg("model_path"), py::arg("param") = Parameter())
      .def("set_face_3dlandmark_pose_model",
          &ArcternManager::set_face_3dlandmark_pose_model,
          "Sets face 3dlandmark pose model",
           py::arg("model_path"), py::arg("param") = Parameter())
      .def("set_mouth_action_model",
          &ArcternManager::set_mouth_action_model,
          "Sets mouth action model",
           py::arg("model_path"), py::arg("param") = Parameter())
      .def("set_face_extendmultiattribute_model",
          &ArcternManager::set_face_extendmultiattribute_model,
          "Sets face extendmultiattr model",
           py::arg("model_path"), py::arg("param") = Parameter())
      
      .def("set_person_det_model",
           &ArcternManager::set_person_det_model,
           "Sets person detection model",
           py::arg("model_path"), py::arg("param") = Parameter())
      .def("set_person_tiny_det_model",
           &ArcternManager::set_person_tiny_det_model,
           "Sets person detection tiny model",
           py::arg("model_path"), py::arg("param") = Parameter())
      .def("set_person_feat_model", &ArcternManager::set_person_feat_model,
           "Sets person features model",
           py::arg("model_path"), py::arg("param") = Parameter())
      .def("set_person_quality_predictor", &ArcternManager::set_person_quality_predictor,
           "Sets person quality predictor",
           py::arg("model_path"), py::arg("param") = Parameter())
      .def("set_person_completeness_model", &ArcternManager::set_person_completeness_model,
           "Sets person completeness model",
           py::arg("model_path"), py::arg("param") = Parameter())
      .def("set_person_attributes_model", &ArcternManager::set_person_attributes_model,
           "Sets person attributes model",
           py::arg("model_path"), py::arg("param") = Parameter())
      .def("set_bike_det_model", &ArcternManager::set_bike_det_model,
           "Sets bike detection model",
           py::arg("model_path"), py::arg("param") = Parameter())
      .def("set_car_types_model", &ArcternManager::set_car_types_model,
           "Sets car types model",
           py::arg("model_path"), py::arg("param") = Parameter())
      .def("set_car_attributes_model", &ArcternManager::set_car_attributes_model,
           "Sets car attributes model",
           py::arg("model_path"), py::arg("param") = Parameter())
      .def("set_multi_cat_tiny_det_model", &ArcternManager::set_multi_cat_tiny_det_model,
           "Sets multi cat tiny detect model",
           py::arg("model_path"), py::arg("param") = MultiDetectorParameter())
      .def("set_multi_cat_det_center_model", &ArcternManager::set_multi_cat_det_center_model,
           "Sets multi cat detect center model",
           py::arg("model_path"), py::arg("param") = MultiDetectorParameter())
      .def("set_body_face_head_det_center_model", &ArcternManager::set_body_face_head_det_center_model,
           "Sets body-face-head detect center model",
           py::arg("model_path"), py::arg("param") = MultiDetectorParameter())
      .def("set_fashion_det_model", &ArcternManager::set_fashion_det_model,
           "Sets fashion detection model",
           py::arg("model_path"), py::arg("param") = Parameter())
      .def("set_fashionstyle_det_model", &ArcternManager::set_fashionstyle_det_model,
           "Sets fashionstyle detection model",
           py::arg("model_path"), py::arg("param") = Parameter())
      .def("set_face_tracker_model",
           &ArcternManager::set_face_tracker_model,
           "Sets face tracker models",
           py::arg("det_model"), py::arg("feat_model"), 
           py::arg("param") = FaceTrackerParameter())
      .def("set_pedestrian_tracker_model",
           &ArcternManager::set_pedestrian_tracker_model,
           "Sets pedestrian models",
           py::arg("det_model"), py::arg("feat_model"),
           py::arg("param") = PedestrianTrackerParameter())
      
      .def("set_similarly_mapping",
           &ArcternManager::set_similarly_mapping,
           "Sets Similarly mapping",
           py::arg("threshold"), py::arg("type") = SimilarlyMappingType::SIMAP_NORMAL)
      .def("similarly_mapping",
           &ArcternManager::similarly_mapping,
           py::arg("raw_similarly"))
      .def("detect_face",
           (std::vector<std::pair<std::vector<int>, float>>(ArcternManager::*)(
               const cv::Mat&)) &
               ArcternManager::detect_face,
           "A function to detect face", py::arg("image"))
      .def("detect_face_tiny",
           (std::vector<std::pair<std::vector<int>, float>>(ArcternManager::*)(
               const cv::Mat&)) &
               ArcternManager::detect_face_tiny,
           "A function to detect face using yolo tiny model", py::arg("image"))
      .def("predict_face_landmark_pose",
           (std::pair<std::vector<float>, std::vector<float>>(
               ArcternManager::*)(const cv::Mat&, const std::vector<int>&)) &
               ArcternManager::predict_face_landmark_pose,
           "A function to predict face landmark pose", py::arg("image"),
           py::arg("face"))
      .def( "predict_face_mask",
           (std::pair<bool, float>   (ArcternManager::*)(const cv::Mat&, const std::vector<int>&)) &
              ArcternManager::predict_face_mask,
           "A function to predict face mask", py::arg("image"), py::arg("face"))
      .def( "predict_face_age",
           (float (ArcternManager::*)(const cv::Mat&, const std::vector<int>&)) &
              ArcternManager::predict_face_age,
           "A function to predict face age", py::arg("image"), py::arg("face"))
      .def("predict_face_expression",
            (std::pair<Expression, float>(ArcternManager::*)(
             const cv::Mat&, const std::vector<int>&, const std::vector<float>&))
            &ArcternManager::predict_face_expression,
            "A function to predict face expression according to landmarks",
            py::arg("image"), py::arg("face"), py::arg("landmarks"))
      .def("predict_face_expression",
            (std::pair<ExpressionType, float>(ArcternManager::*)(
             const cv::Mat&, const std::vector<int>&))
            &ArcternManager::predict_face_expression,
            "A function to predict face expression",
            py::arg("image"), py::arg("face"))
      .def("predict_face_quality_pose",
           (std::pair<float, std::vector<float>>(ArcternManager::*)(
               const cv::Mat&, const std::vector<int>&)) &
               ArcternManager::predict_face_quality_pose,
           "A function to predict face quality pose", py::arg("image"),
           py::arg("face"))
      .def("predict_face_gender",
           (std::pair<Gender, float>(ArcternManager::*)(
               const cv::Mat&, const std::vector<int>&)) &
               ArcternManager::predict_face_gender,
           "A function to predict face gender", py::arg("image"),
           py::arg("face"))
      .def("predict_face_glass",
           (std::pair<Glass, float>(ArcternManager::*)(
               const cv::Mat&, const std::vector<int>&)) &
               ArcternManager::predict_face_glass,
           "A function to predict face glass", py::arg("image"),
           py::arg("face"))
      .def("predict_face_hat",
           (std::pair<Hat, float>(ArcternManager::*)(const cv::Mat&,
                                                     const std::vector<int>&)) &
               ArcternManager::predict_face_hat,
           "A function to predict face hat", py::arg("image"), py::arg("face"))
      .def("predict_face_angle",
           (std::pair<FaceAngle, float>(ArcternManager::*)(const cv::Mat&,
                                                     const std::vector<int>&)) &
            ArcternManager::predict_face_angle,
           "A function to predict face angle", py::arg("image"), py::arg("face"))
      .def("predict_face_quality",
           (float(ArcternManager::*)(const cv::Mat&, const std::vector<int>&))
              &ArcternManager::predict_face_quality, "A function to predict face quality",
              py::arg("image"), py::arg("face"))
      .def("predict_face_attributes",
           (FaceAttribute(ArcternManager::*)(
            const cv::Mat&, const std::vector<int>&)) &
            ArcternManager::predict_face_attributes,
            "A function to predict face attributes", 
            py::arg("image"), py::arg("face"))
      .def("predict_face_gender_age",
           (FaceAttribute(ArcternManager::*)(
            const cv::Mat&, const std::vector<int>&)) &
            ArcternManager::predict_face_gender_age,
            "A function to predict face gender age", 
            py::arg("image"), py::arg("face"))
      .def("get_face_feat",
           (std::vector<uint8>(ArcternManager::*)(const cv::Mat&,
                const std::vector<int>&, 
                FaceFeatParams facefeatparams = FaceFeatParams()))
            &ArcternManager::get_face_feat,
           "A function to get face feature", 
           py::arg("image"), py::arg("face"), py::arg("facefeatparams") = FaceFeatParams())
      .def("predict_face_noise_motionblur",
            (std::pair<NoiseType, float>(ArcternManager::*)(
             const cv::Mat&, const std::vector<int>&))
            &ArcternManager::predict_face_noise_motionblur,
            "A function to predict face noise motionblur",
            py::arg("image"), py::arg("face"))
      .def("predict_facial_landmark",
            (std::vector<float>(ArcternManager::*)(
             const cv::Mat&, const std::vector<int>&))
            &ArcternManager::predict_facial_landmark,
            "A function to predict facial landmark",
            py::arg("image"), py::arg("face"))
      .def("predict_facial_landmark_tiny_7",
            (std::vector<float>(ArcternManager::*)(
             const cv::Mat&, const std::vector<int>&))
            &ArcternManager::predict_facial_landmark_tiny_7,
            "A function to predict facial landmark tiny 7",
            py::arg("image"), py::arg("face"))
      .def("predict_facial_landmark_detect",
            (std::vector<float>(ArcternManager::*)(
             const cv::Mat&, const std::vector<int>&))
            &ArcternManager::predict_facial_landmark_detect,
            "A function to predict facial landmark detect",
            py::arg("image"), py::arg("face"))
      .def("predict_face_pose_cls",
           (std::vector<std::pair<int, float>>(ArcternManager::*)(
            const cv::Mat&, const std::vector<int>&)) &
            ArcternManager::predict_face_pose_cls,
            "A function to predict face pose classification", 
            py::arg("image"), py::arg("face"))
      .def( "predict_face_liveness_ir",
           (float (ArcternManager::*)(const cv::Mat&, const std::vector<int>&)) &
              ArcternManager::predict_face_liveness_ir,
           "A function to predict face liveness ir", py::arg("image"), py::arg("face"))
      .def( "predict_face_image_color",
           (float (ArcternManager::*)(const cv::Mat&, const std::vector<int>&)) &
              ArcternManager::predict_face_image_color,
           "A function to predict face image color", py::arg("image"), py::arg("face"))
      .def( "predict_faceocc_attributes",
           (std::vector<std::pair<std::string, std::pair<int, float>>> (ArcternManager::*)(
            const cv::Mat&, const std::vector<int>&)) &
            ArcternManager::predict_faceocc_attributes,
           "A function to predict faceocc attributes", py::arg("image"), py::arg("face"))
      .def("predict_face_abnomal_light",
           (std::pair<AbnomalLightType, float>(ArcternManager::*)(const cv::Mat&,
                                                     const std::vector<int>&)) &
               ArcternManager::predict_face_abnomal_light,
           "A function to predict face abnormal light", py::arg("image"), py::arg("face"))
      .def("predict_face_3dlandmark_pose",
           (std::vector<float>(ArcternManager::*)(const cv::Mat&,
                                                  const std::vector<int>&)) &
               ArcternManager::predict_face_3dlandmark_pose,
           "A function to predict face 3dlandmark pose", py::arg("image"), py::arg("face"))
      .def("predict_mouth_action",
           (std::pair<MouthAction, float>(ArcternManager::*)(const cv::Mat&,
                                                     const std::vector<int>&)) &
               ArcternManager::predict_mouth_action,
           "A function to predict mouth action", py::arg("image"), py::arg("face"))
      .def("predict_face_extendmultiattribute",
           (std::map<std::string, std::pair<int, float>>(ArcternManager::*)(const cv::Mat&,
                                                     const std::vector<int>&)) &
               ArcternManager::predict_face_extendmultiattribute,
           "A function to predict face extendmultiattr", py::arg("image"), py::arg("face"))
      
      .def("detect_image_abnormalexposure",
           (std::vector<std::pair<AbnormalExposureType, float>>(ArcternManager::*)(
            const cv::Mat&, const std::vector<int>&, const std::vector<float>&)) &
            ArcternManager::detect_image_abnormalexposure,
            "A function to detect image abnormal or exposure", 
            py::arg("image"), py::arg("face"), py::arg("config"))
     
      .def("detect_person",
           (std::vector<PersonDetection>(ArcternManager::*)(
               const cv::Mat& image)) &
               ArcternManager::detect_person,
           "A function to detect person", py::arg("image"))
      .def("get_pedestrian_feat",
           (std::vector<uint8>(ArcternManager::*)(const cv::Mat&,
                                                  const std::vector<int>&)) &
               ArcternManager::get_pedestrian_feat,
           "A function to get pedestrian feature", py::arg("image"),
           py::arg("face"))
      .def( "predict_person_quality",
           (float (ArcternManager::*)(const cv::Mat&, const std::vector<int>&)) &
              ArcternManager::predict_person_quality,
           "A function to predict person quality", py::arg("image"), py::arg("person"))
      .def( "predict_person_completeness",
           (PersonCompletenessInfo (ArcternManager::*)(const cv::Mat&, const std::vector<int>&)) &
              ArcternManager::predict_person_completeness,
           "A function to predict person completeness", py::arg("image"), py::arg("person"))
      .def( "predict_person_attributes",
           (std::vector<std::pair<std::string, std::pair<std::string, float>>> 
            (ArcternManager::*)(const cv::Mat&, const std::vector<int>&, const std::vector<float>)) &
           ArcternManager::predict_person_attributes,
           "A function to predict person attributes", 
           py::arg("image"), py::arg("person"), py::arg("pad_scale"))
      .def("detect_person_tiny",
           (std::vector<PersonDetection>(ArcternManager::*)(
               const cv::Mat&)) & ArcternManager::detect_person_tiny,
           "A function to detect person with tiny model", py::arg("image"))
      
      .def("detect_bike",
           (std::vector<std::pair<std::vector<int>, float>>(ArcternManager::*)(
               const cv::Mat&)) &
               ArcternManager::detect_bike,
           "A function to detect bike", py::arg("image"))
      .def("predict_car_types",
           (std::vector<std::pair<std::string, float>>(ArcternManager::*)(
            const cv::Mat&, const std::vector<int>&)) &
            ArcternManager::predict_car_types,
            "A function to predict car types", 
            py::arg("image"), py::arg("car_roi"))
      .def("predict_car_attributes",
           (std::vector<std::pair<std::string, std::pair<std::string, float>>>(ArcternManager::*)(
            const cv::Mat&, const std::vector<int>&)) &
            ArcternManager::predict_car_attributes,
            "A function to predict car attributes", 
            py::arg("image"), py::arg("car_roi"))
      .def("detect_fashion",
           (std::vector<FashionDetection>(ArcternManager::*)(
               const cv::Mat&)) & ArcternManager::detect_fashion,
           "A function to detect fashion", py::arg("image"))
      .def("detect_fashionstyle",
           (std::pair<FashionStyleType, float>(ArcternManager::*)(
               const cv::Mat&)) & ArcternManager::detect_fashionstyle,
           "A function to detect fashionstyle type", py::arg("image"))
      .def("detect_multi_tiny_cat",
           (std::vector<MultiCatDetection>(ArcternManager::*)(
               const cv::Mat&)) & ArcternManager::detect_multi_tiny_cat,
           "A function to detect multi categories tiny", py::arg("image"))
      .def("detect_multi_cat_center",
           (std::vector<MultiCatDetection>(ArcternManager::*)(
               const cv::Mat&)) & ArcternManager::detect_multi_cat_center,
           "A function to detect multi categories center", py::arg("image"))
      .def("detect_body_face_head_center",
           (std::vector<MultiCatDetection>(ArcternManager::*)(
               const cv::Mat&)) & ArcternManager::detect_body_face_head_center,
           "A function to detect body-face-head center", py::arg("image"))
      
      .def("predict_face_interactive",
           (bool(ArcternManager::*)(const std::vector<cv::Mat>&,
            const std::vector<FaceInteractiveAction>&, FaceInteractiveLevel lv)) &
            ArcternManager::predict_face_interactive,
           "A function to predict face interactive living",
           py::arg("image"), py::arg("action"), 
           py::arg("level") = FaceInteractiveLevel::INTERRUPTED)
      
      .def("detect_face_batch",
           (std::vector<std::vector<std::pair<std::vector<int>, float>>>(ArcternManager::*)(
            const std::vector<cv::Mat>&)) & ArcternManager::detect_face_batch,
           "A function to detect face", py::arg("images"))
      .def("detect_face_tiny_batch",
           (std::vector<std::vector<std::pair<std::vector<int>, float>>>(ArcternManager::*)(
            const std::vector<cv::Mat>&)) & ArcternManager::detect_face_tiny_batch,
           "A function to detect face using yolo tiny model", py::arg("images"))
      .def("predict_face_landmark_pose_batch",
           (std::vector<std::vector<std::pair<std::vector<float>, std::vector<float>>>>(
            ArcternManager::*)(const std::vector<cv::Mat>&,
            const std::vector<std::vector<std::vector<int>>>&)) &
           ArcternManager::predict_face_landmark_pose_batch,
           "A function to predict face landmark pose", py::arg("images"), py::arg("faces"))
      .def( "predict_face_mask_batch",
           (std::vector<std::vector<std::pair<bool, float>>>  (ArcternManager::*)(
            const std::vector<cv::Mat>&, const std::vector<std::vector<std::vector<int>>>&)) &
           ArcternManager::predict_face_mask_batch,
           "A function to predict face mask", py::arg("images"), py::arg("faces"))
      .def( "predict_face_age_batch",
           (std::vector<std::vector<float>> (ArcternManager::*)(
            const std::vector<cv::Mat>&, const std::vector<std::vector<std::vector<int>>>&)) &
           ArcternManager::predict_face_age_batch,
           "A function to predict face age", py::arg("images"), py::arg("faces"))
       .def("predict_face_expression_batch",
            (std::vector<std::vector<std::pair<int, float>>>(ArcternManager::*)(
             const std::vector<cv::Mat>&, const std::vector<std::vector<std::vector<int>>>&,
             const std::vector<std::vector<std::vector<float>>>&))
            &ArcternManager::predict_face_expression_batch,
            "A function to predict face expression according to landmarks",
            py::arg("images"), py::arg("faces"), py::arg("landmarks"))
       .def("predict_face_expression_batch",
            (std::vector<std::vector<std::pair<ExpressionType, float>>>(ArcternManager::*)(
                const std::vector<cv::Mat>&, const std::vector<std::vector<std::vector<int>>>&))
            &ArcternManager::predict_face_expression_batch,
            "A function to predict face expression", py::arg("images"), py::arg("faces"))
       .def("predict_face_quality_pose_batch",
           (std::vector<std::vector<std::pair<float, std::vector<float>>>>(ArcternManager::*)(
            const std::vector<cv::Mat>&, const std::vector<std::vector<std::vector<int>>>&))
           &ArcternManager::predict_face_quality_pose_batch,
           "A function to predict face quality pose", py::arg("images"), py::arg("faces"))
      .def("predict_face_gender_batch",
           (std::vector<std::vector<std::pair<Gender, float>>>(ArcternManager::*)(
                const std::vector<cv::Mat>&, const std::vector<std::vector<std::vector<int>>>&))
           &ArcternManager::predict_face_gender_batch,
           "A function to predict face gender", py::arg("images"), py::arg("faces"))
      .def("predict_face_glass_batch",
           (std::vector<std::vector<std::pair<int, float>>>(ArcternManager::*)(
                const std::vector<cv::Mat>&, const std::vector<std::vector<std::vector<int>>>&))
           &ArcternManager::predict_face_glass_batch,
           "A function to predict face glass", py::arg("images"), py::arg("faces"))
      .def("predict_face_hat_batch",
           (std::vector<std::vector<std::pair<Hat, float>>>(ArcternManager::*)(
            const std::vector<cv::Mat>&, const std::vector<std::vector<std::vector<int>>>&))
           &ArcternManager::predict_face_hat_batch,
           "A function to predict face hat", py::arg("images"), py::arg("faces"))
      .def("predict_face_angle_batch",
           (std::vector<std::vector<std::pair<FaceAngle, float>>>(ArcternManager::*)(
            const std::vector<cv::Mat>&, const std::vector<std::vector<std::vector<int>>>&))
           &ArcternManager::predict_face_angle_batch,
           "A function to predict face angle", py::arg("images"), py::arg("faces"))
      .def("predict_face_quality_batch",
           (std::vector<std::vector<float>>(ArcternManager::*)(
                const std::vector<cv::Mat>&, const std::vector<std::vector<std::vector<int>>>&))
            &ArcternManager::predict_face_quality_batch,
            "A function to predict face quality", py::arg("images"), py::arg("faces"))
      .def("predict_face_attributes_batch",
              (std::vector<std::vector<FaceAttribute>>(ArcternManager::*)(
                  const std::vector<cv::Mat>&, const std::vector<std::vector<std::vector<int>>>)) &
             ArcternManager::predict_face_attributes_batch,
             "A function to predict face attributes", py::arg("images"), py::arg("faces"))
      .def("predict_face_gender_age_batch",
              (std::vector<std::vector<FaceAttribute>>(ArcternManager::*)(
                  const std::vector<cv::Mat>&, const std::vector<std::vector<std::vector<int>>>)) &
             ArcternManager::predict_face_gender_age_batch,
             "A function to predict face gender age", py::arg("images"), py::arg("faces"))
      .def("get_face_feat_batch",
           (std::vector<std::vector<std::vector<uint8>>>(ArcternManager::*)(
            const std::vector<cv::Mat>&, 
            const std::vector<std::vector<std::vector<int>>>&,
            FaceFeatParams facefeatparams = FaceFeatParams()))
           &ArcternManager::get_face_feat_batch,
           "A function to get face feature", 
           py::arg("images"), py::arg("faces"), 
           py::arg("facefeatparams") = FaceFeatParams())
      .def("predict_face_noise_motionblur_batch",
            (std::vector<std::vector<std::pair<NoiseType, float>>>(ArcternManager::*)(
                const std::vector<cv::Mat>&, const std::vector<std::vector<std::vector<int>>>&))
            &ArcternManager::predict_face_noise_motionblur_batch,
            "A function to predict face noise motionblur", py::arg("images"), py::arg("faces"))
      .def("predict_facial_landmark_batch",
            (std::vector<std::vector<std::vector<float>>>(ArcternManager::*)(
             const std::vector<cv::Mat>&, const std::vector<std::vector<std::vector<int>>>&))
            &ArcternManager::predict_facial_landmark_batch,
            "A function to predict facial landmark",
            py::arg("images"), py::arg("faces"))
      .def("predict_facial_landmark_tiny_7_batch",
            (std::vector<std::vector<std::vector<float>>>(ArcternManager::*)(
             const std::vector<cv::Mat>&, const std::vector<std::vector<std::vector<int>>>&))
            &ArcternManager::predict_facial_landmark_tiny_7_batch,
            "A function to predict facial landmark tiny 7",
            py::arg("images"), py::arg("faces"))
      .def("predict_facial_landmark_detect_batch",
            (std::vector<std::vector<std::vector<float>>>(ArcternManager::*)(
             const std::vector<cv::Mat>&, const std::vector<std::vector<std::vector<int>>>&))
            &ArcternManager::predict_facial_landmark_detect_batch,
            "A function to predict facial landmark detect",
            py::arg("images"), py::arg("faces"))
      .def("predict_face_pose_cls_batch",
           (std::vector<std::vector<std::vector<std::pair<int, float>>>>(ArcternManager::*)(
            const std::vector<cv::Mat>&, const std::vector<std::vector<std::vector<int>>>&)) &
            ArcternManager::predict_face_pose_cls_batch,
            "A function to predict face pose classification", 
            py::arg("images"), py::arg("faces"))
      .def("predict_face_liveness_ir_batch",
           (std::vector<std::vector<float>> (ArcternManager::*)(
            const std::vector<cv::Mat>&, const std::vector<std::vector<std::vector<int>>>&)) &
           ArcternManager::predict_face_liveness_ir_batch,
           "A function to predict face liveness ir", py::arg("images"), py::arg("faces"))
      .def("predict_face_image_color_batch",
           (std::vector<std::vector<float>> (ArcternManager::*)(
            const std::vector<cv::Mat>&, const std::vector<std::vector<std::vector<int>>>&)) &
           ArcternManager::predict_face_image_color_batch,
           "A function to predict face image color", py::arg("images"), py::arg("faces"))
      .def("predict_faceocc_attributes_batch",
           (std::vector<std::vector<std::vector<std::pair<std::string, std::pair<int, float>>>>> (ArcternManager::*)(
            const std::vector<cv::Mat>&, const std::vector<std::vector<std::vector<int>>>&)) &
           ArcternManager::predict_faceocc_attributes_batch,
           "A function to predict faceocc attributes", py::arg("images"), py::arg("faces"))
      .def("predict_face_abnomal_light_batch",
           (std::vector<std::vector<std::pair<AbnomalLightType, float>>>(ArcternManager::*)(
            const std::vector<cv::Mat>&, const std::vector<std::vector<std::vector<int>>>&))
           &ArcternManager::predict_face_abnomal_light_batch,
           "A function to predict face abnormal light", py::arg("images"), py::arg("faces"))
      
      .def("predict_face_3dlandmark_pose_batch",
           (std::vector<std::vector<std::vector<float>>>(ArcternManager::*)(
            const std::vector<cv::Mat>&, const std::vector<std::vector<std::vector<int>>>&))
           &ArcternManager::predict_face_3dlandmark_pose_batch,
           "A function to predict face 3dlandmark pose", py::arg("images"), py::arg("faces"))
      .def("predict_face_extendmultiattribute_batch",
           (std::vector<std::vector<std::map<std::string, std::pair<int, float>>>>(ArcternManager::*)(
            const std::vector<cv::Mat>&, const std::vector<std::vector<std::vector<int>>>&)) &
               ArcternManager::predict_face_extendmultiattribute_batch,
           "A function to predict face extendmultiattr", py::arg("images"), py::arg("faces"))

      .def("detect_image_abnormalexposure_batch",
           (std::vector<std::vector<std::vector<std::pair<AbnormalExposureType, float>>>>(ArcternManager::*)(
            const std::vector<cv::Mat>&, 
            const std::vector<std::vector<std::vector<int>>>&,
            const std::vector<float>&)) &
            ArcternManager::detect_image_abnormalexposure_batch,
            "A function to detect image abnormal or exposure", 
            py::arg("images"), py::arg("faces"), py::arg("config"))
     
      .def("detect_person_batch",
           (std::vector<std::vector<PersonDetection>>(ArcternManager::*)(
               const std::vector<cv::Mat>& image)) & ArcternManager::detect_person_batch,
           "A function to detect person", py::arg("images"))
      .def("get_pedestrian_feat_batch",
           (std::vector<std::vector<std::vector<uint8>>>(ArcternManager::*)(
            const std::vector<cv::Mat>&, const std::vector<std::vector<std::vector<int>>>&))
           &ArcternManager::get_pedestrian_feat_batch,
           "A function to get pedestrian feature", py::arg("images"), py::arg("persons"))
      .def( "predict_person_quality_batch",
           (std::vector<std::vector<float>> (ArcternManager::*)(
            const std::vector<cv::Mat>&, const std::vector<std::vector<std::vector<int>>>&)) &
           ArcternManager::predict_person_quality_batch,
           "A function to predict person quality", py::arg("images"), py::arg("person"))
      .def( "predict_person_completeness_batch",
           (std::vector<std::vector<PersonCompletenessInfo>> (ArcternManager::*)(
            const std::vector<cv::Mat>&, const std::vector<std::vector<std::vector<int>>>&)) &
           ArcternManager::predict_person_completeness_batch,
           "A function to predict person completeness", py::arg("images"), py::arg("person"))
      .def( "predict_person_attributes_batch",
           (std::vector<std::vector<std::vector<std::pair<std::string, std::pair<std::string, float>>> >>
            (ArcternManager::*)(const std::vector<cv::Mat>&, const std::vector<std::vector<std::vector<int>>>&, 
                const std::vector<std::vector<std::vector<float>>>)) &
           ArcternManager::predict_person_attributes_batch,
           "A function to predict person attributes", 
           py::arg("images"), py::arg("person"), py::arg("pad_scales"))
      .def("detect_person_tiny_batch",
           (std::vector<std::vector<PersonDetection>>(ArcternManager::*)(
               const std::vector<cv::Mat>&)) & ArcternManager::detect_person_tiny_batch,
           "A function to detect person with tiny model", py::arg("images"))
      
      .def("detect_bike_batch",
           (std::vector<std::vector<std::pair<std::vector<int>, float>>>(ArcternManager::*)(
               const std::vector<cv::Mat>&)) & ArcternManager::detect_bike_batch,
           "A function to detect bike", py::arg("images"))
      .def("predict_car_types_batch",
           (std::vector<std::vector<std::vector<std::pair<std::string, float>>>>(ArcternManager::*)(
            const std::vector<cv::Mat>&, const std::vector<std::vector<std::vector<int>>>&)) &
            ArcternManager::predict_car_types_batch,
            "A function to predict car types", 
            py::arg("images"), py::arg("car_rois"))
      .def("predict_car_attributes_batch",
           (std::vector<std::vector<std::vector<std::pair<std::string, std::pair<std::string, float>>>>>(ArcternManager::*)(
            const std::vector<cv::Mat>&, const std::vector<std::vector<std::vector<int>>>&)) &
            ArcternManager::predict_car_attributes_batch,
            "A function to predict car attributes", 
            py::arg("images"), py::arg("car_rois"))
      .def("detect_fashion_batch",
           (std::vector<std::vector<FashionDetection>>(ArcternManager::*)(
                const std::vector<cv::Mat>&)) & ArcternManager::detect_fashion_batch,
           "A function to detect fashion", py::arg("images"))
      .def("detect_fashionstyle_batch",
           (std::vector<std::pair<FashionStyleType, float>>(ArcternManager::*)(
               const std::vector<cv::Mat>&)) & ArcternManager::detect_fashionstyle_batch,
           "A function to detect fashionstyle type", py::arg("images"))
      .def("detect_multi_tiny_cat_batch",
           (std::vector<std::vector<MultiCatDetection>>(ArcternManager::*)(
            const std::vector<cv::Mat>&)) & ArcternManager::detect_multi_tiny_cat_batch,
           "A function to detect multi categories tiny", py::arg("images"))
      .def("detect_multi_cat_center_batch",
           (std::vector<std::vector<MultiCatDetection>>(ArcternManager::*)(
            const std::vector<cv::Mat>&)) & ArcternManager::detect_multi_cat_center_batch,
           "A function to detect multi categories center", py::arg("images"))
      .def("detect_body_face_head_center_batch",
           (std::vector<std::vector<MultiCatDetection>>(ArcternManager::*)(
            const std::vector<cv::Mat>&)) & ArcternManager::detect_body_face_head_center_batch,
           "A function to detect body-face-head center", py::arg("images"))
      
      .def("crop_aligned_face", &ArcternManager::crop_aligned_face)
#ifdef WITH_TF
      .def("set_safety_model", &ArcternManager::set_safety_model)
      .def("detect_safety", &ArcternManager::detect_safety)
#endif
      .def("track_face", (TrackResult(ArcternManager::*)(
                const cv::Mat&))&ArcternManager::track_face,
           "A function to track face", py::arg("image"))
      .def("track_pedestrian", (TrackResult(ArcternManager::*)(
                const cv::Mat&))&ArcternManager::track_pedestrian,
           "A function to track pedestrian", py::arg("image"));
}
