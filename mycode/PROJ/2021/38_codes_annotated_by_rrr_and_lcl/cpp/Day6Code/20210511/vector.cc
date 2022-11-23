#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

void printVectorCapacity(const vector<int> &vec)
{
    cout << "vec.size = " << vec.size() << endl
         << "vec.capacity = " << vec.capacity() << endl;
}

void test()
{
    //1、vs下面的扩容机制是1.5倍
    //2、不能直接在老的空间后面直接扩容，这样会可以能会操作不属于老的空间的内存
    //3、2倍扩容的机制对于push_back是可以的，但是对于vector中的insert是不行的
    //
    //vector底层的扩容机制：当size() == capacity()的时候，如果还有元素插进来的
    //时候，就会按照2 * size()进行扩容，然后将老的空间中的元素拷贝的新的空间
    //里面来，接着将新的元素插入到新的空间的尾部，最后将老的空间进行回收
    vector<int> number;
    number.reserve(20);//预留空间
    /* number[0] = 1;//error */
    printVectorCapacity(number);

    cout << endl;
    number.push_back(1);
    printVectorCapacity(number);

    cout << endl;
    number.push_back(2);
    printVectorCapacity(number);

    cout << endl;
    number.push_back(3);
    printVectorCapacity(number);

    cout << endl;
    number.push_back(4);
    printVectorCapacity(number);

    cout << endl;
    number.push_back(5);
    printVectorCapacity(number);

    cout << endl;
    number.push_back(6);
    printVectorCapacity(number);

    cout << endl;
    number.push_back(7);
    printVectorCapacity(number);

    cout << endl;
    number.push_back(8);
    printVectorCapacity(number);

    cout << endl;
    number.push_back(5);//push_back应该是一种【值拷贝】
    printVectorCapacity(number);
}

int main(int argc, char **argv)
{
    test();
    return 0;
}

