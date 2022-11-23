

#ifndef __SAMPLE_COMM_H__
#define __SAMPLE_COMM_H__

#include <pthread.h>

#include "hi_common.h"
#include "hi_buffer.h"
#include "hi_comm_sys.h"
#include "hi_comm_vb.h"
#include "hi_comm_isp.h"
#include "hi_comm_vi.h"
#include "hi_comm_vo.h"
#include "hi_comm_venc.h"
// #include "hi_comm_vdec.h"
#include "hi_comm_vpss.h"
// #include "hi_comm_region.h"
// #include "hi_comm_adec.h"
// #include "hi_comm_aenc.h"
// #include "hi_comm_ai.h"
// #include "hi_comm_ao.h"
// #include "hi_comm_aio.h"
#include "hi_defines.h"
// #include "hi_comm_hdmi.h"
#include "hi_mipi.h"
#include "hi_comm_vgs.h"

#include "mpi_sys.h"
#include "mpi_vb.h"
#include "mpi_vi.h"
#include "mpi_vo.h"
#include "mpi_venc.h"
// #include "mpi_vdec.h"
#include "mpi_vpss.h"
// #include "mpi_region.h"
// #include "mpi_audio.h"
#include "mpi_isp.h"
// #include "mpi_ae.h"
// #include "mpi_awb.h"
#include "hi_math.h"
// #include "hi_sns_ctrl.h"
#include "mpi_hdmi.h"
#include "mpi_vgs.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */


/*******************************************************
    macro define
*******************************************************/
#define FILE_NAME_LEN               128

#define CHECK_CHN_RET(express,Chn,name)\
    do{\
        HI_S32 Ret;\
        Ret = express;\
        if (HI_SUCCESS != Ret)\
        {\
            printf("\033[0;31m%s chn %d failed at %s: LINE: %d with %#x!\033[0;39m\n", name, Chn, __FUNCTION__, __LINE__, Ret);\
            fflush(stdout);\
            return Ret;\
        }\
    }while(0)

#define CHECK_RET(express,name)\
    do{\
        HI_S32 Ret;\
        Ret = express;\
        if (HI_SUCCESS != Ret)\
        {\
            printf("\033[0;31m%s failed at %s: LINE: %d with %#x!\033[0;39m\n", name, __FUNCTION__, __LINE__, Ret);\
            return Ret;\
        }\
    }while(0)
#define SAMPLE_PIXEL_FORMAT         PIXEL_FORMAT_YVU_SEMIPLANAR_420

#define TLV320_FILE "/dev/tlv320aic31"
#define COLOR_RGB_RED      0xFF0000
#define COLOR_RGB_GREEN    0x00FF00
#define COLOR_RGB_BLUE     0x0000FF
#define COLOR_RGB_BLACK    0x000000
#define COLOR_RGB_YELLOW   0xFFFF00
#define COLOR_RGB_CYN      0x00ffff
#define COLOR_RGB_WHITE    0xffffff

#define SAMPLE_VO_DEV_DHD0 0                  /* VO's device HD0 */
#define SAMPLE_VO_DEV_DHD1 1                  /* VO's device HD1 */
#define SAMPLE_VO_DEV_UHD  SAMPLE_VO_DEV_DHD0 /* VO's ultra HD device:HD0 */
#define SAMPLE_VO_DEV_HD   SAMPLE_VO_DEV_DHD1 /* VO's HD device:HD1 */
#define SAMPLE_VO_LAYER_VHD0 0
#define SAMPLE_VO_LAYER_VHD1 1
#define SAMPLE_VO_LAYER_VHD2 2
#define SAMPLE_VO_LAYER_PIP  SAMPLE_VO_LAYER_VHD2

#define SAMPLE_AUDIO_EXTERN_AI_DEV 0
#define SAMPLE_AUDIO_EXTERN_AO_DEV 0
#define SAMPLE_AUDIO_INNER_AI_DEV 0
#define SAMPLE_AUDIO_INNER_AO_DEV 0
#define SAMPLE_AUDIO_INNER_HDMI_AO_DEV 1

#define SAMPLE_AUDIO_PTNUMPERFRM   480

#define WDR_MAX_PIPE_NUM        4


#define PAUSE()  do {\
        printf("---------------press Enter key to exit!---------------\n");\
        getchar();\
    } while (0)


#define SAMPLE_PRT(fmt...)   \
    do {\
        printf("[%s]-%d: ", __FUNCTION__, __LINE__);\
        printf(fmt);\
    }while(0)

#define CHECK_NULL_PTR(ptr)\
    do{\
        if(NULL == ptr)\
        {\
            printf("func:%s,line:%d, NULL pointer\n",__FUNCTION__,__LINE__);\
            return HI_FAILURE;\
        }\
    }while(0)


