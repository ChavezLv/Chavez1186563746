#include <stdlib.h>
#include "../arm_types.h"
#ifdef _ARM_NEON
#include <arm_neon.h>
#endif
#include "stn_if.h"
//#include "lib_print.h"

#ifdef _ARM_NEON
void gluon_stn_resize_float(float* _in, float* _out, float* _weight, s32 inchan, s32 inheight, s32 inwidth, s32 oheight, s32 owidth)
{
	double Matrix[6],fxy_temp;
	float d0,d1,d2;
	float s0,s1,s2,s3;
	s32 src_ich_off = inheight*inwidth;
	s32 dst_ich_off = oheight*owidth;
	
	//cout<<_weight[0]<<"  "<<_weight[1]<<"  "<<_weight[2]<<"  "
	//        <<_weight[3]<<"  "<<_weight[4]<<"  "<<_weight[5]<<"\r\n";
	Matrix[2] = (_weight[2] + 1 - _weight[0] - _weight[1]) * (inwidth - 1) * 0.5;
	Matrix[5] = (_weight[5] + 1 - _weight[3] - _weight[4]) * (inheight - 1) * 0.5;
	Matrix[0] = _weight[0] *  (inwidth - 1) /  (owidth - 1);
	Matrix[1] = _weight[1] *  (inwidth - 1) /  (oheight - 1);
	Matrix[3] = _weight[3] *  (inheight - 1) /  (owidth - 1);  
	Matrix[4] = _weight[4] *  (inheight - 1) /  (oheight - 1);
	//cout<<Matrix[0]<<"  "<<Matrix[1]<<"  "<<Matrix[2]<<"  "
	//        <<Matrix[3]<<"  "<<Matrix[4]<<"  "<<Matrix[5]<<"\r\n";
	
	//计算sx，fx0，fx1，sy,fy0,fy1
	s16* sx_ptr = (s16*)(_weight+8);
	s16* sx1_ptr = sx_ptr+dst_ich_off;
	s16* sy_ptr = sx1_ptr+dst_ich_off;
	s16* sy1_ptr = sy_ptr+dst_ich_off;
	float* fx0_ptr = (float*)(sy1_ptr+dst_ich_off);
	float* fx1_ptr = fx0_ptr + dst_ich_off;
	float* fy0_ptr = fx1_ptr + dst_ich_off;
	float* fy1_ptr = fy0_ptr + dst_ich_off;
	
	float fx,fy;
	float fxx[4],fyy[4];
	for(s32 i=0; i<oheight; i++)
	{
		s32 j=0;
		for(; j+3< owidth; j+=4)
		{
			//
			fxy_temp = j * Matrix[0] + i * Matrix[1] + Matrix[2];
			fxx[0] = fxy_temp;fxy_temp += Matrix[0];
			fxx[1] = fxy_temp;fxy_temp += Matrix[0];
			fxx[2] = fxy_temp;fxy_temp += Matrix[0];
			fxx[3] = fxy_temp;
			
			fxy_temp = j * Matrix[3] + i * Matrix[4] + Matrix[5];
			fyy[0] = fxy_temp;fxy_temp += Matrix[3];
			fyy[1] = fxy_temp;fxy_temp += Matrix[3];
			fyy[2] = fxy_temp;fxy_temp += Matrix[3];
			fyy[3] = fxy_temp;
			
			float32x4_t fx_data = vld1q_f32(fxx);
			float32x4_t fy_data = vld1q_f32(fyy);
			
			//
			float32x4_t all_ones_f = vdupq_n_f32(1.0);
			float32x4_t all_zeros_f = vdupq_n_f32(0.0);
			uint32x4_t cond = vcltq_f32(fx_data,all_zeros_f);
			float32x4_t sub_value_f = vbslq_f32(cond,all_ones_f,all_zeros_f);
			float32x4_t fx_data1 = vsubq_f32(fx_data,sub_value_f);
			cond = vcltq_f32(fy_data,all_zeros_f);
			sub_value_f = vbslq_f32(cond,all_ones_f,all_zeros_f);
			float32x4_t fy_data1 = vsubq_f32(fy_data,sub_value_f);
			
			int32x4_t all_ones = vdupq_n_s32(1);
			int32x4_t sx_data = vcvtq_s32_f32(fx_data1);
			int32x4_t sx1_data = vaddq_s32(sx_data,all_ones);
			float32x4_t sx_data_f = vcvtq_f32_s32(sx_data);
			int32x4_t sy_data = vcvtq_s32_f32(fy_data1);
			int32x4_t sy1_data = vaddq_s32(sy_data,all_ones);
			float32x4_t sy_data_f = vcvtq_f32_s32(sy_data);
								
			float32x4_t afx1_data = vsubq_f32(fx_data,sx_data_f);
			float32x4_t afx0_data = vsubq_f32(all_ones_f,afx1_data);
			float32x4_t afy1_data = vsubq_f32(fy_data,sy_data_f);
			float32x4_t afy0_data = vsubq_f32(all_ones_f,afy1_data);
			
			int32x4_t all_zeros = vdupq_n_s32(0);
			//x边界处理
			int32x4_t width_value = vdupq_n_s32(inwidth);
			cond = vcltq_s32(sx1_data,all_zeros);
			afx0_data = vbslq_f32(cond,all_zeros_f,afx0_data);
			afx1_data = vbslq_f32(cond,all_zeros_f,afx1_data);
			sx_data = vbslq_s32(cond,all_zeros,sx_data);
			sx1_data = vbslq_s32(cond,all_zeros,sx1_data);
			cond = vcltq_s32(sx_data,width_value);
			afx0_data = vbslq_f32(cond,afx0_data,all_zeros_f);
			afx1_data = vbslq_f32(cond,afx1_data,all_zeros_f);
			sx_data = vbslq_s32(cond,sx_data,all_zeros);
			sx1_data = vbslq_s32(cond,sx1_data,all_zeros);
			cond = vcltq_s32(sx_data,all_zeros);
			afx0_data = vbslq_f32(cond,all_zeros_f,afx0_data);
			sx_data = vbslq_s32(cond,all_zeros,sx_data);
			cond = vcltq_s32(sx1_data,width_value);
			afx1_data = vbslq_f32(cond,afx1_data,all_zeros_f);
			sx1_data = vbslq_s32(cond,sx1_data,all_zeros);
			
			//y边界处理
			int32x4_t height_value = vdupq_n_s32(inheight);
			cond = vcltq_s32(sy1_data,all_zeros);
			afy0_data = vbslq_f32(cond,all_zeros_f,afy0_data);
			afy1_data = vbslq_f32(cond,all_zeros_f,afy1_data);
			sy_data = vbslq_s32(cond,all_zeros,sy_data);
			sy1_data = vbslq_s32(cond,all_zeros,sy1_data);
			cond = vcltq_s32(sy_data,height_value);
			afy0_data = vbslq_f32(cond,afy0_data,all_zeros_f);
			afy1_data = vbslq_f32(cond,afy1_data,all_zeros_f);
			sy_data = vbslq_s32(cond,sy_data,all_zeros);
			sy1_data = vbslq_s32(cond,sy1_data,all_zeros);
			cond = vcltq_s32(sy_data,all_zeros);
			afy0_data = vbslq_f32(cond,all_zeros_f,afy0_data);
			sy_data = vbslq_s32(cond,all_zeros,sy_data);
			cond = vcltq_s32(sy1_data,height_value);
			afy1_data = vbslq_f32(cond,afy1_data,all_zeros_f);
			sy1_data = vbslq_s32(cond,sy1_data,all_zeros);
			
			vst1_s16(sx_ptr,vmovn_s32(sx_data));sx_ptr+=4;
			vst1_s16(sx1_ptr,vmovn_s32(sx1_data));sx1_ptr+=4;
			vst1_s16(sy_ptr,vmovn_s32(sy_data));sy_ptr+=4;
			vst1_s16(sy1_ptr,vmovn_s32(sy1_data));sy1_ptr+=4;
			vst1q_f32(fx0_ptr,afx0_data);fx0_ptr+=4;
			vst1q_f32(fx1_ptr,afx1_data);fx1_ptr+=4;
			vst1q_f32(fy0_ptr,afy0_data);fy0_ptr+=4;
			vst1q_f32(fy1_ptr,afy1_data);fy1_ptr+=4;
		}
		
		for(; j< owidth; j++)
		{                  
			//
			fx = j * Matrix[0] + i * Matrix[1] + Matrix[2];
			fy = j * Matrix[3] + i * Matrix[4] + Matrix[5];

			if(fx < 0)
					*sx_ptr = (s16)(fx - 1);
			else
					*sx_ptr = (s16)(fx);
			*sx1_ptr = *sx_ptr+1;

			if(fy < 0)
					*sy_ptr = (s16)(fy - 1);
			else
					*sy_ptr = (s16)(fy);
			*sy1_ptr = *sy_ptr+1;

			fx = fx - *sx_ptr;
			fy = fy - *sy_ptr;
			*fx0_ptr = (1-fx);
			*fx1_ptr = fx;
			*fy0_ptr = (1-fy);
			*fy1_ptr = fy;                    
			
			//x边界处理
			if((*sx1_ptr < 0) || (*sx_ptr >= inwidth))
			{
				*fx0_ptr = 0;
				*fx1_ptr = 0;
				*sx_ptr = 0;
				*sx1_ptr = 0;
			}
			else if(*sx_ptr < 0)
			{
				*fx0_ptr = 0;
				*sx_ptr = 0;
			}
			else if(*sx1_ptr >= inwidth)
			{
				*fx1_ptr = 0;
				*sx1_ptr = 0;
			}
			
			//y边界处理
			if((*sy1_ptr < 0) || (*sy_ptr >= inheight))
			{
				*fy0_ptr = 0;
				*fy1_ptr = 0;
				*sy_ptr = 0;
				*sy1_ptr = 0;
			}
			else if(*sy_ptr < 0)
			{
				*fy0_ptr = 0;
				*sy_ptr = 0;
			}
			else if(*sy1_ptr >= inheight)
			{
				*fy1_ptr = 0;
				*sy1_ptr = 0;
			}
			
			sx_ptr++;
			sx1_ptr++;
			sy_ptr++;
			sy1_ptr++;
			fx0_ptr++;
			fx1_ptr++;
			fy0_ptr++;
			fy1_ptr++;
		}
	}

	for(s32 i=0; i<inchan; i++)
	{
		float* src_data = _in + i*src_ich_off;
		float* dst_data = _out + i*dst_ich_off;
		
		sx_ptr = (s16*)(_weight+8);
		sx1_ptr = sx_ptr+dst_ich_off;
		sy_ptr = sx1_ptr+dst_ich_off;
		sy1_ptr = sy_ptr+dst_ich_off;
		fx0_ptr = (float*)(sy1_ptr+dst_ich_off);
		fx1_ptr = fx0_ptr + dst_ich_off;
		fy0_ptr = fx1_ptr + dst_ich_off;
		fy1_ptr = fy0_ptr + dst_ich_off;
		
		s32 j=0;
		for(; j+3< dst_ich_off; j+=4)
		{
			s32 idx00 = *sy_ptr * inwidth + *sx_ptr;
			s32 idx01 = *sy_ptr++ * inwidth + *sx1_ptr;
			s32 idy00 = *sy1_ptr * inwidth + *sx_ptr++;
			s32 idy01 = *sy1_ptr++ * inwidth + *sx1_ptr++;
			s32 idx10 = *sy_ptr * inwidth + *sx_ptr;
			s32 idx11 = *sy_ptr++ * inwidth + *sx1_ptr;
			s32 idy10 = *sy1_ptr * inwidth + *sx_ptr++;
			s32 idy11 = *sy1_ptr++ * inwidth + *sx1_ptr++;
			s32 idx20 = *sy_ptr * inwidth + *sx_ptr;
			s32 idx21 = *sy_ptr++ * inwidth + *sx1_ptr;
			s32 idy20 = *sy1_ptr * inwidth + *sx_ptr++;
			s32 idy21 = *sy1_ptr++ * inwidth + *sx1_ptr++;
			s32 idx30 = *sy_ptr * inwidth + *sx_ptr;
			s32 idx31 = *sy_ptr++ * inwidth + *sx1_ptr;
			s32 idy30 = *sy1_ptr * inwidth + *sx_ptr++;
			s32 idy31 = *sy1_ptr++ * inwidth + *sx1_ptr++;
			
			float32x4_t src_s00,src_s01,src_s10,src_s11;
			src_s00 = vld1q_lane_f32(src_data+idx00,src_s00,0);
			src_s00 = vld1q_lane_f32(src_data+idx10,src_s00,1);
			src_s00 = vld1q_lane_f32(src_data+idx20,src_s00,2);
			src_s00 = vld1q_lane_f32(src_data+idx30,src_s00,3);
			src_s01 = vld1q_lane_f32(src_data+idx01,src_s01,0);
			src_s01 = vld1q_lane_f32(src_data+idx11,src_s01,1);
			src_s01 = vld1q_lane_f32(src_data+idx21,src_s01,2);
			src_s01 = vld1q_lane_f32(src_data+idx31,src_s01,3);
			
			src_s10 = vld1q_lane_f32(src_data+idy00,src_s10,0);
			src_s10 = vld1q_lane_f32(src_data+idy10,src_s10,1);
			src_s10 = vld1q_lane_f32(src_data+idy20,src_s10,2);
			src_s10 = vld1q_lane_f32(src_data+idy30,src_s10,3);
			src_s11 = vld1q_lane_f32(src_data+idy01,src_s11,0);
			src_s11 = vld1q_lane_f32(src_data+idy11,src_s11,1);
			src_s11 = vld1q_lane_f32(src_data+idy21,src_s11,2);
			src_s11 = vld1q_lane_f32(src_data+idy31,src_s11,3);
			
			float32x4_t fx0_data = vld1q_f32(fx0_ptr);fx0_ptr+=4;
			float32x4_t fx1_data = vld1q_f32(fx1_ptr);fx1_ptr+=4;
			float32x4_t data_d0 = vmulq_f32(src_s00,fx0_data);
			data_d0 = vmlaq_f32(data_d0,src_s01,fx1_data);
			float32x4_t data_d1 = vmulq_f32(src_s10,fx0_data);
			data_d1 = vmlaq_f32(data_d1,src_s11,fx1_data);
			
			float32x4_t fy0_data = vld1q_f32(fy0_ptr);fy0_ptr+=4;
			float32x4_t fy1_data = vld1q_f32(fy1_ptr);fy1_ptr+=4;
			float32x4_t data_d2 = vmulq_f32(data_d0,fy0_data);
			data_d2 = vmlaq_f32(data_d2,data_d1,fy1_data);
			
			vst1q_f32(dst_data,data_d2);dst_data+=4;
		}
		
		for(; j< dst_ich_off; j++)
		{                  
			s0 = *(src_data+*sy_ptr * inwidth + *sx_ptr);
			s1 = *(src_data+*sy_ptr * inwidth + *sx1_ptr);
			s2 = *(src_data+*sy1_ptr * inwidth + *sx_ptr);
			s3 = *(src_data+*sy1_ptr * inwidth + *sx1_ptr);
			
			d0 = s0 * *fx0_ptr + s1 * *fx1_ptr;
			d1 = s2 * *fx0_ptr + s3 * *fx1_ptr;
			d2 = d0 * *fy0_ptr + d1 * *fy1_ptr;

			*dst_data++ = d2;  

			sx_ptr++;
			sx1_ptr++;
			sy_ptr++;
			sy1_ptr++;
			fx0_ptr++;
			fx1_ptr++;
			fy0_ptr++;
			fy1_ptr++;
		}
	}
}

