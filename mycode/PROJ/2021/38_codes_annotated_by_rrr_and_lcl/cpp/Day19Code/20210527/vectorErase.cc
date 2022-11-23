#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

template <typename Container>
void display(const Container &c)
{
    for(auto &elem : c)
    {
        cout << elem << "  ";
    }
    cout << endl;
}

void test()
{
    vector<int> number;
    number.push_back(1);
    number.push_back(2);
    number.push_back(6);
    number.push_back(6);
    number.push_back(7);
    number.push_back(9);
    display(number);

    auto it = number.begin();
    for(; it != number.end();)
    {
        if(*it == 6)
        {
            it = number.erase(it);
        }
        else
        {
            ++it;
        }
    }
    display(number);
}
int main(int argc, char **argv)
{
    test();
    return 0;
}

