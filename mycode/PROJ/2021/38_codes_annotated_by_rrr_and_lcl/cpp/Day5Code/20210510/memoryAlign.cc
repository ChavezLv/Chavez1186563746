#include <iostream>
#pragma pack(4)

using std::cout;
using std::endl;

struct x
{
    char a;
    int b;
    short c;
    char d;
}MyStructX;//12

struct y
{
    int b;
    char a;
    char d;
    short c;
}MyStructY;//8

struct SS
{
    int a;
    char b;
    short c;
    int d;
    struct FF
    {
        int a1;
        char b1;
        short c1;
        char d1;
    }MyStructFF;

#if 1
    /* char e;//28 */
    int e;
    double ww;//40
#endif
}MyStructSS;


struct DD
{
    int a;
    char b;
    short c;
    int d;

    struct FF
    {
        double a1;
        char b1;
        short c1;
        char d1;
    }MyStructFF;

    char e;//40
    
}MyStructDD;

struct GG
{
    char e[2];
    short h;

    struct A
    {
        int a;
        double b;
        float c;
    }MyStructA;
}MyStructGG;//32



int main(int argc, char **argv)
{
    cout <<"sizeof(MyStructX) = " << sizeof(MyStructX) << endl;
    cout <<"sizeof(MyStructY) = " << sizeof(MyStructY) << endl;
    cout <<"sizeof(MyStructSS) = " << sizeof(MyStructSS) << endl;
    cout <<"sizeof(MyStructDD) = " << sizeof(MyStructDD) << endl;
    cout <<"sizeof(MyStructGG) = " << sizeof(MyStructGG) << endl;
    
    return 0;
}

