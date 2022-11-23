#include "IVELKTracker.h"

static HI_S32 SAMPLE_IVE_St_Lk_DMA(IVE_HANDLE *pIveHandle, IVE_SRC_IMAGE_S *pstSrc,
                                IVE_DST_IMAGE_S *pstDst, IVE_DMA_CTRL_S *pstDmaCtrl, HI_BOOL bInstant) {
    HI_S32 s32Ret = HI_FAILURE;
    IVE_SRC_DATA_S stDataSrc;
    IVE_DST_DATA_S stDataDst;

    stDataSrc.u64VirAddr    = pstSrc->au64VirAddr[0];
    stDataSrc.u64PhyAddr    = pstSrc->au64PhyAddr[0];
    stDataSrc.u32Width      = pstSrc->u32Width;
    stDataSrc.u32Height     = pstSrc->u32Height;
    stDataSrc.u32Stride     = pstSrc->au32Stride[0];

    stDataDst.u64VirAddr    = pstDst->au64VirAddr[0];
    stDataDst.u64PhyAddr    = pstDst->au64PhyAddr[0];
    stDataDst.u32Width      = pstDst->u32Width;
    stDataDst.u32Height     = pstDst->u32Height;
    stDataDst.u32Stride     = pstDst->au32Stride[0];
    s32Ret = HI_MPI_IVE_DMA(pIveHandle, &stDataSrc, &stDataDst,pstDmaCtrl,bInstant);
    SAMPLE_CHECK_EXPR_RET(HI_SUCCESS != s32Ret,s32Ret,"Error(%#x),HI_MPI_IVE_DMA failed!\n",s32Ret);
    if (s32Ret != HI_SUCCESS) {
      std::cout << "src: " << stDataSrc.u64VirAddr << ", " << stDataSrc.u64PhyAddr << ", " << stDataSrc.u32Width << ", " << stDataSrc.u32Height << ", " << stDataSrc.u32Stride << std::endl;
      std::cout << "dst: " << stDataDst.u64VirAddr << ", " << stDataDst.u64PhyAddr << ", " << stDataDst.u32Width << ", " << stDataDst.u32Height << ", " << stDataDst.u32Stride << std::endl;
    }

    return s32Ret;
}

static HI_S32 SAMPLE_IVE_St_Lk_CopyPyr(IVE_SRC_IMAGE_S astPyrSrc[], IVE_DST_IMAGE_S astPyrDst[], HI_U8 u8MaxLevel) {
    HI_U8 i;
    HI_S32 s32Ret = HI_FAILURE;
    IVE_HANDLE hIveHandle;

    IVE_DMA_CTRL_S stCtrl;
    memset(&stCtrl,0,sizeof(stCtrl));
    stCtrl.enMode = IVE_DMA_MODE_DIRECT_COPY;

    for (i = 0; i <= u8MaxLevel; i++) {
        s32Ret = SAMPLE_IVE_St_Lk_DMA(&hIveHandle,&astPyrSrc[i],&astPyrDst[i],&stCtrl,HI_FALSE);
        SAMPLE_CHECK_EXPR_RET(HI_SUCCESS != s32Ret,s32Ret,"Error(%#x), SAMPLE_IVE_St_Lk_DMA failed!\n",s32Ret);
    }
    return s32Ret;
}

HI_S32 if_IVE_LK_uninit(IVE_LK_TRACK* pstStLk) {
    for (HI_U16 i = 0; i <= pstStLk->stLkPyrCtrl.u8MaxLevel; i++) {
        IVE_MMZ_FREE(pstStLk->astPrevPyr[i].au64PhyAddr[0], pstStLk->astPrevPyr[i].au64VirAddr[0]);
        IVE_MMZ_FREE(pstStLk->astNextPyr[i].au64PhyAddr[0], pstStLk->astNextPyr[i].au64VirAddr[0]);
    }

    IVE_MMZ_FREE(pstStLk->stPrevPts.u64PhyAddr, pstStLk->stPrevPts.u64VirAddr);
    IVE_MMZ_FREE(pstStLk->stNextPts.u64PhyAddr, pstStLk->stNextPts.u64VirAddr);
    IVE_MMZ_FREE(pstStLk->stStatus.u64PhyAddr, pstStLk->stStatus.u64VirAddr);
    IVE_MMZ_FREE(pstStLk->stErr.u64PhyAddr, pstStLk->stErr.u64VirAddr);

    IVE_MMZ_FREE(pstStLk->stTmp.au64PhyAddr[0], pstStLk->stTmp.au64VirAddr[0]);

    memset(pstStLk, 0, sizeof(*pstStLk));
    return HI_SUCCESS;
}

