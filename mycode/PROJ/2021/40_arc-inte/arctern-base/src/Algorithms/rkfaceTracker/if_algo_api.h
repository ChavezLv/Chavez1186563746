
#ifndef _IF_ALGO_API_H
#define _IF_ALGO_API_H

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int  u32;
typedef float f32;
typedef signed char s8;
typedef signed short s16;
typedef signed int  s32;
typedef signed long long s64;
typedef unsigned long long u64;

typedef enum
{
	E_CPU_ARM = 0,
	E_SEL_DSP = 1,
}e_cpu_mode;

#define TRACK_FACE_NUM_MAX 128

typedef enum {
    E_FACE_DEFAULT = 0,
	E_FACE_DELETE0,     //delete0 : no suit some conditions
	E_FACE_DELETE1,	    // delete1 : max face in track db
	E_FACE_ADDED, 	    // add : no face in track db
	E_FACE_AMENDDED     // amend : face in track db
}e_sync_face_status;

typedef struct
{
	s16 face_roi[TRACK_FACE_NUM_MAX][4];
	s16 face_id[TRACK_FACE_NUM_MAX];
	s16 face_num;
}t_track_face_deil;

typedef struct
{
    f32 scale;
    s16 height;
    s16 width;

    u64 cls;
    u64 reg;
}t_cmm_pnet_level_info;



#endif

