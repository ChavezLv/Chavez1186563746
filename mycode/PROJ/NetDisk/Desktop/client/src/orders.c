#include"../include/work_Queue.h"
#include"../include/orders.h"
int beginSystem(int sfd)
{
    Order_t order;
    reOrder_t reOrder;
    char buf[96];
    system("clear");
    while(1){
        memset(&order, 0, sizeof(order));
        memset(&reOrder, 0, sizeof(reOrder));
        printf("$$$please choose:\n1、load\n2、register\n3、exit\n>>");
        memset(buf, 0, sizeof(buf));
        scanf("%s", buf);
        while(getchar() != '\n');
        if(buf[0] == '1'){
            order.orderNum = LOAD;
            strcpy(order.orderTarget, "user:liuergouzi password:123456");
            send(sfd, &order, sizeof(order), 0);
            recv(sfd, &reOrder, sizeof(reOrder), MSG_WAITALL);
            if(reOrder.reOrderNum == 0){
                system("clear");
                printf("%s\n", reOrder.reInfo);
                return 0;
            }
        }
        else if(buf[0] == '2'){
            order.orderNum = REGISTER;
            strcpy(order.orderTarget, "user:liuergouzi password:123456");
            send(sfd, &order, sizeof(order), 0);
            recv(sfd, &reOrder, sizeof(reOrder), MSG_WAITALL);
            printf("%s\n", reOrder.reInfo);
        }
        else if(buf[0] == '3'){
            printf("over\n");
            order.orderNum = EXIT;
            send(sfd, &order, sizeof(order), 0);
            close(sfd);
            return -1;
        }
        else{
            printf("error cmd\n");
        }
    }
}

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
        while((ch = getchar()) != ' ' && ch != '\n' && i < 1024){
            buf_orderTarget[i] = ch;
            ++i;
        }
    }

    ret = changeOrder(buf_orderNum, &order.orderNum);
    if(ret == -1){
        printf("error cmd\n");
        return -1;
    }
    strcpy(order.orderTarget, buf_orderTarget);
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

int read_Order(int sfd)
{
    reOrder_t reOrder;
    memset(&reOrder, 0, sizeof(reOrder));
    recv(sfd, &reOrder, sizeof(reOrder), MSG_WAITALL);
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
