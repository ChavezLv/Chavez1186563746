/*
 * lk_pyramid_PC.c
 *
 *  Created on: 2018-10-30
 *      Author: Administrator
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../arm_types.h"
#ifdef _ARM_NEON
#include <arm_neon.h>
#endif
#include "slk_tracker_cmm.h"
#include "lk_pyramid_PC.h"
//#include "lib_print.h"

s32 ifcvFloor(f32 value)
{
	if(value >= 0 )
	{
		return (s32)value;
	}
	else
	{
		return (s32)(value - 1);
	}
}

s32 ifcvRound(f32 value)
{
	if(value >= 0 )
	{
		return (s32)(value + 0.5);
	}
	else
	{
		return (s32)(value - 0.5);
	}
}

s32 ifcvCeil(f32 value)
{
	if(value >= 0 )
	{
		return (s32)value + 1;
	}
	else
	{
		return (s32)value;
	}
}

s16 cal_Level_num(s16 w, s16 h)
{
	s16 level = 0;
	s16 levelnum = 0;
	s16 in_w = w;
	s16 in_h = h;

	for (; level < TRACKER_MAX_PYR_LEVEL; level++)
	{
		if (in_w <= TRACKER_WIN_SIZE || in_h <= TRACKER_WIN_SIZE)
		{
			break;
		}

		in_w = (in_w + 1) >> 1;
		in_h = (in_h + 1) >> 1;
		levelnum++;
	}

	return levelnum;
}

void validate_rect(Rect *prect, s32 width, s32 height)
{
	if (prect->x < 0)
	{
		prect->w = prect->w + prect->x;
		prect->x = 0;
	}

	if (prect->y < 0)
	{
		prect->h = prect->h + prect->y;
		prect->y = 0;
	}

	if (prect->w > (width - prect->x))
	{
		prect->w = width - prect->x;
	}

	if (prect->h > (height - prect->y))
	{
		prect->h = height - prect->y;
	}
}

bool cfg_pyr_sharr(s16 levelnum, Rect *PrevRect, Rect *CurRect, u8** pPyrddr_addr, t_pyramid_info* pPrevPyr, t_pyramid_info* pSharrPyr, t_pyramid_info* pCurPyr)
{
    s16 Prevh = PrevRect->h;
    s16 Prevw = PrevRect->w;
    s16 Curh = CurRect->h;
    s16 Curw = CurRect->w;
	u32 pyr_buf_size = 0;
    u32 pyr_size[3] = {0,0,0};

	s32 level = 0;
	for (; level < levelnum; level++)
	{
		//prev pyramid info
		pPrevPyr[level].h = Prevh;
		pPrevPyr[level].w = Prevw;
		pPrevPyr[level].step = pPrevPyr[level].w + TRACKER_WIN_SIZE*2;
		pPrevPyr[level].mem_len = pPrevPyr[level].step * (pPrevPyr[level].h + TRACKER_WIN_SIZE*2);

		pyr_size[0] += pPrevPyr[level].mem_len;
		pyr_buf_size += pPrevPyr[level].mem_len;

		//sharr pyramid info
		pSharrPyr[level].h = Prevh;
		pSharrPyr[level].w = Prevw*2;
		pSharrPyr[level].step = pSharrPyr[level].w + TRACKER_WIN_SIZE*4;
		pSharrPyr[level].mem_len = pSharrPyr[level].step * (pSharrPyr[level].h + TRACKER_WIN_SIZE*2) * 2;

		pyr_size[1] += pSharrPyr[level].mem_len;
		pyr_buf_size += pSharrPyr[level].mem_len;

		//cur pyramid info
		pCurPyr[level].h = Curh;
		pCurPyr[level].w = Curw;
		pCurPyr[level].step = pCurPyr[level].w + TRACKER_WIN_SIZE*2;
		pCurPyr[level].mem_len = pCurPyr[level].step * (pCurPyr[level].h + TRACKER_WIN_SIZE*2);

		pyr_size[2] += pCurPyr[level].mem_len;
		pyr_buf_size += pCurPyr[level].mem_len;

		Prevw = (Prevw + 1) >> 1;
		Prevh = (Prevh + 1) >> 1;
		Curw = (Curw + 1) >> 1;
		Curh = (Curh + 1) >> 1;
	}

    //��Դ���뼰����
   // LIB_ALGO_PRINT("1 : %d\n", pyr_buf_size + 1024);
    *pPyrddr_addr = malloc(pyr_buf_size+1024);
   // LIB_ALGO_PRINT("2 - %p\n", pPyrddr_addr);
	if(NULL == *pPyrddr_addr)
	{
	    return false;
	}
	
	u8* sharr_pyramid_start = *pPyrddr_addr;
	u8* prev_pyramid_start = sharr_pyramid_start + pyr_size[1];
	u8* cur_pyramid_start = prev_pyramid_start + pyr_size[0];

	level = 0;
	for (; level < levelnum; level++)
	{
		//prev pyramid info
		pPrevPyr[level].datastart = prev_pyramid_start;
		pPrevPyr[level].data = pPrevPyr[level].datastart + TRACKER_WIN_SIZE * pPrevPyr[level].step;
		prev_pyramid_start += pPrevPyr[level].mem_len;

		//sharr pyramid info
		pSharrPyr[level].datastart = sharr_pyramid_start;
		pSharrPyr[level].data = pSharrPyr[level].datastart + TRACKER_WIN_SIZE * pSharrPyr[level].step*2;
		sharr_pyramid_start += pSharrPyr[level].mem_len;

		//cur pyramid info
		pCurPyr[level].datastart = cur_pyramid_start;
		pCurPyr[level].data = pCurPyr[level].datastart + TRACKER_WIN_SIZE * pCurPyr[level].step;
		cur_pyramid_start += pCurPyr[level].mem_len;
	}

    return true;
}

void Lk_Pad_Zero(u8 *pBuffer, s16 Skip, s16 Rep)
{
	s32 i = 0;
	s32 gap1 = 8;
	u8 *pSrc = pBuffer;
	u8 *pSrc1 = pSrc+Skip+gap1;
	u8 *pSrc2 = pSrc1+Skip+gap1;
	u8 *pSrc3 = pSrc2+Skip+gap1;
	s16 step = 3*(Skip+gap1);

	for(; i+3 < Rep; i+=4)
	{
	 *pSrc++ = 0;
	 *pSrc1++ = 0;
	 *pSrc2++ = 0;
	 *pSrc3++ = 0;
	 *pSrc++ = 0;
	 *pSrc1++ = 0;
	 *pSrc2++ = 0;
	 *pSrc3++ = 0;
	 *pSrc++ = 0;
	 *pSrc1++ = 0;
	 *pSrc2++ = 0;
	 *pSrc3++ = 0;
	 *pSrc++ = 0;
	 *pSrc1++ = 0;
	 *pSrc2++ = 0;
	 *pSrc3++ = 0;

	 pSrc += Skip;
	 pSrc1 += Skip;
	 pSrc2 += Skip;
	 pSrc3 += Skip;

	 *pSrc++ = 0;
	 *pSrc1++ = 0;
	 *pSrc2++ = 0;
	 *pSrc3++ = 0;
	 *pSrc++ = 0;
	 *pSrc1++ = 0;
	 *pSrc2++ = 0;
	 *pSrc3++ = 0;
	 *pSrc++ = 0;
	 *pSrc1++ = 0;
	 *pSrc2++ = 0;
	 *pSrc3++ = 0;
	 *pSrc++ = 0;
	 *pSrc1++ = 0;
	 *pSrc2++ = 0;
	 *pSrc3++ = 0;

	 pSrc += step;
	 pSrc1 += step;
	 pSrc2 += step;
	 pSrc3 += step;
	}

	for(; i < Rep; i++)
	{
	 *pSrc++ = 0;
	 *pSrc++ = 0;
	 *pSrc++ = 0;
	 *pSrc++ = 0;
	 pSrc += Skip;
	 *pSrc++ = 0;
	 *pSrc++ = 0;
	 *pSrc++ = 0;
	 *pSrc++ = 0;
	}
}

void Lk_Pad_Zero_s16(s16 *pBuffer, s16 Skip, s16 Rep)
{
	s32 i = 0;
	s32 gap1 = 8;
	s16 *pSrc = pBuffer;
	s16 *pSrc1 = pSrc+Skip+gap1;
	s16 *pSrc2 = pSrc1+Skip+gap1;
	s16 *pSrc3 = pSrc2+Skip+gap1;
	s16 step = 3*(Skip+gap1);

	for(; i+3 < Rep; i+=4)
	{
	 *pSrc++ = 0;
	 *pSrc1++ = 0;
	 *pSrc2++ = 0;
	 *pSrc3++ = 0;
	 *pSrc++ = 0;
	 *pSrc1++ = 0;
	 *pSrc2++ = 0;
	 *pSrc3++ = 0;
	 *pSrc++ = 0;
	 *pSrc1++ = 0;
	 *pSrc2++ = 0;
	 *pSrc3++ = 0;
	 *pSrc++ = 0;
	 *pSrc1++ = 0;
	 *pSrc2++ = 0;
	 *pSrc3++ = 0;

	 pSrc += Skip;
	 pSrc1 += Skip;
	 pSrc2 += Skip;
	 pSrc3 += Skip;

	 *pSrc++ = 0;
	 *pSrc1++ = 0;
	 *pSrc2++ = 0;
	 *pSrc3++ = 0;
	 *pSrc++ = 0;
	 *pSrc1++ = 0;
	 *pSrc2++ = 0;
	 *pSrc3++ = 0;
	 *pSrc++ = 0;
	 *pSrc1++ = 0;
	 *pSrc2++ = 0;
	 *pSrc3++ = 0;
	 *pSrc++ = 0;
	 *pSrc1++ = 0;
	 *pSrc2++ = 0;
	 *pSrc3++ = 0;

	 pSrc += step;
	 pSrc1 += step;
	 pSrc2 += step;
	 pSrc3 += step;
	}

	for(; i < Rep; i++)
	{
	 *pSrc++ = 0;
	 *pSrc++ = 0;
	 *pSrc++ = 0;
	 *pSrc++ = 0;
	 pSrc += Skip;
	 *pSrc++ = 0;
	 *pSrc++ = 0;
	 *pSrc++ = 0;
	 *pSrc++ = 0;
	}
}

/*
02. Various border types, image boundaries are denoted with '|'
03.
04. * BORDER_REPLICATE=1:        aaaaaa|abcdefgh|hhhhhhh
05. * BORDER_REFLECT=2:            fedcba|abcdefgh|hgfedcb
06. * BORDER_REFLECT_101=4:    gfedcb|abcdefgh|gfedcba
07. * BORDER_WRAP=3:                cdefgh|abcdefgh|abcdefg
08. * BORDER_CONSTANT=0:         iiiiii|abcdefgh|iiiiiii  with some specified 'i'
09. */
int ifborderInterpolate(int p, int len, int borderType)
{
	if ((unsigned)p < (unsigned)len)
	{
		;
	}
	else if (borderType == IFBORDER_REPLICATE)
	{
		p = p < 0 ? 0 : len - 1;
	}
	else if (borderType == IFBORDER_REFLECT || borderType == IFBORDER_REFLECT_101)
	{
		int delta = borderType == IFBORDER_REFLECT_101;
		if (len == 1)
		{
			return 0;
		}

		do
		{
			if (p < 0)
			{
				p = -p - 1 + delta;
			}
			else
			{
				p = len - 1 - (p - len) - delta;
			}
		} while ((unsigned)p >= (unsigned)len);
	}
	else if (borderType == IFBORDER_WRAP)
	{
		if (p < 0)
		{
			p -= ((p - len + 1) / len)*len;
		}

		if (p >= len)
		{
			p %= len;
		}
	}
	else if (borderType == IFBORDER_CONSTANT)
	{
		p = -1;
	}
	else
	{
		printf("err:CV_StsBadArg, Unknown/unsupported border type!");
	}

	return p;
}

