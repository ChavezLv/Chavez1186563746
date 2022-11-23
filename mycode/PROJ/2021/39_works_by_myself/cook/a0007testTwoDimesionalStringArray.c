/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：a0006prime.c
*   @author     ：anonymous
*   @date       ：2021年09月12日 16:02:26
*   @description：
*
================================================================*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void test01(){
    char (* pStr)[128] = (char(*)[128]) calloc(1, 128);//数组指针
    int cnt = 0;
    while(scanf("%s", pStr[cnt]) != EOF){
        ++ cnt;
    }

    printf("%d\n", cnt);
    for(int index = 1; index < cnt; ++ index){
        char tmp[128] = { 0 };
        strcpy(tmp, pStr[index]);
        int j;
        for(j = index - 1; j >= 0 && strcmp(tmp, pStr[j]) > 0; --j){
            strcpy(pStr[j + 1], pStr[j]);
        }
        strcpy(pStr[j + 1], tmp);

    }
    
    for(int index = 0; index < cnt; ++ index){
        printf("%s ", pStr[index]);

    }
    printf("\n");

    free(pStr);//valgrind  --tool=memcheck --leak-check=full ./a.out
    pStr = NULL;//应该是可以的吧
}
void test02(){
    char * pStr[100] = { NULL };//指针数组
    /* char *pStr[100];// = (char*[])calloc(100, 128); */
    for(size_t index = 0;index < 100; ++index){
        pStr[index] = (char*)calloc(1, 128);
    }

    int cnt = 0;
    while(scanf("%s", pStr[cnt]) != EOF){
        ++ cnt;
    }

    printf("%d\n", cnt);
    for(int index = 1; index < cnt; ++ index){
        char tmp[128] = { 0 };
        strcpy(tmp, pStr[index]);
        int j;
        for(j = index - 1; j >= 0 && strcmp(tmp, pStr[j]) > 0; --j){
            strcpy(pStr[j + 1], pStr[j]);
        }
        strcpy(pStr[j + 1], tmp);

    }

    for(int index = 0; index < cnt; ++ index){
        printf("%s ", pStr[index]);

    }
    printf("\n");

    /* free(pStr);//error */
    for(int index = 0; index < 100; ++ index){
        free(pStr[index]);
        pStr[index] = NULL;
    }
}
void test03(){
    char **pStr = (char**) calloc(100, sizeof(char*));
    for(size_t index = 0; index < 100; ++index){
        pStr[index] = (char*)calloc(1, 128);
    }
    size_t cnt = 0;
    while(fgets(pStr[cnt], 128, stdin) != NULL){
        size_t len = strlen(pStr[cnt]);
        if(pStr[cnt][len - 1] == '\n'){
            printf("\\n to \\0 \n");
            pStr[cnt][len - 1] = '\0';

        }
        ++cnt;
    }
    for(size_t index = 0; index < cnt; ++ index){
        printf("%s ", pStr[index]);
    }
    size_t test1 = 0;
    --test1;

    printf("test1: %lu\n", test1);

    for(size_t orderedIndex = 1; orderedIndex < cnt; ++orderedIndex){
        char tmp[128] = { 0 };
        strcpy(tmp, pStr[orderedIndex]);
        size_t insertorderedIndex = 0;
        /* for(insertorderedIndex = orderedIndex -1; insertorderedIndex >= 0 */ 
        for(insertorderedIndex = orderedIndex - 1; insertorderedIndex != 0xffffffffffffffff//linux下和win下的size_t不一样! 
            && strcmp(tmp, pStr[insertorderedIndex]) > 0 ; --insertorderedIndex){
            
            printf("-1\n");
            strcpy(pStr[insertorderedIndex + 1], pStr[insertorderedIndex]);
        }
        
        strcpy(pStr[insertorderedIndex + 1], tmp);//0xffffffffffffffff + 1
    }
    printf("ok\n");

    //print
    for(size_t index = 0; index < cnt; ++ index){
        printf("%s ", pStr[index]);
    }
    printf("\n");
    
    //free
    for(size_t index = 0; index < 100; ++ index){
        free(pStr[index]);//只有这个也不行
        pStr[index] = NULL;
    }
    free(pStr);//只有这个不行
}
void test021(){
    char *p[128] = {"hello", "world"};
    p[2] = "ya";
    /* scanf("%s", p[3]);//error */
    char c;
    int i;
    while(c = getchar(), c != '\n'){// error
        p[3][i] = c;
        ++i;
    }
    printf("%p\n", p[3]);//null
    printf("%s\n", p[3]);//error

}
void test033(){
    char p[128] = { 0 };
    scanf("%s", p);
    printf("%s\n", p);//ok
}

int main(){
    /* test01(); */
    /* test02(); */
    test03();
    return 0;
}
