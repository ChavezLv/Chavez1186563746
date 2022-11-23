#include "IVEKCFTracker.h"
#include "IVELKTracker.h"

#include <sys/time.h>




int frame_cnt = 0;
bool _track_to_roi_invoked_ = false;

#ifdef DEBUG
// for test
static HI_VOID _test_show_pstKCF_bbox_decrypted(IVE_KCF_TRACK *pstKCF, const std::string &prompt) {

  HI_FLOAT scale = pstKCF->img_change_scale;
  HI_FLOAT scale_h = pstKCF->img_change_scale_h;

  std::cout << prompt << ": " << pstKCF->u32BboxNum << std::endl;
  for (int i = 0; i < pstKCF->u32BboxNum; ++i) {
    int x1 = (HI_S16)((pstKCF->astBbox[i].stRoiInfo.stRoi.s24q8X / 256 & (~1)) / scale);
    int y1 = (HI_S16)((pstKCF->astBbox[i].stRoiInfo.stRoi.s24q8Y / 256 & (~1)) / scale_h);
    int x2 = (HI_S16)(x1 + (pstKCF->astBbox[i].stRoiInfo.stRoi.u32Width & (~1)) / scale);
    int y2 = (HI_S16)(y1 + (pstKCF->astBbox[i].stRoiInfo.stRoi.u32Height & (~1)) / scale_h);

    std::cout << "  " << x1 << ", " << y1 << ", " << x2 << ", " << y2 << std::endl;
  }
}
#endif // if def DEBUG
// for test ends

HI_VOID IVE_Bbox_to_face_deil(IVE_KCF_TRACK* pstKCF, t_track_face_deil& track_bboxes) {
#ifdef AARCH64_HIMIX200_3516DV300
  HI_FLOAT scale = pstKCF->img_change_scale;
  HI_FLOAT scale_h = pstKCF->img_change_scale_h;

  track_bboxes.face_num = pstKCF->u32BboxNum;
  for (HI_U32 i = 0; i < pstKCF->u32BboxNum; i++) {
    track_bboxes.face_roi[i][0] = (HI_S16)((pstKCF->astBbox[i].stRoiInfo.stRoi.s24q8X / 256 & (~1)) / scale);
    track_bboxes.face_roi[i][1] = (HI_S16)((pstKCF->astBbox[i].stRoiInfo.stRoi.s24q8Y / 256 & (~1)) / scale_h);
    track_bboxes.face_roi[i][2] = (HI_S16)(track_bboxes.face_roi[i][0] + 
                                  (pstKCF->astBbox[i].stRoiInfo.stRoi.u32Width & (~1)) / scale);
    track_bboxes.face_roi[i][3] = (HI_S16)(track_bboxes.face_roi[i][1] + 
                                  (pstKCF->astBbox[i].stRoiInfo.stRoi.u32Height & (~1)) / scale_h);
    if (track_bboxes.face_roi[i][0] < 0)
       track_bboxes.face_roi[i][0] = 0; 
    if (track_bboxes.face_roi[i][1] < 0)
       track_bboxes.face_roi[i][1] = 0; 
    if (track_bboxes.face_roi[i][2] > (pstKCF->img_width / scale))
       track_bboxes.face_roi[i][2] = (pstKCF->img_width / scale); 
    if (track_bboxes.face_roi[i][3] > (pstKCF->img_height / scale_h))
       track_bboxes.face_roi[i][3] = (pstKCF->img_height / scale_h); 

    track_bboxes.face_id[i] = pstKCF->astBbox[i].stRoiInfo.u32RoiId;
  }
  #endif
}

