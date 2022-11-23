#include <string.h>

#include <iostream>
#include <vector>
#include <algorithm>

using std::cout;
using std::endl;

class Computer{
public:
    /* Computer(const char* brand, float price): */
    /* _brand((new char[strlen(brand) + 1]()), */
    /* _price(price){ */
        /* strcpy(_brand, brand); */
    /* } */
    inline
    void setBrand(const char *brand){
        strcpy(_brand, brand);
    }

    inline
        void setPrice(float price){
            _price = price;
        }

private:
    char _brand[20];//m_brand
    float _price;
};
int main()
{
    Computer com;
    /* com._brand; */
    return 0;
}

