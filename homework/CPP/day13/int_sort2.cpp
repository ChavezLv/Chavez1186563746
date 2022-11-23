#include<iostream>
using namespace std;
void sort(int *arr,int size){
    int lp=0;
    int rp=size-1;
    int tmp=0;
    cout<<"1"<<endl;
    while(lp<rp){
        if(arr[lp]%2==1){
            lp++;
            continue;
        }
        if(arr[rp]%2==0){
            rp--;
            continue;
        }
    cout<<"0"<<endl;
        tmp=arr[rp];
        arr[rp]=arr[lp];
        arr[lp]=tmp;
        rp--;
        lp++;
    }
    cout<<"2"<<endl;
}
int main(){
    int arr[10]={1,2,4,5,9,4,6,6,9,7};
    for (auto i:arr){
        cout<<i<<" ";
    }
    cout<<endl;
    sort(arr,10);
    for (auto i:arr){
        cout<<i<<" ";
    }
    cout<<endl;
    return 0;
}
