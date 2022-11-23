/*
 * slk_tracker.c
 *
 *  Created on: 2018-10-30
 *      Author: Administrator
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../arm_types.h"
#include "slk_tracker_cmm.h"
#include "slk_tracker_in.h"
#include "lk_pyramid_PC.h"
#include "slk_tracker_if.h"
//#include "lib_print.h"
/*  
void GenGridPoints(Rect *zone, Point2f *pzonePt)
{
	int nIncremX = (zone->w - TRACKER_MARGIN_XY * 2) / (TRACKER_PT_CNT_XY - 1);//m_nYCount=m_nXCount=10;   m_nMarginX/Y=  5
	int nIncremY = (zone->h - TRACKER_MARGIN_XY * 2) / (TRACKER_PT_CNT_XY - 1);
    int nIncremXTmp = 0, nIncremYTmp = 0;
    Point2f tNewRect;
    tNewRect.x = (float)(zone->x + TRACKER_MARGIN_XY);
	tNewRect.y = (float)(zone->y + TRACKER_MARGIN_XY);

	for (int r = 0; r < TRACKER_PT_CNT_XY; r++) //m_nYCount=m_nXCount=10
	{
		for (int c = 0; c <TRACKER_PT_CNT_XY; c++)
		{
			pzonePt->x = tNewRect.x + nIncremXTmp;
			pzonePt->y = tNewRect.y + nIncremYTmp;
			nIncremXTmp += nIncremX;
			pzonePt++;
		}
		nIncremXTmp = 0;
		nIncremYTmp += nIncremY;
	}
}
*/
void GenGridPoints(Rect *zone, Point2f *pzonePt, int countx, int county)
{
    //printf("Gengreid addr = %d; %d; %d\n",pzonePt, countx, county);
	int nIncremX = (zone->w - TRACKER_MARGIN_XY * 2) / (countx - 1);//m_nYCount=m_nXCount=10;   m_nMarginX/Y=  5
	int nIncremY = (zone->h - TRACKER_MARGIN_XY * 2) / (county - 1);
    int nIncremXTmp = 0;
    int nIncremYTmp = 0;
    Point2f tNewRect;
    tNewRect.x = (float)(zone->x + TRACKER_MARGIN_XY);
	tNewRect.y = (float)(zone->y + TRACKER_MARGIN_XY);
    int idx = 0;
	 
    for (int r = 0; r < countx; r++) //m_nYCount=m_nXCount=10
	{
		for (int c = 0; c <county; c++)
		{
            pzonePt[idx].x = tNewRect.x + nIncremXTmp;
			pzonePt[idx].y = tNewRect.y + nIncremYTmp;
			nIncremXTmp += nIncremX;
           // printf("pointf addr = %d\n", &pzonePt[idx]);
            idx++;
		}
		nIncremXTmp = 0;
		nIncremYTmp += nIncremY;
	}
}

bool ValidateResult(s16 *nX, s16 *nY, s16 *nWidth, s16 *nHeight, s16 img_h, s16 img_w)
{
	if((nWidth <= 0) || (nHeight <= 0))
	{
		return false;
	}
	#if 0
	else if ((*nX  <  0) || (*nY  <  0) || (*nX + *nWidth) > img_w || (*nY + *nHeight) > img_h)
	{
		Rect face(*nX, *nY, *nWidth, *nHeight);
		validate_rect(&face, img_w, img_h);
/*
		if(((f32)(face.width * face.height) / (nWidth * nHeight)) < (1 - 0.3))
		{
			printf("ValidateResult Fail![%d %d][%d %d]\n", nX, nY, nX + nWidth, nY + nHeight);
			return false;
		}*/
		if((face.width <= 0) || (face.height <= 0))
		{
		    return false;
		}
		else
		{
			*nX = face.x;
			*nY = face.y;
			*nWidth = face.width;
			*nHeight = face.height;
			return true;
		}
	}
	#endif
	else
	{
		return true;
	}
}

