#include <iostream>
#include <fstream>

//this define can avoid some logs which you don't need to care about.
#define LOGGER_LEVEL LL_WARN 

#include "../include/cppjieba/KeywordExtractor.hpp"
#include "../include/simhash/Simhasher.hpp"
using namespace simhash;

//要在上级目录编译 : c++ test/14simhash-demo.cc -I./include
int main(int argc, char** argv)
{
    Simhasher simhasher("./data/dict/jieba.dict.utf8", "./data/dict/hmm_model.utf8", "./data/dict/idf.utf8", "./data/dict/stop_words.utf8");
    /* string s("我是蓝翔技工拖拉机学院手扶拖拉机专业的。不用多久，我就会升职加薪，当上总经理，出任CEO，走上人生巅峰。"); */
    string s("我是CEO蓝翔蓝翔技工工人多久拖拉机学院手扶多久拖拉机专业的多久。不用多久，我就会升职加薪，当上总经理，出任CEO，走上人生巅峰。");
    string s1("我是CEO蓝翔技工多久拖拉机学院手扶多久拖拉机专业的多久。不用多久，我就会升职加，当上总经理，出任CEO，走上人生巅峰。");
    size_t topN = 10;//这个是返回top几的
    uint64_t u64 = 0;//作为simhash值返回
    uint64_t u64_1 = 0;//作为simhash值返回
    vector<pair<string ,double> > res;
    vector<pair<string ,double> > res1;
    simhasher.extract(s, res, topN);
    simhasher.extract(s1, res1, topN);
    /* simhasher.make(s, topN, u64); */
    simhasher.make(s, 50, u64);//
    simhasher.make(s1, 50, u64_1);// 1-4 true
    cout<< "文本：\"" << s << "\"" << endl;
    cout << "关键词序列s是: " << res << endl;
    cout << "关键词序列s1是: " << res1 << endl;
    cout<< "simhash值是: " << u64<<endl;
    cout<< "simhash值是: " << u64_1<<endl;
    cout << Simhasher::isEqual(u64, u64_1) << endl;


    const char * bin1 = "100010110110";
    const char * bin2 = "110001110011";
    uint64_t u1, u2;
    u1 = Simhasher::binaryStringToUint64(bin1); 
    u2 = Simhasher::binaryStringToUint64(bin2); 
    
    cout<< bin1 << "和" << bin2 << " simhash值的相等判断如下："<<endl;
    cout<< "海明距离阈值默认设置为3，则isEqual结果为：" << (Simhasher::isEqual(u1, u2)) << endl; 
    cout<< "海明距离阈值默认设置为5，则isEqual结果为：" << (Simhasher::isEqual(u1, u2, 5)) << endl; 
    return EXIT_SUCCESS;
}
