#include<iostream>
#include<set>
using namespace std;
//时间复杂度与空间复杂度的权衡;
void odd_even(int *arr,int size){
    int tmp[size];
    int i=0;
    for(int s=0;s<size;++s){
        if(arr[s]%2!=0){
            tmp[i]=arr[s];
            ++i;
        }
    }
    int j=0;
    for(int z=0;z<size;++z){
        if(arr[z]%2==0)
        {tmp[i+j]=arr[z];
            ++j;
    }}
    cout<<arr[5]<<endl<<tmp[5]<<endl;
    for(int k=0;k<size;++k){
        arr[k]=tmp[k];
    }
}
int main(){
    int arr[10]={1,2,4,5,9,4,6,6,9,7};
    for (auto i:arr){
        cout<<i<<" ";
    }
    cout<<endl;
    odd_even(arr,10);
    for (auto i:arr){
        cout<<i<<" ";
    }
    cout<<endl;
    return 0;
}
