//#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sched.h>
#include <pthread.h>
#include "arm_types.h"
#include "nms/nms_if.h"
#include "../if_algo_api.h"
#include "arm_api.h"
#include "arm_api_intra.h"
#include "lk_track/slk_tracker_if.h"
#include "resize/resize.h"
#include "stn/stn_if.h"


void if_data_transform_align16(u8* datain, u8* dataout, s16 chan, s16 height, s16 width)
{
    if((0==(width&0xf)))
    {
    	memcpy(dataout,datain,chan*height*width);
    }
	else
	{
		s16 wstep = (width&0xfff0)+16;
		s32 loop0 = chan*height;

	    memset(dataout,0,loop0*wstep);
		
		s32 i=0;
		for(;i<loop0;i++)
		{
			memcpy(dataout,datain,width);
			dataout+=wstep;
			datain+=width;
		}
	}
}

void if_data_transform_noalign16(u8* datain, u8* dataout, s16 chan, s16 height, s16 width)
{
    if((0==(width&0xf)))
    {
    	memcpy(dataout,datain,chan*height*width);
    }
	else
	{
		s16 wstep = (width&0xfff0)+16;
		s32 loop0 = chan*height;
		
		s32 i=0;
		for(;i<loop0;i++)
		{
			memcpy(dataout,datain,width);
			dataout+=width;
			datain+=wstep;
		}
	}
}

#ifdef _GNU_SOURCE
void cpu_res_alloc(s16 total_num, s16* cpu_id, s16* group_size, s16* group_num)
{
    s16 group_num0 = 0;
	s16 a53_num = (total_num/3)&0xfffe;// 2 a53
	if(5 == total_num) a53_num = 1;
	s16 a73_num = (total_num - a53_num);// 2 a73
	
	if(0!=a53_num)
	{
	    if(a53_num>1)
	    {
		    group_size[group_num0] = (a53_num>>1);
			cpu_id[group_num0] = 0;
			group_num0++;
	    }
		group_size[group_num0] = a53_num - (a53_num>>1);
		cpu_id[group_num0] = 1;
	    group_num0++;
	}

	if(0!=a73_num)
	{
		if(a73_num>1)
	    {
		    group_size[group_num0] = (a73_num>>1);
			cpu_id[group_num0] = 2;
			group_num0++;
		}
		group_size[group_num0] = a73_num - (a73_num>>1);
		cpu_id[group_num0] = 3;
	    group_num0++;
	}

	*group_num = group_num0;
}
#endif

void SVP_ARM_FaceTrack_Thread(void* mPara, s32 min_obj_size)
{
    t_track_para* pParaTmp = (t_track_para*)mPara;
    s32 i = 0;
    s16 *pIn  = (s16 *)pParaTmp->BoxIn;
	s16 *pId  = (s16 *)pParaTmp->BoxId;
    s16 *pOut = (s16 *)pParaTmp->BoxOut;
	s16 *pOutId = (s16 *)pParaTmp->BoxOutId;
	s16 out_face_num = 0;
	s16 cpu_id = pParaTmp->cpu_id;

#ifdef _GNU_SOURCE	
	cpu_set_t mask;
	CPU_ZERO(&mask);
    CPU_SET(cpu_id,&mask);
    if(pthread_setaffinity_np(pthread_self(), sizeof(mask),&mask)< 0){
        printf("set thread affinity failed!\n");
    }
#endif

#if ARM_PROFILE_ON
    struct timespec timestart = {0};
    struct timespec timeend = {0};        

    clock_gettime(CLOCK_MONOTONIC, &timestart);   
#endif

    for(i = 0; i < pParaTmp->BoxInNum; i++)
    {
        //printf("juwneqi-----002,%d,%d,%d,%d\n",pIn[0],pIn[1],pIn[2],pIn[3]);
        if(true == slk_tracker(pIn, pOut, (u8 *)pParaTmp->ImgPre, (u8 *)pParaTmp->ImgCur, pParaTmp->Img_H, pParaTmp->Img_W, min_obj_size))
        {
        	pOut += 4;
			*pOutId++ = pId[i];
			out_face_num++;
        }else{
            //pOut[0]=pIn[0];
           // pOut[1]=pIn[1];
            //pOut[2]=pIn[2];
            //pOut[3]=pIn[3];
        	//pOut += 4;
			//*pOutId++ = pId[i];
			//out_face_num++;
        }
        pIn  += 4;
    }

    pParaTmp->BoxOutNum = out_face_num;

#if ARM_PROFILE_ON
    clock_gettime(CLOCK_MONOTONIC, &timeend);
    printf("arm track thread%d time = %f ms!\n", cpu_id, (timeend.tv_sec - timestart.tv_sec) * 1000 + (float)(timeend.tv_nsec - timestart.tv_nsec) / 1000000);
#endif
}

