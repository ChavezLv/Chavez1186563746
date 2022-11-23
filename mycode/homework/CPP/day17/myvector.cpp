#include<iostream>
#include<memory>
#include<algorithm>
using std::cout;
using std::endl;
template<typename T> 
class Vector 
{ 
public:     
    Vector(){_start=_finish=_end_of_storage=nullptr;}     
    ~Vector(){
        if(_start){
            while(_finish!=_start){
                _alloc.destroy(--_finish);
            }
            cout<<"core dump"<<endl;
//            _alloc.deallocate(_start,capacity());

        }
    }          
    void push_back(const T &);      
    void pop_back();         
    int size()const;     
    int capacity()const; 
    typedef T* iterator;
    iterator begin(){
        return _start;
    }
    iterator end(){
        return _finish;
    }
    void print(){
        /* for(auto it=_start;_start!=_finish;++it){ */
        /*     std::cout<<*it<<" "<<std::endl; */
        /* } */
        auto i=_start;
        for(auto it=0;it<7;++it){
            std::cout<<*i++<<" "<<std::endl;
        }
    }
private:     
    void reallocate();//重新分配内存,动态扩容要用的 
private:         
    static std::allocator<T> _alloc;          
    T *_start;                 //指向数组中的第一个元素     
    T *_finish;                //指向最后一个实际元素之后的那个元素     
    T *_end_of_storage;        //指向数组本身之后的位置 
};
//静态成员类外初始化;
template<typename T> 
std::allocator<T> Vector<T>::_alloc;
template<typename T> 
void Vector<T>::reallocate(){
#if 1
    int size=Vector<T>::size();
    int newCapacity=2*std::max(capacity(),1);
    std::cout<<"size="<<size<<", capacity="<<newCapacity<<std::endl;
    T* newStart=_alloc.allocate(newCapacity);
    auto tmp=newStart;
    for(T* ptr=_start;ptr!=_finish;){
        *tmp=*ptr;
        ++tmp;
        Vector<T>::_alloc.destroy(ptr++);//销毁对象
    }
    if(_start){
        _alloc.deallocate(_start,capacity());//销毁空间
    }
    _start=newStart;
    _finish=newStart+size;
    _end_of_storage=newStart+newCapacity;
    std::cout<<" 1:"<<_start<<" 2:"<<_finish<<" 3:"<<_end_of_storage<<std::endl;
#endif
#if 0
    int oldCapacity = capacity();
    int newCapacity = 2 * oldCapacity > 0 ? 2 * oldCapacity : 1;

    T *tmp = _alloc.allocate(newCapacity);//申请新的空间
    if(_start)
    {
        std::uninitialized_copy(_start, _finish, tmp);//老的空间上的元素拷贝到新的空间
        while(_finish != _start)
        {
            _alloc.destroy(--_finish);//销毁老的空间上的对象
        }
        _alloc.deallocate(_start, oldCapacity);//回收老的空间
    }

    //将三个指针进行重新置位
    _start = tmp;
    _finish = tmp + oldCapacity;
    _end_of_storage = tmp + newCapacity;
#endif
}
template<typename T> 
void Vector<T>::push_back(const T & elem){
    if(_finish==_end_of_storage){
        Vector<T>::reallocate();
    }
    _alloc.construct(_finish++,elem);
}
template<typename T> 
void Vector<T>::pop_back(){
    if(_start!=_finish){
        _alloc.destroy(_finish);
        --_finish;
    }
    if(_finish){
        _alloc.deallocate(_finish);
    }
}
template<typename T> 
int Vector<T>::size()const{
    return _finish-_start;
}
template<typename T> 
int Vector<T>::capacity()const{
    return _end_of_storage-_start;
}

int main(){
    Vector<int>vec;
    vec.push_back(2);
    vec.push_back(7);
    vec.push_back(3);
    vec.push_back(6);
    vec.push_back(2);
    vec.push_back(5);
    vec.push_back(3);
    vec.print();
    std::cout<<"size="<<vec.size()<<", capacity="<<vec.capacity()<<std::endl;
    return 0;
}
