#include "../arm_types.h"
#ifdef _ARM_NEON
#include <arm_neon.h>
#endif
#include "resize.h"

#ifdef _ARM_NEON
void memcpy_neon(unsigned char *dst, unsigned char *src, int len) {
    int jj = 0;
    unsigned char* src_in1 = src;
    unsigned char* src_out1 = dst;
    uint8x16_t data_u8;
    for(;jj+15<len;jj+=16)
    {
        data_u8 = vld1q_u8(src_in1); 
        vst1q_u8(src_out1,data_u8);
        
        src_in1 += 16;
        src_out1 += 16;
    }
    
    uint8x8_t data1_u8;
    for(;jj+7<len;jj+=8)
    {
        data1_u8 = vld1_u8(src_in1); 
        vst1_u8(src_out1,data1_u8);
        
        src_in1 += 8;
        src_out1 += 8;                                
    }  
    
    for(;jj<len;jj++)
    {
        *src_out1 = *src_in1;
        src_in1 ++;
        src_out1 ++;                                
    }

}
#endif

void Cmm_Adjust_Box_New(short* pBox, int Img_h, int Img_w, Pull_Type Pull)
{
 int top_x = pBox[0];
 int top_y = pBox[1];
 int buttom_x = pBox[2];
 int buttom_y = pBox[3];
 int width = pBox[2] - pBox[0] + 1;
 int height = pBox[3] - pBox[1] + 1;

 if(E_NEED_PULL_HEIGHT0 == Pull)
 {
	 int scale_up = (int)((float)height * 0.5);
	 int scale_down = (int)((float)height * 0.1);

	 top_y -= scale_up;
	 buttom_y += scale_down;

	 height = buttom_y - top_y + 1;
	 if(height > width)
	 {
		 int hal_w = (height - width)>>1;
		 top_x -= hal_w;
		 buttom_x += hal_w;
	 }
	 else if(width > height)
	 {
		 int hal_h = (width - height)>>1;
		 top_y -= hal_h;
		 buttom_y += hal_h;
	 }
 }
 else if(E_NEED_PULL_HEIGHT1 == Pull)
 {
	 if(height > width)
	 {
		 int hal_w = (height - width)>>1;
		 top_x -= hal_w;
		 buttom_x += hal_w;
	 }
	 else if(width > height)
	 {
		 int hal_h = (width - height)>>1;
		 top_y -= hal_h;
		 buttom_y += hal_h;
	 }

	 //H扩展0.083
	 height = buttom_y - top_y + 1;
	 int scale = (int)((float)height * 0.083);

	 top_y -= scale;
	 buttom_y += scale;
 }
 else if(E_NEED_PULL_HW0 == Pull)
 {
	 if(height > width)
	 {
		 int hal_w = (height - width)>>1;
		 top_x -= hal_w;
		 buttom_x += hal_w;
	 }
	 else if(width > height)
	 {
		 int hal_h = (width - height)>>1;
		 top_y -= hal_h;
		 buttom_y += hal_h;
	 }

	 //H扩展0.12
	 height = buttom_y - top_y + 1;
	 int scale = (int)((float)height * 0.12);

	 top_y -= scale;
	 buttom_y += scale;

	 //W扩展0.06
	 width = buttom_x - top_x + 1;
	 scale = (int)((float)width * 0.06);

	 top_x -= scale;
	 buttom_x += scale;
 }

 if(top_x < 0) top_x = 0;
 if(top_y < 0) top_y = 0;
 if(buttom_x >= Img_w) buttom_x = Img_w - 1;
 if(buttom_y >= Img_h) buttom_y = Img_h - 1;

 pBox[0] = top_x;
 pBox[1] = top_y;
 pBox[2] = buttom_x;
 pBox[3] = buttom_y;
}

#ifdef _ARM_NEON
void face_resize_scale_adjust(int _src_h, int _src_w, int _search_h, int _search_w, int _dst_h, int _dst_w,
		int _box_x, int _box_y, short* _sx, short* _cbufxi, short* _sy, short* _cbufyi)
{
	double invscale_x = (double)_dst_w / _search_w;
	double invscale_y = (double)_dst_h / _search_h;
	double scale_x = 1./invscale_x;
	double scale_y = 1./invscale_y;  
	short* sx = _sx;
	short* cbufxi = _cbufxi;
	short* sy = _sy;
	short* cbufyi = _cbufyi;

	float afx[4];
	int w = 0;
	for (; w+3 < _dst_w; w+=4)
	{
		afx[0] = (float)((w + 0.5) * scale_x - 0.5);
		afx[1] = (float)((w + 1.5) * scale_x - 0.5);
		afx[2] = (float)((w + 2.5) * scale_x - 0.5);
		afx[3] = (float)((w + 3.5) * scale_x - 0.5);

		float32x4_t afx_data = vld1q_f32(afx);
		int32x4_t sx_data = vcvtq_s32_f32(afx_data);
		float32x4_t sx_data_f = vcvtq_f32_s32(sx_data);

		afx_data = vsubq_f32(afx_data,sx_data_f);
		int32x4_t all_box_x = vdupq_n_s32(_box_x);
		sx_data = vaddq_s32(sx_data,all_box_x);
		
		int32x4_t all_zeros = vdupq_n_s32(0);
		uint32x4_t cond = vcltq_s32(sx_data,all_zeros);
		sx_data = vbslq_s32(cond,all_zeros,sx_data);
		float32x4_t all_zeros_f = vdupq_n_f32(0);
		afx_data = vbslq_f32(cond,all_zeros_f,afx_data);

		int32x4_t all_maxs = vdupq_n_s32((int)(_src_w - 1));
		cond = vcltq_s32(sx_data,all_maxs);
		sx_data = vbslq_s32(cond,sx_data,all_maxs);
		afx_data = vbslq_f32(cond,afx_data,all_zeros_f);

		int16x4_t sx_data_16 = vmovn_s32(sx_data);
		vst1_s16(&sx[w],sx_data_16);

		float32x4_t fixed_data = vdupq_n_f32(2048);
		float32x4_t fixed_data2 = vdupq_n_f32(0.5);
		float32x4_t cbufx1_f = vmlaq_f32(fixed_data2,afx_data,fixed_data);
		float32x4_t all_ones_f = vdupq_n_f32(1);
		float32x4_t cbufx0_f = vmlaq_f32(fixed_data2,vsubq_f32(all_ones_f,afx_data),fixed_data);
		int16x4_t cbufx0_data = vmovn_s32(vcvtq_s32_f32(cbufx0_f));
		int16x4_t cbufx1_data = vmovn_s32(vcvtq_s32_f32(cbufx1_f));

		int16x4x2_t cbufx_data = vzip_s16(cbufx0_data,cbufx1_data);
		int16x8_t cbufx_data0 = vcombine_s16(cbufx_data.val[0],cbufx_data.val[1]);
		vst1q_s16(&cbufxi[w<<1],cbufx_data0);
	}    

	float cbufx0,cbufx1;
	for (; w < _dst_w; w++)
	{
			float fx = (float)((w + 0.5) * scale_x - 0.5);
			sx[w] = (int)fx;
			fx -= sx[w];

			sx[w] += _box_x;
			if (sx[w] < 0)
			{
					fx = 0; sx[w] = 0;
			}
			
			if (sx[w] >= _src_w - 1)
			{
					fx = 0; sx[w] = _src_w - 1;
			}

			cbufx0 = 1.f - fx;
			cbufx1 = fx;

			cbufxi[w<<1] = (short)(cbufx0 * 2048 + 0.5);
			cbufxi[(w<<1)+1] = (short)(cbufx1 * 2048 + 0.5);
	}

	float cbufy[2];
	float afy[4];
	int h = 0;
	for (; h+3 < _dst_h; h+=4)
	{
		afy[0] = (float)((h + 0.5) * scale_y - 0.5);
		afy[1] = (float)((h + 1.5) * scale_y - 0.5);
		afy[2] = (float)((h + 2.5) * scale_y - 0.5);
		afy[3] = (float)((h + 3.5) * scale_y - 0.5);

		float32x4_t afy_data = vld1q_f32(afy);
		int32x4_t sy_data = vcvtq_s32_f32(afy_data);
		float32x4_t sy_data_f = vcvtq_f32_s32(sy_data);

		afy_data = vsubq_f32(afy_data,sy_data_f);
		int32x4_t all_box_y = vdupq_n_s32(_box_y);
		sy_data = vaddq_s32(sy_data,all_box_y);
		
		int32x4_t all_zeros = vdupq_n_s32(0);
		uint32x4_t cond = vcltq_s32(sy_data,all_zeros);
		sy_data = vbslq_s32(cond,all_zeros,sy_data);
		float32x4_t all_zeros_f = vdupq_n_f32(0);
		afy_data = vbslq_f32(cond,all_zeros_f,afy_data);

		int32x4_t all_maxs = vdupq_n_s32((int)(_src_h - 1));
		cond = vcltq_s32(sy_data,all_maxs);
		sy_data = vbslq_s32(cond,sy_data,all_maxs);
		afy_data = vbslq_f32(cond,afy_data,all_zeros_f);

		int16x4_t sy_data_16 = vmovn_s32(sy_data);
		vst1_s16(&sy[h],sy_data_16);

		float32x4_t fixed_data = vdupq_n_f32(2048);
		float32x4_t fixed_data2 = vdupq_n_f32(0.5);
		float32x4_t cbufy1_f = vmlaq_f32(fixed_data2,afy_data,fixed_data);
		float32x4_t all_ones_f = vdupq_n_f32(1);
		float32x4_t cbufy0_f = vmlaq_f32(fixed_data2,vsubq_f32(all_ones_f,afy_data),fixed_data);
		int16x4_t cbufy0_data = vmovn_s32(vcvtq_s32_f32(cbufy0_f));
		int16x4_t cbufy1_data = vmovn_s32(vcvtq_s32_f32(cbufy1_f));

		int16x4x2_t cbufy_data = vzip_s16(cbufy0_data,cbufy1_data);
		int16x8_t cbufy_data0 = vcombine_s16(cbufy_data.val[0],cbufy_data.val[1]);
		vst1q_s16(&cbufyi[h<<1],cbufy_data0);                
	}
	
	for (; h < _dst_h; h++)
	{
			float fy = (float)((h + 0.5) * scale_y - 0.5);
			
			sy[h] = (int)fy;
			fy -= sy[h];
			
			sy[h] += _box_y;
			if(sy[h] < 0)
			{
					sy[h] = 0;
					fy = 0;
			}
			else if (sy[h] >= _src_h - 1)
			{
					sy[h] = _src_h - 1;
					fy = 0;
			}

			cbufy[0] = 1.f - fy;
			cbufy[1] = fy;
			
			cbufyi[h<<1] = (short)(cbufy[0] * 2048 + 0.5);
			cbufyi[(h<<1)+1] = (short)(cbufy[1] * 2048 + 0.5);
	} 
}

