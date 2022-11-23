#include"../include/work_Queue.h"
#include"../include/orders.h"

int send_Order(int sfd)
{
    Order_t order;
    memset(&order, 0, sizeof(order));
    int ret;
    char ch;
    char buf_orderNum[24] = {0};
    char buf_orderTarget[1024] = {0};
    int i = 0;
    
    while((ch = getchar()) != ' ' && ch != '\n'){
        buf_orderNum[i] = ch;
        ++i;
    }
    if(ch == ' '){
        i = 0;
        while((ch = getchar()) == ' ');
        buf_orderTarget[i] = ch;
        ++i;
        if(ch != '\n'){
            while((ch = getchar()) != ' ' && ch != '\n' && i < 1024){
                buf_orderTarget[i] = ch;
                ++i;
            }
        }
    }

    ret = changeOrder(buf_orderNum, &order.orderNum);
    if(ret == -1){
        printf("error cmd\n");
        return -1;
    }
    strcpy(order.orderTarget, buf_orderTarget);
    if(order.orderNum == PUTS){
        putsFile(sfd, &order);
        return -1;
    }
    send(sfd, &order, sizeof(order), 0);

    if(order.orderNum == EXIT){
        printf("over\n");
        exit(0);
    }
    if(ch != '\n'){
        while((ch = getchar()) != '\n');
    }
    return 0;
}

int read_Order(int sfd, pPool_t pPool)
{
    reOrder_t reOrder;
    memset(&reOrder, 0, sizeof(reOrder));
    int ret = recv(sfd, &reOrder, sizeof(reOrder), MSG_WAITALL);
    if(ret == 0){
        printf("$system>> connect error\n");
        close(sfd);
        exit(1);
    }

    if(reOrder.reOrderNum == SENDFILE || reOrder.reOrderNum == RECVFILE){
        pWork_t pNewTransWork = (pWork_t )calloc(1, sizeof(Work_t));
        if(reOrder.reOrderNum == SENDFILE){
            pNewTransWork->type = SENDFILE;
        }
        else if(reOrder.reOrderNum == RECVFILE){
            pNewTransWork->type = RECVFILE;
        }

        memcpy(pNewTransWork->workArgs, reOrder.reInfo, sizeof(reOrder.reInfo));
        
        pthread_mutex_lock(&pPool->mutex);
        work_Add(pPool->Queue, pNewTransWork);
        pthread_mutex_unlock(&pPool->mutex);
        pthread_cond_signal(&pPool->cond);
        return reOrder.reOrderNum;
    }
    printf("%s\n", reOrder.reInfo);
    return reOrder.reOrderNum;
}
int changeOrder(char *buf_orderNum, int *orderNum)
{
    if(strcmp(buf_orderNum, "cd") == 0){
        *orderNum = CD;
    }
    else if(strcmp(buf_orderNum, "ls") == 0){
        *orderNum = LS;
    }
    else if(strcmp(buf_orderNum, "pwd") == 0){
        *orderNum = PWD;
    }
    else if(strcmp(buf_orderNum, "puts") == 0){
        *orderNum = PUTS;
    }
    else if(strcmp(buf_orderNum, "gets") == 0){
        *orderNum = GETS;
    }
    else if(strcmp(buf_orderNum, "rm") == 0){
        *orderNum = RM;
    }
    else if(strcmp(buf_orderNum, "mkdir") == 0){
        *orderNum = MKDIR;
    }
    else if(strcmp(buf_orderNum, "rmdir") == 0){
        *orderNum = RMDIR;
    }
    else if(strcmp(buf_orderNum, "tree") == 0){
        *orderNum = TREE;
    }
    else if(strcmp(buf_orderNum, "exit") == 0){
        *orderNum = EXIT;
    }
    else{
        *orderNum = ERROR;
        return -1;
    }
    return 0;
}
char *genRandomString(void)
{
    int flag, i;
    srand((unsigned)time(NULL));
    char *string = (char*)malloc(RANDOM_STR_LEN);
    char *salt = (char*)malloc(SALT_LEN);
    bzero(salt, SALT_LEN);
    
    for(i = 0; i < RANDOM_STR_LEN+1; ++i){
        flag = rand() % 3;
        switch(flag){
        case 0:
            string[i] = 'A' + rand() % 26;
            break;
        case 1:
            string[i] = 'a' + rand() % 26;
            break;
        case 2:
            string[i] = '0' + rand() % 10;
            break;
        default:
            string[i] = 'x';
            break;       
        }
    }
    string[RANDOM_STR_LEN] = '\0';
    sprintf(salt, "$6$%s", string);
    return salt;
}

//将密码和盐值加密，生成密文
char *genCipherText(char *salt, char *password){
    //如果salt相同，password相同，则crypt()返回的字符串相同
    char *cipherText = crypt(password, salt);
    return cipherText;
}

