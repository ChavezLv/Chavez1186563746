#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::cerr;
using std::cin;
using std::ifstream;
using std::ofstream;
using std::fstream;
using std::string;

void test()
{
    ifstream ifs("Point1.cc", std::ios::ate);
    if(!ifs.good())
    {
        cerr << "ifstream is not good " << endl;
        return;
    }

    cout << "ifs.tellg = " << ifs.tellg() << endl;

    //对于文件输出流而言，当文件不存的时候，就创建文件
    //当文件存在的时候，将文件清空然后在去进行写入
    ofstream ofs("wuhan.txt");
    if(!ofs.good())
    {
        cerr << "ofstream is not good " << endl;
        ifs.close();
        return;

    }

    string line;
    while(getline(ifs, line))//对于文件输入流而言，默认情况下都是以空格为分隔符
    {
        ofs << line << endl;
    }

    ifs.close();
    ofs.close();
}


void test2()
{
    //对于文件输入输出流而言，当文件不存在的时候，就是失败
    //当文件存在的时候，才可以进行正常操作
    fstream fs("test.txt");
    if(!fs)
    {
        cerr << "fstream is not good " << endl;
        return;
    }

    int number = 0;
    for(size_t idx = 0; idx != 5; ++idx)
    {
        cin >> number;
        fs << number << " ";
    }

    //判断文件指针的位置tellg/tellp
    cout << "fs.tellg() = " << fs.tellg() << endl;
    
    //文件指针的偏移
    //seekp（put）/seekg（get）
    
    /* fs.seekp(0);//ok，传绝对位置 */
    fs.seekp(0, std::ios::beg);

    for(size_t idx = 0; idx != 5; ++idx)
    {
#if 0
        cout << "fs.fail = " << fs.fail() << endl;
        cout << "fs.eof = " << fs.eof() << endl;
        cout << "fs.good = " << fs.good() << endl;
#endif
        fs >> number;

        cout << number << " ";
    }
    cout << endl;

    fs.close();

}

void test3()
{
    ofstream ofs("Point2.cc", std::ios::app);
    if(!ofs.good())
    {
        cerr << "ofstream is not good " << endl;
        return;

    }

    cout << "ofs.tellp = " << ofs.tellp() << endl;

    ofs << "hello kiki" << endl;
    ofs << "hello kiki" << endl;
    ofs << "hello kiki" << endl;
    cout << "ofs.tellp = " << ofs.tellp() << endl;

    ofs.close();

}
int main(int argc, char **argv)
{
    test3();
    return 0;
}