s32 SVP_ARM_FaceTrack(u64 ImgPre, u64 ImgCur, void *BoxIn, void *BoxId, void *BoxOut, void *BoxOutId, 
	s16 BoxInNum, s16* BoxOutNum, s16 Img_H, s16 Img_W, s32 min_obj_size)
{
    int i = 0;
    s16 group_num = 0;// (BoxInNum>16) ? ARM_THREAD_MAX:((BoxInNum + 3) >> 2);
    s16 group_size[4] = {0,};//(BoxInNum + group_num -1)/group_num;

    s16 cpu_id[4] = {0,};
#ifdef _GNU_SOURCE
	cpu_res_alloc(BoxInNum, cpu_id, group_size, &group_num);
#else
    group_num = (BoxInNum>16) ? ARM_THREAD_MAX:((BoxInNum + 3) >> 2);
    s16 BoxInNum1 = BoxInNum;
    group_size[0] = (BoxInNum + group_num -1)/group_num;
	BoxInNum1 -= group_size[0];
	group_size[1] = (BoxInNum1>group_size[0]) ? group_size[0]:BoxInNum1;
	BoxInNum1 -= group_size[1];
	group_size[2] = (BoxInNum1>group_size[0]) ? group_size[0]:BoxInNum1;
	BoxInNum1 -= group_size[2];
	group_size[3] = (BoxInNum1>group_size[0]) ? group_size[0]:BoxInNum1;
#endif

    s16 *pIn  = (s16 *)BoxIn;
	s16 *pInId  = (s16 *)BoxId;
  //LIB_ALGO_PRINT("1 : %d\n", BoxInNum * 4 * sizeof(s16));
    s16 *pOut = (s16 *)malloc(BoxInNum*4*sizeof(s16));
  //LIB_ALGO_PRINT("2 - %p\n", pOut);
	if(NULL == pOut)
	{
	    *BoxOutNum = 0;
		return ALGO_FAILURE;
	}
  //LIB_ALGO_PRINT("3 : %d\n", BoxInNum * sizeof(s16));
	s16 *pOutId = (s16 *)malloc(BoxInNum*sizeof(s16));
  //LIB_ALGO_PRINT("4 - %p\n", pOutId);
	if(NULL == pOutId)
	{
		*BoxOutNum = 0;
		free(pOut);
		return ALGO_FAILURE;
	}
	s16 out_face_num = 0;
	pthread_t thread_id[ARM_THREAD_MAX];
	t_track_para mPara[ARM_THREAD_MAX];

    //thread create
    s16 *pOut0 = pOut;
	s16 *pOutId0 = pOutId;
    s16 face_num_total = BoxInNum;
	for(i = 0; i < group_num; i++)
    {
        int ret;
		mPara[i].cpu_id = cpu_id[i];
	    mPara[i].BoxIn = pIn;
	    mPara[i].BoxId = pInId;
        mPara[i].BoxInNum = group_size[i];
		mPara[i].BoxOut = pOut0;
		mPara[i].BoxOutId = pOutId0;
		mPara[i].ImgCur = ImgCur;
		mPara[i].ImgPre = ImgPre;
		mPara[i].Img_H = Img_H;
		mPara[i].Img_W = Img_W;
        mPara[i].BoxOutNum = 0;
		
		do
		{
			//ret=pthread_create(&thread_id[i],NULL,(void *)SVP_ARM_FaceTrack_Thread,(void *)&mPara[i]); // �ɹ�����0�����󷵻ش�����
		     SVP_ARM_FaceTrack_Thread((void *)&mPara[i], min_obj_size);
        }while(0!=ret);

        pIn  += 4*mPara[i].BoxInNum;
		pInId += mPara[i].BoxInNum;
		pOut0 += 4*mPara[i].BoxInNum;
		pOutId0 += mPara[i].BoxInNum;
	}

    //thread join
    for(i = 0; i < group_num; i++)
    {
        //pthread_join(thread_id[i],NULL);

        memcpy(BoxOut,mPara[i].BoxOut,mPara[i].BoxOutNum*4*sizeof(s16));
		memcpy(BoxOutId,mPara[i].BoxOutId,mPara[i].BoxOutNum*sizeof(s16));
        BoxOut += 4*mPara[i].BoxOutNum*sizeof(s16);
		BoxOutId += mPara[i].BoxOutNum*sizeof(s16);
		out_face_num += mPara[i].BoxOutNum;
    }

	*BoxOutNum = out_face_num;

	free(pOut);
	free(pOutId);

    return ALGO_SUCCESS;
}

