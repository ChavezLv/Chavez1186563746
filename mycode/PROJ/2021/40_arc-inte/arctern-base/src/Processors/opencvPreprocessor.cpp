//
// Created by gh on 2021/10/31.
//

#include "opencvPreprocessor.h"
#include "alignFace.h"

namespace arctern {
OpencvPreprocessorRunRt::OpencvPreprocessorRunRt() {

}

OpencvPreprocessorRunRt::~OpencvPreprocessorRunRt() {

}

OpencvPreprocessor::OpencvPreprocessor() {

}

ErrorType OpencvPreprocessor::Init(OpencvPreprocessorInitPt *p) {
  init_pt_ = *p;
  return ERR_SUCCESS;
}

std::unique_ptr<OpencvPreprocessorRet> OpencvPreprocessor::Process(OpencvPreprocessorRunRt *p) {
  std::unique_ptr<OpencvPreprocessorRet> result;
  if (nullptr == p || p->image_.empty()) {
    return result;
  }
  for (auto type : p->sequence_) {
    //std::cout<<"--> type"<<static_cast<std::underlying_type<OperatorType>::type>(type)<<std::endl;
    switch (type) {
      case OperatorType::cvtColor :cvtColor(p);
        break;
      case OperatorType::crop :crop(p);
        break;
      case OperatorType::affine:affine(p);
        break;
      case OperatorType::normalized:normalize(p);
        break;
      case OperatorType::padding:padding(p);
        break;
      case OperatorType::detFace :detFace(p);
        break;
      case OperatorType::resized :resize(p);
        break;
      case OperatorType::twiceResized:twiceResize(p);
        break;
      case OperatorType::faceExp:faceExp(p);
        break;
      case OperatorType::multiCat:multiCat(p);
        break;
      case OperatorType::splitNormalized:splitNormalized(p);
        break;
      case OperatorType::faceWipe:faceWipe(p);
        break;
      case OperatorType::faceJoy:faceJoy(p);
        break;
      case OperatorType::faceFeat : {
        faceFeat(p);
        break;
      }
      default:break;

    }
  }

  result.reset(new OpencvPreprocessorRet);
  result->mat_ = p->image_;
  return result;
}

int OpencvPreprocessor::calcMatrix(OpencvPreprocessorRunRt *pRunParam, double dArray[2][3]) {
  return 0;
}

int OpencvPreprocessor::cvtColor(OpencvPreprocessorRunRt *pRunParam) {
  if (pRunParam->cvt_type_ == -1) {
    return -1;
  }
  switch (pRunParam->cvt_type_) {
    case -1:return -1;
    case BGR2RGB:
    case RGB2BGR:cv::cvtColor(pRunParam->image_, pRunParam->image_, cv::COLOR_BGR2RGB);
      break;
    case BGR2GRAY:cv::cvtColor(pRunParam->image_, pRunParam->image_, cv::COLOR_BGR2GRAY);
      break;
    case RGB2GRAY :cv::cvtColor(pRunParam->image_, pRunParam->image_, cv::COLOR_RGB2GRAY);
      break;
    default:return -1;
  }

  return 0;
}

int OpencvPreprocessor::crop(OpencvPreprocessorRunRt *pRunParam) {
  cv::Rect roi(pRunParam->roi_.x, pRunParam->roi_.y, pRunParam->roi_.width, pRunParam->roi_.height);
  pRunParam->image_ = pRunParam->image_(roi).clone();
  return 0;
}
int OpencvPreprocessor::resize(OpencvPreprocessorRunRt *pRunParam) {
  cv::resize(pRunParam->image_, pRunParam->image_, cv::Size(init_pt_.net_w_, init_pt_.net_h_));
  return 0;
}
int OpencvPreprocessor::affine(OpencvPreprocessorRunRt *pRunParam) {

  auto &lmk = pRunParam->lmk_;
  auto &img = pRunParam->image_;
  // auto format = p->format_;

  assert(lmk.size() == init_pt_.lmk_.size());

  cv::Mat align_face;
  cv::Mat msrc2(2, 2, CV_32F);
  cv::Mat mdst2(2, 2, CV_32F);
  float *sd = msrc2.ptr<float>();
  float *md = mdst2.ptr<float>();

  sd[0] = init_pt_.lmk_[0].x;
  sd[1] = init_pt_.lmk_[0].y;
  sd[2] = init_pt_.lmk_[1].x;
  sd[3] = init_pt_.lmk_[1].y;

  md[0] = lmk[0].x;
  md[1] = lmk[0].y;
  md[2] = lmk[1].x;
  md[3] = lmk[1].y;

  cv::Mat M_out = similarTransform(mdst2, msrc2);

  cv::Mat M(2, 3, CV_32F);
  for (auto i = 0; i < M.rows; ++i) {
    for (auto j = 0; j < M.cols; ++j) {
      M.at<float>(i, j) = M_out.at<float>(i, j);
      // cout << M.at<float>(i,j)<<endl;
    }
  }

  const auto input_w = init_pt_.net_w_;
  const auto input_h = init_pt_.net_h_;
  // printfImg<uchar>(img);

  cv::warpAffine(img, align_face, M, cv::Size(input_w, input_h));
  pRunParam->image_ = align_face;
  return 0;
}
int OpencvPreprocessor::normalize(OpencvPreprocessorRunRt *pRunParam) {
  cv::Mat fimage;
  pRunParam->image_.convertTo(fimage, cv::DataType<float>::type,
                              1.0 / init_pt_.std_, -1.0 * (init_pt_.mean_ / init_pt_.std_));
  pRunParam->image_ = fimage;
  return 0;
}
int OpencvPreprocessor::padding(OpencvPreprocessorRunRt *pRunParam) {
  return 0;
}
int OpencvPreprocessor::extractChannel(OpencvPreprocessorRunRt *pRunParam) {

  return 0;
}

int OpencvPreprocessor::detFace(OpencvPreprocessorRunRt *p) {
  auto &img = p->image_;
  const auto input_w = init_pt_.net_w_;
  const auto input_h = init_pt_.net_h_;
  const auto alignment = init_pt_.alignMent_;
  const auto borderValue = init_pt_.border_value;

  cv::Mat resized;

  if (input_w == input_h) {
    int dest_w(input_w), dest_h(input_h);
    int padbottom(0), padright(0);
    int padtop = 0;
    int padleft = 0;
    if (img.cols > img.rows) {
      dest_h = static_cast<int>(img.rows * input_h / static_cast<float>(img.cols));
      padbottom = input_h - dest_h;
      padbottom = padbottom >= 0 ? padbottom : 0;
    } else {
      dest_w = static_cast<int>(img.cols * input_w / static_cast<float>(img.rows));
      padright = input_w - dest_w;
      padright = padright >= 0 ? padright : 0;
    }

    if (alignment == AlignMent::AlignCenter) {
      padtop = padbottom / 2;
      padleft = padright / 2;
      padbottom -= padtop;
      padright -= padleft;
    }

    cv::resize(img, resized, cv::Size(dest_w, dest_h), 0.0f, 0.0f);
    cv::copyMakeBorder(resized,
                       resized,
                       padtop,
                       padbottom,
                       padleft,
                       padright,
                       cv::BORDER_CONSTANT,
                       cv::Scalar(borderValue, borderValue, borderValue));
  } else {
    cv::resize(img, resized, cv::Size(input_w, input_h));
  }
  p->image_ = resized;

  return 0;
}

int OpencvPreprocessor::merge(OpencvPreprocessorRunRt *pRunParam) {
  return 0;
}

int OpencvPreprocessor::splitNormalized(OpencvPreprocessorRunRt *pRunParam) {
  cv::Mat &img = pRunParam->image_;
  cv::Mat fimage;
  const float scale = 1.0 / 255;
  img.convertTo(fimage, cv::DataType<float>::type, scale);
  int num_lines = fimage.cols * fimage.channels();
  for (int r = 0; r < fimage.rows; ++r) {
    float *data = fimage.ptr<float>(r);
    for (int c = 0; c < num_lines;) {
      data[c] = (data[c] - init_pt_.meanV_[0]) / init_pt_.stdV_[0];
      data[c + 1] = (data[c + 1] - init_pt_.meanV_[1]) / init_pt_.stdV_[1];
      data[c + 2] = (data[c + 2] - init_pt_.meanV_[2]) / init_pt_.stdV_[2];
      c += 3;
    }
  }

  pRunParam->image_ = fimage;

  return 0;
}

int OpencvPreprocessor::twiceResize(OpencvPreprocessorRunRt *pRunParam) {
  auto &img = pRunParam->image_;
  const auto input_w = init_pt_.net_w_;
  const auto input_h = init_pt_.net_h_;

  cv::Mat resize_image;
  if (img.cols > input_w * 4 || img.rows > input_h * 4) {
    cv::Mat temp_buf_mat;
    cv::resize(img, temp_buf_mat, cv::Size(input_w * 2, input_h * 2),
               cv::INTER_AREA);
    cv::resize(temp_buf_mat, resize_image, cv::Size(input_w, input_h),
               cv::INTER_AREA);
  } else {
    cv::resize(img, resize_image, cv::Size(input_w, input_h));
  }
  pRunParam->image_ = resize_image;
  return 0;
}

int OpencvPreprocessor::multiCat(OpencvPreprocessorRunRt *pRunParam) {
  auto &img = pRunParam->image_;
  const auto mean = init_pt_.mean_;
  const auto std = init_pt_.std_;

  const auto borderValue = init_pt_.border_value;

  cv::Mat resized;
  const int w = init_pt_.net_w_;
  const int h = init_pt_.net_h_;
  const int img_width = img.cols;
  const int img_height = img.rows;
  int new_w = img_width;
  int new_h = img_height;

  if ((static_cast<float>(w) / img_width) <
      (static_cast<float>(h) / img_height)) {
    new_w = w;
    new_h = (img_height * w) / img_width;
  } else {
    new_h = h;
    new_w = (img_width * h) / img_height;
  }

  cv::resize(img, resized, cv::Size(new_w, new_h), 0.0f, 0.0f);

  cv::Mat boxed = cv::Mat(h, w, img.type(), CV_RGB(borderValue, borderValue, borderValue));
  unsigned char *psrc = reinterpret_cast<unsigned char *>(resized.data);
  unsigned char *pdst = reinterpret_cast<unsigned char *>(boxed.data);
  const int offset_w = (w - new_w) >> 1;
  const int offset_h = (h - new_h) >> 1;

  int offset_bytes = offset_w * boxed.channels();
  int line_bytes = boxed.cols * boxed.channels();
  int resize_line_bytes = resized.cols * resized.channels();
  int index_y = (offset_h - 1) * line_bytes + offset_bytes;
  int index_y1 = -resize_line_bytes;
  for (int y = 0; y < resized.rows; ++y) {
    index_y += line_bytes;
    index_y1 += resize_line_bytes;
    memcpy(pdst + index_y, psrc + index_y1, resize_line_bytes);
  }

  pRunParam->image_ = boxed;
  return 0;
}
int OpencvPreprocessor::faceJoy(OpencvPreprocessorRunRt *pRunParam) {
  //auto derivedP = dynamic_cast<AttrPreProRunParameter *>(p);

  auto &lmk = pRunParam->lmk_;
  auto &image = pRunParam->image_;
  //auto format = p->format_;

  auto rotate_point = [](int ref_x, int ref_y, int x, int y, float angle,
                         int &rotated_x, int &rotated_y) {
    float x0 = x - ref_x;
    float y0 = y - ref_y;
    rotated_x = x0 * std::cos(angle) - y0 * std::sin(angle) + ref_x;
    rotated_y = x0 * std::sin(angle) + y0 * std::cos(angle) + ref_y;
  };

  const float offset_ratio_x = 0.375f;
  const float offset_ratio_y = 0.5f;
  const int exptected_width = 64;
  const int exptected_height = 64;

  const int &left_eye_left_corner_x = lmk[0].x;
  const int &left_eye_left_corner_y = lmk[0].y;
  const int &left_eye_right_corner_x = lmk[1].x;
  const int &left_eye_right_corner_y = lmk[1].y;
  const int &right_eye_left_corner_x = lmk[2].x;
  const int &right_eye_left_corner_y = lmk[2].y;
  const int &right_eye_right_corner_x = lmk[3].x;
  const int &right_eye_right_corner_y = lmk[3].y;
  const int &nose_x = lmk[4].x;
  const int &nose_y = lmk[4].y;
  const int &mouse_left_corner_x = lmk[5].x;
  const int &mouse_left_corner_y = lmk[5].y;
  const int &mouse_right_corner_x = lmk[6].x;
  const int &mouse_right_corner_y = lmk[6].y;

  int left_eye_x = (left_eye_left_corner_x + left_eye_right_corner_x) / 2;
  int left_eye_y = (left_eye_left_corner_y + left_eye_right_corner_y) / 2;
  int right_eye_x = (right_eye_left_corner_x + right_eye_right_corner_x) / 2;
  int right_eye_y = (right_eye_left_corner_y + right_eye_right_corner_y) / 2;
  int eye_x = (left_eye_x + right_eye_x) / 2;
  int eye_y = (left_eye_y + right_eye_y) / 2;
  int mouse_x = (mouse_left_corner_x + mouse_right_corner_x) / 2;
  int mouse_y = (mouse_left_corner_y + mouse_right_corner_y) / 2;

  float reference_height = exptected_height * 0.375f;
  assert(reference_height >= 1);

  float angle = std::atan2(static_cast<float>(right_eye_y - left_eye_y),
                           static_cast<float>(right_eye_x - left_eye_x)) *
      180.0f / M_PI;

  cv::Mat rotation_matrix =
      cv::getRotationMatrix2D(cv::Point(nose_x, nose_y), angle, 1);
  cv::Mat rotated_image;
  cv::warpAffine(image, rotated_image, rotation_matrix, image.size());

  angle *= -1.0f / 180.0f * M_PI;

  int rotated_eye_x;
  int rotated_eye_y;
  int rotated_mouth_x;
  int rotated_mouth_y;

  rotate_point(nose_x, nose_y, eye_x, eye_y, angle, rotated_eye_x,
               rotated_eye_y);
  rotate_point(nose_x, nose_y, mouse_x, mouse_y, angle, rotated_mouth_x,
               rotated_mouth_y);

  int rotated_image_width = rotated_image.cols;
  int rotated_image_height = rotated_image.rows;

  float scale = reference_height * 1.0f / (rotated_mouth_y - rotated_eye_y);

  cv::resize(rotated_image, rotated_image, cv::Size(), scale, scale);
  rotated_eye_x = (rotated_eye_x - rotated_image_width / 2) * scale +
      rotated_image.cols / 2;
  rotated_eye_y = (rotated_eye_y - rotated_image_height / 2) * scale +
      rotated_image.rows / 2;

  cv::Rect box(rotated_eye_x - exptected_width / 2,
               rotated_eye_y - reference_height, exptected_width,
               exptected_height);

  int left = (box.x < 0) ? (-box.x) : 0;
  int top = (box.y < 0) ? (-box.y) : 0;
  int right = (box.x + box.width > rotated_image.cols)
              ? (box.x + box.width - rotated_image.cols)
              : 0;
  int bottom = (box.y + box.height > rotated_image.rows)
               ? (box.y + box.height - rotated_image.rows)
               : 0;
  box.x += left;
  box.y += top;

  cv::copyMakeBorder(rotated_image, rotated_image, top, bottom, left, right,
                     cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0));

