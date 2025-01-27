#include "../include/Configuration.hpp"
#include "../include/DictProducer.hpp"
#include "../include/SplitToolJieba.hpp"
#include <iostream>
#include <fstream>

using std::cout;
using std::endl;
using std::ifstream;
using std::vector;
using std::string;

void test()
{
    wd::Configuration config("../conf/myconf.conf"); 
    wd::SplitTool *pCppjieba= new wd::SplitToolCppJieba(config);
    ifstream ifs("../data/cnArt/C3-Art0009.txt",std::ios::ate);
        size_t lenth = ifs.tellg();
        char *buff = new char[lenth + 1](); 
        ifs.seekg(0);
        ifs.read(buff, lenth);
        string content (buff);
        vector<string> tmp = pCppjieba->cut(content);
        cout << "before" << endl;
        for (auto &word : tmp){
            cout << word << endl;
        }
        cout << "after" << endl;
}
int main(int argc,char *argv[])
{
    /* test(); */
    wd::Configuration config("../conf/myconf.conf"); 
    wd::SplitTool *pCppjieba= new wd::SplitToolCppJieba(config);
    wd::DictProducer enDictProducer(config.getConfigMap()["EN_ART_DIR"]);
    enDictProducer.readStopWords(config.getConfigMap()["STOP_WORD_PATH"].c_str());
    enDictProducer.buildDict();
    /* enDictProducer.showFiles(); */
    /* enDictProducer.showDict(); */
    enDictProducer.storeDict("../data/enDict.dat");
    enDictProducer.buildIndex();
    enDictProducer.storeIndex("../data/enIndexTable.dat");
    
    wd::DictProducer cnDictProducer(config.getConfigMap()["CN_ART_DIR"], pCppjieba);
    cnDictProducer.readStopWords(config.getConfigMap()["STOP_WORD_PATH"].c_str());
    cnDictProducer.buildCnDict();
    /* cnDictProducer.showFiles(); */
    /* cnDictProducer.showDict(); */
    cnDictProducer.storeDict("../data/cnDict.dat");
    cnDictProducer.buildIndex();
    cnDictProducer.storeIndex("../data/cnIndexTable.dat");
    
    return 0;
}

