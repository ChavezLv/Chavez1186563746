/*
 * slk_tracker_cmm.h
 *
 *  Created on: 2018-10-30
 *      Author: Administrator
 */

#ifndef _SLK_TRACKER_CMM_H
#define _SLK_TRACKER_CMM_H
#include "../arm_types.h"

typedef struct
{
	s16 x;
	s16 y;
}Point2i;

typedef struct
{
	f32 x;
	f32 y;
}Point2f;

typedef struct
{
	s16 w;
	s16 h;
}Size;

typedef struct
{
	s16 x;
	s16 y;
	s16 w;
	s16 h;
}Rect;

typedef struct
{
	u8 *datastart;
	u8 *data;
	s16 w;
	s16 h;
	s32 mem_len;
	s32 step;
	s16 reserved[6];
}t_pyramid_info;


#endif /*  */
