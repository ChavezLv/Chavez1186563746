#ifndef IVEKCFTRACKER_H
#define IVEKCFTRACKER_H
/***********************************/
//  only used for 3516dv300
//  all ive kfc api are only supported by 3516
/***********************************/
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
#include <pthread.h>
#include <sys/prctl.h>
#include <math.h>
#include <vector>
#include <unordered_set>
//#include "arctern_manager.h"

extern "C" {
#include <if_algo/if_algo_api.h>
}

#define IVE_KCF_NODE_MAX_NUM                   64
#define IVE_KCF_GAUSS_PEAK_TOTAL_SIZE          455680
#define IVE_KCF_COS_WINDOW_TOTAL_SIZE          832
#define IVE_KCF_COS_WINDOW_SIZE                64
#define IVE_KCF_TEMP_BUF_SIZE                  47616
#define IVE_KCF_HOG_FEATRUE_BUF_SIZE           4761
#define IVE_KCF_NMS_THRESHOLD                  0.3

#define IVE_KCF_ROI_PADDING_MAX_WIDTH          1024
#define IVE_KCF_ROI_PADDING_MIN_WIDTH          40
#define IVE_KCF_ROI_PADDING_MAX_HEIGHT         1024
#define IVE_KCF_ROI_PADDING_MIN_HEIGHT         40

#ifdef AARCH64_HIMIX200_3516DV300
typedef struct hiIVE_KCF_S
{
    IVE_ROI_INFO_S astRoiInfo[64];
    HI_U32 u32RoiNum;
    IVE_MEM_INFO_S stTotalMem;
    IVE_MEM_INFO_S stListMem;
    IVE_MEM_INFO_S stCosWinX;
    IVE_MEM_INFO_S stCosWinY;
    IVE_MEM_INFO_S stGaussPeak;
    IVE_KCF_OBJ_LIST_S stObjList;
    IVE_KCF_PRO_CTRL_S stKcfProCtrl;
    IVE_KCF_BBOX_S astBbox[64];
    HI_U32 u32BboxNum;
    IVE_KCF_BBOX_CTRL_S stKcfBboxCtrl;
    HI_U3Q5 u3q5Padding;

    IVE_IMAGE_S stSrcImg;
    HI_U32 img_width;// 输入图像的大小，需明确指定
    HI_U32 img_height;// 输入图像的大小，需明确指定
    HI_FLOAT img_change_scale;//根据输入图像大小除以原始图像大小计算获得，对坐标换算有影响，需明确设定
    HI_FLOAT img_change_scale_h;//根据输入图像大小除以原始图像大小计算获得，对坐标换算有影响，需明确设定

    HI_U32 u32BboxIndex;
}IVE_KCF_TRACK;
#else
typedef struct hiIVE_KCF_S{
    IVE_IMAGE_S stSrcImg;
    HI_U32 img_width;// 输入图像的大小，需明确指定
    HI_U32 img_height;// 输入图像的大小，需明确指定
    HI_FLOAT img_change_scale;//根据输入图像大小除以原始图像大小计算获得，对坐标换算有影响，需明确设定
    HI_FLOAT img_change_scale_h;//根据输入图像大小除以原始图像大小计算获得，对坐标换算有影响，需明确设定
    HI_U32 u32BboxIndex;
}IVE_KCF_TRACK;

#endif
HI_S32 if_IVE_KCF_track(IVE_KCF_TRACK* pstKCF, t_track_face_deil& track_bboxes, HI_BOOL change_track_bboxes = HI_TRUE);
HI_S32 if_IVE_KCF_update(IVE_KCF_TRACK* pstKCF, t_track_face_deil& track_bboxes, t_track_face_deil &new_detection_boxes);

HI_S32 if_IVE_KCF_uninit(IVE_KCF_TRACK* pstKCF);
HI_S32 if_IVE_KCF_init(IVE_KCF_TRACK* pstKCF, const arctern::ObjectTrackerParameter& track_params);


#endif //end for IVEKCFTRACKER_H