/*******************************************************
    enum define
*******************************************************/

typedef enum hiPIC_SIZE_E
{
    PIC_CIF,
    PIC_D1_PAL,    /* 720 * 576 */
    PIC_D1_NTSC,   /* 720 * 480 */
    PIC_720P,      /* 1280 * 720  */
    PIC_1080P,     /* 1920 * 1080 */
    PIC_2592x1520,
    PIC_2592x1536,
    PIC_2592x1944,
    PIC_2716x1524,
    PIC_3840x2160,
    PIC_4096x2160,
    PIC_3000x3000,
    PIC_4000x3000,
    PIC_7680x4320,
    PIC_3840x8640,
    PIC_BUTT
} PIC_SIZE_E;

typedef enum hiSAMPLE_SNS_TYPE_E
{
    SONY_IMX327_MIPI_2M_30FPS_12BIT,
    SONY_IMX327_MIPI_2M_30FPS_12BIT_WDR2TO1,
    SONY_IMX327_2L_MIPI_2M_30FPS_12BIT,
    SONY_IMX327_2L_MIPI_2M_30FPS_12BIT_WDR2TO1,
    SONY_IMX307_MIPI_2M_30FPS_12BIT,
    SONY_IMX307_MIPI_2M_30FPS_12BIT_WDR2TO1,
    SONY_IMX335_MIPI_5M_30FPS_12BIT,
    SONY_IMX335_MIPI_5M_30FPS_10BIT_WDR2TO1,
    SONY_IMX335_MIPI_4M_30FPS_12BIT,
    SONY_IMX335_MIPI_4M_30FPS_10BIT_WDR2TO1,
    SONY_IMX458_MIPI_8M_30FPS_10BIT,
    SONY_IMX458_MIPI_12M_20FPS_10BIT,
    SONY_IMX458_MIPI_4M_60FPS_10BIT,
    SONY_IMX458_MIPI_4M_40FPS_10BIT,
    SONY_IMX458_MIPI_2M_90FPS_10BIT,
    SONY_IMX458_MIPI_1M_129FPS_10BIT,
    PANASONIC_MN34220_LVDS_2M_30FPS_12BIT,
    SAMPLE_SNS_TYPE_BUTT,
} SAMPLE_SNS_TYPE_E;


typedef enum hiSAMPLE_VO_MODE_E
{
    VO_MODE_1MUX  ,
    VO_MODE_2MUX  ,
    VO_MODE_4MUX  ,
    VO_MODE_8MUX  ,
    VO_MODE_9MUX  ,
    VO_MODE_16MUX ,
    VO_MODE_25MUX ,
    VO_MODE_36MUX ,
    VO_MODE_49MUX ,
    VO_MODE_64MUX ,
    VO_MODE_2X4   ,
    VO_MODE_BUTT
} SAMPLE_VO_MODE_E;

typedef enum hiSAMPLE_RC_E
{
    SAMPLE_RC_CBR = 0,
    SAMPLE_RC_VBR,
    SAMPLE_RC_AVBR,
    SAMPLE_RC_QVBR,
    SAMPLE_RC_QPMAP,
    SAMPLE_RC_FIXQP
} SAMPLE_RC_E;


/*******************************************************
    structure define
*******************************************************/
typedef struct hiSAMPLE_VENC_GETSTREAM_PARA_S
{
    HI_BOOL bThreadStart;
    VENC_CHN VeChn[VENC_MAX_CHN_NUM];
    HI_S32  s32Cnt;
} SAMPLE_VENC_GETSTREAM_PARA_S;

typedef struct hiSAMPLE_VENC_QPMAP_SENDFRAME_PARA_S
{
    HI_BOOL  bThreadStart;
    VPSS_GRP VpssGrp;
    VPSS_CHN VpssChn;
    VENC_CHN VeChn[VENC_MAX_CHN_NUM];
    HI_S32   s32Cnt;
    SIZE_S   stSize;
} SAMPLE_VENC_QPMAP_SENDFRAME_PARA_S;


typedef struct hiSAMPLE_VI_DUMP_THREAD_INFO_S
{
    VI_PIPE     ViPipe;
    HI_S32      s32Cnt;
    HI_BOOL     bDump;
    HI_CHAR     aszName[128];
    pthread_t   ThreadId;
} SAMPLE_VI_DUMP_THREAD_INFO_S;

typedef struct hiSAMPLE_SENSOR_INFO_S
{
    SAMPLE_SNS_TYPE_E   enSnsType;
    HI_S32              s32SnsId;
    HI_S32              s32BusId;
    combo_dev_t           MipiDev;
} SAMPLE_SENSOR_INFO_S;

