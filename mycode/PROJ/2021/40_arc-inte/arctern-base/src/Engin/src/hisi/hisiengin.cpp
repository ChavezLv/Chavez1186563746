/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.23
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include "hisiengin.h"
#include "HisiEngineInitParameter.h"
#include "HisiEngineManager.h"
#include "hisienginUtil.h"
#include "log/logger.h"
#include "nnie/sample_comm_svp.h"
using arctern::HisiEngin;
using arctern::ErrorType;
using arctern::Result;

using arctern::HisiEngineInitParameter;
using arctern::HisiEngineRunParameter;

#define SVP_WK_QUANT_BASE 4096.0

HisiEngin::HisiEngin() {
  return;
}

HisiEngin::~HisiEngin() {
  HisiEngin::Release();
}

ErrorType HisiEngin::Init(const InitParameter *p) {

 // std::cout << "**************11" << std::endl;
  if (p == nullptr) {
    SPDLOG_ERROR("INIT parameter is error type when init engin!!!");
    return ErrorType::ERR_BAD_INIT_PARAMETER_TYPE;
  }

  //std::cout << "**************44" << std::endl;

  HisiEngineInitParameter *tmp = dynamic_cast<HisiEngineInitParameter *>(p);
  this->loadModel(tmp->mxnetModel_, tmp->batchNum_);
 // std::cout << "**************55" << std::endl;

  memset(&s_stCnnNnieParam, 0, sizeof(SAMPLE_SVP_NNIE_PARAM_S));
//  std::cout << "**************66" << std::endl;

  HI_S32 s32Ret = HI_SUCCESS;

  stNnieCfg.u32MaxInputNum = tmp->batchNum_;///u32MaxInputNum; //max input image num in each batch
  stNnieCfg.u32MaxRoiNum = 0;

#ifdef AARCH64_HIMIX100 // in HIMIX100 (3559AV100), users can choose nnie core
  switch(gpu_id_) {
    case 0:
      stNnieCfg.aenNnieCoreId[0] = SVP_NNIE_ID_0;
      break;
    case 1:
      stNnieCfg.aenNnieCoreId[0] = SVP_NNIE_ID_1;
      break;
    default:
      stNnieCfg.aenNnieCoreId[0] = SVP_NNIE_ID_0;
      break;
  }
#else // if not def AARCH64_HIMIX100 (that is, if in 3516 or 3519)
  stNnieCfg.aenNnieCoreId[0] = SVP_NNIE_ID_0;
#endif // if def AARCH64_HIMIX100 or not

#if DEBUG
  std::cout << "running in nnie core " << stNnieCfg.aenNnieCoreId[0] << std::endl;
#endif

  /*CNN parameter initialization*/
  s_stCnnNnieParam.pstModel = &s_stNNIEModel->s_stCnnModel.stModel;
  /*init hardware para*/
  s32Ret = SAMPLE_COMM_SVP_NNIE_ParamInit(&stNnieCfg, &s_stCnnNnieParam);
  //std::cout << "**************77" << std::endl;
  if (s32Ret != HI_SUCCESS) {
    std::cerr << "param init failed!" << std::endl;
    return ErrorType::ERR_UNDEFINED;
  }

  return ErrorType::ERR_SUCCESS;
}

void HisiEngin::Release() {
  //SAMPLE_COMM_SVP_NNIE_UnloadModel(&s_stCnnModel);
  SAMPLE_COMM_SVP_NNIE_ParamDeinit(&s_stCnnNnieParam);
  return;
}

bool HisiEngin::loadModel(const MxnetModel *model, int batch) {
  auto s_hashstr = nnie_calc_md5(model);

  if (s_hashstr.size() == 0) {
    std::cout << "memory optimization warning: savingid is not set!!!" << __LINE__ << std::endl;
  }
  auto storage = nnie_get_storage();
  s_stNNIEModel = storage->try_get_model(s_hashstr);
  if (!s_stNNIEModel) {
    s_stNNIEModel.reset(new NNIEModel(s_hashstr));
    if (!s_stNNIEModel->init(model)) {
      std::cerr << "init netmodel failed!" << std::endl;
      return false;
    }
    storage->save_model(s_stNNIEModel);
  }

  return true;
}

