/*
 * nms.c
 *
 *  Created on: 2018-11-26
 *      Author: Administrator
 */
#include <stdio.h>
#include <string.h>
#include "../arm_types.h"
#include "nms_if.h"


int cv_Round(float value) {
	/* it's ok if round does not comply with IEEE754 standard;
	 the tests should allow +/-1 difference when the tested functions use round */
	return (int)(value + (value >= 0 ? 0.5f : -0.5f));
}
	
/**************************************************************
 * ���ݸ�ʽchw    pnet
 * thres = 140 <-->  thres_softmax_f = 0.75
 */
s16 Pnet_Pool_Max_3x3_Nms(s16* cls, s16* reg, s32 height, s32 width, float scale, s16 thres, s16 stride_step,
		s16 min_size, s16 reg_fixQ, s16* faces_roi, s16* faces_score)
{
    s32 plane = width*height;
	s16* cls0 = cls;
	s16* cls1 = cls+plane;
	const s32 cellsize = min_size;
	const s32 stride = stride_step;
	float reg_scale = (float)(1<<reg_fixQ);
	s16 validcnt = 0;
	
	s32 h = 0;
	for(; h < height; h++) 
	{
		s32 w = 0;
		for(; w < width; w++, cls0++, cls1++) 
		{
			s32 score_E = cls1[0] - cls0[0];	// E��
			//s32 max_score = score_E;

			if(score_E > thres) 
			{
//				LOG(DEBUG) << "h =" << h << ", w = " << w << ", [E] = ["
//				<< cls0[0] << "," << cls1[0] << "], [F] = ["
//				<< cls0[1] << "," << cls1[1] << "], [G] = ["
//				<< cls0[width-1] << "," << cls1[width-1] << "], [H] = ["
//				<< cls0[width] << "," << cls1[width] << "], [I] = ["
//				<< cls0[width+1] << "," << cls1[width+1] << "]";

				if(h + 1 < height) {
					if((w > 0) && (cls1[width-1] - cls0[width-1]) > score_E) continue;	// G ��
					if(cls1[width] - cls0[width] > score_E) continue;	// H ��
					if((w + 1 < width) && (cls1[width+1] - cls0[width+1]) > score_E) continue;	// I ��
				}

				if((w + 1 < width) && (cls1[1] - cls0[1]) > score_E) continue;	// F ��
				if((w > 0) && (cls1[-1] - cls0[-1]) > score_E) continue;	// D ��

				if(h > 0) {
					if((w > 0) && (cls1[-width-1] - cls0[-width-1]) > score_E) continue;	// A ��
					if(cls1[-width] - cls0[-width] > score_E) continue;	// B ��
					if((w + 1 < width) && (cls1[-width+1] - cls0[-width+1]) > score_E) continue;	// C ��
				}


				// ���ڳ������޵ĵ㣬����ܱ�δ����ĵ�(G,H,I)
				if(h > 0) {
					if(w > 0)	cls1[-width-1] = cls0[-width-1] = 0; // A ��
					cls1[-width] = cls0[-width] = 0;	// B ��
					if(w + 1 < width)	cls1[-width+1] = cls0[-width+1] = 0;	// C ��
				}

				if(w > 0)	cls1[-1] = cls0[-1] = 0; // D ��
				if(w + 1 < width)	cls1[1] = cls0[1] = 0;	// F ��

				if(h + 1 < height) {
					if(w > 0)	cls1[width-1] = cls0[width-1] = 0;	// G ��
					cls1[width] = cls0[width] = 0;	// H ��
					if(w + 1 < width)	cls1[width+1] = cls0[width+1] = 0;	// I ��
				}

				//if(max_score == score_E) {
					s32 off = h * width + w;
					s16 bw = cv_Round((stride * w + cellsize) / scale) - cv_Round((stride * w) / scale) + 1;
					s16 bh = cv_Round((stride * h + cellsize) / scale) - cv_Round((stride * h) / scale) + 1;
					faces_roi[0] = cv_Round(cv_Round((stride * w) / scale) + reg[off]/reg_scale * bw);
					faces_roi[1] = cv_Round(cv_Round((stride * h) / scale) + reg[off + plane]/reg_scale * bh);
					faces_roi[2] = cv_Round(cv_Round((stride * w + cellsize) / scale) + reg[off + 2 * plane]/reg_scale * bw);
					faces_roi[3] = cv_Round(cv_Round((stride * h + cellsize) / scale) + reg[off + 3 * plane]/reg_scale * bh);
					*faces_score++ = score_E;
					faces_roi+=4;
					validcnt++;

//					LOG(INFO) << "h =" << h << ", w = " << w << ", lu_pos = [" << face.lu_pos[0] << "," << face.lu_pos[1] << "]";
				//}
			}
		}
	}

	return validcnt;
}