void face_resize_scale_yuv2rgb_chw(unsigned char* _src_yuv, int is_yuv_NV21, int _src_h, int _src_w, int box_x, int box_y,
		int _search_h, int _search_w, int _dst_h, int _dst_w, short* _dst)
{
	short sx[_dst_w];
	short cbufxi[_dst_w+1][2];//防止批处理读越界
	short sy[_dst_h];
	short cbufyi[_dst_h+1][2];//防止批处理读越界
	
	face_resize_scale_adjust(_src_h,_src_w,_search_h,_search_w,_dst_h,_dst_w,box_x,box_y,
			&sx[0],&cbufxi[0][0],&sy[0],&cbufyi[0][0]);

	int itur_bt_shift_v = (1<<(YUV_RGB_ITUR_BT_601_SHIFT - 1));
	
	unsigned char* _src_y = _src_yuv;
	unsigned char* _src_uv = _src_yuv + _src_h*_src_w;
	int h = 0;
	for (; h < _dst_h-RESIZE_REMAIN_BUF; h++)
	{
			unsigned char* y_src0 = _src_y + sy[h]*_src_w;
			unsigned char* y_src1 = _src_y + (sy[h]+1)*_src_w;
			unsigned char* uv_src0 = _src_uv + (sy[h]>>1)*_src_w;
			unsigned char* uv_src1 = _src_uv + ((sy[h]+1)>>1)*_src_w;
			int w = 0;
			short* cbufxi_ptr = &cbufxi[0][0];
			short* cbufyi_ptr = &cbufyi[h][0];
			short* rgb_dst0 = _dst + h*_dst_w;
			short* rgb_dst1 = rgb_dst0 + _dst_h*_dst_w;
			short* rgb_dst2 = rgb_dst1 + _dst_h*_dst_w;

			for(;w+1 < _dst_w; w+=2)
			{
					unsigned char* y_src00 = y_src0+sx[w];
					unsigned char* y_src01 = y_src0+sx[w+1];
					unsigned char* y_src10 = y_src1+sx[w];
					unsigned char* y_src11 = y_src1+sx[w+1];
					unsigned char* uv_src00 = uv_src0+sx[w];
					unsigned char* uv_src01 = uv_src0+sx[w+1];
					unsigned char* uv_src10 = uv_src1+sx[w];
					unsigned char* uv_src11 = uv_src1+sx[w+1];
					
					uint8x8_t y_data_temp,u_data_temp,v_data_temp;
					
					y_data_temp = vld1_lane_u8(y_src00,y_data_temp,0);
					y_data_temp = vld1_lane_u8(y_src00+1,y_data_temp,1);
					y_data_temp = vld1_lane_u8(y_src01,y_data_temp,2);
					y_data_temp = vld1_lane_u8(y_src01+1,y_data_temp,3);
					y_data_temp = vld1_lane_u8(y_src10,y_data_temp,4);
					y_data_temp = vld1_lane_u8(y_src10+1,y_data_temp,5);
					y_data_temp = vld1_lane_u8(y_src11,y_data_temp,6);
					y_data_temp = vld1_lane_u8(y_src11+1,y_data_temp,7);
				
					if(0 == (sx[w]&0x1))
					{
						if(is_yuv_NV21) {
						v_data_temp = vld1_lane_u8(uv_src00,v_data_temp,0);
						v_data_temp = vld1_lane_u8(uv_src00,v_data_temp,1);
						v_data_temp = vld1_lane_u8(uv_src10,v_data_temp,4);
						v_data_temp = vld1_lane_u8(uv_src10,v_data_temp,5);

						u_data_temp = vld1_lane_u8(uv_src00+1,u_data_temp,0);
						u_data_temp = vld1_lane_u8(uv_src00+1,u_data_temp,1);
						u_data_temp = vld1_lane_u8(uv_src10+1,u_data_temp,4);
						u_data_temp = vld1_lane_u8(uv_src10+1,u_data_temp,5);
						}
						else
						{
						u_data_temp = vld1_lane_u8(uv_src00,u_data_temp,0);
						u_data_temp = vld1_lane_u8(uv_src00,u_data_temp,1);
						u_data_temp = vld1_lane_u8(uv_src10,u_data_temp,4);
						u_data_temp = vld1_lane_u8(uv_src10,u_data_temp,5);

						v_data_temp = vld1_lane_u8(uv_src00+1,v_data_temp,0);
						v_data_temp = vld1_lane_u8(uv_src00+1,v_data_temp,1);
						v_data_temp = vld1_lane_u8(uv_src10+1,v_data_temp,4);
						v_data_temp = vld1_lane_u8(uv_src10+1,v_data_temp,5);
						}
					}
					else
					{
						if(is_yuv_NV21) {
						v_data_temp = vld1_lane_u8(uv_src00-1,v_data_temp,0);
						v_data_temp = vld1_lane_u8(uv_src00+1,v_data_temp,1);
						v_data_temp = vld1_lane_u8(uv_src10-1,v_data_temp,4);
						v_data_temp = vld1_lane_u8(uv_src10+1,v_data_temp,5);

						u_data_temp = vld1_lane_u8(uv_src00,u_data_temp,0);
						u_data_temp = vld1_lane_u8(uv_src00+2,u_data_temp,1);
						u_data_temp = vld1_lane_u8(uv_src10,u_data_temp,4);
						u_data_temp = vld1_lane_u8(uv_src10+2,u_data_temp,5);  
						}
						else
						{
						u_data_temp = vld1_lane_u8(uv_src00-1,u_data_temp,0);
						u_data_temp = vld1_lane_u8(uv_src00+1,u_data_temp,1);
						u_data_temp = vld1_lane_u8(uv_src10-1,u_data_temp,4);
						u_data_temp = vld1_lane_u8(uv_src10+1,u_data_temp,5);

						v_data_temp = vld1_lane_u8(uv_src00,v_data_temp,0);
						v_data_temp = vld1_lane_u8(uv_src00+2,v_data_temp,1);
						v_data_temp = vld1_lane_u8(uv_src10,v_data_temp,4);
						v_data_temp = vld1_lane_u8(uv_src10+2,v_data_temp,5);  
						}
					}

					if(0 == (sx[w+1]&0x1))
					{
						if(is_yuv_NV21) {
						v_data_temp = vld1_lane_u8(uv_src01,v_data_temp,2);
						v_data_temp = vld1_lane_u8(uv_src01,v_data_temp,3);
						v_data_temp = vld1_lane_u8(uv_src11,v_data_temp,6);
						v_data_temp = vld1_lane_u8(uv_src11,v_data_temp,7);

						u_data_temp = vld1_lane_u8(uv_src01+1,u_data_temp,2);
						u_data_temp = vld1_lane_u8(uv_src01+1,u_data_temp,3);
						u_data_temp = vld1_lane_u8(uv_src11+1,u_data_temp,6);
						u_data_temp = vld1_lane_u8(uv_src11+1,u_data_temp,7);
						}
						else
						{
						u_data_temp = vld1_lane_u8(uv_src01,u_data_temp,2);
						u_data_temp = vld1_lane_u8(uv_src01,u_data_temp,3);
						u_data_temp = vld1_lane_u8(uv_src11,u_data_temp,6);
						u_data_temp = vld1_lane_u8(uv_src11,u_data_temp,7);

						v_data_temp = vld1_lane_u8(uv_src01+1,v_data_temp,2);
						v_data_temp = vld1_lane_u8(uv_src01+1,v_data_temp,3);
						v_data_temp = vld1_lane_u8(uv_src11+1,v_data_temp,6);
						v_data_temp = vld1_lane_u8(uv_src11+1,v_data_temp,7);
						}
					}
					else
					{
						if(is_yuv_NV21) {
						v_data_temp = vld1_lane_u8(uv_src01-1,v_data_temp,2);
						v_data_temp = vld1_lane_u8(uv_src01+1,v_data_temp,3);
						v_data_temp = vld1_lane_u8(uv_src11-1,v_data_temp,6);
						v_data_temp = vld1_lane_u8(uv_src11+1,v_data_temp,7);

						u_data_temp = vld1_lane_u8(uv_src01,u_data_temp,2);
						u_data_temp = vld1_lane_u8(uv_src01+2,u_data_temp,3);
						u_data_temp = vld1_lane_u8(uv_src11,u_data_temp,6);
						u_data_temp = vld1_lane_u8(uv_src11+2,u_data_temp,7);
						}
						else
						{
						u_data_temp = vld1_lane_u8(uv_src01-1,u_data_temp,2);
						u_data_temp = vld1_lane_u8(uv_src01+1,u_data_temp,3);
						u_data_temp = vld1_lane_u8(uv_src11-1,u_data_temp,6);
						u_data_temp = vld1_lane_u8(uv_src11+1,u_data_temp,7);

						v_data_temp = vld1_lane_u8(uv_src01,v_data_temp,2);
						v_data_temp = vld1_lane_u8(uv_src01+2,v_data_temp,3);
						v_data_temp = vld1_lane_u8(uv_src11,v_data_temp,6);
						v_data_temp = vld1_lane_u8(uv_src11+2,v_data_temp,7); 
						}
					}

				//
				int32x4x2_t cys_data;
				int16x8_t y_min_val = vdupq_n_s16(16);
				int16x8_t temp_data = vreinterpretq_s16_u16(vmovl_u8(y_data_temp));
				temp_data = vsubq_s16(vmaxq_s16(temp_data,y_min_val),y_min_val);
				cys_data.val[0] = vmovl_s16(vget_low_s16(temp_data));
				cys_data.val[1] = vmovl_s16(vget_high_s16(temp_data));
				
				int32x4x2_t cus_data,cvs_data;
				int16x8_t uv_sub_val = vdupq_n_s16(128);
				temp_data = vreinterpretq_s16_u16(vmovl_u8(u_data_temp));
				temp_data = vsubq_s16(temp_data,uv_sub_val);
				cus_data.val[0] = vmovl_s16(vget_low_s16(temp_data));
				cus_data.val[1] = vmovl_s16(vget_high_s16(temp_data));
				
				temp_data = vreinterpretq_s16_u16(vmovl_u8(v_data_temp));
				temp_data = vsubq_s16(temp_data,uv_sub_val);
				cvs_data.val[0] = vmovl_s16(vget_low_s16(temp_data));
				cvs_data.val[1] = vmovl_s16(vget_high_s16(temp_data));

				int32x4_t neon_bt_601_cy = vdupq_n_s32(YUV_RGB_ITUR_BT_601_CY);
				int32x4_t neon_itur_bt_shift_v = vdupq_n_s32(itur_bt_shift_v);
				cys_data.val[0] = vmlaq_s32(neon_itur_bt_shift_v,cys_data.val[0],neon_bt_601_cy);
				cys_data.val[1] = vmlaq_s32(neon_itur_bt_shift_v,cys_data.val[1],neon_bt_601_cy);
				
				//calc R = 1220542*Y + 1673527*V
				int32x4_t neon_bt_601_cvr = vdupq_n_s32(YUV_RGB_ITUR_BT_601_CVR);
				int32x4_t vacc0 = vmlaq_s32(cys_data.val[0],cvs_data.val[0],neon_bt_601_cvr);
				int32x4_t vacc1 = vmlaq_s32(cys_data.val[1],cvs_data.val[1],neon_bt_601_cvr);
				int16x4_t r_data0 = vmovn_s32(vshrq_n_s32(vacc0,YUV_RGB_ITUR_BT_601_SHIFT));
				int16x4_t r_data1 = vmovn_s32(vshrq_n_s32(vacc1,YUV_RGB_ITUR_BT_601_SHIFT));         

				int16x4_t rbg_max_val = vdup_n_s16(255);
				int16x4_t rbg_min_val = vdup_n_s16(0);
				r_data0 = vmax_s16(r_data0,rbg_min_val);
				r_data0 = vmin_s16(r_data0,rbg_max_val);
				r_data1 = vmax_s16(r_data1,rbg_min_val);
				r_data1 = vmin_s16(r_data1,rbg_max_val);

				//calc G = 1220542*Y - 409993*U - 852492*V
				int32x4_t neon_bt_601_cug = vdupq_n_s32(YUV_RGB_ITUR_BT_601_CUG);
				int32x4_t neon_bt_601_cvg = vdupq_n_s32(YUV_RGB_ITUR_BT_601_CVG);
				vacc0 = vmlaq_s32(cys_data.val[0],cus_data.val[0],neon_bt_601_cug);
				vacc0 = vmlaq_s32(vacc0,cvs_data.val[0],neon_bt_601_cvg);
				vacc1 = vmlaq_s32(cys_data.val[1],cus_data.val[1],neon_bt_601_cug);
				vacc1 = vmlaq_s32(vacc1,cvs_data.val[1],neon_bt_601_cvg);
				int16x4_t g_data0 = vmovn_s32(vshrq_n_s32(vacc0,YUV_RGB_ITUR_BT_601_SHIFT));
				int16x4_t g_data1 = vmovn_s32(vshrq_n_s32(vacc1,YUV_RGB_ITUR_BT_601_SHIFT));
								
				g_data0 = vmax_s16(g_data0,rbg_min_val);
				g_data0 = vmin_s16(g_data0,rbg_max_val);
				g_data1 = vmax_s16(g_data1,rbg_min_val);
				g_data1 = vmin_s16(g_data1,rbg_max_val);
				
				//calc B = 1220542*Y + 2116026*U
				int32x4_t neon_bt_601_cub = vdupq_n_s32(YUV_RGB_ITUR_BT_601_CUB);
				vacc0 = vmlaq_s32(cys_data.val[0],cus_data.val[0],neon_bt_601_cub);
				vacc1 = vmlaq_s32(cys_data.val[1],cus_data.val[1],neon_bt_601_cub);
				int16x4_t b_data0 = vmovn_s32(vshrq_n_s32(vacc0,YUV_RGB_ITUR_BT_601_SHIFT));
				int16x4_t b_data1 = vmovn_s32(vshrq_n_s32(vacc1,YUV_RGB_ITUR_BT_601_SHIFT)); 

				b_data0 = vmax_s16(b_data0,rbg_min_val);
				b_data0 = vmin_s16(b_data0,rbg_max_val);
				b_data1 = vmax_s16(b_data1,rbg_min_val);
				b_data1 = vmin_s16(b_data1,rbg_max_val);
				
				int16x4_t vcbufxi = vld1_s16(cbufxi_ptr);
				//r filter
				int32x4_t r_ss0 = vmull_s16(r_data0,vcbufxi);
				int32x4_t r_ss1 = vmull_s16(r_data1,vcbufxi);
				int32x2_t r_sss0 = vpadd_s32(vget_low_s32(r_ss0),vget_high_s32(r_ss0));
				int32x2_t r_sss1 = vpadd_s32(vget_low_s32(r_ss1),vget_high_s32(r_ss1));
				
				r_sss0 = vshr_n_s32(r_sss0,4);
				r_sss1 = vshr_n_s32(r_sss1,4);
				r_sss0 = vmul_n_s32(r_sss0,(int)cbufyi_ptr[0]);
				r_sss1 = vmul_n_s32(r_sss1,(int)cbufyi_ptr[1]);
				r_sss0 = vshr_n_s32(r_sss0,16);
				r_sss1 = vshr_n_s32(r_sss1,16);
				r_sss0 = vadd_s32(r_sss0,r_sss1);
				//r_sss0 = vshr_n_s32(r_sss0,16);
				int32x2_t vround_add = vdup_n_s32(2);
				r_sss0 = vadd_s32(r_sss0,vround_add);
				r_sss0 = vshr_n_s32(r_sss0,2);
				
				//
				int32x2_t  all_zeros_s32 = vdup_n_s32(0);
				int32x2_t  all_maxval_s32 = vdup_n_s32(255);
				int32x2_t  all_mulval_s32 = vdup_n_s32(2);
				int32x2_t  all_addval_s32 = vdup_n_s32(-255);
				r_sss0 = vmax_s32(r_sss0,all_zeros_s32);
				r_sss0 = vmin_s32(r_sss0,all_maxval_s32);
				r_sss0 = vmla_s32(all_addval_s32,r_sss0,all_mulval_s32);
				
				rgb_dst0[0] = (short)vget_lane_s32(r_sss0, 0);
				rgb_dst0[1] = (short)vget_lane_s32(r_sss0, 1);
				
		//g filter
				int32x4_t g_ss0 = vmull_s16(g_data0,vcbufxi);
				int32x4_t g_ss1 = vmull_s16(g_data1,vcbufxi);
				int32x2_t g_sss0 = vpadd_s32(vget_low_s32(g_ss0),vget_high_s32(g_ss0));
				int32x2_t g_sss1 = vpadd_s32(vget_low_s32(g_ss1),vget_high_s32(g_ss1));
				
				g_sss0 = vshr_n_s32(g_sss0,4);
				g_sss1 = vshr_n_s32(g_sss1,4);
				g_sss0 = vmul_n_s32(g_sss0,(int)cbufyi_ptr[0]);
				g_sss1 = vmul_n_s32(g_sss1,(int)cbufyi_ptr[1]);
				g_sss0 = vshr_n_s32(g_sss0,16);
				g_sss1 = vshr_n_s32(g_sss1,16);
				g_sss0 = vadd_s32(g_sss0,g_sss1);
				//g_sss0 = vshr_n_s32(g_sss0,16);
				vround_add = vdup_n_s32(2);
				g_sss0 = vadd_s32(g_sss0,vround_add);
				g_sss0 = vshr_n_s32(g_sss0,2);
				
				//
				g_sss0 = vmax_s32(g_sss0,all_zeros_s32);
				g_sss0 = vmin_s32(g_sss0,all_maxval_s32);
				g_sss0 = vmla_s32(all_addval_s32,g_sss0,all_mulval_s32);
				
				rgb_dst1[0] = (short)vget_lane_s32(g_sss0, 0);
				rgb_dst1[1] = (short)vget_lane_s32(g_sss0, 1);
				
				//b filter
				int32x4_t b_ss0 = vmull_s16(b_data0,vcbufxi);
				int32x4_t b_ss1 = vmull_s16(b_data1,vcbufxi);
				int32x2_t b_sss0 = vpadd_s32(vget_low_s32(b_ss0),vget_high_s32(b_ss0));
				int32x2_t b_sss1 = vpadd_s32(vget_low_s32(b_ss1),vget_high_s32(b_ss1));
				
				b_sss0 = vshr_n_s32(b_sss0,4);
				b_sss1 = vshr_n_s32(b_sss1,4);
				b_sss0 = vmul_n_s32(b_sss0,(int)cbufyi_ptr[0]);
				b_sss1 = vmul_n_s32(b_sss1,(int)cbufyi_ptr[1]);
				b_sss0 = vshr_n_s32(b_sss0,16);
				b_sss1 = vshr_n_s32(b_sss1,16);
				b_sss0 = vadd_s32(b_sss0,b_sss1);
				//b_sss0 = vshr_n_s32(b_sss0,16);
				vround_add = vdup_n_s32(2);
				b_sss0 = vadd_s32(b_sss0,vround_add);
				b_sss0 = vshr_n_s32(b_sss0,2);
				
				//
				b_sss0 = vmax_s32(b_sss0,all_zeros_s32);
				b_sss0 = vmin_s32(b_sss0,all_maxval_s32);
				b_sss0 = vmla_s32(all_addval_s32,b_sss0,all_mulval_s32);
				
				rgb_dst2[0] = (short)vget_lane_s32(b_sss0, 0);
				rgb_dst2[1] = (short)vget_lane_s32(b_sss0, 1);

				
			cbufxi_ptr += 4;
			rgb_dst0+=2;
			rgb_dst1+=2;
			rgb_dst2+=2;
		}
		
		for(;w < _dst_w; w++)
		{
			int data_Y[4],data_U[4],data_V[4];
			data_Y[0] = (int)*(y_src0+sx[w]);
			data_Y[1] = (int)*(y_src0+sx[w]+1);
			data_Y[2] = (int)*(y_src1+sx[w]);
			data_Y[3] = (int)*(y_src1+sx[w]+1);
			
			if(0 == (sx[w]&0x1))
			{
				if(is_yuv_NV21) {
				data_V[0] = (int)*(uv_src0+sx[w]);
				data_V[1] = data_V[0];
				data_V[2] = (int)*(uv_src1+sx[w]);
				data_V[3] = data_V[2];

				data_U[0] = (int)*(uv_src0+sx[w]+1);
				data_U[1] = data_U[0];
				data_U[2] = (int)*(uv_src1+sx[w]+1);
				data_U[3] = data_U[2]; 
				}
				else
				{
				data_U[0] = (int)*(uv_src0+sx[w]);
				data_U[1] = data_U[0];
				data_U[2] = (int)*(uv_src1+sx[w]);
				data_U[3] = data_U[2];

				data_V[0] = (int)*(uv_src0+sx[w]+1);
				data_V[1] = data_V[0];
				data_V[2] = (int)*(uv_src1+sx[w]+1);
				data_V[3] = data_V[2]; 
				}
			}
			else
			{
				if(is_yuv_NV21) {
				data_V[0] = (int)*(uv_src0+sx[w]-1);
				data_V[1] = (int)*(uv_src0+sx[w]+1);
				data_V[2] = (int)*(uv_src1+sx[w]-1);
				data_V[3] = (int)*(uv_src1+sx[w]+1);

				data_U[0] = (int)*(uv_src0+sx[w]);
				data_U[1] = (int)*(uv_src0+sx[w]+2);
				data_U[2] = (int)*(uv_src1+sx[w]);
				data_U[3] = (int)*(uv_src1+sx[w]+2); 
				}
				else
				{
				data_U[0] = (int)*(uv_src0+sx[w]-1);
				data_U[1] = (int)*(uv_src0+sx[w]+1);
				data_U[2] = (int)*(uv_src1+sx[w]-1);
				data_U[3] = (int)*(uv_src1+sx[w]+1);

				data_V[0] = (int)*(uv_src0+sx[w]);
				data_V[1] = (int)*(uv_src0+sx[w]+2);
				data_V[2] = (int)*(uv_src1+sx[w]);
				data_V[3] = (int)*(uv_src1+sx[w]+2); 
				}                    
			}
			
			int32x4_t scr_Y,scr_U,scr_V;
			scr_Y = vld1q_s32(data_Y);
			scr_U = vld1q_s32(data_U);
			scr_V = vld1q_s32(data_V);
			
			int32x4_t y_min_val = vdupq_n_s32(16);
			scr_Y = vsubq_s32(vmaxq_s32(scr_Y,y_min_val),y_min_val);
			int32x4_t neon_bt_601_cy = vdupq_n_s32(YUV_RGB_ITUR_BT_601_CY);
			int32x4_t neon_itur_bt_shift_v = vdupq_n_s32(itur_bt_shift_v);
			scr_Y = vmlaq_s32(neon_itur_bt_shift_v,scr_Y,neon_bt_601_cy);
			
			int32x4_t uv_sub_val = vdupq_n_s32(128);
			scr_U = vsubq_s32(scr_U,uv_sub_val);
			scr_V = vsubq_s32(scr_V,uv_sub_val);
			
			//calc R = 1220542*Y + 1673527*V
			int32x4_t neon_bt_601_cvr = vdupq_n_s32(YUV_RGB_ITUR_BT_601_CVR);
			int32x4_t r_acc = vmlaq_s32(scr_Y,scr_V,neon_bt_601_cvr);
			int16x4_t r_data = vmovn_s32(vshrq_n_s32(r_acc,YUV_RGB_ITUR_BT_601_SHIFT));
			
			int16x4_t rbg_max_val = vdup_n_s16(255);
			int16x4_t rbg_min_val = vdup_n_s16(0);
			r_data = vmax_s16(r_data,rbg_min_val);
			r_data = vmin_s16(r_data,rbg_max_val);
				
			//calc G = 1220542*Y - 409993*U - 852492*V
			int32x4_t neon_bt_601_cug = vdupq_n_s32(YUV_RGB_ITUR_BT_601_CUG);
			int32x4_t neon_bt_601_cvg = vdupq_n_s32(YUV_RGB_ITUR_BT_601_CVG);
			int32x4_t g_acc = vmlaq_s32(scr_Y,scr_U,neon_bt_601_cug);
			g_acc = vmlaq_s32(g_acc,scr_V,neon_bt_601_cvg);
			int16x4_t g_data = vmovn_s32(vshrq_n_s32(g_acc,YUV_RGB_ITUR_BT_601_SHIFT));
			
			rbg_max_val = vdup_n_s16(255);
			rbg_min_val = vdup_n_s16(0);
			g_data = vmax_s16(g_data,rbg_min_val);
			g_data = vmin_s16(g_data,rbg_max_val);
			
			//calc B = 1220542*Y + 2116026*U
			int32x4_t neon_bt_601_cub = vdupq_n_s32(YUV_RGB_ITUR_BT_601_CUB);
			int32x4_t b_acc = vmlaq_s32(scr_Y,scr_U,neon_bt_601_cub);
			int16x4_t b_data = vmovn_s32(vshrq_n_s32(b_acc,YUV_RGB_ITUR_BT_601_SHIFT));
			
			rbg_max_val = vdup_n_s16(255);
			rbg_min_val = vdup_n_s16(0);
			b_data = vmax_s16(b_data,rbg_min_val);
			b_data = vmin_s16(b_data,rbg_max_val);
			
			int32x2_t vcbufxi = vreinterpret_s32_s16(vld1_s16(cbufxi_ptr));
			int32x2x2_t fxi_temp = vzip_s32(vcbufxi,vcbufxi);
			int16x4_t vcbufxi00 = vreinterpret_s16_s32(fxi_temp.val[0]);
			int16x4_t fyi_temp = vld1_s16(cbufyi_ptr);
			int32x2_t vcbufyi = vget_low_s32(vmovl_s16(fyi_temp)); 
			
			//r fliter
			int32x4_t r_ss0 = vmull_s16(r_data,vcbufxi00);
			int32x2_t r_sss0 = vpadd_s32(vget_low_s32(r_ss0),vget_high_s32(r_ss0));
			r_sss0 = vshr_n_s32(r_sss0,4);
			r_sss0 = vmul_s32(r_sss0,vcbufyi);
			r_sss0 = vshr_n_s32(r_sss0,16);
			int32x2_t r_sss1 = vpadd_s32(r_sss0,r_sss0);
			int32x2_t vround_add = vdup_n_s32(2);
			r_sss1 = vadd_s32(r_sss1,vround_add);
			r_sss1 = vshr_n_s32(r_sss1,2);
			
			//
			int32x2_t  all_zeros_s32 = vdup_n_s32(0);
			int32x2_t  all_maxval_s32 = vdup_n_s32(255);
			int32x2_t  all_mulval_s32 = vdup_n_s32(2);
			int32x2_t  all_addval_s32 = vdup_n_s32(-255);
			r_sss1 = vmax_s32(r_sss1,all_zeros_s32);
			r_sss1 = vmin_s32(r_sss1,all_maxval_s32);
			r_sss1 = vmla_s32(all_addval_s32,r_sss1,all_mulval_s32);
			
			*rgb_dst0 = (short)vget_lane_s32(r_sss1, 0);
			
			//g fliter
			int32x4_t g_ss0 = vmull_s16(g_data,vcbufxi00);
			int32x2_t g_sss0 = vpadd_s32(vget_low_s32(g_ss0),vget_high_s32(g_ss0));
			g_sss0 = vshr_n_s32(g_sss0,4);
			g_sss0 = vmul_s32(g_sss0,vcbufyi);
			g_sss0 = vshr_n_s32(g_sss0,16);
			int32x2_t g_sss1 = vpadd_s32(g_sss0,g_sss0);
			vround_add = vdup_n_s32(2);
			g_sss1 = vadd_s32(g_sss1,vround_add);
			g_sss1 = vshr_n_s32(g_sss1,2);
			
			//
			g_sss1 = vmax_s32(g_sss1,all_zeros_s32);
			g_sss1 = vmin_s32(g_sss1,all_maxval_s32);
			g_sss1 = vmla_s32(all_addval_s32,g_sss1,all_mulval_s32);
			
			*rgb_dst1 = (short)vget_lane_s32(g_sss1, 0);
			
			//b fliter
			int32x4_t b_ss0 = vmull_s16(b_data,vcbufxi00);
			int32x2_t b_sss0 = vpadd_s32(vget_low_s32(b_ss0),vget_high_s32(b_ss0));
			b_sss0 = vshr_n_s32(b_sss0,4);
			b_sss0 = vmul_s32(b_sss0,vcbufyi);
			b_sss0 = vshr_n_s32(b_sss0,16);
			int32x2_t b_sss1 = vpadd_s32(b_sss0,b_sss0);
			vround_add = vdup_n_s32(2);
			b_sss1 = vadd_s32(b_sss1,vround_add);
			b_sss1 = vshr_n_s32(b_sss1,2);
			
			//
			b_sss1 = vmax_s32(b_sss1,all_zeros_s32);
			b_sss1 = vmin_s32(b_sss1,all_maxval_s32);
			b_sss1 = vmla_s32(all_addval_s32,b_sss1,all_mulval_s32);
			
			*rgb_dst2 = (short)vget_lane_s32(b_sss1, 0);
			
			cbufxi_ptr += 2;
			rgb_dst0++;
			rgb_dst1++;
			rgb_dst2++;
		}
	}    
	//处理RESIZE_REMAIN_BUF�?
	for (; h < _dst_h; h++)
	{
		unsigned char *y_src0,*y_src1,*uv_src0,*uv_src1;
		if(sy[h] == _src_h-1)
		{
			y_src0 = _src_y + sy[h]*_src_w;
			y_src1 = y_src0;
			uv_src0 = _src_uv + (sy[h]>>1)*_src_w;
			uv_src1 = uv_src0;
		}
		else
		{
			y_src0 = _src_y + sy[h]*_src_w;
			y_src1 = _src_y + (sy[h]+1)*_src_w;
			uv_src0 = _src_uv + (sy[h]>>1)*_src_w;
			uv_src1 = _src_uv + ((sy[h]+1)>>1)*_src_w;
		}
			int w = 0;
			short* cbufxi_ptr = &cbufxi[0][0];
			short* cbufyi_ptr = &cbufyi[h][0];
			short* rgb_dst0 = _dst + h*_dst_w;
			short* rgb_dst1 = rgb_dst0 + _dst_h*_dst_w;
			short* rgb_dst2 = rgb_dst1 + _dst_h*_dst_w;

			for(;w+1 < _dst_w-RESIZE_REMAIN_BUF; w+=2)
			{
					unsigned char* y_src00 = y_src0+sx[w];
					unsigned char* y_src01 = y_src0+sx[w+1];
					unsigned char* y_src10 = y_src1+sx[w];
					unsigned char* y_src11 = y_src1+sx[w+1];
					unsigned char* uv_src00 = uv_src0+sx[w];
					unsigned char* uv_src01 = uv_src0+sx[w+1];
					unsigned char* uv_src10 = uv_src1+sx[w];
					unsigned char* uv_src11 = uv_src1+sx[w+1];
					
					uint8x8_t y_data_temp,u_data_temp,v_data_temp;
					
					y_data_temp = vld1_lane_u8(y_src00,y_data_temp,0);
					y_data_temp = vld1_lane_u8(y_src00+1,y_data_temp,1);
					y_data_temp = vld1_lane_u8(y_src01,y_data_temp,2);
					y_data_temp = vld1_lane_u8(y_src01+1,y_data_temp,3);
					y_data_temp = vld1_lane_u8(y_src10,y_data_temp,4);
					y_data_temp = vld1_lane_u8(y_src10+1,y_data_temp,5);
					y_data_temp = vld1_lane_u8(y_src11,y_data_temp,6);
					y_data_temp = vld1_lane_u8(y_src11+1,y_data_temp,7);
				
					if(0 == (sx[w]&0x1))
					{
						if(is_yuv_NV21) {
						v_data_temp = vld1_lane_u8(uv_src00,v_data_temp,0);
						v_data_temp = vld1_lane_u8(uv_src00,v_data_temp,1);
						v_data_temp = vld1_lane_u8(uv_src10,v_data_temp,4);
						v_data_temp = vld1_lane_u8(uv_src10,v_data_temp,5);

						u_data_temp = vld1_lane_u8(uv_src00+1,u_data_temp,0);
						u_data_temp = vld1_lane_u8(uv_src00+1,u_data_temp,1);
						u_data_temp = vld1_lane_u8(uv_src10+1,u_data_temp,4);
						u_data_temp = vld1_lane_u8(uv_src10+1,u_data_temp,5);
						}
						else
						{
						u_data_temp = vld1_lane_u8(uv_src00,u_data_temp,0);
						u_data_temp = vld1_lane_u8(uv_src00,u_data_temp,1);
						u_data_temp = vld1_lane_u8(uv_src10,u_data_temp,4);
						u_data_temp = vld1_lane_u8(uv_src10,u_data_temp,5);

						v_data_temp = vld1_lane_u8(uv_src00+1,v_data_temp,0);
						v_data_temp = vld1_lane_u8(uv_src00+1,v_data_temp,1);
						v_data_temp = vld1_lane_u8(uv_src10+1,v_data_temp,4);
						v_data_temp = vld1_lane_u8(uv_src10+1,v_data_temp,5);
						}
					}
					else
					{
						if(is_yuv_NV21) {
						v_data_temp = vld1_lane_u8(uv_src00-1,v_data_temp,0);
						v_data_temp = vld1_lane_u8(uv_src00+1,v_data_temp,1);
						v_data_temp = vld1_lane_u8(uv_src10-1,v_data_temp,4);
						v_data_temp = vld1_lane_u8(uv_src10+1,v_data_temp,5);

						u_data_temp = vld1_lane_u8(uv_src00,u_data_temp,0);
						u_data_temp = vld1_lane_u8(uv_src00+2,u_data_temp,1);
						u_data_temp = vld1_lane_u8(uv_src10,u_data_temp,4);
						u_data_temp = vld1_lane_u8(uv_src10+2,u_data_temp,5); 
						}
						else
						{
						u_data_temp = vld1_lane_u8(uv_src00-1,u_data_temp,0);
						u_data_temp = vld1_lane_u8(uv_src00+1,u_data_temp,1);
						u_data_temp = vld1_lane_u8(uv_src10-1,u_data_temp,4);
						u_data_temp = vld1_lane_u8(uv_src10+1,u_data_temp,5);

						v_data_temp = vld1_lane_u8(uv_src00,v_data_temp,0);
						v_data_temp = vld1_lane_u8(uv_src00+2,v_data_temp,1);
						v_data_temp = vld1_lane_u8(uv_src10,v_data_temp,4);
						v_data_temp = vld1_lane_u8(uv_src10+2,v_data_temp,5);  
						}
					}

					if(0 == (sx[w+1]&0x1))
					{
						if(is_yuv_NV21) {
						v_data_temp = vld1_lane_u8(uv_src01,v_data_temp,2);
						v_data_temp = vld1_lane_u8(uv_src01,v_data_temp,3);
						v_data_temp = vld1_lane_u8(uv_src11,v_data_temp,6);
						v_data_temp = vld1_lane_u8(uv_src11,v_data_temp,7);

						u_data_temp = vld1_lane_u8(uv_src01+1,u_data_temp,2);
						u_data_temp = vld1_lane_u8(uv_src01+1,u_data_temp,3);
						u_data_temp = vld1_lane_u8(uv_src11+1,u_data_temp,6);
						u_data_temp = vld1_lane_u8(uv_src11+1,u_data_temp,7);
						}
						else
						{
						u_data_temp = vld1_lane_u8(uv_src01,u_data_temp,2);
						u_data_temp = vld1_lane_u8(uv_src01,u_data_temp,3);
						u_data_temp = vld1_lane_u8(uv_src11,u_data_temp,6);
						u_data_temp = vld1_lane_u8(uv_src11,u_data_temp,7);

						v_data_temp = vld1_lane_u8(uv_src01+1,v_data_temp,2);
						v_data_temp = vld1_lane_u8(uv_src01+1,v_data_temp,3);
						v_data_temp = vld1_lane_u8(uv_src11+1,v_data_temp,6);
						v_data_temp = vld1_lane_u8(uv_src11+1,v_data_temp,7);
						}
					}
					else
					{
						if(is_yuv_NV21) {
						v_data_temp = vld1_lane_u8(uv_src01-1,v_data_temp,2);
						v_data_temp = vld1_lane_u8(uv_src01+1,v_data_temp,3);
						v_data_temp = vld1_lane_u8(uv_src11-1,v_data_temp,6);
						v_data_temp = vld1_lane_u8(uv_src11+1,v_data_temp,7);

						u_data_temp = vld1_lane_u8(uv_src01,u_data_temp,2);
						u_data_temp = vld1_lane_u8(uv_src01+2,u_data_temp,3);
						u_data_temp = vld1_lane_u8(uv_src11,u_data_temp,6);
						u_data_temp = vld1_lane_u8(uv_src11+2,u_data_temp,7);
						}
						else
						{
						u_data_temp = vld1_lane_u8(uv_src01-1,u_data_temp,2);
						u_data_temp = vld1_lane_u8(uv_src01+1,u_data_temp,3);
						u_data_temp = vld1_lane_u8(uv_src11-1,u_data_temp,6);
						u_data_temp = vld1_lane_u8(uv_src11+1,u_data_temp,7);

						v_data_temp = vld1_lane_u8(uv_src01,v_data_temp,2);
						v_data_temp = vld1_lane_u8(uv_src01+2,v_data_temp,3);
						v_data_temp = vld1_lane_u8(uv_src11,v_data_temp,6);
						v_data_temp = vld1_lane_u8(uv_src11+2,v_data_temp,7); 
						}
					}

				//
				int32x4x2_t cys_data;
				int16x8_t y_min_val = vdupq_n_s16(16);
				int16x8_t temp_data = vreinterpretq_s16_u16(vmovl_u8(y_data_temp));
				temp_data = vsubq_s16(vmaxq_s16(temp_data,y_min_val),y_min_val);
				cys_data.val[0] = vmovl_s16(vget_low_s16(temp_data));
				cys_data.val[1] = vmovl_s16(vget_high_s16(temp_data));
				
				int32x4x2_t cus_data,cvs_data;
				int16x8_t uv_sub_val = vdupq_n_s16(128);
				temp_data = vreinterpretq_s16_u16(vmovl_u8(u_data_temp));
				temp_data = vsubq_s16(temp_data,uv_sub_val);
				cus_data.val[0] = vmovl_s16(vget_low_s16(temp_data));
				cus_data.val[1] = vmovl_s16(vget_high_s16(temp_data));
				
				temp_data = vreinterpretq_s16_u16(vmovl_u8(v_data_temp));
				temp_data = vsubq_s16(temp_data,uv_sub_val);
				cvs_data.val[0] = vmovl_s16(vget_low_s16(temp_data));
				cvs_data.val[1] = vmovl_s16(vget_high_s16(temp_data));

				int32x4_t neon_bt_601_cy = vdupq_n_s32(YUV_RGB_ITUR_BT_601_CY);
				int32x4_t neon_itur_bt_shift_v = vdupq_n_s32(itur_bt_shift_v);
				cys_data.val[0] = vmlaq_s32(neon_itur_bt_shift_v,cys_data.val[0],neon_bt_601_cy);
				cys_data.val[1] = vmlaq_s32(neon_itur_bt_shift_v,cys_data.val[1],neon_bt_601_cy);
				
				//calc R = 1220542*Y + 1673527*V
				int32x4_t neon_bt_601_cvr = vdupq_n_s32(YUV_RGB_ITUR_BT_601_CVR);
				int32x4_t vacc0 = vmlaq_s32(cys_data.val[0],cvs_data.val[0],neon_bt_601_cvr);
				int32x4_t vacc1 = vmlaq_s32(cys_data.val[1],cvs_data.val[1],neon_bt_601_cvr);
				int16x4_t r_data0 = vmovn_s32(vshrq_n_s32(vacc0,YUV_RGB_ITUR_BT_601_SHIFT));
				int16x4_t r_data1 = vmovn_s32(vshrq_n_s32(vacc1,YUV_RGB_ITUR_BT_601_SHIFT));         

				int16x4_t rbg_max_val = vdup_n_s16(255);
				int16x4_t rbg_min_val = vdup_n_s16(0);
				r_data0 = vmax_s16(r_data0,rbg_min_val);
				r_data0 = vmin_s16(r_data0,rbg_max_val);
				r_data1 = vmax_s16(r_data1,rbg_min_val);
				r_data1 = vmin_s16(r_data1,rbg_max_val);

				//calc G = 1220542*Y - 409993*U - 852492*V
				int32x4_t neon_bt_601_cug = vdupq_n_s32(YUV_RGB_ITUR_BT_601_CUG);
				int32x4_t neon_bt_601_cvg = vdupq_n_s32(YUV_RGB_ITUR_BT_601_CVG);
				vacc0 = vmlaq_s32(cys_data.val[0],cus_data.val[0],neon_bt_601_cug);
				vacc0 = vmlaq_s32(vacc0,cvs_data.val[0],neon_bt_601_cvg);
				vacc1 = vmlaq_s32(cys_data.val[1],cus_data.val[1],neon_bt_601_cug);
				vacc1 = vmlaq_s32(vacc1,cvs_data.val[1],neon_bt_601_cvg);
				int16x4_t g_data0 = vmovn_s32(vshrq_n_s32(vacc0,YUV_RGB_ITUR_BT_601_SHIFT));
				int16x4_t g_data1 = vmovn_s32(vshrq_n_s32(vacc1,YUV_RGB_ITUR_BT_601_SHIFT));
								
				g_data0 = vmax_s16(g_data0,rbg_min_val);
				g_data0 = vmin_s16(g_data0,rbg_max_val);
				g_data1 = vmax_s16(g_data1,rbg_min_val);
				g_data1 = vmin_s16(g_data1,rbg_max_val);
				
				//calc B = 1220542*Y + 2116026*U
				int32x4_t neon_bt_601_cub = vdupq_n_s32(YUV_RGB_ITUR_BT_601_CUB);
				vacc0 = vmlaq_s32(cys_data.val[0],cus_data.val[0],neon_bt_601_cub);
				vacc1 = vmlaq_s32(cys_data.val[1],cus_data.val[1],neon_bt_601_cub);
				int16x4_t b_data0 = vmovn_s32(vshrq_n_s32(vacc0,YUV_RGB_ITUR_BT_601_SHIFT));
				int16x4_t b_data1 = vmovn_s32(vshrq_n_s32(vacc1,YUV_RGB_ITUR_BT_601_SHIFT)); 

				b_data0 = vmax_s16(b_data0,rbg_min_val);
				b_data0 = vmin_s16(b_data0,rbg_max_val);
				b_data1 = vmax_s16(b_data1,rbg_min_val);
				b_data1 = vmin_s16(b_data1,rbg_max_val);
				
				int16x4_t vcbufxi = vld1_s16(cbufxi_ptr);
				//r filter
				int32x4_t r_ss0 = vmull_s16(r_data0,vcbufxi);
				int32x4_t r_ss1 = vmull_s16(r_data1,vcbufxi);
				int32x2_t r_sss0 = vpadd_s32(vget_low_s32(r_ss0),vget_high_s32(r_ss0));
				int32x2_t r_sss1 = vpadd_s32(vget_low_s32(r_ss1),vget_high_s32(r_ss1));
				
				r_sss0 = vshr_n_s32(r_sss0,4);
				r_sss1 = vshr_n_s32(r_sss1,4);
				r_sss0 = vmul_n_s32(r_sss0,(int)cbufyi_ptr[0]);
				r_sss1 = vmul_n_s32(r_sss1,(int)cbufyi_ptr[1]);
				r_sss0 = vshr_n_s32(r_sss0,16);
				r_sss1 = vshr_n_s32(r_sss1,16);
				r_sss0 = vadd_s32(r_sss0,r_sss1);
				//r_sss0 = vshr_n_s32(r_sss0,16);
				int32x2_t vround_add = vdup_n_s32(2);
				r_sss0 = vadd_s32(r_sss0,vround_add);
				r_sss0 = vshr_n_s32(r_sss0,2);
				
				//
				int32x2_t  all_zeros_s32 = vdup_n_s32(0);
				int32x2_t  all_maxval_s32 = vdup_n_s32(255);
				int32x2_t  all_mulval_s32 = vdup_n_s32(2);
				int32x2_t  all_addval_s32 = vdup_n_s32(-255);
				r_sss0 = vmax_s32(r_sss0,all_zeros_s32);
				r_sss0 = vmin_s32(r_sss0,all_maxval_s32);
				r_sss0 = vmla_s32(all_addval_s32,r_sss0,all_mulval_s32);
				
				rgb_dst0[0] = (short)vget_lane_s32(r_sss0, 0);
				rgb_dst0[1] = (short)vget_lane_s32(r_sss0, 1);
				
		//g filter
				int32x4_t g_ss0 = vmull_s16(g_data0,vcbufxi);
				int32x4_t g_ss1 = vmull_s16(g_data1,vcbufxi);
				int32x2_t g_sss0 = vpadd_s32(vget_low_s32(g_ss0),vget_high_s32(g_ss0));
				int32x2_t g_sss1 = vpadd_s32(vget_low_s32(g_ss1),vget_high_s32(g_ss1));
				
				g_sss0 = vshr_n_s32(g_sss0,4);
				g_sss1 = vshr_n_s32(g_sss1,4);
				g_sss0 = vmul_n_s32(g_sss0,(int)cbufyi_ptr[0]);
				g_sss1 = vmul_n_s32(g_sss1,(int)cbufyi_ptr[1]);
				g_sss0 = vshr_n_s32(g_sss0,16);
				g_sss1 = vshr_n_s32(g_sss1,16);
				g_sss0 = vadd_s32(g_sss0,g_sss1);
				//g_sss0 = vshr_n_s32(g_sss0,16);
				vround_add = vdup_n_s32(2);
				g_sss0 = vadd_s32(g_sss0,vround_add);
				g_sss0 = vshr_n_s32(g_sss0,2);
				
				//
				g_sss0 = vmax_s32(g_sss0,all_zeros_s32);
				g_sss0 = vmin_s32(g_sss0,all_maxval_s32);
				g_sss0 = vmla_s32(all_addval_s32,g_sss0,all_mulval_s32);
				
				rgb_dst1[0] = (short)vget_lane_s32(g_sss0, 0);
				rgb_dst1[1] = (short)vget_lane_s32(g_sss0, 1);
				
				//b filter
				int32x4_t b_ss0 = vmull_s16(b_data0,vcbufxi);
				int32x4_t b_ss1 = vmull_s16(b_data1,vcbufxi);
				int32x2_t b_sss0 = vpadd_s32(vget_low_s32(b_ss0),vget_high_s32(b_ss0));
				int32x2_t b_sss1 = vpadd_s32(vget_low_s32(b_ss1),vget_high_s32(b_ss1));
				
				b_sss0 = vshr_n_s32(b_sss0,4);
				b_sss1 = vshr_n_s32(b_sss1,4);
				b_sss0 = vmul_n_s32(b_sss0,(int)cbufyi_ptr[0]);
				b_sss1 = vmul_n_s32(b_sss1,(int)cbufyi_ptr[1]);
				b_sss0 = vshr_n_s32(b_sss0,16);
				b_sss1 = vshr_n_s32(b_sss1,16);
				b_sss0 = vadd_s32(b_sss0,b_sss1);
				//b_sss0 = vshr_n_s32(b_sss0,16);
				vround_add = vdup_n_s32(2);
				b_sss0 = vadd_s32(b_sss0,vround_add);
				b_sss0 = vshr_n_s32(b_sss0,2);
				
				//
				b_sss0 = vmax_s32(b_sss0,all_zeros_s32);
				b_sss0 = vmin_s32(b_sss0,all_maxval_s32);
				b_sss0 = vmla_s32(all_addval_s32,b_sss0,all_mulval_s32);
				
				rgb_dst2[0] = (short)vget_lane_s32(b_sss0, 0);
				rgb_dst2[1] = (short)vget_lane_s32(b_sss0, 1);

				
			cbufxi_ptr += 4;
			rgb_dst0+=2;
			rgb_dst1+=2;
			rgb_dst2+=2;
		}
		
		for(;w < _dst_w; w++)
		{
			int data_Y[4],data_U[4],data_V[4];
			if(sx[w]==_src_w-1)
			{
				data_Y[0] = (int)*(y_src0+sx[w]);
				data_Y[1] = data_Y[0];
				data_Y[2] = (int)*(y_src1+sx[w]);
				data_Y[3] = data_Y[2];
			}
			else
			{
				data_Y[0] = (int)*(y_src0+sx[w]);
				data_Y[1] = (int)*(y_src0+sx[w]+1);
				data_Y[2] = (int)*(y_src1+sx[w]);
				data_Y[3] = (int)*(y_src1+sx[w]+1);
			}
			
			if(0 == (sx[w]&0x1))
			{
				if(is_yuv_NV21) {
					data_V[0] = (int)*(uv_src0+sx[w]);
					data_V[1] = data_V[0];
					data_V[2] = (int)*(uv_src1+sx[w]);
					data_V[3] = data_V[2];

					if(sx[w]==_src_w-1)
					{
					data_U[0] = 0;
					data_U[1] = data_U[0];
					data_U[2] = 0;
					data_U[3] = data_U[2]; 
					}
					else
					{
					data_U[0] = (int)*(uv_src0+sx[w]+1);
					data_U[1] = data_U[0];
					data_U[2] = (int)*(uv_src1+sx[w]+1);
					data_U[3] = data_U[2]; 
					}
				}
				else
				{
					data_U[0] = (int)*(uv_src0+sx[w]);
					data_U[1] = data_U[0];
					data_U[2] = (int)*(uv_src1+sx[w]);
					data_U[3] = data_U[2];

					if(sx[w]==_src_w-1)
					{
					data_V[0] = 0;
					data_V[1] = data_V[0];
					data_V[2] = 0;
					data_V[3] = data_V[2]; 
					}
					else
					{
					data_V[0] = (int)*(uv_src0+sx[w]+1);
					data_V[1] = data_V[0];
					data_V[2] = (int)*(uv_src1+sx[w]+1);
					data_V[3] = data_V[2]; 
					}
				}
			}
			else
			{
				if(is_yuv_NV21) {
					if(sx[w]==_src_w-1)
					{
					data_V[0] = (int)*(uv_src0+sx[w]-1);
					data_V[1] = data_V[0];
					data_V[2] = (int)*(uv_src1+sx[w]-1);
					data_V[3] = data_V[2];                        
					}
					else
					{
					data_V[0] = (int)*(uv_src0+sx[w]-1);
					data_V[1] = (int)*(uv_src0+sx[w]+1);
					data_V[2] = (int)*(uv_src1+sx[w]-1);
					data_V[3] = (int)*(uv_src1+sx[w]+1);
					}

					if(sx[w]>=_src_w-2)
					{
					data_U[0] = (int)*(uv_src0+sx[w]);
					data_U[1] = data_U[0];
					data_U[2] = (int)*(uv_src1+sx[w]);
					data_U[3] = data_U[2];
					}
					else
					{
					data_U[0] = (int)*(uv_src0+sx[w]);
					data_U[1] = (int)*(uv_src0+sx[w]+2);
					data_U[2] = (int)*(uv_src1+sx[w]);
					data_U[3] = (int)*(uv_src1+sx[w]+2);                        
					}
				}
				else
				{
					if(sx[w]==_src_w-1)
					{
					data_U[0] = (int)*(uv_src0+sx[w]-1);
					data_U[1] = data_U[0];
					data_U[2] = (int)*(uv_src1+sx[w]-1);
					data_U[3] = data_U[2];                        
					}
					else
					{
					data_U[0] = (int)*(uv_src0+sx[w]-1);
					data_U[1] = (int)*(uv_src0+sx[w]+1);
					data_U[2] = (int)*(uv_src1+sx[w]-1);
					data_U[3] = (int)*(uv_src1+sx[w]+1);
					}

					if(sx[w]>=_src_w-2)
					{
					data_V[0] = (int)*(uv_src0+sx[w]);
					data_V[1] = data_V[0];
					data_V[2] = (int)*(uv_src1+sx[w]);
					data_V[3] = data_V[2];
					}
					else
					{
					data_V[0] = (int)*(uv_src0+sx[w]);
					data_V[1] = (int)*(uv_src0+sx[w]+2);
					data_V[2] = (int)*(uv_src1+sx[w]);
					data_V[3] = (int)*(uv_src1+sx[w]+2);                        
					}
				}
			}
			
			int32x4_t scr_Y,scr_U,scr_V;
			scr_Y = vld1q_s32(data_Y);
			scr_U = vld1q_s32(data_U);
			scr_V = vld1q_s32(data_V);
			
			int32x4_t y_min_val = vdupq_n_s32(16);
			scr_Y = vsubq_s32(vmaxq_s32(scr_Y,y_min_val),y_min_val);
			int32x4_t neon_bt_601_cy = vdupq_n_s32(YUV_RGB_ITUR_BT_601_CY);
			int32x4_t neon_itur_bt_shift_v = vdupq_n_s32(itur_bt_shift_v);
			scr_Y = vmlaq_s32(neon_itur_bt_shift_v,scr_Y,neon_bt_601_cy);
			
			int32x4_t uv_sub_val = vdupq_n_s32(128);
			scr_U = vsubq_s32(scr_U,uv_sub_val);
			scr_V = vsubq_s32(scr_V,uv_sub_val);
			
			//calc R = 1220542*Y + 1673527*V
			int32x4_t neon_bt_601_cvr = vdupq_n_s32(YUV_RGB_ITUR_BT_601_CVR);
			int32x4_t r_acc = vmlaq_s32(scr_Y,scr_V,neon_bt_601_cvr);
			int16x4_t r_data = vmovn_s32(vshrq_n_s32(r_acc,YUV_RGB_ITUR_BT_601_SHIFT));
			
			int16x4_t rbg_max_val = vdup_n_s16(255);
			int16x4_t rbg_min_val = vdup_n_s16(0);
			r_data = vmax_s16(r_data,rbg_min_val);
			r_data = vmin_s16(r_data,rbg_max_val);
				
			//calc G = 1220542*Y - 409993*U - 852492*V
			int32x4_t neon_bt_601_cug = vdupq_n_s32(YUV_RGB_ITUR_BT_601_CUG);
			int32x4_t neon_bt_601_cvg = vdupq_n_s32(YUV_RGB_ITUR_BT_601_CVG);
			int32x4_t g_acc = vmlaq_s32(scr_Y,scr_U,neon_bt_601_cug);
			g_acc = vmlaq_s32(g_acc,scr_V,neon_bt_601_cvg);
			int16x4_t g_data = vmovn_s32(vshrq_n_s32(g_acc,YUV_RGB_ITUR_BT_601_SHIFT));
			
			rbg_max_val = vdup_n_s16(255);
			rbg_min_val = vdup_n_s16(0);
			g_data = vmax_s16(g_data,rbg_min_val);
			g_data = vmin_s16(g_data,rbg_max_val);
			
			//calc B = 1220542*Y + 2116026*U
			int32x4_t neon_bt_601_cub = vdupq_n_s32(YUV_RGB_ITUR_BT_601_CUB);
			int32x4_t b_acc = vmlaq_s32(scr_Y,scr_U,neon_bt_601_cub);
			int16x4_t b_data = vmovn_s32(vshrq_n_s32(b_acc,YUV_RGB_ITUR_BT_601_SHIFT));
			
			rbg_max_val = vdup_n_s16(255);
			rbg_min_val = vdup_n_s16(0);
			b_data = vmax_s16(b_data,rbg_min_val);
			b_data = vmin_s16(b_data,rbg_max_val);
			
			int32x2_t vcbufxi = vreinterpret_s32_s16(vld1_s16(cbufxi_ptr));
			int32x2x2_t fxi_temp = vzip_s32(vcbufxi,vcbufxi);
			int16x4_t vcbufxi00 = vreinterpret_s16_s32(fxi_temp.val[0]);
			int16x4_t fyi_temp = vld1_s16(cbufyi_ptr);
			int32x2_t vcbufyi = vget_low_s32(vmovl_s16(fyi_temp)); 
			
			//r fliter
			int32x4_t r_ss0 = vmull_s16(r_data,vcbufxi00);
			int32x2_t r_sss0 = vpadd_s32(vget_low_s32(r_ss0),vget_high_s32(r_ss0));
			r_sss0 = vshr_n_s32(r_sss0,4);
			r_sss0 = vmul_s32(r_sss0,vcbufyi);
			r_sss0 = vshr_n_s32(r_sss0,16);
			int32x2_t r_sss1 = vpadd_s32(r_sss0,r_sss0);
			int32x2_t vround_add = vdup_n_s32(2);
			r_sss1 = vadd_s32(r_sss1,vround_add);
			r_sss1 = vshr_n_s32(r_sss1,2);
			
			//
			int32x2_t  all_zeros_s32 = vdup_n_s32(0);
			int32x2_t  all_maxval_s32 = vdup_n_s32(255);
			int32x2_t  all_mulval_s32 = vdup_n_s32(2);
			int32x2_t  all_addval_s32 = vdup_n_s32(-255);
			r_sss1 = vmax_s32(r_sss1,all_zeros_s32);
			r_sss1 = vmin_s32(r_sss1,all_maxval_s32);
			r_sss1 = vmla_s32(all_addval_s32,r_sss1,all_mulval_s32);
			
			*rgb_dst0 = (short)vget_lane_s32(r_sss1, 0);
			
			//g fliter
			int32x4_t g_ss0 = vmull_s16(g_data,vcbufxi00);
			int32x2_t g_sss0 = vpadd_s32(vget_low_s32(g_ss0),vget_high_s32(g_ss0));
			g_sss0 = vshr_n_s32(g_sss0,4);
			g_sss0 = vmul_s32(g_sss0,vcbufyi);
			g_sss0 = vshr_n_s32(g_sss0,16);
			int32x2_t g_sss1 = vpadd_s32(g_sss0,g_sss0);
			vround_add = vdup_n_s32(2);
			g_sss1 = vadd_s32(g_sss1,vround_add);
			g_sss1 = vshr_n_s32(g_sss1,2);
			
			//
			g_sss1 = vmax_s32(g_sss1,all_zeros_s32);
			g_sss1 = vmin_s32(g_sss1,all_maxval_s32);
			g_sss1 = vmla_s32(all_addval_s32,g_sss1,all_mulval_s32);
			
			*rgb_dst1 = (short)vget_lane_s32(g_sss1, 0);
			
			//b fliter
			int32x4_t b_ss0 = vmull_s16(b_data,vcbufxi00);
			int32x2_t b_sss0 = vpadd_s32(vget_low_s32(b_ss0),vget_high_s32(b_ss0));
			b_sss0 = vshr_n_s32(b_sss0,4);
			b_sss0 = vmul_s32(b_sss0,vcbufyi);
			b_sss0 = vshr_n_s32(b_sss0,16);
			int32x2_t b_sss1 = vpadd_s32(b_sss0,b_sss0);
			vround_add = vdup_n_s32(2);
			b_sss1 = vadd_s32(b_sss1,vround_add);
			b_sss1 = vshr_n_s32(b_sss1,2);
			
			//
			b_sss1 = vmax_s32(b_sss1,all_zeros_s32);
			b_sss1 = vmin_s32(b_sss1,all_maxval_s32);
			b_sss1 = vmla_s32(all_addval_s32,b_sss1,all_mulval_s32);
			
			*rgb_dst2 = (short)vget_lane_s32(b_sss1, 0);
			
			cbufxi_ptr += 2;
			rgb_dst0++;
			rgb_dst1++;
			rgb_dst2++;
		}
	}              
}

