/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.23
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/
#include "hisienginUtil.h"

#ifndef CRYPTOPP_ENABLE_NAMESPACE_WEAK
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#endif

#include <cryptopp/base64.h>
#include <cryptopp/files.h>
#include <cryptopp/hex.h>
#include <cryptopp/md5.h>

using arctern::NNIEModelStorage;
using arctern::NNIEModel;
using arctern::MxnetModel;

static std::unique_ptr<NNIEModelStorage> g_storage;
static std::once_flag g_once;

namespace arctern {
NNIEModelStorage *nnie_get_storage() {
  std::call_once(g_once, []() {
    g_storage.reset(new NNIEModelStorage());
  });
  return g_storage.get();
}

std::string nnie_calc_md5(const MxnetModel *model) {
  auto netParams = model->params();
  auto n = netParams->size();

  std::string hashstr;
  CryptoPP::byte res[16];
  hashstr.resize(16 * 2);
  CryptoPP::Weak1::MD5 md5;
  md5.Update((CryptoPP::byte *) netParams, n);
  md5.Final(res);
  for (size_t i = 0; i < sizeof(res); ++i) {
    sprintf(&hashstr[2 * i], "%02x", res[i]);
  }
  return hashstr;
}
}

bool NNIEModel::init(const MxnetModel *net_model) {
  auto net_params = net_model->params();
  HI_U32 u32Size = net_params->size();
#ifdef HIMIX_CHECK_NNIE_INIT
  SAMPLE_COMM_SVP_CheckSysInit();
#endif // ifdef DHIMIX_CHECK_SVP_INIT

  HI_S32 s32Ret = HI_SUCCESS;

  /*CNN Load model*/
  HI_U64 u64PhyAddr = 0;
  HI_U8 *pu8VirAddr = NULL;

  /*malloc model file mem*/
#ifdef DEBUG
  std::cout << "net model bytes: " << u32Size << std::endl;
#endif // ifdef DEBUG
  s32Ret = HI_MPI_SYS_MmzAlloc((HI_U64 *) &u64PhyAddr, (void **) &pu8VirAddr, NULL, NULL, u32Size);
  if (s32Ret != HI_SUCCESS) {
    std::cerr << "mmz alloc failed! " << std::hex << s32Ret << std::dec << ", size: " << u32Size << std::endl;
    return false;
  }

  s_stCnnModel.stModelBuf.u32Size = (HI_U32) u32Size;
  s_stCnnModel.stModelBuf.u64PhyAddr = u64PhyAddr;
  s_stCnnModel.stModelBuf.u64VirAddr = (HI_U64) pu8VirAddr;

  memcpy((void *) s_stCnnModel.stModelBuf.u64VirAddr, net_params->data(), net_params->size());
  /*load model*/
  s32Ret = HI_MPI_SVP_NNIE_LoadModel(&s_stCnnModel.stModelBuf, &s_stCnnModel.stModel);
  if (s32Ret != HI_SUCCESS) {
    std::cerr << "load model failed!" << std::endl;
    return false;
  }

  return true;
}

NNIEModel::~NNIEModel() {
  SAMPLE_COMM_SVP_NNIE_UnloadModel(&s_stCnnModel);
}

std::shared_ptr<NNIEModel> NNIEModelStorage::try_get_model(const std::string &hashstr) {
  std::lock_guard<std::mutex> lock(s_mtx);
  for (auto &t: s_models) {
    auto sp = t.lock();
    if (sp && sp->s_hashstr == hashstr) {
      return sp;
    }
  }
  return std::shared_ptr<NNIEModel>(); // empty
}
void NNIEModelStorage::save_model(std::shared_ptr<NNIEModel> &model) {
  std::lock_guard<std::mutex> lock(s_mtx);

  s_models.erase(std::remove_if(s_models.begin(), s_models.end(), [](const std::weak_ptr<NNIEModel> &wp) {
    return wp.expired();
  }), s_models.end());

  if (model->s_hashstr.empty())
    return;

  for (auto &t: s_models) {
    auto sp = t.lock();
    if (sp) {
      if (sp->s_hashstr == model->s_hashstr) {
        std::cerr << "error! bug already exists!" << std::endl;
        return;
      }
    }
  }
  s_models.emplace_back(std::weak_ptr<NNIEModel>(model));
}