void Pnet_Nms_Proc(t_pnet_nms_info* pnet_nms_info)
{
	s32 level_num = pnet_nms_info->level_num;
	s16 thres = pnet_nms_info->thres;
	s16 stride_step = pnet_nms_info->stride_step;
	s16 min_size = pnet_nms_info->min_size;
	s16 reg_fixQ = pnet_nms_info->reg_fixQ;
	//s16* pDdrTemp = pnet_nms_info->pDdrBuf;
	s16 face_num = 0;

    s32 i=0;
    for(;i<level_num;i++)
    {
    	float scale = pnet_nms_info->pnet_level_info[i].scale;
    	s16 height = pnet_nms_info->pnet_level_info[i].height;
    	s16 width = pnet_nms_info->pnet_level_info[i].width;
    	s16* cls = pnet_nms_info->pnet_level_info[i].cls;
    	s16* reg = pnet_nms_info->pnet_level_info[i].reg;
    	s16* faces_roi = pnet_nms_info->pFace_Roi+face_num*4;
    	s16* faces_score = pnet_nms_info->pFace_Score+face_num;

    	s16 validcnt = Pnet_Pool_Max_3x3_Nms(cls, reg, height, width, scale, thres, stride_step,
    			min_size, reg_fixQ, faces_roi, faces_score);
    	face_num+=validcnt;
    	//printf("level(%d) validcnt:%d, total:%d,[%d,%d,%d,%f,%d,%d,%d].\n",i,validcnt,face_num, height, width, scale, thres, stride_step, min_size, reg_fixQ);
    }

    *(pnet_nms_info->face_num) = face_num;
}

s16 Face_Rnet_Adjust(s16* in_face_roi, s16* cls, s16* reg, s16* in_score, s16 in_face_num, s16 thres, s16 reg_fixQ,
		s16* out_box0, s16* out_box1, s16* out_box2, s16* out_box3, s16* out_score)
{	
	s16 round = (1<<(reg_fixQ-1));
	s16 validcnt = 0;
	s32 i = 0;
	
	for(;i<in_face_num;i++)
	{
		//cls
		s16 vscore = cls[1] - cls[0];
		if(vscore > thres)
		{
			s16 bw = in_face_roi[2] - in_face_roi[0] + 1;
			s16 bh = in_face_roi[3] - in_face_roi[1] + 1;
			
			out_box0[validcnt] = in_face_roi[0] + ((bw*reg[0]+round)>>reg_fixQ);
			out_box1[validcnt] = in_face_roi[1] + ((bh*reg[1]+round)>>reg_fixQ);
			out_box2[validcnt] = in_face_roi[2] + ((bw*reg[2]+round)>>reg_fixQ);
			out_box3[validcnt] = in_face_roi[3] + ((bh*reg[3]+round)>>reg_fixQ);
			out_score[validcnt] = in_score[i];

			validcnt++;
		}
		in_face_roi+=4;
		reg+=4;
		cls+=2;
	}

    return validcnt;
}