int HisiEngin::forward(const Tensor<uint8> &input_tensor,
                       std::vector<arctern::Tensor<float>> &outputTensors) {
  std::lock_guard<std::mutex> lock(mtx_);
  // tensor shape is NCHW
  //auto start = std::chrono::high_resolution_clock::now();
//  HI_S32 s32Ret = HI_SUCCESS;
 // SVP_NNIE_HANDLE SvpNnieHandle;
 // SVP_NNIE_ID_E enNnieId = SVP_NNIE_ID_0;
 // HI_BOOL bInstant = HI_TRUE;
 // HI_BOOL bFinish = HI_FALSE;
 // HI_BOOL bBlock = HI_TRUE;
  //tensor to nnie
  int tensor_n = NNIE_set_input(input_tensor);
  SAMPLE_SVP_NNIE_INPUT_DATA_INDEX_S stInputDataIdx = {0};
  SAMPLE_SVP_NNIE_PROCESS_SEG_INDEX_S stProcSegIdx = {0};
  // stInputDataIdx.u32SegIdx = 0;
  // stInputDataIdx.u32NodeIdx = 0;
  // stProcSegIdx.u32SegIdx = 0;

  HI_S32 s32Ret = SAMPLE_SVP_NNIE_Forward(&s_stCnnNnieParam, &stInputDataIdx, &stProcSegIdx, HI_TRUE);

  assert(HI_SUCCESS == s32Ret);

  //std::vector<Tensor<float>> outputs;
  int dstNodeNum = s_stCnnNnieParam.pstModel->astSeg[0].u16DstNum;
  for (int i = 0; i < dstNodeNum; ++i) {
    //HI_U32 n = s_stCnnNnieParam.astSegData[0].astDst[i].u32Num;
    HI_U32 c = s_stCnnNnieParam.astSegData[0].astDst[i].unShape.stWhc.u32Chn;
    HI_U32 h = s_stCnnNnieParam.astSegData[0].astDst[i].unShape.stWhc.u32Height;
    HI_U32 w = s_stCnnNnieParam.astSegData[0].astDst[i].unShape.stWhc.u32Width;
    Tensor<float> output(TensorShape(tensor_n, c, h, w));
    outputTensors.push_back(output);
  }
  //nnie to tensor
  NNIE_ouput2tensor(outputTensors);

  return 0;
}

bool HisiEngin::input_shape_plain_to_piled(const int *plain_shapes,
                                           const size_t plain_shapes_num, const int *plain_idxes,
                                           const size_t plain_idxes_num,
                                           std::vector<TensorShape> &piled_shapes) {
  if (plain_shapes == nullptr || plain_shapes_num <= 0) {
    return false;
  }
  if (plain_idxes == nullptr || plain_idxes_num <= 1) {
    TensorShape shape(0, 0, 0, 0);
    for (int i = 0; i < std::min<int>(4, plain_shapes_num); ++i) {
      shape[i] = plain_shapes[i];
    }
    piled_shapes.push_back(shape);
    return true;
  } else {
    for (size_t i = 0; i < plain_idxes_num - 1; ++i) {
      TensorShape shape(0, 0, 0, 0);
      size_t plain_shapes_start_idx = plain_idxes[i];

      if (plain_shapes_start_idx >= plain_shapes_num) {
        break;
      }
      size_t plain_shapes_end_idx = plain_idxes[i + 1];
      if (plain_shapes_start_idx >= plain_shapes_end_idx) {
        piled_shapes.clear();
        return false;
      }
      for (int j = 0; j < std::min<int>(4,
                                        plain_shapes_end_idx - plain_shapes_start_idx); ++j) {
        shape[j] = plain_shapes[plain_shapes_start_idx + j];
      }
      piled_shapes.push_back(shape);
    }
    return true;
  }
}
const Result *HisiEngin::Process(const RunParameter *p) {
#ifdef HISI_ALLOWED
  std::cout << "ok ,use hisi nnie  process!!" << std::endl;
#else
  SPDLOG_ERROR(" ERROR, MXNET ENGIN NOT ALLOWED!!!! %s");
  result_.errorType_ = ErrorType::ERR_BAD_ENGIN;
  return static_cast<Result *>(&result_);
#endif
  HisiEngineRunParameter *mp = dynamic_cast<HisiEngineRunParameter *>(p);
  this->forward(*(mp->input_tensor), *(mp->outputs));

  result_.errorType_ = ErrorType::ERR_SUCCESS;
  return &result_;
}

ErrorType HisiEngin::SetParameter(ConfigParameter *p) {
  return ErrorType::ERR_SUCCESS;
}

const StatusParameter *HisiEngin::GetParameter(StatusParameter *p) {
  return nullptr;
}

