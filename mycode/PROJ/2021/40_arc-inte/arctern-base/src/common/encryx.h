/****************************************************************************
 *  Filename:       aes_cryptor.cpp
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         wxm
 *  Last modified:  2020.05.11
 *  email:          wxm@intellif.com
 ****************************************************************************/

#ifndef SRC_COMMON_ENCRYX_H_
#define SRC_COMMON_ENCRYX_H_

#include <string>
#include <vector>
#ifdef WIN32
#include <cstdint>
#endif

namespace arctern {

enum class ENCRYX_STATUS : int {
  SUCCESS                    =  0,     /// < success return
  ERROR_CODELENGTH_MISMATCH  = -1,     /// < code length is mismatch
};
//

///////////////////////////////////////////////////////////////////////////////////
// Utility function to encrypt face-embedding vectors.
// Note that this encrypter is not safe enough for security applications.

namespace intellif {
// Function to encrypt the input embedidng vector.
// Caller owns the encrypted_embedding.
// bool EmbeddingEncrypter(const std::vector<uint8_t>
// &embbedding,std::vector<uint8_t>& encrypted_embedding);

// Function to decode the embedding encrypted by the function above.
// Caller owns the embedding.
// bool EmbeddingDecrypter(std::vector<uint8_t>
// &encrypted_embedding,std::vector<uint8_t>& embedding);

bool EmbeddingEncrypter(uint8_t* embedding, int Le,
                        uint8_t* encrypted_embedding);
bool EmbeddingDecrypter(uint8_t* ori_encrypted_embedding, int L,
                        uint8_t* embedding);
}  // namespace intellif

/////////////////////////////////////////////////////////////////////////

double elapsed();

// use to load pq model, return PQ model pointer
/************************************
input:
        pfile -- PQ model file pointer
output:
        dim, nlist, ksub, M, table_size -- some parameters in PQ model
*************************************/
void gen_feat(float* feat, int d);

void print_code(uint8_t* xcode, int Lc);

// encode input x to PQ code and encrypt it
ENCRYX_STATUS x2encry(float* x, int Lx, uint8_t* encoded_msg, int Lc);

ENCRYX_STATUS encry2x(float* x, int Lx, uint8_t* encoded_msg, int Lc);

}  // namespace arctern
#endif  // SRC_COMMON_ENCRYX_H_