void face_resize_scale_yuv2rgb_chw_no_normalize(unsigned char* _src_yuv, int is_yuv_NV21, int _src_h, int _src_w, int box_x, int box_y,
		int _search_h, int _search_w, int _dst_h, int _dst_w, unsigned char* _dst)
{
	short sx[_dst_w];
	short cbufxi[_dst_w+1][2];//防止批处理读越界
	short sy[_dst_h];
	short cbufyi[_dst_h+1][2];//防止批处理读越界
	
	face_resize_scale_adjust(_src_h,_src_w,_search_h,_search_w,_dst_h,_dst_w,box_x,box_y,
			&sx[0],&cbufxi[0][0],&sy[0],&cbufyi[0][0]);

	int itur_bt_shift_v = (1<<(YUV_RGB_ITUR_BT_601_SHIFT - 1));
	
	unsigned char* _src_y = _src_yuv;
	unsigned char* _src_uv = _src_yuv + _src_h*_src_w;
	int h = 0;
	for (; h < _dst_h-RESIZE_REMAIN_BUF; h++)
	{
			unsigned char* y_src0 = _src_y + sy[h]*_src_w;
			unsigned char* y_src1 = _src_y + (sy[h]+1)*_src_w;
			unsigned char* uv_src0 = _src_uv + (sy[h]>>1)*_src_w;
			unsigned char* uv_src1 = _src_uv + ((sy[h]+1)>>1)*_src_w;
			int w = 0;
			short* cbufxi_ptr = &cbufxi[0][0];
			short* cbufyi_ptr = &cbufyi[h][0];
			unsigned char* rgb_dst0 = _dst + h*_dst_w;
			unsigned char* rgb_dst1 = rgb_dst0 + _dst_h*_dst_w;
			unsigned char* rgb_dst2 = rgb_dst1 + _dst_h*_dst_w;

			for(;w+1 < _dst_w; w+=2)
			{
					unsigned char* y_src00 = y_src0+sx[w];
					unsigned char* y_src01 = y_src0+sx[w+1];
					unsigned char* y_src10 = y_src1+sx[w];
					unsigned char* y_src11 = y_src1+sx[w+1];
					unsigned char* uv_src00 = uv_src0+sx[w];
					unsigned char* uv_src01 = uv_src0+sx[w+1];
					unsigned char* uv_src10 = uv_src1+sx[w];
					unsigned char* uv_src11 = uv_src1+sx[w+1];
					
					uint8x8_t y_data_temp,u_data_temp,v_data_temp;
					
					y_data_temp = vld1_lane_u8(y_src00,y_data_temp,0);
					y_data_temp = vld1_lane_u8(y_src00+1,y_data_temp,1);
					y_data_temp = vld1_lane_u8(y_src01,y_data_temp,2);
					y_data_temp = vld1_lane_u8(y_src01+1,y_data_temp,3);
					y_data_temp = vld1_lane_u8(y_src10,y_data_temp,4);
					y_data_temp = vld1_lane_u8(y_src10+1,y_data_temp,5);
					y_data_temp = vld1_lane_u8(y_src11,y_data_temp,6);
					y_data_temp = vld1_lane_u8(y_src11+1,y_data_temp,7);
				
					if(0 == (sx[w]&0x1))
					{
						if(is_yuv_NV21) {
						v_data_temp = vld1_lane_u8(uv_src00,v_data_temp,0);
						v_data_temp = vld1_lane_u8(uv_src00,v_data_temp,1);
						v_data_temp = vld1_lane_u8(uv_src10,v_data_temp,4);
						v_data_temp = vld1_lane_u8(uv_src10,v_data_temp,5);

						u_data_temp = vld1_lane_u8(uv_src00+1,u_data_temp,0);
						u_data_temp = vld1_lane_u8(uv_src00+1,u_data_temp,1);
						u_data_temp = vld1_lane_u8(uv_src10+1,u_data_temp,4);
						u_data_temp = vld1_lane_u8(uv_src10+1,u_data_temp,5);
						}
						else
						{
						u_data_temp = vld1_lane_u8(uv_src00,u_data_temp,0);
						u_data_temp = vld1_lane_u8(uv_src00,u_data_temp,1);
						u_data_temp = vld1_lane_u8(uv_src10,u_data_temp,4);
						u_data_temp = vld1_lane_u8(uv_src10,u_data_temp,5);

						v_data_temp = vld1_lane_u8(uv_src00+1,v_data_temp,0);
						v_data_temp = vld1_lane_u8(uv_src00+1,v_data_temp,1);
						v_data_temp = vld1_lane_u8(uv_src10+1,v_data_temp,4);
						v_data_temp = vld1_lane_u8(uv_src10+1,v_data_temp,5);
						}
					}
					else
					{
						if(is_yuv_NV21) {
						v_data_temp = vld1_lane_u8(uv_src00-1,v_data_temp,0);
						v_data_temp = vld1_lane_u8(uv_src00+1,v_data_temp,1);
						v_data_temp = vld1_lane_u8(uv_src10-1,v_data_temp,4);
						v_data_temp = vld1_lane_u8(uv_src10+1,v_data_temp,5);

						u_data_temp = vld1_lane_u8(uv_src00,u_data_temp,0);
						u_data_temp = vld1_lane_u8(uv_src00+2,u_data_temp,1);
						u_data_temp = vld1_lane_u8(uv_src10,u_data_temp,4);
						u_data_temp = vld1_lane_u8(uv_src10+2,u_data_temp,5);
						}
						else
						{
						u_data_temp = vld1_lane_u8(uv_src00-1,u_data_temp,0);
						u_data_temp = vld1_lane_u8(uv_src00+1,u_data_temp,1);
						u_data_temp = vld1_lane_u8(uv_src10-1,u_data_temp,4);
						u_data_temp = vld1_lane_u8(uv_src10+1,u_data_temp,5);

						v_data_temp = vld1_lane_u8(uv_src00,v_data_temp,0);
						v_data_temp = vld1_lane_u8(uv_src00+2,v_data_temp,1);
						v_data_temp = vld1_lane_u8(uv_src10,v_data_temp,4);
						v_data_temp = vld1_lane_u8(uv_src10+2,v_data_temp,5);
						}
					}

					if(0 == (sx[w+1]&0x1))
					{
						if(is_yuv_NV21) {
						v_data_temp = vld1_lane_u8(uv_src01,v_data_temp,2);
						v_data_temp = vld1_lane_u8(uv_src01,v_data_temp,3);
						v_data_temp = vld1_lane_u8(uv_src11,v_data_temp,6);
						v_data_temp = vld1_lane_u8(uv_src11,v_data_temp,7);

						u_data_temp = vld1_lane_u8(uv_src01+1,u_data_temp,2);
						u_data_temp = vld1_lane_u8(uv_src01+1,u_data_temp,3);
						u_data_temp = vld1_lane_u8(uv_src11+1,u_data_temp,6);
						u_data_temp = vld1_lane_u8(uv_src11+1,u_data_temp,7);
						}
						else
						{
						u_data_temp = vld1_lane_u8(uv_src01,u_data_temp,2);
						u_data_temp = vld1_lane_u8(uv_src01,u_data_temp,3);
						u_data_temp = vld1_lane_u8(uv_src11,u_data_temp,6);
						u_data_temp = vld1_lane_u8(uv_src11,u_data_temp,7);

						v_data_temp = vld1_lane_u8(uv_src01+1,v_data_temp,2);
						v_data_temp = vld1_lane_u8(uv_src01+1,v_data_temp,3);
						v_data_temp = vld1_lane_u8(uv_src11+1,v_data_temp,6);
						v_data_temp = vld1_lane_u8(uv_src11+1,v_data_temp,7);
						}
					}
					else
					{
						if(is_yuv_NV21) {
						v_data_temp = vld1_lane_u8(uv_src01-1,v_data_temp,2);
						v_data_temp = vld1_lane_u8(uv_src01+1,v_data_temp,3);
						v_data_temp = vld1_lane_u8(uv_src11-1,v_data_temp,6);
						v_data_temp = vld1_lane_u8(uv_src11+1,v_data_temp,7);

						u_data_temp = vld1_lane_u8(uv_src01,u_data_temp,2);
						u_data_temp = vld1_lane_u8(uv_src01+2,u_data_temp,3);
						u_data_temp = vld1_lane_u8(uv_src11,u_data_temp,6);
						u_data_temp = vld1_lane_u8(uv_src11+2,u_data_temp,7);
						}
						else
						{
						u_data_temp = vld1_lane_u8(uv_src01-1,u_data_temp,2);
						u_data_temp = vld1_lane_u8(uv_src01+1,u_data_temp,3);
						u_data_temp = vld1_lane_u8(uv_src11-1,u_data_temp,6);
						u_data_temp = vld1_lane_u8(uv_src11+1,u_data_temp,7);

						v_data_temp = vld1_lane_u8(uv_src01,v_data_temp,2);
						v_data_temp = vld1_lane_u8(uv_src01+2,v_data_temp,3);
						v_data_temp = vld1_lane_u8(uv_src11,v_data_temp,6);
						v_data_temp = vld1_lane_u8(uv_src11+2,v_data_temp,7);
						}
					}

				//
				int32x4x2_t cys_data;
				int16x8_t y_min_val = vdupq_n_s16(16);
				int16x8_t temp_data = vreinterpretq_s16_u16(vmovl_u8(y_data_temp));
				temp_data = vsubq_s16(vmaxq_s16(temp_data,y_min_val),y_min_val);
				cys_data.val[0] = vmovl_s16(vget_low_s16(temp_data));
				cys_data.val[1] = vmovl_s16(vget_high_s16(temp_data));
				
				int32x4x2_t cus_data,cvs_data;
				int16x8_t uv_sub_val = vdupq_n_s16(128);
				temp_data = vreinterpretq_s16_u16(vmovl_u8(u_data_temp));
				temp_data = vsubq_s16(temp_data,uv_sub_val);
				cus_data.val[0] = vmovl_s16(vget_low_s16(temp_data));
				cus_data.val[1] = vmovl_s16(vget_high_s16(temp_data));
				
				temp_data = vreinterpretq_s16_u16(vmovl_u8(v_data_temp));
				temp_data = vsubq_s16(temp_data,uv_sub_val);
				cvs_data.val[0] = vmovl_s16(vget_low_s16(temp_data));
				cvs_data.val[1] = vmovl_s16(vget_high_s16(temp_data));

				int32x4_t neon_bt_601_cy = vdupq_n_s32(YUV_RGB_ITUR_BT_601_CY);
				int32x4_t neon_itur_bt_shift_v = vdupq_n_s32(itur_bt_shift_v);
				cys_data.val[0] = vmlaq_s32(neon_itur_bt_shift_v,cys_data.val[0],neon_bt_601_cy);
				cys_data.val[1] = vmlaq_s32(neon_itur_bt_shift_v,cys_data.val[1],neon_bt_601_cy);

				//calc R = 1220542*Y + 1673527*V
				int32x4_t neon_bt_601_cvr = vdupq_n_s32(YUV_RGB_ITUR_BT_601_CVR);
				int32x4_t vacc0 = vmlaq_s32(cys_data.val[0],cvs_data.val[0],neon_bt_601_cvr);
				int32x4_t vacc1 = vmlaq_s32(cys_data.val[1],cvs_data.val[1],neon_bt_601_cvr);
				int16x4_t r_data0 = vmovn_s32(vshrq_n_s32(vacc0,YUV_RGB_ITUR_BT_601_SHIFT));
				int16x4_t r_data1 = vmovn_s32(vshrq_n_s32(vacc1,YUV_RGB_ITUR_BT_601_SHIFT));

				int16x4_t rbg_max_val = vdup_n_s16(255);
				int16x4_t rbg_min_val = vdup_n_s16(0);
				r_data0 = vmax_s16(r_data0,rbg_min_val);
				r_data0 = vmin_s16(r_data0,rbg_max_val);
				r_data1 = vmax_s16(r_data1,rbg_min_val);
				r_data1 = vmin_s16(r_data1,rbg_max_val);

				//calc G = 1220542*Y - 409993*U - 852492*V
				int32x4_t neon_bt_601_cug = vdupq_n_s32(YUV_RGB_ITUR_BT_601_CUG);
				int32x4_t neon_bt_601_cvg = vdupq_n_s32(YUV_RGB_ITUR_BT_601_CVG);
				vacc0 = vmlaq_s32(cys_data.val[0],cus_data.val[0],neon_bt_601_cug);
				vacc0 = vmlaq_s32(vacc0,cvs_data.val[0],neon_bt_601_cvg);
				vacc1 = vmlaq_s32(cys_data.val[1],cus_data.val[1],neon_bt_601_cug);
				vacc1 = vmlaq_s32(vacc1,cvs_data.val[1],neon_bt_601_cvg);
				int16x4_t g_data0 = vmovn_s32(vshrq_n_s32(vacc0,YUV_RGB_ITUR_BT_601_SHIFT));
				int16x4_t g_data1 = vmovn_s32(vshrq_n_s32(vacc1,YUV_RGB_ITUR_BT_601_SHIFT));

				g_data0 = vmax_s16(g_data0,rbg_min_val);
				g_data0 = vmin_s16(g_data0,rbg_max_val);
				g_data1 = vmax_s16(g_data1,rbg_min_val);
				g_data1 = vmin_s16(g_data1,rbg_max_val);

				//calc B = 1220542*Y + 2116026*U
				int32x4_t neon_bt_601_cub = vdupq_n_s32(YUV_RGB_ITUR_BT_601_CUB);
				vacc0 = vmlaq_s32(cys_data.val[0],cus_data.val[0],neon_bt_601_cub);
				vacc1 = vmlaq_s32(cys_data.val[1],cus_data.val[1],neon_bt_601_cub);
				int16x4_t b_data0 = vmovn_s32(vshrq_n_s32(vacc0,YUV_RGB_ITUR_BT_601_SHIFT));
				int16x4_t b_data1 = vmovn_s32(vshrq_n_s32(vacc1,YUV_RGB_ITUR_BT_601_SHIFT));

				b_data0 = vmax_s16(b_data0,rbg_min_val);
				b_data0 = vmin_s16(b_data0,rbg_max_val);
				b_data1 = vmax_s16(b_data1,rbg_min_val);
				b_data1 = vmin_s16(b_data1,rbg_max_val);

				int16x4_t vcbufxi = vld1_s16(cbufxi_ptr);
				//r filter
				int32x4_t r_ss0 = vmull_s16(r_data0,vcbufxi);
				int32x4_t r_ss1 = vmull_s16(r_data1,vcbufxi);
				int32x2_t r_sss0 = vpadd_s32(vget_low_s32(r_ss0),vget_high_s32(r_ss0));
				int32x2_t r_sss1 = vpadd_s32(vget_low_s32(r_ss1),vget_high_s32(r_ss1));

				r_sss0 = vshr_n_s32(r_sss0,4);
				r_sss1 = vshr_n_s32(r_sss1,4);
				r_sss0 = vmul_n_s32(r_sss0,(int)cbufyi_ptr[0]);
				r_sss1 = vmul_n_s32(r_sss1,(int)cbufyi_ptr[1]);
				r_sss0 = vshr_n_s32(r_sss0,16);
				r_sss1 = vshr_n_s32(r_sss1,16);
				r_sss0 = vadd_s32(r_sss0,r_sss1);
				//r_sss0 = vshr_n_s32(r_sss0,16);
				int32x2_t vround_add = vdup_n_s32(2);
				r_sss0 = vadd_s32(r_sss0,vround_add);
				r_sss0 = vshr_n_s32(r_sss0,2);

				rgb_dst0[0] = (unsigned char)vget_lane_s32(r_sss0, 0);
				rgb_dst0[1] = (unsigned char)vget_lane_s32(r_sss0, 1);

		//g filter
				int32x4_t g_ss0 = vmull_s16(g_data0,vcbufxi);
				int32x4_t g_ss1 = vmull_s16(g_data1,vcbufxi);
				int32x2_t g_sss0 = vpadd_s32(vget_low_s32(g_ss0),vget_high_s32(g_ss0));
				int32x2_t g_sss1 = vpadd_s32(vget_low_s32(g_ss1),vget_high_s32(g_ss1));

				g_sss0 = vshr_n_s32(g_sss0,4);
				g_sss1 = vshr_n_s32(g_sss1,4);
				g_sss0 = vmul_n_s32(g_sss0,(int)cbufyi_ptr[0]);
				g_sss1 = vmul_n_s32(g_sss1,(int)cbufyi_ptr[1]);
				g_sss0 = vshr_n_s32(g_sss0,16);
				g_sss1 = vshr_n_s32(g_sss1,16);
				g_sss0 = vadd_s32(g_sss0,g_sss1);
				//g_sss0 = vshr_n_s32(g_sss0,16);
				vround_add = vdup_n_s32(2);
				g_sss0 = vadd_s32(g_sss0,vround_add);
				g_sss0 = vshr_n_s32(g_sss0,2);

				rgb_dst1[0] = (unsigned char)vget_lane_s32(g_sss0, 0);
				rgb_dst1[1] = (unsigned char)vget_lane_s32(g_sss0, 1);

				//b filter
				int32x4_t b_ss0 = vmull_s16(b_data0,vcbufxi);
				int32x4_t b_ss1 = vmull_s16(b_data1,vcbufxi);
				int32x2_t b_sss0 = vpadd_s32(vget_low_s32(b_ss0),vget_high_s32(b_ss0));
				int32x2_t b_sss1 = vpadd_s32(vget_low_s32(b_ss1),vget_high_s32(b_ss1));
				
				b_sss0 = vshr_n_s32(b_sss0,4);
				b_sss1 = vshr_n_s32(b_sss1,4);
				b_sss0 = vmul_n_s32(b_sss0,(int)cbufyi_ptr[0]);
				b_sss1 = vmul_n_s32(b_sss1,(int)cbufyi_ptr[1]);
				b_sss0 = vshr_n_s32(b_sss0,16);
				b_sss1 = vshr_n_s32(b_sss1,16);
				b_sss0 = vadd_s32(b_sss0,b_sss1);
				//b_sss0 = vshr_n_s32(b_sss0,16);
				vround_add = vdup_n_s32(2);
				b_sss0 = vadd_s32(b_sss0,vround_add);
				b_sss0 = vshr_n_s32(b_sss0,2);

				rgb_dst2[0] = (unsigned char)vget_lane_s32(b_sss0, 0);
				rgb_dst2[1] = (unsigned char)vget_lane_s32(b_sss0, 1);

				
			cbufxi_ptr += 4;
			rgb_dst0+=2;
			rgb_dst1+=2;
			rgb_dst2+=2;
		}

		for(;w < _dst_w; w++)
		{
			int data_Y[4],data_U[4],data_V[4];
			data_Y[0] = (int)*(y_src0+sx[w]);
			data_Y[1] = (int)*(y_src0+sx[w]+1);
			data_Y[2] = (int)*(y_src1+sx[w]);
			data_Y[3] = (int)*(y_src1+sx[w]+1);

			if(is_yuv_NV21) {
				if(0 == (sx[w]&0x1))
				{
				data_V[0] = (int)*(uv_src0+sx[w]);
				data_V[1] = data_V[0];
				data_V[2] = (int)*(uv_src1+sx[w]);
				data_V[3] = data_V[2];

				data_U[0] = (int)*(uv_src0+sx[w]+1);
				data_U[1] = data_U[0];
				data_U[2] = (int)*(uv_src1+sx[w]+1);
				data_U[3] = data_U[2];
				}
				else
				{
				data_V[0] = (int)*(uv_src0+sx[w]-1);
				data_V[1] = (int)*(uv_src0+sx[w]+1);
				data_V[2] = (int)*(uv_src1+sx[w]-1);
				data_V[3] = (int)*(uv_src1+sx[w]+1);

				data_U[0] = (int)*(uv_src0+sx[w]);
				data_U[1] = (int)*(uv_src0+sx[w]+2);
				data_U[2] = (int)*(uv_src1+sx[w]);
				data_U[3] = (int)*(uv_src1+sx[w]+2);
				}
			}
			else
			{
				if(0 == (sx[w]&0x1))
				{
				data_U[0] = (int)*(uv_src0+sx[w]);
				data_U[1] = data_U[0];
				data_U[2] = (int)*(uv_src1+sx[w]);
				data_U[3] = data_U[2];

				data_V[0] = (int)*(uv_src0+sx[w]+1);
				data_V[1] = data_V[0];
				data_V[2] = (int)*(uv_src1+sx[w]+1);
				data_V[3] = data_V[2];
				}
				else
				{
				data_U[0] = (int)*(uv_src0+sx[w]-1);
				data_U[1] = (int)*(uv_src0+sx[w]+1);
				data_U[2] = (int)*(uv_src1+sx[w]-1);
				data_U[3] = (int)*(uv_src1+sx[w]+1);

				data_V[0] = (int)*(uv_src0+sx[w]);
				data_V[1] = (int)*(uv_src0+sx[w]+2);
				data_V[2] = (int)*(uv_src1+sx[w]);
				data_V[3] = (int)*(uv_src1+sx[w]+2);
				}
			}

			int32x4_t scr_Y,scr_U,scr_V;
			scr_Y = vld1q_s32(data_Y);
			scr_U = vld1q_s32(data_U);
			scr_V = vld1q_s32(data_V);

			int32x4_t y_min_val = vdupq_n_s32(16);
			scr_Y = vsubq_s32(vmaxq_s32(scr_Y,y_min_val),y_min_val);
			int32x4_t neon_bt_601_cy = vdupq_n_s32(YUV_RGB_ITUR_BT_601_CY);
			int32x4_t neon_itur_bt_shift_v = vdupq_n_s32(itur_bt_shift_v);
			scr_Y = vmlaq_s32(neon_itur_bt_shift_v,scr_Y,neon_bt_601_cy);

			int32x4_t uv_sub_val = vdupq_n_s32(128);
			scr_U = vsubq_s32(scr_U,uv_sub_val);
			scr_V = vsubq_s32(scr_V,uv_sub_val);

			//calc R = 1220542*Y + 1673527*V
			int32x4_t neon_bt_601_cvr = vdupq_n_s32(YUV_RGB_ITUR_BT_601_CVR);
			int32x4_t r_acc = vmlaq_s32(scr_Y,scr_V,neon_bt_601_cvr);
			int16x4_t r_data = vmovn_s32(vshrq_n_s32(r_acc,YUV_RGB_ITUR_BT_601_SHIFT));

			int16x4_t rbg_max_val = vdup_n_s16(255);
			int16x4_t rbg_min_val = vdup_n_s16(0);
			r_data = vmax_s16(r_data,rbg_min_val);
			r_data = vmin_s16(r_data,rbg_max_val);
				
			//calc G = 1220542*Y - 409993*U - 852492*V
			int32x4_t neon_bt_601_cug = vdupq_n_s32(YUV_RGB_ITUR_BT_601_CUG);
			int32x4_t neon_bt_601_cvg = vdupq_n_s32(YUV_RGB_ITUR_BT_601_CVG);
			int32x4_t g_acc = vmlaq_s32(scr_Y,scr_U,neon_bt_601_cug);
			g_acc = vmlaq_s32(g_acc,scr_V,neon_bt_601_cvg);
			int16x4_t g_data = vmovn_s32(vshrq_n_s32(g_acc,YUV_RGB_ITUR_BT_601_SHIFT));

			rbg_max_val = vdup_n_s16(255);
			rbg_min_val = vdup_n_s16(0);
			g_data = vmax_s16(g_data,rbg_min_val);
			g_data = vmin_s16(g_data,rbg_max_val);

			//calc B = 1220542*Y + 2116026*U
			int32x4_t neon_bt_601_cub = vdupq_n_s32(YUV_RGB_ITUR_BT_601_CUB);
			int32x4_t b_acc = vmlaq_s32(scr_Y,scr_U,neon_bt_601_cub);
			int16x4_t b_data = vmovn_s32(vshrq_n_s32(b_acc,YUV_RGB_ITUR_BT_601_SHIFT));

			rbg_max_val = vdup_n_s16(255);
			rbg_min_val = vdup_n_s16(0);
			b_data = vmax_s16(b_data,rbg_min_val);
			b_data = vmin_s16(b_data,rbg_max_val);

			int32x2_t vcbufxi = vreinterpret_s32_s16(vld1_s16(cbufxi_ptr));
			int32x2x2_t fxi_temp = vzip_s32(vcbufxi,vcbufxi);
			int16x4_t vcbufxi00 = vreinterpret_s16_s32(fxi_temp.val[0]);
			int16x4_t fyi_temp = vld1_s16(cbufyi_ptr);
			int32x2_t vcbufyi = vget_low_s32(vmovl_s16(fyi_temp));

			//r fliter
			int32x4_t r_ss0 = vmull_s16(r_data,vcbufxi00);
			int32x2_t r_sss0 = vpadd_s32(vget_low_s32(r_ss0),vget_high_s32(r_ss0));
			r_sss0 = vshr_n_s32(r_sss0,4);
			r_sss0 = vmul_s32(r_sss0,vcbufyi);
			r_sss0 = vshr_n_s32(r_sss0,16);
			int32x2_t r_sss1 = vpadd_s32(r_sss0,r_sss0);
			int32x2_t vround_add = vdup_n_s32(2);
			r_sss1 = vadd_s32(r_sss1,vround_add);
			r_sss1 = vshr_n_s32(r_sss1,2);

			*rgb_dst0 = (unsigned char)vget_lane_s32(r_sss1, 0);

			//g fliter
			int32x4_t g_ss0 = vmull_s16(g_data,vcbufxi00);
			int32x2_t g_sss0 = vpadd_s32(vget_low_s32(g_ss0),vget_high_s32(g_ss0));
			g_sss0 = vshr_n_s32(g_sss0,4);
			g_sss0 = vmul_s32(g_sss0,vcbufyi);
			g_sss0 = vshr_n_s32(g_sss0,16);
			int32x2_t g_sss1 = vpadd_s32(g_sss0,g_sss0);
			vround_add = vdup_n_s32(2);
			g_sss1 = vadd_s32(g_sss1,vround_add);
			g_sss1 = vshr_n_s32(g_sss1,2);

			*rgb_dst1 = (unsigned char)vget_lane_s32(g_sss1, 0);

			//b fliter
			int32x4_t b_ss0 = vmull_s16(b_data,vcbufxi00);
			int32x2_t b_sss0 = vpadd_s32(vget_low_s32(b_ss0),vget_high_s32(b_ss0));
			b_sss0 = vshr_n_s32(b_sss0,4);
			b_sss0 = vmul_s32(b_sss0,vcbufyi);
			b_sss0 = vshr_n_s32(b_sss0,16);
			int32x2_t b_sss1 = vpadd_s32(b_sss0,b_sss0);
			vround_add = vdup_n_s32(2);
			b_sss1 = vadd_s32(b_sss1,vround_add);
			b_sss1 = vshr_n_s32(b_sss1,2);

			*rgb_dst2 = (unsigned char)vget_lane_s32(b_sss1, 0);

			cbufxi_ptr += 2;
			rgb_dst0++;
			rgb_dst1++;
			rgb_dst2++;
		}
	} 

	//处理RESIZE_REMAIN_BUF�?
	for (; h < _dst_h; h++)
	{
		unsigned char *y_src0,*y_src1,*uv_src0,*uv_src1;
		if(sy[h] == _src_h-1)
		{
			y_src0 = _src_y + sy[h]*_src_w;
			y_src1 = y_src0;
			uv_src0 = _src_uv + (sy[h]>>1)*_src_w;
			uv_src1 = uv_src0;
		}
		else
		{
			y_src0 = _src_y + sy[h]*_src_w;
			y_src1 = _src_y + (sy[h]+1)*_src_w;
			uv_src0 = _src_uv + (sy[h]>>1)*_src_w;
			uv_src1 = _src_uv + ((sy[h]+1)>>1)*_src_w;
		}
		int w = 0;
		short* cbufxi_ptr = &cbufxi[0][0];
		short* cbufyi_ptr = &cbufyi[h][0];
		unsigned char* rgb_dst0 = _dst + h*_dst_w;
		unsigned char* rgb_dst1 = rgb_dst0 + _dst_h*_dst_w;
		unsigned char* rgb_dst2 = rgb_dst1 + _dst_h*_dst_w;

		for(;w+1 < _dst_w-RESIZE_REMAIN_BUF; w+=2)
		{
			unsigned char* y_src00 = y_src0+sx[w];
			unsigned char* y_src01 = y_src0+sx[w+1];
			unsigned char* y_src10 = y_src1+sx[w];
			unsigned char* y_src11 = y_src1+sx[w+1];
			unsigned char* uv_src00 = uv_src0+sx[w];
			unsigned char* uv_src01 = uv_src0+sx[w+1];
			unsigned char* uv_src10 = uv_src1+sx[w];
			unsigned char* uv_src11 = uv_src1+sx[w+1];
			
			uint8x8_t y_data_temp,u_data_temp,v_data_temp;
			
			y_data_temp = vld1_lane_u8(y_src00,y_data_temp,0);
			y_data_temp = vld1_lane_u8(y_src00+1,y_data_temp,1);
			y_data_temp = vld1_lane_u8(y_src01,y_data_temp,2);
			y_data_temp = vld1_lane_u8(y_src01+1,y_data_temp,3);
			y_data_temp = vld1_lane_u8(y_src10,y_data_temp,4);
			y_data_temp = vld1_lane_u8(y_src10+1,y_data_temp,5);
			y_data_temp = vld1_lane_u8(y_src11,y_data_temp,6);
			y_data_temp = vld1_lane_u8(y_src11+1,y_data_temp,7);
		
			if(0 == (sx[w]&0x1))
			{
				if(is_yuv_NV21) {
				v_data_temp = vld1_lane_u8(uv_src00,v_data_temp,0);
				v_data_temp = vld1_lane_u8(uv_src00,v_data_temp,1);
				v_data_temp = vld1_lane_u8(uv_src10,v_data_temp,4);
				v_data_temp = vld1_lane_u8(uv_src10,v_data_temp,5);

				u_data_temp = vld1_lane_u8(uv_src00+1,u_data_temp,0);
				u_data_temp = vld1_lane_u8(uv_src00+1,u_data_temp,1);
				u_data_temp = vld1_lane_u8(uv_src10+1,u_data_temp,4);
				u_data_temp = vld1_lane_u8(uv_src10+1,u_data_temp,5);
				}
				else
				{
				u_data_temp = vld1_lane_u8(uv_src00,u_data_temp,0);
				u_data_temp = vld1_lane_u8(uv_src00,u_data_temp,1);
				u_data_temp = vld1_lane_u8(uv_src10,u_data_temp,4);
				u_data_temp = vld1_lane_u8(uv_src10,u_data_temp,5);

				v_data_temp = vld1_lane_u8(uv_src00+1,v_data_temp,0);
				v_data_temp = vld1_lane_u8(uv_src00+1,v_data_temp,1);
				v_data_temp = vld1_lane_u8(uv_src10+1,v_data_temp,4);
				v_data_temp = vld1_lane_u8(uv_src10+1,v_data_temp,5);
				}
			}
			else
			{
				if(is_yuv_NV21) {
				v_data_temp = vld1_lane_u8(uv_src00-1,v_data_temp,0);
				v_data_temp = vld1_lane_u8(uv_src00+1,v_data_temp,1);
				v_data_temp = vld1_lane_u8(uv_src10-1,v_data_temp,4);
				v_data_temp = vld1_lane_u8(uv_src10+1,v_data_temp,5);

				u_data_temp = vld1_lane_u8(uv_src00,u_data_temp,0);
				u_data_temp = vld1_lane_u8(uv_src00+2,u_data_temp,1);
				u_data_temp = vld1_lane_u8(uv_src10,u_data_temp,4);
				u_data_temp = vld1_lane_u8(uv_src10+2,u_data_temp,5);
				}
				else
				{
				u_data_temp = vld1_lane_u8(uv_src00-1,u_data_temp,0);
				u_data_temp = vld1_lane_u8(uv_src00+1,u_data_temp,1);
				u_data_temp = vld1_lane_u8(uv_src10-1,u_data_temp,4);
				u_data_temp = vld1_lane_u8(uv_src10+1,u_data_temp,5);

				v_data_temp = vld1_lane_u8(uv_src00,v_data_temp,0);
				v_data_temp = vld1_lane_u8(uv_src00+2,v_data_temp,1);
				v_data_temp = vld1_lane_u8(uv_src10,v_data_temp,4);
				v_data_temp = vld1_lane_u8(uv_src10+2,v_data_temp,5);
				}
			}

			if(0 == (sx[w+1]&0x1))
			{
				if(is_yuv_NV21) {
				v_data_temp = vld1_lane_u8(uv_src01,v_data_temp,2);
				v_data_temp = vld1_lane_u8(uv_src01,v_data_temp,3);
				v_data_temp = vld1_lane_u8(uv_src11,v_data_temp,6);
				v_data_temp = vld1_lane_u8(uv_src11,v_data_temp,7);

				u_data_temp = vld1_lane_u8(uv_src01+1,u_data_temp,2);
				u_data_temp = vld1_lane_u8(uv_src01+1,u_data_temp,3);
				u_data_temp = vld1_lane_u8(uv_src11+1,u_data_temp,6);
				u_data_temp = vld1_lane_u8(uv_src11+1,u_data_temp,7);
				}
				else
				{
				u_data_temp = vld1_lane_u8(uv_src01,u_data_temp,2);
				u_data_temp = vld1_lane_u8(uv_src01,u_data_temp,3);
				u_data_temp = vld1_lane_u8(uv_src11,u_data_temp,6);
				u_data_temp = vld1_lane_u8(uv_src11,u_data_temp,7);

				v_data_temp = vld1_lane_u8(uv_src01+1,v_data_temp,2);
				v_data_temp = vld1_lane_u8(uv_src01+1,v_data_temp,3);
				v_data_temp = vld1_lane_u8(uv_src11+1,v_data_temp,6);
				v_data_temp = vld1_lane_u8(uv_src11+1,v_data_temp,7);
				}
			}
			else
			{
				if(is_yuv_NV21) {
				v_data_temp = vld1_lane_u8(uv_src01-1,v_data_temp,2);
				v_data_temp = vld1_lane_u8(uv_src01+1,v_data_temp,3);
				v_data_temp = vld1_lane_u8(uv_src11-1,v_data_temp,6);
				v_data_temp = vld1_lane_u8(uv_src11+1,v_data_temp,7);

				u_data_temp = vld1_lane_u8(uv_src01,u_data_temp,2);
				u_data_temp = vld1_lane_u8(uv_src01+2,u_data_temp,3);
				u_data_temp = vld1_lane_u8(uv_src11,u_data_temp,6);
				u_data_temp = vld1_lane_u8(uv_src11+2,u_data_temp,7);
				}
				else
				{
				u_data_temp = vld1_lane_u8(uv_src01-1,u_data_temp,2);
				u_data_temp = vld1_lane_u8(uv_src01+1,u_data_temp,3);
				u_data_temp = vld1_lane_u8(uv_src11-1,u_data_temp,6);
				u_data_temp = vld1_lane_u8(uv_src11+1,u_data_temp,7);

				v_data_temp = vld1_lane_u8(uv_src01,v_data_temp,2);
				v_data_temp = vld1_lane_u8(uv_src01+2,v_data_temp,3);
				v_data_temp = vld1_lane_u8(uv_src11,v_data_temp,6);
				v_data_temp = vld1_lane_u8(uv_src11+2,v_data_temp,7);
				}
			}

			//
			int32x4x2_t cys_data;
			int16x8_t y_min_val = vdupq_n_s16(16);
			int16x8_t temp_data = vreinterpretq_s16_u16(vmovl_u8(y_data_temp));
			temp_data = vsubq_s16(vmaxq_s16(temp_data,y_min_val),y_min_val);
			cys_data.val[0] = vmovl_s16(vget_low_s16(temp_data));
			cys_data.val[1] = vmovl_s16(vget_high_s16(temp_data));
			
			int32x4x2_t cus_data,cvs_data;
			int16x8_t uv_sub_val = vdupq_n_s16(128);
			temp_data = vreinterpretq_s16_u16(vmovl_u8(u_data_temp));
			temp_data = vsubq_s16(temp_data,uv_sub_val);
			cus_data.val[0] = vmovl_s16(vget_low_s16(temp_data));
			cus_data.val[1] = vmovl_s16(vget_high_s16(temp_data));
			
			temp_data = vreinterpretq_s16_u16(vmovl_u8(v_data_temp));
			temp_data = vsubq_s16(temp_data,uv_sub_val);
			cvs_data.val[0] = vmovl_s16(vget_low_s16(temp_data));
			cvs_data.val[1] = vmovl_s16(vget_high_s16(temp_data));

			int32x4_t neon_bt_601_cy = vdupq_n_s32(YUV_RGB_ITUR_BT_601_CY);
			int32x4_t neon_itur_bt_shift_v = vdupq_n_s32(itur_bt_shift_v);
			cys_data.val[0] = vmlaq_s32(neon_itur_bt_shift_v,cys_data.val[0],neon_bt_601_cy);
			cys_data.val[1] = vmlaq_s32(neon_itur_bt_shift_v,cys_data.val[1],neon_bt_601_cy);
			
			//calc R = 1220542*Y + 1673527*V
			int32x4_t neon_bt_601_cvr = vdupq_n_s32(YUV_RGB_ITUR_BT_601_CVR);
			int32x4_t vacc0 = vmlaq_s32(cys_data.val[0],cvs_data.val[0],neon_bt_601_cvr);
			int32x4_t vacc1 = vmlaq_s32(cys_data.val[1],cvs_data.val[1],neon_bt_601_cvr);
			int16x4_t r_data0 = vmovn_s32(vshrq_n_s32(vacc0,YUV_RGB_ITUR_BT_601_SHIFT));
			int16x4_t r_data1 = vmovn_s32(vshrq_n_s32(vacc1,YUV_RGB_ITUR_BT_601_SHIFT));

			int16x4_t rbg_max_val = vdup_n_s16(255);
			int16x4_t rbg_min_val = vdup_n_s16(0);
			r_data0 = vmax_s16(r_data0,rbg_min_val);
			r_data0 = vmin_s16(r_data0,rbg_max_val);
			r_data1 = vmax_s16(r_data1,rbg_min_val);
			r_data1 = vmin_s16(r_data1,rbg_max_val);

			//calc G = 1220542*Y - 409993*U - 852492*V
			int32x4_t neon_bt_601_cug = vdupq_n_s32(YUV_RGB_ITUR_BT_601_CUG);
			int32x4_t neon_bt_601_cvg = vdupq_n_s32(YUV_RGB_ITUR_BT_601_CVG);
			vacc0 = vmlaq_s32(cys_data.val[0],cus_data.val[0],neon_bt_601_cug);
			vacc0 = vmlaq_s32(vacc0,cvs_data.val[0],neon_bt_601_cvg);
			vacc1 = vmlaq_s32(cys_data.val[1],cus_data.val[1],neon_bt_601_cug);
			vacc1 = vmlaq_s32(vacc1,cvs_data.val[1],neon_bt_601_cvg);
			int16x4_t g_data0 = vmovn_s32(vshrq_n_s32(vacc0,YUV_RGB_ITUR_BT_601_SHIFT));
			int16x4_t g_data1 = vmovn_s32(vshrq_n_s32(vacc1,YUV_RGB_ITUR_BT_601_SHIFT));

			g_data0 = vmax_s16(g_data0,rbg_min_val);
			g_data0 = vmin_s16(g_data0,rbg_max_val);
			g_data1 = vmax_s16(g_data1,rbg_min_val);
			g_data1 = vmin_s16(g_data1,rbg_max_val);

			//calc B = 1220542*Y + 2116026*U
			int32x4_t neon_bt_601_cub = vdupq_n_s32(YUV_RGB_ITUR_BT_601_CUB);
			vacc0 = vmlaq_s32(cys_data.val[0],cus_data.val[0],neon_bt_601_cub);
			vacc1 = vmlaq_s32(cys_data.val[1],cus_data.val[1],neon_bt_601_cub);
			int16x4_t b_data0 = vmovn_s32(vshrq_n_s32(vacc0,YUV_RGB_ITUR_BT_601_SHIFT));
			int16x4_t b_data1 = vmovn_s32(vshrq_n_s32(vacc1,YUV_RGB_ITUR_BT_601_SHIFT));

			b_data0 = vmax_s16(b_data0,rbg_min_val);
			b_data0 = vmin_s16(b_data0,rbg_max_val);
			b_data1 = vmax_s16(b_data1,rbg_min_val);
			b_data1 = vmin_s16(b_data1,rbg_max_val);

			int16x4_t vcbufxi = vld1_s16(cbufxi_ptr);
			//r filter
			int32x4_t r_ss0 = vmull_s16(r_data0,vcbufxi);
			int32x4_t r_ss1 = vmull_s16(r_data1,vcbufxi);
			int32x2_t r_sss0 = vpadd_s32(vget_low_s32(r_ss0),vget_high_s32(r_ss0));
			int32x2_t r_sss1 = vpadd_s32(vget_low_s32(r_ss1),vget_high_s32(r_ss1));

			r_sss0 = vshr_n_s32(r_sss0,4);
			r_sss1 = vshr_n_s32(r_sss1,4);
			r_sss0 = vmul_n_s32(r_sss0,(int)cbufyi_ptr[0]);
			r_sss1 = vmul_n_s32(r_sss1,(int)cbufyi_ptr[1]);
			r_sss0 = vshr_n_s32(r_sss0,16);
			r_sss1 = vshr_n_s32(r_sss1,16);
			r_sss0 = vadd_s32(r_sss0,r_sss1);
			//r_sss0 = vshr_n_s32(r_sss0,16);
			int32x2_t vround_add = vdup_n_s32(2);
			r_sss0 = vadd_s32(r_sss0,vround_add);
			r_sss0 = vshr_n_s32(r_sss0,2);

			rgb_dst0[0] = (unsigned char)vget_lane_s32(r_sss0, 0);
			rgb_dst0[1] = (unsigned char)vget_lane_s32(r_sss0, 1);

			//g filter
			int32x4_t g_ss0 = vmull_s16(g_data0,vcbufxi);
			int32x4_t g_ss1 = vmull_s16(g_data1,vcbufxi);
			int32x2_t g_sss0 = vpadd_s32(vget_low_s32(g_ss0),vget_high_s32(g_ss0));
			int32x2_t g_sss1 = vpadd_s32(vget_low_s32(g_ss1),vget_high_s32(g_ss1));

			g_sss0 = vshr_n_s32(g_sss0,4);
			g_sss1 = vshr_n_s32(g_sss1,4);
			g_sss0 = vmul_n_s32(g_sss0,(int)cbufyi_ptr[0]);
			g_sss1 = vmul_n_s32(g_sss1,(int)cbufyi_ptr[1]);
			g_sss0 = vshr_n_s32(g_sss0,16);
			g_sss1 = vshr_n_s32(g_sss1,16);
			g_sss0 = vadd_s32(g_sss0,g_sss1);
			//g_sss0 = vshr_n_s32(g_sss0,16);
			vround_add = vdup_n_s32(2);
			g_sss0 = vadd_s32(g_sss0,vround_add);
			g_sss0 = vshr_n_s32(g_sss0,2);

			rgb_dst1[0] = (unsigned char)vget_lane_s32(g_sss0, 0);
			rgb_dst1[1] = (unsigned char)vget_lane_s32(g_sss0, 1);
			
			//b filter
			int32x4_t b_ss0 = vmull_s16(b_data0,vcbufxi);
			int32x4_t b_ss1 = vmull_s16(b_data1,vcbufxi);
			int32x2_t b_sss0 = vpadd_s32(vget_low_s32(b_ss0),vget_high_s32(b_ss0));
			int32x2_t b_sss1 = vpadd_s32(vget_low_s32(b_ss1),vget_high_s32(b_ss1));
			
			b_sss0 = vshr_n_s32(b_sss0,4);
			b_sss1 = vshr_n_s32(b_sss1,4);
			b_sss0 = vmul_n_s32(b_sss0,(int)cbufyi_ptr[0]);
			b_sss1 = vmul_n_s32(b_sss1,(int)cbufyi_ptr[1]);
			b_sss0 = vshr_n_s32(b_sss0,16);
			b_sss1 = vshr_n_s32(b_sss1,16);
			b_sss0 = vadd_s32(b_sss0,b_sss1);
			//b_sss0 = vshr_n_s32(b_sss0,16);
			vround_add = vdup_n_s32(2);
			b_sss0 = vadd_s32(b_sss0,vround_add);
			b_sss0 = vshr_n_s32(b_sss0,2);

			rgb_dst2[0] = (unsigned char)vget_lane_s32(b_sss0, 0);
			rgb_dst2[1] = (unsigned char)vget_lane_s32(b_sss0, 1);

				
			cbufxi_ptr += 4;
			rgb_dst0+=2;
			rgb_dst1+=2;
			rgb_dst2+=2;
		}


		for(;w < _dst_w; w++)
		{
			int data_Y[4],data_U[4],data_V[4];
			if(sx[w]==_src_w-1)
			{
				data_Y[0] = (int)*(y_src0+sx[w]);
				data_Y[1] = data_Y[0];
				data_Y[2] = (int)*(y_src1+sx[w]);
				data_Y[3] = data_Y[2];
			}
			else
			{
				data_Y[0] = (int)*(y_src0+sx[w]);
				data_Y[1] = (int)*(y_src0+sx[w]+1);
				data_Y[2] = (int)*(y_src1+sx[w]);
				data_Y[3] = (int)*(y_src1+sx[w]+1);
			}

			if(0 == (sx[w]&0x1))
			{
				if(is_yuv_NV21) {
					data_V[0] = (int)*(uv_src0+sx[w]);
					data_V[1] = data_V[0];
					data_V[2] = (int)*(uv_src1+sx[w]);
					data_V[3] = data_V[2];

					if(sx[w]==_src_w-1)
					{
					data_U[0] = 0;
					data_U[1] = data_U[0];
					data_U[2] = 0;
					data_U[3] = data_U[2];
					}
					else
					{
					data_U[0] = (int)*(uv_src0+sx[w]+1);
					data_U[1] = data_U[0];
					data_U[2] = (int)*(uv_src1+sx[w]+1);
					data_U[3] = data_U[2];
					}
				}
				else
				{
					data_U[0] = (int)*(uv_src0+sx[w]);
					data_U[1] = data_U[0];
					data_U[2] = (int)*(uv_src1+sx[w]);
					data_U[3] = data_U[2];

					if(sx[w]==_src_w-1)
					{
					data_V[0] = 0;
					data_V[1] = data_V[0];
					data_V[2] = 0;
					data_V[3] = data_V[2];
					}
					else
					{
					data_V[0] = (int)*(uv_src0+sx[w]+1);
					data_V[1] = data_V[0];
					data_V[2] = (int)*(uv_src1+sx[w]+1);
					data_V[3] = data_V[2];
					}
				}
			}
			else
			{
				if(is_yuv_NV21) {
					if(sx[w]==_src_w-1)
					{
					data_V[0] = (int)*(uv_src0+sx[w]-1);
					data_V[1] = data_V[0];
					data_V[2] = (int)*(uv_src1+sx[w]-1);
					data_V[3] = data_V[2];
					}
					else
					{
					data_V[0] = (int)*(uv_src0+sx[w]-1);
					data_V[1] = (int)*(uv_src0+sx[w]+1);
					data_V[2] = (int)*(uv_src1+sx[w]-1);
					data_V[3] = (int)*(uv_src1+sx[w]+1);
					}

					if(sx[w]>=_src_w-2)
					{
					data_U[0] = (int)*(uv_src0+sx[w]);
					data_U[1] = data_U[0];
					data_U[2] = (int)*(uv_src1+sx[w]);
					data_U[3] = data_U[2];
					}
					else
					{
					data_U[0] = (int)*(uv_src0+sx[w]);
					data_U[1] = (int)*(uv_src0+sx[w]+2);
					data_U[2] = (int)*(uv_src1+sx[w]);
					data_U[3] = (int)*(uv_src1+sx[w]+2);
					}
				}
				else
				{
					if(sx[w]==_src_w-1)
					{
					data_U[0] = (int)*(uv_src0+sx[w]-1);
					data_U[1] = data_U[0];
					data_U[2] = (int)*(uv_src1+sx[w]-1);
					data_U[3] = data_U[2];
					}
					else
					{
					data_U[0] = (int)*(uv_src0+sx[w]-1);
					data_U[1] = (int)*(uv_src0+sx[w]+1);
					data_U[2] = (int)*(uv_src1+sx[w]-1);
					data_U[3] = (int)*(uv_src1+sx[w]+1);
					}

					if(sx[w]>=_src_w-2)
					{
					data_V[0] = (int)*(uv_src0+sx[w]);
					data_V[1] = data_V[0];
					data_V[2] = (int)*(uv_src1+sx[w]);
					data_V[3] = data_V[2];
					}
					else
					{
					data_V[0] = (int)*(uv_src0+sx[w]);
					data_V[1] = (int)*(uv_src0+sx[w]+2);
					data_V[2] = (int)*(uv_src1+sx[w]);
					data_V[3] = (int)*(uv_src1+sx[w]+2);
					}
				}
			}

			int32x4_t scr_Y,scr_U,scr_V;
			scr_Y = vld1q_s32(data_Y);
			scr_U = vld1q_s32(data_U);
			scr_V = vld1q_s32(data_V);

			int32x4_t y_min_val = vdupq_n_s32(16);
			scr_Y = vsubq_s32(vmaxq_s32(scr_Y,y_min_val),y_min_val);
			int32x4_t neon_bt_601_cy = vdupq_n_s32(YUV_RGB_ITUR_BT_601_CY);
			int32x4_t neon_itur_bt_shift_v = vdupq_n_s32(itur_bt_shift_v);
			scr_Y = vmlaq_s32(neon_itur_bt_shift_v,scr_Y,neon_bt_601_cy);

			int32x4_t uv_sub_val = vdupq_n_s32(128);
			scr_U = vsubq_s32(scr_U,uv_sub_val);
			scr_V = vsubq_s32(scr_V,uv_sub_val);

			//calc R = 1220542*Y + 1673527*V
			int32x4_t neon_bt_601_cvr = vdupq_n_s32(YUV_RGB_ITUR_BT_601_CVR);
			int32x4_t r_acc = vmlaq_s32(scr_Y,scr_V,neon_bt_601_cvr);
			int16x4_t r_data = vmovn_s32(vshrq_n_s32(r_acc,YUV_RGB_ITUR_BT_601_SHIFT));

			int16x4_t rbg_max_val = vdup_n_s16(255);
			int16x4_t rbg_min_val = vdup_n_s16(0);
			r_data = vmax_s16(r_data,rbg_min_val);
			r_data = vmin_s16(r_data,rbg_max_val);
				
			//calc G = 1220542*Y - 409993*U - 852492*V
			int32x4_t neon_bt_601_cug = vdupq_n_s32(YUV_RGB_ITUR_BT_601_CUG);
			int32x4_t neon_bt_601_cvg = vdupq_n_s32(YUV_RGB_ITUR_BT_601_CVG);
			int32x4_t g_acc = vmlaq_s32(scr_Y,scr_U,neon_bt_601_cug);
			g_acc = vmlaq_s32(g_acc,scr_V,neon_bt_601_cvg);
			int16x4_t g_data = vmovn_s32(vshrq_n_s32(g_acc,YUV_RGB_ITUR_BT_601_SHIFT));

			rbg_max_val = vdup_n_s16(255);
			rbg_min_val = vdup_n_s16(0);
			g_data = vmax_s16(g_data,rbg_min_val);
			g_data = vmin_s16(g_data,rbg_max_val);

			//calc B = 1220542*Y + 2116026*U
			int32x4_t neon_bt_601_cub = vdupq_n_s32(YUV_RGB_ITUR_BT_601_CUB);
			int32x4_t b_acc = vmlaq_s32(scr_Y,scr_U,neon_bt_601_cub);
			int16x4_t b_data = vmovn_s32(vshrq_n_s32(b_acc,YUV_RGB_ITUR_BT_601_SHIFT));

			rbg_max_val = vdup_n_s16(255);
			rbg_min_val = vdup_n_s16(0);
			b_data = vmax_s16(b_data,rbg_min_val);
			b_data = vmin_s16(b_data,rbg_max_val);

			int32x2_t vcbufxi = vreinterpret_s32_s16(vld1_s16(cbufxi_ptr));
			int32x2x2_t fxi_temp = vzip_s32(vcbufxi,vcbufxi);
			int16x4_t vcbufxi00 = vreinterpret_s16_s32(fxi_temp.val[0]);
			int16x4_t fyi_temp = vld1_s16(cbufyi_ptr);
			int32x2_t vcbufyi = vget_low_s32(vmovl_s16(fyi_temp));

			//r fliter
			int32x4_t r_ss0 = vmull_s16(r_data,vcbufxi00);
			int32x2_t r_sss0 = vpadd_s32(vget_low_s32(r_ss0),vget_high_s32(r_ss0));
			r_sss0 = vshr_n_s32(r_sss0,4);
			r_sss0 = vmul_s32(r_sss0,vcbufyi);
			r_sss0 = vshr_n_s32(r_sss0,16);
			int32x2_t r_sss1 = vpadd_s32(r_sss0,r_sss0);
			int32x2_t vround_add = vdup_n_s32(2);
			r_sss1 = vadd_s32(r_sss1,vround_add);
			r_sss1 = vshr_n_s32(r_sss1,2);

			*rgb_dst0 = (unsigned char)vget_lane_s32(r_sss1, 0);

			//g fliter
			int32x4_t g_ss0 = vmull_s16(g_data,vcbufxi00);
			int32x2_t g_sss0 = vpadd_s32(vget_low_s32(g_ss0),vget_high_s32(g_ss0));
			g_sss0 = vshr_n_s32(g_sss0,4);
			g_sss0 = vmul_s32(g_sss0,vcbufyi);
			g_sss0 = vshr_n_s32(g_sss0,16);
			int32x2_t g_sss1 = vpadd_s32(g_sss0,g_sss0);
			vround_add = vdup_n_s32(2);
			g_sss1 = vadd_s32(g_sss1,vround_add);
			g_sss1 = vshr_n_s32(g_sss1,2);

			*rgb_dst1 = (unsigned char)vget_lane_s32(g_sss1, 0);

			//b fliter
			int32x4_t b_ss0 = vmull_s16(b_data,vcbufxi00);
			int32x2_t b_sss0 = vpadd_s32(vget_low_s32(b_ss0),vget_high_s32(b_ss0));
			b_sss0 = vshr_n_s32(b_sss0,4);
			b_sss0 = vmul_s32(b_sss0,vcbufyi);
			b_sss0 = vshr_n_s32(b_sss0,16);
			int32x2_t b_sss1 = vpadd_s32(b_sss0,b_sss0);
			vround_add = vdup_n_s32(2);
			b_sss1 = vadd_s32(b_sss1,vround_add);
			b_sss1 = vshr_n_s32(b_sss1,2);

			*rgb_dst2 = (unsigned char)vget_lane_s32(b_sss1, 0);

			cbufxi_ptr += 2;
			rgb_dst0++;
			rgb_dst1++;
			rgb_dst2++;
		}
	}
}
		