short pick_valid_points(Point2f *pCurPt, Point2f *pPrePt, u8 *pStatus, Point2f *pValid_pre, Point2f *pValid_cur)
{
	Point2f  *pCur = pCurPt;
	Point2f  *pPre = pPrePt;
	s16 i = 0;
	s16 validCnt = 0;
	s16 line_cnt = TRACKER_PT_CNT_XY;
	s16 line_over = TRACKER_PT_CNT_XY - 1;

	for(i = 0; i < line_cnt*line_cnt; i++)
	{
		if(0 == pStatus[i])
		{
			continue;
		}

		s16 k = 0,j = i;
		while(j >= line_cnt)
		{
		   k++;
		   j-=line_cnt;
		}

		s16 cond_cnt = 0;
		#if 1
		bool bleft,bright,bup,bbot;
		//left
		if(j==0)
		{
			bleft = (pCur[i].x<pCur[i+2].x);
		}
		else
		{
			bleft = (pCur[i].x>pCur[i-1].x);
		}
		cond_cnt += bleft;

	    //right
		if(j==line_over)
		{
			bright = (pCur[i].x>pCur[i-2].x);
		}
		else
		{
			bright = (pCur[i].x<pCur[i+1].x);
		}
		cond_cnt += bleft;

		//top
		if(k==0)
		{
			bup = (pCur[i].y<pCur[i+2*line_cnt].y);
		}
		else
		{
			bup = (pCur[i].y>pCur[i-line_cnt].y);
		}
		cond_cnt += bleft;

		//bottom
		if(k==line_over)
		{
			bbot = (pCur[i].y>pCur[i-2*line_cnt].y);
		}
		else
		{
			bbot = (pCur[i].y<pCur[i+line_cnt].y);
		}
		cond_cnt += bleft;
#else
		bool bleft = (((pCur[i].x>pCur[i-1].x) && (j!=0)) || ((pCur[i].x<pCur[i+2].x) && (j==0)));
		cond_cnt += bleft;
		bool bright = (((pCur[i].x<pCur[i+1].x) && (j!=line_over)) || ((pCur[i].x>pCur[i-2].x) && (j==line_over)));
		cond_cnt += bright;
		bool bup = (((pCur[i].y>pCur[i-line_cnt].y) && (k!=0)) || ((pCur[i].y<pCur[i+2*line_cnt].y) && (k==0)));
		cond_cnt += bup;
		bool bbot = (((pCur[i].y<pCur[i+line_cnt].y) && (k!=line_over)) || ((pCur[i].y>pCur[i-2*line_cnt].y) && (k==line_over)));
		cond_cnt += bbot;
#endif

		if(3 <= cond_cnt)
		{
			pValid_pre[validCnt].x = pPre[i].x;
			pValid_pre[validCnt].y = pPre[i].y;
			pValid_cur[validCnt].x = pCur[i].x;
			pValid_cur[validCnt].y = pCur[i].y;
			validCnt++;
		}
	}

	return validCnt;
}

void face_distance_calc(s16 nPoints, Point2f *pCurPt, Point2f *pPrePt, float *pCurDis, float *pPreDis)
{
	s32 i, j, k = 0;
	Point2f point2fTmp;

	for (i = 0; i < nPoints; i++)
	{
		for (j = i + 1; j < nPoints; j++)
		{
			point2fTmp.x = pPrePt[j].x - pPrePt[i].x;
			point2fTmp.y = pPrePt[j].y - pPrePt[i].y;
			pPreDis[k] = (float)((double)point2fTmp.x * point2fTmp.x + (double)point2fTmp.y * point2fTmp.y);
			point2fTmp.x = pCurPt[j].x - pCurPt[i].x;
			point2fTmp.y = pCurPt[j].y - pCurPt[i].y;
			pCurDis[k] = (float)((double)point2fTmp.x * point2fTmp.x + (double)point2fTmp.y * point2fTmp.y);

			k++;
		}
	}
}

