#ifndef __SAMPLE_COMM_NNIE_H__
#define __SAMPLE_COMM_NNIE_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */
#include "hi_comm_svp.h"
#include "hi_nnie.h"
#include "mpi_nnie.h"
//#include "sample_comm_ive.h"

/*16Byte align*/
#define SAMPLE_SVP_NNIE_ALIGN_16 16
#define SAMPLE_SVP_NNIE_ALIGN16(u32Num) ((u32Num + SAMPLE_SVP_NNIE_ALIGN_16-1) / SAMPLE_SVP_NNIE_ALIGN_16*SAMPLE_SVP_NNIE_ALIGN_16)
/*32Byte align*/
#define SAMPLE_SVP_NNIE_ALIGN_32 32
#define SAMPLE_SVP_NNIE_ALIGN32(u32Num) ((u32Num + SAMPLE_SVP_NNIE_ALIGN_32-1) / SAMPLE_SVP_NNIE_ALIGN_32*SAMPLE_SVP_NNIE_ALIGN_32)

#define SAMPLE_SVP_COORDI_NUM                     4        /*num of coordinates*/
#define SAMPLE_SVP_PROPOSAL_WIDTH                 6        /*the width of each proposal array*/
#define SAMPLE_SVP_QUANT_BASE                     4096     /*the basic quantity*/
#define SAMPLE_SVP_NNIE_MAX_SOFTWARE_MEM_NUM      4
#define SAMPLE_SVP_NNIE_SSD_REPORT_NODE_NUM       12
#define SAMPLE_SVP_NNIE_SSD_PRIORBOX_NUM          6
#define SAMPLE_SVP_NNIE_SSD_SOFTMAX_NUM           6
#define SAMPLE_SVP_NNIE_SSD_ASPECT_RATIO_NUM      6
#define SAMPLE_SVP_NNIE_YOLOV1_WIDTH_GRID_NUM     7
#define SAMPLE_SVP_NNIE_YOLOV1_HEIGHT_GRID_NUM    7
#define SAMPLE_SVP_NNIE_EACH_SEG_STEP_ADDR_NUM    2
#define SAMPLE_SVP_NNIE_MAX_CLASS_NUM             30
#define SAMPLE_SVP_NNIE_MAX_ROI_NUM_OF_CLASS      50
#define SAMPLE_SVP_NNIE_REPORT_NAME_LENGTH        64

typedef struct hiSAMPLE_SVP_NNIE_MODEL_S
{
    SVP_NNIE_MODEL_S    stModel;
    SVP_MEM_INFO_S      stModelBuf;//store Model file
}SAMPLE_SVP_NNIE_MODEL_S;


/*each seg input and output memory*/
typedef struct hiSAMPLE_SVP_NNIE_SEG_DATA_S
{
	SVP_SRC_BLOB_S astSrc[SVP_NNIE_MAX_INPUT_NUM];
	SVP_DST_BLOB_S astDst[SVP_NNIE_MAX_OUTPUT_NUM];
}SAMPLE_SVP_NNIE_SEG_DATA_S;

/*each seg input and output data memory size*/
typedef struct hiSAMPLE_SVP_NNIE_BLOB_SIZE_S
{
	HI_U32 au32SrcSize[SVP_NNIE_MAX_INPUT_NUM];
	HI_U32 au32DstSize[SVP_NNIE_MAX_OUTPUT_NUM];
}SAMPLE_SVP_NNIE_BLOB_SIZE_S;

/*NNIE Execution parameters */
typedef struct hiSAMPLE_SVP_NNIE_PARAM_S
{
    SVP_NNIE_MODEL_S*    pstModel;
    HI_U32 u32TmpBufSize;
    HI_U32 au32TaskBufSize[SVP_NNIE_MAX_NET_SEG_NUM];
    SVP_MEM_INFO_S      stTaskBuf;
	SVP_MEM_INFO_S      stTmpBuf;
    SVP_MEM_INFO_S      stStepBuf;//store Lstm step info
    SAMPLE_SVP_NNIE_SEG_DATA_S astSegData[SVP_NNIE_MAX_NET_SEG_NUM];//each seg's input and output blob
    SVP_NNIE_FORWARD_CTRL_S astForwardCtrl[SVP_NNIE_MAX_NET_SEG_NUM];
	SVP_NNIE_FORWARD_WITHBBOX_CTRL_S astForwardWithBboxCtrl[SVP_NNIE_MAX_NET_SEG_NUM];
}SAMPLE_SVP_NNIE_PARAM_S;

/*NNIE input or output data index*/
typedef struct hiSAMPLE_SVP_NNIE_DATA_INDEX_S
{
	HI_U32 u32SegIdx;
	HI_U32 u32NodeIdx;
}SAMPLE_SVP_NNIE_DATA_INDEX_S;

/*this struct is used to indicate the input data from which seg's input or report node*/
typedef SAMPLE_SVP_NNIE_DATA_INDEX_S  SAMPLE_SVP_NNIE_INPUT_DATA_INDEX_S;
/*this struct is used to indicate which seg will be executed*/
typedef SAMPLE_SVP_NNIE_DATA_INDEX_S  SAMPLE_SVP_NNIE_PROCESS_SEG_INDEX_S;

