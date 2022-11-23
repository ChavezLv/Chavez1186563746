#include <iostream>

using std::cout;
using std::endl;

void test()
{
    int iNumber = 10;
    float fNumber = 12.345;
    /* iNumber = (int)fNumber; */
    iNumber = int(fNumber);
}

void test2()
{
    int iNumber = 10;
    float fNumber = 12.345;
    iNumber = static_cast<int>(fNumber);
    cout << "inumber = " << iNumber << endl;

    void *pret = malloc(sizeof(int));
    int *pInt = static_cast<int *>(pret);


    free(pInt);
    pInt = nullptr;
}

void test3()
{
    const int number = 100;
    /* int *p1 = &number;//error */
    int *p1 = const_cast<int *>(&number);
    cout << "*p1 = " << *p1 << endl;
    printf("p1's address : %p\n", p1);
    printf("number's address : %p\n", &number);

    cout << endl;
    *p1 = 200;//未定义行为
    cout << "*p1 = " << *p1 << endl;
    cout << "number = " << number << endl;
    printf("p1's address : %p\n", p1);
    printf("number's address : %p\n", &number);

}
int main(int argc, char **argv)
{
    test3();
    return 0;
}

