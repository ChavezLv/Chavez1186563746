//
// Created by Admin on 2021/7/2.
//

#ifndef ARCTERN_BASE_SRC_ALGORITHMS_PERSONCOMPLETENESS_UTIL_PERSONCOMPLETENESSUTIL_H_
#define ARCTERN_BASE_SRC_ALGORITHMS_PERSONCOMPLETENESS_UTIL_PERSONCOMPLETENESSUTIL_H_
#include <vector>
namespace arctern{
extern void parse_truncation(const float * out, int vsize,
                             std::vector<float> &truncation) ;

extern void parse_visible_parts(const float *out, int vsize,
                                std::vector<bool> &visible_parts,
                                std::vector<float> &visible_parts_score) ;

extern void parse_soft_mask( const float * out, int vsize,
                            std::vector<std::vector<bool>> &soft_mask,
                            std::vector<std::vector<float>> &soft_mask_score,
                             int rows = 16, int cols = 8, float thresh = 0.1) ;

}
#endif //ARCTERN_BASE_SRC_ALGORITHMS_PERSONCOMPLETENESS_UTIL_PERSONCOMPLETENESSUTIL_H_
