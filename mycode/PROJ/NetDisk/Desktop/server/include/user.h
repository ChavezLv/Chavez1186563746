#ifndef __USER_H__
#define __USER_H__

#include "../include/head.h"
#define SALT_LEN 2


//生成两字节随机字符串（盐值），用于与密码加密生成密文。
char *genRandomString(int length);

//将密码和盐值加密，生成密文
char *genCipherText(char *salt, char *password);

#endif
