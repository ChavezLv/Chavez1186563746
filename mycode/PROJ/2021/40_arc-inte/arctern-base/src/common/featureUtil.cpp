/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.26
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#include <iostream>
#include <vector>
#include "./featureUtil.h"
#include "./encryx.h"
#include "opencv2/opencv.hpp"

std::vector<uint8> arctern::encrypt_feature(const std::vector<float> &feat) {
  const int ndim = feat.size();
  const int pqcode_size = ndim * 4;
  const int makeup16 = (pqcode_size % 16 == 0 ? 0 : (16 - pqcode_size % 16));
  const int codeL = pqcode_size + makeup16 + 16;

  std::vector<uint8> encrypted(codeL);
  if (x2encry(const_cast<float *>(&feat[0]), ndim, &encrypted[0], codeL) ==
      ENCRYX_STATUS::ERROR_CODELENGTH_MISMATCH) {
    printf("ERROR : illegal code length ! EXIT\n");
    exit(0);
  }

  return encrypted;
}

void arctern::encrypt_feature(const std::vector<float> &feat,std::vector<uint8> & encrypted) {
  const int ndim = feat.size();
  const int pqcode_size = ndim * 4;
  const int makeup16 = (pqcode_size % 16 == 0 ? 0 : (16 - pqcode_size % 16));
  const int codeL = pqcode_size + makeup16 + 16;
  encrypted.resize(codeL);
  if (x2encry(const_cast<float *>(&feat[0]), ndim, &encrypted[0], codeL) ==
      ENCRYX_STATUS::ERROR_CODELENGTH_MISMATCH) {
    printf("ERROR : illegal code length ! EXIT\n");
    exit(0);
  }
}

size_t caculate_feature_length(size_t len) {
  return (len - 16) / 4;
}

std::vector<float> arctern::decrypt_feature(const std::vector<uint8> &encrypted) {
  const int ndim = caculate_feature_length(encrypted.size());
  // std::cout << "Feature to be decrypted dimension: " << ndim << std::endl;
  std::vector<float> feat(ndim);
  const int codeL = encrypted.size();
  if (arctern::encry2x(&feat[0], ndim, const_cast<uint8 *>(&encrypted[0]), codeL) ==
      arctern::ENCRYX_STATUS::ERROR_CODELENGTH_MISMATCH) {
    printf("ERROR : illegal code length ! EXIT\n");
    exit(0);
  }

  return feat;
}

//float arctern::compute_similarity(const std::vector<uint8> &encrypted1,
//                         const std::vector<uint8> &encrypted2) {
//  // size > 16 is needed by caculate_feature_length in utils.cpp
//  if (encrypted1.size() != encrypted2.size() || encrypted1.size() <= 16) {
//    std::cerr << "[ArcternManager::compute_similarity]: two features size mismatch, or feature size too short"
//              << std::endl;
//    return 0;
//  }
//
//  std::vector<float> feat1 = decrypt_feature(encrypted1);
//  std::vector<float> feat2 = decrypt_feature(encrypted2);
//  cv::Mat mfeat1(feat1);
//  cv::Mat mfeat2(feat2);
//
//  return mfeat1.dot(mfeat2);
//}