typedef struct hiSAMPLE_SNAP_INFO_S
{
    HI_BOOL  bSnap;
    HI_BOOL  bDoublePipe;
    VI_PIPE    VideoPipe;
    VI_PIPE    SnapPipe;
    VI_VPSS_MODE_E  enVideoPipeMode;
    VI_VPSS_MODE_E  enSnapPipeMode;
}SAMPLE_SNAP_INFO_S;

typedef struct hiSAMPLE_DEV_INFO_S
{
    VI_DEV      ViDev;
    WDR_MODE_E  enWDRMode;
} SAMPLE_DEV_INFO_S;

typedef struct hiSAMPLE_PIPE_INFO_S
{
    VI_PIPE         aPipe[WDR_MAX_PIPE_NUM];
    VI_VPSS_MODE_E  enMastPipeMode;
    HI_BOOL         bMultiPipe;
    HI_BOOL         bVcNumCfged;
    HI_BOOL         bIspBypass;
    PIXEL_FORMAT_E  enPixFmt;
    HI_U32          u32VCNum[WDR_MAX_PIPE_NUM];
} SAMPLE_PIPE_INFO_S;

typedef struct hiSAMPLE_CHN_INFO_S
{
    VI_CHN              ViChn;
    PIXEL_FORMAT_E      enPixFormat;
    DYNAMIC_RANGE_E     enDynamicRange;
    VIDEO_FORMAT_E      enVideoFormat;
    COMPRESS_MODE_E     enCompressMode;
} SAMPLE_CHN_INFO_S;

typedef struct hiSAMPLE_VI_INFO_S
{
    SAMPLE_SENSOR_INFO_S    stSnsInfo;
    SAMPLE_DEV_INFO_S       stDevInfo;
    SAMPLE_PIPE_INFO_S      stPipeInfo;
    SAMPLE_CHN_INFO_S       stChnInfo;
    SAMPLE_SNAP_INFO_S      stSnapInfo;
} SAMPLE_VI_INFO_S;

typedef struct hiSAMPLE_VI_CONFIG_S
{
    SAMPLE_VI_INFO_S    astViInfo[VI_MAX_DEV_NUM];
    HI_S32              as32WorkingViId[VI_MAX_DEV_NUM];
    HI_S32              s32WorkingViNum;
} SAMPLE_VI_CONFIG_S;

typedef struct hiSAMPLE_VI_FRAME_CONFIG_S
{
    HI_U32                  u32Width;
    HI_U32                  u32Height;
    HI_U32                  u32ByteAlign;
    PIXEL_FORMAT_E          enPixelFormat;
    VIDEO_FORMAT_E          enVideoFormat;
    COMPRESS_MODE_E         enCompressMode;
    DYNAMIC_RANGE_E         enDynamicRange;
} SAMPLE_VI_FRAME_CONFIG_S;

typedef struct hiSAMPLE_VI_FRAME_INFO_S
{
    VB_BLK             VbBlk;
    HI_U32             u32Size;
    VIDEO_FRAME_INFO_S stVideoFrameInfo;
} SAMPLE_VI_FRAME_INFO_S;

// typedef struct hiSAMPLE_VI_FPN_CALIBRATE_INFO_S
// {
//     HI_U32                  u32Threshold;
//     HI_U32                  u32FrameNum;
//     ISP_FPN_TYPE_E          enFpnType;
//     PIXEL_FORMAT_E          enPixelFormat;
//     COMPRESS_MODE_E         enCompressMode;
// } SAMPLE_VI_FPN_CALIBRATE_INFO_S;

// typedef struct hiSAMPLE_VI_FPN_CORRECTION_INFO_S
// {
//     ISP_OP_TYPE_E           enOpType;
//     ISP_FPN_TYPE_E          enFpnType;
//     HI_U32                  u32Strength;
//     PIXEL_FORMAT_E          enPixelFormat;
//     COMPRESS_MODE_E         enCompressMode;
//     SAMPLE_VI_FRAME_INFO_S  stViFrameInfo;
// } SAMPLE_VI_FPN_CORRECTION_INFO_S;

typedef struct tag_SAMPLE_VO_WBC_CONFIG
{
    VO_WBC_SOURCE_TYPE_E    enSourceType;
    DYNAMIC_RANGE_E         enDynamicRange;
    COMPRESS_MODE_E         enCompressMode;
    HI_S32 s32Depth;

    HI_S32                  VoWbc;
    VO_WBC_ATTR_S           stWbcAttr;
    VO_WBC_SOURCE_S         stWbcSource;
    VO_WBC_MODE_E           enWbcMode;

}SAMPLE_VO_WBC_CONFIG;

typedef struct hiSAMPLE_COMM_VO_LAYER_CONFIG_S
{
    /* for layer */
    VO_LAYER                VoLayer;
    VO_INTF_SYNC_E          enIntfSync;
    RECT_S                  stDispRect;
    SIZE_S                  stImageSize;
    PIXEL_FORMAT_E          enPixFormat;

    HI_U32                  u32DisBufLen;
    DYNAMIC_RANGE_E         enDstDynamicRange;

    /* for chn */
    SAMPLE_VO_MODE_E        enVoMode;
}SAMPLE_COMM_VO_LAYER_CONFIG_S;