//@zzf for 2^n bytes align it is right
static int ifalignSize(int sz, int n)
{
	return (sz + n - 1) & -n;
}

#define PD_SZ        5
#define PD_SZ_HALF   2
#ifdef _ARM_NEON
int PyrDownVec_32s8u(s32** src, u8* dst, s32 width)
{
    int x = 0;
    const unsigned int *row0 = (unsigned int*)src[0], *row1 = (unsigned int*)src[1],
                       *row2 = (unsigned int*)src[2], *row3 = (unsigned int*)src[3],
                       *row4 = (unsigned int*)src[4];
    uint16x8_t v_delta = vdupq_n_u16(128);

    for( ; x <= width - 16; x += 16 )
    {
        uint16x8_t v_r0 = vcombine_u16(vqmovn_u32(vld1q_u32(row0 + x)), vqmovn_u32(vld1q_u32(row0 + x + 4)));
        uint16x8_t v_r1 = vcombine_u16(vqmovn_u32(vld1q_u32(row1 + x)), vqmovn_u32(vld1q_u32(row1 + x + 4)));
        uint16x8_t v_r2 = vcombine_u16(vqmovn_u32(vld1q_u32(row2 + x)), vqmovn_u32(vld1q_u32(row2 + x + 4)));
        uint16x8_t v_r3 = vcombine_u16(vqmovn_u32(vld1q_u32(row3 + x)), vqmovn_u32(vld1q_u32(row3 + x + 4)));
        uint16x8_t v_r4 = vcombine_u16(vqmovn_u32(vld1q_u32(row4 + x)), vqmovn_u32(vld1q_u32(row4 + x + 4)));

        v_r0 = vaddq_u16(vaddq_u16(v_r0, v_r4), vaddq_u16(v_r2, v_r2));
        v_r1 = vaddq_u16(vaddq_u16(v_r1, v_r2), v_r3);
        uint16x8_t v_dst0 = vaddq_u16(v_r0, vshlq_n_u16(v_r1, 2));

        v_r0 = vcombine_u16(vqmovn_u32(vld1q_u32(row0 + x + 8)), vqmovn_u32(vld1q_u32(row0 + x + 12)));
        v_r1 = vcombine_u16(vqmovn_u32(vld1q_u32(row1 + x + 8)), vqmovn_u32(vld1q_u32(row1 + x + 12)));
        v_r2 = vcombine_u16(vqmovn_u32(vld1q_u32(row2 + x + 8)), vqmovn_u32(vld1q_u32(row2 + x + 12)));
        v_r3 = vcombine_u16(vqmovn_u32(vld1q_u32(row3 + x + 8)), vqmovn_u32(vld1q_u32(row3 + x + 12)));
        v_r4 = vcombine_u16(vqmovn_u32(vld1q_u32(row4 + x + 8)), vqmovn_u32(vld1q_u32(row4 + x + 12)));

        v_r0 = vaddq_u16(vaddq_u16(v_r0, v_r4), vaddq_u16(v_r2, v_r2));
        v_r1 = vaddq_u16(vaddq_u16(v_r1, v_r2), v_r3);
        uint16x8_t v_dst1 = vaddq_u16(v_r0, vshlq_n_u16(v_r1, 2));

        vst1q_u8(dst + x, vcombine_u8(vqmovn_u16(vshrq_n_u16(vaddq_u16(v_dst0, v_delta), 8)),
                                      vqmovn_u16(vshrq_n_u16(vaddq_u16(v_dst1, v_delta), 8))));
    }

    return x;
}
#endif