void SVP_ARM_PnetNms_Thread(void* mPara)
{
    t_pnet_nms_para* pnet_nms_para = (t_pnet_nms_para*)mPara;
	s16 cpu_id = pnet_nms_para->cpu_id;

#ifdef _GNU_SOURCE
	cpu_set_t mask;
	CPU_ZERO(&mask);
    CPU_SET(cpu_id,&mask);
    if(pthread_setaffinity_np(pthread_self(), sizeof(mask),&mask)< 0){
        printf("set thread affinity failed!\n");
    }
#endif

#if ARM_PROFILE_ON
    struct timespec timestart = {0};
    struct timespec timeend = {0};        

    clock_gettime(CLOCK_MONOTONIC, &timestart);   
#endif

	t_pnet_nms_info* pnet_nms_info = &pnet_nms_para->pnet_nms_info;
    Pnet_Nms_Proc(pnet_nms_info);

#if ARM_PROFILE_ON
    clock_gettime(CLOCK_MONOTONIC, &timeend);
    printf("arm pnet nms thread%d time = %f ms!\n", cpu_id, (timeend.tv_sec - timestart.tv_sec) * 1000 + (float)(timeend.tv_nsec - timestart.tv_nsec) / 1000000);
#endif	
}

s32 SVP_ARM_PnetNms(s16 LevelNum, t_cmm_pnet_level_info *Pnet_level_info, void *BoxOut, void *BoxNum, void *Score)
{
    pthread_t thread_id;
    t_pnet_nms_para pnet_nms_para;
#ifdef _GNU_SOURCE
    pnet_nms_para.cpu_id = 2;
#else
    pnet_nms_para.cpu_id = 0;
#endif
	
    t_pnet_nms_info* pnet_nms_info = &pnet_nms_para.pnet_nms_info;
    pnet_nms_info->level_num = LevelNum;
	pnet_nms_info->min_size = 12;
	pnet_nms_info->reg_fixQ = 11;
	pnet_nms_info->stride_step = 2;
	pnet_nms_info->thres = 140;
  //LIB_ALGO_PRINT("1 : %d\n", 32768 * sizeof(s16));
	pnet_nms_info->pDdrBuf = (s16 *)malloc(32768*sizeof(s16));//32k s16
  //LIB_ALGO_PRINT("2 - %p\n", pnet_nms_info->pDdrBuf);
	pnet_nms_info->pFace_Roi = (s16*)BoxOut;
	pnet_nms_info->pFace_Score = (s16*)Score;
	pnet_nms_info->face_num = (s16*)BoxNum;

	for(s16 i=0; i<LevelNum; i++)
	{
		pnet_nms_info->pnet_level_info[i].scale = Pnet_level_info[i].scale;
		pnet_nms_info->pnet_level_info[i].height = Pnet_level_info[i].height;
		pnet_nms_info->pnet_level_info[i].width = Pnet_level_info[i].width;
		pnet_nms_info->pnet_level_info[i].cls = (s16*)Pnet_level_info[i].cls;
		pnet_nms_info->pnet_level_info[i].reg = (s16*)Pnet_level_info[i].reg;
	}
	
    int ret;
	do
	{
		ret=pthread_create(&thread_id,NULL,(void *)SVP_ARM_PnetNms_Thread,(void *)&pnet_nms_para); // �ɹ�����0�����󷵻ش�����
	}while(0!=ret);	

	pthread_join(thread_id,NULL);
	
	free(pnet_nms_info->pDdrBuf);
	
	return ALGO_SUCCESS;
}

