#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
/* class Solution { */
/* public: */

/* }; */
#define M 1000
typedef struct node_s {
    int index;
    int val;
    struct node_s* pNext;

}Node_t, * pNode_t;
int hashfunc(int num) {
    return num % M;

}
void inset(pNode_t hash[], int nums[],int i) {
    int hashval1 = hashfunc(nums[i]);//返回的hashval
    if (hash[abs(hashval1)] == NULL) {
        hash[abs(hashval1)] = (pNode_t)calloc(1, sizeof(Node_t));
        hash[abs(hashval1)]->index = i;
        hash[abs(hashval1)]->val = nums[i];
        if (i == 0) {
            hash[abs(hashval1)]->index = -1;

        }

    }
    else {
        pNode_t pNew = (pNode_t)calloc(1, sizeof(Node_t));
        pNew->index = i;
        pNew->val = nums[i];
        if (i == 0) {
            pNew->index = -1;

        }
        pNode_t pCur = hash[abs(hashval1)];
        while (pCur->pNext) {
            pCur = pCur->pNext;

        }
        pCur->pNext = pNew;

    }

}
int* twoSum(int* nums, int numsSize, int target, int* returnSize) {
    int* retArr = (int*)calloc(2, sizeof(int));
    pNode_t hash[M] = { 0  };
    int logo = 0;
    for (int i = 0; i < numsSize; ++i) {
        int hashval2 = hashfunc(target - nums[i]);
        if (hash[abs(hashval2)] != NULL && hash[abs(hashval2)]->index != 0) {
            pNode_t pCur = hash[abs(hashval2)];
            while (pCur) {
                if (pCur->val + nums[i] == target) {
                    retArr[1] = i;
                    retArr[0] = pCur->index;
                    if (pCur->index == -1) {
                        retArr[0] = 0;

                    }
                    logo = 2;
                    break;

                }
                pCur = pCur->pNext;

            }
            if(logo==0) {
                inset(hash, nums, i);

            }else {
                break;

            }

        }
        else {
            inset(hash, nums, i);

        }

    }
    *returnSize = 2;
    return retArr;

}
int main()
{

    return 0;
}

