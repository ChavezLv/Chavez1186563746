/*
 * nms_if.h
 *
 *  Created on: 2018-11-26
 *      Author: Administrator
 */

#ifndef _MTCNN_IF_H
#define _MTCNN_IF_H

typedef enum{
	NMS_UNION_MODE = 0,
	NMS_MIN_MODE = 1,

}Nms_Mode;

typedef enum{
	NMS_PNET = 0,
	NMS_RNET = 1,
	NMS_ONET = 2,

}Nms_Net_Type;

typedef struct{
    float scale;
    s16 height;
    s16 width;

    s16* cls;
    s16* reg;
}t_pnet_level_info;

typedef struct{
	s16 level_num;  //level max 16
	s16 thres;
	s16 stride_step;
	s16 min_size;
	s16 reg_fixQ;
	s16 padding;

	t_pnet_level_info  pnet_level_info[16];

	s16* face_num;
	s16* pFace_Roi;    //box
	s16* pFace_Score;
	s16* pDdrBuf;
}t_pnet_nms_info;

typedef struct{
	s16 in_face_num;  //level max 16
	s16 RO_thres;
	s16 reg_fixQ;
	s16 nms_mode;        //0  NMS_UNION_MODE; 1 NMS_MIN_MODE
	s16 nms_thres;
	s16 nms_fixQ;
	s16* in_face_roi;    //box
	s16* cls;
	s16* reg;
	s16* in_face_score;

	s16* out_face_num;
	s16* out_face_roi;   //box
	s16* out_face_score;
}t_rnet_onet_nms_info;

bool Face_Nmsmode_Overlap(s16 box0, s16 box1, s16 box2, s16 box3, s16 refbox0, s16 refbox1, s16 refbox2, s16 refbox3, s16 nms_thres,
		s16 nms_fixQ, Nms_Mode nms_mode);

void Pnet_Nms_Proc(t_pnet_nms_info* pnet_nms_info);
void Face_Net_Nms_Proc(t_rnet_onet_nms_info* rnet_onet_nms_info, Nms_Net_Type nms_net_type);


#endif /*  */
