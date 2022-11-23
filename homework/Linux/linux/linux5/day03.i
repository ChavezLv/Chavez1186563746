# 1 "day03.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 31 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 32 "<command-line>" 2
# 1 "day03.c"
int func1(int *arr, int size)
{
    int cnt = 0;
    for(int i = 0; i < size; ++i)
    {
        cnt += arr[i];
    }
    return cnt;
}
void func2(int a, int b)
{
    int tmp = a;
    a = b;
    b = tmp;
}
void func3(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}
int main()
{
    int i = 1;
    int j = 3;
    int k = 5;
    int arr[6] = {1,2,3,4,5,6};
    int res = func1(arr,6);
    func2(i, j);
    func3(&i, &k);
    return 0;
}
