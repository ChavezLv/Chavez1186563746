/****************************************************************************
 *  Filename:       aes_cryptor.cpp
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         wxm
 *  Last modified:  2020.05.11
 *  email:          wxm@intellif.com
 ****************************************************************************/

#include "./encryx.h"
#include <float.h>
#include <vector>
#include <string>
#include <utility>
#include <ctime>
#if defined(__SSE2__) || defined(_WIN32) || defined(_WIN64)
#include <emmintrin.h>
#include <tmmintrin.h>
#elif defined(__ARM_NEON__) || defined(__ARM_NEON)
#include <arm_neon.h>
#endif
#include <memory.h>
#include <random>

#ifndef WIN32
#include <sys/time.h>
#endif

namespace arctern {

#if defined(__ARM_NEON__) || defined(__ARM_NEON)
#define int8x16_to_8x8x2(v) ((int8x8x2_t){vget_low_s8(v), vget_high_s8(v)})
#endif

double elapsed() {
#ifndef WIN32
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1e3 + tv.tv_usec * 1e-3;
#else
  return 0.0;
#endif
}

void gen_feat(float *feat, int d) {
  float temp = 0.0;
  for (int i = 0; i < d; i++) {
    feat[i] = rand() / static_cast<float>(RAND_MAX) * 2.0 - 1.0;
    temp += (feat[i] * feat[i]);
  }
  temp = sqrt(temp);
  for (int i = 0; i < d; i++) {
    feat[i] = feat[i] / temp;
  }
}

void print_code(uint8_t *xcode, int Lc) {
  for (int i = 0; i < Lc; i++) printf("%d ", xcode[i]);
  printf("\n");
}

namespace intellif {
namespace {
const unsigned int LCN_MOD = 2147483647;  // 2^31 -1
const unsigned int LCN_A = 48271;
// Generate the order to shuffle and reverse the shuffle of 16 elements.
// A sample forwward and backward pair for 4 bytes would be:
// forward: 2,1,3,0
// backward: 3,1,0,2
// which shuffles an array of A,B,C,D to C,B,D,A.
void GenerateShuffuleOrder(char *forward, char *backward) {
  unsigned int rnd = static_cast<int>(std::time(nullptr));
  for (int i = 0; i < 16; ++i) {
    forward[i] = static_cast<char>(i);
  }
  for (int i = 16 - 1; i > 1; --i) {
    rnd = (rnd * LCN_A) % LCN_MOD;
    int j = rnd % i;
    std::swap(forward[i], forward[j]);
  }
  for (int i = 0; i < 16; ++i) {
    backward[static_cast<int>(forward[i])] = static_cast<char>(i);
  }
}
}  // namespace

bool EmbeddingEncrypter(uint8_t *embedding, int Le,
                        uint8_t *encrypted_embedding) {
  if ((Le % 16) != 0) {
    printf("embedding size %% 16 != 0 size %d\n", Le);
    return false;
  }

  char forward[16];
  char backward[16];
  GenerateShuffuleOrder(forward, backward);
  std::vector<uint8_t> encrypted_embedding_tmp(Le);

#if (defined(__SSE2__) || defined(_WIN32) || defined(_WIN64))
  //  printf("******Test Come here\n");
  __m128i shuffle_order = _mm_loadu_si128((__m128i *) (&forward));

  // encrypted_embedding.resize(embedding.size());
  for (int i = 0; i < Le; i += 16) {
    __m128i original = _mm_loadu_si128((__m128i *) (embedding + i));
    __m128i shuffled = _mm_shuffle_epi8(original, shuffle_order);
    _mm_storeu_si128((__m128i *) (encrypted_embedding_tmp.data() + i), shuffled);
  }
#elif(defined(__ARM_NEON__) || defined(__ARM_NEON))
  int8x16_t shuffle_order = vld1q_s8((int8_t*)forward);

  for (int i = 0; i < Le; i += 16) {
    int8x16_t original = vld1q_s8((int8_t*)(embedding + i));
    /*!
          int8x16_t shuffled = _mm_shuffle_epi8(original, shuffle_order);
          uint8x16_t c = vcombine_u8(vtbl2_u8(uint8x16_to_8x8x2(a),
       vget_low_u8(b)), vtbl2_u8(uint8x16_to_8x8x2(a), vget_high_u8(b)));
    */
    int8x16_t shuffled = vcombine_s8(
        vtbl2_s8(int8x16_to_8x8x2(original), vget_low_s8(shuffle_order)),
        vtbl2_s8(int8x16_to_8x8x2(original), vget_high_s8(shuffle_order)));
    vst1q_s8((int8_t*)(encrypted_embedding_tmp.data() + i), shuffled);
  }
#else
  printf("encryptor: neither arm neon nor sse defined\n");
  assert(false);
#endif

  int location = (int) backward[12] + (int) backward[13] + (int) backward[14] +
      (int) backward[15];
  for (int i = 0; i < 16; ++i) {
    if (i < 12)
      encrypted_embedding_tmp.insert(
          encrypted_embedding_tmp.begin() + location + i, backward[i]);
    else
      encrypted_embedding_tmp.insert(encrypted_embedding_tmp.end(),
                                     backward[i]);
  }
  ////////////////////////////////////////////////////////////////////////////

  // std::memcpy(const_cast<char *>(encrypted_embedding->data()) +
  // embedding.size(),
  // static_cast<const void *>(&backward), sizeof(char) * 16);

  for (int i = 0; i < (Le + 16); i++)
    encrypted_embedding[i] = encrypted_embedding_tmp[i];

  return true;
}

// bool EmbeddingDecrypter(std::vector<uint8_t>& ori_encrypted_embedding,
//                       std::vector<uint8_t>& embedding) {
bool EmbeddingDecrypter(uint8_t *ori_encrypted_embedding, int L,
                        uint8_t *embedding) {
  int embedding_size = L - 16;
  uint8_t *encrypted_embedding = new uint8_t[embedding_size];

  int location =
      ori_encrypted_embedding[L - 4] + ori_encrypted_embedding[L - 3] +
          ori_encrypted_embedding[L - 2] + ori_encrypted_embedding[L - 1];

  memcpy(encrypted_embedding, ori_encrypted_embedding, location);
  memcpy(encrypted_embedding + location,
         ori_encrypted_embedding + location + 12, embedding_size - location);

  uint8_t tmp_key[16];
  memcpy(tmp_key, ori_encrypted_embedding + location, 12);
  memcpy(tmp_key + 12, ori_encrypted_embedding + L - 4, 4);

  if (L % 16 != 0 || L < 16) {
    printf("Decrypter get an invalid input size.\n");
    return false;
  }

  for (int i = 0; i < embedding_size; i += 16) {
    uint8_t *en = &encrypted_embedding[i];
    uint8_t *de = &embedding[i];
    for (int j = 0; j < 16; j++) {
      de[j] = en[tmp_key[j]];
    }
  }

  // printf("decry time %f\n",elapsed()-t0);
  delete[] encrypted_embedding;
  return true;
}
}  // namespace intellif
///////////////////////////////////////////////////////////////////////////////////////

// encode input x to PQ code and encrypt it
ENCRYX_STATUS x2encry(float *x, int Lx, uint8_t *encoded_msg, int Lc) {
  int pqcode_size = Lx * 4;
  int makeup16 = (pqcode_size % 16 == 0 ? 0 : (16 - pqcode_size % 16));

  // printf("pqcode_size %d, makeup16 %d \n",pqcode_size,makeup16);
  if (Lc != pqcode_size + makeup16 + 16) {
    return ENCRYX_STATUS::ERROR_CODELENGTH_MISMATCH;
  }

  uint8_t *xcode = new uint8_t[pqcode_size + makeup16];

  memcpy(xcode, x, pqcode_size);

  // print_code (xcode,pqcode_size);
  intellif::EmbeddingEncrypter(xcode, pqcode_size + makeup16, encoded_msg);

  // print_code (encoded_msg,pqcode_size+makeup16);
  delete[] xcode;
  return ENCRYX_STATUS::SUCCESS;
}

ENCRYX_STATUS encry2x(float *x, int Lx, uint8_t *encoded_msg, int Lc) {
  int pqcode_size = Lx * 4;
  int makeup16 = (pqcode_size % 16 == 0 ? 0 : (16 - pqcode_size % 16));

  int code_length = pqcode_size + makeup16 + 16;

  if (Lc != code_length) {
    return ENCRYX_STATUS::ERROR_CODELENGTH_MISMATCH;
  }

  uint8_t *xcode =
      new uint8_t[code_length - 16];  // contains centroids code and pq code
  intellif::EmbeddingDecrypter(encoded_msg, code_length, xcode);

  memcpy(x, xcode, pqcode_size);

  delete[] xcode;
  return ENCRYX_STATUS::SUCCESS;
}

}  // namespace arctern