void Ccv_Gauss_5x5(u8 *src, u8 *dst, s16 src_w, s16 src_h, s16 src_step, s16 dst_w, s16 dst_h, s16 dst_step, int borderType)
{
	int bufstep = (int)ifalignSize(dst_w, 16);
	//int buf[bufstep*PD_SZ + 16];
	int *_buf = (int *)NULL, *buf = (int *)NULL;
	int tabL[IFCV_CN_MAX*(PD_SZ + 2)], tabR[IFCV_CN_MAX*(PD_SZ + 2)];
	int* rows[PD_SZ];

  //LIB_ALGO_PRINT("1 : %d\n", (sizeof(int)* (bufstep * PD_SZ + 16)));
	_buf = (int *)malloc(sizeof(int)* (bufstep * PD_SZ + 16));
 // LIB_ALGO_PRINT("2 - %p\n", _buf);
	buf = _buf;//(int *)((u64)(_buf)&0xffffffffffffffc0)+16;


	int k, x, sy0 = -PD_SZ_HALF, sy = sy0, width0 = MIN2((src_w - PD_SZ_HALF - 1) / 2 + 1, dst_w);  //@zzf for even width, the width0 is width/2 + 1; for uneven width ,the width0 is width/2

	for (x = 0; x <= PD_SZ + 1; x++)
	{
		int sx0 = ifborderInterpolate(x - PD_SZ_HALF, src_w, borderType); //@zzf borderType use default value 4, the borderInterpolate type is reflect.
		int sx1 = ifborderInterpolate(x + width0 * 2 - PD_SZ_HALF, src_w, borderType);
		tabL[x] = sx0; //@zzf tabL and tabR cast x in outside range  to x in inside range 
		tabR[x] = sx1;
	}

	for (int y = 0; y < dst_h; y++)
	{
		u8* dstRow = (u8*)(dst + dst_step*y);
		int *row0, *row1, *row2, *row3, *row4;

		// fill the ring buffer (horizontal convolution and decimation)
		for (; sy <= y * 2 + 2; sy++) //@zzf sy denotes y in src image;
		{
			int* row = buf + ((sy - sy0) % PD_SZ)*bufstep;
			int _sy = ifborderInterpolate(sy, src_h, borderType);
			const u8* srcRow = (const u8*)(src + src_step*_sy);
			const int* tab = tabL;

			for (x = 0;;)
			{
				row[x] = srcRow[tab[x + 2]] * 6 + (srcRow[tab[x + 1]] + srcRow[tab[x + 3]]) * 4 +
					srcRow[tab[x]] + srcRow[tab[x + 4]];
				x++;

				if (x == dst_w)
					break;

				for (; x < width0; x++)
				{
					row[x] = srcRow[x * 2] * 6 + (srcRow[x * 2 - 1] + srcRow[x * 2 + 1]) * 4 +
						srcRow[x * 2 - 2] + srcRow[x * 2 + 2];
				}

				tab = tabR - x;
			}
		}

		// do vertical convolution and decimation and write the result to the destination image
		for (k = 0; k < PD_SZ; k++)
		{
			rows[k] = buf + ((y * 2 - PD_SZ_HALF + k - sy0) % PD_SZ)*bufstep;
		}

		row0 = rows[0]; row1 = rows[1]; row2 = rows[2]; row3 = rows[3]; row4 = rows[4];

#ifdef _ARM_NEON
		x = PyrDownVec_32s8u(rows, dstRow, dst_w);
#else
		x = 0;
#endif

		for (; x < dst_w; x++) //@zzf core code
		{
			dstRow[x] = (u8)(((row2[x] * 6 + (row1[x] + row3[x]) * 4 + row0[x] + row4[x]) + 128) >> 8);
		}
	}
	free(_buf);
}

