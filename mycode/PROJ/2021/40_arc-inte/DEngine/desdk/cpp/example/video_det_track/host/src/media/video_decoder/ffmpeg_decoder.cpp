/*
 * @file    FFmpegDecoder.cpp
 * @date    2015-07-08
 * @brief   class and function impletement of FFmpegDecoder
 * @author  zhong.bin (zhong.bin@intellif.com)
 *
 * Copyright (c) 2015, Shenzhen Intellifusion Technologies Co., Ltd.
 */

#include <stdio.h>

#include "ffmpeg_decoder.hpp"

#include "opencv2/opencv.hpp"
// #include "boost/date_time/posix_time/posix_time.hpp"
// using namespace boost::posix_time;

static long crv_tab[256];
static long cbu_tab[256];
static long cgu_tab[256];
static long cgv_tab[256];
static long tab_76309[256];
static unsigned char clp[1024];   //for clip in CCIR601   

void init_yuv420p_table()
{
    long int crv, cbu, cgu, cgv;
    int i, ind;
    static int init = 0;

    if (init == 1) return;

    crv = 104597; cbu = 132201;  /* fra matrise i global.h */
    cgu = 25675;  cgv = 53279;

    for (i = 0; i < 256; i++)
    {
        crv_tab[i] = (i - 128) * crv;
        cbu_tab[i] = (i - 128) * cbu;
        cgu_tab[i] = (i - 128) * cgu;
        cgv_tab[i] = (i - 128) * cgv;
        tab_76309[i] = 76309 * (i - 16);
    }

    for (i = 0; i < 384; i++)
        clp[i] = 0;
    ind = 384;
    for (i = 0; i < 256; i++)
        clp[ind++] = i;
    ind = 640;
    for (i = 0; i < 384; i++)
        clp[ind++] = 255;

    init = 1;
}

void yuv420sp_to_rgb24(int type, unsigned char* ybuffer, unsigned char* UVbuffer, unsigned char* rgbbuffer, int width, int height)
{
    int y1, y2, u = 0, v = 0;
    unsigned char *py1, *py2;
    int i, j, c1, c2, c3, c4;
    unsigned char *d1, *d2;
    unsigned char *src_u;
    static int init_yuv420p = 0;

    src_u = UVbuffer;   // u

    py1 = ybuffer;   // y
    py2 = py1 + width;
    d1 = rgbbuffer;
    d2 = d1 + 3 * width;

	init_yuv420p_table();

    for (j = 0; j < height; j += 2)
    {
        for (i = 0; i < width; i += 2)
        {
            if (type == 0)
            {
                u = *src_u++;
                v = *src_u++;      //
            }
            if (type == 1)
            {
                v = *src_u++;
                u = *src_u++;      //
            }

            c1 = crv_tab[v];
            c2 = cgu_tab[u];
            c3 = cgv_tab[v];
            c4 = cbu_tab[u];

            //up-left   
            y1 = tab_76309[*py1++];
            *d1++ = clp[384 + ((y1 + c1) >> 16)];
            *d1++ = clp[384 + ((y1 - c2 - c3) >> 16)];
            *d1++ = clp[384 + ((y1 + c4) >> 16)];

            //down-left   
            y2 = tab_76309[*py2++];
            *d2++ = clp[384 + ((y2 + c1) >> 16)];
            *d2++ = clp[384 + ((y2 - c2 - c3) >> 16)];
            *d2++ = clp[384 + ((y2 + c4) >> 16)];

            //up-right   
            y1 = tab_76309[*py1++];
            *d1++ = clp[384 + ((y1 + c1) >> 16)];
            *d1++ = clp[384 + ((y1 - c2 - c3) >> 16)];
            *d1++ = clp[384 + ((y1 + c4) >> 16)];

            //down-right   
            y2 = tab_76309[*py2++];
            *d2++ = clp[384 + ((y2 + c1) >> 16)];
            *d2++ = clp[384 + ((y2 - c2 - c3) >> 16)];
            *d2++ = clp[384 + ((y2 + c4) >> 16)];
        }
        d1 += 3 * width;
        d2 += 3 * width;
        py1 += width;
        py2 += width;
    }
}

