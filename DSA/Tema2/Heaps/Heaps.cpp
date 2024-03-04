/*
Pentru metoda BottomUp ne vom folosi de functia Heapify cu o complexitate O(logn)
Astfel, avand un loop in functia BottomUp, complexitatea va fi O(n*logn)

Pentru TopDown avem functai de Insert cu o complexitate lgn si functia IncreaseKey cu aceeasi complexitate.
*/
#include <stdio.h>
#include <stdlib.h>
#define STEP 100
#define MAX 10000
#define TESTS 5
#include "Profiler.h"

Profiler Heap("Average");

void Heapify(int* H, int i, int n, Operation Total)
{
	int largest = i;
	int l = 2 * i +1;
	int r = 2 * i + 2;
	Total.count();
	if(l < n && H[l] > H[largest])
	{
		largest = l;
	}
	Total.count();
	if (r < n && H[r] > H[largest])
	{
		largest = r;
	}
	if (largest != i)
	{
		Total.count(3);
		int temp = H[i];
		H[i] = H[largest];
		H[largest] = temp;
		Heapify(H, largest, n, Total);
	}
}

void BottomUp(int* H, int n)
{
	Operation BUTotal = Heap.createOperation("BUTotal", n);
	for (int i = n / 2 - 1; i >= 0; i--)
	{
		Heapify(H, i, n, BUTotal);
	}
}

void IncreaseKey(int *H, int i, int k, Operation TDTotal) 
{
	TDTotal.count();
	H[i] = k;
	TDTotal.count();
	while (i > 0 && H[(i - 1) / 2] < H[i])
	{
		TDTotal.count();
		int temp = H[(i - 1) / 2];
		H[(i - 1) / 2] = H[i];
		H[i] = temp;
		TDTotal.count(3);
		i = (i - 1) / 2;
	}
}

void MaxHeapInsert(int *H, int k, int *size, Operation TDTotal)
{
	(*size)++;
	TDTotal.count();
	H[(* size) - 1] = -100000000;
	IncreaseKey(H, (*size -1), k, TDTotal);
}

void TopDown(int* H, int *size, int n)
{
	Operation TDTotal = Heap.createOperation("TDTotal", n);
	(*size) = 0;
	for (int i = 0; i < n; i++)
	{
		MaxHeapInsert(H, H[i], size,TDTotal);
	}
}

void HeapSort(int* H, int n)
{
	Operation HSTotal = Heap.createOperation("HSTotal", n);
	for (int i = n - 1; i >= 0; i--)
	{
		HSTotal.count(3);
		int temp = H[0];
		H[0] = H[i];
		H[i] = temp;
		Heapify(H, 0, i-1, HSTotal);
	}
}

void test(int order)
{
	int a[MAX], b[MAX];
	int size = 0;
	for (int n = STEP; n <= MAX; n += STEP)
	{
		for (int test = 0; test < TESTS; test++)
		{
			FillRandomArray(a, n, 0, 10000, false, order);
			for (int i = 0; i < n; i++)
			{
				b[i] = a[i];
			}
			BottomUp(a, n);
			TopDown(b, &size, n);
		}
	}
	Heap.divideValues("BUTotal", TESTS);
	Heap.divideValues("TDTotal", TESTS);
	Heap.createGroup("BU/TD", "BUTotal", "TDTotal");
}

void test_all()
{
	test(UNSORTED);

	Heap.reset("Worst");
	test(ASCENDING);

	Heap.showReport();

}
void demo()
{
	int a[] = { 2, 7, 6, 5 ,8, 3, 8};
	int b[] = { 2, 7, 6, 5 ,8, 3, 8};
    int n = sizeof(a) / sizeof(int);
	int size;
	BottomUp(a, n);
	TopDown(b, &size, n);
	for(int i=0;i<n;i++)
	{
		printf("%d ", a[i]);
	}
	puts("");
	for (int i = 0; i < n; i++)
	{
		printf("%d ", b[i]);
	}
	TopDown(b, &size, n);
	HeapSort(a, n);
	puts("");
		for(int i=0;i<n;i++)
	{
		printf("%d ", a[i]);
	}
}

int main()
{
	demo();
	//test_all ();
	return 0;
}