void face_resize_scale_yuv2Y(unsigned char* _src_y, int _src_h, int _src_w, int box_x, int box_y, 
		int _search_h, int _search_w, int _dst_h, int _dst_w, short* _dst_y)
{
    s16 sx[_dst_w];
    s16 cbufxi[_dst_w][2];
    s16 sy[_dst_h];
    s16 cbufyi[_dst_h][2];
    
    face_resize_scale_adjust(_src_h,_src_w,_search_h,_search_w,_dst_h,_dst_w,box_x,box_y,
            &sx[0],&cbufxi[0][0],&sy[0],&cbufyi[0][0]);

    int h = 0;	
	for (; h < _dst_h; h++)
	{		
	    unsigned char *y_src0,*y_src1;
		if(sy[h] == _src_h-1)
		{
			y_src0 = _src_y + sy[h]*_src_w;
			y_src1 = y_src0;
		}
		else
		{
			y_src0 = _src_y + sy[h]*_src_w;
			y_src1 = _src_y + (sy[h]+1)*_src_w;
		}

		int w = 0;
		short* cbufxi_ptr = &cbufxi[0][0];
		short* y_dst0 = _dst_y + h*_dst_w;

		for (; w+3 < _dst_w; w+=4)
		{
			unsigned char* y_src00 = y_src0+sx[w];
			unsigned char* y_src01 = y_src0+sx[w+1];
			unsigned char* y_src02 = y_src0+sx[w+2];
			unsigned char* y_src03 = y_src0+sx[w+3];                            
			unsigned char* y_src10 = y_src1+sx[w];
			unsigned char* y_src11 = y_src1+sx[w+1];
			unsigned char* y_src12 = y_src1+sx[w+2];
			unsigned char* y_src13 = y_src1+sx[w+3];

			uint8x8_t y_data_temp,y_data_temp0;
			
			y_data_temp = vld1_lane_u8(y_src00,y_data_temp,0);
			y_data_temp = vld1_lane_u8(y_src00+1,y_data_temp,1);
			y_data_temp = vld1_lane_u8(y_src01,y_data_temp,2);
			y_data_temp = vld1_lane_u8(y_src01+1,y_data_temp,3);
			y_data_temp = vld1_lane_u8(y_src02,y_data_temp,4);
			y_data_temp = vld1_lane_u8(y_src02+1,y_data_temp,5);
			y_data_temp = vld1_lane_u8(y_src03,y_data_temp,6);
			y_data_temp = vld1_lane_u8(y_src03+1,y_data_temp,7);

			y_data_temp0 = vld1_lane_u8(y_src10,y_data_temp0,0);
			y_data_temp0 = vld1_lane_u8(y_src10+1,y_data_temp0,1);
			y_data_temp0 = vld1_lane_u8(y_src11,y_data_temp0,2);
			y_data_temp0 = vld1_lane_u8(y_src11+1,y_data_temp0,3);
			y_data_temp0 = vld1_lane_u8(y_src12,y_data_temp0,4);
			y_data_temp0 = vld1_lane_u8(y_src12+1,y_data_temp0,5);
			y_data_temp0 = vld1_lane_u8(y_src13,y_data_temp0,6);
			y_data_temp0 = vld1_lane_u8(y_src13+1,y_data_temp0,7);                           
	  
			int16x8_t y_data_temp1 = vreinterpretq_s16_u16(vmovl_u8(y_data_temp));
			int16x8_t y_data_temp2 = vreinterpretq_s16_u16(vmovl_u8(y_data_temp0));
			y_data_temp1 = vshlq_n_s16(y_data_temp1,1);
			y_data_temp2 = vshlq_n_s16(y_data_temp2,1);
			
			int16x8_t all_sub_val = vdupq_n_s16(255);
			y_data_temp1 = vsubq_s16(y_data_temp1,all_sub_val);
			y_data_temp2 = vsubq_s16(y_data_temp2,all_sub_val);

			int16x8_t vcbufxi = vld1q_s16(cbufxi_ptr);
			//filter
			int32x4_t y_ss0 = vmull_s16(vget_low_s16(y_data_temp1),vget_low_s16(vcbufxi));
			int32x4_t y_ss1 = vmull_s16(vget_high_s16(y_data_temp1),vget_high_s16(vcbufxi));
			int32x4_t y_ss2 = vmull_s16(vget_low_s16(y_data_temp2),vget_low_s16(vcbufxi));
			int32x4_t y_ss3 = vmull_s16(vget_high_s16(y_data_temp2),vget_high_s16(vcbufxi));
			int32x2_t y_sss0 = vpadd_s32(vget_low_s32(y_ss0),vget_high_s32(y_ss0));
			int32x2_t y_sss1 = vpadd_s32(vget_low_s32(y_ss1),vget_high_s32(y_ss1));
			int32x2_t y_sss2 = vpadd_s32(vget_low_s32(y_ss2),vget_high_s32(y_ss2));
			int32x2_t y_sss3 = vpadd_s32(vget_low_s32(y_ss3),vget_high_s32(y_ss3));
			
			y_ss0 = vcombine_s32(y_sss0,y_sss1); //y
			y_ss1 = vcombine_s32(y_sss2,y_sss3); //y+1
			
			y_ss0 = vshrq_n_s32(y_ss0,4);
			y_ss1 = vshrq_n_s32(y_ss1,4);
			y_ss0 = vmulq_n_s32(y_ss0,(int)cbufyi[h][0]);
			y_ss1 = vmulq_n_s32(y_ss1,(int)cbufyi[h][1]);
			y_ss0 = vshrq_n_s32(y_ss0,16);
			y_ss1 = vshrq_n_s32(y_ss1,16);
			y_ss0 = vaddq_s32(y_ss0,y_ss1);
			//y_ss0 = vshrq_n_s32(y_ss0,16);
			int32x4_t vround_add = vdupq_n_s32(2);
			y_ss0 = vaddq_s32(y_ss0,vround_add);
			y_ss0 = vshrq_n_s32(y_ss0,2);

			int16x4_t y_out_data = vmovn_s32(y_ss0);
			vst1_s16(y_dst0,y_out_data);

			cbufxi_ptr += 8;
			y_dst0+=4;
		}

		if(w+1 < _dst_w)
		{
			unsigned char* y_src00 = y_src0+sx[w];
			unsigned char* y_src01 = y_src0+sx[w+1];
			unsigned char* y_src10 = y_src1+sx[w];
			unsigned char* y_src11 = y_src1+sx[w+1];

			uint8x8_t y_data_temp;
			
			y_data_temp = vld1_lane_u8(y_src00,y_data_temp,0);
			y_data_temp = vld1_lane_u8(y_src00+1,y_data_temp,1);
			y_data_temp = vld1_lane_u8(y_src01,y_data_temp,2);
			y_data_temp = vld1_lane_u8(y_src01+1,y_data_temp,3);
			y_data_temp = vld1_lane_u8(y_src10,y_data_temp,4);
			y_data_temp = vld1_lane_u8(y_src10+1,y_data_temp,5);
			y_data_temp = vld1_lane_u8(y_src11,y_data_temp,6);
			y_data_temp = vld1_lane_u8(y_src11+1,y_data_temp,7);
	  
			int16x8_t y_data_temp1 = vreinterpretq_s16_u16(vmovl_u8(y_data_temp));                         
			y_data_temp1 = vshlq_n_s16(y_data_temp1,1);
			
			int16x8_t all_sub_val = vdupq_n_s16(255);
			y_data_temp1 = vsubq_s16(y_data_temp1,all_sub_val);

			int16x4_t vcbufxi = vld1_s16(cbufxi_ptr);
			//filter
			int32x4_t y_ss0 = vmull_s16(vget_low_s16(y_data_temp1),vcbufxi);
			int32x4_t y_ss1 = vmull_s16(vget_high_s16(y_data_temp1),vcbufxi);
			int32x2_t y_sss0 = vpadd_s32(vget_low_s32(y_ss0),vget_high_s32(y_ss0));
			int32x2_t y_sss1 = vpadd_s32(vget_low_s32(y_ss1),vget_high_s32(y_ss1));

			y_sss0 = vshr_n_s32(y_sss0,4);
			y_sss1 = vshr_n_s32(y_sss1,4);
			y_sss0 = vmul_n_s32(y_sss0,(int)cbufyi[h][0]);
			y_sss1 = vmul_n_s32(y_sss1,(int)cbufyi[h][1]);
			y_sss0 = vshr_n_s32(y_sss0,16);
			y_sss1 = vshr_n_s32(y_sss1,16);
			y_sss0 = vadd_s32(y_sss0,y_sss1);
			//y_sss0 = vshr_n_s32(y_sss0,16);
			int32x2_t vround_add = vdup_n_s32(2);
			y_sss0 = vadd_s32(y_sss0,vround_add);
			y_sss0 = vshr_n_s32(y_sss0,2);

			y_dst0[0] = (short)vget_lane_s32(y_sss0, 0);
			y_dst0[1] = (short)vget_lane_s32(y_sss0, 1);

			cbufxi_ptr += 4;
			y_dst0+=2;
			w+=2;
		}                    

		if(w < _dst_w)
		{
			unsigned char* y_src00 = y_src0+sx[w];
			unsigned char* y_src10 = y_src1+sx[w];
			int y_data00 = (int)y_src00[0];
			int y_data01 = (int)y_src00[1];
			int y_data10 = (int)y_src10[0];
			int y_data11 = (int)y_src10[1];
		  
			y_data00 = (y_data00 << 1) - 255;
			y_data01 = (y_data01 << 1) - 255;
			y_data10 = (y_data10 << 1) - 255;
			y_data11 = (y_data11 << 1) - 255;

			*y_dst0 =
					(short)(((((y_data00 * cbufxi[w][0] + y_data01 * cbufxi[w][1]) >> 4) * cbufyi[h][0]) >> 16) +
					((((y_data10 * cbufxi[w][0] + y_data11 * cbufxi[w][1]) >> 4) * cbufyi[h][1]) >> 16) + 2) >> 2;

			cbufxi_ptr += 2;
			y_dst0++;
			w++;
		}
	}    
}