#ifdef AARCH64_HIMIX200_3516DV300
HI_VOID IVE_face_deil_to_new_ROI(IVE_KCF_TRACK* pstKCF, const t_track_face_deil& new_detection_boxes, 
                            IVE_ROI_INFO_S astNewRoiInfo[], HI_U32& u32NewRoiNum) {
    HI_U32 u32PaddW, u32PaddH;
    HI_FLOAT scale = pstKCF->img_change_scale;
    HI_FLOAT scale_h = pstKCF->img_change_scale_h;
    HI_U3Q5 u3q5Padding = pstKCF->u3q5Padding;

    u32NewRoiNum = new_detection_boxes.face_num;
    for(HI_U32 i = 0; i < new_detection_boxes.face_num; i++) {
        astNewRoiInfo[i].stRoi.s24q8X = IVE_MAX(0, new_detection_boxes.face_roi[i][0]) * 256 * scale;
        astNewRoiInfo[i].stRoi.s24q8Y = IVE_MAX(0, new_detection_boxes.face_roi[i][1]) * 256 * scale_h;
        astNewRoiInfo[i].stRoi.u32Width = (new_detection_boxes.face_roi[i][2] - new_detection_boxes.face_roi[i][0]) * scale;
        astNewRoiInfo[i].stRoi.u32Height = (new_detection_boxes.face_roi[i][3] - new_detection_boxes.face_roi[i][1]) * scale_h;
        
        u32PaddW = astNewRoiInfo[i].stRoi.u32Width * u3q5Padding / 32;
        if (u32PaddW > IVE_KCF_ROI_PADDING_MAX_WIDTH) {
            astNewRoiInfo[i].stRoi.u32Width = IVE_KCF_ROI_PADDING_MAX_WIDTH * 32 / u3q5Padding;
        }
        else if (u32PaddW < IVE_KCF_ROI_PADDING_MIN_WIDTH) {
            astNewRoiInfo[i].stRoi.u32Width = IVE_KCF_ROI_PADDING_MIN_WIDTH * 32 / u3q5Padding + 2;
        }

        u32PaddH = astNewRoiInfo[i].stRoi.u32Height * u3q5Padding / 32;
        if (u32PaddH > IVE_KCF_ROI_PADDING_MAX_WIDTH) {
            astNewRoiInfo[i].stRoi.u32Height = IVE_KCF_ROI_PADDING_MAX_HEIGHT * 32 / u3q5Padding;
        }
        else if (u32PaddH < IVE_KCF_ROI_PADDING_MIN_WIDTH) {
            astNewRoiInfo[i].stRoi.u32Height = IVE_KCF_ROI_PADDING_MIN_HEIGHT * 32 / u3q5Padding + 2;
        }

        astNewRoiInfo[i].u32RoiId = new_detection_boxes.face_id[i];
        if (i >= 63) {
            return;
        }
    }
}
#endif