s32 partition(f32 *dst, s32 left, s32 right, s32 pivot)
{
	f32 value = dst[pivot];
	s32 index = left;
	s32 i;
	s32 not_all_same = 0;
	/* move the pivot to the right */
	SORT_SWAP(dst[pivot], dst[right]);

	for (i = left; i < right; i++)
	{
		s32 cmp = SORT_CMP(dst[i], value);
		/* check if everything is all the same */
		not_all_same |= cmp;

		if (cmp < 0)
		{
			SORT_SWAP(dst[i], dst[index]);
			index++;
		}
	}

	SORT_SWAP(dst[right], dst[index]);

	/* avoid degenerate case */
	if (not_all_same == 0)
	{
		return SORT_SIZE_MAX;
	}

	return index;
}

f32 find_kth(f32 *dst, s32 size, s32 k)
{
    s32 left0 = 0;
    s32 right0 = size - 1;
    if(size<=1) return dst[0];
	if(size<=k) return dst[right0];

	while(right0>left0)
	{
		s32 pivot = left0 + ((right0 - left0) >> 1);
		/* this seems to perform worse by a small amount... ? */
		/* pivot = MEDIAN(dst, left, pivot, right); */
		s32 new_pivot = partition(dst, left0, right0, pivot);

		/* check for partition all equal */
		if((new_pivot == SORT_SIZE_MAX) || (k == new_pivot))
		{
			break;
		}

		if (k < new_pivot)
		{
			right0 = new_pivot-1;
		}
		else
		{
			left0 = new_pivot+1;
		}
	}

	return dst[k];
}

bool face_position_predict(s16 nPoints, Rect *pre_face, Rect *cur_face, Point2f *pointsPrev,
                    Point2f *pointsCur, s16 img_h, s16 img_w)
{
	int nDistPairs = nPoints*(nPoints - 1) / 2;
	float *vecDistPrev = NULL, *vecDistCur = NULL, *vecScale = NULL;
 // LIB_ALGO_PRINT("1 : %d\n", (3 * nDistPairs * sizeof(float)));
	vecDistPrev = (float *)malloc(3 * nDistPairs * sizeof(float));
 // LIB_ALGO_PRINT("1 - %p\n", vecDistPrev);
	if(vecDistPrev == NULL)
	{
	      return false;
	}
	vecDistCur = vecDistPrev + nDistPairs;
	vecScale = vecDistCur + nDistPairs;
	
	face_distance_calc(nPoints, pointsCur, pointsPrev, vecDistCur, vecDistPrev);
	
	int i = 0;
	int k = 0;
	f32 div_TH_up = 1.3;
	f32 div_TH_down = 0.7;
	for (; i<nDistPairs; i++)
	{
		if (vecDistPrev[i] > 0.0f)
		{
			vecScale[k] = vecDistCur[i] / vecDistPrev[i];
			if((vecScale[k]>=div_TH_down)&&(vecScale[k]<=div_TH_up))
			k++;
		}
	}

	f32 fScale = find_kth(vecScale, k, (k >> 1));
    fScale = (float)sqrt(fScale);//@lhj 20170208 not sqrt in function 'PairwiseDistance'
    //printf("\n MatrixPredict_Opt find_kth end1 fScale:%f\n", fScale);
 
	if (fScale < 0.5)
	{
		free(vecDistPrev);
		printf("maxtric predict scale:%f\n", fScale);
		return false;
	}
	if (fScale > 3)
	{
		free(vecDistPrev);
		printf("maxtric predict scale:%f\n", fScale);
		return false;
	}

	if (fScale > 1.02)
	{
		fScale = (f32)1.02;
	}

	f32 fScaleX = (f32)0.5 * (fScale - 1) * pre_face->w;
	f32 fScaleY = (f32)0.5 * (fScale - 1) * pre_face->h;
	
    float *vecXOffset = vecDistPrev, *vecYOffset = vecDistPrev + nPoints; //when n > 3, then n*(n-1)/2 > n
	for (i = 0; i < nPoints; i++)
	{
		vecXOffset[i] = pointsCur[i].x - pointsPrev[i].x;
		vecYOffset[i] = pointsCur[i].y - pointsPrev[i].y;
	}
	
	//
	f32 fTranslationX = find_kth(vecXOffset, nPoints, nPoints >> 1);
	f32 fTranslationY = find_kth(vecYOffset, nPoints, nPoints >> 1);

	free(vecDistPrev);

	s16 nRectX = (s16)(pre_face->x + fTranslationX - fScaleX + 0.5);
	s16 nRectY = (s16)(pre_face->y + fTranslationY - fScaleY + 0.5);
	s16 nRectWidht = (s16)(pre_face->w  * fScale + 0.5);
	s16 nRectHeight = (s16)(pre_face->h * fScale + 0.5);

	if(ValidateResult(&nRectX, &nRectY, &nRectWidht, &nRectHeight,img_h, img_w))
	{
		cur_face->x = nRectX;
		cur_face->y = nRectY;
		cur_face->w = nRectWidht;
		cur_face->h = nRectHeight;
		return true;
	}
	else
	{
		printf("maxtric predict ValidResult false\n");
		return false;
	}
}