/*
   -3  -10    -3
    0   0      0
    3   10     3
    ---------------

    -3     0     3
    -10    0     10
    -3     0     3
*/
//@zzf no name namespace can only be used in the file where it is defined.
//dst��ͨ���ֱ�洢df/dx,  df/dy
static void Ccv_Sharr_3x3(u8 *src, s16 *dst, s16 src_w, s16 src_h, s16 src_step, s16 dst_w, s16 dst_h, s16 dst_step)
{
	int rows = src_h, cols = src_w, cn = 1, colsn = cols*cn;//depth = src.depth();
	int x, y, delta = (((cols + 2)*cn)&0xfffffff0) + 16;
	s16 *trow0 = NULL, *trow1 = NULL, *_tempBuf = NULL;
  //LIB_ALGO_PRINT("1 : %d\n", ((delta * 2 + 64) * sizeof(s16)));
	_tempBuf = (s16 *)malloc((delta * 2 + 64) * sizeof(s16));
  //LIB_ALGO_PRINT("2 - %p\n", _tempBuf);
	trow0 = _tempBuf + cn;//(s16 *)(((u64)(_tempBuf + cn))&0xffffffffffffffe0) + 16;
	trow1 = trow0 + delta;

	for (y = 0; y < rows; y++)
	{
		const u8* srow0 = src + src_step * (y > 0 ? y - 1 : rows > 1 ? 1 : 0);
		const u8* srow1 = src + src_step * y;
		const u8* srow2 = src + src_step * (y < rows - 1 ? y + 1 : rows > 1 ? rows - 2 : 0);
		s16* drow = (s16 *)(dst + y * dst_step);

		// do vertical convolution
		x = 0;
#ifdef _ARM_NEON
        const uint16x8_t q8 = vdupq_n_u16(3);
        const uint8x8_t d18 = vdup_n_u8(10);

        const int16x8_t q8i = vdupq_n_s16(3);
        const int16x8_t q9 = vdupq_n_s16(10);

        for( ; x <= colsn - 8; x += 8)
        {
            uint8x8_t d0 = vld1_u8((const uint8_t*)&srow0[x]);
            uint8x8_t d1 = vld1_u8((const uint8_t*)&srow1[x]);
            uint8x8_t d2 = vld1_u8((const uint8_t*)&srow2[x]);
            uint16x8_t q4 = vaddl_u8(d0, d2);
            uint16x8_t q11 = vsubl_u8(d2, d0);
            uint16x8_t q5 = vmulq_u16(q4, q8);
            uint16x8_t q6 = vmull_u8(d1, d18);
            uint16x8_t q10 = vaddq_u16(q6, q5);
            vst1q_u16((uint16_t*)&trow0[x], q10);
            vst1q_u16((uint16_t*)&trow1[x], q11);

        }
#endif
        for( ; x < colsn; x++ )
        {
			int t0 = (srow0[x] + srow2[x]) * 3 + srow1[x] * 10;//��-[3 10 3],    ����������ˮƽ������������
			int t1 = srow2[x] - srow0[x];                      //��[-3 0  3]����3������ˮƽ���������
			trow0[x] = (s16)t0;
			trow1[x] = (s16)t1;
		}

		// make border
		int x0 = (cols > 1 ? 1 : 0)*cn, x1 = (cols > 1 ? cols - 2 : 0)*cn;
		for (int k = 0; k < cn; k++)
		{
			trow0[-cn + k] = trow0[x0 + k];
			trow0[colsn + k] = trow0[x1 + k];
			trow1[-cn + k] = trow1[x0 + k];
			trow1[colsn + k] = trow1[x1 + k];
		}

		// do horizontal convolution, interleave the results and store them to dst
		x = 0;
#ifdef _ARM_NEON
        for( ; x <= colsn - 8; x += 8 )
        {

            int16x8_t q0 = vld1q_s16((const int16_t*)&trow0[x+cn]);
            int16x8_t q1 = vld1q_s16((const int16_t*)&trow0[x-cn]);
            int16x8_t q2 = vld1q_s16((const int16_t*)&trow1[x+cn]);
            int16x8_t q3 = vld1q_s16((const int16_t*)&trow1[x-cn]);
            int16x8_t q5 = vsubq_s16(q0, q1);
            int16x8_t q6 = vaddq_s16(q2, q3);
            int16x8_t q4 = vld1q_s16((const int16_t*)&trow1[x]);
            int16x8_t q7 = vmulq_s16(q6, q8i);
            int16x8_t q10 = vmulq_s16(q4, q9);
            int16x8_t q11 = vaddq_s16(q7, q10);
            int16x4_t d22 = vget_low_s16(q11);
            int16x4_t d23 = vget_high_s16(q11);
            int16x4_t d11 = vget_high_s16(q5);
            int16x4_t d10 = vget_low_s16(q5);
            int16x4x2_t q5x2, q11x2;
            q5x2.val[0] = d10; q5x2.val[1] = d22;
            q11x2.val[0] = d11; q11x2.val[1] = d23;
            vst2_s16((int16_t*)&drow[x*2], q5x2);
            vst2_s16((int16_t*)&drow[(x*2)+8], q11x2);

        }
#endif
		for (; x < colsn; x++)
		{
			s16 t0 = (s16)(trow0[x + cn] - trow0[x - cn]);// cn=1    ��[-3 0 3]
			s16 t1 = (s16)((trow1[x + cn] + trow1[x - cn]) * 3 + trow1[x] * 10);  //��[3 10 3]
			drow[x * 2] = t0;
			drow[x * 2 + 1] = t1;
		}
	}

	free(_tempBuf);
	_tempBuf = NULL;
	trow0 = NULL;
	trow1 = NULL;
}