HI_S32 if_IVE_LK_init(IVE_LK_TRACK* pstStLk, const arctern::ObjectTrackerParameter& track_params) {
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32Size = 0;
    HI_U16 i;

#ifdef HIMIX_CHECK_NNIE_INIT
    SAMPLE_COMM_IVE_CheckIveMpiInit();
#endif // ifdef HIMIX_CHECK_SYS_INIT
    memset(pstStLk, 0, sizeof(IVE_LK_TRACK));

    pstStLk->img_change_scale = 1;
    pstStLk->img_change_scale_h = 1;
    pstStLk->img_width = track_params.ive_input_img_width;
    pstStLk->img_height = track_params.ive_input_img_height;
    if(pstStLk->img_width % 8 != 0 || pstStLk->img_height % 8 != 0){
        std::cerr << "track_params.ive_input_img_width or track_params.ive_input_img_height error!" << std::endl;
        return HI_FAILURE;
    }

    pstStLk->bbox_resize_scale = track_params.bbox_resize_scale;
    pstStLk->OF_calc_threshold = track_params.OF_calc_threshold;
    pstStLk->OF_point_num_per_row = track_params.OF_point_num_per_row;
    pstStLk->OF_point_num_per_col = track_params.OF_point_num_per_col;
    pstStLk->max_level = track_params.max_level;

    pstStLk->stLkPyrCtrl.enOutMode = IVE_LK_OPTICAL_FLOW_PYR_OUT_MODE_BOTH;//output mode of OF
    pstStLk->stLkPyrCtrl.bUseInitFlow = HI_FALSE;
    pstStLk->stLkPyrCtrl.u16PtsNum = MAX_PTS_NUM;//芯片api参数，设置为最大值500即可
    pstStLk->stLkPyrCtrl.u8MaxLevel = pstStLk->max_level;
    pstStLk->stLkPyrCtrl.u0q8MinEigThr = 2;//最小特征阈值，设置小值，过大导致计算光流失败
    pstStLk->stLkPyrCtrl.u8IterCnt = 15; // 迭代次数
    pstStLk->stLkPyrCtrl.u0q8Eps = 200;//迭代收敛阈值，推荐值2，设置大阈值利于计算得到光流值
    
    //Init Pyr
    for(i = 0; i <= pstStLk->stLkPyrCtrl.u8MaxLevel; i++) {
        s32Ret = SAMPLE_COMM_IVE_CreateImage(&pstStLk->astPrevPyr[i], IVE_IMAGE_TYPE_U8C1,
            pstStLk->img_width >> i, pstStLk->img_height >> i);
        SAMPLE_CHECK_EXPR_RET(HI_SUCCESS != s32Ret,s32Ret,"Error(%#x),SAMPLE_COMM_IVE_CreateImage failed!\n",s32Ret);

        s32Ret = SAMPLE_COMM_IVE_CreateImage(&pstStLk->astNextPyr[i], IVE_IMAGE_TYPE_U8C1,
            pstStLk->astPrevPyr[i].u32Width, pstStLk->astPrevPyr[i].u32Height);
        SAMPLE_CHECK_EXPR_RET(HI_SUCCESS != s32Ret,s32Ret,"Error(%#x),SAMPLE_COMM_IVE_CreateImage failed!\n",s32Ret);
    }
    
    s32Ret = SAMPLE_COMM_IVE_CreateImage(&pstStLk->stTmp, IVE_IMAGE_TYPE_U8C1, pstStLk->img_width, pstStLk->img_height);
    SAMPLE_CHECK_EXPR_RET(HI_SUCCESS != s32Ret,s32Ret,"Error(%#x),SAMPLE_COMM_IVE_CreateImage failed!\n",s32Ret);

    //Init prev pts
    u32Size = sizeof(IVE_POINT_S25Q7_S) * pstStLk->stLkPyrCtrl.u16PtsNum;
    u32Size = SAMPLE_COMM_IVE_CalcStride(u32Size, IVE_ALIGN);
    s32Ret = SAMPLE_COMM_IVE_CreateMemInfo(&(pstStLk->stPrevPts), u32Size);
    SAMPLE_CHECK_EXPR_RET(HI_SUCCESS != s32Ret,s32Ret,"Error(%#x),SAMPLE_COMM_IVE_CreateMemInfo failed!\n",s32Ret);
    //Init next pts
    s32Ret = SAMPLE_COMM_IVE_CreateMemInfo(&(pstStLk->stNextPts), u32Size);
    SAMPLE_CHECK_EXPR_RET(HI_SUCCESS != s32Ret,s32Ret,"Error(%#x),SAMPLE_COMM_IVE_CreateMemInfo failed!\n",s32Ret);

    //Init status
    u32Size = sizeof(HI_U8) *pstStLk->stLkPyrCtrl.u16PtsNum;
    u32Size = SAMPLE_COMM_IVE_CalcStride(u32Size, IVE_ALIGN);
    s32Ret = SAMPLE_COMM_IVE_CreateMemInfo(&(pstStLk->stStatus), u32Size);
    SAMPLE_CHECK_EXPR_RET(HI_SUCCESS != s32Ret,s32Ret,"Error(%#x),SAMPLE_COMM_IVE_CreateMemInfo failed!\n",s32Ret);

    //Init err
    u32Size = sizeof(HI_U9Q7) * pstStLk->stLkPyrCtrl.u16PtsNum;
    u32Size = SAMPLE_COMM_IVE_CalcStride(u32Size, IVE_ALIGN);
    s32Ret = SAMPLE_COMM_IVE_CreateMemInfo(&(pstStLk->stErr), u32Size);
    SAMPLE_CHECK_EXPR_RET(HI_SUCCESS != s32Ret,s32Ret,"Error(%#x),SAMPLE_COMM_IVE_CreateMemInfo failed!\n",s32Ret);

    if (HI_SUCCESS != s32Ret) {
        if_IVE_LK_uninit(pstStLk);
    }
    return s32Ret;
}