HI_VOID IVE_face_deil_to_pst(IVE_KCF_TRACK* pstKCF, const t_track_face_deil& track_bboxes, 
                            const t_track_face_deil& new_detection_boxes) { 
#ifdef AARCH64_HIMIX200_3516DV300
    HI_U32 u32PaddW, u32PaddH, u32Cnt = 0;
    HI_FLOAT scale = pstKCF->img_change_scale;
    HI_FLOAT scale_h = pstKCF->img_change_scale_h;
    HI_U3Q5 u3q5Padding = pstKCF->u3q5Padding;
    std::unordered_set<HI_U32> dict;

    for (HI_U32 i = 0; i < new_detection_boxes.face_num; i++) {
        dict.insert(new_detection_boxes.face_id[i]);
    }
    
    for (HI_U32 i = 0; i < track_bboxes.face_num; i++) {
        if (dict.find(track_bboxes.face_id[i]) != dict.end()) {
            continue;
        }
        for (HI_U32 j = 0; j < pstKCF->u32BboxNum; j++) {
            if (pstKCF->astBbox[j].stRoiInfo.u32RoiId != track_bboxes.face_id[i]) {
                continue;
            }
            pstKCF->astBbox[j].bRoiRefresh = HI_TRUE;
            pstKCF->astBbox[j].bTrackOk = HI_TRUE;
            pstKCF->astBbox[j].stRoiInfo.stRoi.s24q8X = IVE_MAX(0, track_bboxes.face_roi[i][0]) * 256 * scale;
            pstKCF->astBbox[j].stRoiInfo.stRoi.s24q8Y = IVE_MAX(0, track_bboxes.face_roi[i][1]) * 256 * scale_h;
            pstKCF->astBbox[j].stRoiInfo.stRoi.u32Width = (track_bboxes.face_roi[i][2] - track_bboxes.face_roi[i][0]) * scale;
            pstKCF->astBbox[j].stRoiInfo.stRoi.u32Height = (track_bboxes.face_roi[i][3] - track_bboxes.face_roi[i][1]) * scale_h;
            
            u32PaddW = pstKCF->astBbox[j].stRoiInfo.stRoi.u32Width * u3q5Padding / 32;
            if (u32PaddW > IVE_KCF_ROI_PADDING_MAX_WIDTH) {
                pstKCF->astBbox[j].stRoiInfo.stRoi.u32Width = IVE_KCF_ROI_PADDING_MAX_WIDTH * 32 / u3q5Padding;
            } else if (u32PaddW < IVE_KCF_ROI_PADDING_MIN_WIDTH) {
                pstKCF->astBbox[j].stRoiInfo.stRoi.u32Width = IVE_KCF_ROI_PADDING_MIN_WIDTH * 32 / u3q5Padding + 2;
            }

            u32PaddH = pstKCF->astBbox[j].stRoiInfo.stRoi.u32Height * u3q5Padding / 32;
            if (u32PaddH > IVE_KCF_ROI_PADDING_MAX_WIDTH) {
                pstKCF->astBbox[j].stRoiInfo.stRoi.u32Height = IVE_KCF_ROI_PADDING_MAX_HEIGHT * 32 / u3q5Padding;
            } else if (u32PaddH < IVE_KCF_ROI_PADDING_MIN_WIDTH) {
                pstKCF->astBbox[j].stRoiInfo.stRoi.u32Height = IVE_KCF_ROI_PADDING_MIN_HEIGHT * 32 / u3q5Padding + 2;
            }

            pstKCF->astBbox[j].stRoiInfo.u32RoiId = track_bboxes.face_id[i];
            if (u32Cnt >= 63) {
                return;
            }
        }
    }
#endif
}

HI_S32 if_IVE_KCF_uninit(IVE_KCF_TRACK* pstKCF) {
#ifdef AARCH64_HIMIX200_3516DV300
    (HI_VOID)HI_MPI_IVE_KCF_DestroyObjList(&pstKCF->stObjList);
    IVE_MMZ_FREE(pstKCF->stTotalMem.u64PhyAddr, pstKCF->stTotalMem.u64VirAddr);
#endif // if def AARCH64_HIMIX200_3516DV300

    return HI_SUCCESS;
}

