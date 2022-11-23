/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：recursion.c
*   @author     ：anonymous
*   @date       ：2021年09月11日 17:51:49
*   @description：
*
================================================================*/

#include <stdio.h>

int rr(int x){
    int q;
    static int ctrl;
    static int odds;
    if(x < 9){
        ++ctrl;
        if(ctrl < 3){
            q = x / 2;
        }else{
            q = x / 3;
        }
        if(ctrl > 1){
            q += ctrl;
        }
    }else{
        q = rr(x - 2) + rr(x / 3);
        /* q = rr(x / 3) + rr(x - 2); // 不等于上一个, 左结合性*/
    }

    if((odds + q) % 2 == 0){
        printf("I like %d\n", q);
    }
    if((odds + q) % 2 == 1){
        printf("I hate %d\n", q);
    }
    return q;
}

int main (){
    printf("%d\n", rr(13));
    /* printf("%d\n", rr(rr(rr(13) - 3) - 5)); */
    return 0;
}

