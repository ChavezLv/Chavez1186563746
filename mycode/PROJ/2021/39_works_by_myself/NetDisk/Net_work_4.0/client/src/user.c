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
        printf("\n$system>> please choose:\n1、load\n2、register\n3、exit\n>>");
        fflush(stdout);
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
            printf("$system>> error cmd\n");
        }
    }
    return 0;
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
    int wrongTimes = 0;
    while(1){
        if(wrongTimes > 2){
            return 0;
        }
        i = 0;
        memset(buf, 0, sizeof(buf));
        printf("please input username(less than 20):\n");
        while((ch = getchar()) != ' ' && ch != '\n'){
            buf[i] = ch;
            ++i;
        }
        if(ch == ' '){
            printf("$system>> username cannot include space!\n\n");
            return 0;;
        }
        else{
            pOrder->orderNum = REGISTER_USERNAME;
            strcpy(pOrder->orderTarget, buf);
            send(sfd, pOrder, sizeof(Order_t), 0);
            ret = recv(sfd, pReOrder, sizeof(reOrder_t), MSG_WAITALL);
            if(ret == 0){
                printf("$system>> connect error\n");
                exit(1);
            }
            if(pReOrder->reOrderNum == -1){
                printf("%s\n\n", pReOrder->reInfo);
                ++wrongTimes;
            }
            else{
                break;
            }
        }

    }
    wrongTimes = 0;
    while(1){
        if(wrongTimes > 2){
            return 0;
        }
        password = getpass("please input password:\n");
        if(strlen(password) < 6){
            printf("$system>> password should be big then 6\n");
            ++wrongTimes;
            continue;
        }
        if(strlen(password) > 20){
            printf("$system>> password should be less then 20\n");
            ++wrongTimes;
            continue;
        }
        memset(buf, 0, sizeof(buf));
        strcpy(buf, password);
        password = getpass("please input password again:\n");
        puts(buf);
        puts(password);
        if(strcmp(buf, password) != 0){
            printf("$system>> two password are different\n\n");
            ++wrongTimes;
            continue;
        }
        pOrder->orderNum = REGISTER_CRYPT;
        strcpy(salt, pReOrder->reInfo);
        crypt = genCipherText(salt, password);
        strcpy(pOrder->orderTarget, crypt);
        send(sfd, pOrder, sizeof(Order_t), 0);
        ret = recv(sfd, pReOrder, sizeof(reOrder_t), MSG_WAITALL);
        if(ret == 0){
            printf("$system>> connect error\n");
            exit(1);
        }
        if(pReOrder->reOrderNum == -1){
            printf("%s\n\n", pReOrder->reInfo);
            return -1;
        }
        else{
            printf("%s\n\n", pReOrder->reInfo);
            return 0;
        }
    }
    return 0;
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
    int wrongTimes = 0;
    while(1){
        if(wrongTimes > 2){
            return -1;
        }
        memset(buf, 0, sizeof(buf));
        i = 0;
        printf("please input username:\n");
        while((ch = getchar()) != ' ' && ch != '\n'){
            buf[i] = ch;
            ++i;
        }
        if(ch == ' '){
            printf("$system>> username is wrong!\n\n");
            ++wrongTimes;
            continue;
        }
        else{
            pOrder->orderNum = LOAD_USERNAME;
            strcpy(pOrder->orderTarget, buf);
            send(sfd, pOrder, sizeof(Order_t), 0);
            ret = recv(sfd, pReOrder, sizeof(reOrder_t), MSG_WAITALL);
            if(ret == 0){
                printf("$system>> connect error\n");
                exit(1);
            }
            if(pReOrder->reOrderNum == -1){
                printf("%s\n\n", pReOrder->reInfo);
                ++wrongTimes;
                continue;
            }
            else{
                break;
            }
        }

    }
    strcpy(salt, pReOrder->reInfo);
    wrongTimes = 0;
    while(1){
        if(wrongTimes > 2){
            return -1;
        }
        password = getpass("$system>> please input password:\n");
        if(strlen(password) < 6){
            printf("$system>> password should be more then 6\n");
            ++wrongTimes;
            continue;
        }
        if(strlen(password) > 20){
            printf("$system>> password should be less then 20\n");
            ++wrongTimes;
            continue;
        }
        pOrder->orderNum = LOAD_CRYPT;
        crypt = genCipherText(salt, password);
        strcpy(pOrder->orderTarget, crypt);
        send(sfd, pOrder, sizeof(Order_t), 0);
        ret = recv(sfd, pReOrder, sizeof(reOrder_t), MSG_WAITALL);
        if(ret == 0){
            printf("$system>> connect error\n");
            exit(1);
        }
            
        if(pReOrder->reOrderNum == -1){
            printf("%s\n\n", pReOrder->reInfo);
            ++wrongTimes;
            continue;
        }
        else{
            printf("%s\n\n", pReOrder->reInfo);
            return 0;
        }
    }
    return 0;
}