typedef struct hiSAMPLE_VO_CONFIG_S
{
    /* for device */
    VO_DEV                  VoDev;
    VO_INTF_TYPE_E          enVoIntfType;
    VO_INTF_SYNC_E          enIntfSync;
    PIC_SIZE_E              enPicSize;
    HI_U32                  u32BgColor;

    /* for layer */
    PIXEL_FORMAT_E          enPixFormat;
    RECT_S                  stDispRect;
    SIZE_S                  stImageSize;
    VO_PART_MODE_E          enVoPartMode;

    HI_U32                  u32DisBufLen;
    DYNAMIC_RANGE_E         enDstDynamicRange;

    /* for chnnel */
    SAMPLE_VO_MODE_E        enVoMode;
} SAMPLE_VO_CONFIG_S;


typedef enum hiTHREAD_CONTRL_E
{
    THREAD_CTRL_START,
    THREAD_CTRL_PAUSE,
    THREAD_CTRL_STOP,
}THREAD_CONTRL_E;

typedef struct hiVDEC_THREAD_PARAM_S
{
    HI_S32 s32ChnId;
    PAYLOAD_TYPE_E enType;
    HI_CHAR cFilePath[128];
    HI_CHAR cFileName[128];
    HI_S32 s32StreamMode;
    HI_S32 s32MilliSec;
    HI_S32 s32MinBufSize;
    HI_S32 s32IntervalTime;
    THREAD_CONTRL_E eThreadCtrl;
    HI_U64  u64PtsInit;
    HI_U64  u64PtsIncrease;
    HI_BOOL bCircleSend;
}VDEC_THREAD_PARAM_S;

typedef struct hiSAMPLE_VDEC_BUF
{
    HI_U32  u32PicBufSize;
    HI_U32  u32TmvBufSize;
    HI_BOOL bPicBufAlloc;
    HI_BOOL bTmvBufAlloc;
}SAMPLE_VDEC_BUF;


// typedef struct hiSAMPLE_VDEC_VIDEO_ATTR
// {
//     VIDEO_DEC_MODE_E enDecMode;
//     HI_U32              u32RefFrameNum;
//     DATA_BITWIDTH_E  enBitWidth;
// }SAMPLE_VDEC_VIDEO_ATTR;

typedef struct hiSAMPLE_VDEC_PICTURE_ATTR
{
    PIXEL_FORMAT_E enPixelFormat;
    HI_U32         u32Alpha;
}SAMPLE_VDEC_PICTURE_ATTR;


// typedef struct hiSAMPLE_VDEC_ATTR
// {
//     PAYLOAD_TYPE_E enType;
//     VIDEO_MODE_E   enMode;
//     HI_U32 u32Width;
//     HI_U32 u32Height;
//     HI_U32 u32FrameBufCnt;
//     HI_U32 u32DisplayFrameNum;
//     union
//     {
//         SAMPLE_VDEC_VIDEO_ATTR stSapmleVdecVideo;      /* structure with video ( h265/h264) */
//         SAMPLE_VDEC_PICTURE_ATTR stSapmleVdecPicture; /* structure with picture (jpeg/mjpeg )*/
//     };
// }SAMPLE_VDEC_ATTR;

typedef struct hiSAMPLE_VB_BASE_INFO_S
{
    PIXEL_FORMAT_E      enPixelFormat;
    HI_U32              u32Width;
    HI_U32              u32Height;
    HI_U32              u32Align;
    COMPRESS_MODE_E     enCompressMode;
}SAMPLE_VB_BASE_INFO_S;

typedef struct hiSAMPLE_VB_CAL_CONFIG_S
{
    HI_U32 u32VBSize;

    HI_U32 u32HeadStride;
    HI_U32 u32HeadSize;
    HI_U32 u32HeadYSize;

    HI_U32 u32MainStride;
    HI_U32 u32MainSize;
    HI_U32 u32MainYSize;

    HI_U32 u32ExtStride;
    HI_U32 u32ExtYSize;
}SAMPLE_VB_CAL_CONFIG_S;

/*******************************************************
    function announce
*******************************************************/

HI_S32 SAMPLE_COMM_SYS_GetPicSize(PIC_SIZE_E enPicSize, SIZE_S* pstSize);
// HI_VOID SAMPLE_COMM_SYS_Exit(void);
HI_S32 SAMPLE_COMM_SYS_Init(VB_CONFIG_S* pstVbConfig);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


#endif /* End of #ifndef __SAMPLE_COMMON_H__ */
