#include <iostream>

using std::cout;
using std::endl;

//1、使用友元
class AutoRelease;//类的前向声明//也只有声明了
//使用友元的原因：我希望autoRelease可以自由的访问singleton的内部元素而完成释放

class Singleton //单例模式//只允许整个进程内有1个堆上的对象。
{
    friend class AutoRelease; //把autoRelease类友元化
public:
    static Singleton *getInstance() //静态函数成员：不需要通过对象也可以执行的函数
        {//getInstrance()，生成的本来也就是【生成唯一实体】，所以【不需要通过对象来调用】
        if(nullptr == _pInstance)//进入检查：【不存在实例才新建】//[NOTE]多线程此处是【不安全】的，需要加锁再检查
        {
            _pInstance = new Singleton();//唯一指定新建实例地点
        }

        return _pInstance;//存在实例就返回原有实例，不存在实例就返回新实例
    }

    static void destroy()
    {
        if(_pInstance)//防double//此处在多线程为临界资源，一样要保护
        {
            delete _pInstance;//单例模式的【删除】需要自己调用+ 【清除堆空间上的内容】
            _pInstance = nullptr;//删除完毕指针一定要置空，不然会出问题
        }
    }
private:
    Singleton()
    {
        cout << "Singleton()" << endl;//构造函数
    }

    ~Singleton()
    {
        cout << "~Singleton()" << endl;//析构函数-->auto_release的目标就是这个析构函数清理时候调用auto_release的
        //函数，然后完成堆空间内的清理-->暂定
    }
private:
    static Singleton *_pInstance;//唯一私有类，指向唯一实例的指针
};

Singleton *Singleton::_pInstance = nullptr;//初始化置空：懒汉模式（等待主函数初始化创建实例）//自己不主动创建，所以兰
//对应：饿汉模式，【1来就很饿，所以一来就创建内容】

class AutoRelease
{
public:
    AutoRelease()
    {
        cout << "AutoRelease()" << endl;//构造release对象
    }

    ~AutoRelease()
    {
        cout << "~AutoRelease()" << endl;//auto_release对象的析构,顺便完成单例模式的收尾
        if(Singleton::_pInstance)
        {
            delete Singleton::_pInstance;//【作为有这个权限的友元】删除单例模式
            Singleton::_pInstance = nullptr;//删除完单例模式置空
        }
    }
};
int main(int argc, char **argv)
{
    Singleton *ps1 = Singleton::getInstance();//通过类调用唯一构造函数
    AutoRelease ar;//栈对象-->必须和getInstrance配套出现，目标是在main结束/函数结束时自动清理
    //所以【ar变量】的生存期要【同步于你所希望的singleton实例的生存期】

    /* Singleton::destroy(); */
    return 0;
}