static HI_S32 SAMPLE_IVE_St_Lk_PyrDown(IVE_LK_TRACK* pstStLk, IVE_SRC_IMAGE_S* pstSrc, 
                                        IVE_DST_IMAGE_S* pstDst) {
    HI_S32 s32Ret = HI_SUCCESS;
    IVE_HANDLE hIveHandle;
//    IVE_DMA_CTRL_S  stDmaCtrl = {IVE_DMA_MODE_INTERVAL_COPY,
//                                 0, 2, 1, 2
//                                };
//    IVE_FILTER_CTRL_S stFltCtrl = {{
//            1, 2, 3, 2, 1,
//            2, 5, 6, 5, 2,
//            3, 6, 8, 6, 3,
//            2, 5, 6, 5, 2,
//            1, 2, 3, 2, 1
//        }, 7
//    };

    IVE_RESIZE_CTRL_S stResizeCtrl = {
        hiIVE_RESIZE_MODE_E::IVE_RESIZE_MODE_LINEAR,
        {
            pstDst->au64PhyAddr[0],
            pstDst->au64VirAddr[0],
            pstDst->au32Stride[0]*pstDst->u32Height
        },
        1
    };

    pstStLk->stTmp.u32Width = pstSrc->u32Width;
    pstStLk->stTmp.u32Height = pstSrc->u32Height;

/*    s32Ret = HI_MPI_IVE_Filter(&hIveHandle, pstSrc, &pstStLk->stTmp, &stFltCtrl,  HI_FALSE);
    SAMPLE_CHECK_EXPR_RET(HI_SUCCESS != s32Ret,s32Ret,"Error(%#x),HI_MPI_IVE_Filter failed!\n",s32Ret);
*/
    // s32Ret = SAMPLE_IVE_St_Lk_DMA(&hIveHandle,&pstStLk->stTmp,pstDst,&stDmaCtrl,HI_FALSE);
    s32Ret = HI_MPI_IVE_Resize(&hIveHandle, &pstStLk->stTmp, pstDst, &stResizeCtrl, HI_FALSE);

    SAMPLE_CHECK_EXPR_RET(HI_SUCCESS != s32Ret,s32Ret,"Error(%#x), SAMPLE_IVE_St_Lk_DMA failed!\n",s32Ret);

    return s32Ret;
}

