#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::cerr;
using std::ifstream;
using std::string;
using std::vector;

void test()
{
    //istream cin
    //对于文件输入流而言，当文件不存在的时候，就打开失败
    //当文件存在的时候，才能进行正常读文件
    ifstream ifs("Point1.cc");
    if(!ifs.good())
    {
        cerr << "ifstream is not good " << endl;
        return;
    }

    string word;
    /* cin >> x >> y; */
    while(ifs >> word)//对于文件输入流而言，默认情况下都是以空格为分隔符
    {
        /* cout << word << " "; */
        cout << word << endl;
    }

    ifs.close();
}

void test2()
{
    ifstream ifs("Point1.cc");
    if(!ifs.good())
    {
        cerr << "ifstream is not good " << endl;
        return;
    }

    string line;
    while(getline(ifs, line))//对于文件输入流而言，默认情况下都是以空格为分隔符
    {
        cout << line << endl;
    }

    ifs.close();
}

void test3()
{
    ifstream ifs("Point1.cc");
    if(!ifs.good())
    {
        cerr << "ifstream is not good " << endl;
        return;
    }

    string line[100];
    size_t idx = 0;
    while(getline(ifs, line[idx]))//对于文件输入流而言，默认情况下都是以空格为分隔符
    {
        cout << line[idx] << endl;
        ++idx;
    }


    cout << endl << "line[34] = " << line[34] << endl;
    ifs.close();
}

void test4()
{
    ifstream ifs("Point1.cc");
    if(!ifs.good())
    {
        cerr << "ifstream is not good " << endl;
        return;
    }

    /* int a = 100; */
    string line;
    vector<string> vec;
    vec.reserve(60);
    while(getline(ifs, line))//对于文件输入流而言，默认情况下都是以空格为分隔符
    {
        vec.push_back(line);
    }

    for(size_t idx = 0; idx != vec.size(); ++idx)
    {
        cout << vec[idx] << endl;
    }

    ifs.close();
}
int main(int argc, char **argv)
{
    test4();
    return 0;
}