void SVP_ARM_ROnetNms_Thread(void* mPara)
{
    t_rnet_onet_nms_para* rnet_onet_nms_para = (t_rnet_onet_nms_para*)mPara;
	s16 cpu_id = rnet_onet_nms_para->cpu_id;

#ifdef _GNU_SOURCE	
	cpu_set_t mask;
	CPU_ZERO(&mask);
    CPU_SET(cpu_id,&mask);
    if(pthread_setaffinity_np(pthread_self(), sizeof(mask),&mask)< 0){
        printf("set thread affinity failed!\n");
    }
#endif

#if ARM_PROFILE_ON
    struct timespec timestart = {0};
    struct timespec timeend = {0};        

    clock_gettime(CLOCK_MONOTONIC, &timestart);   
#endif

    t_rnet_onet_nms_info* rnet_onet_nms_info = &rnet_onet_nms_para->rnet_onet_nms_info;
	Nms_Net_Type nms_net_type0 = rnet_onet_nms_para->nms_net_type;
    Face_Net_Nms_Proc(rnet_onet_nms_info, nms_net_type0);

#if ARM_PROFILE_ON
    clock_gettime(CLOCK_MONOTONIC, &timeend);
    printf("arm ronet nms thread%d time = %f ms!\n", cpu_id, (timeend.tv_sec - timestart.tv_sec) * 1000 + (float)(timeend.tv_nsec - timestart.tv_nsec) / 1000000);
#endif	
}

s32 SVP_ARM_ROnetNms(s16 nms_net_type, void *Cls, void *BoxReg, void *BoxIn, s16 BoxInNum, void *InScore, 
	void *BoxOut, void *BoxOutNum, void *OutScore)
{
#if 1
    t_rnet_onet_nms_info rnet_onet_nms_info;
	Nms_Net_Type nms_net_type0 = (Nms_Net_Type)nms_net_type;

	rnet_onet_nms_info.in_face_num = BoxInNum;
	rnet_onet_nms_info.in_face_roi = (s16*)BoxIn;
	rnet_onet_nms_info.in_face_score = (s16*)InScore;
	rnet_onet_nms_info.out_face_roi = (s16*)BoxOut;
	rnet_onet_nms_info.out_face_score = (s16*)OutScore;
	rnet_onet_nms_info.out_face_num = (s16*)BoxOutNum;
	rnet_onet_nms_info.cls = (s16*)Cls;
	rnet_onet_nms_info.reg = (s16*)BoxReg;
	if(NMS_RNET == nms_net_type0)
	{
		rnet_onet_nms_info.RO_thres = 13;
		rnet_onet_nms_info.reg_fixQ = 11;
		rnet_onet_nms_info.nms_thres = 716;
		rnet_onet_nms_info.nms_fixQ = 10;
		rnet_onet_nms_info.nms_mode = NMS_UNION_MODE;
	}
	else
	{
		rnet_onet_nms_info.RO_thres = 111;
		rnet_onet_nms_info.reg_fixQ = 11;
		rnet_onet_nms_info.nms_thres = 716;
		rnet_onet_nms_info.nms_fixQ = 10;
		rnet_onet_nms_info.nms_mode = NMS_MIN_MODE;
	}

	Face_Net_Nms_Proc(&rnet_onet_nms_info, nms_net_type0);
	
#else
    pthread_t thread_id;
    t_rnet_onet_nms_para rnet_onet_nms_para;
    t_rnet_onet_nms_info* rnet_onet_nms_info = &rnet_onet_nms_para.rnet_onet_nms_info;
	rnet_onet_nms_para.nms_net_type = (Nms_Net_Type)nms_net_type;
#ifdef _GNU_SOURCE
	rnet_onet_nms_para.cpu_id = 2;
#else
    rnet_onet_nms_para.cpu_id = 0;
#endif

	rnet_onet_nms_info->in_face_num = BoxInNum;
	rnet_onet_nms_info->in_face_roi = (s16*)BoxIn;
	rnet_onet_nms_info->in_face_score = (s16*)InScore;
	rnet_onet_nms_info->out_face_roi = (s16*)BoxOut;
	rnet_onet_nms_info->out_face_score = (s16*)OutScore;
	rnet_onet_nms_info->out_face_num = (s16*)BoxOutNum;
	rnet_onet_nms_info->cls = (s16*)Cls;
	rnet_onet_nms_info->reg = (s16*)BoxReg;
	if(NMS_RNET == rnet_onet_nms_para.nms_net_type)
	{
		rnet_onet_nms_info->RO_thres = 13;
		rnet_onet_nms_info->reg_fixQ = 11;
		rnet_onet_nms_info->nms_thres = 716;
		rnet_onet_nms_info->nms_fixQ = 10;
		rnet_onet_nms_info->nms_mode = NMS_UNION_MODE;
	}
	else
	{
		rnet_onet_nms_info->RO_thres = 111;
		rnet_onet_nms_info->reg_fixQ = 11;
		rnet_onet_nms_info->nms_thres = 716;
		rnet_onet_nms_info->nms_fixQ = 10;
		rnet_onet_nms_info->nms_mode = NMS_MIN_MODE;
	}
	
    int ret;
	do
	{
		ret=pthread_create(&thread_id,NULL,(void *)SVP_ARM_ROnetNms_Thread,(void *)&rnet_onet_nms_para); // �ɹ�����0�����󷵻ش�����
	}while(0!=ret);	

	pthread_join(thread_id,NULL);
#endif
	return ALGO_SUCCESS;
}

