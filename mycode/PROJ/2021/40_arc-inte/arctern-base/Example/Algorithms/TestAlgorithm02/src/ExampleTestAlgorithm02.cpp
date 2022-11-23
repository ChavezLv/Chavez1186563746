/****************************************************************************
 *  Filename:       ExampleTestAlgorithm02.cpp
 *  Copyright:      Copyright © 2020 intellifusion Inc. All rights Reserved
 *  Description:
 *  author:         juwenqi
 *  Last modified:  2020.02.20
 *  email:          ju.wenqi@intellif.com
 ****************************************************************************/
#include <iostream>
#include <string>
#include "opencv2/opencv.hpp"
#include "./include/export_errorcode.h"
#include "./src/Algorithms/TestAlgorithm02/src/testalgorithminterface02.h"

int main() {
    arctern::ErrorType ret0(0, "Success");
    arctern::ErrorType ret1(arctern::ErrorType::ERR_SUCCESS);
    arctern::TestAlgorithmInterface02  algo;
    arctern::InitParameter initp;
    initp.licensefile_path = "../../../../Data/sig.lic";
    arctern::RunParameter runp;
    std::cout << "=-===== juwenqi 01 " << std::endl;
    arctern::ErrorType ret = algo.Init(&initp);
    std::cout << "=-===== juwenqi 02 " << std::endl;
    if (ret != arctern::ErrorType::ERR_SUCCESS) {
        std::cerr << "Init error ocurr!!" << std::endl;
    }
    arctern::TestAlgorithmResult02 result;
    result = *((const arctern::TestAlgorithmResult02*)algo.Process(&runp));
    if (result.errorType_ != arctern::ErrorType::ERR_SUCCESS) {
         std::cerr << "Process error ocurr!!" << std::endl;
     }
    algo.Release();
    return 0;
}
