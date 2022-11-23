#include<stdlib.h>
#include<stdio.h>
int maxbit(int data[], int n) //辅助函数，求数据的最大位数
{
    int d = 1; //保存最大的位数
    int p = 10;
    for(int i = 0; i < n; ++i) {
        while(data[i] >= p) {
            p *= 10;
            ++d;
        }
    }
    return d;
}
void radixsort(int data[], int n)   //基数排序
{
    int d = maxbit(data, n);   //数组中的元素的最大位数
    int *tmp = (int *)malloc(n * sizeof(int));
    int *count = (int *)malloc(10 * sizeof(int));   //计数器
    int i, j, k;
    int radix = 1;
    for(i = 1; i <= d; i++) {   //进行d次排序
        for(j = 0; j < 10; j++)
            count[j] = 0;    //每次分配前清空计数器
        for(j = 0; j < n; j++) {
            k = (data[j] / radix) % 10;   //计算每次循环某一位的数字
            count[k]++;    //统计每个桶中的记录数
        }
        for(j = 1; j < 10; j++)
            count[j] = count[j - 1] + count[j];   //第j个桶以及之前所有桶中元素的总数
        for(j = n - 1; j >= 0; j--) {    //将所有桶中记录依次收集到tmp中
            k = (data[j] / radix) % 10;
            tmp[count[k] - 1] = data[j];
            count[k]--;
        }
        for(j = 0; j < n; j++)   //将临时数组的内容复制到data中
            data[j] = tmp[j];
        radix = radix * 10;
    }
    free(tmp);
    free(count);
}
int main()
{
    int a[] = {288, 52, 123, 30, 212, 23, 10, 233};
    int n;
    n = sizeof(a) / sizeof(a[0]);
    radixsort(a, n);
    for(int k = 0; k < n; k++)
        printf("%d ", a[k]);
    printf("\n");
    return 0;
}



