#define __LOG4CPP__
#include "../../libs/Mylog.hh"
#include "../../include/locallib/Configuration.hh"
#include "../../include/locallib/DictProducer.hh"
#include "../../include/locallib/SplitToolCppJieba.hh"

#include <string.h>

#include <iostream>
using std::cout;
using std::endl;

int main(int args, char** argv)
{
    if(args != 3) {
        cout << "需要参数：配置文件路径 {en/cn}\n"
            << "如：../data/conf en" << endl;
        exit(1);
    }

    wd::Configuration conf(argv[1]);

    if(strcmp(argv[2], "en") == 0) {
        wd::DictProducer dict(conf.getConfigMap()["enCorpusDir"]);
        dict.build_dict(conf.getConfigMap()["stopEnPath"].c_str());
        dict.store_dict(conf.getConfigMap()["enDictPath"].c_str());
    } else if (strcmp(argv[2], "cn") == 0) {
        wd::SplitToolCppJieba jieba(conf);
        wd::DictProducer dict(conf.getConfigMap()["cnCorpusDir"], &jieba);
        dict.build_cn_dict(conf.getConfigMap()["stopCnPath"].c_str());
        dict.store_dict(conf.getConfigMap()["cnDictPath"].c_str());
    } else {
        cout << "需要参数：配置文件路径 {en/cn}\n"
            << "如：../data/conf en" << endl;
        exit(1);
    }
    return 0;
}