void gluon_stn_resize_fix(u8* _in, u8* _out, s16* _weight, s32 inchan, s32 inheight, s32 inwidth, s32 oheight, s32 owidth)
{
	double Matrix[6],fxy_temp;
	float d0,d1,d2;
	s16 s0,s1,s2,s3;
	s32 src_ich_off = inheight*inwidth;
	s32 dst_ich_off = oheight*owidth;
	
	for(s32 i = 0; i < 6; i++)
	{
		Matrix[i] = _weight[i] * 0.000244140625;//div 4096
	}
	Matrix[2] = (Matrix[2] + 1 - Matrix[0] - Matrix[1]) * (inwidth - 1) * 0.5;
	Matrix[5] = (Matrix[5] + 1 - Matrix[3] - Matrix[4]) * (inheight - 1) * 0.5;
	Matrix[0] = Matrix[0] *  (inwidth - 1) /  (owidth - 1);
	Matrix[1] = Matrix[1] *  (inwidth - 1) /  (oheight - 1);
	Matrix[3] = Matrix[3] *  (inheight - 1) /  (owidth - 1);  
	Matrix[4] = Matrix[4] *  (inheight - 1) /  (oheight - 1);
	//cout<<Matrix[0]<<"  "<<Matrix[1]<<"  "<<Matrix[2]<<"  "
	//        <<Matrix[3]<<"  "<<Matrix[4]<<"  "<<Matrix[5]<<"\r\n";
   
	//计算sx，fx0，fx1，sy,fy0,fy1
  //LIB_ALGO_PRINT("1 : %d\n", (dst_ich_off*4*(sizeof(s16)+sizeof(float))));
	s16* tempbuf = (s16*)malloc(dst_ich_off*4*(sizeof(s16)+sizeof(float)));
  //LIB_ALGO_PRINT("2 - %p\n", tempbuf);
	s16* sx_ptr = tempbuf;
	s16* sx1_ptr = sx_ptr+dst_ich_off;
	s16* sy_ptr = sx1_ptr+dst_ich_off;
	s16* sy1_ptr = sy_ptr+dst_ich_off;
	float* fx0_ptr = (float*)(sy1_ptr+dst_ich_off);
	float* fx1_ptr = fx0_ptr + dst_ich_off;
	float* fy0_ptr = fx1_ptr + dst_ich_off;
	float* fy1_ptr = fy0_ptr + dst_ich_off;
	
	float fx,fy;
	float fxx[4],fyy[4];
	for(s32 i=0; i<oheight; i++)
	{
		s32 j=0;
		for(; j+3< owidth; j+=4)
		{
			//
			fxy_temp = j * Matrix[0] + i * Matrix[1] + Matrix[2];
			fxx[0] = fxy_temp;fxy_temp += Matrix[0];
			fxx[1] = fxy_temp;fxy_temp += Matrix[0];
			fxx[2] = fxy_temp;fxy_temp += Matrix[0];
			fxx[3] = fxy_temp;
			
			fxy_temp = j * Matrix[3] + i * Matrix[4] + Matrix[5];
			fyy[0] = fxy_temp;fxy_temp += Matrix[3];
			fyy[1] = fxy_temp;fxy_temp += Matrix[3];
			fyy[2] = fxy_temp;fxy_temp += Matrix[3];
			fyy[3] = fxy_temp;
			
			float32x4_t fx_data = vld1q_f32(fxx);
			float32x4_t fy_data = vld1q_f32(fyy);
			
			//
			float32x4_t all_ones_f = vdupq_n_f32(1.0);
			float32x4_t all_zeros_f = vdupq_n_f32(0.0);
			uint32x4_t cond = vcltq_f32(fx_data,all_zeros_f);
			float32x4_t sub_value_f = vbslq_f32(cond,all_ones_f,all_zeros_f);
			float32x4_t fx_data1 = vsubq_f32(fx_data,sub_value_f);
			cond = vcltq_f32(fy_data,all_zeros_f);
			sub_value_f = vbslq_f32(cond,all_ones_f,all_zeros_f);
			float32x4_t fy_data1 = vsubq_f32(fy_data,sub_value_f);
			
			int32x4_t sx_data = vcvtq_s32_f32(fx_data1);
			float32x4_t sx_data_f = vcvtq_f32_s32(sx_data);
			int32x4_t sy_data = vcvtq_s32_f32(fy_data1);
			float32x4_t sy_data_f = vcvtq_f32_s32(sy_data);
								
			float32x4_t afx1_data = vsubq_f32(fx_data,sx_data_f);
			float32x4_t afx0_data = vsubq_f32(all_ones_f,afx1_data);
			float32x4_t afy1_data = vsubq_f32(fy_data,sy_data_f);
			float32x4_t afy0_data = vsubq_f32(all_ones_f,afy1_data);
			
			int32x4_t all_zeros = vdupq_n_s32(0);
			int32x4_t all_ones = vdupq_n_s32(1);
			//x边界处理
			int32x4_t width_value = vdupq_n_s32(inwidth);
			int32x4_t sx_data1 = vaddq_s32(sx_data,all_ones);
			cond = vcltq_s32(sx_data1,all_zeros);
			afx0_data = vbslq_f32(cond,all_zeros_f,afx0_data);
			afx1_data = vbslq_f32(cond,all_zeros_f,afx1_data);
			sx_data = vbslq_s32(cond,all_zeros,sx_data);
			sx_data1 = vbslq_s32(cond,all_zeros,sx_data1);
			cond = vcltq_s32(sx_data,width_value);
			afx0_data = vbslq_f32(cond,afx0_data,all_zeros_f);
			afx1_data = vbslq_f32(cond,afx1_data,all_zeros_f);
			sx_data = vbslq_s32(cond,sx_data,all_zeros);
			sx_data1 = vbslq_s32(cond,sx_data1,all_zeros);
			cond = vcltq_s32(sx_data,all_zeros);
			afx0_data = vbslq_f32(cond,all_zeros_f,afx0_data);
			sx_data = vbslq_s32(cond,all_zeros,sx_data);
			cond = vcltq_s32(sx_data1,width_value);
			afx1_data = vbslq_f32(cond,afx1_data,all_zeros_f);
			sx_data1 = vbslq_s32(cond,sx_data1,all_zeros);
			
			//y边界处理
			int32x4_t height_value = vdupq_n_s32(inheight);
			int32x4_t sy_data1 = vaddq_s32(sy_data,all_ones);
			cond = vcltq_s32(sy_data1,all_zeros);
			afy0_data = vbslq_f32(cond,all_zeros_f,afy0_data);
			afy1_data = vbslq_f32(cond,all_zeros_f,afy1_data);
			sy_data = vbslq_s32(cond,all_zeros,sy_data);
			sy_data1 = vbslq_s32(cond,all_zeros,sy_data1);
			cond = vcltq_s32(sy_data,height_value);
			afy0_data = vbslq_f32(cond,afy0_data,all_zeros_f);
			afy1_data = vbslq_f32(cond,afy1_data,all_zeros_f);
			sy_data = vbslq_s32(cond,sy_data,all_zeros);
			sy_data1 = vbslq_s32(cond,sy_data1,all_zeros);
			cond = vcltq_s32(sy_data,all_zeros);
			afy0_data = vbslq_f32(cond,all_zeros_f,afy0_data);
			sy_data = vbslq_s32(cond,all_zeros,sy_data);
			cond = vcltq_s32(sy_data1,height_value);
			afy1_data = vbslq_f32(cond,afy1_data,all_zeros_f);
			sy_data1 = vbslq_s32(cond,sy_data1,all_zeros);
			
			vst1_s16(sx_ptr,vmovn_s32(sx_data));sx_ptr+=4;
			vst1_s16(sx1_ptr,vmovn_s32(sx_data1));sx1_ptr+=4;
			vst1_s16(sy_ptr,vmovn_s32(sy_data));sy_ptr+=4;
			vst1_s16(sy1_ptr,vmovn_s32(sy_data1));sy1_ptr+=4;
			vst1q_f32(fx0_ptr,afx0_data);fx0_ptr+=4;
			vst1q_f32(fx1_ptr,afx1_data);fx1_ptr+=4;
			vst1q_f32(fy0_ptr,afy0_data);fy0_ptr+=4;
			vst1q_f32(fy1_ptr,afy1_data);fy1_ptr+=4;
		}
		
		for(; j< owidth; j++)
		{                  
			//
			fx = j * Matrix[0] + i * Matrix[1] + Matrix[2];
			fy = j * Matrix[3] + i * Matrix[4] + Matrix[5];

			if(fx < 0)
					*sx_ptr = (s16)(fx - 1);
			else
					*sx_ptr = (s16)(fx);
			*sx1_ptr = *sx_ptr + 1;

			if(fy < 0)
					*sy_ptr = (s16)(fy - 1);
			else
					*sy_ptr = (s16)(fy);
			*sy1_ptr = *sy_ptr + 1;

			fx = fx - *sx_ptr;
			fy = fy - *sy_ptr;
			*fx0_ptr = (1-fx);
			*fx1_ptr = fx;
			*fy0_ptr = (1-fy);
			*fy1_ptr = fy;                    
			
			//x边界处理
			if((*sx1_ptr < 0) || (*sx_ptr >= inwidth))
			{
				*fx0_ptr = 0;
				*fx1_ptr = 0;
				*sx_ptr = 0;
				*sx1_ptr = 0;
			}
			else if(*sx_ptr < 0)
			{
				*fx0_ptr = 0;
				*sx_ptr = 0;
			}
			else if(*sx1_ptr >= inwidth)
			{
				*fx1_ptr = 0;
				*sx1_ptr = 0;
			}
			
			//y边界处理
			if((*sy1_ptr < 0) || (*sy_ptr >= inheight))
			{
				*fy0_ptr = 0;
				*fy1_ptr = 0;
				*sy_ptr = 0;
				*sy1_ptr = 0;
			}
			else if(*sy_ptr < 0)
			{
				*fy0_ptr = 0;
				*sy_ptr = 0;
			}
			else if(*sy1_ptr >= inheight)
			{
				*fy1_ptr = 0;
				*sy1_ptr = 0;
			}
			
			sx_ptr++;
			sx1_ptr++;
			sy_ptr++;
			sy1_ptr++;
			fx0_ptr++;
			fx1_ptr++;
			fy0_ptr++;
			fy1_ptr++;
		}
	}

	for(s32 i=0; i<inchan; i++)
	{
		u8* src_data = _in + i*src_ich_off;
		u8* dst_data = _out + i*dst_ich_off;
		
		sx_ptr = tempbuf;
		sx1_ptr = sx_ptr+dst_ich_off;
		sy_ptr = sx1_ptr+dst_ich_off;
		sy1_ptr = sy_ptr+dst_ich_off;
		fx0_ptr = (float*)(sy1_ptr+dst_ich_off);
		fx1_ptr = fx0_ptr + dst_ich_off;
		fy0_ptr = fx1_ptr + dst_ich_off;
		fy1_ptr = fy0_ptr + dst_ich_off;
		
		s32 j=0;
		for(; j+3< dst_ich_off; j+=4)
		{
			s32 idx00 = *sy_ptr * inwidth + *sx_ptr;
			s32 idx01 = *sy_ptr++ * inwidth + *sx1_ptr;
			s32 idy00 = *sy1_ptr * inwidth + *sx_ptr++;
			s32 idy01 = *sy1_ptr++ * inwidth + *sx1_ptr++;
			s32 idx10 = *sy_ptr * inwidth + *sx_ptr;
			s32 idx11 = *sy_ptr++ * inwidth + *sx1_ptr;
			s32 idy10 = *sy1_ptr * inwidth + *sx_ptr++;
			s32 idy11 = *sy1_ptr++ * inwidth + *sx1_ptr++;
			s32 idx20 = *sy_ptr * inwidth + *sx_ptr;
			s32 idx21 = *sy_ptr++ * inwidth + *sx1_ptr;
			s32 idy20 = *sy1_ptr * inwidth + *sx_ptr++;
			s32 idy21 = *sy1_ptr++ * inwidth + *sx1_ptr++;
			s32 idx30 = *sy_ptr * inwidth + *sx_ptr;
			s32 idx31 = *sy_ptr++ * inwidth + *sx1_ptr;
			s32 idy30 = *sy1_ptr * inwidth + *sx_ptr++;
			s32 idy31 = *sy1_ptr++ * inwidth + *sx1_ptr++;
			
			uint8x8_t src_s0,src_s1;
			src_s0 = vld1_lane_u8(src_data+idx00,src_s0,0);
			src_s0 = vld1_lane_u8(src_data+idx10,src_s0,1);
			src_s0 = vld1_lane_u8(src_data+idx20,src_s0,2);
			src_s0 = vld1_lane_u8(src_data+idx30,src_s0,3);
			src_s0 = vld1_lane_u8(src_data+idx01,src_s0,4);
			src_s0 = vld1_lane_u8(src_data+idx11,src_s0,5);
			src_s0 = vld1_lane_u8(src_data+idx21,src_s0,6);
			src_s0 = vld1_lane_u8(src_data+idx31,src_s0,7);
			
			src_s1 = vld1_lane_u8(src_data+idy00,src_s1,0);
			src_s1 = vld1_lane_u8(src_data+idy10,src_s1,1);
			src_s1 = vld1_lane_u8(src_data+idy20,src_s1,2);
			src_s1 = vld1_lane_u8(src_data+idy30,src_s1,3);
			src_s1 = vld1_lane_u8(src_data+idy01,src_s1,4);
			src_s1 = vld1_lane_u8(src_data+idy11,src_s1,5);
			src_s1 = vld1_lane_u8(src_data+idy21,src_s1,6);
			src_s1 = vld1_lane_u8(src_data+idy31,src_s1,7);
			
			int16x4_t src_s00,src_s01,src_s10,src_s11;
			int16x8_t src_tmp0 = vreinterpretq_s16_u16(vmovl_u8(src_s0));
			int16x8_t src_tmp1 = vreinterpretq_s16_u16(vmovl_u8(src_s1));
			src_s00 = vget_low_s16(src_tmp0);
			src_s01 = vget_high_s16(src_tmp0);
			src_s10 = vget_low_s16(src_tmp1);
			src_s11 = vget_high_s16(src_tmp1);
			
			float32x4_t fx0_data = vld1q_f32(fx0_ptr);fx0_ptr+=4;
			float32x4_t fx1_data = vld1q_f32(fx1_ptr);fx1_ptr+=4;
			float32x4_t src_s00_f = vcvtq_f32_s32(vmovl_s16(src_s00));
			float32x4_t src_s01_f = vcvtq_f32_s32(vmovl_s16(src_s01));
			float32x4_t data_d0 = vmulq_f32(src_s00_f,fx0_data);
			data_d0 = vmlaq_f32(data_d0,src_s01_f,fx1_data);
			
			float32x4_t src_s10_f = vcvtq_f32_s32(vmovl_s16(src_s10));
			float32x4_t src_s11_f = vcvtq_f32_s32(vmovl_s16(src_s11));
			float32x4_t data_d1 = vmulq_f32(src_s10_f,fx0_data);
			data_d1 = vmlaq_f32(data_d1,src_s11_f,fx1_data);
			
			float32x4_t fy0_data = vld1q_f32(fy0_ptr);fy0_ptr+=4;
			float32x4_t fy1_data = vld1q_f32(fy1_ptr);fy1_ptr+=4;
			float32x4_t data_d2 = vmulq_f32(data_d0,fy0_data);
			data_d2 = vmlaq_f32(data_d2,data_d1,fy1_data);
			
			float32x4_t all_zeros_f = vdupq_n_f32(0.0);
			float32x4_t all_half0_f = vdupq_n_f32(-0.5);
			float32x4_t all_half1_f = vdupq_n_f32(0.5);
			uint32x4_t cond = vcltq_f32(data_d2,all_zeros_f);
			float32x4_t round_data = vbslq_f32(cond,all_half0_f,all_half1_f);
			
			data_d2 = vaddq_f32(data_d2,round_data);
			int32x4_t data_d2_temp = vcvtq_s32_f32(data_d2);
			
			//vst1_s16(dst_data,vmovn_s32(data_d2_temp));dst_data+=4;
			int16x4_t out_data = vmovn_s32(data_d2_temp);
			*dst_data++ = (u8)vget_lane_s16(out_data, 0);
			*dst_data++ = (u8)vget_lane_s16(out_data, 1);
			*dst_data++ = (u8)vget_lane_s16(out_data, 2);
			*dst_data++ = (u8)vget_lane_s16(out_data, 3);
		}
		
		for(; j< dst_ich_off; j++)
		{                  
			s0 = *(src_data + *sy_ptr * inwidth + *sx_ptr);
			s1 = *(src_data + *sy_ptr * inwidth + *sx1_ptr);
			s2 = *(src_data + *sy1_ptr * inwidth + *sx_ptr);
			s3 = *(src_data + *sy1_ptr * inwidth + *sx1_ptr);

			d0 = s0 * *fx0_ptr + s1 * *fx1_ptr;
			d1 = s2 * *fx0_ptr + s3 * *fx1_ptr;
			d2 = d0 * *fy0_ptr + d1 * *fy1_ptr;

			*dst_data++ = (d2 < 0) ? (s16)(d2-0.5):(s16)(d2+0.5);  

			sx_ptr++;
			sx1_ptr++;
			sy_ptr++;
			sy1_ptr++;
			fx0_ptr++;
			fx1_ptr++;
			fy0_ptr++;
			fy1_ptr++;
		}
	}  
}
#else
void gluon_stn_resize_float(f32* _in, f32* _out, f32* _weight, s32 inchan, s32 inheight,
	s32 inwidth, s32 oheight, s32 owidth)
{
    double Matrix[6];
    double fx,fy,d0,d1,d2;
    s16 sx,sy;
    float s0,s1,s2,s3;
    s32 src_ich_off = inheight*inwidth;
    s32 dst_ich_off = oheight*owidth;
    
    for(s32 i = 0; i < 6; i++)
    {
		Matrix[i] = _weight[i] * 0.000244140625;//�?4096
    }
    Matrix[2] = (Matrix[2] + 1 - Matrix[0] - Matrix[1]) * (inwidth - 1) * 0.5;
    Matrix[5] = (Matrix[5] + 1 - Matrix[3] - Matrix[4]) * (inheight - 1) * 0.5;
    Matrix[0] = Matrix[0] *  (inwidth - 1) /  (owidth - 1);
    Matrix[1] = Matrix[1] *  (inwidth - 1) /  (oheight - 1);
    Matrix[3] = Matrix[3] *  (inheight - 1) /  (owidth - 1);  
    Matrix[4] = Matrix[4] *  (inheight - 1) /  (oheight - 1);
//            cout<<Matrix[0]<<"  "<<Matrix[1]<<"  "<<Matrix[2]<<"  "
//                    <<Matrix[3]<<"  "<<Matrix[4]<<"  "<<Matrix[5]<<"\r\n";
    for(s32 i=0; i<inchan; i++)
    {
        float* src_data = _in + i*src_ich_off;
        float* dst_data = _out + i*dst_ich_off;
        for(s32 j=0; j< oheight; j++)
        {                  
            for(s32 k=0; k< owidth; k++)
            {
                s0 = 0;
                s1 = 0;
                s2 = 0;
                s3 = 0;

                //
                fx = k * Matrix[0] + j * Matrix[1] + Matrix[2];
                fy = k * Matrix[3] + j * Matrix[4] + Matrix[5];

                if(fx < 0)
                    sx = (s16)(fx - 1);
                else
                    sx = (s16)(fx);

                if(fy < 0)
                    sy = (s16)(fy - 1);
                else
                    sy = (s16)(fy);

                fx = fx - sx;
                fy = fy - sy; 	
	
                if(sx >= 0 && sx < inwidth && sy >= 0 && sy < inheight)
                    s0 = src_data[sy * inwidth + sx];
				if(sx + 1 >= 0 && sx + 1 < inwidth && sy >= 0 && sy < inheight)
			        s1 = src_data[sy * inwidth + sx + 1];
				if(sx >= 0 && sx < inwidth && sy + 1 >= 0 && sy + 1 < inheight)
			        s2 = src_data[(sy + 1) * inwidth + sx];
				if(sx + 1 >= 0 && sx + 1 < inwidth && sy + 1 >= 0 && sy + 1 < inheight)
			        s3 = src_data[(sy + 1) * inwidth + sx + 1];

				d0 = s0 * (1 - fx) + s1 * fx;
				d1 = s2 * (1 - fx) + s3 * fx;
				d2 = d0 * (1 - fy) + d1 * fy;

				*dst_data++ = (float)d2; 
            }    
        }
    }
}

