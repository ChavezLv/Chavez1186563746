#include"orders.h"
void ordAction(int ordNum);

int recv_Order(int clientFd,int *Fd){
    char buff[256];//存放命令的缓冲区
    int ret=recv();//接收命令
    //命令与参数分离
    Ord_Info odr_info=(Ord_Info)buff; 
    //int ordNum=atoi(buff);
    
    
    //判断长短命令
    if(ord_info.ordNum!=GETS&&ord_info.ordNum!=PUTS){
        //短命令处理
        ordAction(ord_info.ordNum);
    }else{
        //建立新的连接
        int listenfdD=tcp_init();//数据tcp
        //长命令处理,先建立第二个线程池,再将其加入到任务队列
        threadPool_Init();//待商榷
        ordAction(ord_info.ordNum);
    }
}

void ordAction(Ord_Info orderNum){
    switch (orderNum)
    {
    case CD:
        ord_cd(ord_info.args);//cd命令接口
        break;
    case GETS:
        ord_gets(ord_info.args);//gets命令接口
        break;
    case PUTS:
        ord_puts(ord_info.args);//puts命令接口
        break;
    default:
        break;
    }
}