int HisiEngin::NNIE_set_input(const Tensor<HI_U8> &input_tensor) {
  HI_U8 *input_nnie_data_ptr = (HI_U8 *) (s_stCnnNnieParam.astSegData[0].astSrc[0].u64VirAddr);
  int n = s_stCnnNnieParam.astSegData[0].astSrc[0].u32Num;
  int c = s_stCnnNnieParam.astSegData[0].astSrc[0].unShape.stWhc.u32Chn;
  int h = s_stCnnNnieParam.astSegData[0].astSrc[0].unShape.stWhc.u32Height;
  int w = s_stCnnNnieParam.astSegData[0].astSrc[0].unShape.stWhc.u32Width;
  int stride = s_stCnnNnieParam.astSegData[0].astSrc[0].u32Stride;
  TensorShape input_tensor_shape = input_tensor.shape();
  int tensor_n = input_tensor_shape[0];
  const HI_U8 *input_tensor_data_ptr = input_tensor.data();
  //assert(input_tensor_shape[0] == n && input_tensor_shape[1] == c && input_tensor_shape[2] == h && input_tensor_shape[3] == w);
  for (int i = 0; i < tensor_n; ++i) {
    for (int j = 0; j < c; ++j) {
      for (int p = 0; p < h; ++p) {
        memcpy(input_nnie_data_ptr, input_tensor_data_ptr, w * sizeof(HI_U8));
        input_nnie_data_ptr += stride;
        input_tensor_data_ptr += w;
      }
    }
  }
  SAMPLE_COMM_SVP_FlushCache(s_stCnnNnieParam.astSegData[0].astSrc[0].u64PhyAddr,
                             (HI_VOID *) s_stCnnNnieParam.astSegData[0].astSrc[0].u64VirAddr,
                             n * c * h * stride);
  return tensor_n;
}

HI_S32 HisiEngin::SAMPLE_SVP_NNIE_Forward(SAMPLE_SVP_NNIE_PARAM_S *pstNnieParam,
                                          SAMPLE_SVP_NNIE_INPUT_DATA_INDEX_S *pstInputDataIdx,
                                          SAMPLE_SVP_NNIE_PROCESS_SEG_INDEX_S *pstProcSegIdx, HI_BOOL bInstant) {
  HI_S32 s32Ret = HI_SUCCESS;
  HI_U32 i = 0, j = 0;
  HI_BOOL bFinish = HI_FALSE;
  SVP_NNIE_HANDLE hSvpNnieHandle = 0;
  HI_U32 u32TotalStepNum = 0;

  SAMPLE_COMM_SVP_FlushCache(pstNnieParam->astForwardCtrl[pstProcSegIdx->u32SegIdx].stTskBuf.u64PhyAddr,
                             (HI_VOID *) pstNnieParam->astForwardCtrl[pstProcSegIdx->u32SegIdx].stTskBuf.u64VirAddr,
                             pstNnieParam->astForwardCtrl[pstProcSegIdx->u32SegIdx].stTskBuf.u32Size);

  /*set input blob according to node name*/
  if (pstInputDataIdx->u32SegIdx != pstProcSegIdx->u32SegIdx) {
    for (i = 0; i < pstNnieParam->pstModel->astSeg[pstProcSegIdx->u32SegIdx].u16SrcNum; i++) {
      for (j = 0; j < pstNnieParam->pstModel->astSeg[pstInputDataIdx->u32SegIdx].u16DstNum; j++) {
        if (0 == strncmp(pstNnieParam->pstModel->astSeg[pstInputDataIdx->u32SegIdx].astDstNode[j].szName,
                         pstNnieParam->pstModel->astSeg[pstProcSegIdx->u32SegIdx].astSrcNode[i].szName,
                         SVP_NNIE_NODE_NAME_LEN)) {
          pstNnieParam->astSegData[pstProcSegIdx->u32SegIdx].astSrc[i] =
              pstNnieParam->astSegData[pstInputDataIdx->u32SegIdx].astDst[j];
          break;
        }
      }
    }
  }
  /*NNIE_Forward*/
  s32Ret = HI_MPI_SVP_NNIE_Forward(&hSvpNnieHandle,
                                   pstNnieParam->astSegData[pstProcSegIdx->u32SegIdx].astSrc,
                                   pstNnieParam->pstModel, pstNnieParam->astSegData[pstProcSegIdx->u32SegIdx].astDst,
                                   &pstNnieParam->astForwardCtrl[pstProcSegIdx->u32SegIdx], bInstant);

  if (s32Ret != HI_SUCCESS) { std::cout << "nnie forward error: " << std::hex << s32Ret << std::dec << std::endl; }
  assert(s32Ret == HI_SUCCESS);
  if (bInstant) {
    /*Wait NNIE finish*/
    while (HI_ERR_SVP_NNIE_QUERY_TIMEOUT
        == (s32Ret = HI_MPI_SVP_NNIE_Query(pstNnieParam->astForwardCtrl[pstProcSegIdx->u32SegIdx].enNnieId,
                                           hSvpNnieHandle, &bFinish, HI_TRUE))) {
      usleep(100);
      //SAMPLE_SVP_TRACE(SAMPLE_SVP_ERR_LEVEL_INFO,
      //"HI_MPI_SVP_NNIE_Query Query timeout!\n");
    }
  }

  bFinish = HI_FALSE;
  for (i = 0; i < pstNnieParam->astForwardCtrl[pstProcSegIdx->u32SegIdx].u32DstNum; i++) {
    if (SVP_BLOB_TYPE_SEQ_S32 == pstNnieParam->astSegData[pstProcSegIdx->u32SegIdx].astDst[i].enType) {
      for (j = 0; j < pstNnieParam->astSegData[pstProcSegIdx->u32SegIdx].astDst[i].u32Num; j++) {
        u32TotalStepNum +=
            *((HI_U32 *) (pstNnieParam->astSegData[pstProcSegIdx->u32SegIdx].astDst[i].unShape.stSeq.u64VirAddrStep)
                + j);
      }
      SAMPLE_COMM_SVP_FlushCache(pstNnieParam->astSegData[pstProcSegIdx->u32SegIdx].astDst[i].u64PhyAddr,
                                 (HI_VOID *) pstNnieParam->astSegData[pstProcSegIdx->u32SegIdx].astDst[i].u64VirAddr,
                                 u32TotalStepNum
                                     * pstNnieParam->astSegData[pstProcSegIdx->u32SegIdx].astDst[i].u32Stride);

    } else {
      SAMPLE_COMM_SVP_FlushCache(pstNnieParam->astSegData[pstProcSegIdx->u32SegIdx].astDst[i].u64PhyAddr,
                                 (HI_VOID *) pstNnieParam->astSegData[pstProcSegIdx->u32SegIdx].astDst[i].u64VirAddr,
                                 pstNnieParam->astSegData[pstProcSegIdx->u32SegIdx].astDst[i].u32Num *
                                     pstNnieParam->astSegData[pstProcSegIdx->u32SegIdx].astDst[i].unShape.stWhc.u32Chn *
                                     pstNnieParam->astSegData[pstProcSegIdx->u32SegIdx].astDst[i].unShape.stWhc.u32Height
                                     *
                                         pstNnieParam->astSegData[pstProcSegIdx->u32SegIdx].astDst[i].u32Stride);
    }
  }

  return s32Ret;
}

