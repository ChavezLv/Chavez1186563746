#include<iostream>
using namespace std;

/*****************************************************/
/*				      快速排序					     */
/*****************************************************/
int partition(int arr[], int low, int high) {
	int pivot = arr[low];
	int i = low, j = high;
	while (i < j) {
		// 先移动j
		while (i < j && arr[j] >= pivot) {
			j--;
		}
		arr[i] = arr[j];

		// 再移动i
		while (i < j && arr[i] <= pivot) {
			i++;
		}
		arr[j] = arr[i];
	}
	// 放置基准值
	arr[i] = pivot;


	//print_array(arr, 10);
	return i;
}

void _quick_sort(int arr[], int low, int high) {
	// 边界条件
	if (low >= high) return;
	// 递归公式
	// 分区
	int idx = partition(arr, low, high); // idx为基准值位于的索引
	// 对左边区间排序
	_quick_sort(arr, low, idx - 1);
	// 对右边区间排序
	_quick_sort(arr, idx + 1, high);
}

void quick_sort(int arr[], int n) {
	_quick_sort(arr, 0, n - 1);
}
int main(){
    int arr[10]={21,3,1,45,6,1,7,3,5,7};
    _quick_sort(arr,0,9);
    for(int i=0;i<10;++i){
        cout<<arr[i]<<" ";
    }
    cout<<endl;
    return 0;

}
