#include <string.h>
#include <iostream>

using std::cout;
using std::endl;

class String //string类
{
public://公共成员：几个构造函数+函数重载 //毕竟是写时复制
    //String s1;
    String()//[1]无参数默认构造函数 //-->构造空字符串+引用计数
    : _pstr(new char[5]() + 4)  
    //1- new char[5]构造了一个[4B(int)+ 1B '\0']//即【空字符串】 --->并返回【首地址的指针】
    //   new type()的形式自带【初始化】的效果，就是说'\0'是自动填好的
    //2- new 运算符【返回一个指针】 ，然后  +4 move到了【“字符串开始的地方”】
    //-->即是说：永恒让_pstr指向【字符串】的部分
    {
        cout << "String()" << endl;//即【无参构造函数】
       *(int *)(_pstr - 4) = 1;// 1-平移指针_pstr回首部的int  2-强转为int指针 3-按照int类型解引用 
       //-->得到ref_count引用计数，并赋值为1 //空字符串也有为1的权利 //-->所以我们的引用计数【并不检索内容】
       //引用计数的加减消失更多和【修改时机】有关系，并不真的严格比对内容
    }

    String(const char *pstr)//[2]拷贝构造函数：基于C字符串
    : _pstr(new char[strlen(pstr) + 5]() + 4)//长度： c字符串长度+'\0'+4位int(strlen+4) //然后再偏移4
    {
        //为什么【用C字符串】和【无参数】都需要new?--->因为【这两个场合之内都不存在string对象】
        cout << "String(const char *)" << endl;//即【基于C字符串构造函数】
        strcpy(_pstr, pstr);//拷贝
        *(int *)(_pstr - 4) = 1;//所有构造函数都有【使引用计数为1】的任务
    }

    //String s2 = s1
    String(const String &rhs) //[3]拷贝构造函数：基于常引用
    : _pstr(rhs._pstr)//[r]??这是在干什么---> [a]浅拷贝，常引用代表【已经有这个string对象】，那【字符的部分用它的空间】就行
    {
        cout << "String(const String &)" << endl;//即【常引用构造函数】
        ++*(int *)(_pstr - 4);//引用计数++ //-->【由常引用来提供copy源】，意味着【string必已经存在】
    }

    //String s3("world");
    //s3 = s1;
    String &operator=(const String &rhs) //[4]重载【赋值运算符】
    //思考赋值运算符可能带来的改变：
    //1-被赋值的量（左操作数）约等于湮灭，引用计数--，且存在内存消亡的可能
    //2-右边的赋值量(右操作数)，已经存在，而且还把自己copy了一个人，引用计数++
    {
        cout << "String &operator=(const String &)" << endl;

        if(this != &rhs)//1、自复制--->如果自己给自己赋值，请什么也不做直接返回
        {
            --*(int *)(_pstr - 4);//2、释放左操作数 //左值引用计数--
            if(0 == *(int *)(_pstr - 4))
            {
                delete [] (_pstr - 4);//如果左值引用计数抵达0，就直接被delete
            }

            _pstr = rhs._pstr;//3、浅拷贝---->就是【只取人家地址】
            ++*(int *)(_pstr - 4);//[R**]任何一个_pstr代表一块【空间】，而【往前移4个一定是本块空间的引用计数】
            //[R]其实【如果多线程这个就是被竞争资源了】
       }

        return *this;//4、返回*this 
        //[r]返回this【解了引用】，其实就是返回【对象本身】，配合【返回是左引用】，就是【返回了可以修改的】对象半身
    }

    //s3[0] = 'H';
    //[5]重载[]符号: op[] (size_t idx)
    //思考[]取内容运算符可能带来的改变：
    /*
        %1-写的时候
            1-若【本块内存引用者只有1人】：
                那你就【返回可修改的本块内存】就行，也不用写时复制，也不需要有新内存产生
            2-若【本内存的引用者不只一人】：//copy_on_write发生
                --->1-原内存引用计数-1
                --->2-copy出【新内存】，并且【返回新内存用于修改】

            3-//[R]其实在这里就看得出来，这个办法的问题在于【万一它只是看看呢，不改呢】
            //-->读也会造成【写时复制】-->所以严格来说【写时复制】就不能放在这，应该是放在赋值运算符那    
        %2-读的时候
            同上3
    */
    char &operator[](size_t idx)  //[5]重载[]符号: op[] (size_t idx)
    {
        if(idx < size())
        {
            if(getRefCount() > 1)//是共享
            {//若没有其他用户共同引用本块内存，那怎么改也就是一个，不用改
                char *ptmp = new char[strlen(_pstr) + 5]() + 4;
                strcpy(ptmp, _pstr);
                --*(int *)(_pstr - 4);//源内存必有1个以上用户，减不完，不需要在此处考虑delete
                _pstr = ptmp;
                *(int *)(_pstr - 4) = 1;
            }
            return _pstr[idx];
        }
        else
        {
            static char nullchar = '\0';//如越界直接返回0，这里无其他含义，只是做个标记方便被察觉
            //static的原因是【返回引用需要一个长生命周期的变量】-->哪怕是回去检测出不合法也得【有机会检测】
            return nullchar;
        }
    }

