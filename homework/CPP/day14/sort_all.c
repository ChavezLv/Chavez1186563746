#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>
#include<time.h>
#define N 25000
#define SWAP(arr, i, j) {	\
	int tmp = arr[i];		\
	arr[i] = arr[j];		\
	arr[j] = tmp;			\
}							\

clock_t start_t, end_t;
long double total_t;
void print_array(int arr[], int n);

void bubble_sort(int arr[], int n);
void selection_sort(int arr[], int n);
void insertion_sort(int arr[], int n);
void shell_sort(int arr[], int n);
void merge_sort(int arr[], int n);
void quick_sort(int arr[], int n);
void heap_sort(int arr[], int n);


void timing(char sort_al[]);
void perform_test(int n);
void copy(int* arr, int* temp,int len);


int main(void) {
	int coeffi[5] = { 1000,5000,25000,125000,625000 };
	for (int i = 0; i < 5; i++) {
		perform_test(coeffi[i]);
	}
	return 0;
}
void perform_test(int len) {
	printf("%d个元素时：\n",len);
	int* arr =(int*) calloc(len, sizeof(int));
	int* temp = (int*)calloc(len, sizeof(int));
	srand(time(0));  //设置时间种子
	for (int i = 0; i < len; i++) {
		arr[i] = rand() % 25000;//生成区间625000~l的随机数 
	}
	//冒泡
	copy(arr,temp,len);
	start_t = clock();
	bubble_sort(arr, len);
	timing("bubble_sort");
	//选择
	copy(temp,arr, len);
	start_t = clock();
	selection_sort(arr, len);
	timing("selection_sort");
	//插入
	copy(temp, arr, len);
	start_t = clock();
	insertion_sort(arr, len);
	timing("insertion_sort");
	//希尔
	copy(temp, arr, len);
	start_t = clock();
	shell_sort(arr, len);
	timing("shell_sort");
	//归并
	copy(temp, arr, len);
	start_t = clock();
	merge_sort(arr, len);
	timing("merge_sort");
	//快速
	copy(temp, arr, len);
	start_t = clock();
	quick_sort(arr, len);
	timing("quick_sort");
	//堆排序
	copy(temp, arr, len);
	start_t = clock();
	heap_sort(arr, len);
	timing("heap_sort");
	printf("\n");
}
void timing(char sort_al[]) {
	end_t = clock();
	total_t = (long double)(end_t - start_t) / CLOCKS_PER_SEC;
	printf("%s算法所用时长：%f\n",sort_al,total_t);
}
void copy(int* arr, int* temp, int len) {
	for (int i = 0; i < len; i++) {
		temp[i] = arr[i];
	}
}

void print_array(int arr[], int n) {
	for (int i = 0; i < n; ++i) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}


/*****************************************************/
/*				      冒泡排序					     */
/*****************************************************/
void bubble_sort(int arr[], int n) {
	for (int i = 1; i < n; ++i) { // 冒泡次数
		bool isSorted = true;

		for (int j = 0; j < n - i; ++j) { // 比较索引为j和j+1的元素
			if (arr[j] > arr[j + 1]) { // 不要写成 arr[j] >= arr[j+1], 冒泡就不稳定
				SWAP(arr, j, j + 1);
				isSorted = false;
			}
 		}

		if (isSorted) break;	// 如果数组已经有序，就提前退出

		//print_array(arr, 10);
	}
}


/*****************************************************/
/*				      选择排序					     */
/*****************************************************/
void selection_sort(int arr[], int n) {
	for (int i = 0; i < n - 1; ++i) { // i表示这次选择最小元素应该放置的位置
		int minIdx = i;
		for (int j = i + 1; j < n; ++j) {
			if (arr[j] < arr[minIdx])
				minIdx = j;
		}
		// minIdx指向未排序序列的最小元素
		SWAP(arr, i, minIdx);

		//print_array(arr, 10);
	}
}