void face_resize_scale_yuv2Y_no_normalize(unsigned char* _src_y, int _src_h, int _src_w, int box_x, int box_y,
		int _search_h, int _search_w, int _dst_h, int _dst_w, unsigned char* _dst_y) 
{
    s16 sx[_dst_w];
    s16 cbufxi[_dst_w][2];
    s16 sy[_dst_h];
    s16 cbufyi[_dst_h][2];
    
    face_resize_scale_adjust(_src_h,_src_w,_search_h,_search_w,_dst_h,_dst_w,box_x,box_y,
            &sx[0],&cbufxi[0][0],&sy[0],&cbufyi[0][0]);

	short h = 0;
	for (; h < _dst_h; h++)
	{
	    unsigned char *y_src0,*y_src1;
		if(sy[h] == _src_h-1)
		{
			y_src0 = _src_y + sy[h]*_src_w;
			y_src1 = y_src0;
		}
		else
		{
			y_src0 = _src_y + sy[h]*_src_w;
			y_src1 = _src_y + (sy[h]+1)*_src_w;
		}
		
		int w = 0;
		short* cbufxi_ptr = &cbufxi[0][0];
		unsigned char* y_dst0 = _dst_y + h*_dst_w;

		for (; w+3 < _dst_w; w+=4)
		{
			unsigned char* y_src00 = y_src0+sx[w];
			unsigned char* y_src01 = y_src0+sx[w+1];
			unsigned char* y_src02 = y_src0+sx[w+2];
			unsigned char* y_src03 = y_src0+sx[w+3];                            
			unsigned char* y_src10 = y_src1+sx[w];
			unsigned char* y_src11 = y_src1+sx[w+1];
			unsigned char* y_src12 = y_src1+sx[w+2];
			unsigned char* y_src13 = y_src1+sx[w+3];

			uint8x8_t y_data_temp,y_data_temp0;
			
			y_data_temp = vld1_lane_u8(y_src00,y_data_temp,0);
			y_data_temp = vld1_lane_u8(y_src00+1,y_data_temp,1);
			y_data_temp = vld1_lane_u8(y_src01,y_data_temp,2);
			y_data_temp = vld1_lane_u8(y_src01+1,y_data_temp,3);
			y_data_temp = vld1_lane_u8(y_src02,y_data_temp,4);
			y_data_temp = vld1_lane_u8(y_src02+1,y_data_temp,5);
			y_data_temp = vld1_lane_u8(y_src03,y_data_temp,6);
			y_data_temp = vld1_lane_u8(y_src03+1,y_data_temp,7);

			y_data_temp0 = vld1_lane_u8(y_src10,y_data_temp0,0);
			y_data_temp0 = vld1_lane_u8(y_src10+1,y_data_temp0,1);
			y_data_temp0 = vld1_lane_u8(y_src11,y_data_temp0,2);
			y_data_temp0 = vld1_lane_u8(y_src11+1,y_data_temp0,3);
			y_data_temp0 = vld1_lane_u8(y_src12,y_data_temp0,4);
			y_data_temp0 = vld1_lane_u8(y_src12+1,y_data_temp0,5);
			y_data_temp0 = vld1_lane_u8(y_src13,y_data_temp0,6);
			y_data_temp0 = vld1_lane_u8(y_src13+1,y_data_temp0,7);                           
	  
			int16x8_t y_data_temp1 = vreinterpretq_s16_u16(vmovl_u8(y_data_temp));
			int16x8_t y_data_temp2 = vreinterpretq_s16_u16(vmovl_u8(y_data_temp0));

			int16x8_t vcbufxi = vld1q_s16(cbufxi_ptr);
			//filter
			int32x4_t y_ss0 = vmull_s16(vget_low_s16(y_data_temp1),vget_low_s16(vcbufxi));
			int32x4_t y_ss1 = vmull_s16(vget_high_s16(y_data_temp1),vget_high_s16(vcbufxi));
			int32x4_t y_ss2 = vmull_s16(vget_low_s16(y_data_temp2),vget_low_s16(vcbufxi));
			int32x4_t y_ss3 = vmull_s16(vget_high_s16(y_data_temp2),vget_high_s16(vcbufxi));
			int32x2_t y_sss0 = vpadd_s32(vget_low_s32(y_ss0),vget_high_s32(y_ss0));
			int32x2_t y_sss1 = vpadd_s32(vget_low_s32(y_ss1),vget_high_s32(y_ss1));
			int32x2_t y_sss2 = vpadd_s32(vget_low_s32(y_ss2),vget_high_s32(y_ss2));
			int32x2_t y_sss3 = vpadd_s32(vget_low_s32(y_ss3),vget_high_s32(y_ss3));
			
			y_ss0 = vcombine_s32(y_sss0,y_sss1); //y
			y_ss1 = vcombine_s32(y_sss2,y_sss3); //y+1
			
			y_ss0 = vshrq_n_s32(y_ss0,4);
			y_ss1 = vshrq_n_s32(y_ss1,4);
			y_ss0 = vmulq_n_s32(y_ss0,(int)cbufyi[h][0]);
			y_ss1 = vmulq_n_s32(y_ss1,(int)cbufyi[h][1]);
			y_ss0 = vshrq_n_s32(y_ss0,16);
			y_ss1 = vshrq_n_s32(y_ss1,16);
			y_ss0 = vaddq_s32(y_ss0,y_ss1);
			//y_ss0 = vshrq_n_s32(y_ss0,16);
			int32x4_t vround_add = vdupq_n_s32(2);
			y_ss0 = vaddq_s32(y_ss0,vround_add);
			y_ss0 = vshrq_n_s32(y_ss0,2);

			*y_dst0++ = (unsigned char)vgetq_lane_s32(y_ss0,0);
			*y_dst0++ = (unsigned char)vgetq_lane_s32(y_ss0,1);
			*y_dst0++ = (unsigned char)vgetq_lane_s32(y_ss0,2);
			*y_dst0++ = (unsigned char)vgetq_lane_s32(y_ss0,3);

			cbufxi_ptr += 8;
		}

		if(w+1 < _dst_w)
		{
			unsigned char* y_src00 = y_src0+sx[w];
			unsigned char* y_src01 = y_src0+sx[w+1];
			unsigned char* y_src10 = y_src1+sx[w];
			unsigned char* y_src11 = y_src1+sx[w+1];

			uint8x8_t y_data_temp;
			
			y_data_temp = vld1_lane_u8(y_src00,y_data_temp,0);
			y_data_temp = vld1_lane_u8(y_src00+1,y_data_temp,1);
			y_data_temp = vld1_lane_u8(y_src01,y_data_temp,2);
			y_data_temp = vld1_lane_u8(y_src01+1,y_data_temp,3);
			y_data_temp = vld1_lane_u8(y_src10,y_data_temp,4);
			y_data_temp = vld1_lane_u8(y_src10+1,y_data_temp,5);
			y_data_temp = vld1_lane_u8(y_src11,y_data_temp,6);
			y_data_temp = vld1_lane_u8(y_src11+1,y_data_temp,7);
	  
			int16x8_t y_data_temp1 = vreinterpretq_s16_u16(vmovl_u8(y_data_temp));                         

			int16x4_t vcbufxi = vld1_s16(cbufxi_ptr);
			//filter
			int32x4_t y_ss0 = vmull_s16(vget_low_s16(y_data_temp1),vcbufxi);
			int32x4_t y_ss1 = vmull_s16(vget_high_s16(y_data_temp1),vcbufxi);
			int32x2_t y_sss0 = vpadd_s32(vget_low_s32(y_ss0),vget_high_s32(y_ss0));
			int32x2_t y_sss1 = vpadd_s32(vget_low_s32(y_ss1),vget_high_s32(y_ss1));

			y_sss0 = vshr_n_s32(y_sss0,4);
			y_sss1 = vshr_n_s32(y_sss1,4);
			y_sss0 = vmul_n_s32(y_sss0,(int)cbufyi[h][0]);
			y_sss1 = vmul_n_s32(y_sss1,(int)cbufyi[h][1]);
			y_sss0 = vshr_n_s32(y_sss0,16);
			y_sss1 = vshr_n_s32(y_sss1,16);
			y_sss0 = vadd_s32(y_sss0,y_sss1);
			//y_sss0 = vshr_n_s32(y_sss0,16);
			int32x2_t vround_add = vdup_n_s32(2);
			y_sss0 = vadd_s32(y_sss0,vround_add);
			y_sss0 = vshr_n_s32(y_sss0,2);

			*y_dst0++ = (unsigned char)vget_lane_s32(y_sss0, 0);
			*y_dst0++ = (unsigned char)vget_lane_s32(y_sss0, 1);

			cbufxi_ptr += 4;
			w+=2;
		}                    

		if(w < _dst_w)
		{
			unsigned char* y_src00 = y_src0+sx[w];
			unsigned char* y_src10 = y_src1+sx[w];
			int y_data00 = (int)y_src00[0];
			int y_data01 = (int)y_src00[1];
			int y_data10 = (int)y_src10[0];
			int y_data11 = (int)y_src10[1];

			*y_dst0++ =
					(unsigned char)((((((y_data00 * cbufxi[w][0] + y_data01 * cbufxi[w][1]) >> 4) * cbufyi[h][0]) >> 16) +
					((((y_data10 * cbufxi[w][0] + y_data11 * cbufxi[w][1]) >> 4) * cbufyi[h][1]) >> 16) + 2) >> 2);

			cbufxi_ptr += 2;
			w++;
		}
	}    
}
#else
void face_resize_scale_adjust(s32 _src_h, s32 _src_w, s32 _search_h, s32 _search_w, s32 _dst_h, s32 _dst_w,
        s32 _box_x, s32 _box_y, s16* _sx, s16* _cbufxi, s16* _sy, s16* _cbufyi)
{
    double invscale_x = (double)_dst_w / _search_w;
    double invscale_y = (double)_dst_h / _search_h;
    double scale_x = 1./invscale_x;
    double scale_y = 1./invscale_y;  
    s16* sx = _sx;
    s16* cbufxi = _cbufxi;
    s16* sy = _sy;
    s16* cbufyi = _cbufyi;

    float cbufx0,cbufx1;
    for (s32 w = 0; w < _dst_w; w++)
    {
            float fx = (float)((w + 0.5) * scale_x - 0.5);
            sx[w] = (s32)fx;
            fx -= sx[w];

            sx[w] += _box_x;
            if (sx[w] < 0)
            {
                    fx = 0; sx[w] = 0;
            }
            if (sx[w] >= _src_w - 1)
            {
                    fx = 0; sx[w] = _src_w - 1;
            }

            cbufx0 = 1.f - fx;
            cbufx1 = fx;

            cbufxi[w<<1] = (s16)(cbufx0 * 2048 + 0.5);
            cbufxi[(w<<1)+1] = (s16)(cbufx1 * 2048 + 0.5);
    }

    float cbufy[2];
    for (s32 h = 0; h < _dst_h; h++)
    {
            float fy = (float)((h + 0.5) * scale_y - 0.5);
            
            sy[h] = (s32)fy;
            fy -= sy[h];
            
            sy[h] += _box_y;
            if(sy[h] < 0)
            {
                    sy[h] = 0;
                    fy = 0;
            }
            else if (sy[h] >= _src_h - 1)
            {
                    sy[h] = _src_h - 1;
                    fy = 0;
            }

            cbufy[0] = 1.f - fy;
            cbufy[1] = fy;
            
            cbufyi[h<<1] = (s16)(cbufy[0] * 2048 + 0.5);
            cbufyi[(h<<1)+1] = (s16)(cbufy[1] * 2048 + 0.5);
    }
}

