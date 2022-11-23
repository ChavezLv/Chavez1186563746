#include"../include/orders.h"
#include"../include/trans.h"
int recv_Order(int clientFd, int *Fd, pPool_t pPoolOrder){
    pWork_t pNewWork = (pWork_t)calloc(1, sizeof(Work_t));
    int ret = recv(clientFd, pNewWork->workArgs, sizeof(Order_t), MSG_WAITALL);//接收并判断命令
    if(ret == 0){//连接断开检查
        printf("$system>>clientFd %d, userID %d connect error!\n", clientFd, Fd[clientFd]);
        close(clientFd);
        Fd[clientFd] = -1;
        return -1;
    }
    
    pNewWork->clientFd = clientFd;
    pNewWork->Fd = Fd;
    pNewWork->pPool = pPoolOrder;
    
    if(pPoolOrder->Queue->workNum >= pPoolOrder->threadNum){
        pOrder_t pOrder = (pOrder_t)pNewWork->workArgs;
        if(pOrder->orderNum != PUTS && pOrder->orderNum != GETS){
            read_Order(pNewWork);
            free(pNewWork);
            free(pOrder);
            return 0;
        }
        free(pOrder);
    }

    pthread_mutex_lock(&pPoolOrder->mutex);
    work_Add(pPoolOrder->Queue, pNewWork);
    pthread_mutex_unlock(&pPoolOrder->mutex);
    pthread_cond_signal(&pPoolOrder->cond);
    return 0;
}
int read_Order(pWork_t pWork)
{
    pOrder_t pOrder = (pOrder_t)pWork->workArgs;
    reOrder_t reOrder;//用于存储回应客户端的信息的结构体
    memset(&reOrder, 0, sizeof(reOrder));
printf("$system>>clientFd %d have order %d\n", pWork->clientFd, pOrder->orderNum);//此处可调用日志接口
printf("$system>>orderTarget:%s\n", pOrder->orderTarget);
    
    switch (pOrder->orderNum){
    case CD:
        order_cd(&reOrder, pWork);
        break;
    case LS:
        order_ls(&reOrder, pWork);
        break;
    case PWD:
        order_pwd(&reOrder,  pWork);
        break;
    case PUTS:
        order_puts(&reOrder, pWork);
        break;
    case GETS:
        order_gets(&reOrder, pWork);
        break;
    case RM:
        order_rm(&reOrder, pWork);
        break;
    case MKDIR:
        order_mkdir(&reOrder, pWork);
        break;
    case RMDIR:
        order_rmdir(&reOrder, pWork);
        break;
    case TREE:
        break;
    case ERROR:
        reOrder.retNum = -1;
        sprintf(reOrder.retInfo, "ERROR cmd");
        break;
    case LOAD_USERNAME:
        pWork->Fd[pWork->clientFd] = userName(&reOrder, pWork);
        break;
    case LOAD_CRYPT:
        userCrypt(&reOrder, pWork);
        break;
    case REGISTER_USERNAME:
        pWork->Fd[pWork->clientFd] = handleRegisterRecvUserName(&reOrder, pWork);        
        break;
    case REGISTER_CRYPT:
        handleRegisterRecvCrypt(&reOrder, pWork);
        break;
    case EXIT:{
        printf("$system>>clientFd = %d is exit\n", pWork->clientFd);
        char op[1024] = {0};
        sprintf(op, "user exit");
        logIntoMysql(pWork->Fd[pWork->clientFd], op);
        close(pWork->clientFd);
        pWork->Fd[pWork->clientFd] = -1;
        }
        return 0;
    case NEWFD:
        newFd_isWhose(pWork);
        return 0;
    case SENDFILE:
        sendfile(pWork);
        return 0;
    case RECVFILE:
        recvfile(pWork);
        return 0;
    default:
        printf("$system>>problem\n");
        return -1;
    }
    send(pWork->clientFd, &reOrder, sizeof(reOrder), 0);
    return 0;
}
