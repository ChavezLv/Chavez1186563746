/*
 * slk_tracker_if.h
 *
 *  Created on: 2018-10-30
 *      Author: Administrator
 */

#ifndef _SLK_TRACKER_IF_H
#define _SLK_TRACKER_IF_H
#include "../arm_types.h"

bool slk_tracker(s16 *box_in, s16 *box_out, u8 *pre_gray, u8 *cur_gray, s16 height, s16 width, s32 min_obj_size);

#endif /*  */