s16 Face_Onet_Adjust(s16* in_face_roi, s16* cls, s16* reg, s16 in_face_num, s16 thres, s16 reg_fixQ,
		s16* out_box0, s16* out_box1, s16* out_box2, s16* out_box3, s16* out_score)
{
	s16 round = (1<<(reg_fixQ-1));
	s16 validcnt = 0;
	s32 i = 0;
	
	for(;i<in_face_num;i++)
	{
		//cls
		s16 vscore = cls[1] - cls[0];
		if(vscore > thres)
		{
			s16 bw = in_face_roi[2] - in_face_roi[0] + 1;
			s16 bh = in_face_roi[3] - in_face_roi[1] + 1;
			
			out_box0[validcnt] = in_face_roi[0] + ((bw*reg[0]+round)>>reg_fixQ);
			out_box1[validcnt] = in_face_roi[1] + ((bh*reg[1]+round)>>reg_fixQ);
			out_box2[validcnt] = in_face_roi[2] + ((bw*reg[2]+round)>>reg_fixQ);
			out_box3[validcnt] = in_face_roi[3] + ((bh*reg[3]+round)>>reg_fixQ);
			out_score[validcnt] = vscore;

			validcnt++;
		}
		in_face_roi+=4;
		reg+=4;
		cls+=2;
	}

    return validcnt;
}

void Nms_Sort(s16* out_box0, s16* out_box1, s16* out_box2, s16* out_box3, s16* out_score, u16 wRoiNum)
{
    u16 wI, wJ;
    s16 maxIdx;
    s16 boxtmp0,boxtmp1,boxtmp2,boxtmp3,scoreTmp;

    wI = 0;
    for(; wI < wRoiNum - 1; wI++)
    {
        maxIdx = wI;
        for(wJ = wI + 1; wJ < wRoiNum; wJ++)
        {
            if(out_score[maxIdx] < out_score[wJ])//If the front is smaller than the back, switch
            {
                maxIdx = wJ;
            }
        }

		if(maxIdx == wI) continue;
		
		boxtmp0 = out_box0[wI];
		boxtmp1 = out_box1[wI];
		boxtmp2 = out_box2[wI];
		boxtmp3 = out_box3[wI];
		scoreTmp = out_score[wI];

		out_box0[wI] = out_box0[maxIdx];
		out_box1[wI] = out_box1[maxIdx];
		out_box2[wI] = out_box2[maxIdx];
		out_box3[wI] = out_box3[maxIdx];
		out_score[wI] = out_score[maxIdx];

		out_box0[maxIdx] = boxtmp0;
		out_box1[maxIdx] = boxtmp1;
		out_box2[maxIdx] = boxtmp2;
		out_box3[maxIdx] = boxtmp3;
		out_score[maxIdx] = scoreTmp;
    }
}

bool Face_Nmsmode_Overlap(s16 box0, s16 box1, s16 box2, s16 box3, s16 refbox0, s16 refbox1, s16 refbox2, s16 refbox3, s16 nms_thres,
		s16 nms_fixQ, Nms_Mode nms_mode)
{
    bool bModeSet;
    s16 swThresFix = nms_thres;
    s16 sdwLuX, sdwLuY, sdwRbX, sdwRbY;
    s32 sdwArea, sdwAreaFix, sdwAreaFixCmp;

    sdwLuX = MAX2(box0, refbox0);
    sdwLuY = MAX2(box1, refbox1);
    sdwRbX = MIN2(box2, refbox2);
    sdwRbY = MIN2(box3, refbox3);

    s16 w = box2 - box0 + 1;
    s16 h = box3 - box1 + 1;
    s16 refw = refbox2 - refbox0 + 1;
    s16 refh = refbox3 - refbox1 + 1;
    s16 overlapw = sdwRbX - sdwLuX + 1;
    s16 overlaph = sdwRbY - sdwLuY + 1;

    sdwArea = ((overlapw > 0) && (overlaph > 0)) ? (overlapw * overlaph) : 0;
    sdwAreaFix = sdwArea << nms_fixQ;

    if(nms_mode == NMS_UNION_MODE)
    {
        sdwAreaFixCmp = (w * h + refw * refh - sdwArea) * swThresFix;
    }
    else
    {
        sdwAreaFixCmp = MIN2(w * h, refw * refh) * swThresFix;
    }

    bModeSet = sdwAreaFix > sdwAreaFixCmp ? true : false;

    return bModeSet;
}

