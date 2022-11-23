#ifndef _RESIZE_H
#define _RESIZE_H


#define  YUV_RGB_ITUR_BT_601_CY         1220542
#define  YUV_RGB_ITUR_BT_601_CUB        2116026
#define  YUV_RGB_ITUR_BT_601_CUG        -409993
#define  YUV_RGB_ITUR_BT_601_CVG        -852492
#define  YUV_RGB_ITUR_BT_601_CVR        1673527 
#define  YUV_RGB_ITUR_BT_601_SHIFT      20

#define  RESIZE_REMAIN_BUF  2 //resize数据保护间隙：1--2 (h rows/w cols);2--4;4--8.

 typedef enum{
	 E_ADJUST_BOX_ONLY = 0,
	 E_NEED_PULL_HEIGHT0 = 1,  //先调高后方形化
	 E_NEED_PULL_HEIGHT1 = 2,  //先方形化再调高
	 E_NEED_PULL_HW0 = 3   //先方形化再调高和宽
 }Pull_Type;

typedef enum{
    IMG_YUV=0,
    IMG_GREY,
//	IMG_YUV422,
}eImgType;

#ifdef _ARM_NEON
void memcpy_neon(unsigned char *dst, unsigned char *src, int len);
#endif

void Cmm_Adjust_Box_New(short* pBox, int Img_h, int Img_w, Pull_Type Pull);

void face_resize_scale_adjust(int _src_h, int _src_w, int _search_h, int _search_w, int _dst_h, int _dst_w,
		int _box_x, int _box_y, short* _sx, short* _cbufxi, short* _sy, short* _cbufyi);

void face_resize_scale_yuv2rgb_chw(unsigned char* _src_yuv, int is_yuv_NV21, int _src_h, int _src_w, int box_x, int box_y,
		int _search_h, int _search_w, int _dst_h, int _dst_w, short* _dst);

void face_resize_scale_yuv2rgb_chw_no_normalize(unsigned char* _src_yuv, int is_yuv_NV21, int _src_h, int _src_w, int box_x, int box_y,
		int _search_h, int _search_w, int _dst_h, int _dst_w, unsigned char* _dst);


void face_resize_scale_yuv2Y(unsigned char* _src_y, int _src_h, int _src_w, int box_x, int box_y, 
		int _search_h, int _search_w, int _dst_h, int _dst_w, short* _dst_y);

void face_resize_scale_yuv2Y_no_normalize(unsigned char* _src_y, int _src_h, int _src_w, int box_x, int box_y,
		int _search_h, int _search_w, int _dst_h, int _dst_w, unsigned char* _dst_y);


#endif