void buildOpticalFlowPyramidPC(bool prevInd, Rect *zone, s16 lvlNum, u8 *img_gray, s16 img_h, s16 img_w, t_pyramid_info* pPyr, t_pyramid_info* pSharr)
{
    s16 i=0;

    //level == 0;
	//��ȡ����
	u8* pInEx = img_gray+zone->y*img_w+zone->x;
	u8* pOutEx = pPyr[0].datastart;
	s16 in_w = pPyr[0].w;
	s16 in_h = pPyr[0].h;
	s16 in_hsize = in_w + TRACKER_WIN_SIZE*2;

    memset(pOutEx,0,TRACKER_WIN_SIZE*in_hsize);
	memset(pOutEx+(in_h+TRACKER_WIN_SIZE)*in_hsize,0,TRACKER_WIN_SIZE*in_hsize);
    pOutEx += TRACKER_WIN_SIZE*in_hsize;
	
	for(;i<in_h;i++)
	{
		memset(pOutEx,0,TRACKER_WIN_SIZE);
		pOutEx+=TRACKER_WIN_SIZE;
		
		memcpy(pOutEx,pInEx,in_w);
		pOutEx+=in_w;
		pInEx+=img_w;
		
		memset(pOutEx,0,TRACKER_WIN_SIZE);
		pOutEx+=TRACKER_WIN_SIZE;
	}

    //level != 0
	for(i=1;i<lvlNum;i++)
	{
		s32 prev_i = i-1;
		//��ȡ����
		in_w = pPyr[prev_i].w;
		in_h = pPyr[prev_i].h;
		in_hsize = in_w + TRACKER_WIN_SIZE*2;

		pInEx = pPyr[prev_i].data+TRACKER_WIN_SIZE;
		pOutEx = pPyr[i].datastart;

		s16 out_w = pPyr[i].w;
		s16 out_h = pPyr[i].h;
		s16 out_hsize = out_w + TRACKER_WIN_SIZE*2;

	    memset(pOutEx,0,TRACKER_WIN_SIZE*in_hsize);
		memset(pOutEx+(out_h+TRACKER_WIN_SIZE)*out_hsize,0,TRACKER_WIN_SIZE*in_hsize);
	    pOutEx += TRACKER_WIN_SIZE*out_hsize;

	    Lk_Pad_Zero(pOutEx, out_w, out_h);
		Ccv_Gauss_5x5(pInEx, pOutEx+TRACKER_WIN_SIZE, in_w, in_h, in_hsize, out_w, out_h, out_hsize, IFBORDER_REFLECT_101);
		
	    //sharr
		if(prevInd)
		{
			s16* pSharr_OutEx = (s16*)pSharr[prev_i].datastart;
			s32 sharr_in_hsize = pSharr[prev_i].step;
			memset(pSharr_OutEx,0,TRACKER_WIN_SIZE*sharr_in_hsize*2);
		    memset(pSharr_OutEx+(in_h+TRACKER_WIN_SIZE)*sharr_in_hsize,0,TRACKER_WIN_SIZE*sharr_in_hsize*2);
			pSharr_OutEx += TRACKER_WIN_SIZE*sharr_in_hsize;
			Lk_Pad_Zero_s16(pSharr_OutEx, pSharr[prev_i].w, in_h);
			Ccv_Sharr_3x3(pInEx, pSharr_OutEx+TRACKER_WIN_SIZE*2, in_w, in_h, in_hsize, pSharr[prev_i].w, in_h, sharr_in_hsize);
		}
	}

	//sharr
	if(prevInd)
	{
		s32 prev_i = i-1;
		//��ȡ����
		in_w = pPyr[prev_i].w;
		in_h = pPyr[prev_i].h;
		in_hsize = in_w + TRACKER_WIN_SIZE*2;

		pInEx = pPyr[prev_i].data+TRACKER_WIN_SIZE;
		s16* pSharr_OutEx = (s16*)pSharr[prev_i].datastart;

		s32 sharr_in_hsize = pSharr[prev_i].step;
		memset(pSharr_OutEx,0,TRACKER_WIN_SIZE*sharr_in_hsize*2);
		memset(pSharr_OutEx+(in_h+TRACKER_WIN_SIZE)*sharr_in_hsize,0,TRACKER_WIN_SIZE*sharr_in_hsize*2);
		pSharr_OutEx += TRACKER_WIN_SIZE*sharr_in_hsize;
		Lk_Pad_Zero_s16(pSharr_OutEx, pSharr[prev_i].w, in_h);
		Ccv_Sharr_3x3(pInEx, pSharr_OutEx+TRACKER_WIN_SIZE*2, in_w, in_h, in_hsize, pSharr[prev_i].w, in_h, sharr_in_hsize);
	}
}

#ifndef FLT_EPSILON
#define FLT_EPSILON	1.192092896e-07F
#endif