static HI_S32 build_pyr(IVE_LK_TRACK* pstStLk, IVE_SRC_IMAGE_S* pyr) {
    IVE_HANDLE hIveHandle;
    HI_S32 s32Ret = HI_SUCCESS;
    IVE_DMA_CTRL_S stDmaCtrl;
    memset(&stDmaCtrl, 0, sizeof(stDmaCtrl));
    stDmaCtrl.enMode = IVE_DMA_MODE_DIRECT_COPY;

/*    s32Ret = SAMPLE_IVE_St_Lk_DMA(&hIveHandle, &pstStLk->stTmp, &pyr[0],
                                      &stDmaCtrl, HI_FALSE);
    SAMPLE_CHECK_EXPR_RET(HI_SUCCESS != s32Ret,s32Ret,"Error(%#x), SAMPLE_IVE_St_Lk_DMA failed!\n",s32Ret);
*/
    IVE_FILTER_CTRL_S stFltCtrl = {{
            1, 2, 3, 2, 1,
            2, 5, 6, 5, 2,
            3, 6, 8, 6, 3,
            2, 5, 6, 5, 2,
            1, 2, 3, 2, 1
        }, 7
    };
    s32Ret = HI_MPI_IVE_Filter(&hIveHandle, &pstStLk->stTmp, &pyr[0], &stFltCtrl,  HI_FALSE);
    SAMPLE_CHECK_EXPR_RET(HI_SUCCESS != s32Ret,s32Ret,"Error(%#x), HI_MPI_IVE_Filter failed!\n",s32Ret);
    
/*    s32Ret = HI_MPI_IVE_Filter(&hIveHandle, &pyr[0], &pstStLk->stTmp, &stFltCtrl,  HI_FALSE);
    SAMPLE_CHECK_EXPR_RET(HI_SUCCESS != s32Ret,s32Ret,"Error(%#x),HI_MPI_IVE_Filter failed!\n",s32Ret);
*/
    // for (HI_U32 k = 1; k <= pstStLk->stLkPyrCtrl.u8MaxLevel; k++) {
    for (HI_U32 k = 1; k < pstStLk->stLkPyrCtrl.u8MaxLevel; k++) {
        s32Ret = SAMPLE_IVE_St_Lk_PyrDown(pstStLk, &pyr[k - 1], &pyr[k]);
        SAMPLE_CHECK_EXPR_RET(HI_SUCCESS != s32Ret,s32Ret,"Error(%#x),SAMPLE_IVE_St_Lk_PyrDown failed!\n",s32Ret);
    }
    return s32Ret;
}

static HI_S32 generate_points(IVE_LK_TRACK* pstStLk, const HI_U32 x1, const HI_U32 y1, 
                                const HI_U32 width, const HI_U32 height, HI_U32 shift){
    //在目标bbox内部中心区域选取待计算光流点坐标，非全bbox选择，排除bbox内目标外的背景干扰
    //例如人体跟踪bbox内人体所占面积较小，选择bbox中心区域选点，使点尽可能落在人体肚皮上，更好反映人体运动趋势
    //选择策略为单纯网格化
    IVE_POINT_S25Q7_S *pre_pts = SAMPLE_COMM_IVE_CONVERT_64BIT_ADDR(IVE_POINT_S25Q7_S, pstStLk->stPrevPts.u64VirAddr);
    pre_pts += shift;

    HI_U32 row_pts = pstStLk->OF_point_num_per_row;
    HI_U32 col_pts = pstStLk ->OF_point_num_per_col;
    HI_U32 step_x = ceil(width / col_pts);
    HI_U32 step_y = ceil(height / row_pts);
    HI_U32 batch_step = ceill(step_y / pstStLk->OF_point_num_per_col);
    HI_U16 tmp_x, tmp_y, cnt = 0;
    for (HI_U32 y = 0; y < row_pts; y++){
        for (HI_U32 x = 0, i = 0; x < col_pts; x++, i += batch_step){
            tmp_x = (HI_U16)(x1 + x * step_x + step_x / 2);
            tmp_y = (HI_U16)(y1 + y * step_y + step_y / 2 + i);
            pre_pts[cnt].s25q7X = (HI_S32)(tmp_x << 7);
            pre_pts[cnt].s25q7Y = (HI_S32)(tmp_y << 7);
            cnt++;
        }
    }
    pre_pts = NULL;
    return HI_SUCCESS;
}

