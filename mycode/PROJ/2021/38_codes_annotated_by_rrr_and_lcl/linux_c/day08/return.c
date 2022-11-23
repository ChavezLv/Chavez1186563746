#include <func.h>
void newFunc(){
    printf("newFunc\n");
}
void func(){
    printf("func");
    //exit函数会清空缓冲区
    _exit(3);
    //_exit _Exit不会清空缓冲区
}
int main()
{
    atexit(newFunc);//注册一个终止处理函数
    //当exit执行的时候，会先执行终止处理函数
    func();
    return 1;
}