HI_S32 if_IVE_KCF_init(IVE_KCF_TRACK* pstKCF, const arctern::ObjectTrackerParameter& track_params) {
#ifdef AARCH64_HIMIX200_3516DV300
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32Size;
    HI_U32 u32TotalSize;
//    HI_S32 s32Len;
    memset(pstKCF, 0, sizeof(IVE_KCF_TRACK));

    pstKCF->img_change_scale = 1;
    pstKCF->img_change_scale_h = 1;
    pstKCF->img_width = track_params.ive_input_img_width;
    pstKCF->img_height = track_params.ive_input_img_height;
    pstKCF->u32BboxIndex = 0;

    s32Ret = SAMPLE_COMM_IVE_CreateImage(&pstKCF->stSrcImg, IVE_IMAGE_TYPE_YUV420SP, 
                                        track_params.ive_input_img_width, track_params.ive_input_img_height);
    SAMPLE_CHECK_EXPR_RET(HI_SUCCESS != s32Ret, s32Ret, 
                            "Error(%#x),SAMPLE_COMM_IVE_CreateImage failed!\n", s32Ret);

    s32Ret = HI_MPI_IVE_KCF_GetMemSize(IVE_KCF_NODE_MAX_NUM, &u32Size);
    SAMPLE_CHECK_EXPR_RET(HI_SUCCESS != s32Ret, s32Ret, 
                            "Error(%#x),HI_MPI_IVE_KCF_GetMemSize failed!\n", s32Ret);
    /* (HOGFeatrue + Alpha + DstBuf) + Guasspeak + CosWinX + CosWinY + TmpBuf*/
    u32TotalSize = u32Size + IVE_KCF_GAUSS_PEAK_TOTAL_SIZE + IVE_KCF_COS_WINDOW_TOTAL_SIZE * 2 + IVE_KCF_TEMP_BUF_SIZE;

    s32Ret = SAMPLE_COMM_IVE_CreateMemInfo(&pstKCF->stTotalMem, u32TotalSize);
    SAMPLE_CHECK_EXPR_RET(HI_SUCCESS != s32Ret, s32Ret, 
                            "Error(%#x),HI_MPI_IVE_KCF_GetMemSize failed!\n", s32Ret);
    memset((HI_U8*)(HI_UL)pstKCF->stTotalMem.u64VirAddr, 0x0, sizeof(u32TotalSize));
    
    pstKCF->stListMem.u64PhyAddr = pstKCF->stTotalMem.u64PhyAddr;
    pstKCF->stListMem.u64VirAddr = pstKCF->stTotalMem.u64VirAddr;
    pstKCF->stListMem.u32Size = u32Size;

    pstKCF->stGaussPeak.u64PhyAddr = pstKCF->stListMem.u64PhyAddr + u32Size;
    pstKCF->stGaussPeak.u64VirAddr = pstKCF->stListMem.u64VirAddr + u32Size;
    pstKCF->stGaussPeak.u32Size = IVE_KCF_GAUSS_PEAK_TOTAL_SIZE;

    pstKCF->stCosWinX.u64PhyAddr = pstKCF->stGaussPeak.u64PhyAddr + IVE_KCF_GAUSS_PEAK_TOTAL_SIZE;
    pstKCF->stCosWinX.u64VirAddr = pstKCF->stGaussPeak.u64VirAddr + IVE_KCF_GAUSS_PEAK_TOTAL_SIZE;
    pstKCF->stCosWinX.u32Size = IVE_KCF_COS_WINDOW_TOTAL_SIZE;

    pstKCF->stCosWinY.u64PhyAddr = pstKCF->stCosWinX.u64PhyAddr + IVE_KCF_COS_WINDOW_TOTAL_SIZE;
    pstKCF->stCosWinY.u64VirAddr = pstKCF->stCosWinX.u64VirAddr + IVE_KCF_COS_WINDOW_TOTAL_SIZE;
    pstKCF->stCosWinY.u32Size = IVE_KCF_COS_WINDOW_TOTAL_SIZE;

    pstKCF->stKcfProCtrl.stTmpBuf.u64PhyAddr = pstKCF->stCosWinY.u64PhyAddr + IVE_KCF_COS_WINDOW_TOTAL_SIZE;
    pstKCF->stKcfProCtrl.stTmpBuf.u64VirAddr = pstKCF->stCosWinY.u64VirAddr + IVE_KCF_COS_WINDOW_TOTAL_SIZE;
    pstKCF->stKcfProCtrl.stTmpBuf.u32Size = IVE_KCF_TEMP_BUF_SIZE;

    pstKCF->stKcfProCtrl.enCscMode = IVE_CSC_MODE_PIC_BT709_YUV2RGB;
    pstKCF->stKcfProCtrl.u0q16InterFactor = 0.02 * 1024 * 32;
    pstKCF->stKcfProCtrl.u0q16Lamda = 10;
    pstKCF->stKcfProCtrl.u0q8Sigma = 0.5 * 256;
    pstKCF->stKcfProCtrl.u4q12TrancAlfa = 0.2 * 4096;
    pstKCF->stKcfProCtrl.u8RespThr = 32;

    pstKCF->u3q5Padding = 160;
    // pstKCF->u3q5Padding = 1.5 * 32;

    pstKCF->stKcfBboxCtrl.u32MaxBboxNum = IVE_KCF_NODE_MAX_NUM;
    pstKCF->stKcfBboxCtrl.s32RespThr = 0;

    s32Ret = HI_MPI_IVE_KCF_CreateObjList(&pstKCF->stListMem, IVE_KCF_NODE_MAX_NUM, &pstKCF->stObjList);
    SAMPLE_CHECK_EXPR_RET(HI_SUCCESS != s32Ret, s32Ret, 
        "Error(%#x),HI_MPI_IVE_KCF_CreateObjList failed!\n", s32Ret);

    s32Ret = HI_MPI_IVE_KCF_CreateGaussPeak(pstKCF->u3q5Padding, &pstKCF->stGaussPeak);
    SAMPLE_CHECK_EXPR_RET(HI_SUCCESS != s32Ret, s32Ret, 
        "Error(%#x),HI_MPI_IVE_KCF_CreateGaussPeak failed!\n", s32Ret);

    s32Ret = HI_MPI_IVE_KCF_CreateCosWin(&pstKCF->stCosWinX, &pstKCF->stCosWinY);
    SAMPLE_CHECK_EXPR_RET(HI_SUCCESS != s32Ret, s32Ret, 
        "Error(%#x),HI_MPI_IVE_KCF_CreateCosWin failed!\n", s32Ret);

    return s32Ret;
#else // if not def AARCH64_HIMIX200_3516DV300
    return HI_SUCCESS;
#endif // if def AARCH64_HIMIX200_3516DV300 or not
}