static HI_FLOAT median(std::vector<HI_FLOAT> v) {
    HI_U32 n = floor(v.size() / 2);
    nth_element(v.begin(), v.begin()+n, v.end());
    return v[n];
}

static HI_S32 predict_bbox(IVE_LK_TRACK* pstStLk, std::vector<cv::Point2f> pre_pts, 
                            std::vector<cv::Point2f> next_pts,
                            std::vector<HI_FLOAT>& xoff, std::vector<HI_FLOAT>& yoff, 
                            t_track_face_deil& track_bboxes, const HI_U16 object_cnt) {
    HI_FLOAT scale = pstStLk->img_change_scale;
    HI_FLOAT scale_h = pstStLk->img_change_scale_h;

    HI_U16 point_cnt = xoff.size(); 
    HI_FLOAT dx = median(xoff);
    HI_FLOAT dy = median(yoff);
    HI_FLOAT s; 
    if (point_cnt > 1){
        std::vector<HI_FLOAT> tmp;
        tmp.reserve(point_cnt * (point_cnt - 1) / 2);
        for (HI_U16 i = 0; i < point_cnt; i++){
            for (HI_U16 j = i + 1; j < point_cnt; j++){
                tmp.push_back(norm(next_pts[i] - next_pts[j]) / norm(pre_pts[i] - pre_pts[j]));
            }
        }
        s = median(tmp);
    } else {
        s = 1.0;
    }
    //if(s > SCALE_CHANGE_MAX) {
    //    s = SCALE_CHANGE_MAX;
    //} else if(s < SCALE_CHANGE_MIN) {
    //    s = SCALE_CHANGE_MIN;
    //}
    if(s > SCALE_CHANGE_MAX) {
        s = 1.0;
    } else if(s < SCALE_CHANGE_MIN) {
        s = 1.0;
    }

    HI_FLOAT x1, y1, x2, y2;
    x1 = track_bboxes.face_roi[object_cnt][0] * scale;
    y1 = track_bboxes.face_roi[object_cnt][1] * scale_h;
    x2 = track_bboxes.face_roi[object_cnt][2] * scale;
    y2 = track_bboxes.face_roi[object_cnt][3] * scale_h;
    HI_FLOAT bbox_width = x2 - x1;
    HI_FLOAT bbox_height = y2 - y1;

    HI_FLOAT s1 = 0.5 * (s - 1.) * bbox_width;
    HI_FLOAT s2 = 0.5 * (s - 1.) * bbox_height;
    x1 += dx - s1;
    y1 += dy - s2;
    
    track_bboxes.face_roi[object_cnt][0] = IVE_MAX(0, (HI_S16)(x1 / scale));
    track_bboxes.face_roi[object_cnt][1] = IVE_MAX(0, (HI_S16)(y1 / scale_h));
    track_bboxes.face_roi[object_cnt][2] = (HI_S16)(IVE_MIN(pstStLk->img_width,
                                             (HI_S16)(x1 + bbox_width * s))) / scale;
    track_bboxes.face_roi[object_cnt][3] = (HI_S16)(IVE_MIN(pstStLk->img_height, 
                                             (HI_S16)(y1 + bbox_height * s))) / scale_h;
    return HI_SUCCESS;
}

HI_S32 if_IVE_LK_update(IVE_LK_TRACK* pstStLk, const t_track_face_deil& track_bboxes) {
    // TIME_DEBUG_START("[if_IVE_LK_update]: start");
//    HI_U32 i, k;
    HI_S32 s32Ret = HI_SUCCESS;
   // HI_U32 x1, y1, width, height;
    build_pyr(pstStLk, pstStLk->astPrevPyr);
    // TIME_DEBUG_END("[if_IVE_LK_track]: all postprocess ended");
    return s32Ret;
}

