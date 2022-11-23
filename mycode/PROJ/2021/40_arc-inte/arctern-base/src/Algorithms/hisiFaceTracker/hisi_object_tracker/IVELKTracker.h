#ifndef IVELKTRACKER_H
#define IVELKTRACKER_H

#include "utils.h"
#ifdef BUILD_AARCH64_HIMIX200
#include "src/Engin/src/hisi/nnie/hi_common.h"
#include "src/Engin/src/hisi/nnie/hi_errno.h"
#include "src/Engin/src/hisi/nnie/hi_comm_sys.h"
#include "src/Engin/src/hisi/nnie/hi_comm_vpss.h"
#include "src/Engin/src/hisi/nnie/hi_comm_isp.h"
#include "src/Engin/src/hisi/nnie/hi_comm_ive.h"
#include "src/Engin/src/hisi/nnie/hi_nnie.h"
#include "src/Engin/src/hisi/nnie/mpi_ive.h"
#include "src/Engin/src/hisi/nnie/mpi_nnie.h"
#include "src/Engin/src/hisi/nnie/sample_comm.h"
#include "src/Engin/src/hisi/nnie/sample_comm_ive.h"
#include "src/Engin/src/hisi/nnie/sample_comm_svp.h"
#include "src/Engin/src/hisi/nnie/sample_comm_nnie.h"
#elif defined(BUILD_AARCH64_HIMIX100)
#include "nnie-3559/hi_common.h"
#include "nnie-3559/hi_errno.h"
#include "nnie-3559/hi_comm_sys.h"
#include "nnie-3559/hi_comm_vpss.h"
#include "nnie-3559/hi_comm_isp.h"
#include "nnie-3559/hi_comm_ive.h"
#include "nnie-3559/hi_nnie.h"
#include "nnie-3559/mpi_ive.h"
#include "nnie-3559/mpi_nnie.h"
#include "nnie-3559/sample_comm.h"
#include "nnie-3559/sample_comm_ive.h"
#include "nnie-3559/sample_comm_svp.h"
#include "nnie-3559/sample_comm_nnie.h"
#else
    #error "bug include"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>
#include <pthread.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <bitset>
#include <opencv2/opencv.hpp>
#include <math.h>
//#include "arctern_manager.h"

extern "C" {
#include <if_algo/if_algo_api.h>
}

#define IVE_MASK 0b00000000000000000000000001111111
#define SCALE_CHANGE_MAX 1.1
#define SCALE_CHANGE_MIN 0.9
#define MAX_PTS_NUM 500 
#define IVE_MAX(a, b) a > b ? a: b
#define IVE_MIN(a, b) a > b ? b: a

typedef struct hiSAMPLE_IVE_ST_LK_S
{
    IVE_SRC_IMAGE_S                       astPrevPyr[4];//图片存储
    IVE_SRC_IMAGE_S                       astNextPyr[4];//图片存储
    IVE_SRC_MEM_INFO_S                    stPrevPts;//光流点坐标
    IVE_MEM_INFO_S                        stNextPts;//光流点坐标
    IVE_DST_MEM_INFO_S                    stStatus;//光流点计算结果状态标志
    IVE_DST_MEM_INFO_S                    stErr;//光流点计算结果误差
    IVE_LK_OPTICAL_FLOW_PYR_CTRL_S        stLkPyrCtrl;//光流技术控制器
    IVE_IMAGE_S                           stTmp;//

    HI_U16                                bbox_num;//目标计数器
    HI_FLOAT                              bbox_resize_scale;//光流点选择范围为bbox内部区域，人脸跟踪选取bbox较大一片区域0.7，人体跟踪选取较小区域0.5
    HI_FLOAT                              OF_calc_threshold;//光流计算阈值，超过认为是错误光流值
    HI_U32                                OF_point_num_per_row;//目标内光流点选择参数，每行光流点个数6，6x6个点足够得到结果
    HI_U32                                OF_point_num_per_col;//
    HI_U8                                 max_level;//lk算法图像金字塔层数，取值0-3，对应1-4层，3层足以
    HI_U32                                img_width;// 输入图像的大小，需明确指定
    HI_U32                                img_height;// 输入图像的大小，需明确指定
    HI_FLOAT                              img_change_scale;//根据输入图像大小除以原始图像大小计算获得，对坐标换算有影响，需明确设定
    HI_FLOAT                              img_change_scale_h;//根据输入图像大小除以原始图像大小计算获得，对坐标换算有影响，需明确设定
} IVE_LK_TRACK;

HI_S32 if_IVE_LK_track(IVE_LK_TRACK* pstStLk, t_track_face_deil& track_bboxes);
HI_S32 if_IVE_LK_update(IVE_LK_TRACK* pstStLk, const t_track_face_deil& track_bboxes);

HI_S32 if_IVE_LK_uninit(IVE_LK_TRACK* pstStLk);
HI_S32 if_IVE_LK_init(IVE_LK_TRACK* pstStLk, const arctern::ObjectTrackerParameter& track_params);

#endif