void face_resize_scale_yuv2rgb_chw(u8* _src_yuv, s32 is_yuv_NV21, s32 _src_h, s32 _src_w, s32 box_x, s32 box_y,
        s32 _search_h, s32 _search_w, s32 _dst_h, s32 _dst_w, s16* _dst)
{
    s16 sx[_dst_w];
    s16 cbufxi[_dst_w][2];
    s16 sy[_dst_h];
    s16 cbufyi[_dst_h][2];
    s32 itur_bt_601_round = (1<<(YUV_RGB_ITUR_BT_601_SHIFT - 1));
    
    face_resize_scale_adjust(_src_h,_src_w,_search_h,_search_w,_dst_h,_dst_w,box_x,box_y,
            &sx[0],&cbufxi[0][0],&sy[0],&cbufyi[0][0]);

    u8* _src_y = _src_yuv;
    u8* _src_uv = _src_yuv + _src_h*_src_w;
    s32 h = 0;
    for (; h < _dst_h-RESIZE_REMAIN_BUF; h++)
    {

        u8* _src_y0 = _src_y + sy[h] * _src_w;
        u8* _src_y1 = _src_y + (sy[h]+1) * _src_w;
        u8* _src_uv0 = _src_uv + (sy[h]>>1) * _src_w;
        u8* _src_uv1 = _src_uv + ((sy[h]+1)>>1) * _src_w;
        s16* _dst_r = _dst + h*_dst_w;
        s16* _dst_g = _dst_r + _dst_h*_dst_w;
        s16* _dst_b = _dst_g + _dst_h*_dst_w;
        
        for (s32 w = 0; w < _dst_w; w++)
        {
            s32 src_u00,src_u01,src_u10,src_u11;
            s32 src_v00,src_v01,src_v10,src_v11;
            
            s32 src_y00 = (s32)_src_y0[sx[w]];
            s32 src_y01 = (s32)_src_y0[sx[w] + 1];
            s32 src_y10 = (s32)_src_y1[sx[w]];
            s32 src_y11 = (s32)_src_y1[sx[w] + 1];
            
            if(is_yuv_NV21)
			{
                if(sx[w]%2 == 0)
                {
	                src_v00 = (s32)_src_uv0[sx[w]];
	                src_v01 = (s32)_src_uv0[sx[w]];
	                src_v10 = (s32)_src_uv1[sx[w]];
	                src_v11 = (s32)_src_uv1[sx[w]];  

	                src_u00 = (s32)_src_uv0[sx[w] + 1];
	                src_u01 = (s32)_src_uv0[sx[w] + 1];
	                src_u10 = (s32)_src_uv1[sx[w] + 1];
	                src_u11 = (s32)_src_uv1[sx[w] + 1];                        
                }
                else
                {
	                src_v00 = (s32)_src_uv0[sx[w]-1];
	                src_v01 = (s32)_src_uv0[sx[w]+1];
	                src_v10 = (s32)_src_uv1[sx[w]-1];
	                src_v11 = (s32)_src_uv1[sx[w]+1];  

	                src_u00 = (s32)_src_uv0[sx[w]];
	                src_u01 = (s32)_src_uv0[sx[w] + 2];
	                src_u10 = (s32)_src_uv1[sx[w]];
	                src_u11 = (s32)_src_uv1[sx[w] + 2];                             
                }
            }
            else
            {
                if(sx[w]%2 == 0)
                {
	                src_u00 = (s32)_src_uv0[sx[w]];
	                src_u01 = (s32)_src_uv0[sx[w]];
	                src_u10 = (s32)_src_uv1[sx[w]];
	                src_u11 = (s32)_src_uv1[sx[w]];  

	                src_v00 = (s32)_src_uv0[sx[w] + 1];
	                src_v01 = (s32)_src_uv0[sx[w] + 1];
	                src_v10 = (s32)_src_uv1[sx[w] + 1];
	                src_v11 = (s32)_src_uv1[sx[w] + 1];                        
                }
                else
                {
	                src_u00 = (s32)_src_uv0[sx[w]-1];
	                src_u01 = (s32)_src_uv0[sx[w]+1];
	                src_u10 = (s32)_src_uv1[sx[w]-1];
	                src_u11 = (s32)_src_uv1[sx[w]+1];  

	                src_v00 = (s32)_src_uv0[sx[w]];
	                src_v01 = (s32)_src_uv0[sx[w] + 2];
	                src_v10 = (s32)_src_uv1[sx[w]];
	                src_v11 = (s32)_src_uv1[sx[w] + 2];                             
                }
            }
           
            src_y00 = MAX2(src_y00,16);
            src_y01 = MAX2(src_y01,16);
            src_y10 = MAX2(src_y10,16);
            src_y11 = MAX2(src_y11,16);
            src_y00 = (src_y00-16)*YUV_RGB_ITUR_BT_601_CY + itur_bt_601_round;
            src_y01 = (src_y01-16)*YUV_RGB_ITUR_BT_601_CY + itur_bt_601_round;
            src_y10 = (src_y10-16)*YUV_RGB_ITUR_BT_601_CY + itur_bt_601_round;
            src_y11 = (src_y11-16)*YUV_RGB_ITUR_BT_601_CY + itur_bt_601_round;
            
            src_u00 -= 128;
            src_u01 -= 128;
            src_u10 -= 128;
            src_u11 -= 128;
            
            src_v00 -= 128;
            src_v01 -= 128;
            src_v10 -= 128;
            src_v11 -= 128;
           
            //R
            s32 r_data00 = (src_y00 + src_v00*YUV_RGB_ITUR_BT_601_CVR) >> YUV_RGB_ITUR_BT_601_SHIFT;
            s32 r_data01 = (src_y01 + src_v01*YUV_RGB_ITUR_BT_601_CVR) >> YUV_RGB_ITUR_BT_601_SHIFT;
            s32 r_data10 = (src_y10 + src_v10*YUV_RGB_ITUR_BT_601_CVR) >> YUV_RGB_ITUR_BT_601_SHIFT;
            s32 r_data11 = (src_y11 + src_v11*YUV_RGB_ITUR_BT_601_CVR) >> YUV_RGB_ITUR_BT_601_SHIFT;

            //G
            s32 g_data00 = (src_y00 + src_u00*YUV_RGB_ITUR_BT_601_CUG + src_v00*YUV_RGB_ITUR_BT_601_CVG) >> YUV_RGB_ITUR_BT_601_SHIFT;
            s32 g_data01 = (src_y01 + src_u01*YUV_RGB_ITUR_BT_601_CUG + src_v01*YUV_RGB_ITUR_BT_601_CVG) >> YUV_RGB_ITUR_BT_601_SHIFT;
            s32 g_data10 = (src_y10 + src_u10*YUV_RGB_ITUR_BT_601_CUG + src_v10*YUV_RGB_ITUR_BT_601_CVG) >> YUV_RGB_ITUR_BT_601_SHIFT;
            s32 g_data11 = (src_y11 + src_u11*YUV_RGB_ITUR_BT_601_CUG + src_v11*YUV_RGB_ITUR_BT_601_CVG) >> YUV_RGB_ITUR_BT_601_SHIFT;
            
            //B
            s32 b_data00 = (src_y00 + src_u00*YUV_RGB_ITUR_BT_601_CUB) >> YUV_RGB_ITUR_BT_601_SHIFT;
            s32 b_data01 = (src_y01 + src_u01*YUV_RGB_ITUR_BT_601_CUB) >> YUV_RGB_ITUR_BT_601_SHIFT;
            s32 b_data10 = (src_y10 + src_u10*YUV_RGB_ITUR_BT_601_CUB) >> YUV_RGB_ITUR_BT_601_SHIFT;
            s32 b_data11 = (src_y11 + src_u11*YUV_RGB_ITUR_BT_601_CUB) >> YUV_RGB_ITUR_BT_601_SHIFT;
            
            r_data00 = (r_data00>255)? 255:((r_data00<0)? 0:r_data00);
            r_data01 = (r_data01>255)? 255:((r_data01<0)? 0:r_data01);
            r_data10 = (r_data10>255)? 255:((r_data10<0)? 0:r_data10);
            r_data11 = (r_data11>255)? 255:((r_data11<0)? 0:r_data11);
            
            g_data00 = (g_data00>255)? 255:((g_data00<0)? 0:g_data00);
            g_data01 = (g_data01>255)? 255:((g_data01<0)? 0:g_data01);
            g_data10 = (g_data10>255)? 255:((g_data10<0)? 0:g_data10);
            g_data11 = (g_data11>255)? 255:((g_data11<0)? 0:g_data11); 
            
            b_data00 = (b_data00>255)? 255:((b_data00<0)? 0:b_data00);
            b_data01 = (b_data01>255)? 255:((b_data01<0)? 0:b_data01);
            b_data10 = (b_data10>255)? 255:((b_data10<0)? 0:b_data10);
            b_data11 = (b_data11>255)? 255:((b_data11<0)? 0:b_data11);

            _dst_r[w] =
                    (((((r_data00 * cbufxi[w][0] + r_data01 * cbufxi[w][1]) >> 4) * cbufyi[h][0]) >> 16) +
                    ((((r_data10 * cbufxi[w][0] + r_data11 * cbufxi[w][1]) >> 4) * cbufyi[h][1]) >> 16) + 2) >> 2;
            _dst_r[w] = _dst_r[w]*2-255;
            _dst_g[w] =
                    (((((g_data00 * cbufxi[w][0] + g_data01 * cbufxi[w][1]) >> 4) * cbufyi[h][0]) >> 16) +
                    ((((g_data10 * cbufxi[w][0] + g_data11 * cbufxi[w][1]) >> 4) * cbufyi[h][1]) >> 16) + 2) >> 2;
            _dst_g[w] = _dst_g[w]*2-255;
            _dst_b[w] =
                    (((((b_data00 * cbufxi[w][0] + b_data01 * cbufxi[w][1]) >> 4) * cbufyi[h][0]) >> 16) +
                    ((((b_data10 * cbufxi[w][0] + b_data11 * cbufxi[w][1]) >> 4) * cbufyi[h][1]) >> 16) + 2) >> 2;
            _dst_b[w] = _dst_b[w]*2-255;    
        }
    }  
    
    //处理RESIZE_REMAIN_BUF????
    for (; h < _dst_h; h++)
    {
        u8 *_src_y0,*_src_y1,*_src_uv0,*_src_uv1;
        if(sy[h]==_src_h-1)
        {
            _src_y0 = _src_y + sy[h] * _src_w;
            _src_y1 = _src_y0;
            _src_uv0 = _src_uv + (sy[h]>>1) * _src_w;
            _src_uv1 = _src_uv0;
        }
        else
        {
            _src_y0 = _src_y + sy[h] * _src_w;
            _src_y1 = _src_y + (sy[h]+1) * _src_w;
            _src_uv0 = _src_uv + (sy[h]>>1) * _src_w;
            _src_uv1 = _src_uv + ((sy[h]+1)>>1) * _src_w;
        }
            s16* _dst_r = _dst + h*_dst_w;
            s16* _dst_g = _dst_r + _dst_h*_dst_w;
            s16* _dst_b = _dst_g + _dst_h*_dst_w;
            
        for (s32 w = 0; w < _dst_w; w++)
        {
        	s32 src_y00,src_y01,src_y10,src_y11;
            s32 src_u00,src_u01,src_u10,src_u11;
            s32 src_v00,src_v01,src_v10,src_v11;
            
            if(sx[w]==_src_w-1)
            {
            src_y00 = (s32)_src_y0[sx[w]];
            src_y01 = src_y00;
            src_y10 = (s32)_src_y1[sx[w]];
            src_y11 = src_y10;
            }
            else
            {
            src_y00 = (s32)_src_y0[sx[w]];
            src_y01 = (s32)_src_y0[sx[w] + 1];
            src_y10 = (s32)_src_y1[sx[w]];
            src_y11 = (s32)_src_y1[sx[w] + 1];
            }
            
            if(is_yuv_NV21)
			{
                if(sx[w]%2 == 0)
                {
                    if(sx[w]==_src_w-1)
                    {
	                    src_v00 = (s32)_src_uv0[sx[w]];
	                    src_v01 = (s32)_src_uv0[sx[w]];
	                    src_v10 = (s32)_src_uv1[sx[w]];
	                    src_v11 = (s32)_src_uv1[sx[w]];  

	                    src_u00 = 0;
	                    src_u01 = 0;
	                    src_u10 = 0;
	                    src_u11 = 0;
                    }
                    else
                    {
	                    src_v00 = (s32)_src_uv0[sx[w]];
	                    src_v01 = (s32)_src_uv0[sx[w]];
	                    src_v10 = (s32)_src_uv1[sx[w]];
	                    src_v11 = (s32)_src_uv1[sx[w]];  

	                    src_u00 = (s32)_src_uv0[sx[w] + 1];
	                    src_u01 = (s32)_src_uv0[sx[w] + 1];
	                    src_u10 = (s32)_src_uv1[sx[w] + 1];
	                    src_u11 = (s32)_src_uv1[sx[w] + 1];  
                    }
                }
                else
                {
                    if(sx[w]==_src_w-1)
                    {
	                    src_v00 = (s32)_src_uv0[sx[w]-1];
	                    src_v01 = src_v00;
	                    src_v10 = (s32)_src_uv1[sx[w]-1];
	                    src_v11 = src_v10; 
                    }
                    else
                    {
	                    src_v00 = (s32)_src_uv0[sx[w]-1];
	                    src_v01 = (s32)_src_uv0[sx[w]+1];
	                    src_v10 = (s32)_src_uv1[sx[w]-1];
	                    src_v11 = (s32)_src_uv1[sx[w]+1]; 
                    }

                    if(sx[w]>=_src_w-2)
                    {
	                    src_u00 = (s32)_src_uv0[sx[w]];
	                    src_u01 = src_u00;
	                    src_u10 = (s32)_src_uv1[sx[w]];
	                    src_u11 = src_u10;
                    }
                    else
                    {
	                    src_u00 = (s32)_src_uv0[sx[w]];
	                    src_u01 = (s32)_src_uv0[sx[w] + 2];
	                    src_u10 = (s32)_src_uv1[sx[w]];
	                    src_u11 = (s32)_src_uv1[sx[w] + 2];
                    }
                }
            }
            else
            {
                if(sx[w]%2 == 0)
                {
                    if(sx[w]==_src_w-1)
                    {
	                    src_u00 = (s32)_src_uv0[sx[w]];
	                    src_u01 = (s32)_src_uv0[sx[w]];
	                    src_u10 = (s32)_src_uv1[sx[w]];
	                    src_u11 = (s32)_src_uv1[sx[w]];  

	                    src_v00 = 0;
	                    src_v01 = 0;
	                    src_v10 = 0;
	                    src_v11 = 0;
                    }
                    else
                    {
	                    src_u00 = (s32)_src_uv0[sx[w]];
	                    src_u01 = (s32)_src_uv0[sx[w]];
	                    src_u10 = (s32)_src_uv1[sx[w]];
	                    src_u11 = (s32)_src_uv1[sx[w]];  

	                    src_v00 = (s32)_src_uv0[sx[w] + 1];
	                    src_v01 = (s32)_src_uv0[sx[w] + 1];
	                    src_v10 = (s32)_src_uv1[sx[w] + 1];
	                    src_v11 = (s32)_src_uv1[sx[w] + 1];  
                    }
                }
                else
                {
                    if(sx[w]==_src_w-1)
                    {
	                    src_u00 = (s32)_src_uv0[sx[w]-1];
	                    src_u01 = src_u00;
	                    src_u10 = (s32)_src_uv1[sx[w]-1];
	                    src_u11 = src_u10; 
                    }
                    else
                    {
	                    src_u00 = (s32)_src_uv0[sx[w]-1];
	                    src_u01 = (s32)_src_uv0[sx[w]+1];
	                    src_u10 = (s32)_src_uv1[sx[w]-1];
	                    src_u11 = (s32)_src_uv1[sx[w]+1]; 
                    }

                    if(sx[w]>=_src_w-2)
                    {
	                    src_v00 = (s32)_src_uv0[sx[w]];
	                    src_v01 = src_v00;
	                    src_v10 = (s32)_src_uv1[sx[w]];
	                    src_v11 = src_v10;
                    }
                    else
                    {
	                    src_v00 = (s32)_src_uv0[sx[w]];
	                    src_v01 = (s32)_src_uv0[sx[w] + 2];
	                    src_v10 = (s32)_src_uv1[sx[w]];
	                    src_v11 = (s32)_src_uv1[sx[w] + 2];
                    }
                }
            }
           
            src_y00 = MAX2(src_y00,16);
            src_y01 = MAX2(src_y01,16);
            src_y10 = MAX2(src_y10,16);
            src_y11 = MAX2(src_y11,16);
            src_y00 = (src_y00-16)*YUV_RGB_ITUR_BT_601_CY + itur_bt_601_round;
            src_y01 = (src_y01-16)*YUV_RGB_ITUR_BT_601_CY + itur_bt_601_round;
            src_y10 = (src_y10-16)*YUV_RGB_ITUR_BT_601_CY + itur_bt_601_round;
            src_y11 = (src_y11-16)*YUV_RGB_ITUR_BT_601_CY + itur_bt_601_round;
            
            src_u00 -= 128;
            src_u01 -= 128;
            src_u10 -= 128;
            src_u11 -= 128;
            
            src_v00 -= 128;
            src_v01 -= 128;
            src_v10 -= 128;
            src_v11 -= 128;
           
            //R
            s32 r_data00 = (src_y00 + src_v00*YUV_RGB_ITUR_BT_601_CVR) >> YUV_RGB_ITUR_BT_601_SHIFT;
            s32 r_data01 = (src_y01 + src_v01*YUV_RGB_ITUR_BT_601_CVR) >> YUV_RGB_ITUR_BT_601_SHIFT;
            s32 r_data10 = (src_y10 + src_v10*YUV_RGB_ITUR_BT_601_CVR) >> YUV_RGB_ITUR_BT_601_SHIFT;
            s32 r_data11 = (src_y11 + src_v11*YUV_RGB_ITUR_BT_601_CVR) >> YUV_RGB_ITUR_BT_601_SHIFT;
            
            //G
            s32 g_data00 = (src_y00 + src_u00*YUV_RGB_ITUR_BT_601_CUG + src_v00*YUV_RGB_ITUR_BT_601_CVG) >> YUV_RGB_ITUR_BT_601_SHIFT;
            s32 g_data01 = (src_y01 + src_u01*YUV_RGB_ITUR_BT_601_CUG + src_v01*YUV_RGB_ITUR_BT_601_CVG) >> YUV_RGB_ITUR_BT_601_SHIFT;
            s32 g_data10 = (src_y10 + src_u10*YUV_RGB_ITUR_BT_601_CUG + src_v10*YUV_RGB_ITUR_BT_601_CVG) >> YUV_RGB_ITUR_BT_601_SHIFT;
            s32 g_data11 = (src_y11 + src_u11*YUV_RGB_ITUR_BT_601_CUG + src_v11*YUV_RGB_ITUR_BT_601_CVG) >> YUV_RGB_ITUR_BT_601_SHIFT;
            
            //B
            s32 b_data00 = (src_y00 + src_u00*YUV_RGB_ITUR_BT_601_CUB) >> YUV_RGB_ITUR_BT_601_SHIFT;
            s32 b_data01 = (src_y01 + src_u01*YUV_RGB_ITUR_BT_601_CUB) >> YUV_RGB_ITUR_BT_601_SHIFT;
            s32 b_data10 = (src_y10 + src_u10*YUV_RGB_ITUR_BT_601_CUB) >> YUV_RGB_ITUR_BT_601_SHIFT;
            s32 b_data11 = (src_y11 + src_u11*YUV_RGB_ITUR_BT_601_CUB) >> YUV_RGB_ITUR_BT_601_SHIFT;

            r_data00 = (r_data00>255)? 255:((r_data00<0)? 0:r_data00);
            r_data01 = (r_data01>255)? 255:((r_data01<0)? 0:r_data01);
            r_data10 = (r_data10>255)? 255:((r_data10<0)? 0:r_data10);
            r_data11 = (r_data11>255)? 255:((r_data11<0)? 0:r_data11);
            
            g_data00 = (g_data00>255)? 255:((g_data00<0)? 0:g_data00);
            g_data01 = (g_data01>255)? 255:((g_data01<0)? 0:g_data01);
            g_data10 = (g_data10>255)? 255:((g_data10<0)? 0:g_data10);
            g_data11 = (g_data11>255)? 255:((g_data11<0)? 0:g_data11); 
            
            b_data00 = (b_data00>255)? 255:((b_data00<0)? 0:b_data00);
            b_data01 = (b_data01>255)? 255:((b_data01<0)? 0:b_data01);
            b_data10 = (b_data10>255)? 255:((b_data10<0)? 0:b_data10);
            b_data11 = (b_data11>255)? 255:((b_data11<0)? 0:b_data11);
            
            _dst_r[w] =
                    (((((r_data00 * cbufxi[w][0] + r_data01 * cbufxi[w][1]) >> 4) * cbufyi[h][0]) >> 16) +
                    ((((r_data10 * cbufxi[w][0] + r_data11 * cbufxi[w][1]) >> 4) * cbufyi[h][1]) >> 16) + 2) >> 2;
            _dst_r[w] = _dst_r[w]*2-255;
            _dst_g[w] =
                    (((((g_data00 * cbufxi[w][0] + g_data01 * cbufxi[w][1]) >> 4) * cbufyi[h][0]) >> 16) +
                    ((((g_data10 * cbufxi[w][0] + g_data11 * cbufxi[w][1]) >> 4) * cbufyi[h][1]) >> 16) + 2) >> 2;
            _dst_g[w] = _dst_g[w]*2-255;
            _dst_b[w] =
                    (((((b_data00 * cbufxi[w][0] + b_data01 * cbufxi[w][1]) >> 4) * cbufyi[h][0]) >> 16) +
                    ((((b_data10 * cbufxi[w][0] + b_data11 * cbufxi[w][1]) >> 4) * cbufyi[h][1]) >> 16) + 2) >> 2;
            _dst_b[w] = _dst_b[w]*2-255;    
        }
    } 
}   

