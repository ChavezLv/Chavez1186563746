#include"../include/orders.h"
int recv_Order(int clientFd, int *Fd, pPool_t pPoolOrder){
    pWork_t pNewOrder = (pWork_t)calloc(1, sizeof(Work_t));
    int ret = recv(clientFd, pNewOrder->workArgs, sizeof(Order_t), MSG_WAITALL);//接收并判断命令
    if(ret == 0){//连接断开检查
printf("$system>>clientFd %d, userID %d connect error!\n", clientFd, Fd[clientFd]);
        close(clientFd);
        Fd[clientFd] = -1;
        return -1;
    }
    pNewOrder->clientFd = clientFd;
    pNewOrder->Fd = Fd;

    pthread_mutex_lock(&pPoolOrder->mutex);
    work_Add(pPoolOrder->Queue, pNewOrder);
    pthread_mutex_unlock(&pPoolOrder->mutex);
    pthread_cond_signal(&pPoolOrder->cond);
    return 0;
}
int read_Order(void* order, int clientFd, int *Fd)
{
    pOrder_t pOrder = (pOrder_t)order;
    reOrder_t reOrder;//用于存储回应客户端的信息的结构体
    memset(&reOrder, 0, sizeof(reOrder));
    //未登录用户只能用LOAD，REGISTER，EXIT三个选项
printf("$system>>clientFd %d have order %d\n", clientFd, pOrder->orderNum);//此处可调用日志接口
printf("$system>>orderTarget:%s\n", pOrder->orderTarget);
    if(Fd[clientFd] == -1 && pOrder->orderNum < 11){
printf("$system>>attact or send problem\n");
        return -1;
    }
    //按命令编号调用对应接口，接口需将回复加入reOrder
    switch (pOrder->orderNum){
    case CD:
        order_cd(&reOrder, pOrder->orderTarget, Fd[clientFd]);
        break;
    case LS:
        order_ls(&reOrder, pOrder->orderTarget, Fd[clientFd]);
        break;
    case PWD:
        order_pwd(&reOrder, Fd[clientFd]);
        break;
    case PUTS:
        //order_puts(&reOrder, pOrder->orderTarget, Fd[clientFd]);
        order_puts(&reOrder, clientFd, Fd[clientFd]);
        break;
    case GETS:
        order_gets(&reOrder, pOrder->orderTarget, Fd[clientFd]);
        break;
    case RM:
        order_rm(&reOrder, pOrder->orderTarget, Fd[clientFd]);
        break;
    case MKDIR:
        order_mkdir(&reOrder, pOrder->orderTarget, Fd[clientFd]);
        break;
    case RMDIR:
        order_rmdir(&reOrder, pOrder->orderTarget, Fd[clientFd]);
        break;
    case TREE:
        order_tree(&reOrder, pOrder->orderTarget, Fd[clientFd]);
        break;
    case ERROR:
        reOrder.retNum = -1;
        sprintf(reOrder.retInfo, "ERRO%s\n", pOrder->orderTarget);
        break;
    case LOAD_USERNAME:
        Fd[clientFd] = userName(&reOrder, pOrder->orderTarget);
        break;
    case LOAD_CRYPT:
        userCrypt(&reOrder, pOrder->orderTarget, Fd[clientFd]);
        break;
    case REGISTER_USERNAME:
        Fd[clientFd] = handleRegisterRecvUserName(&reOrder, pOrder->orderTarget);        
        break;
    case REGISTER_CRYPT:
        handleRegisterRecvCrypt(&reOrder, pOrder->orderTarget, Fd[clientFd]);
        break;
    case EXIT:{
printf("$system>>clientFd = %d is exit\n", clientFd);
        char op[1024] = {0};
        sprintf(op, "user exit");
        logIntoMysql(Fd[clientFd], op);
        close(clientFd);
        Fd[clientFd] = -1;
              }
        return 0;
    default:
printf("$system>>attact or trans problem\n");
        return -1;
    }
    send(clientFd, &reOrder, sizeof(reOrder), 0);
    return 0;
}
