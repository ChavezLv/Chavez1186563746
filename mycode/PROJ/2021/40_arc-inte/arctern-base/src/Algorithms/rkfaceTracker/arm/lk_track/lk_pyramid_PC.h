/*
 * lk_pyramid_PC.h
 *
 *  Created on: 2018-10-30
 *      Author: Administrator
 */

#ifndef _LK_PYRAMID_PC_H
#define _LK_PYRAMID_PC_H
#include "../arm_types.h"

#define TRACKER_MAX_PYR_LEVEL  6
#define TRACKER_WIN_SIZE       4

#define IFBORDER_CONSTANT   0
#define IFBORDER_REPLICATE  1
#define IFBORDER_REFLECT    2
#define IFBORDER_WRAP       3
#define IFBORDER_REFLECT_101    4
#define IFBORDER_TRANSPARENT    5

#define IFCV_CN_MAX     512
#define IFCV_DESCALE(x,n)     (((x) + (1 << ((n)-1))) >> (n))

//#define TRACKER_PYR_DRAM_IN_SIZE       31744//(31 * 1024)u8
//#define TRACKER_PYR_DRAM_OUT_SIZE     18432//(18 * 1024)u8
//#define TRACKER_SHARR_DRAM_OUT_SIZE    63488//(62 * 1024)s16

//#define TRACKER_PYR_PRE_SIZE          5120//(5 * 1024)u8
//#define TRACKER_PYR_CUR_SIZE          153600//(150 * 1024)u8
//#define TRACKER_PYR_SHARR_SIZE        20480//(20 * 1024)s16

#define TRACKER_MARGIN_XY      5
#define TRACKER_PT_CNT_XY      6

#define TRACKER_EIGEN_TH_16x  0.0016
#define TRACKER_EIGEN_TH_8x  0.0008
//#define W_BITS                 14
//#define W_BITS1                9
#define W_BITS_SHIFT14         16384
#define W_BITS_SHIFT13         8192
#define W_BITS_SHIFT8          256

#define TRACKER_LK_ITERA_CNT   5
#define TRACKER_LK_EPLISION    (0.03 * 0.03)
#define TRACKER_LK_EARLY_STOP  1
#define TRACKER_LK_DELTA_TH_EN 1
#define TRACKER_LK_DELTA_TH    5.0

typedef	enum
{
	TermCriteria_COUNT = 1, //!< the maximum number of iterations or elements to compute
	TermCriteria_MAX_ITER = TermCriteria_COUNT, //!< ditto
	TermCriteria_EPS = 2 //!< the desired accuracy or change in parameters at which the iterative algorithm stops
}E_TermCriteria_TYPE;

typedef struct
{
	int type; //!< the type of termination criteria: COUNT, EPS or COUNT + EPS
	int maxCount; // the maximum number of iterations/elements
	double epsilon; // the desired accuracy
}TermCriteria;

s16 cal_Level_num(s16 w, s16 h);
void validate_rect(Rect *prect, s32 width, s32 height);
bool cfg_pyr_sharr(s16 levelnum, Rect *PrevRect, Rect *CurRect, u8** pPyrddr_addr, t_pyramid_info* pPrevPyr, t_pyramid_info* pSharrPyr, t_pyramid_info* pCurPyr);
void Lk_Pad_Zero(u8 *pBuffer, s16 Skip, s16 Rep);
void Lk_Pad_Zero_s16(s16 *pBuffer, s16 Skip, s16 Rep);
void buildOpticalFlowPyramidPC(bool prevInd, Rect *zone, s16 lvlNum, u8 *img_gray, s16 img_h, s16 img_w, t_pyramid_info* pPyr, t_pyramid_info* pSharr);

bool calcOpticalFlowPyrLKPC(Rect *pre_face, u8 *pre_gray, u8 *cur_gray, Point2f* nextPts, Point2f* prevPts, u8* status, s16 height, s16 width);


#endif /*  */