void SVP_ARM_Resize_Thread(void* mPara)
{
    t_resize_para* resize_para = (t_resize_para*)mPara;
	eImgType img_type = (eImgType)resize_para->Img_Type;
	s16* face_roi = resize_para->Face_Roi;
	u8* face_out = resize_para->DataOut;
	s16 cpu_id = resize_para->cpu_id;

#ifdef _GNU_SOURCE
	cpu_set_t mask;
	CPU_ZERO(&mask);
    CPU_SET(cpu_id,&mask);
    if(pthread_setaffinity_np(pthread_self(), sizeof(mask),&mask)< 0){
        printf("set thread affinity failed!\n");
    }
#endif

#if ARM_PROFILE_ON
    struct timespec timestart = {0};
    struct timespec timeend = {0};        

    clock_gettime(CLOCK_MONOTONIC, &timestart);   
#endif

	if(IMG_YUV == img_type)
	{
	    s16 out_len = 3*resize_para->Dst_H*resize_para->Dst_W;
    //LIB_ALGO_PRINT("1 : %d\n", out_len);
		u8* dataout0 = malloc(out_len);
    //LIB_ALGO_PRINT("2 - %p\n", dataout0);
		
		for(s16 i=0; i<resize_para->FaceNum; i++)
		{
		    s32 _search_h = face_roi[3] - face_roi[1] + 1;
			s32 _search_w = face_roi[2] - face_roi[0] + 1;
			
			face_resize_scale_yuv2rgb_chw_no_normalize(resize_para->Img_Data, 0, resize_para->Img_inH, resize_para->Img_inW, 
				face_roi[0], face_roi[1], _search_h, _search_w, resize_para->Dst_H, resize_para->Dst_W, dataout0);
			if_data_transform_align16(dataout0, face_out, 3, resize_para->Dst_H, resize_para->Dst_W);
			
			face_roi+=4;
			face_out+=out_len;
		}

		free(dataout0);
	}
	else if(IMG_GREY == img_type)
	{
	    s16 out_len = resize_para->Dst_H*resize_para->Dst_W;
   // LIB_ALGO_PRINT("3 : %d\n", out_len);
		u8* dataout0 = malloc(out_len);
   // LIB_ALGO_PRINT("4 - %p\n", dataout0);
		
		for(s16 i=0; i<resize_para->FaceNum; i++)
		{
		    s32 _search_h = face_roi[3] - face_roi[1] + 1;
			s32 _search_w = face_roi[2] - face_roi[0] + 1;
			
			face_resize_scale_yuv2Y_no_normalize(resize_para->Img_Data, resize_para->Img_inH, resize_para->Img_inW, 
				face_roi[0], face_roi[1], _search_h, _search_w, resize_para->Dst_H, resize_para->Dst_W, dataout0);
			if_data_transform_align16(dataout0, face_out, 1, resize_para->Dst_H, resize_para->Dst_W);
			
			face_roi+=4;
			face_out+=out_len;
		}

		free(dataout0);
	}

#if ARM_PROFILE_ON
    clock_gettime(CLOCK_MONOTONIC, &timeend);
    printf("arm resize thread%d time = %f ms!\n", cpu_id, (timeend.tv_sec - timestart.tv_sec) * 1000 + (float)(timeend.tv_nsec - timestart.tv_nsec) / 1000000);
#endif
}

