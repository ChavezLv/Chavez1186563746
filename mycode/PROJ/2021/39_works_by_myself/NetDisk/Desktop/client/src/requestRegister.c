#include"../include/orders.h"
typedef struct DataTrain_s{
    /* size_t len; */
    int len;
    char buf[1024];
}DataTrain_t,*pDataTrian_t;

int requestRegister(int sfd){
    DataTrain_t train;
    //开始注册
    train.len = strlen(train.buf)-1; 
    send(sfd, &train, 4 + train.len, 0);

    do{
        //输入用户名
        printf("please input user name:\n");
        memset(&train.buf, 0, sizeof(train.buf));
        read(STDIN_FILENO, train.buf, sizeof(train.buf)-1);
        train.len = strlen(train.buf)-1;
        send(sfd, &train, 4 + train.len, 0);
        //server确认用户名合法
        memset(&train.buf, 0, sizeof(train.buf));
        recv(sfd, &train.len, 4, 0);
        recv(sfd, train.buf, train.len, 0);
        printf("trainbuf = %s\n",train.buf);
        if(0 == strcmp(train.buf, "1")){
            break;
        }else{
            printf("user name already existed\n");
        }
    }while(1);

    // 接收salt
    memset(&train.buf, 0, sizeof(train.buf));
    recv(sfd, &train.len, 4, 0);
    recv(sfd, train.buf, train.len, 0);

    char salt[SALT_LEN];
    //最后生成的密文占13个字节
    char cipherText[13] = {0};
    char *password;
    //用户输入密码
    password = getpass("input password:");
    strcpy(salt, genRandomString(SALT_LEN));
    strcpy(cipherText, genCipherText(salt, password));

    //发送密文给服务器
    train.len = strlen(cipherText);
    strcpy(train.buf, cipherText);
    send(sfd, &train, 4 + train.len, 0);

    //注册成功
    memset(&train.buf, 0, sizeof(train.buf));
    recv(sfd, &train.len, 4, 0);
    recv(sfd, train.buf, train.len, 0);
    if(0 == strcmp(train.buf, "1")){
        printf("register success\n");
    }
    return 0;
}
