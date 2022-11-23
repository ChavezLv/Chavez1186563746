#ifndef _ARM_API_H_
#define _ARM_API_H_


extern s32 SVP_ARM_FaceTrack(u64 ImgPre, u64 ImgCur, void *BoxIn, void *BoxId, void *BoxOut, void *BoxOutId, 
	s16 BoxInNum, s16* BoxOutNum, s16 Img_H, s16 Img_W, s32 min_obj_size);

#endif