  pRunParam->image_ = rotated_image(cv::Rect(2, 2, 60, 60)).clone();
//  result.oMat_ = normalized(rotated_image(box)(cv::Rect(2, 2, 60, 60)), format);
//  result.error_type_ = ErrorType::ERR_SUCCESS;

  return 0;
}
int OpencvPreprocessor::faceExp(OpencvPreprocessorRunRt *pRunParam) {
  cv::Rect crop_rec = cv::Rect(3, 3, 90, 90);
  cv::Mat &resize_image = pRunParam->image_;
  auto crop_image = resize_image(crop_rec);
  cv::Mat mergeMat;
  std::vector<cv::Mat> channel_vec(3, crop_image);
  cv::merge(channel_vec, mergeMat);
  pRunParam->image_ = mergeMat;
  return 0;
}
int OpencvPreprocessor::detFace2(OpencvPreprocessorRunRt *pRunParam) {
  const int input_w = init_pt_.net_w_;
  const int input_h = init_pt_.net_h_;
  cv::Mat &img = pRunParam->image_;
  cv::Mat resized;

  if (input_w == input_h) {
    int dest_w(input_w), dest_h(input_h);
    int padbottom(0), padright(0);
    int padtop = 0;
    int padleft = 0;
    if (img.cols > img.rows) {
      dest_h = static_cast<int>(img.rows * input_h / static_cast<float>(img.cols));
      padbottom = input_h - dest_h;
      padbottom = padbottom >= 0 ? padbottom : 0;
    } else {
      dest_w = static_cast<int>(img.cols * input_w / static_cast<float>(img.rows));
      padright = input_w - dest_w;
      padright = padright >= 0 ? padright : 0;
    }

    if (init_pt_.alignMent_ == AlignMent::AlignCenter) {
      padtop = padbottom / 2;
      padleft = padright / 2;
      padbottom -= padtop;
      padright -= padleft;
    }

    cv::resize(img, resized, cv::Size(dest_w, dest_h), 0.0f, 0.0f);
    cv::copyMakeBorder(resized,
                       resized,
                       padtop,
                       padbottom,
                       padleft,
                       padright,
                       cv::BORDER_CONSTANT,
                       cv::Scalar(114, 114, 114));
  } else {
    cv::resize(img, resized, cv::Size(input_w, input_h));
  }

  pRunParam->image_ = resized;

  return 0;
}
int OpencvPreprocessor::faceFeat(OpencvPreprocessorRunRt *pRunParam) {
  cv::Rect bbox(pRunParam->roi_.x, pRunParam->roi_.y, \
                pRunParam->roi_.width, pRunParam->roi_.height);
  const auto model_w = init_pt_.net_w_;
  const auto model_h = init_pt_.net_h_;
  float resize_ratiow = model_w * 1.0 / bbox.width;
  float resize_ratioh = model_h * 1.0 / bbox.height;

  cv::Mat &image = pRunParam->image_;

  int imgw = image.cols;
  int imgh = image.rows;
  cv::Mat img_resized;
  int new_w = roundf(imgw * resize_ratiow);
  int new_h = roundf(imgh * resize_ratioh);
  cv::resize(image, img_resized, cv::Size(new_w, new_h));

  int crop_x1 = roundf(bbox.tl().x * resize_ratiow);
  int crop_y1 = roundf(bbox.tl().y * resize_ratioh);
  int crop_x2 = roundf((bbox.tl().x + bbox.width) * resize_ratiow);
  int crop_y2 = roundf((bbox.tl().y + bbox.height) * resize_ratioh);

  int ncrop_x = std::max(-crop_x1, 0);
  int ncrop_y = std::max(-crop_y1, 0);

  crop_x1 = crop_x1 < 0 ? 0 : crop_x1;
  crop_y1 = crop_y1 < 0 ? 0 : crop_y1;
  crop_x2 = crop_x2 < new_w ? crop_x2 : new_w;
  crop_y2 = crop_y2 < new_h ? crop_y2 : new_h;

  crop_x2 = crop_x2 - crop_x1 + ncrop_x <= model_w ? crop_x2 : crop_x1 + model_w - ncrop_x;
  crop_y2 = crop_y2 - crop_y1 + ncrop_y <= model_h ? crop_y2 : crop_y1 + model_h - ncrop_y;

  cv::Rect crop_Rect = cv::Rect(cv::Point(crop_x1, crop_y1), cv::Point(crop_x2, crop_y2));
  cv::Mat img_crop = img_resized(crop_Rect).clone();

  cv::Mat canvas = cv::Mat::zeros(model_h, model_w, CV_8UC3);

  cv::Rect roi_rect = cv::Rect(ncrop_x, ncrop_y, img_crop.cols, img_crop.rows);
  img_crop.copyTo(canvas(roi_rect));
  pRunParam->image_ = canvas;
  return 0;
}

int OpencvPreprocessor::faceWipe(OpencvPreprocessorRunRt *pRunParam) {
  cv::Mat &mat = pRunParam->image_;
  cv::Mat wipe_face = mat.clone();
  int w_height;
  w_height = wipe_face.rows * 0.5;
  const float value = -127.5 / 128.0;

  cv::Rect r_low_half(0, w_height, wipe_face.cols, wipe_face.rows - w_height);
  wipe_face(r_low_half).setTo(value);
  pRunParam->image_ = wipe_face;

  return 0;
}

}