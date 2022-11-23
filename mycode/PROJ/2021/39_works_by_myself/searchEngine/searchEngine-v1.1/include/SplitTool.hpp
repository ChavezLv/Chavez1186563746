/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：SplitTool.hpp
*   @date       ：2021年09月04日 20:41:13
*   @description：
*
================================================================*/


#ifndef __WD_SPLITTOOL_HPP__
#define __WD_SPLITTOOL_HPP__


#include "TemplateSingleton.hpp"
#include "cppjieba/Jieba.hpp"

#include <iostream>
#include <vector>
#include <string>

namespace wd{

const char* const DICT_PATH = "./data/dict/jieba.dict.utf8";
const char* const HMM_PATH = "./data/dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "./data/dict/user.dict.utf8";

/* const char* const IDF_PATH = "../data/dict/idf.utf8";//not need for simhash's jieba*/
/* const char* const STOP_WORD_PATH = "../data/dict/stop_words.utf8";//not need for CutForSearch */

class SplitTool;
using SplitToolPtr = SplitTool*; 
//因为jieba类的每次初始化有点耗时, 导致客户端卡顿, 所以设置为单例类了

class SplitTool
{
public:
    SplitTool()
    : _jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH)
    {
        std::cout << "cppjieba init success!" << std::endl;
    }

    std::vector<std::string> operator()(const char *src)
    {
        std::vector<std::string> words;
        // _jieba.CutAll(src, words);
        _jieba.CutForSearch(src, words);
        return words;
    }

private:
    cppjieba::Jieba _jieba;
};

}//end of namespace wd

#endif //__WD_SPLITTOOL_HPP__
