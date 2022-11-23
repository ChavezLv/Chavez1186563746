/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：a0013testListReverse.c
*   @author     ：anonymous
*   @description：
*
================================================================*/


#include <iostream>
#include <vector>
#include <list>

using namespace std;

struct ListNode{
    int val;
    ListNode * next;
    ListNode(int x)
    : val(x)
    , next(nullptr){

    }

};
using ListNodePtr = ListNode*;
class Solution{
public:

    void headInsert(ListNodePtr *ppHead, ListNodePtr *ppTail, const int &val){
        ListNodePtr pNew = new ListNode(val);
        /* pNew->val = val; */

        if(*ppHead == nullptr){
            *ppHead = pNew;
            *ppTail = pNew;
        }else{
            pNew->next = *ppHead;
            *ppHead = pNew;
        }
    }

    ListNode* reverseList(ListNode* pHead){
        if(pHead == nullptr){
            return nullptr;
        }
        ListNode* pCur = pHead;
        ListNode* pPre = nullptr;
        ListNode* pRear = pHead->next;

        while(pRear){
            pCur->next = pPre;

            pPre = pCur;
            pCur = pRear;
            pRear = pRear->next;

        }
        pCur->next = pPre;
        return pCur;
    }
    void reverseList(ListNodePtr *ppHead, ListNodePtr* ppTail){
        if(*ppHead == *ppTail){
            return;
        }
        ListNodePtr pPre = *ppHead;
        ListNodePtr pCur = (*ppHead)->next;
        ListNodePtr pRear = (*ppHead)->next->next;
        *ppTail = pPre;
        *ppHead = pCur;
        pPre->next = nullptr;
        while(pRear){
            pCur->next = pPre;
            pPre = pCur;
            pCur = pRear;
            pRear = pRear->next;

        }
        pCur->next = pPre; 
    }

    void reverseList(ListNodePtr *ppHead, ListNodePtr* ppTail){
        /* if(*ppHead == nullptr || (*ppHead)->next == (*ppTail)->next){ //ok*/
        if(*ppHead== *ppTail){
            cout << "only 1" << endl;
            return;
        }
        ListNodePtr pCur = (*ppHead)->next;
        ListNodePtr pPre = *ppHead;
        ListNodePtr pRear = (*ppHead)->next->next; 
        
        *ppTail = pPre; 
        *ppHead = pCur;

        pPre->next = nullptr;
        while(pRear){
            pCur->next = pPre;

            pPre = pCur;
            pCur = pRear;
            pRear = pRear->next;
        }
        pCur->next = pPre;

        *ppHead = pCur;

    }

    void printList(ListNodePtr pHead){
        ListNodePtr pCur = pHead;
        while(pCur){
            cout << pCur->val << " " ;
            pCur = pCur->next;
        }
        cout << endl;

    }
};

void test01(){
    ListNodePtr pHead = nullptr;
    ListNodePtr pTail = nullptr;

    vector<int> vec = {1, 3, 4, 5, 6, 7};
    /* vector<int> vec = {1, 3}; */
    /* vector<int> vec = { 3}; */
    /* vector<int> vec = {}; */
    for(auto &elem: vec){
        Solution().headInsert(&pHead, &pTail, elem);
    }

    Solution().printList(pHead);
    /* Solution().reverseList(&pHead, &pTail); */
    ListNodePtr pNewHead = Solution().reverseList(pHead);
    Solution().printList(pNewHead);
}
static int cnt = 0;
int f(int n)
		{
			if(n==0)  { return 1; }
			if(n==1)  { return 2; }
            if(n==2) { return 2 * 1 * 1; }
            
            ++cnt;
			return n*f(n-1)*(n-2);
			/* return n*f(n-1); */
		}

void test02(){
    int result = f(100);
    cout << "cnt: " << cnt << endl;
    cout << "result: " << result << endl;
}

int main(){
    test01();
    /* test02(); */
    return 0;
}
