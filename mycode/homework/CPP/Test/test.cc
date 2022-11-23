#include<iostream>
using namespace std;
int func(int x)
{
int countx = 0;
while(x)
{
countx++;
x = x&(x-1);
}
return countx;
}
int main()
{
cout<<"func(9999)"<<func(9999)<<endl;
int a[5]={1,2,3,4,5};
int *ptr=(int *)(&a+1);
printf("a=%d\na+1=%d\na+2=%d\na+3=%d\na+4=%d\n",
       (a),a+1,a+2,a+3,a+4);
printf("*(a+4)=%d\n",*(a+4));
printf("&(a)=%d\n&a+1=%d\n&a+2=%d\n&a+3=%d\n&a+4=%d\n",
       &(a),&a+1,&a+2,&a+3,&a+4);
printf("*(a+1)=%d\n*(ptr-1)=%d\n",*(a+1),*(ptr-1));
return 0;
}