#ifndef IFCV_DESCALE
#define  IFCV_DESCALE(x,n)     (((x) + (1 << ((n)-1))) >> (n))
#endif


//static IFaceSDK::Size LKTrackWinSize(4U, 4U );
//static IFaceSDK::Point2f LKTrackHalfWinSize(1.5f, 1.5f);

#define LK_TRACK_WIN_X	4
#define LK_TRACK_WIN_Y	4
#define LK_TRACK_HALF_WIN_X	1.5f
#define LK_TRACK_HALF_WIN_Y	1.5f

void LKTrackerRun_1level(int nPoints,
	const unsigned char *I,//prevImg,
	const short *prevDeriv,
	const unsigned char *J,//nextImg,
	const Point2f* prevPts,
	Point2f* nextPts,
	int stepI, int dstep, int stepJ,
	int img_h, int img_w,
	unsigned char* status,
	TermCriteria *criteria,
	int level,
	int maxLevel)
	{
		const int W_BITS = 14, W_BITS1 = 14;
		const float FLT_SCALE = 1.f / (1 << 20);

		int x, y;
		const float minEigThreshold_16x = TRACKER_EIGEN_TH_16x;
		const float minEigThreshold_8x = TRACKER_EIGEN_TH_8x;
		/* �����Ĺ۲촰�ڣ�[4,4] */
		const int win_width = 4, win_height = 4;
		const float hwin_width = 1.5f, hwin_height = 1.5f;
		short IWinBuf[4][4];
		short derivIWinBuf[4][4][2];
		float levelFactor = (float)(1. / (1 << level));

		/* �ڶ�Ӧ������level���������������ƫ�� */
		for (int ptidx = 0; ptidx < nPoints; ptidx++) {
		    Point2f prevPt, nextPt;
			prevPt.x = prevPts[ptidx].x * levelFactor;
			prevPt.y = prevPts[ptidx].y * levelFactor;

			/* nextPtΪ���ٵĹ�����λ�ã����Ϊ���������㣬��ô��ʼ��Ϊԭʼλ��*levelFactor��
			�������һ�����䵽��һ�㣬��ôpts������չ2�� */
			if (level != maxLevel)
			{
				nextPt.x = nextPts[ptidx].x * (float)2.0;
				nextPt.y = nextPts[ptidx].y * (float)2.0;
			}
			else
			{
				nextPt.x = nextPts[ptidx].x * levelFactor;
				nextPt.y = nextPts[ptidx].y * levelFactor;
			}
			nextPts[ptidx] = nextPt;

			/* ���¼���prevPt�Ĵ���λ�ã����ϣ�*/
			Point2i iprevPt, inextPt;
			prevPt.x -= hwin_width;
			prevPt.y -= hwin_height;
			iprevPt.x = ifcvFloor(prevPt.x);
			iprevPt.y = ifcvFloor(prevPt.y);

			float a = prevPt.x - iprevPt.x;
			float b = prevPt.y - iprevPt.y;
			int iw00 = ifcvRound((1.f - a)*(1.f - b)*(1 << W_BITS)); //iw����ԭ��˫���Բ�ֵ
			int iw01 = ifcvRound(a*(1.f - b)*(1 << W_BITS));
			int iw10 = ifcvRound((1.f - a)*b*(1 << W_BITS));
			int iw11 = (1 << W_BITS) - iw00 - iw01 - iw10; // iw00 + iw01 + iw10 + iw11 = 1
			long iA11 = 0, iA12 = 0, iA22 = 0;
			float A11, A12, A22;

			/* step 1: ����I֡�ϸ��ٵ㴰�ڵ�Ival(�Ҷ�ֵ)��Ix,Iy�ݶ�ֵ��2ά�����Բ�ֵ */
			for (y = 0; y < 4; y++) {
				const unsigned char* src = I + (y + iprevPt.y)*stepI + iprevPt.x; //��-winSize.height <= y+iprevPt.y <0  ʱ����ȡI��ROI����߿�����
				const short* dsrc = prevDeriv + (y + iprevPt.y)*dstep + iprevPt.x * 2;

				short* Iptr = (short*)(IWinBuf[y]);
				short* dIptr = (short*)(derivIWinBuf[y]);

				for (x = 0; x < 4; x++, dsrc += 2, dIptr += 2) {
					short ival = IFCV_DESCALE(src[x] * iw00 + src[x + 1] * iw01 +
						src[x + stepI] * iw10 + src[x + stepI + 1] * iw11, W_BITS1 - 5);
					short ixval = IFCV_DESCALE(dsrc[0] * iw00 + dsrc[2] * iw01 +
						dsrc[dstep] * iw10 + dsrc[dstep + 2] * iw11, W_BITS1);
					short iyval = IFCV_DESCALE(dsrc[1] * iw00 + dsrc[3] * iw01 + dsrc[dstep + 1] * iw10 +
						dsrc[dstep + 3] * iw11, W_BITS1);

					Iptr[x] = ival;
					dIptr[0] = ixval;
					dIptr[1] = iyval;

					iA11 += ixval*ixval;    //��Ӧ��ʽ�е�A_t * A,    �Ŵ���2^10
					iA12 += ixval*iyval;    //�Խ�Ԫ�����A12=A21
					iA22 += iyval*iyval;
				}
			}

			A11 = iA11 * FLT_SCALE;
			A12 = iA12 * FLT_SCALE;
			A22 = iA22 * FLT_SCALE;

			/* step 2: Check һ�¸��ٵ㴰�ڵľ����Ƿ�Ϊ������������� */
			/*
				��֪�� minEigThreshold_16x = minEigThreshold * 16, minEigThreshold_8x = minEigThreshold * 8
				D = A11*A22 - A12*A12;
				A = [	A11		A12
						A12		A22		]
				minEig = { (A22 + A11) - sqrt[ (A11-A22)^2 + 4*A12*A12] } / [2 * (width * height)]
				minEig < minEigThreshold , width = 4, height = 4
				===> { (A22 + A11) - sqrt[ (A11-A22)^2 + 4*A12*A12] } / [2 * (width * height)] < minEigThreshold
				===> { (A22 + A11) - sqrt[ (A11-A22)^2 + 4*A12*A12] } < 32 * minEigThreshold
				===> (A22 + A11) - 32 * minEigThreshold  < sqrt[ (A11-A22)^2 + 4*A12*A12 ]
				===> ((A22 + A11 - 32 * minEigThreshold)^2 < (A11-A22)^2 + 4*A12*A12
				===> A11*A22 - A12*A12 < 16 * minEigThreshold*(A22 + A11) - 128 * minEigThreshold ^ 2
				===> D < minEigThreshold_16x * ( A22 + A11 ) - minEigThreshold_16x  * minEigThreshold_8x
				===> D < minEigThreshold_16x *[ ( A22 + A11 ) - minEigThreshold_8x ]
				˵������OpenCV�Ϲ�ʽһ�£���ʵ�ַ�ʽ��������thres���޼�Сֵ���������߼��������𣬵�ʹ����չ��ʽ��׼ȷ
				ԭʼͼ����level == 1 && ptidx == 68�����㷨��filter��
			*/
			float D = A11*A22 - A12*A12;
			if ( D < minEigThreshold_16x * (A11 + A22 - minEigThreshold_8x)) {
				//printf("--------------- level %d, ptidx %d, D = %f, FLT_EPSILON = %f\r\n", level, ptidx, D, FLT_EPSILON);
				if (level == 0)
				{
					status[ptidx] = false;
				}
				continue;
			}

			D = 1.f / D;

			/* step 3: �ڱ�level���������㣩�ϵ������� */
			/* ���¼���NextPts�Ĵ���λ�ã����ϣ�*/
			nextPt.x -= hwin_width;
			nextPt.y -= hwin_height;
			Point2f prevDelta;
			prevDelta.x = 0.f;
			prevDelta.y = 0.f; //ǰ�ε���ʣ��delta
			
			int iteridx;

			for (iteridx = 0; iteridx < criteria->maxCount; iteridx++) {
				inextPt.x = ifcvFloor(nextPt.x);
				inextPt.y = ifcvFloor(nextPt.y);

				/* nextPt���ٵ���ܳ�������Ҫcheckһ�� */
				if (inextPt.x < -win_width || inextPt.x >= img_w ||
					inextPt.y < -win_height || inextPt.y >= img_h)
				{
					if (level == 0)
						status[ptidx] = false;
					break;
				}

				a = nextPt.x - inextPt.x;
				b = nextPt.y - inextPt.y;
				iw00 = ifcvRound((1.f - a)*(1.f - b)*(1 << W_BITS));//w
				iw01 = ifcvRound(a*(1.f - b)*(1 << W_BITS));
				iw10 = ifcvRound((1.f - a)*b*(1 << W_BITS));
				iw11 = (1 << W_BITS) - iw00 - iw01 - iw10;
				int ib1 = 0, ib2 = 0;
				float b1 = 0, b2 = 0;
				for (y = 0; y < win_height; y++)
				{
					const unsigned char* Jptr = J + (y + inextPt.y)*stepJ + inextPt.x;
					const short* Iptr = (const short*)(IWinBuf[y]);
					const short* dIptr = (const short*)(derivIWinBuf[y]);

					for (x = 0; x < win_width; x++, dIptr += 2)
					{
						//��Ӧ��ʽ��J(x,y) - I(x,y)
						int diff = IFCV_DESCALE(Jptr[x] * iw00 + Jptr[x + 1] * iw01 +
							Jptr[x + stepJ] * iw10 + Jptr[x + stepJ + 1] * iw11,
							W_BITS1 - 5) - Iptr[x];
						ib1 += diff*dIptr[0];
						ib2 += diff*dIptr[1];
					}
				}

				b1 = (float)ib1 * FLT_SCALE;
				b2 = (float)ib2 * FLT_SCALE;

				Point2f delta;
				delta.x = (float)((A12*b2 - A22*b1) * D);
				delta.y = (float)((A12*b1 - A11*b2) * D);

				nextPt.x += delta.x;
				nextPt.y += delta.y;

				if (((double)delta.x * delta.x + (double)delta.y * delta.y) <= criteria->epsilon) {
					break;
				}
				//������� "  ����λ��+ ǰ�ε���λ��"     ��С����ǰ��������
				if ((iteridx > 0) && (fabs(delta.x + prevDelta.x) < 0.01) &&
					(fabs(delta.y + prevDelta.y) < 0.01)) {
					nextPt.x -= delta.x * 0.5f;
					nextPt.y -= delta.y * 0.5f;
					break;
				}
				prevDelta = delta;
			}
			nextPts[ptidx].x = nextPt.x + hwin_width;
			nextPts[ptidx].y = nextPt.y + hwin_height;

			//printf("ptidx : %d, pos = [%f, %f]\r\n", ptidx, nextPts[ptidx].x, nextPts[ptidx].y);

			/* �ڽ������ײ�ȷ�ϸ��ٵ�λ���Ƿ���߽� */
			if ((level == 0) && (status[ptidx])) {
			    Point2i inextPoint;
				inextPoint.x = ifcvFloor(nextPts[ptidx].x - hwin_width);
				inextPoint.y = ifcvFloor(nextPts[ptidx].y - hwin_height);

				if (inextPoint.x < -win_width || inextPoint.x >= img_w ||
					inextPoint.y < -win_height || inextPoint.y >= img_h) {
					status[ptidx] = false;
				}
			}
		}
}

