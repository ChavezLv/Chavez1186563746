/*
 * slk_tracker_in.h
 *
 *  Created on: 2018-10-30
 *      Author: Administrator
 */

#ifndef _SLK_TRACKER_IN_H
#define _SLK_TRACKER_IN_H

//#define TRACKER_MATRIX_SORT_AVG   1  //0:search kth scale  1:average scale between above and below threshold

#define SORT_SIZE_MAX  0X7FFFFFFF
#define SORT_CMP(x,y)  ((x) < (y) ? -1 : ((x) == (y) ? 0 : 1))
#define SORT_SWAP(x,y) {f32 __SORT_SWAP_t = (x); (x) = (y); (y) = __SORT_SWAP_t;}

bool face_position_predict(s16 nPoints, Rect *pre_face, Rect *cur_face, Point2f *pointsPrev,
                    Point2f *pointsCur, s16 img_h, s16 img_w);


#endif /*  */