void yuv420sp_to_rgb24_new(int type, unsigned char* ybuffer, unsigned char* UVbuffer, unsigned char* rgbbuffer, int img_w, int img_h)
{
  const unsigned char* Y = ybuffer;
  const unsigned char* UV = UVbuffer;
  const int ITUR_BT_601_CY = 1220542;
  const int ITUR_BT_601_CUB = 2116026;
  const int ITUR_BT_601_CUG = -409993;
  const int ITUR_BT_601_CVG = -852492;
  const int ITUR_BT_601_CVR = 1673527;
  const int ITUR_BT_601_SHIFT = 20;
#define uchar_up_down_saturate(A) if((A) < 0){(A) = 0;} else if ((A) > 255){ (A) = 255; }
#define MAX2(a, b)  (((a) >= (b)) ? (a): (b))

  const unsigned char *uv, *uv_row;

  /*YUV: 4:2:0   YY...Y UVUV..UV*/
  //R = Y + 1.4075 * (V - 128)                       = Y + 1.4075 * V - 180.16
  //G = Y - 0.3455 * (U - 128) - 0.7169 * (V - 128)  = Y - 0.3455 * U - 0.7169 * V + 135.9872
  //B = Y + 1.779 * (U - 128)                        = Y + 1.779 * U - 227.712
  if (type == 0)
  {
    for (int j = 0; j < img_h; j++)
    {
      uv_row = UV + j / 2 * img_w;
      int idx = j * img_w;
      // 每两个pixel处理一次
      for (int i = 0; i < img_w; i += 2, idx += 2)
      {
        uv = uv_row + i;
        /*correspond to cvtColor(src, dst, CV_YUV2RGB_IYUV) */
        int R1, G1, B1, R2, G2, B2;
        int uv1_minus_128 = uv[1] - 128;
        int uv0_minus_128 = uv[0] - 128;
        int itur_bt_shift_v = (1 << (ITUR_BT_601_SHIFT - 1));
        int Y16_0 = MAX2(0, Y[idx] - 16) * ITUR_BT_601_CY;
        int Y16_1 = MAX2(0, Y[idx + 1] - 16) * ITUR_BT_601_CY;
        short* img_rgb_short = (short*)&rgbbuffer[3 * idx];

        /* 偶数索引pixel： R1, G1*/
        int r_tmp = itur_bt_shift_v + ITUR_BT_601_CVR * uv1_minus_128;
        R1 = (Y16_0 + r_tmp) >> ITUR_BT_601_SHIFT;
        uchar_up_down_saturate(R1);

        int g_tmp = itur_bt_shift_v + ITUR_BT_601_CVG * uv1_minus_128 + ITUR_BT_601_CUG * uv0_minus_128;
        G1 = (Y16_0 + g_tmp) >> ITUR_BT_601_SHIFT;
        uchar_up_down_saturate(G1);

        *img_rgb_short++ = (G1 << 8) | R1;

        /* 偶数索引pixel： B1,  奇数索引pixel: R2*/
        int b_tmp = itur_bt_shift_v + ITUR_BT_601_CUB * uv0_minus_128;
        B1 = (Y16_0 + b_tmp) >> ITUR_BT_601_SHIFT;
        uchar_up_down_saturate(B1);

        R2 = (Y16_1 + r_tmp) >> ITUR_BT_601_SHIFT;
        uchar_up_down_saturate(R2);
        *img_rgb_short++ = (R2 << 8) | B1;

        /* 奇数索引pixel: G2, B2 */
        G2 = (Y16_1 + g_tmp) >> ITUR_BT_601_SHIFT;
        uchar_up_down_saturate(G2);

        B2 = (Y16_1 + b_tmp) >> ITUR_BT_601_SHIFT;
        uchar_up_down_saturate(B2);

        *img_rgb_short++ = (B2 << 8) | G2;
      }
    }
  }

  if (type == 1) {
    for (int j = 0; j < img_h; j++) {
      uv_row = UV + j / 2 * img_w;
      int idx = j * img_w;
      // 每两个pixel处理一次
      for (int i = 0; i < img_w; i += 2, idx += 2) {
        uv = uv_row + i;
        /*correspond to cvtColor(src, dst, CV_YUV2RGB_IYUV) */
        int R1, G1, B1, R2, G2, B2;
        //int uv1_minus_128 = uv[1] - 128;
        //int uv0_minus_128 = uv[0] - 128;
        int uv1_minus_128 = uv[0] - 128;
        int uv0_minus_128 = uv[1] - 128;
        int itur_bt_shift_v = (1 << (ITUR_BT_601_SHIFT - 1));
        int Y16_0 = MAX2(0, Y[idx] - 16) * ITUR_BT_601_CY;
        int Y16_1 = MAX2(0, Y[idx + 1] - 16) * ITUR_BT_601_CY;
        short* img_rgb_short = (short*)&rgbbuffer[3 * idx];

        /* 偶数索引pixel： R1, G1*/
        int r_tmp = itur_bt_shift_v + ITUR_BT_601_CVR * uv1_minus_128;
        R1 = (Y16_0 + r_tmp) >> ITUR_BT_601_SHIFT;
        uchar_up_down_saturate(R1);

        int g_tmp = itur_bt_shift_v + ITUR_BT_601_CVG * uv1_minus_128 + ITUR_BT_601_CUG * uv0_minus_128;
        G1 = (Y16_0 + g_tmp) >> ITUR_BT_601_SHIFT;
        uchar_up_down_saturate(G1);

        *img_rgb_short++ = (G1 << 8) | R1;

        /* 偶数索引pixel： B1,  奇数索引pixel: R2*/
        int b_tmp = itur_bt_shift_v + ITUR_BT_601_CUB * uv0_minus_128;
        B1 = (Y16_0 + b_tmp) >> ITUR_BT_601_SHIFT;
        uchar_up_down_saturate(B1);

        R2 = (Y16_1 + r_tmp) >> ITUR_BT_601_SHIFT;
        uchar_up_down_saturate(R2);
        *img_rgb_short++ = (R2 << 8) | B1;

        /* 奇数索引pixel: G2, B2 */
        G2 = (Y16_1 + g_tmp) >> ITUR_BT_601_SHIFT;
        uchar_up_down_saturate(G2);

        B2 = (Y16_1 + b_tmp) >> ITUR_BT_601_SHIFT;
        uchar_up_down_saturate(B2);

        *img_rgb_short++ = (B2 << 8) | G2;
      }
    }
  }
}