HI_S32 if_IVE_LK_track(IVE_LK_TRACK* pstStLk, t_track_face_deil& track_bboxes) {
    HI_S32 s32Ret = HI_SUCCESS;
    if(track_bboxes.face_num == 0) {
        SAMPLE_IVE_St_Lk_CopyPyr(pstStLk->astNextPyr, pstStLk->astPrevPyr, pstStLk->stLkPyrCtrl.u8MaxLevel);
        return s32Ret;
    }
    // TIME_DEBUG_START("[if_IVE_LK_track]: start");
    HI_U32 face_num_each_epoch;
    face_num_each_epoch = MAX_PTS_NUM / pstStLk->OF_point_num_per_row / pstStLk->OF_point_num_per_col;
 
    HI_U32  k;
    HI_U32 changed_x1, changed_y1, changed_width, changed_height;
    cv::Mat tmpImg;
    IVE_HANDLE hIveHandle;
    HI_BOOL bBlock = HI_TRUE;
    HI_BOOL bFinish = HI_FALSE;
    HI_U32 total_point_num = pstStLk->OF_point_num_per_col * pstStLk->OF_point_num_per_row;
    IVE_POINT_S25Q7_S *pre_pts = SAMPLE_COMM_IVE_CONVERT_64BIT_ADDR(IVE_POINT_S25Q7_S, pstStLk->stPrevPts.u64VirAddr);
    IVE_POINT_S25Q7_S *next_pts = SAMPLE_COMM_IVE_CONVERT_64BIT_ADDR(IVE_POINT_S25Q7_S, pstStLk->stNextPts.u64VirAddr);

    for (HI_U32 face_begin = 0, face_end = 0; face_begin < track_bboxes.face_num; ) {
        if(face_begin + face_num_each_epoch < track_bboxes.face_num) {
            face_end = face_begin + face_num_each_epoch;
        }
        else {
            face_end = track_bboxes.face_num;
        }
       // grid to calculate
        for (HI_U32 idx = face_begin; idx < face_end; ++idx) {
            changed_width = (HI_U32)((track_bboxes.face_roi[idx][2] - track_bboxes.face_roi[idx][0]) * 
                            pstStLk->img_change_scale);
            changed_height = (HI_U32)((track_bboxes.face_roi[idx][3] - track_bboxes.face_roi[idx][1]) * 
                            pstStLk->img_change_scale_h);
            changed_x1 = (HI_U32)(track_bboxes.face_roi[idx][0] * pstStLk->img_change_scale + 
                                changed_width * (1 - pstStLk->bbox_resize_scale) / 2);
            changed_y1 = (HI_U32)(track_bboxes.face_roi[idx][1] * pstStLk->img_change_scale_h + 
                                changed_height * (1 - pstStLk->bbox_resize_scale) / 2);
            generate_points(pstStLk, changed_x1, changed_y1, changed_width * pstStLk->bbox_resize_scale,
                            changed_height * pstStLk->bbox_resize_scale, (idx - face_begin) * total_point_num);
        }
        pstStLk->stLkPyrCtrl.u16PtsNum = (face_end - face_begin) * pstStLk->OF_point_num_per_row * pstStLk->OF_point_num_per_col;

        // TIME_DEBUG_LOG("[if_IVE_LK_track]: generate pts");
        if(face_begin == 0) {
            build_pyr(pstStLk, pstStLk->astNextPyr);
        }

        // TIME_DEBUG_LOG("[if_IVE_LK_track]: build pyr");
        s32Ret = HI_MPI_IVE_LKOpticalFlowPyr(&hIveHandle,
                pstStLk->astPrevPyr, pstStLk->astNextPyr,
                &pstStLk->stPrevPts, &pstStLk->stNextPts,
                &pstStLk->stStatus, &pstStLk->stErr,
                &pstStLk->stLkPyrCtrl, HI_TRUE);
        if (HI_SUCCESS != s32Ret) {
        // this erorr log is needed only when have faces to track
            if (track_bboxes.face_num > 0) {
                std::cout << "Error(" << std::hex << s32Ret << std::dec << "), HI_MPI_IVE_LKOpticalFlowPyr failed!" << std::endl; 
            }
            return s32Ret;
        } // s32Ret check

        s32Ret = HI_MPI_IVE_Query(hIveHandle, &bFinish, bBlock);
        while (HI_ERR_IVE_QUERY_TIMEOUT == s32Ret) {
            usleep(1000);
            s32Ret = HI_MPI_IVE_Query(hIveHandle, &bFinish, bBlock);
        }
        SAMPLE_CHECK_EXPR_RET(HI_SUCCESS != s32Ret, s32Ret, "Error(%#x), HI_MPI_IVE_Query failed!\n",s32Ret);

        // TIME_DEBUG_LOG("[if_IVE_LK_track]: calculate OF");
        std::vector<HI_FLOAT> xoff;
        std::vector<HI_FLOAT> yoff;
        std::vector<cv::Point2f> pre_pts_vector, next_pts_vector;

        HI_FLOAT pre_x, pre_y, next_x, next_y;
        HI_FLOAT bbox_width, bbox_height;
        HI_U16 object_cnt = face_begin;
        bbox_width = (track_bboxes.face_roi[object_cnt][2] - track_bboxes.face_roi[object_cnt][0]) * pstStLk->OF_calc_threshold;
        bbox_height = (track_bboxes.face_roi[object_cnt][3] - track_bboxes.face_roi[object_cnt][1]) * pstStLk->OF_calc_threshold;
        for (k = 0; k <= pstStLk->stLkPyrCtrl.u16PtsNum; k++) {
            if(k != 0 && k % total_point_num == 0) {//use for grid pts
                if(xoff.size() != 0)
                    predict_bbox(pstStLk, pre_pts_vector, next_pts_vector, xoff, yoff, track_bboxes, object_cnt);
                pre_pts_vector.clear();
                next_pts_vector.clear();
                xoff.clear();     
                yoff.clear();   
                object_cnt++;  
                bbox_width = (track_bboxes.face_roi[object_cnt][2] - track_bboxes.face_roi[object_cnt][0]) * pstStLk->OF_calc_threshold;
                bbox_height = (track_bboxes.face_roi[object_cnt][3] - track_bboxes.face_roi[object_cnt][1]) * pstStLk->OF_calc_threshold;
                if(k == pstStLk->stLkPyrCtrl.u16PtsNum)
                    break;
            }
            if(!(SAMPLE_COMM_IVE_CONVERT_64BIT_ADDR(HI_U8,pstStLk->stStatus.u64VirAddr))[k]) {
                continue;
            }
            pre_x = (HI_FLOAT)(pre_pts[k].s25q7X >> 7) + (HI_FLOAT)(IVE_MASK & pre_pts[k].s25q7X) / 128.;
            pre_y = (HI_FLOAT)(pre_pts[k].s25q7Y >> 7) + (HI_FLOAT)(IVE_MASK & pre_pts[k].s25q7Y) / 128.;
            next_x = (HI_FLOAT)(next_pts[k].s25q7X >> 7) + (HI_FLOAT)(IVE_MASK & next_pts[k].s25q7X) / 128.;
            next_y = (HI_FLOAT)(next_pts[k].s25q7Y >> 7) + (HI_FLOAT)(IVE_MASK & next_pts[k].s25q7Y) / 128.;

            if(abs(pre_y - next_y) > (bbox_height) || abs(pre_x - next_x) > (bbox_width))
                continue;
            pre_pts_vector.push_back(cv::Point2f(pre_x, pre_y));
            next_pts_vector.push_back(cv::Point2f(next_x, next_y));
            xoff.push_back(next_x - pre_x);
            yoff.push_back(next_y - pre_y);
        }

        face_begin += face_num_each_epoch;
    }

    SAMPLE_IVE_St_Lk_CopyPyr(pstStLk->astNextPyr, pstStLk->astPrevPyr, pstStLk->stLkPyrCtrl.u8MaxLevel);
    pre_pts = NULL;
    next_pts = NULL;
    // TIME_DEBUG_END("[if_IVE_LK_track]: all postprocess ended");
    return s32Ret;
}