s32 SVP_ARM_Resize(u8* Img_Data, u8* DataOut, s16* Face_Roi, s16 FaceNum, s16 Img_Type, s16 Img_inH, 
	s16 Img_inW, s16 Dst_H, s16 Dst_W)
{
    s32 i = 0;
    s16 group_num = 0;//(FaceNum>16) ? ARM_THREAD_MAX:((FaceNum + 3) >> 2);
    s16 group_size[4] = {0,};//(FaceNum + group_num -1)/group_num;
    u8* pIn  = (u8*)Img_Data;
	eImgType img_type = (eImgType)Img_Type;
	s16 out_len = Dst_H*Dst_W;
	if(IMG_YUV == img_type)
	{
		out_len *= 3;
	}

    s16 cpu_id[4] = {0,};
#ifdef _GNU_SOURCE
	cpu_res_alloc(FaceNum, cpu_id, group_size, &group_num);
#else
    group_num = (FaceNum>16) ? ARM_THREAD_MAX:((FaceNum + 3) >> 2);
    s16 BoxInNum1 = FaceNum;
    group_size[0] = (FaceNum + group_num -1)/group_num;
	BoxInNum1 -= group_size[0];
	group_size[1] = (BoxInNum1>group_size[0]) ? group_size[0]:BoxInNum1;
	BoxInNum1 -= group_size[1];
	group_size[2] = (BoxInNum1>group_size[0]) ? group_size[0]:BoxInNum1;
	BoxInNum1 -= group_size[2];
	group_size[3] = (BoxInNum1>group_size[0]) ? group_size[0]:BoxInNum1;
#endif
	
	pthread_t thread_id[ARM_THREAD_MAX];
	t_resize_para mPara[ARM_THREAD_MAX];

    //thread create
    s16 face_num_total = FaceNum;
	for(i = 0; i < group_num; i++)
    {
        int ret;
		mPara[i].cpu_id = cpu_id[i];
		mPara[i].Img_Data = pIn;
	    mPara[i].FaceNum = group_size[i];
	    mPara[i].Img_inH = Img_inH;
        mPara[i].Img_inW = Img_inW;
		mPara[i].Dst_H = Dst_H;
		mPara[i].Dst_W = Dst_W;
		mPara[i].Img_Type = Img_Type;
		mPara[i].Face_Roi = Face_Roi;
		mPara[i].DataOut = DataOut;
		
        Face_Roi += 4*mPara[i].FaceNum;
		DataOut += mPara[i].FaceNum * out_len;
		
		do
		{
			ret=pthread_create(&thread_id[i],NULL,(void *)SVP_ARM_Resize_Thread,(void *)&mPara[i]); // �ɹ�����0�����󷵻ش�����
		}while(0!=ret);

	}

    //thread join
    for(i = 0; i < group_num; i++)
    {
        pthread_join(thread_id[i],NULL);
    }

    return ALGO_SUCCESS;
}

