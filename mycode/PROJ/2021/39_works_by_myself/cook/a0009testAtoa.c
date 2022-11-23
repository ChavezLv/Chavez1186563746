/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：a0008alpha.c
*   @author     ：anonymous
*   @date       ：2021年09月12日 16:42:07
*   @description：
*
================================================================*/


#include <stdio.h>

int main(){

    char inputChar;
    while(inputChar = getchar(), inputChar != EOF){
        if(inputChar >= 97 && inputChar <= 122){
            inputChar -= 32;
        }else if(inputChar >= 65 && inputChar <= 90){

        }else{
            printf("input error alpha\n");
            break;
        }
        printf("to %c\n", inputChar);
        getchar();// get \n
    }

    return 0;
}