HI_S32 if_IVE_KCF_update(IVE_KCF_TRACK* pstKCF, t_track_face_deil& track_bboxes, 
                         t_track_face_deil& new_detection_boxes) {
#ifdef AARCH64_HIMIX200_3516DV300
    //printf("[ive_kcf_tracker::if_IVE_KCF_update]: started\n");
    frame_cnt++;

    IVE_ROI_INFO_S astNewRoiInfo[IVE_KCF_NODE_MAX_NUM] = {0};
    HI_U32 u32NewRoiNum = 0;
    HI_S32 s32Ret;
    //printf("[ive_kcf_tracker::if_IVE_KCF_update]: preprocess done\n");

    if (frame_cnt == 1) {
        IVE_face_deil_to_new_ROI(pstKCF, track_bboxes, astNewRoiInfo, u32NewRoiNum); // this statement should call only once
        //printf("[ive_kcf_tracker::if_IVE_KCF_update]: IVE_face_deil_to_new_ROI-1 done\n");
    } else {
        for (HI_U32 j = 0; j < pstKCF->u32BboxNum; j++) {
            pstKCF->astBbox[j].bTrackOk = HI_FALSE;
        }
        IVE_face_deil_to_pst(pstKCF, track_bboxes, new_detection_boxes);
        //printf("[ive_kcf_tracker::if_IVE_KCF_update]: IVE_face_deil_to_pst done\n");
        IVE_face_deil_to_new_ROI(pstKCF, new_detection_boxes, astNewRoiInfo, u32NewRoiNum);
        //printf("[ive_kcf_tracker::if_IVE_KCF_update]: IVE_face_deil_to_new_ROI-2 done\n");
    }
    s32Ret = HI_MPI_IVE_KCF_ObjUpdate(&pstKCF->stObjList, pstKCF->astBbox,  pstKCF->u32BboxNum);
    SAMPLE_CHECK_EXPR_RET(HI_SUCCESS != s32Ret, s32Ret, 
        "Error(%#x),HI_MPI_IVE_KCF_ObjUpdate failed!\n", s32Ret);
    //printf("[ive_kcf_tracker::if_IVE_KCF_update]: HI_MPI_IVE_KCF_ObjUpdate done\n");

    s32Ret = HI_MPI_IVE_KCF_GetTrainObj(pstKCF->u3q5Padding, astNewRoiInfo, u32NewRoiNum,
                            &pstKCF->stCosWinX, &pstKCF->stCosWinY, &pstKCF->stGaussPeak, &pstKCF->stObjList);
    SAMPLE_CHECK_EXPR_RET(HI_SUCCESS != s32Ret, s32Ret, 
        "Error(%#x),HI_MPI_IVE_KCF_GetTrainObj failed!\n", s32Ret);
    //printf("[ive_kcf_tracker::if_IVE_KCF_update]: HI_MPI_IVE_KCF_GetTrainObj done\n");

    s32Ret = if_IVE_KCF_track(pstKCF, track_bboxes, HI_TRUE);
    SAMPLE_CHECK_EXPR_RET(HI_SUCCESS != s32Ret, s32Ret, 
        "Error(%#x),if_IVE_KCF_track failed!\n", s32Ret);
    pstKCF->u32BboxNum = pstKCF->stObjList.u32TrackObjNum;
    //printf("[ive_kcf_tracker::if_IVE_KCF_update]: if_IVE_KCF_track done\n");

    return s32Ret;
#else // if not def AARCH64_HIMIX200_3516DV300
    return HI_SUCCESS;
#endif // if def AARCH64_HIMIX200_3516DV300 or not
}

