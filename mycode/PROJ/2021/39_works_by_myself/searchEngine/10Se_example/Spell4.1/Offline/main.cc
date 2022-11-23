#include "Configuration.h"
#include "DictProducer.hh"
#include "GetIndexfile.hh"

void test1()
{

    //Class Configuration(配置文件类) 基本实现
    cout << "test1()" << endl;

    yt::Configuration p = yt::Configuration("conf");
    p.getConfigMap();
    p.printfconfigmap();
    string c1=p.getenDictPath();
    cout << c1 <<endl;
}
/* void test2() */
/* { */

/* cout << "test2()" <<endl; */
/* yt::Configuration p = yt::Configuration("conf"); */
/* yt::DictProducer d = yt::DictProducer(p.getenDictPath(),p.getcnDictPath(),p.getaimsdir()); */
/* d.build_dict(); */
/* d.store_dict(); */
/* /1* d.show_files(); *1/ */
/* d.get_enfiles(); */
/* d.push_dict("kdealfy");  //大概在52332行位置 */
/* d.store_dict(); */
/* } */
/* void test3() */
/* { */
/*     cout << DICT_PATH <<endl; */

/* } */

void test4()
{

    cout << "test4()" <<endl;
    yt::Configuration p = yt::Configuration("conf");
    yt::DictProducer d = yt::DictProducer(p.getenDictPath(),p.getcnDictPath(),p.getaimsdir());
    d.build_cn_dict();
    d.build_dict();
    d.store_dict();
    
    /* d.show_files(); */
    /* d.store_dict(); */
}
void test5()
{
    cout << "test5()" <<endl;
    yt::Configuration p = yt::Configuration("conf");
    yt::DictProducer d = yt::DictProducer(p.getenDictPath(),p.getcnDictPath(),p.getaimsdir());
    d.build_cn_dict();
    d.build_dict();
    d.store_dict();

   GetIndexFile index(p.getaimsdir(),p.getindexdir(),p.getstopdir());
   index.create_index();
   index.store_index();


}

int main()
{   
    /* test1(); */
    /* test2(); */
    /* test3(); */
    /* test4(); */
    test5();

    cout << "main执行到最后了" << endl;
    return 0;
}


