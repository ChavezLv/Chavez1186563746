#include"../include/work_Queue.h"
#include"../include/orders.h"
int beginSystem(int sfd)
{
    Order_t order;
    reOrder_t reOrder;
    char buf[96];
    int ret = 0;
    system("clear");
    while(1){
        memset(&order, 0, sizeof(order));
        memset(&reOrder, 0, sizeof(reOrder));
        printf("$$$please choose:\n1、load\n2、register\n3、exit\n>>");
        memset(buf, 0, sizeof(buf));
        scanf("%s", buf);
        while(getchar() != '\n');
        if(buf[0] == '1'){
            ret = order_load(sfd, &order, &reOrder);
            if(ret == 0){
                break;
            }
        }
        else if(buf[0] == '2'){
            order_register(sfd, &order, &reOrder);
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
int order_register(int sfd, pOrder_t pOrder, pReOrder_t pReOrder)
{
    char buf[24] = {0};
    int ret = 0;
    char *password; 
    char *crypt;
    char salt[SALT_LEN];
    char ch;
    int i = 0;
    while(1){
        memset(buf, 0, sizeof(buf));
        i = 0;
        printf("please input username(less than 20):\n");
        while((ch = getchar()) != ' ' && ch != '\n'){
            buf[i] = ch;
            ++i;
        }
        if(ch == ' '){
            printf("username cannot include space!\n\n");
            continue;
        }
        else{
            pOrder->orderNum = REGISTER_USERNAME;
            strcpy(pOrder->orderTarget, buf);
            send(sfd, pOrder, sizeof(Order_t), 0);
            ret = recv(sfd, pReOrder, sizeof(reOrder_t), MSG_WAITALL);
            if(ret == 0){
                printf("connect error\n");
                return -1;
            }
            if(pReOrder->reOrderNum == -1){
                printf("%s\n\n", pReOrder->reInfo);
                continue;
            }
            else{
                break;
            }
        }

    }
    while(1){
        password = getpass("please input password:");
        if(strlen(password) < 6){
            printf("password should be big then 6\n");
            continue;
        }
        if(strlen(password) > 20){
            printf("password should be less then 20\n");
            continue;
        }
        memset(buf, 0, sizeof(buf));
        strcpy(buf, password);
        password = getpass("please input password again:");
        puts(buf);
        puts(password);
        if(strcmp(buf, password) != 0){
            printf("two password are different\n\n");
            continue;
        }
        pOrder->orderNum = REGISTER_CRYPT;
        strcpy(salt, pReOrder->reInfo);
        crypt = genCipherText(salt, password);
        strcpy(pOrder->orderTarget, crypt);
        send(sfd, pOrder, sizeof(Order_t), 0);
        ret = recv(sfd, pReOrder, sizeof(reOrder_t), MSG_WAITALL);
        if(ret == 0){
            printf("connect error\n");
            return -1;
        }
        if(pReOrder->reOrderNum == -1){
            printf("%s\n\n", pReOrder->reInfo);
            continue;
        }
        else{
            printf("%s\n\n", pReOrder->reInfo);
            return 0;
        }
    }
}

int order_load(int sfd, pOrder_t pOrder, pReOrder_t pReOrder)
{
    char buf[24] = {0};
    int ret;
    char *password; 
    char *crypt;
    char salt[SALT_LEN];
    char ch;
    int i = 0;
    while(1){
        memset(buf, 0, sizeof(buf));
        i = 0;
        printf("please input username:\n");
        while((ch = getchar()) != ' ' && ch != '\n'){
            buf[i] = ch;
            ++i;
        }
        if(ch == ' '){
            printf("username is wrong!\n\n");
            continue;
        }
        else{
            pOrder->orderNum = LOAD_USERNAME;
            strcpy(pOrder->orderTarget, buf);
            send(sfd, pOrder, sizeof(Order_t), 0);
            ret = recv(sfd, pReOrder, sizeof(reOrder_t), MSG_WAITALL);
            if(ret == 0){
                printf("connect error\n");
                return -1;
            }
            if(pReOrder->reOrderNum == -1){
                printf("%s\n\n", pReOrder->reInfo);
                continue;
            }
            else{
                break;
            }
        }

    }
    while(1){
        password = getpass("please input password:");
        if(strlen(password) < 6){
            printf("password should be less then 6\n");
            continue;
        }
        if(strlen(password) > 20){
            printf("password should be more then 20\n");
            continue;
        }
        pOrder->orderNum = LOAD_CRYPT;
        strcpy(salt, pReOrder->reInfo);
        crypt = genCipherText(salt, password);
        strcpy(pOrder->orderTarget, crypt);
        send(sfd, pOrder, sizeof(Order_t), 0);
        ret = recv(sfd, pReOrder, sizeof(reOrder_t), MSG_WAITALL);
        if(ret == 0){
            printf("connect error\n");
            return -1;
        }
            
        if(pReOrder->reOrderNum == -1){
            printf("%s\n\n", pReOrder->reInfo);
            continue;
        }
        else{
            printf("%s\n\n", pReOrder->reInfo);
            return 0;
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
    int ret = recv(sfd, &reOrder, sizeof(reOrder), MSG_WAITALL);
    if(ret == 0){
        printf("connect error\n");
        close(sfd);
        exit(1);
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
char *genRandomString(int length)
{
    int flag, i;
    char *string;
    srand((unsigned)time(NULL));
    string = (char*)malloc(length);
    for(i = 0; i < length+1; ++i){
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
    string[length] = '\0';
    return string;
}

//将密码和盐值加密，生成密文
char *genCipherText(char *salt, char *password){
    //如果salt相同，password相同，则crypt()返回的字符串相同
    char *cipherText = crypt(password, salt);
    return cipherText;
}