void face_resize_scale_yuv2rgb_chw_no_normalize(u8* _src_yuv, s32 is_yuv_NV21, s32 _src_h, s32 _src_w, s32 box_x, s32 box_y,
        s32 _search_h, s32 _search_w, s32 _dst_h, s32 _dst_w, u8* _dst)
{
    s16 sx[_dst_w];
    s16 cbufxi[_dst_w][2];
    s16 sy[_dst_h];
    s16 cbufyi[_dst_h][2];
    s32 itur_bt_601_round = (1<<(YUV_RGB_ITUR_BT_601_SHIFT - 1));
    
    face_resize_scale_adjust(_src_h,_src_w,_search_h,_search_w,_dst_h,_dst_w,box_x,box_y,
            &sx[0],&cbufxi[0][0],&sy[0],&cbufyi[0][0]);

    u8* _src_y = _src_yuv;
    u8* _src_uv = _src_yuv + _src_h*_src_w;
    s32 h = 0;
    for (; h < _dst_h-RESIZE_REMAIN_BUF; h++)
    {

        u8* _src_y0 = _src_y + sy[h] * _src_w;
        u8* _src_y1 = _src_y + (sy[h]+1) * _src_w;
        u8* _src_uv0 = _src_uv + (sy[h]>>1) * _src_w;
        u8* _src_uv1 = _src_uv + ((sy[h]+1)>>1) * _src_w;
        u8* _dst_r = _dst + h*_dst_w;
        u8* _dst_g = _dst_r + _dst_h*_dst_w;
        u8* _dst_b = _dst_g + _dst_h*_dst_w;
        
        for (s32 w = 0; w < _dst_w; w++)
        {
            s32 src_u00,src_u01,src_u10,src_u11;
            s32 src_v00,src_v01,src_v10,src_v11;
            
            s32 src_y00 = (s32)_src_y0[sx[w]];
            s32 src_y01 = (s32)_src_y0[sx[w] + 1];
            s32 src_y10 = (s32)_src_y1[sx[w]];
            s32 src_y11 = (s32)_src_y1[sx[w] + 1];
            
            if(is_yuv_NV21)
			{
                if(sx[w]%2 == 0)
                {
	                src_v00 = (s32)_src_uv0[sx[w]];
	                src_v01 = (s32)_src_uv0[sx[w]];
	                src_v10 = (s32)_src_uv1[sx[w]];
	                src_v11 = (s32)_src_uv1[sx[w]];  

	                src_u00 = (s32)_src_uv0[sx[w] + 1];
	                src_u01 = (s32)_src_uv0[sx[w] + 1];
	                src_u10 = (s32)_src_uv1[sx[w] + 1];
	                src_u11 = (s32)_src_uv1[sx[w] + 1];                        
                }
                else
                {
	                src_v00 = (s32)_src_uv0[sx[w]-1];
	                src_v01 = (s32)_src_uv0[sx[w]+1];
	                src_v10 = (s32)_src_uv1[sx[w]-1];
	                src_v11 = (s32)_src_uv1[sx[w]+1];  

	                src_u00 = (s32)_src_uv0[sx[w]];
	                src_u01 = (s32)_src_uv0[sx[w] + 2];
	                src_u10 = (s32)_src_uv1[sx[w]];
	                src_u11 = (s32)_src_uv1[sx[w] + 2];                             
                }
            }
            else
            {
                if(sx[w]%2 == 0)
                {
	                src_u00 = (s32)_src_uv0[sx[w]];
	                src_u01 = (s32)_src_uv0[sx[w]];
	                src_u10 = (s32)_src_uv1[sx[w]];
	                src_u11 = (s32)_src_uv1[sx[w]];  

	                src_v00 = (s32)_src_uv0[sx[w] + 1];
	                src_v01 = (s32)_src_uv0[sx[w] + 1];
	                src_v10 = (s32)_src_uv1[sx[w] + 1];
	                src_v11 = (s32)_src_uv1[sx[w] + 1];                        
                }
                else
                {
	                src_u00 = (s32)_src_uv0[sx[w]-1];
	                src_u01 = (s32)_src_uv0[sx[w]+1];
	                src_u10 = (s32)_src_uv1[sx[w]-1];
	                src_u11 = (s32)_src_uv1[sx[w]+1];  

	                src_v00 = (s32)_src_uv0[sx[w]];
	                src_v01 = (s32)_src_uv0[sx[w] + 2];
	                src_v10 = (s32)_src_uv1[sx[w]];
	                src_v11 = (s32)_src_uv1[sx[w] + 2];                             
                }
            }
           
            src_y00 = MAX2(src_y00,16);
            src_y01 = MAX2(src_y01,16);
            src_y10 = MAX2(src_y10,16);
            src_y11 = MAX2(src_y11,16);
            src_y00 = (src_y00-16)*YUV_RGB_ITUR_BT_601_CY + itur_bt_601_round;
            src_y01 = (src_y01-16)*YUV_RGB_ITUR_BT_601_CY + itur_bt_601_round;
            src_y10 = (src_y10-16)*YUV_RGB_ITUR_BT_601_CY + itur_bt_601_round;
            src_y11 = (src_y11-16)*YUV_RGB_ITUR_BT_601_CY + itur_bt_601_round;
            
            src_u00 -= 128;
            src_u01 -= 128;
            src_u10 -= 128;
            src_u11 -= 128;
            
            src_v00 -= 128;
            src_v01 -= 128;
            src_v10 -= 128;
            src_v11 -= 128;
           
            //R
            s32 r_data00 = (src_y00 + src_v00*YUV_RGB_ITUR_BT_601_CVR) >> YUV_RGB_ITUR_BT_601_SHIFT;
            s32 r_data01 = (src_y01 + src_v01*YUV_RGB_ITUR_BT_601_CVR) >> YUV_RGB_ITUR_BT_601_SHIFT;
            s32 r_data10 = (src_y10 + src_v10*YUV_RGB_ITUR_BT_601_CVR) >> YUV_RGB_ITUR_BT_601_SHIFT;
            s32 r_data11 = (src_y11 + src_v11*YUV_RGB_ITUR_BT_601_CVR) >> YUV_RGB_ITUR_BT_601_SHIFT;
           
            //G
            s32 g_data00 = (src_y00 + src_u00*YUV_RGB_ITUR_BT_601_CUG + src_v00*YUV_RGB_ITUR_BT_601_CVG) >> YUV_RGB_ITUR_BT_601_SHIFT;
            s32 g_data01 = (src_y01 + src_u01*YUV_RGB_ITUR_BT_601_CUG + src_v01*YUV_RGB_ITUR_BT_601_CVG) >> YUV_RGB_ITUR_BT_601_SHIFT;
            s32 g_data10 = (src_y10 + src_u10*YUV_RGB_ITUR_BT_601_CUG + src_v10*YUV_RGB_ITUR_BT_601_CVG) >> YUV_RGB_ITUR_BT_601_SHIFT;
            s32 g_data11 = (src_y11 + src_u11*YUV_RGB_ITUR_BT_601_CUG + src_v11*YUV_RGB_ITUR_BT_601_CVG) >> YUV_RGB_ITUR_BT_601_SHIFT;
            
            //B
            s32 b_data00 = (src_y00 + src_u00*YUV_RGB_ITUR_BT_601_CUB) >> YUV_RGB_ITUR_BT_601_SHIFT;
            s32 b_data01 = (src_y01 + src_u01*YUV_RGB_ITUR_BT_601_CUB) >> YUV_RGB_ITUR_BT_601_SHIFT;
            s32 b_data10 = (src_y10 + src_u10*YUV_RGB_ITUR_BT_601_CUB) >> YUV_RGB_ITUR_BT_601_SHIFT;
            s32 b_data11 = (src_y11 + src_u11*YUV_RGB_ITUR_BT_601_CUB) >> YUV_RGB_ITUR_BT_601_SHIFT;

            r_data00 = (r_data00>255)? 255:((r_data00<0)? 0:r_data00);
            r_data01 = (r_data01>255)? 255:((r_data01<0)? 0:r_data01);
            r_data10 = (r_data10>255)? 255:((r_data10<0)? 0:r_data10);
            r_data11 = (r_data11>255)? 255:((r_data11<0)? 0:r_data11);
            
            g_data00 = (g_data00>255)? 255:((g_data00<0)? 0:g_data00);
            g_data01 = (g_data01>255)? 255:((g_data01<0)? 0:g_data01);
            g_data10 = (g_data10>255)? 255:((g_data10<0)? 0:g_data10);
            g_data11 = (g_data11>255)? 255:((g_data11<0)? 0:g_data11); 
            
            b_data00 = (b_data00>255)? 255:((b_data00<0)? 0:b_data00);
            b_data01 = (b_data01>255)? 255:((b_data01<0)? 0:b_data01);
            b_data10 = (b_data10>255)? 255:((b_data10<0)? 0:b_data10);
            b_data11 = (b_data11>255)? 255:((b_data11<0)? 0:b_data11);
            
            _dst_r[w] =
                    (((((r_data00 * cbufxi[w][0] + r_data01 * cbufxi[w][1]) >> 4) * cbufyi[h][0]) >> 16) +
                    ((((r_data10 * cbufxi[w][0] + r_data11 * cbufxi[w][1]) >> 4) * cbufyi[h][1]) >> 16) + 2) >> 2;

            _dst_g[w] =
                    (((((g_data00 * cbufxi[w][0] + g_data01 * cbufxi[w][1]) >> 4) * cbufyi[h][0]) >> 16) +
                    ((((g_data10 * cbufxi[w][0] + g_data11 * cbufxi[w][1]) >> 4) * cbufyi[h][1]) >> 16) + 2) >> 2;

            _dst_b[w] =
                    (((((b_data00 * cbufxi[w][0] + b_data01 * cbufxi[w][1]) >> 4) * cbufyi[h][0]) >> 16) +
                    ((((b_data10 * cbufxi[w][0] + b_data11 * cbufxi[w][1]) >> 4) * cbufyi[h][1]) >> 16) + 2) >> 2;

        }
    }  
    
    //处理RESIZE_REMAIN_BUF????
    for (; h < _dst_h; h++)
    {
        u8 *_src_y0,*_src_y1,*_src_uv0,*_src_uv1;
        if(sy[h]==_src_h-1)
        {
            _src_y0 = _src_y + sy[h] * _src_w;
            _src_y1 = _src_y0;
            _src_uv0 = _src_uv + (sy[h]>>1) * _src_w;
            _src_uv1 = _src_uv0;
        }
        else
        {
            _src_y0 = _src_y + sy[h] * _src_w;
            _src_y1 = _src_y + (sy[h]+1) * _src_w;
            _src_uv0 = _src_uv + (sy[h]>>1) * _src_w;
            _src_uv1 = _src_uv + ((sy[h]+1)>>1) * _src_w;
        }
        u8* _dst_r = _dst + h*_dst_w;
        u8* _dst_g = _dst_r + _dst_h*_dst_w;
        u8* _dst_b = _dst_g + _dst_h*_dst_w;
        
        for (s32 w = 0; w < _dst_w; w++)
        {
        	s32 src_y00,src_y01,src_y10,src_y11;
            s32 src_u00,src_u01,src_u10,src_u11;
            s32 src_v00,src_v01,src_v10,src_v11;
            
            if(sx[w]==_src_w-1)
            {
	            src_y00 = (s32)_src_y0[sx[w]];
	            src_y01 = src_y00;
	            src_y10 = (s32)_src_y1[sx[w]];
	            src_y11 = src_y10;
            }
            else
            {
	            src_y00 = (s32)_src_y0[sx[w]];
	            src_y01 = (s32)_src_y0[sx[w] + 1];
	            src_y10 = (s32)_src_y1[sx[w]];
	            src_y11 = (s32)_src_y1[sx[w] + 1];
            }
            
            if(is_yuv_NV21)
			{
                if(sx[w]%2 == 0)
                {
                    if(sx[w]==_src_w-1)
                    {
	                    src_v00 = (s32)_src_uv0[sx[w]];
	                    src_v01 = (s32)_src_uv0[sx[w]];
	                    src_v10 = (s32)_src_uv1[sx[w]];
	                    src_v11 = (s32)_src_uv1[sx[w]];  

	                    src_u00 = 0;
	                    src_u01 = 0;
	                    src_u10 = 0;
	                    src_u11 = 0;
                    }
                    else
                    {
	                    src_v00 = (s32)_src_uv0[sx[w]];
	                    src_v01 = (s32)_src_uv0[sx[w]];
	                    src_v10 = (s32)_src_uv1[sx[w]];
	                    src_v11 = (s32)_src_uv1[sx[w]];  

	                    src_u00 = (s32)_src_uv0[sx[w] + 1];
	                    src_u01 = (s32)_src_uv0[sx[w] + 1];
	                    src_u10 = (s32)_src_uv1[sx[w] + 1];
	                    src_u11 = (s32)_src_uv1[sx[w] + 1];  
                    }
                }
                else
                {
                    if(sx[w]==_src_w-1)
                    {
	                    src_v00 = (s32)_src_uv0[sx[w]-1];
	                    src_v01 = src_v00;
	                    src_v10 = (s32)_src_uv1[sx[w]-1];
	                    src_v11 = src_v10; 
                    }
                    else
                    {
	                    src_v00 = (s32)_src_uv0[sx[w]-1];
	                    src_v01 = (s32)_src_uv0[sx[w]+1];
	                    src_v10 = (s32)_src_uv1[sx[w]-1];
	                    src_v11 = (s32)_src_uv1[sx[w]+1]; 
                    }

                    if(sx[w]>=_src_w-2)
                    {
	                    src_u00 = (s32)_src_uv0[sx[w]];
	                    src_u01 = src_u00;
	                    src_u10 = (s32)_src_uv1[sx[w]];
	                    src_u11 = src_u10;
                    }
                    else
                    {
	                    src_u00 = (s32)_src_uv0[sx[w]];
	                    src_u01 = (s32)_src_uv0[sx[w] + 2];
	                    src_u10 = (s32)_src_uv1[sx[w]];
	                    src_u11 = (s32)_src_uv1[sx[w] + 2];
                    }
                }
            }
            else
            {
                if(sx[w]%2 == 0)
                {
                    if(sx[w]==_src_w-1)
                    {
	                    src_u00 = (s32)_src_uv0[sx[w]];
	                    src_u01 = (s32)_src_uv0[sx[w]];
	                    src_u10 = (s32)_src_uv1[sx[w]];
	                    src_u11 = (s32)_src_uv1[sx[w]];  

	                    src_v00 = 0;
	                    src_v01 = 0;
	                    src_v10 = 0;
	                    src_v11 = 0;
                    }
                    else
                    {
	                    src_u00 = (s32)_src_uv0[sx[w]];
	                    src_u01 = (s32)_src_uv0[sx[w]];
	                    src_u10 = (s32)_src_uv1[sx[w]];
	                    src_u11 = (s32)_src_uv1[sx[w]];  

	                    src_v00 = (s32)_src_uv0[sx[w] + 1];
	                    src_v01 = (s32)_src_uv0[sx[w] + 1];
	                    src_v10 = (s32)_src_uv1[sx[w] + 1];
	                    src_v11 = (s32)_src_uv1[sx[w] + 1];  
                    }
                }
                else
                {
                    if(sx[w]==_src_w-1)
                    {
	                    src_u00 = (s32)_src_uv0[sx[w]-1];
	                    src_u01 = src_u00;
	                    src_u10 = (s32)_src_uv1[sx[w]-1];
	                    src_u11 = src_u10; 
                    }
                    else
                    {
	                    src_u00 = (s32)_src_uv0[sx[w]-1];
	                    src_u01 = (s32)_src_uv0[sx[w]+1];
	                    src_u10 = (s32)_src_uv1[sx[w]-1];
	                    src_u11 = (s32)_src_uv1[sx[w]+1]; 
                    }

                    if(sx[w]>=_src_w-2)
                    {
	                    src_v00 = (s32)_src_uv0[sx[w]];
	                    src_v01 = src_v00;
	                    src_v10 = (s32)_src_uv1[sx[w]];
	                    src_v11 = src_v10;
                    }
                    else
                    {
	                    src_v00 = (s32)_src_uv0[sx[w]];
	                    src_v01 = (s32)_src_uv0[sx[w] + 2];
	                    src_v10 = (s32)_src_uv1[sx[w]];
	                    src_v11 = (s32)_src_uv1[sx[w] + 2];
                    }
                }
            }
           
            src_y00 = MAX2(src_y00,16);
            src_y01 = MAX2(src_y01,16);
            src_y10 = MAX2(src_y10,16);
            src_y11 = MAX2(src_y11,16);
            src_y00 = (src_y00-16)*YUV_RGB_ITUR_BT_601_CY + itur_bt_601_round;
            src_y01 = (src_y01-16)*YUV_RGB_ITUR_BT_601_CY + itur_bt_601_round;
            src_y10 = (src_y10-16)*YUV_RGB_ITUR_BT_601_CY + itur_bt_601_round;
            src_y11 = (src_y11-16)*YUV_RGB_ITUR_BT_601_CY + itur_bt_601_round;
            
            src_u00 -= 128;
            src_u01 -= 128;
            src_u10 -= 128;
            src_u11 -= 128;
            
            src_v00 -= 128;
            src_v01 -= 128;
            src_v10 -= 128;
            src_v11 -= 128;
           
            //R
            s32 r_data00 = (src_y00 + src_v00*YUV_RGB_ITUR_BT_601_CVR) >> YUV_RGB_ITUR_BT_601_SHIFT;
            s32 r_data01 = (src_y01 + src_v01*YUV_RGB_ITUR_BT_601_CVR) >> YUV_RGB_ITUR_BT_601_SHIFT;
            s32 r_data10 = (src_y10 + src_v10*YUV_RGB_ITUR_BT_601_CVR) >> YUV_RGB_ITUR_BT_601_SHIFT;
            s32 r_data11 = (src_y11 + src_v11*YUV_RGB_ITUR_BT_601_CVR) >> YUV_RGB_ITUR_BT_601_SHIFT;
           
            //G
            s32 g_data00 = (src_y00 + src_u00*YUV_RGB_ITUR_BT_601_CUG + src_v00*YUV_RGB_ITUR_BT_601_CVG) >> YUV_RGB_ITUR_BT_601_SHIFT;
            s32 g_data01 = (src_y01 + src_u01*YUV_RGB_ITUR_BT_601_CUG + src_v01*YUV_RGB_ITUR_BT_601_CVG) >> YUV_RGB_ITUR_BT_601_SHIFT;
            s32 g_data10 = (src_y10 + src_u10*YUV_RGB_ITUR_BT_601_CUG + src_v10*YUV_RGB_ITUR_BT_601_CVG) >> YUV_RGB_ITUR_BT_601_SHIFT;
            s32 g_data11 = (src_y11 + src_u11*YUV_RGB_ITUR_BT_601_CUG + src_v11*YUV_RGB_ITUR_BT_601_CVG) >> YUV_RGB_ITUR_BT_601_SHIFT;
            
            //B
            s32 b_data00 = (src_y00 + src_u00*YUV_RGB_ITUR_BT_601_CUB) >> YUV_RGB_ITUR_BT_601_SHIFT;
            s32 b_data01 = (src_y01 + src_u01*YUV_RGB_ITUR_BT_601_CUB) >> YUV_RGB_ITUR_BT_601_SHIFT;
            s32 b_data10 = (src_y10 + src_u10*YUV_RGB_ITUR_BT_601_CUB) >> YUV_RGB_ITUR_BT_601_SHIFT;
            s32 b_data11 = (src_y11 + src_u11*YUV_RGB_ITUR_BT_601_CUB) >> YUV_RGB_ITUR_BT_601_SHIFT;

            r_data00 = (r_data00>255)? 255:((r_data00<0)? 0:r_data00);
            r_data01 = (r_data01>255)? 255:((r_data01<0)? 0:r_data01);
            r_data10 = (r_data10>255)? 255:((r_data10<0)? 0:r_data10);
            r_data11 = (r_data11>255)? 255:((r_data11<0)? 0:r_data11);
            
            g_data00 = (g_data00>255)? 255:((g_data00<0)? 0:g_data00);
            g_data01 = (g_data01>255)? 255:((g_data01<0)? 0:g_data01);
            g_data10 = (g_data10>255)? 255:((g_data10<0)? 0:g_data10);
            g_data11 = (g_data11>255)? 255:((g_data11<0)? 0:g_data11); 
            
            b_data00 = (b_data00>255)? 255:((b_data00<0)? 0:b_data00);
            b_data01 = (b_data01>255)? 255:((b_data01<0)? 0:b_data01);
            b_data10 = (b_data10>255)? 255:((b_data10<0)? 0:b_data10);
            b_data11 = (b_data11>255)? 255:((b_data11<0)? 0:b_data11);
            
            _dst_r[w] =
                    (((((r_data00 * cbufxi[w][0] + r_data01 * cbufxi[w][1]) >> 4) * cbufyi[h][0]) >> 16) +
                    ((((r_data10 * cbufxi[w][0] + r_data11 * cbufxi[w][1]) >> 4) * cbufyi[h][1]) >> 16) + 2) >> 2;

            _dst_g[w] =
                    (((((g_data00 * cbufxi[w][0] + g_data01 * cbufxi[w][1]) >> 4) * cbufyi[h][0]) >> 16) +
                    ((((g_data10 * cbufxi[w][0] + g_data11 * cbufxi[w][1]) >> 4) * cbufyi[h][1]) >> 16) + 2) >> 2;

            _dst_b[w] =
                    (((((b_data00 * cbufxi[w][0] + b_data01 * cbufxi[w][1]) >> 4) * cbufyi[h][0]) >> 16) +
                    ((((b_data10 * cbufxi[w][0] + b_data11 * cbufxi[w][1]) >> 4) * cbufyi[h][1]) >> 16) + 2) >> 2;

        }
    } 
}

