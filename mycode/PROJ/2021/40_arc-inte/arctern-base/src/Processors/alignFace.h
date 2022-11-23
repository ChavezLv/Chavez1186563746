/****************************************************************************
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         chengaohua
 *  Last modified:  2020.08.25
 *  email:          chen.gaohua@intellif.com
 ****************************************************************************/

#ifndef SRC_PROCESSORS_ALGORITHMS_ALIGNFACE_H_
#define SRC_PROCESSORS_ALGORITHMS_ALIGNFACE_H_

#include "opencv2/opencv.hpp"

namespace arctern {
    cv::Mat similarTransform(cv::Mat src, cv::Mat dst);
}   // namespace arctern
#endif  // SRC_PROCESSORS_ALGORITHMS_ALIGNFACE_H_
