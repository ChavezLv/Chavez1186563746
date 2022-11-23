#include <string.h>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <deque>
using std::deque;

#include <list>
using std::list;

#include <map>
using std::map;

#include <memory>
/* using std::allocator; */

template<typename T>
class Vector
{
public:
    typedef T value_type;
    typedef value_type* pointer;
    typedef value_type* iterator;
    typedef size_t size_type;
public:
    Vector()
    : _start()
    , _finish()
    , _end_of_storage()
    {
        //初始化列表赋初值为0

    }
    ~Vector(){
        _alloc.deallocate(_start, capacity());
    }

    void 
    push_back(const value_type &x){
        if(this->_finish != this->_end_of_storage){
            _alloc.construct(_finish, x);
            ++ this->_finish;
        }else{
            //需要扩容
            insert_aux(end(), x);
        }
    }

    void
    pop_back(){
        memset(_finish, 0 , sizeof(value_type));
        -- this->_finish;    

    }

    size_type size(){
        return this->_finish - this->_start;
    }

    size_type capacity(){
        return this->_end_of_storage - this->_start;
    }

    iterator begin(){
        return _start;
    }

    iterator end(){
        return _finish;
    }
    bool empty(){
        return begin() == end();
    }
    value_type& operator[](size_type n){
        return *(begin() + n);
    }
    value_type& front(){
        return *begin();
    }
    value_type& back(){
        return *(end() - 1);
    }
private:
    void insert_aux(iterator , const value_type&);


    void deallocate(){
        if(this->_start){
            //...
        }
    }


    void
    reallocate(iterator pos, const value_type& x);//重新分配内存,动态扩容要用的

private:    
    static std::allocator<value_type> _alloc;

    pointer _start;                 //指向数组中的第一个元素
    pointer _finish;                //指向最后一个实际元素之后的那个元素
    pointer _end_of_storage;        //指向数组本身之后的位置

};

template<typename T>
std::allocator<T> Vector<T>::_alloc;

template<typename T>
void Vector<T>::reallocate(iterator pos, const value_type& x){
    const size_type old_size = size();
    const size_type len = old_size != 0 ? 2 * old_size : 1;//原为0->1; 原不为0->两倍
    iterator new_start = _alloc.allocate(len);
    iterator new_finish = new_start;
    try{
        //拷贝原空间插入点之前的内容
        /* cout << "old value:" << endl; */
        /* for(int i = 0; i < pos - _start; ++i){ */
        /*     cout << _start[i] << " "; */
        /* } */
        /* cout << endl; */

        /* cout << "new value:" << endl; */
        memmove(new_start, _start, (pos - _start) * sizeof(value_type));
        /* for(int i = 0; i < pos - _start; ++i){ */
        /*     cout << "pos - start:  " << pos - _start << endl; */
        /*     cout << new_start[i] << " "; */
        /* } */
        /* cout << endl; */

        new_finish += pos - _start;

        _alloc.construct(new_finish, x);
        ++ new_finish;
        /* cout << "new_finish: " << *new_finish << endl; */

        //拷贝原空间插入点之后的内容
        memmove(new_finish, pos, (_finish - pos) * sizeof(value_type));
        /* cout << "_finish - pos: " << _finish - pos << endl; */

        new_finish += _finish - pos;
    }
    catch(...){
        /* destroy(new_start, new_finish); */
        _alloc.deallocate(new_start, len );
        throw;
    }

    //释放旧空间
    _alloc.deallocate(_start, capacity());

    _start = new_start;
    _finish = new_finish;
    _end_of_storage = new_start + len;

}

template<typename T>
void Vector<T>::insert_aux(iterator pos, const value_type& x){
    if(_finish != _end_of_storage){//还有空间
        //insert 有可能用到
    }else{//无空间
        reallocate(pos, x);
    }
}

void test01(){
    Vector<int> vec;
    cout << "initial capacity: " << vec.capacity() << endl;;
    cout << "initial size: " << vec.size() << endl;;

    vec.push_back(100);
    /* cout << vec[0] << endl; */
    cout << "size: " << vec.size() << endl;;
    cout << "capacity: " << vec.capacity() << endl;;

    vec.push_back(200);
    cout << "size: " << vec.size() << endl;;
    cout << "capacity: " << vec.capacity() << endl;;
    /* cout << vec[0] << endl; */
    /* cout << vec[1] << endl; */

    cout << "size: " << vec.size() << endl;;
    cout << "capacity: " << vec.capacity() << endl;;
    vec.push_back(300);
    cout << "size: " << vec.size() << endl;;
    cout << "capacity: " << vec.capacity() << endl;;
    /* cout << vec[0] << endl; */
    /* cout << vec[1] << endl; */
    /* cout << vec[2] << endl; */

    for(int i = 0; i < vec.size(); ++i){
        cout << vec[i] << " ";
    }
    cout << endl;

    vec.pop_back(); 
    for(int i = 0; i < vec.size(); ++i){
        cout << vec[i] << " ";
    }
    cout << endl;

}

void test02(){
    Vector<int> vec;
    vec.push_back(100);
    vec.push_back(200);
    vec.push_back(300);
    
}
int main(int argc, char* argv[])
{

    test01();
    return 0;
}

