/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：heap.c
*   @author     ：anonymous
*   @date       ：2021年09月08日 17:34:06
*   @description：
*
================================================================*/

#include <stdio.h>

auto swap = [](int& a, int & b){
    int tmp = 0;
    tmp = a;
    a = b;
    b = tmp;
};

void adjustMaxHeap(int *arr, int dIndex, int len){
	int dadIndex = dIndex;
	// dadPos = dadIndex + 1 ;
	// sonPos = 2 * dadPos = 2dadInex + 2;
	int sonIndex = 2 * dIndex + 2 - 1;
	while(sonIndex < len){
		if(sonIndex + 1 < len && arr[sonIndex + 1] > arr[sonIndex]){
			//如果右孩子下标小于len，且左孩子小于右孩子, 则兄弟之争完成
			++sonIndex;
		}
		if(arr[sonIndex] > arr[dadIndex]){
			//如果父子之争儿子大， 则交换位置
            swap(arr[sonIndex], arr[dadIndex]);
            //然后继续往下进行
			dadIndex = sonIndex;
			sonIndex = 2 * dadIndex + 1;
        }else{
            break;
        }
	}
}
void heapSort(int* arr, int len){
	for(int dIndex = len / 2 - 1; dIndex >= 0; --dIndex){
		adjustMaxHeap(arr, dIndex, len);
	}
    swap(arr[0], arr[len - 1]);
    for(int adjustLen = len - 1; adjustLen >= 2; --adjustLen){
        adjustMaxHeap(arr, 0, adjustLen);
        swap(arr[0], arr[adjustLen - 1]);
    }
}

void test01(){
    int a = 10;
    int b = 20;
    swap(a, b);
    printf("%d, %d\n", a, b);

}

int main(){
    int arr[10] = { 3, 4, 4, 5, 6, 8, 2, 9, 1 ,};
    heapSort(arr, 10);
    for(int i = 0; i < 10; ++i){
        printf("%d ", i);
    }
    printf("\n");
    return 0;
}