void HisiEngin::NNIE_ouput2tensor(std::vector<Tensor<float>> &outputs) {
//  HI_U32 u32ElemSize = sizeof(HI_S32);
  int tensor_n = outputs[0].shape()[0];
  for (size_t i = 0; i < outputs.size(); ++i) {
    HI_FLOAT *tensor_data_ptr = outputs[i].data();
//    HI_U32 n = s_stCnnNnieParam.astSegData[0].astDst[i].u32Num;
    HI_U32 c = s_stCnnNnieParam.astSegData[0].astDst[i].unShape.stWhc.u32Chn;
    HI_U32 h = s_stCnnNnieParam.astSegData[0].astDst[i].unShape.stWhc.u32Height;
    HI_U32 w = s_stCnnNnieParam.astSegData[0].astDst[i].unShape.stWhc.u32Width;
    HI_U32 stride = s_stCnnNnieParam.astSegData[0].astDst[i].u32Stride;

    HI_U8 *NNIE_output = (HI_U8 *) s_stCnnNnieParam.astSegData[0].astDst[i].u64VirAddr;
    for (int k = 0; k < tensor_n; ++k) {
      for (HI_U32 j = 0; j < c; ++j) {
        for (HI_U32 p = 0; p < h; ++p) {
          for (HI_U32 q = 0; q < w; ++q) {
            int current_tensor_index = k * c * h * w + j * h * w + p * w + q;
            int current_NNIE_output_index = k * c * h * stride + j * h * stride + p * stride + q * sizeof(HI_S32);
            tensor_data_ptr[current_tensor_index] =
                *((HI_S32 *) (NNIE_output + current_NNIE_output_index)) * 1.0f / SVP_WK_QUANT_BASE;
          }
        }
      }
    }
  }
}

