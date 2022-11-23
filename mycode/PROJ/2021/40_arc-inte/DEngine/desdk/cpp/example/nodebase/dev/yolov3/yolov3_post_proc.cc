#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <string.h>
#include "yolov3_post_proc.h"

namespace de
{
namespace arctern
{

float overlap(float x1, float w1, float x2, float w2) {
	float l1 = x1 - w1 / 2;
	float l2 = x2 - w2 / 2;
	float left = l1 > l2 ? l1 : l2;
	float r1 = x1 + w1/2;
	float r2 = x2  + w2/2;
	float right = r1 < r2 ? r1 : r2;
	return right -left;
}

float box_intersecton(box a, box b) {
    float w = overlap(a.x, a.w, b.x, b.w); 
	float h = overlap(a.y, a.h, b.y, b.h); 
	if(w < 0 || h < 0) return 0;
	return w*h;
}

float box_union(box a, box b) {
    float i = box_intersecton(a, b);
	float u = a.w*a.h + b.w*b.h - i;
	return u;
}

float box_iou(box a, box b) { return box_intersecton(a,b )/box_union(a,b);}

int nms_comparator(const void* pa, const void*pb) {
     detection a = *(detection*)pa;
	 detection b = *(detection*)pb;
	 float diff = 0.;
	 if(b.sort_class >= 0) {
	 	diff = a.prob[b.sort_class] - b.prob[b.sort_class] ;
	 } else {
	    diff = a.objectness - b.objectness;
	 }
	 if(diff < 0) return 1;
	 else if(diff > 0)return -1;
	 return 0;
}


YoloV3PostProc::YoloV3PostProc(
	const int classes ,
	const int input_dimension,
	const std::vector<int> anchors_box ,
	const float conf_thr,
	const float nms_thr,
	const bool tiny_flag,
	const bool square_flag,
	const float extw_scale_face,
	const float exth_scale_face ) {
    
	num_classes = classes;
	input_dim = input_dimension;
	conf_thresh = conf_thr;
	nms_thresh = nms_thr;

	CHECK(anchors_box.size() % 2 == 0);
	for(uint32_t i = 0; i < anchors_box.size(); i+=2) {
		anchors.push_back({anchors_box[i], anchors_box[i+1]});
	}

	tiny_yolov3_flag = tiny_flag;
	square_face_ = square_flag;
	nstrides = tiny_yolov3_flag ? 2 : 3;
	nboxes = anchors.size() / nstrides;
	if (tiny_yolov3_flag)
	    nboxes = 3;

	step.resize(nstrides);

	int pre_step = 0;
	for(int i = 0; i < nstrides; ++i) {
		int stride = input_dim * (int)pow(2, i) / 32;
		strides.push_back(stride);

		step[i].push_back(pre_step);
		pre_step += stride * stride * nboxes;
		step[i].push_back(pre_step);
	}	

   conf_thresh_ = de::arctern::Fp32ToFp16(conf_thresh);
    de::arctern::get_fp16_info(conf_thresh_, conf_thresh_sign, conf_thresh_exponent, conf_thresh_mantissa);
    // LOG(WARNING) << "sign=" << conf_thresh_sign << ", exponent=" << conf_thresh_exponent << ", mantissa=" << conf_thresh_mantissa << ", conf_thresh_=" << conf_thresh_  << std::endl;
    // float conf_thresh_test = de::arctern::Fp16ToFp32(conf_thresh_);
    // LOG(WARNING) << "conf_thresh=" << conf_thresh << ", conf_thresh_test=" << conf_thresh_test;

    pOutSpace = (float *)malloc(8*1024*1024);

}

YoloV3PostProc::~YoloV3PostProc() {

  free(pOutSpace);
  }


Vbbox YoloV3PostProc::run(const std::vector<de::NDArray>& outs, int img_w, int img_h)const {
  int total_size=0;
  if (tiny_yolov3_flag){
	total_size = outs[1].GetTensorDataSize() + outs[0].GetTensorDataSize();
  }else{
	total_size = outs[1].GetTensorDataSize() + outs[2].GetTensorDataSize() + outs[0].GetTensorDataSize();
  }
  int16_t *pPart = NULL;//(int16_t*)data;
  uint32_t num = total_size / sizeof(int16_t);

  // float *pDataFloat = (float*)malloc(sizeof(float) * num);
  float *pDataFloat = pOutSpace ;
  CHECK(pDataFloat);

  int over_th_num = 0;
  int len_one_box = 5+num_classes;

  int part = 0;
  
  // printf("----------nstrides %d nboxes %d\n",nstrides,nboxes);

  //公版模型后处理，shape为1,255,stride,stride
  for (int stride_id = 0; stride_id < nstrides; stride_id++) {
    int stride = strides[stride_id];
    int step = stride * stride;

	if (tiny_yolov3_flag){
		if (stride_id == 0){
			pPart = (int16_t*)outs[1].GetTensorData();
			// printf("tensorsize %d\n", outs[1].GetTensorDataSize());
		}
		else if (stride_id == 1){
			pPart = (int16_t*)outs[0].GetTensorData();
			// printf("tensorsize %d\n", outs[0].GetTensorDataSize());
		}
	}else{
		if (stride_id == 0){
			pPart = (int16_t*)outs[1].GetTensorData();
		}
		else if (stride_id == 1){
			pPart = (int16_t*)outs[2].GetTensorData();
		}
		else if (stride_id == 2)
		  pPart = (int16_t*)outs[0].GetTensorData();
	}
    // printf("stride[%d] %d\n", stride_id, stride);

    for (int i = 0; i < nboxes; i++) {
      for (int j = 0; j < step; j++) {
        float score = de::arctern::Fp16ToFp32(pPart[ i*len_one_box*step + 4*step + j]);
        //int32_t ret = de::arctern::fp16_compare_with_positive_f16(pPart[part + i*len_one_box*step + 4*step + j], conf_thresh_exponent, conf_thresh_mantissa);
        //if (ret>0)
        if(score > conf_thresh) 
        {
          pDataFloat[part + len_one_box*i + 3*len_one_box*j + 0] = de::arctern::Fp16ToFp32(pPart[ i*len_one_box*step + 0*step + j]);
          pDataFloat[part + len_one_box*i + 3*len_one_box*j + 1] = de::arctern::Fp16ToFp32(pPart[ i*len_one_box*step + 1*step + j]);
          pDataFloat[part + len_one_box*i + 3*len_one_box*j + 2] = de::arctern::Fp16ToFp32(pPart[ i*len_one_box*step + 2*step + j]);
          pDataFloat[part + len_one_box*i + 3*len_one_box*j + 3] = de::arctern::Fp16ToFp32(pPart[ i*len_one_box*step + 3*step + j]);
          pDataFloat[part + len_one_box*i + 3*len_one_box*j + 4] = score;
          //pDataFloat[part + len_one_box*i + 3*len_one_box*j + 4] = de::arctern::Fp16ToFp32(pPart[part + i*len_one_box*step + 4*step + j]);
          for(int32_t offset = 5; offset < len_one_box; offset++) {
            pDataFloat[part + len_one_box*i + 3*len_one_box*j + offset] = de::arctern::Fp16ToFp32(pPart[ i*len_one_box*step + offset*step + j]);
          }
          over_th_num++;
          //printf("[%d,%d] score %f, over_th_num %d\n", i, j, score, over_th_num);
        } else {
          pDataFloat[part + len_one_box*i + 3*len_one_box*j + 4] = 0.0;
        }
      }
    }
    
    part += step * len_one_box * 3;
  }
  transform_grid(pDataFloat);

  std::vector<detection> detections = pack_valid_detections(pDataFloat);

  do_nms_sort(&detections[0], detections.size());

  Vbbox boxes = pack_results(detections, img_w, img_h);
  if(square_face_) {
    ConvertSquare(boxes, exth_scale_face_, extw_scale_face_);
  }

  // free(pDataFloat);
  return boxes;
}


//multi
std::vector<de::arctern::MultiCatDetection> YoloV3PostProc::run_multi_cat(const std::vector<de::NDArray>& outs, int32_t img_w, int32_t img_h) const {
	//LOG(INFO) << "dataLen = " << data_len << ", imgW = " << img_w << ", imgH = " << img_h;
	Vbbox boxes = run(outs, img_w, img_h);
	std::vector<de::arctern::MultiCatDetection> rslt;
	for(auto it = boxes.begin(); it != boxes.end(); it++) {
	  deRect roi = {(int)it->x1, (int)it->y1,(int)it->x2 - (int)it->x1 + 1,(int)it->y2 - (int)it->y1 + 1};
	  de::arctern::MultiCatDetection ret;
	  ret.box =check_shrink_box(roi, img_w, img_h);
	  ret.confidence = it->score;
	  ret.type = it->label;
	  // rslt.push_back(ret);
	  if (ret.type == 2||ret.type == 5||ret.type == 7) // car
	  {
		  ret.type = 2l << 8;
		  rslt.push_back(ret);
	  }
	  else if (ret.type == 1) //bike
	  {
		  ret.type = 3l << 8;
		  rslt.push_back(ret);
	  }
	  else 
	  if (ret.type == 0) //person
	  {
		ret.type = 1l << 8;
		rslt.push_back(ret);
	  }else
	  {
		ret.type = 0;
	  }
	// printf("RECT[%d, %d, %d, %d, score: %f, cat type: %d\n", roi.x, roi.y, roi.w, roi.h, it->score, it->label);

	  //LOG(INFO) << "RECT[" << roi.x<<","<<roi.y<<","<<roi.w<<","<<roi.h<<"], conf :" << it->score<<", cat:"<<it->label ;
	}
	//LOG(INFO) << "RECT NUM = " << rslt.size();
	return rslt;
}


void YoloV3PostProc:: do_nms_sort(detection * dets, int total)const {
	int i, j, k;
	k = total - 1;
	for( i = 0; i <= k; ++i) {
		if(dets[i].objectness == 0) {
			detection swap = dets[i];
			dets[i] = dets[k];
		    dets[k] = swap;
		    --k;
		    --i;
		}
	}
	total = k + 1;

	for(k = 0; k < num_classes; ++k) {
		for(i = 0; i < total; i++) {
			dets[i].sort_class = k;
		}
	
    	qsort(dets, total, sizeof(detection), nms_comparator);
    	for(i = 0; i < total; i++) {
    		if(dets[i].prob[k] == 0) continue;
    		box a = dets[i].bbox;
    	    for(j = i + 1; j < total; ++j) {
    			box b = dets[j].bbox;
    			if(box_iou(a,b) > nms_thresh) {
    				dets[j].prob[k] = 0;
    			}
    		}
    	}
	}
}
void YoloV3PostProc::transform_grid(float* prediction)const {
   std::vector<std::vector<int>> anchors_masks;
   const std::vector<std::vector<int>> ANCHORS_MASKS6 = {{3,4,5},{0, 1,2}};
   const std::vector<std::vector<int>> ANCHORS_MASKS8 = {{4,5,6,7},{0, 1,2,3}};
   const std::vector<std::vector<int>> ANCHORS_MASKS9 = {{6,7,8},{3,4,5},{0,1,2}};
   const std::vector<std::vector<int>> ANCHORS_MASKS12 = {{8,9,10,11},{4,5,6,7},{0,1,2,3}};
   if(anchors.size() == 6) {
   	  anchors_masks = ANCHORS_MASKS6 ;
   } else if(anchors.size() == 8) {
      anchors_masks = ANCHORS_MASKS8 ;
   }else if(anchors.size() == 9) {
   	  anchors_masks = ANCHORS_MASKS9 ;
   }else if(anchors.size() == 12) {
   	  anchors_masks = ANCHORS_MASKS12 ;
   } else {
   	  CHECK(0);
   }

  int len = 5 + num_classes;
  for(int i = 0; i < nstrides; i++) {
  	int stride = strides[i];
	  float input_dim_div_stride = (float)input_dim / stride;
    int to_offset = step[i][0] * len;
    int tmp_idx_1 = 0;
    for (int x_offset = 0; x_offset < stride; ++x_offset) {
      int tmp_idx_2 = 0;
      for(int y_offset = 0; y_offset < stride; ++y_offset) {
        int tmp_idx_3 = 0;
        for(int kk = 0; kk < nboxes; ++kk) {
          int tmp_idx = to_offset + tmp_idx_1 + tmp_idx_2 + tmp_idx_3;
          if(prediction[tmp_idx+4] >= conf_thresh) {
            prediction[tmp_idx+0] += y_offset;
            prediction[tmp_idx+0] *= input_dim_div_stride ;
            prediction[tmp_idx+1] += x_offset ;
            prediction[tmp_idx+1] *= input_dim_div_stride;
            prediction[tmp_idx+2] = std::exp(prediction[tmp_idx+2] )* anchors[anchors_masks[i][kk]][0] ;
            prediction[tmp_idx+3] = std::exp(prediction[tmp_idx+3] )* anchors[anchors_masks[i][kk]][1] ;
          }
          tmp_idx_3 += len;
        }
        tmp_idx_2 += nboxes*len;
      }
      tmp_idx_1 += stride*nboxes*len;
    }
  }
   
}
void YoloV3PostProc::ConvertSquare(Vbbox& bbox, float exth_scale, float extw_scale)const {
    int max_side = 0, w = 0, h = 0;
	for(uint32_t i = 0; i < bbox.size(); i++) {
		w = bbox[i].x2 - bbox[i].x1 + 1;
		h = bbox[i].y2 -bbox[i].y1 + 1;
    max_side = std::max(w,h);
		bbox[i].x1 -= 0.5*(max_side*(1+2.0*extw_scale) -w);
		bbox[i].y1 -= 0.5*(max_side*(1+2.0*exth_scale) -h);
		bbox[i].x2 = bbox[i].x1 + max_side * (1+2.0*extw_scale) -1;
		bbox[i].y2 = bbox[i].y1 + max_side * (1+2.0*exth_scale) -1;
	}
}
Vbbox YoloV3PostProc::pack_results(std::vector<detection>& detections, int img_w, int img_h)const {
   Vbbox boxes;
   int len = 5 + num_classes;
   
   int max_dim = std::max(img_w, img_h);
   float scaling_factor = (float)input_dim / max_dim;

   float top_occluded_prob = 0.f;
   float bottom_occluded_prob = 0.f;
   for(uint32_t i = 0; i< detections.size(); i++) {
    float x = detections[i].bbox.x;
		float y = detections[i].bbox.y;
    float w = detections[i].bbox.w;
    float h = detections[i].bbox.h;
		
		float x1 = x - w/2;
		float y1 = y - h/2;
		float x2 = x + w/2;
		float y2 = y + h/2;
		x1 -= (input_dim - scaling_factor * img_w) /2.0f;
		x2 -= (input_dim - scaling_factor * img_w) /2.0f;
		y1 -= (input_dim - scaling_factor * img_h) /2.0f;
		y2 -= (input_dim - scaling_factor * img_h) /2.0f;
		x1 /= scaling_factor;
		x2 /= scaling_factor;
		y1 /= scaling_factor;
		y2 /= scaling_factor;

		x1 = std::min(img_w, std::max((int)x1, 0));
		x2 = std::min(img_w, std::max((int)x2, 0));
		y1 = std::min(img_h, std::max((int)y1, 0));
		y2 = std::min(img_h, std::max((int)y2, 0));

		float score = 0.;
		for (int j = 0; j < num_classes; ++j) {
			if(detections[i].prob[j] > 0) {
				score = detections[i].prob[j];
				Bbox b(x1, y1, x2, y2, score);
				b.label = j;
				boxes.push_back(b);
			}
		}	
   }
   return boxes;     
}
std::vector<detection> YoloV3PostProc::pack_valid_detections(float* prediction)const {
   std::vector<int> valid_detections;
   int len = 5 + num_classes;

   for(int i = 0; i < step[nstrides - 1][1]; i++) {
   	    float score = prediction[i*len+4];
		if(score > conf_thresh) {
			valid_detections.push_back(i);
		}
   }
   
   float top_occluded_prob = 0.f;
   float bottom_occluded_prob = 0.f;
   std::vector<detection> detections;
   for(uint32_t i = 0; i < valid_detections.size(); i++) {
	   	detection det;
		int iprediction = valid_detections[i];

		float score = prediction[iprediction*len + 4];
		float x = prediction[iprediction*len + 0];
		float y = prediction[iprediction*len + 1];
		float w = prediction[iprediction*len + 2];
		float h = prediction[iprediction*len + 3];

		det.bbox.x = x;
		det.bbox.y = y;
		det.bbox.w = w;
		det.bbox.h = h;
		det.objectness = score;
        det.prob = prediction + iprediction*len + 5;

		for(int j = 0; j < num_classes; j++) {
			float prob = score * det.prob[j];
			det.prob[j] = (prob > conf_thresh) ? prob : 0;
		}

		detections.push_back(det);       
   }

   return detections;   
}

}
}

