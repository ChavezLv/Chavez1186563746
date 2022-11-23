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

bool isPrime(int n, int *primeArray, size_t arraySize){
    for(size_t index = 0; index < arraySize; ++index ){
        if(0 == n % primeArray[index]){
            //if can be % to 0
            return false;  
        }
    }
    return true;
}

int getPrimeArray(int n, int * primeArray){
    if(n < 2){
        return 0;
    }
    primeArray[0] = 2;//initial first prime number 2

    int number = 3;//start from 3
    int cntPrime = 1;//start from index 1
    while(number <= n){
        if(isPrime(number, primeArray, cntPrime)){
            primeArray[cntPrime] = number;
            ++cntPrime;
        }
        ++number;
    } 
    return cntPrime;

}
void test01(){

    int *primeArray = (int*)malloc(sizeof(int)*100);
    memset(primeArray, 0, sizeof(int) * 100);

    int numRange = 100;
    int cntPrime = getPrimeArray(numRange, primeArray);
    printf("from 1 to %d have %d prime number.\n", numRange, cntPrime);

    for(int index = 0; index < cntPrime; ++index){
        printf("%d ", primeArray[index]);
    }
    printf("\n");

    free(primeArray);
    primeArray = nullptr;
}
int main(){
    test01();
    return 0;
}
