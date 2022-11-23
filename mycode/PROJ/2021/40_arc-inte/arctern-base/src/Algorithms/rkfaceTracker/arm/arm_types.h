/*!
 *  Copyright (c) 2018 by Contributors
 * \file types.h
 * \brief basic type definition.
 *
 */

#ifndef ARM_TYPES_H_
#define ARM_TYPES_H_

#ifdef _ARM_NEON
#define ALGO_FE_COMPARE
#endif

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int  u32;
typedef float f32;
typedef signed char s8;
typedef signed short s16;
typedef signed int  s32;
typedef signed long long s64;
typedef unsigned long long u64;

#ifndef bool
#define bool int
#endif
#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif

#ifndef MAX2
#define MAX2(a, b)		((a) > (b) ? (a) : (b))
#endif

#ifndef MIN2
#define MIN2(a, b)		((a) < (b) ? (a) : (b))
#endif

#ifndef ARR_DIM
#define ARR_DIM(arr)	((s32)(sizeof(arr)/sizeof(arr[0])))
#endif

#ifndef ALGO_SUCCESS
#define ALGO_SUCCESS 0
#endif

#ifndef ALGO_FAILURE
#define ALGO_FAILURE -1
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

typedef enum {
    ALGO_FALSE = 0,
    ALGO_TRUE  = 1,
}ALGO_BOOL;


#endif