void SVP_ARM_Stn_Resize_Thread(void* mPara)
{
  t_stn_resize_para* stn_resize_para = (t_stn_resize_para*)mPara;
  s16 cpu_id = stn_resize_para->cpu_id;
  u8* face_in = stn_resize_para->DataIn;
	u8* face_out = stn_resize_para->DataOut;
	s16* weight = stn_resize_para->Weight;
	s32 in_len = 3*stn_resize_para->Data_inH*stn_resize_para->Data_inW;
	s32 out_len = 3*stn_resize_para->Dst_H*stn_resize_para->Dst_W;
  //LIB_ALGO_PRINT("1 : %d\n", in_len);
	u8* datain0 = malloc(in_len);
  //LIB_ALGO_PRINT("2 - %p, : %d\n", datain0, out_len);
	u8* dataout0 = malloc(out_len);
 // LIB_ALGO_PRINT("3 - %p\n", dataout0);

#ifdef _GNU_SOURCE
	cpu_set_t mask;
	CPU_ZERO(&mask);
    CPU_SET(cpu_id,&mask);
    if(pthread_setaffinity_np(pthread_self(), sizeof(mask),&mask)< 0){
        printf("set thread affinity failed!\n");
    }
#endif

#if ARM_PROFILE_ON
    struct timespec timestart = {0};
    struct timespec timeend = {0};

    clock_gettime(CLOCK_MONOTONIC, &timestart);
#endif

	for(s16 i=0; i<stn_resize_para->FaceNum; i++)
	{
        if_data_transform_noalign16(face_in, datain0, 3, stn_resize_para->Data_inH, stn_resize_para->Data_inW);
		gluon_stn_resize_fix(datain0, dataout0, weight, 3, stn_resize_para->Data_inH, stn_resize_para->Data_inW,
			stn_resize_para->Dst_H, stn_resize_para->Dst_W);
        if_data_transform_align16(dataout0, face_out, 3, stn_resize_para->Dst_H, stn_resize_para->Dst_W);

		face_in+=in_len;
		face_out+=out_len;
		weight+=6;
	}

#if ARM_PROFILE_ON
    clock_gettime(CLOCK_MONOTONIC, &timeend);
    printf("arm stn thread%d time = %f ms!\n", cpu_id, (timeend.tv_sec - timestart.tv_sec) * 1000 + (float)(timeend.tv_nsec - timestart.tv_nsec) / 1000000);
#endif

	free(datain0);
	free(dataout0);
}

s32 SVP_ARM_Stn_Resize(u8* DataIn, u8* DataOut, s16* Weight, s16 FaceNum, s16 Data_inH, s16 Data_inW, 
	s16 Dst_H, s16 Dst_W)
{
    s32 i = 0;
    s16 group_num = 0;// (FaceNum>16) ? ARM_THREAD_MAX:((FaceNum + 3) >> 2);
    s16 group_size[4] = {0,};// (FaceNum + group_num -1)/group_num;
	s32 in_len = 3*Data_inH*Data_inW;
	s32 out_len = 3*Dst_H*Dst_W;
	s16 cpu_id[4] = {0,};

#ifdef _GNU_SOURCE
	cpu_res_alloc(FaceNum, cpu_id, group_size, &group_num);
#else
    group_num = (FaceNum>16) ? ARM_THREAD_MAX:((FaceNum + 3) >> 2);
    s16 BoxInNum1 = FaceNum;
    group_size[0] = (FaceNum + group_num -1)/group_num;
	BoxInNum1 -= group_size[0];
	group_size[1] = (BoxInNum1>group_size[0]) ? group_size[0]:BoxInNum1;
	BoxInNum1 -= group_size[1];
	group_size[2] = (BoxInNum1>group_size[0]) ? group_size[0]:BoxInNum1;
	BoxInNum1 -= group_size[2];
	group_size[3] = (BoxInNum1>group_size[0]) ? group_size[0]:BoxInNum1;
#endif
	
	pthread_t thread_id[ARM_THREAD_MAX];
	t_stn_resize_para mPara[ARM_THREAD_MAX];

    //thread create
    s16 face_num_total = FaceNum;
	for(i = 0; i < group_num; i++)
    {
        int ret;
		mPara[i].cpu_id = cpu_id[i];
		mPara[i].DataIn = DataIn;
	    mPara[i].FaceNum = group_size[i];
	    mPara[i].Data_inH = Data_inH;
        mPara[i].Data_inW = Data_inW;
		mPara[i].Dst_H = Dst_H;
		mPara[i].Dst_W = Dst_W;
		mPara[i].DataOut = DataOut;
		mPara[i].Weight = Weight;

		DataIn += mPara[i].FaceNum * in_len;
		DataOut += mPara[i].FaceNum * out_len;
		Weight += mPara[i].FaceNum * 6; 
		
		do
		{
			ret=pthread_create(&thread_id[i],NULL,(void *)SVP_ARM_Stn_Resize_Thread,(void *)&mPara[i]); // �ɹ�����0�����󷵻ش�����
		}while(0!=ret);

	}

    //thread join
    for(i = 0; i < group_num; i++)
    {
        pthread_join(thread_id[i],NULL);
    }

    return ALGO_SUCCESS;
}

