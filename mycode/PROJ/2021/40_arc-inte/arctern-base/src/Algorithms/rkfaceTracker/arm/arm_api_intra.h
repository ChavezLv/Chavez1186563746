#ifndef _ARM_API_INTRA_H_
#define _ARM_API_INTRA_H_

#define ARM_PROFILE_ON  0
#define ARM_THREAD_MAX  4  

typedef struct
{
    s16 cpu_id;  //0/1 a53; 2/3 a73
	u64 ImgPre;
	u64 ImgCur;
	void *BoxIn;
	void *BoxId;
	void *BoxOut;
	void *BoxOutId;
	s16 BoxInNum;
	s16 BoxOutNum;
	s16 Img_H;
	s16 Img_W;	
}t_track_para;

typedef struct
{
    s16 cpu_id;  //0/1 a53; 2/3 a73
    t_pnet_nms_info pnet_nms_info;	
}t_pnet_nms_para;

typedef struct
{
    s16 cpu_id;  //0/1 a53; 2/3 a73
    Nms_Net_Type nms_net_type;
    t_rnet_onet_nms_info rnet_onet_nms_info;	
}t_rnet_onet_nms_para;

typedef struct
{
    s16 cpu_id;   //0/1 a53; 2/3 a73
    u8* Img_Data;
	u8* DataOut;
	s16* Face_Roi;
	s16 FaceNum;
	s16 Img_Type;
	s16 Img_inH; 
	s16 Img_inW;
	s16 Dst_H;
	s16 Dst_W;	
}t_resize_para;

typedef struct
{
    s16 cpu_id;   //0/1 a53; 2/3 a73
	u8* DataIn;
	u8* DataOut;
	s16* Weight;
	s16 FaceNum;
	s16 Data_inH;
	s16 Data_inW; 
	s16 Dst_H;
	s16 Dst_W;	
}t_stn_resize_para;

typedef struct
{
    s16 cpu_id;    //0/1 a53; 2/3 a73
    s8* InFeat;
	s8* RefFeat;
	s16 Feat_Num;
	s32 refFeat_Num;
	s16 thresh;
	s16 TopN;
	s16 Fe_Len;
	s32* Index;
	s16* Score;
}t_fe_compare_para;



#endif
