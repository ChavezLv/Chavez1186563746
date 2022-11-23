/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.10.23
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef ARCTERN_SRC_ENGIN_SRC_HISI_HISIENGINUTIL_H_
#define ARCTERN_SRC_ENGIN_SRC_HISI_HISIENGINUTIL_H_

#include <list>
#include <mutex>
#include <memory>
#include "nnie/mpi_sys.h"
#include "nnie/mpi_nnie.h"
#include "nnie/sample_comm_nnie.h"
#include "src/common/arcterndefs_generated.h"
namespace arctern {

class NNIEModel {
 public:
  NNIEModel(const std::string &hashstr) : s_hashstr(hashstr) {
    memset(&s_stCnnModel, 0, sizeof(s_stCnnModel));
  }
  ~NNIEModel();
  bool init(const MxnetModel *net_model);

 public:
  SAMPLE_SVP_NNIE_MODEL_S s_stCnnModel;
  std::string s_hashstr;
};

class NNIEModelStorage // store weak_ptr here to minimize model memory consumption
{
 public:
  std::shared_ptr<NNIEModel> try_get_model(const std::string &hashstr);
  void save_model(std::shared_ptr<NNIEModel> &model);
 private:
  std::mutex s_mtx;
  std::list<std::weak_ptr<NNIEModel>> s_models;
};

std::string nnie_calc_md5(const MxnetModel *model);
NNIEModelStorage *nnie_get_storage();
}

#endif //ARCTERN_SRC_ENGIN_SRC_HISI_HISIENGINUTIL_H_
