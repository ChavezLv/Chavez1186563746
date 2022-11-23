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

    vector<int> number = {1, 3, 5, 9, 8, 7, 5, 3, 2, 4};
    display(number);
    cout << "number.size() = " << number.size() << endl;
    cout << "number.capacity() = " << number.capacity() <<endl;

    cout << endl << "在vector的尾部进行插入与删除" << endl;
    number.push_back(12);
    number.push_back(6);
    display(number);
    cout << "number.size() = " << number.size() << endl;
    cout << "number.capacity() = " << number.capacity() <<endl;
    display(number);

    //capacity() = n, size() = t, 插入元素的个数m
    //1、当m < n - t,此时不需要扩容
    //2、n - t < m < t,就会按照2 * t进行扩容
    //3、n - t < m < n, m > t, 就会按照t + m进行扩容
    //4、m > n,就会按照t + m进行扩容
    cout << endl << "在vector中间去进行插入" << endl;
    cout << "number.size() = " << number.size() << endl;
    cout << "number.capacity() = " << number.capacity() <<endl;
    auto it = number.begin();
    ++it;
    ++it;
    cout << "*it = " << *it << endl;
    number.insert(it, 100);
    display(number);
    cout << "*it = " << *it << endl;
    cout << "number.size() = " << number.size() << endl;
    cout << "number.capacity() = " << number.capacity() <<endl;

    cout << endl;
    cout << "*it = " << *it << endl;
    number.insert(it, 100, 200);
    display(number);
    cout << "*it = " << *it << endl;
    cout << "number.size() = " << number.size() << endl;
    cout << "number.capacity() = " << number.capacity() <<endl;

    //每次进行插入的时候，将迭代器重新置位（防止迭代器失效的问题）
    cout << endl;
    int arr[5] = {1, 6, 9, 8, 7};
    it = number.begin();
    ++it;
    ++it;
    cout << "*it = " << *it << endl;
    number.insert(it, arr, arr + 5);
    display(number);
    cout << "*it = " << *it << endl;
    cout << "number.size() = " << number.size() << endl;
    cout << "number.capacity() = " << number.capacity() <<endl;
}

int main(int argc, char **argv)
{
    test();

    return 0;
}

