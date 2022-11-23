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
	printf("%d��Ԫ��ʱ��\n",len);
	int* arr =(int*) calloc(len, sizeof(int));
	int* temp = (int*)calloc(len, sizeof(int));
	srand(time(0));  //����ʱ������
	for (int i = 0; i < len; i++) {
		arr[i] = rand() % 25000;//��������625000~l������� 
	}
	//ð��
	copy(arr,temp,len);
	start_t = clock();
	bubble_sort(arr, len);
	timing("bubble_sort");
	//ѡ��
	copy(temp,arr, len);
	start_t = clock();
	selection_sort(arr, len);
	timing("selection_sort");
	//����
	copy(temp, arr, len);
	start_t = clock();
	insertion_sort(arr, len);
	timing("insertion_sort");
	//ϣ��
	copy(temp, arr, len);
	start_t = clock();
	shell_sort(arr, len);
	timing("shell_sort");
	//�鲢
	copy(temp, arr, len);
	start_t = clock();
	merge_sort(arr, len);
	timing("merge_sort");
	//����
	copy(temp, arr, len);
	start_t = clock();
	quick_sort(arr, len);
	timing("quick_sort");
	//������
	copy(temp, arr, len);
	start_t = clock();
	heap_sort(arr, len);
	timing("heap_sort");
	printf("\n");
}
void timing(char sort_al[]) {
	end_t = clock();
	total_t = (long double)(end_t - start_t) / CLOCKS_PER_SEC;
	printf("%s�㷨����ʱ����%f\n",sort_al,total_t);
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
/*				      ð������					     */
/*****************************************************/
void bubble_sort(int arr[], int n) {
	for (int i = 1; i < n; ++i) { // ð�ݴ���
		bool isSorted = true;

		for (int j = 0; j < n - i; ++j) { // �Ƚ�����Ϊj��j+1��Ԫ��
			if (arr[j] > arr[j + 1]) { // ��Ҫд�� arr[j] >= arr[j+1], ð�ݾͲ��ȶ�
				SWAP(arr, j, j + 1);
				isSorted = false;
			}
 		}

		if (isSorted) break;	// ��������Ѿ����򣬾���ǰ�˳�

		//print_array(arr, 10);
	}
}


/*****************************************************/
/*				      ѡ������					     */
/*****************************************************/
void selection_sort(int arr[], int n) {
	for (int i = 0; i < n - 1; ++i) { // i��ʾ���ѡ����СԪ��Ӧ�÷��õ�λ��
		int minIdx = i;
		for (int j = i + 1; j < n; ++j) {
			if (arr[j] < arr[minIdx])
				minIdx = j;
		}
		// minIdxָ��δ�������е���СԪ��
		SWAP(arr, i, minIdx);

		//print_array(arr, 10);
	}
}


/*****************************************************/
/*				      ��������					     */
/*****************************************************/
void insertion_sort(int arr[], int n) {
	for (int i = 1; i < n; ++i) {
		int value = arr[i]; // �������Ԫ��
		int j = i - 1;
		// �Ӻ���ǰ��valueҪ�����λ��
		while (j >= 0 && arr[j] > value) {
			arr[j + 1] = arr[j];
			j--;
		}
		// ����value
		arr[j + 1] = value;

		//print_array(arr, 10);
	}
}


/*****************************************************/
/*				      ϣ������					     */
/*****************************************************/
void shell_sort(int arr[], int n) {
	int gap = n / 2;
	while (gap != 0) {
		// ����������
		for (int i = gap; i < n; ++i) {
			int value = arr[i]; // �������Ԫ��
			int j = i - gap;
			// �Ӻ���ǰ�Ҳ���λ��
			while (j >= 0 && arr[j] > value) {
				arr[j + gap] = arr[j];
				j -= gap;
			}
			arr[j + gap] = value;
		}

		//print_array(arr, 10);
		// ��С����
		gap = gap / 2;
	}
}

/*****************************************************/
/*				      �鲢����					     */
/*****************************************************/
void merge(int arr[], int low, int mid, int high) {
	// ������ʱ����
	int* tmp =(int*) malloc((high - low + 1) * sizeof(int));
	if (tmp == NULL) {
		printf("malloc failed in merge\n");
		exit(1);
	}
	int i = low, j = mid + 1, k = 0;
	// �������䶼��Ԫ��
	while (i <= mid && j <= high) {
		if (arr[i] <= arr[j]) {  // ע�ⲻ��д��<
			tmp[k++] = arr[i++];
		}
		else {
			tmp[k++] = arr[j++];
		}
	}
	// ������������Ԫ��
	while (i <= mid) {
		tmp[k++] = arr[i++];
	}
	// ����ұ�������Ԫ��
	while (j <= high) {
		tmp[k++] = arr[j++];
	}
	// ����Ԫ��
	for (int k = 0; k < high - low + 1; ++k) {
		arr[low + k] = tmp[k];
	}
	// �ͷ�
	free(tmp);}

void _merge_sort(int arr[], int low, int high) {
	// �߽�����
	if (low >= high) return;
	// �ݹ鹫ʽ
	int mid = low + (high - low >> 1);   // �������
	// �������������
	_merge_sort(arr, low, mid);
	// ���ұ���������
	_merge_sort(arr, mid + 1, high);
	// �ϲ����������������
	merge(arr, low, mid, high);

	//print_array(arr, 10);
}

void merge_sort(int arr[], int n) {
	// ί��
	_merge_sort(arr, 0, n - 1);
}


/*****************************************************/
/*				      ��������					     */
/*****************************************************/
int partition(int arr[], int low, int high) {
	int pivot = arr[low];
	int i = low, j = high;
	while (i < j) {
		// ���ƶ�j
		while (i < j && arr[j] >= pivot) {
			j--;
		}
		arr[i] = arr[j];

		// ���ƶ�i
		while (i < j && arr[i] <= pivot) {
			i++;
		}
		arr[j] = arr[i];
	}
	// ���û�׼ֵ
	arr[i] = pivot;


	//print_array(arr, 10);
	return i;
}

void _quick_sort(int arr[], int low, int high) {
	// �߽�����
	if (low >= high) return;
	// �ݹ鹫ʽ
	// ����
	int idx = partition(arr, low, high); // idxΪ��׼ֵλ�ڵ�����
	// �������������
	_quick_sort(arr, low, idx - 1);
	// ���ұ���������
	_quick_sort(arr, idx + 1, high);
}

void quick_sort(int arr[], int n) {
	_quick_sort(arr, 0, n - 1);
}

/*****************************************************/
/*				      ������					         */
/*****************************************************/
void heapify(int arr[], int i, int len) {
	while (i < len) {
		// �� i, lchild, rchild�����ֵ
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
		// ����
		SWAP(arr, i, maxIdx);
		// �ݹ鵽��һ�����
		i = maxIdx;
	}
}

void build_heap(int arr[], int n) {
	// �ҵ�һ����Ҷ�ӽ��, �Ӻ���ǰ�����󶥶�
	// leftchild(i) = 2i + 1 <= n-1   i <= (n-2)/2
	for (int i = (n - 2 >> 1); i >= 0; --i) {
		heapify(arr, i, n);
	}
}

void heap_sort(int arr[], int n) {
	// �����󶥶�
	build_heap(arr, n);
	//print_array(arr, 10);
	int len = n; // len��ʾ�������ĳ���
	while (len > 1) {
		// �����Ѷ������������һ��Ԫ��
		SWAP(arr, 0, len - 1);
		len--;
		// �Ѷ����µ����ɴ󶥶�
		heapify(arr, 0, len);
		//print_array(arr, 10);
	}
}
