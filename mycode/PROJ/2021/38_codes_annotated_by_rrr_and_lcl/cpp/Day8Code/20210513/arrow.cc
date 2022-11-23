#include <iostream>

using std::cout;
using std::endl;

class Data
{
public:
    Data(int data = 10)
    :_data(data)
    {
        cout << "Data(int = 10)" << endl;
    }

    int getData() const
    {
        return _data;
    }

    ~Data()
    {
        cout << "~Data()" << endl;
    }
private:
    int _data;
};

class SecondLayer
{
public:
    SecondLayer(Data *pdata)
    : _pdata(pdata)
    {
        cout << "SecondLayer(Data *)" << endl;
    }

    Data *operator->()
    {
        return _pdata;
    }

    Data &operator*()
    {
        return *_pdata;
    }


    ~SecondLayer()
    {
        cout << "~SecondLayer()" << endl;
        if(_pdata)
        {
            delete _pdata;
            _pdata = nullptr;
        }
    }
private:
    Data *_pdata;
};

class ThirdLayer
{
public:
    ThirdLayer(SecondLayer *sl)
    : _sl(sl)
    {
        cout << "ThirdLayer(sl *)" << endl;
    }

    SecondLayer &operator->()
    {
        return *_sl;//解引用, 再引用
    }

    ~ThirdLayer()
    {
        cout << "~ThirdLayer()" << endl;
        if(_sl)
        {
            delete _sl;
            _sl= nullptr;
        }
    }
private:
    SecondLayer *_sl;//类的指针作为数据成员
};
int main(int argc, char **argv)
{
    //int a = 10;
    SecondLayer sl(new Data());//栈对象 , new的时候,没有用delete也被干掉了的原因?
    //因为在second里面有delete
    /* sl.operator->()->getData();//原始版本*/
    //sl.operator->()返回的是data的指针, 接着去 data-> getdata() 
    cout << "sl->getData = " << sl->getData() << endl;//第二层的对象去调用getdata函数
    /* sl.operator*().getData();//原始版本 */
    cout << "(*sl).getData() = " << (*sl).getData() << endl;

    cout << endl;
    ThirdLayer tl(new SecondLayer(new Data(20)));//栈对象
    //tl.operator->().operator->()->getData();
    cout << "tl->getData() = " << tl->getData() << endl;
    return 0;
}