typedef enum hiSAMPLE_SVP_NNIE_NET_TYPE_E
{
	SAMPLE_SVP_NNIE_ALEXNET_FASTER_RCNN       =  0x0,  /*FasterRcnn Alexnet*/
	SAMPLE_SVP_NNIE_VGG16_FASTER_RCNN         =  0x1,  /*FasterRcnn Vgg16*/
	SAMPLE_SVP_NNIE_PVANET_FASTER_RCNN       = 0x2, /*pavenet fasterRcnn  <wz>*/

	SAMPLE_SVP_NNIE_NET_TYPE_BUTT
}SAMPLE_SVP_NNIE_NET_TYPE_E;


/*NNIE configuration parameter*/
typedef struct hiSAMPLE_SVP_NNIE_CFG_S
{
    HI_CHAR *pszPic;
    HI_U32 u32MaxInputNum;
    HI_U32 u32MaxRoiNum;
    HI_U64 au64StepVirAddr[SAMPLE_SVP_NNIE_EACH_SEG_STEP_ADDR_NUM*SVP_NNIE_MAX_NET_SEG_NUM];//virtual addr of LSTM's or RNN's step buffer
	SVP_NNIE_ID_E	aenNnieCoreId[SVP_NNIE_MAX_NET_SEG_NUM];
}SAMPLE_SVP_NNIE_CFG_S;


 /*****************************************************************************
 *   Prototype    : SAMPLE_COMM_SVP_NNIE_ParamDeinit
 *   Description  : Deinit NNIE parameters
 *   Input        : SAMPLE_SVP_NNIE_PARAM_S        *pstNnieParam     NNIE Parameter
 *                  SAMPLE_SVP_NNIE_SOFTWARE_MEM_S *pstSoftWareMem   software mem
 *
 *
 *
 *
 *   Output       :
 *   Return Value :  HI_S32,HI_SUCCESS:Success,Other:failure
 *   Spec         :
 *   Calls        :
 *   Called By    :
 *   History:
 *
 *       1.  Date         : 2017-11-20
 *           Author       :
 *           Modification : Create
 *
 *****************************************************************************/
 HI_S32 SAMPLE_COMM_SVP_NNIE_ParamDeinit(SAMPLE_SVP_NNIE_PARAM_S *pstNnieParam);

 /*****************************************************************************
 *   Prototype    : SAMPLE_COMM_SVP_NNIE_ParamInit
 *   Description  : Init NNIE  parameters
 *   Input        : SAMPLE_SVP_NNIE_CFG_S   *pstNnieCfg    NNIE configure parameter
 *                  SAMPLE_SVP_NNIE_PARAM_S *pstNnieParam    NNIE parameters
 *
 *
 *
 *   Output       :
 *   Return Value : HI_S32,HI_SUCCESS:Success,Other:failure
 *   Spec         :
 *   Calls        :
 *   Called By    :
 *   History:
 *
 *       1.  Date         : 2017-11-20
 *           Author       :
 *           Modification : Create
 *
 *****************************************************************************/
 HI_S32 SAMPLE_COMM_SVP_NNIE_ParamInit(SAMPLE_SVP_NNIE_CFG_S *pstNnieCfg,
     SAMPLE_SVP_NNIE_PARAM_S *pstNnieParam);

 /*****************************************************************************
 *   Prototype    : SAMPLE_COMM_SVP_NNIE_UnloadModel
 *   Description  : unload NNIE model
 *   Input        : SAMPLE_SVP_NNIE_MODEL_S *pstNnieModel      NNIE Model
 *
 *
 *
 *   Output       :
 *   Return Value : HI_S32,HI_SUCCESS:Success,Other:failure
 *   Spec         :
 *   Calls        :
 *   Called By    :
 *   History:
 *
 *       1.  Date         : 2017-11-20
 *           Author       :
 *           Modification : Create
 *
 *****************************************************************************/
 HI_S32 SAMPLE_COMM_SVP_NNIE_UnloadModel(SAMPLE_SVP_NNIE_MODEL_S *pstNnieModel);

 /*****************************************************************************
 *   Prototype    : SAMPLE_COMM_SVP_NNIE_LoadModel
 *   Description  : load NNIE model
 *   Input        : HI_CHAR                 * pszModelFile    Model file name
 *                  SAMPLE_SVP_NNIE_MODEL_S *pstNnieModel     NNIE Model
 *
 *
 *
 *   Output       :
 *   Return Value : HI_S32,HI_SUCCESS:Success,Other:failure
 *   Spec         :
 *   Calls        :
 *   Called By    :
 *   History:
 *
 *       1.  Date         : 2017-11-20
 *           Author       :
 *           Modification : Create
 *
 *****************************************************************************/
 HI_S32 SAMPLE_COMM_SVP_NNIE_LoadModel(HI_CHAR * pszModelFile,
     SAMPLE_SVP_NNIE_MODEL_S *pstNnieModel);

/*****************************************************************************
*   Prototype    : SAMPLE_COMM_SVP_NNIE_FillRect
*   Description  : Draw rect
*   Input        : VIDEO_FRAME_INFO_S             *pstFrmInfo   Frame info
* 		            SAMPLE_SVP_NNIE_RECT_ARRAY_S  *pstRect       Rect
*                  HI_U32                         u32Color      Color
*
*
*   Output       :
*   Return Value : HI_S32
*   Spec         :
*   Calls        :
*   Called By    :
*   History:
*
*       1.  Date         : 2017-03-14
*           Author       :
*           Modification : Create
*
*****************************************************************************/
//HI_S32 SAMPLE_COMM_SVP_NNIE_FillRect(VIDEO_FRAME_INFO_S *pstFrmInfo, SAMPLE_SVP_NNIE_RECT_ARRAY_S* pstRect, HI_U32 u32Color);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __SAMPLE_COMM_NNIE_H__ */
