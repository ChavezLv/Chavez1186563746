#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

using std::cout;
using std::endl;
using std::istringstream;
using std::ostringstream;
using std::stringstream;
using std::string;
using std::ifstream;

//字符串IO是做格式化转换的

/* string s1 = "hello"; */
string int2String(int number)
{
    ostringstream oss;
    oss << number;

    return oss.str(); 
}
void test()
{
    int value = 100;
    string s1 = int2String(value);
    cout << "s1 = " << s1 << endl;
}

void test2()
{
    int number1 = 10;
    int number2 = 200;
    stringstream ss;
    ss << "number1= " << number1 << " ,number2= " << number2 << endl;
    string s1 = ss.str();
    cout << s1;

    string key;
    int value;
    while(ss >> key >>value)
    {
        cout << key << "         " << value << endl;
    }
}

void read(const string &filename)
{
    ifstream ifs(filename);
    if(!ifs)
    {
        std::cerr << "the ifstream is not good" << endl;
        return;
    }

    string line;
    while(getline(ifs, line))
    {
        istringstream iss(line);
        string key;
        string value;
        iss >> key >> value;
        cout << key << "=====>" << value << endl;
        /* _dict.word = key; */
        /* _dict._fre = value */
    }

    ifs.close();
}

void test3()
{
    read("my.conf");
}
int main(int argc, char **argv)
{
    test3();
    return 0;
}