HI_S32 if_IVE_KCF_track(IVE_KCF_TRACK* pstKCF, t_track_face_deil& track_bboxes, HI_BOOL change_track_bboxes) {
#ifdef AARCH64_HIMIX200_3516DV300
    //printf("[ive_kcf_tracker::if_IVE_KCF_track]: started\n");
    TIME_DEBUG_START("[IVEKCFTracker::if_IVE_KCF_track]");
    HI_S32 s32Ret = HI_SUCCESS;
    HI_BOOL bBlock = HI_TRUE, bFinish, bInstant = HI_TRUE;
    IVE_HANDLE hIveHandle;
    //printf("[ive_kcf_tracker::if_IVE_KCF_track]: preprocess done\n");

    if (pstKCF->stObjList.u32TrackObjNum != 0 || pstKCF->stObjList.u32TrainObjNum != 0) {
        s32Ret = HI_MPI_IVE_KCF_Process(&hIveHandle, &pstKCF->stSrcImg, &pstKCF->stObjList,
                                        &pstKCF->stKcfProCtrl, bInstant);
        SAMPLE_CHECK_EXPR_RET(HI_SUCCESS != s32Ret, s32Ret, 
            "Error(%#x), HI_MPI_IVE_KCF_Process failed!\n", s32Ret);
        //printf("[ive_kcf_tracker::if_IVE_KCF_track]: HI_MPI_IVE_KCF_Process done\n");

        s32Ret = HI_MPI_IVE_Query(hIveHandle, &bFinish, bBlock);
        while (HI_ERR_IVE_QUERY_TIMEOUT == s32Ret) {
            usleep(100);
            s32Ret = HI_MPI_IVE_Query(hIveHandle, &bFinish, bBlock);
            std::cout << "slept\n" << std::endl;
        }
        //printf("[ive_kcf_tracker::if_IVE_KCF_track]: HI_MPI_IVE_Query done\n");
        TIME_DEBUG_LOG("[IVEKCFTracker::if_IVE_KCF_track]: process done");

        s32Ret = HI_MPI_IVE_KCF_GetObjBbox(&pstKCF->stObjList,  pstKCF->astBbox, &pstKCF->u32BboxNum, &pstKCF->stKcfBboxCtrl);
        SAMPLE_CHECK_EXPR_RET(HI_SUCCESS != s32Ret, s32Ret, 
            "Error(%#x), HI_MPI_IVE_KCF_GetObjBbox failed!\n", s32Ret);
        //printf("[ive_kcf_tracker::if_IVE_KCF_track]: HI_MPI_IVE_KCF_GetObjBbox done\n");
        TIME_DEBUG_LOG("[IVEKCFTracker::if_IVE_KCF_track]: getobjbbox done");

        if (change_track_bboxes) {
            IVE_Bbox_to_face_deil(pstKCF, track_bboxes);
            //printf("[ive_kcf_tracker::if_IVE_KCF_track]: IVE_Bbox_to_face_deil done\n");
            TIME_DEBUG_LOG("[IVEKCFTracker::if_IVE_KCF_track]: to face deil done");
        }
    }
    //printf("[ive_kcf_tracker::if_IVE_KCF_track]: done\n");
    TIME_DEBUG_END("[IVEKCFTracker::if_IVE_KCF_track]: done");
    return s32Ret;
#else // if not def AARCH64_HIMIX200_3516DV300
    return HI_SUCCESS;
#endif // if def AARCH64_HIMIX200_3516DV300 or not
}