void GetValidRect(int inx,int iny, int inw, int inh,  Rect *r,
                  int imgw, int imgh)
{
    int x = inx<0?0:inx;
    int y = iny<0?0:iny;
    int rx = inx + inw - 1;
    int ry = iny + inh - 1;
    if(rx >= imgw) rx = imgw - 1;
    if(ry >= imgh) ry = imgh - 1;
    r->x=x;
    r->y=y;
    r->w = rx - x + 1;
    r->h = ry - y + 1;
    if(r->w < 0 ) r->w = 0;
    if(r->h < 0) r->h = 0;

    return ;
}



bool slk_tracker(s16 *box_in, s16 *box_out, u8 *pre_gray, u8 *cur_gray, s16 height, s16 width, s32 min_obj_size)
{
    s16 tx,ty,tw,th;
	Rect pre_face;
	s16 box_tmp[4];

	if (min_obj_size <= 0)
		min_obj_size = 20; // default

    /////////////////////
    //
    
    //////////////////////
	box_tmp[0] = MIN2(width - 1, MAX2(0, box_in[0]));
	box_tmp[1] = MIN2(height - 1, MAX2(0, box_in[1]));
	box_tmp[2] = MIN2(width - 1, MAX2(0, box_in[2]));
	box_tmp[3] = MIN2(height - 1, MAX2(0, box_in[3]));
	tx = box_tmp[0];
	ty = box_tmp[1];
	tw = box_tmp[2] - box_tmp[0] + 1;
	th = box_tmp[3] - box_tmp[1] + 1;
	////////////////////////////
	pre_face.x = box_tmp[0];
	pre_face.y = box_tmp[1];
	pre_face.w = box_tmp[2] - box_tmp[0] + 1;
	pre_face.h = box_tmp[3] - box_tmp[1] + 1;
    
    //GetValidRect(tx,ty,tw,th,&pre_face.x,&pre_face.y,  &pre_face.w,&pre_face.h, width,height);
    GetValidRect(tx,ty,tw,th,&pre_face, width,height);
    // printf("------juwenqi, %d,%d,%d,%d\n", pre_face.x,pre_face.y,pre_face.w,pre_face.h);
     //pre_face.x=box_in[0];

     if((pre_face.x < 0) || (pre_face.x >= width) || (pre_face.y < 0) || (pre_face.y >= height)\
			|| (pre_face.w <= min_obj_size ) || (pre_face.h <= min_obj_size))
	{
		printf("slk_tracker face_in check fail! do not track: [%d %d %d %d]\n",box_tmp[0], box_tmp[1], box_tmp[2], box_tmp[3]);
      	//box_out[0] = pre_face.x;
	    //box_out[1] = pre_face.y;
	    //box_out[2] = pre_face.x + pre_face.w - 1;
 	    //box_out[3] = pre_face.y + pre_face.h - 1;
        //return  true;
        return false;
	}

	if((pre_face.w > 600) || (pre_face.h > 600))
	{
		printf("LK Tracker not support width or height greater than 600 pixels this version!\n");
		//box_out[0] = pre_face.x;
	    //box_out[1] = pre_face.y;
	    //box_out[2] = pre_face.x + pre_face.w - 1;
 	    //box_out[3] = pre_face.y + pre_face.h - 1;
        //return true;
        return false;
	}
  const int countxy = TRACKER_PT_CNT_XY;
  int slk_tracker_pt_number = countxy*countxy;
  //LIB_ALGO_PRINT("1 : %d \n", slk_tracker_pt_number * sizeof(Point2f));
	Point2f* pCurPt = (Point2f *)malloc(slk_tracker_pt_number*sizeof(Point2f));
	if(pCurPt == NULL){return false;}
  //LIB_ALGO_PRINT("2 : %d \n", slk_tracker_pt_number * sizeof(Point2f));
	Point2f* pPrePt = (Point2f *)malloc(slk_tracker_pt_number*sizeof(Point2f));
	if(pPrePt == NULL){free(pCurPt);return false;}
  //LIB_ALGO_PRINT("3 : slk_tracker_pt_number\n");
	u8* pStatus = (u8 *)malloc(slk_tracker_pt_number);
  //LIB_ALGO_PRINT("4\n");
	if(pStatus == NULL){free(pCurPt);free(pStatus);return false;}    

	GenGridPoints(&pre_face, pPrePt, countxy, countxy);
	/* optical flow tracking points*/
	bool Isok_PyrLKPC = calcOpticalFlowPyrLKPC(&pre_face, pre_gray, cur_gray, pCurPt, pPrePt, pStatus, height, width);    
	if(false == Isok_PyrLKPC)
	{
	    free(pCurPt);
	    free(pPrePt);
	    free(pStatus);
		printf("calcOpticalFlowPyrLKPC is fail!!!\n");
		return false;
	}

	/*pick valid points*/
  //LIB_ALGO_PRINT("5 : %d \n", slk_tracker_pt_number * sizeof(Point2f));
	Point2f* pValid_pre = (Point2f *)malloc(slk_tracker_pt_number*sizeof(Point2f));
	if(pValid_pre == NULL){free(pStatus);free(pCurPt);free(pStatus);return false;}
 // LIB_ALGO_PRINT("6 : %d \n", slk_tracker_pt_number * sizeof(Point2f));
	Point2f* pValid_cur = (Point2f *)malloc(slk_tracker_pt_number*sizeof(Point2f));
  //LIB_ALGO_PRINT("7\n");
	if(pValid_cur == NULL){free(pStatus);free(pCurPt);free(pStatus);free(pValid_pre);return false;}
	
	s16 nPoints = pick_valid_points(pCurPt, pPrePt, pStatus, pValid_pre, pValid_cur);
	
	free(pCurPt);
	free(pPrePt);
	free(pStatus);
	//printf("valid track points num:%d\n", nPoints);
	if(nPoints <= 3)
	{
		free(pValid_pre);
	    free(pValid_cur);
		printf("valid track points num:%d", nPoints);
		return false;
	}

	/*matrix predict*/
	Rect cur_face;
	bool bSuccess = face_position_predict(nPoints, &pre_face, &cur_face, pValid_pre, pValid_cur, height, width);
	box_out[0] = cur_face.x;
	box_out[1] = cur_face.y;
	box_out[2] = cur_face.x + cur_face.w - 1;
	box_out[3] = cur_face.y + cur_face.h - 1;
	free(pValid_pre);
	free(pValid_cur);

	return bSuccess;	
}