    ~String()//[6]重载【析构函数】
    {
        //本函数发生在所有【delete string in 堆】，还有【string消亡in 函数栈】的场合
        //实际上关键是【delete掉<在堆上的char数组空间>+<引用计数空间>】
        cout << "~String()" << endl;
        --*(int *)(_pstr - 4);//引用计数先--
        if(0 == *(int *)(_pstr - 4))//引用计数为0就释放量
        {
            delete [] (_pstr - 4);//移动回去才delete，就是为了把头4个字节带上(而且最初new的时候，在这个数组头部有标记的)
            //可以帮助找到整个数组+delete
        }
    }

    const char *c_str() const//[7]重载c_str: 返回内部的c_str字符串
    {
        return _pstr;
    }

    int getRefCount() const//[8]新函数get_ref_count()：返回引用计数的值
    {
        return *(int *)(_pstr - 4);
    }

    size_t size() const//[9]新函数size: 得到c字符串的长度
    {
        return strlen(_pstr);
    }

    friend std::ostream &operator<<(std::ostream &os, const String &rhs);
    //[10]友元【<<】符号函数：
    //为了重载<<的时候能够顺滑的取得新的string类的值//原本的string类好像就有这个效果

private:
    char *_pstr;//私有变量：*_pstr
                //其实这里面暗含俩量：
                //1-c_str字符串(从[4]，第五个位置开始,一般来说是_pstr指向的位置)
                //2-refer_count([0,3],一般来说的_pstr往前移动4位)
};

std::ostream &operator<<(std::ostream &os, const String &rhs)
//[7]重载<<：使得可以【顺滑的打印出自定义str内部的值】
{
    if(rhs._pstr)
    {
        os << rhs._pstr;
    }
    return os;
}
//总结：以上全部完成可以漂亮完成的写时复制的string，除了一个问题，【[]】这个符号莫名其妙的【写时复制，读时也复制】，请通过重载【=】
//解决这个问题
void test()
{
    String s1("hello");//c字符串构造函数
    cout << "s1 = " << s1 << endl;
    printf("s1's address: %p\n", s1.c_str());
    printf("s1's RefCount = %d\n", s1.getRefCount());

    cout << endl;
    String s2 = s1;//拷贝构造函数-->常引用构造函数
    cout << "s1 = " << s1 << endl;
    cout << "s2 = " << s2 << endl;
    printf("s1's address: %p\n", s1.c_str());
    printf("s2's address: %p\n", s2.c_str());
    printf("s1's RefCount = %d\n", s1.getRefCount());
    printf("s2's RefCount = %d\n", s2.getRefCount());

    cout << endl;
    String s3("world");//c字符串构造函数
    cout << "s3 = " << s3 << endl;
    printf("s3's address: %p\n", s3.c_str());
    printf("s3's RefCount = %d\n", s3.getRefCount());

    cout << endl << "执行s3 = s1操作" << endl;
    s3 = s1;//赋值函数//--->赋值和拷贝构造的区别在于【是否在初始化这一步/前方是否有类型】
    cout << "s1 = " << s1 << endl;
    cout << "s2 = " << s2 << endl;
    cout << "s3 = " << s3 << endl;
    printf("s1's address: %p\n", s1.c_str());
    printf("s2's address: %p\n", s2.c_str());
    printf("s3's address: %p\n", s3.c_str());
    printf("s1's RefCount = %d\n", s1.getRefCount());
    printf("s2's RefCount = %d\n", s2.getRefCount());
    printf("s3's RefCount = %d\n", s3.getRefCount());

    cout << endl << "对s3[0] = \'H\'" << endl;
    s3[0] = 'H';//[R]最难点：[]符号的重载，如何【读时别复制】、【写时复制】？
    cout << "s1 = " << s1 << endl;
    cout << "s2 = " << s2 << endl;
    cout << "s3 = " << s3 << endl;
    printf("s1's address: %p\n", s1.c_str());
    printf("s2's address: %p\n", s2.c_str());
    printf("s3's address: %p\n", s3.c_str());
    printf("s1's RefCount = %d\n", s1.getRefCount());
    printf("s2's RefCount = %d\n", s2.getRefCount());
    printf("s3's RefCount = %d\n", s3.getRefCount());
}
int main(int argc, char **argv)
{
    test();
    return 0;
}
