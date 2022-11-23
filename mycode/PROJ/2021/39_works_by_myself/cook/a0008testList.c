
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Student_s{
    int stuNum;
    char stuName[20];
    double stuScore;
    struct Student_s * pNext;
}Student_t, *pStudent_t;

void printList(pStudent_t pHead){
    pStudent_t pCur = pHead;  
    while(pCur){
        printf("%3d %10s %5lf\n", pCur->stuNum, pCur->stuName, pCur->stuScore);
        pCur = pCur->pNext;
    }
}

void headInsert(pStudent_t *ppHead, pStudent_t *ppTail, const Student_t & stuNode){
    pStudent_t pNew = (pStudent_t) calloc(1, sizeof(Student_t));
    pNew->stuNum = stuNode.stuNum;
    strcpy(pNew->stuName, stuNode.stuName);
    pNew->stuScore = stuNode.stuScore;

    if(*ppHead == nullptr){
        *ppHead = pNew;
        *ppTail = pNew;
    }else{
        pNew->pNext = *ppHead;
        *ppHead = pNew;
    }
}

void test01(){
    pStudent_t pHead = nullptr;
    pStudent_t pTail = nullptr;

    Student_t stuNode;
    int cnt = 4;
    while(--cnt){
        scanf("%d%s%lf", &stuNode.stuNum, stuNode.stuName, &stuNode.stuScore);
        headInsert(&pHead, &pTail, stuNode);
    }

    printList(pHead);

}


int main(int argc, char* argv[])
{
   
       test01();
          return 0;
}