/*****************************************************/
/*				      插入排序					     */
/*****************************************************/
void insertion_sort(int arr[], int n) {
	for (int i = 1; i < n; ++i) {
		int value = arr[i]; // 待插入的元素
		int j = i - 1;
		// 从后往前找value要插入的位置
		while (j >= 0 && arr[j] > value) {
			arr[j + 1] = arr[j];
			j--;
		}
		// 插入value
		arr[j + 1] = value;

		//print_array(arr, 10);
	}
}


/*****************************************************/
/*				      希尔排序					     */
/*****************************************************/
void shell_sort(int arr[], int n) {
	int gap = n / 2;
	while (gap != 0) {
		// 组间插入排序
		for (int i = gap; i < n; ++i) {
			int value = arr[i]; // 待插入的元素
			int j = i - gap;
			// 从后往前找插入位置
			while (j >= 0 && arr[j] > value) {
				arr[j + gap] = arr[j];
				j -= gap;
			}
			arr[j + gap] = value;
		}

		//print_array(arr, 10);
		// 缩小增量
		gap = gap / 2;
	}
}

/*****************************************************/
/*				      归并排序					     */
/*****************************************************/
void merge(int arr[], int low, int mid, int high) {
	// 申请临时数组
	int* tmp =(int*) malloc((high - low + 1) * sizeof(int));
	if (tmp == NULL) {
		printf("malloc failed in merge\n");
		exit(1);
	}
	int i = low, j = mid + 1, k = 0;
	// 两个区间都有元素
	while (i <= mid && j <= high) {
		if (arr[i] <= arr[j]) {  // 注意不能写成<
			tmp[k++] = arr[i++];
		}
		else {
			tmp[k++] = arr[j++];
		}
	}
	// 如果左边区间有元素
	while (i <= mid) {
		tmp[k++] = arr[i++];
	}
	// 如果右边区间有元素
	while (j <= high) {
		tmp[k++] = arr[j++];
	}
	// 复制元素
	for (int k = 0; k < high - low + 1; ++k) {
		arr[low + k] = tmp[k];
	}
	// 释放
	free(tmp);}

void _merge_sort(int arr[], int low, int high) {
	// 边界条件
	if (low >= high) return;
	// 递归公式
	int mid = low + (high - low >> 1);   // 避免溢出
	// 对左边区间排序
	_merge_sort(arr, low, mid);
	// 对右边区间排序
	_merge_sort(arr, mid + 1, high);
	// 合并成整体有序的数组
	merge(arr, low, mid, high);

	//print_array(arr, 10);
}

void merge_sort(int arr[], int n) {
	// 委托
	_merge_sort(arr, 0, n - 1);
}


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

/*****************************************************/
/*				      堆排序					         */
/*****************************************************/
void heapify(int arr[], int i, int len) {
	while (i < len) {
		// 找 i, lchild, rchild的最大值
		int lchild = 2 * i + 1;
		int rchild = 2 * i + 2;
		int maxIdx = i;
		if (lchild < len && arr[lchild] > arr[maxIdx]) {
			maxIdx = lchild;
		}
		if (rchild < len && arr[rchild] > arr[maxIdx]) {
			maxIdx = rchild;
		}
		if (maxIdx == i) break;
		// 交换
		SWAP(arr, i, maxIdx);
		// 递归到下一个结点
		i = maxIdx;
	}
}

void build_heap(int arr[], int n) {
	// 找第一个非叶子结点, 从后往前构建大顶堆
	// leftchild(i) = 2i + 1 <= n-1   i <= (n-2)/2
	for (int i = (n - 2 >> 1); i >= 0; --i) {
		heapify(arr, i, n);
	}
}

void heap_sort(int arr[], int n) {
	// 构建大顶堆
	build_heap(arr, n);
	//print_array(arr, 10);
	int len = n; // len表示无序区的长度
	while (len > 1) {
		// 交换堆顶和无序区最后一个元素
		SWAP(arr, 0, len - 1);
		len--;
		// 把堆重新调整成大顶堆
		heapify(arr, 0, len);
		//print_array(arr, 10);
	}
}