void face_resize_scale_yuv2Y(u8* _src_y, s32 _src_h, s32 _src_w, s32 box_x, s32 box_y, 
        s32 _search_h, s32 _search_w, s32 _dst_h, s32 _dst_w, s16* _dst_y)
{
    s16 sx[_dst_w];
    s16 cbufxi[_dst_w][2];
    s16 sy[_dst_h];
    s16 cbufyi[_dst_h][2];
    
    face_resize_scale_adjust(_src_h,_src_w,_search_h,_search_w,_dst_h,_dst_w,box_x,box_y,
            &sx[0],&cbufxi[0][0],&sy[0],&cbufyi[0][0]);

    for (s32 h = 0; h < _dst_h; h++)
    {
    	unsigned char *y_src0,*y_src1;
		if(sy[h] == _src_h-1)
		{
			y_src0 = _src_y + sy[h]*_src_w;
			y_src1 = y_src0;
		}
		else
		{
			y_src0 = _src_y + sy[h]*_src_w;
			y_src1 = _src_y + (sy[h]+1)*_src_w;
		}
		
        for (s32 w = 0; w < _dst_w; w++)
        {
            s32 src_y00,src_y01,src_y10,src_y11;
			if(sy[h] == _src_h-1)
		    {
		    	src_y00 = (s32)y_src0[sx[w]];
	            src_y01 = (s32)y_src0[sx[w] + 1];
	            src_y10 = src_y00;
	            src_y11 = src_y01;
			}
		    else
		    {
	            src_y00 = (s32)y_src0[sx[w]];
	            src_y01 = (s32)y_src0[sx[w] + 1];
	            src_y10 = (s32)y_src1[sx[w]];
	            src_y11 = (s32)y_src1[sx[w] + 1];
		    }
            
            src_y00 = (src_y00 << 1) - 255;
            src_y01 = (src_y01 << 1) - 255;
            src_y10 = (src_y10 << 1) - 255;
            src_y11 = (src_y11 << 1) - 255;

            _dst_y[h * _dst_w + w] =
                    (((((src_y00 * cbufxi[w][0] + src_y01 * cbufxi[w][1]) >> 4) * cbufyi[h][0]) >> 16) +
                    ((((src_y10 * cbufxi[w][0] + src_y11 * cbufxi[w][1]) >> 4) * cbufyi[h][1]) >> 16) + 2) >> 2;   
        }
    }   
}  

void face_resize_scale_yuv2Y_no_normalize(u8* _src_y, s32 _src_h, s32 _src_w, s32 box_x, s32 box_y,
        s32 _search_h, s32 _search_w, s32 _dst_h, s32 _dst_w, u8* _dst_y) 
{
    s16 sx[_dst_w];
    s16 cbufxi[_dst_w][2];
    s16 sy[_dst_h];
    s16 cbufyi[_dst_h][2];
    
    face_resize_scale_adjust(_src_h,_src_w,_search_h,_search_w,_dst_h,_dst_w,box_x,box_y,
            &sx[0],&cbufxi[0][0],&sy[0],&cbufyi[0][0]);

    for (s32 h = 0; h < _dst_h; h++)
    {
		unsigned char *y_src0,*y_src1;
		if(sy[h] == _src_h-1)
		{
			y_src0 = _src_y + sy[h]*_src_w;
			y_src1 = y_src0;
		}
		else
		{
			y_src0 = _src_y + sy[h]*_src_w;
			y_src1 = _src_y + (sy[h]+1)*_src_w;
		}

        for (s32 w = 0; w < _dst_w; w++)
        {
            s32 src_y00,src_y01,src_y10,src_y11;
			if(sy[h] == _src_h-1)
		    {
		    	src_y00 = (s32)y_src0[sx[w]];
	            src_y01 = (s32)y_src0[sx[w] + 1];
	            src_y10 = src_y00;
	            src_y11 = src_y01;
			}
		    else
		    {
	            src_y00 = (s32)y_src0[sx[w]];
	            src_y01 = (s32)y_src0[sx[w] + 1];
	            src_y10 = (s32)y_src1[sx[w]];
	            src_y11 = (s32)y_src1[sx[w] + 1];
		    }

            _dst_y[h * _dst_w + w] =
                    (((((src_y00 * cbufxi[w][0] + src_y01 * cbufxi[w][1]) >> 4) * cbufyi[h][0]) >> 16) +
                    ((((src_y10 * cbufxi[w][0] + src_y11 * cbufxi[w][1]) >> 4) * cbufyi[h][1]) >> 16) + 2) >> 2;
        }

    } 
}
        
#endif
	
