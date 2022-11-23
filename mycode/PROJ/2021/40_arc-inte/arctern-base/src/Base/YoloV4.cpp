/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.14
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#include "YoloV4.h"
#include "common/imageConvert.h"
#include "common/common.h"
#include <algorithm>
#include <vector>


namespace arctern {
YoloV4::YoloV4() { }
YoloV4::~YoloV4() {  }
int YoloV4::getConfigInfo(arctern::MxnetModel *model) {
  std::vector<std::string> anchors;
  height_ = 0;
  auto config = model->configs();
  if(config != nullptr){
    for(auto it = config->begin();it != config->end(); it++){
      std::string tmp = it->key()->str();
      if(tmp == "CLASS_NUM")
      {
        num_classes = string2int(it->value()->str());
      }
      else if(tmp == "INPUT_DIM")
      {
        width_ = string2int(it->value()->str());
      }
      else if(tmp == "INPUT_DIM2")
      {
        height_ = string2int(it->value()->str());
      }
      else if(tmp == "NMS_THRESH")
      {
        nms_thresh = string2T<float>(it->value()->str());
      }
      else if(tmp == "CONF_THRESH" && thresh_ <= 0)
      {
        thresh_ = string2T<float>(it->value()->str());
      }
    }
  }

  return 0;
}

int YoloV4::getConfigInfo(arctern::TrtModel *model) {


  return 0;
}

int YoloV4::getNetH() {
  return height_;
}

int YoloV4::getNetW() {
  return width_;
}


void YoloV4::transform_grid(float *prediction) {
  int input_dim = 608;
  std::vector<std::vector<int>> anchors = {{12, 16}, {19, 36}, {40, 28},
                                           {36, 75}, {76, 55}, {72, 146},
                                           {142, 110}, {192, 243}, {459, 401}};
  const std::vector<std::vector<int> > ANCHORS_MASKS9 = {{6, 7, 8}, {3, 4, 5}, {0, 1, 2}};
  std::vector<std::vector<int> > anchors_masks;
  anchors_masks = ANCHORS_MASKS9;
  // for stride 19
  std::vector<int> offset_anchor_19;
  for (int i = 0; i < 19; i++){
    for (int j = 0; j < 19; j++){
      for (int n = 0; n < 3; n++){
        offset_anchor_19.push_back(j); offset_anchor_19.push_back(i);
        offset_anchor_19.push_back(anchors[anchors_masks[0][n]][0]);
        offset_anchor_19.push_back(anchors[anchors_masks[0][n]][1]);
        offset_anchor_19.push_back(0); offset_anchor_19.push_back(0);
        // std::cout<<j<<" "<<i<<" "<<anchors[anchors_masks[0][n]][0]<<" "<<anchors[anchors_masks[0][n]][1]<<std::endl;
      }
    }
  }
  int stride_19 = 19;
  float div_19 = (float)input_dim / (float)stride_19;
  for (int i = 0; i < (1083*6); i+=6){
    if (prediction[i+4] >= thresh_){
      prediction[i] += offset_anchor_19[i]; prediction[i] *= div_19;
      prediction[i+1] += offset_anchor_19[i+1]; prediction[i+1] *= div_19;
      prediction[i+2] = (std::exp(prediction[i+2]) * offset_anchor_19[i+2]);
      prediction[i+3] = (std::exp(prediction[i+3]) * offset_anchor_19[i+3]);
    }}
  // for stride 38
  std::vector<int> offset_anchor_38;
  for (int i = 0; i < 38; i++){
    for (int j = 0; j < 38; j++){
      for (int n = 0; n < 3; n++){
        offset_anchor_38.push_back(j); offset_anchor_38.push_back(i);
        offset_anchor_38.push_back(anchors[anchors_masks[1][n]][0]);
        offset_anchor_38.push_back(anchors[anchors_masks[1][n]][1]);
        offset_anchor_38.push_back(0); offset_anchor_38.push_back(0);
        // std::cout<<j<<" "<<i<<" "<<anchors[anchors_masks[1][n]][0]<<" "<<anchors[anchors_masks[1][n]][1]<<std::endl;
      }
    }
  }
  int stride_38 = 38;
  float div_38 = (float)input_dim / (float)stride_38;
  for (int i = 1083*6; i < (5415*6); i+=6)
  {
    // std::cout<<"here6"<<std::endl;
      /* std::cout << __func__ << ", " << __LINE__ << ", " <<  thresh_ << std::endl; */
    if (prediction[i+4] >= thresh_)
    {
      prediction[i] += offset_anchor_38[i-1083*6]; prediction[i] *= div_38;
      prediction[i+1] += offset_anchor_38[i+1-1083*6]; prediction[i+1] *= div_38;
      prediction[i+2] = (std::exp(prediction[i+2]) * offset_anchor_38[i+2-1083*6]);
      prediction[i+3] = (std::exp(prediction[i+3]) * offset_anchor_38[i+3-1083*6]);
      // std::cout<<offset_anchor_38[i-1083*6]<<" "<<offset_anchor_38[i+1-1083*6]<<" "<<offset_anchor_38[i+2-1083*6]<<" "<<offset_anchor_38[i+3-1083*6]<<std::endl;
    }
  }
  // for stride 76
  std::vector<int> offset_anchor_76;
  for (int i = 0; i < 76; i++)
  {
    for (int j = 0; j < 76; j++)
    {
      for (int n = 0; n < 3; n++)
      {
        offset_anchor_76.push_back(j); offset_anchor_76.push_back(i);
        offset_anchor_76.push_back(anchors[anchors_masks[2][n]][0]);
        offset_anchor_76.push_back(anchors[anchors_masks[2][n]][1]);
        offset_anchor_76.push_back(0); offset_anchor_76.push_back(0);
      }
    }
  }
  int stride_76 = 76;
  float div_76 = (float)input_dim / (float)stride_76;
  for (int i = 5415*6; i < (22743*6); i+=6)
  {
      /* std::cout << __func__ << ", " << __LINE__ << ", " <<  thresh_ << std::endl; */
    if (prediction[i+4] >= thresh_)
    {
      // std::cout<<"here7"<<std::endl;
      prediction[i] += offset_anchor_76[i-5415*6]; prediction[i] *= div_76;
      prediction[i+1] += offset_anchor_76[i+1-5415*6]; prediction[i+1] *= div_76;
      prediction[i+2] = (std::exp(prediction[i+2]) * offset_anchor_76[i+2-5415*6]);
      prediction[i+3] = (std::exp(prediction[i+3]) * offset_anchor_76[i+3-5415*6]);
    }
  }
}

void YoloV4::transform_grid2(float *prediction) {
  int input_dim = 608;
  std::vector<std::vector<int>> anchors = {{12, 16}, {19, 36}, {40, 28},
                                           {36, 75}, {76, 55}, {72, 146},
                                           {142, 110}, {192, 243}, {459, 401}};
  const std::vector<std::vector<int> > ANCHORS_MASKS9 = {{6, 7, 8}, {3, 4, 5}, {0, 1, 2}};
  std::vector<std::vector<int> > anchors_masks;
  anchors_masks = ANCHORS_MASKS9;

  int zeros = 1;
  int len = 5+num_classes;

  // for stride 19
  std::vector<int> offset_anchor_19;
  for (int i = 0; i < 19; i++)
  {
    for (int j = 0; j < 19; j++)
    {
      for (int n = 0; n < 3; n++)
      {
        offset_anchor_19.push_back(j); offset_anchor_19.push_back(i);
        offset_anchor_19.push_back(anchors[anchors_masks[0][n]][0]);
        offset_anchor_19.push_back(anchors[anchors_masks[0][n]][1]);
        for (int m = 0; m < (zeros+num_classes); m++)
          offset_anchor_19.push_back(0);
        // offset_anchor_19.push_back(0); offset_anchor_19.push_back(0);
        // std::cout<<j<<" "<<i<<" "<<anchors[anchors_masks[0][n]][0]<<" "<<anchors[anchors_masks[0][n]][1]<<std::endl;
      }
    }
  }

  // std::cout<<"here1"<<std::endl;
  int stride_19 = 19;
  float div_19 = (float)input_dim / (float)stride_19;
  for (int i = 0; i < (1083*7); i+=7)
  {
    // std::cout<<"here3"<<std::endl;
    if (prediction[i+4] >= thresh_)
    {
      // std::cout<<"here4"<<std::endl;
      prediction[i] += offset_anchor_19[i]; prediction[i] *= div_19;
      prediction[i+1] += offset_anchor_19[i+1]; prediction[i+1] *= div_19;
      prediction[i+2] = (std::exp(prediction[i+2]) * offset_anchor_19[i+2]);
      prediction[i+3] = (std::exp(prediction[i+3]) * offset_anchor_19[i+3]);
    }

  }

  // std::cout<<"here5"<<std::endl;
  // for stride 38
  std::vector<int> offset_anchor_38;
  for (int i = 0; i < 38; i++)
  {
    for (int j = 0; j < 38; j++)
    {
      for (int n = 0; n < 3; n++)
      {
        offset_anchor_38.push_back(j); offset_anchor_38.push_back(i);
        offset_anchor_38.push_back(anchors[anchors_masks[1][n]][0]);
        offset_anchor_38.push_back(anchors[anchors_masks[1][n]][1]);
        offset_anchor_38.push_back(0); offset_anchor_38.push_back(0); offset_anchor_38.push_back(0);
        // std::cout<<j<<" "<<i<<" "<<anchors[anchors_masks[1][n]][0]<<" "<<anchors[anchors_masks[1][n]][1]<<std::endl;
      }
    }
  }

  int stride_38 = 38;
  float div_38 = (float)input_dim / (float)stride_38;
  for (int i = 1083*7; i < (5415*7); i+=7)
  {
    // std::cout<<"here6"<<std::endl;
    if (prediction[i+4] >= thresh_)
    {
      prediction[i] += offset_anchor_38[i-1083*7]; prediction[i] *= div_38;
      prediction[i+1] += offset_anchor_38[i+1-1083*7]; prediction[i+1] *= div_38;
      prediction[i+2] = (std::exp(prediction[i+2]) * offset_anchor_38[i+2-1083*7]);
      prediction[i+3] = (std::exp(prediction[i+3]) * offset_anchor_38[i+3-1083*7]);
      // std::cout<<offset_anchor_38[i-1083*6]<<" "<<offset_anchor_38[i+1-1083*6]<<" "<<offset_anchor_38[i+2-1083*6]<<" "<<offset_anchor_38[i+3-1083*6]<<std::endl;
    }
  }



  // for stride 76
  std::vector<int> offset_anchor_76;
  for (int i = 0; i < 76; i++)
  {
    for (int j = 0; j < 76; j++)
    {
      for (int n = 0; n < 3; n++)
      {
        offset_anchor_76.push_back(j); offset_anchor_76.push_back(i);
        offset_anchor_76.push_back(anchors[anchors_masks[2][n]][0]);
        offset_anchor_76.push_back(anchors[anchors_masks[2][n]][1]);
        offset_anchor_76.push_back(0); offset_anchor_76.push_back(0); offset_anchor_76.push_back(0);
      }
    }
  }

  int stride_76 = 76;
  float div_76 = (float)input_dim / (float)stride_76;
  for (int i = 5415*7; i < (22743*7); i+=7)
  {
    if (prediction[i+4] >= thresh_)
    {
      // std::cout<<"here7"<<std::endl;
      prediction[i] += offset_anchor_76[i-5415*7]; prediction[i] *= div_76;
      prediction[i+1] += offset_anchor_76[i+1-5415*7]; prediction[i+1] *= div_76;
      prediction[i+2] = (std::exp(prediction[i+2]) * offset_anchor_76[i+2-5415*7]);
      prediction[i+3] = (std::exp(prediction[i+3]) * offset_anchor_76[i+3-5415*7]);
    }
  }
}


std::vector<detection> YoloV4::pack_valid_detections(float* prediction) {
  std::vector<int> valid_detections;
  int nstrides = 3;
  int len = 5 + num_classes;
  std::vector<std::vector<int>> step = {{0, 1083}, {1083, 5415}, {5415, 22743}};
  std::vector<std::vector<int>> strides = {{19,19}, {38,38}, {76,76}};
  // find out the number of valid detections
  for (int i = 0; i < step[nstrides - 1][1]; ++i) {
    float score = prediction[i * len + 4];
    if (score > thresh_) {
      /* std::cout << __func__ << ", " << __LINE__ << ", " <<  thresh_ << std::endl; */
      valid_detections.push_back(i);
      /* std::cout << __func__ << " i " << i << std::endl; */
    }
  }
  // convert to box_corner
  std::vector<detection> detections;
  for (int i = 0; i < valid_detections.size(); ++i) {
    detection det;
    int iprediction = valid_detections[i];
    float score = prediction[iprediction * len + 4];
    float x = prediction[iprediction * len + 0];
    float y = prediction[iprediction * len + 1];
    float w = prediction[iprediction * len + 2];
    float h = prediction[iprediction * len + 3];
    det.bbox.x = x;
    det.bbox.y = y;
    det.bbox.width = w;
    det.bbox.height = h;
    det.objectness = score;
    det.prob = prediction + iprediction * len + 5;
    for (int j = 0; j < num_classes; ++j){
      float prob = score * det.prob[j];
      // std::cout<<prob<<" "<<score<<" "<<det.prob[j]<<std::endl;
      det.prob[j] = (prob > thresh_) ? prob : 0;
      /* std::cout << __func__ << ", " << __LINE__ << ", " <<  thresh_ << std::endl; */
    }
    detections.push_back(det);
  }
  return detections;
}
inline float overlap(float x1, float w1, float x2, float w2) {
    float l1 = x1 - w1 / 2;
    float l2 = x2 - w2 / 2;
    float left = l1 > l2 ? l1 : l2;
    float r1 = x1 + w1 / 2;
    float r2 = x2 + w2 / 2;
    float right = r1 < r2 ? r1 : r2;
    return right - left;

}
std::ostream &operator<<(std::ostream &os, const ArcternRect& val){
    os << "[" << val.x << ", " << val.y << ", " << val.width << ", " << val.height << "]";
    return os;
}
void YoloV4::do_nms_sort(detection* dets, int total) {
      /* std::cout << __func__ << ", " << __LINE__ << ", total" <<  total << std::endl; */
  int i, j, k;
  k = total - 1;
  for (i = 0; i <= k; ++i) {
    if (dets[i].objectness == 0) {
      detection swap = dets[i];
      dets[i] = dets[k];
      dets[k] = swap;
      --k;
      --i;
    }
  }
  total = k + 1;

  for (k = 0; k < num_classes; ++k) {
    for (i = 0; i < total; ++i) {
      dets[i].sort_class = k;
    }
    qsort(dets, total, sizeof(detection), nms_comparator);
    for (i = 0; i < total; ++i) {
      if (dets[i].prob[k] == 0) continue;
      ArcternRect a = dets[i].bbox;
      for (j = i + 1; j < total; ++j) {
        ArcternRect b = dets[j].bbox;
        std::cout << a << " vs ";
        std::cout << b << std::endl;
        if (box_iou(a, b) > nms_thresh) {
            std::cout << __LINE__ << box_iou(a, b) << " vs " <<  nms_thresh << std::endl;
            sleep(1);
          dets[j].prob[k] = 0;
        }
      }
    }
  }
}


Vbbox YoloV4::pack_results(int oriWidth, int oriHeight, std::vector<detection>& detections) {
  Vbbox boxes;
  float scaling_factor1,scaling_factor2;
  int len = 5 + num_classes;
  //std::cout<<ori_width<<" "<<ori_height<<std::endl;
  scaling_factor1 = 608 * 1.0f / oriWidth;
  scaling_factor2 = 608 * 1.0f / oriHeight;
  //std::cout<<scaling_factor1<<" "<<scaling_factor2<<std::endl;
  int num = 0;
  for (int i = 0; i < detections.size(); ++i) {
    float x = detections[i].bbox.x;
    float y = detections[i].bbox.y;
    float w = detections[i].bbox.width;
    float h = detections[i].bbox.height;
    float x1 = x - w / 2;
    float y1 = y - h / 2;
    float x2 = x + w / 2;
    float y2 = y + h / 2;
    std::cout<<x1<<" "<<y1<<" "<<x2<<" "<<y2<<std::endl;
    x1 /= scaling_factor1;x2 /= scaling_factor1;
    y1 /= scaling_factor2;y2 /= scaling_factor2;
     std::cout<<x1<<" "<<y1<<" "<<x2<<" "<<y2<<std::endl;
    x1 = std::min(oriWidth, std::max((int)x1, 0));
    x2 = std::min(oriWidth, std::max((int)x2, 0));
    y1 = std::min(oriHeight, std::max((int)y1, 0));
    y2 = std::min(oriHeight, std::max((int)y2, 0));
    float score = 0.0f;
    for (int j = 0; j < num_classes; ++j){
      if (detections[i].prob[j] > 0){
          std::cout << detections[i].prob[j] << std::endl;
        score = detections[i].prob[j];
        Bbox b(x1, y1, x2, y2, score);
        b.label = j;
        boxes.push_back(b);
        std::cout << __func__ << " asdfasdfasd" << b.x1 << ", " << b.x2 << ", " << b.y1 << ", " << b.y2 << std::endl;
        if(3 == num){
            return boxes;
        }
        ++ num;
        std::cout << "num = " << num << std::endl;

      }
    }
  }
  return boxes;
}

Vbbox YoloV4::postProcess(float *data1, int width, int height, float thresh) {

  std::vector<float> out;
  std::string filepath = + "/workspace/result.txt";
  std::ofstream ofs(filepath); 
  for(int i = 0; i < 136458; ++i){
      ofs << *(data1 + i) << std::endl;
  }
  	float *data = (float*)malloc(sizeof(float) * 136458);
    /* memcpy(data, data1, sizeof(float) * 136458 ); */
    std::ifstream ifs("/workspace/result.txt");
    if(!ifs.good()){
        std::cout << "!" << std::endl;
    }
    std::string line;
    std::stringstream ss(line);
    int32_t cnt = 0;
    while(getline(ifs, line)){
        ss.clear();
        ss << line;
        while(ss >> *(data + cnt)){
            /* std::cout << *(data + cnt) << std::endl; */
          ++cnt; 
        }
    }
  Vbbox vbbox;
  if(thresh > 0 && thresh < 1) {
    thresh_ = thresh;
  }
  transform_grid(data);
  std::vector<detection> detections = pack_valid_detections(data);
  int32_t i = 0;
  do_nms_sort(&detections[0], detections.size());
  vbbox = pack_results(width, height, detections);
  std::cout << vbbox.size() << " : " << __func__ << ", " << __LINE__ << std::endl;
  for(auto &box: vbbox){
      std::cout << "[" << box.x1 << ", " << box.x2 << ", " << box.y1 << "," << box.y2 << "]" << std::endl;
  }

    free(data);
  return vbbox;
}
Vbbox YoloV4::postProcess2(float * data, int width, int height, float thresh){
  Vbbox vbbox;
  if(thresh > 0 && thresh < 1) {
    thresh_ = thresh;
  }
  transform_grid2(data);
  std::vector<detection> detections = pack_valid_detections(data);
  do_nms_sort(&detections[0], detections.size());
  vbbox = pack_results(width, height, detections);
  return vbbox;
}

} // namespace arctern