s16 Face_Net_Nms(s16* box0, s16* box1, s16* box2, s16* box3, s16* score, s16* valid_set, s16 face_num, s16 nms_thres, s16 nms_fixQ,
		Nms_Mode nms_mode, s16* out_face, s16* out_score)
{
	s16 wFaceIndex = 0;

    memset(valid_set, 0, face_num*sizeof(u16));
    s32 i=0;
    for (; i < face_num; i++)
    {
        if(valid_set[i] != 1)
        {
        	out_face[4*wFaceIndex] = box0[i];
        	out_face[4*wFaceIndex+1] = box1[i];
        	out_face[4*wFaceIndex+2] = box2[i];
        	out_face[4*wFaceIndex+3] = box3[i];
            out_score[wFaceIndex++] = score[i];
            s32 j = i+1;
            for (; j < face_num; j++)
            {
                if(valid_set[j] != 1)
                {
                	bool bvalid = Face_Nmsmode_Overlap(box0[j], box1[j], box2[j], box3[j], box0[i], box1[i], box2[i], box3[i], nms_thres,
                			nms_fixQ, nms_mode);
                    if(bvalid)
                    {
                    	valid_set[j] = 1;
                    }
                }
            }
        }
    }

    return wFaceIndex;
}

s16 g_face_net_buf[60*1024]; //50k
void Face_Net_Nms_Proc(t_rnet_onet_nms_info* rnet_onet_nms_info, Nms_Net_Type nms_net_type)
{
	s16* out_box0 = (s16*)g_face_net_buf;  //10k s16
	s16* out_box1 = out_box0+10240;  //10k s16
	s16* out_box2 = out_box1+10240;   //10k s16
	s16* out_box3 = out_box2+10240;   //10k s16
	s16* out_score = out_box3+10240;   //10k s16
	s16 out_face_num = 0;

	//1.��Ч�ԣ�����
	s16 in_face_num = rnet_onet_nms_info->in_face_num;
	s16 thres = rnet_onet_nms_info->RO_thres;
	s16 reg_fixQ = rnet_onet_nms_info->reg_fixQ;
	s16* pInFaceEx = rnet_onet_nms_info->in_face_roi;
	s16* pInRegEx = rnet_onet_nms_info->reg;
	s16* pInClsEx = rnet_onet_nms_info->cls;

	s16 validcnt = 0;
	if(NMS_RNET == nms_net_type)
	{
		s16* pInScoreEx = rnet_onet_nms_info->in_face_score;
		validcnt = Face_Rnet_Adjust(pInFaceEx, pInClsEx, pInRegEx, pInScoreEx, in_face_num, thres, reg_fixQ, out_box0, out_box1, out_box2, out_box3, out_score);
	}
	else
	{
		validcnt = Face_Onet_Adjust(pInFaceEx, pInClsEx, pInRegEx, in_face_num, thres, reg_fixQ, out_box0, out_box1, out_box2, out_box3, out_score);
	}

	out_face_num=validcnt;

	//2.nms
    Nms_Sort(out_box0, out_box1, out_box2, out_box3, out_score, out_face_num);

    s16* valid_set = out_score+10240;  //8k s16
    s16 nms_thres = rnet_onet_nms_info->nms_thres;
    s16 nms_fixQ = rnet_onet_nms_info->nms_fixQ;
	s16* out_faceEx = rnet_onet_nms_info->out_face_roi;
    s16* out_scoreEx = rnet_onet_nms_info->out_face_score;
    Nms_Mode nms_mode = (Nms_Mode)rnet_onet_nms_info->nms_mode;

    out_face_num = Face_Net_Nms(out_box0, out_box1, out_box2, out_box3, out_score, valid_set, out_face_num, nms_thres, nms_fixQ,
    		nms_mode, out_faceEx, out_scoreEx);

    *(rnet_onet_nms_info->out_face_num) = out_face_num;
}