void LKTrackerRun(s16 maxLevel, Point2f* prevPts, Point2f* nextPts, u8* status, t_pyramid_info* pPyrPre, t_pyramid_info* pSharr, t_pyramid_info* pPyrCur)
{
    s32 i = 0;
    TermCriteria criteria;
	//float lvl_factor,cur_lvl_factor, factorTb[6] = {1.0, 0.5, 0.25, 0.125, 0.0625, 0.03125};//(float)(1. / (1 << lvl))
	//Point2f pre_point[16], cur_point[16];
	criteria.maxCount = TRACKER_LK_ITERA_CNT;
	criteria.epsilon = TRACKER_LK_EPLISION;

	//cur_lvl_factor = factorTb[maxLevel-1];
	for(i=maxLevel-1;i>=0;i--)
	{
		//��ȡ����
		//lvl_factor = factorTb[i];

		u8* pInPreEx = pPyrPre[i].data+TRACKER_WIN_SIZE;
		s16* pInSharrEx = (s16*)pSharr[i].data+TRACKER_WIN_SIZE*2;
		u8* pInCurEx = pPyrCur[i].data+TRACKER_WIN_SIZE;

		s16 j=0;
		for(;j<TRACKER_PT_CNT_XY;j++)
		{
			Point2f* prevPts0 = &prevPts[TRACKER_PT_CNT_XY*j];
			Point2f* nextPts0 = &nextPts[TRACKER_PT_CNT_XY*j];
			#if 0
			s16 k=0;
			for(;k<TRACKER_PT_CNT_XY;k++)
			{
				pre_point[k].x = prevPts0[k].x*lvl_factor;
				pre_point[k].y = prevPts0[k].y*lvl_factor;
				cur_point[k].x = nextPts0[k].x*cur_lvl_factor;
				cur_point[k].y = nextPts0[k].y*cur_lvl_factor;
			}
			#endif
			
			LKTrackerRun_1level(TRACKER_PT_CNT_XY,
				pInPreEx,//prevImg,
				pInSharrEx,
				pInCurEx,//nextImg,
				prevPts0,
				nextPts0,
				pPyrPre[i].step, pSharr[i].step, pPyrCur[i].step,
				pPyrCur[i].h,pPyrCur[i].w,
				status,
				&criteria,
				i,
				maxLevel-1);
			//memcpy(nextPts0,cur_point,TRACKER_PT_CNT_XY*sizeof(Point2f));
		}
		//
		//cur_lvl_factor = 2;
	}
}