void gluon_stn_resize_fix(u8* _in, u8* _out, s16* _weight, s32 inchan, s32 inheight,
	s32 inwidth, s32 oheight, s32 owidth)
{
    double Matrix[6];
    double fx,fy,d0,d1,d2;
    s16 sx,sy;
    s16 s0,s1,s2,s3;
    s32 src_ich_off = inheight*inwidth;
    s32 dst_ich_off = oheight*owidth;
    
    for(s32 i = 0; i < 6; i++)
    {
		Matrix[i] = _weight[i] * 0.000244140625;//�?4096
    }
    Matrix[2] = (Matrix[2] + 1 - Matrix[0] - Matrix[1]) * (inwidth - 1) * 0.5;
    Matrix[5] = (Matrix[5] + 1 - Matrix[3] - Matrix[4]) * (inheight - 1) * 0.5;
    Matrix[0] = Matrix[0] *  (inwidth - 1) /  (owidth - 1);
    Matrix[1] = Matrix[1] *  (inwidth - 1) /  (oheight - 1);
    Matrix[3] = Matrix[3] *  (inheight - 1) /  (owidth - 1);  
    Matrix[4] = Matrix[4] *  (inheight - 1) /  (oheight - 1);
//            cout<<Matrix[0]<<"  "<<Matrix[1]<<"  "<<Matrix[2]<<"  "
//                    <<Matrix[3]<<"  "<<Matrix[4]<<"  "<<Matrix[5]<<"\r\n";
    for(s32 i=0; i<inchan; i++)
    {
        u8* src_data = _in + i*src_ich_off;
        u8* dst_data = _out + i*dst_ich_off;
        for(s32 j=0; j< oheight; j++)
        {                  
            for(s32 k=0; k< owidth; k++)
            {
                s0 = 0;
                s1 = 0;
                s2 = 0;
                s3 = 0;

                //
                fx = k * Matrix[0] + j * Matrix[1] + Matrix[2];
                fy = k * Matrix[3] + j * Matrix[4] + Matrix[5];

                if(fx < 0)
                        sx = (s16)(fx - 1);
                else
                        sx = (s16)(fx);

                if(fy < 0)
                        sy = (s16)(fy - 1);
                else
                        sy = (s16)(fy);

                fx = fx - sx;
                fy = fy - sy; 	
	
                if(sx >= 0 && sx < inwidth && sy >= 0 && sy < inheight)
                    s0 = src_data[sy * inwidth + sx];
				if(sx + 1 >= 0 && sx + 1 < inwidth && sy >= 0 && sy < inheight)
			                    s1 = src_data[sy * inwidth + sx + 1];
				if(sx >= 0 && sx < inwidth && sy + 1 >= 0 && sy + 1 < inheight)
			                    s2 = src_data[(sy + 1) * inwidth + sx];
				if(sx + 1 >= 0 && sx + 1 < inwidth && sy + 1 >= 0 && sy + 1 < inheight)
			                    s3 = src_data[(sy + 1) * inwidth + sx + 1];

				d0 = s0 * (1 - fx) + s1 * fx;
				d1 = s2 * (1 - fx) + s3 * fx;
				d2 = d0 * (1 - fy) + d1 * fy;

				*dst_data++ = (d2 < 0) ? (s16)(d2-0.5):(s16)(d2+0.5); 
            }    
        }
    }
}
        
#endif
		
