#include "../include/head.h"
#include "encryption.h"

//生成两字节随机字符串（盐值），用于与密码加密生成密文。
char *genRandomString(int length){
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