bool calcOpticalFlowPyrLKPC(Rect *pre_face, u8 *pre_gray, u8 *cur_gray, Point2f* nextPts, Point2f* prevPts, u8* status, s16 height, s16 width)
{
	s16 maxLevel, edgePad;
	Rect prevIRect, curIRect;

	/*cal pyramid layer*/
	maxLevel = cal_Level_num(pre_face->w, pre_face->h);

	/*cal pyramid zone*/
	edgePad = 1 << (maxLevel-1);
	prevIRect.x = pre_face->x - edgePad;
	prevIRect.y = pre_face->y - edgePad;
	prevIRect.w = pre_face->w + 2*edgePad;
	prevIRect.h = pre_face->h + 2*edgePad;

	curIRect.x = prevIRect.x - pre_face->w;
	curIRect.y = prevIRect.y - pre_face->h;
	curIRect.w = prevIRect.w + 2 * pre_face->w;
	curIRect.h = prevIRect.h + 2 * pre_face->h;

	validate_rect(&prevIRect, width, height);
	validate_rect(&curIRect, width, height);

  //LIB_ALGO_PRINT("1 : %d\n", (3*sizeof(t_pyramid_info)*maxLevel));
	u8* ppyrbuf_Temp = (u8 *)malloc(3*sizeof(t_pyramid_info)*maxLevel);
  //LIB_ALGO_PRINT("2 - %p\n", ppyrbuf_Temp);
	if(ppyrbuf_Temp == NULL)
	{
		return false;
	}  
    
	t_pyramid_info* g_pre_pyr = (t_pyramid_info*)ppyrbuf_Temp;
	t_pyramid_info* g_pre_sharr = g_pre_pyr+maxLevel;
	t_pyramid_info* g_cur_pyr = g_pre_sharr+maxLevel;
	u8* pDdrbuf_Temp = NULL;
	/*cal pyramid layer and level*/
	if(false == cfg_pyr_sharr(maxLevel, &prevIRect, &curIRect, &pDdrbuf_Temp, g_pre_pyr, 
		g_pre_sharr, g_cur_pyr))
	{
	    free(ppyrbuf_Temp);
		return false;
	}

	/*cal pre pyramid*/
	buildOpticalFlowPyramidPC(true, &prevIRect, maxLevel, pre_gray, height, width, g_pre_pyr, g_pre_sharr);

	/*cal cur pyramid*/
	buildOpticalFlowPyramidPC(false, &curIRect, maxLevel, cur_gray, height, width, g_cur_pyr, NULL);

	/* generate grid points to track; cal relateive AXIS, init status 1*/
	s16 npoints = TRACKER_PT_CNT_XY*TRACKER_PT_CNT_XY;
	s16 i;
	for (i = 0; i < npoints; i++)
	{
		nextPts[i].x = prevPts[i].x - curIRect.x;     //nextPts�Ǵ�Ԥ��ĵ㣬�˴���prevPts�����ʼ��
		nextPts[i].y = prevPts[i].y - curIRect.y;
		prevPts[i].x = prevPts[i].x - prevIRect.x;
		prevPts[i].y = prevPts[i].y - prevIRect.y;
		status[i] = true;
		//printf("[%f %f][%f %f]\n", nextPts[i].x, nextPts[i].y, prevPts[i].x, prevPts[i].y);
	}
	
	/*LK iterator*/
	LKTrackerRun(maxLevel, prevPts, nextPts, status, g_pre_pyr, g_pre_sharr, g_cur_pyr);

	free(pDdrbuf_Temp);
	free(ppyrbuf_Temp);
	ppyrbuf_Temp = NULL;
	pDdrbuf_Temp = NULL;

	/* recover absolute AXIS*/
	for (i = 0; i < npoints; i++)
	{
		nextPts[i].x += curIRect.x; 
		nextPts[i].y += curIRect.y;
		prevPts[i].x += prevIRect.x;
		prevPts[i].y += prevIRect.y;
	}
	return true;
}

