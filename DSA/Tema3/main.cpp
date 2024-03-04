#include<stdio.h>
#include "Profiler.h"
#define STEP 100
#define MAX 10000
#define TESTS 5

Profiler sort("Average");
Profiler BSort("Avg");

void swap(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

int partition(int *a, int p, int r, Operation QSTotal)
{
	QSTotal.count();
	int x = a[r];
	int i = p - 1;
	for (int j = p; j < r; j++)
	{
		QSTotal.count();
		if (a[j] < x)
		{
			i++;
			QSTotal.count(3);
			swap(&a[i], &a[j]);
		}
	}
	QSTotal.count(3);
	swap(&a[i + 1], &a[r]);
	return i + 1;
}

int partitionBestCase(int* a, int p, int r, Operation QSTotalBC)
{
	QSTotalBC.count();
	int x = a[(r-p)/2+p];
	int i = p - 1;
	for (int j = p; j < r; j++)
	{
		QSTotalBC.count();
		if (a[j] < x)
		{
			i++;
			QSTotalBC.count(3);
			swap(&a[i], &a[j]);
		}
	}
	QSTotalBC.count(3);
	swap(&a[i + 1], &a[r]);
	return i + 1;
}

void quickSort(int* a, int p, int r, Operation QSTotal)
{
	QSTotal.count();
	if (p < r)
	{
		int q = partition(a, p, r, QSTotal);
		quickSort(a, p, q - 1, QSTotal);
		quickSort(a, q + 1, r, QSTotal);
	}
}

void quickSortBestCase(int* a, int p, int r, Operation QSTotalBC)
{
	QSTotalBC.count();
	if (p < r)
	{
		int q = partitionBestCase(a, p, r, QSTotalBC);
		quickSort(a, p, q - 1, QSTotalBC);
		quickSort(a, q + 1, r, QSTotalBC);
	}
}

void qS(int*a, int p, int r)
{
	Operation QSTotal = sort.createOperation("QSTotal", r + 1);
	quickSort(a, p, r, QSTotal);
}

void qSBC(int* a, int p, int r)
{
	Operation QSTotalBC = sort.createOperation("QSTotalBC", r + 1);
	quickSortBestCase(a, p, r, QSTotalBC);
}

void heapify(int* H, int i, int n, Operation HSTotal)
{
	int largest = i;
	int l = 2 * i + 1;
	int r = 2 * i + 2;
	HSTotal.count();
	if (l < n && H[l] > H[largest])
	{
		largest = l;
	}
	HSTotal.count();
	if (r < n && H[r] > H[largest])
	{
		largest = r;
	}
	if (largest != i)
	{
		HSTotal.count(3);
		int temp = H[i];
		H[i] = H[largest];
		H[largest] = temp;
		heapify(H, largest, n, HSTotal);
	}
}

void bottomUp(int* H, int n, Operation HSTotal)
{
	for (int i = n / 2 - 1; i >= 0; i--)
	{
		heapify(H, i, n, HSTotal);
	}
}

void heapSort(int* H, int n)
{
	Operation HSTotal = sort.createOperation("HSTotal", n);
	bottomUp(H, n, HSTotal);
	for (int i = n - 1; i >= 0; i--)
	{
		HSTotal.count(3);
		int temp = H[0];
		H[0] = H[i];
		H[i] = temp;
		heapify(H, 0, i - 1, HSTotal);
	}
}

void demo()
{
	int a[] = { 1, 2, 0, 5, 4, 3, 32, 62, 2352, 352, 754, 2, 1, 0, 3463463, 2 };
	int b[] = { 1, 2, 0, 5, 4, 3, 32, 62, 2352, 352, 754, 2, 1, 0, 3463463, 2 };
	int n = sizeof(a) / sizeof(int);
	qS(a, 0, n-1);
	heapSort(b, n);
	for(int i=0;i<n;i++)
	{
		printf("%d ", a[i]);
	}
	puts("");
	for (int i = 0; i < n; i++)
	{
		printf("%d ", b[i]);
	}
}

void demoBC()
{
	int a[] = { 1, 2, 0, 5, 4, 3, 32, 62, 2352, 352, 754, 2, 1, 0, 3463463, 2 };
	int n = sizeof(a) / sizeof(int);
	qSBC(a, 0, n - 1);
	for (int i = 0; i < n; i++)
	{
		printf("%d ", a[i]);
	}
}

void test(int order)
{
	int a[10000], b[10000];
	for (int n = STEP; n <= MAX; n += STEP)
	{
		for (int test = 0; test < TESTS; test++)
		{
			FillRandomArray(a, n, 0, 10000, false, order);
			for (int i = 0; i <n; i++)
			{
				b[i] = a[i];
			}
			qS(a, 0, n - 1);
			heapSort(b, n);
		}
	}
	sort.divideValues("QSTotal", TESTS);
	sort.divideValues("HSTotal", TESTS);
	sort.createGroup("Sort", "HSTotal", "QSTotal");
}

void test_hSqS()
{
	test(UNSORTED);
	
	//Best Case Pivot = a[(low+high)/2+low]

	sort.reset("Worst");
	test(DESCENDING);

	sort.showReport();
}

void testBC()
{
	int a[10000];
	for (int n = STEP; n <= MAX; n += STEP)
	{
		for (int test = 0; test < TESTS; test++)
		{
			FillRandomArray(a, n);
			qSBC(a, 0, n - 1);
		}
	}
	sort.divideValues("QSTotalBC", TESTS);
	sort.showReport();
}

void bubbleSortIterativ(int* a, int n)
{
	Operation BSTotalI = BSort.createOperation("BSTotalI", n);
	int sortat = 1;
	for (int i = 0; i < n - 1; i++)
	{
		sortat = 1;
		for (int j = 0; j < n - 1 - i; j++)
		{
			BSTotalI.count();
			if (a[j] > a[j + 1])
			{
				int temp = a[j];
				a[j] = a[j + 1];
				a[j + 1] = temp;
				BSTotalI.count(3);
				sortat = 0;
			}
		}
		if (sortat)
		{
			break;
		}
	}
}
void bubbleSortRecursiv(int* a, int n, Operation BSTotalR)
{
	if (n == 1)
	{
		return;
	}
	int sortat = 1;
	for (int j = 0; j < n - 1; j++)
	{
		sortat = 1;
		BSTotalR.count();
		if (a[j] >= a[j + 1])
		{
			int temp = a[j];
			a[j] = a[j + 1];
			a[j + 1] = temp;
			sortat = 0;
			BSTotalR.count(3);
		}
	}
	if (sortat)
	{
		return;
	}
	bubbleSortRecursiv(a, n - 1, BSTotalR);
}

void demoBS()
{
	int a[] = { 1, 2, 0, 5, 4, 3, 32, 62, 2352, 352, 754, 2, 1, 0, 3463463, 2 };
	int b[] = { 1, 2, 0, 5, 4, 3, 32, 62, 2352, 352, 754, 2, 1, 0, 3463463, 2 };
	int n = sizeof(a) / sizeof(int);
	bubbleSortIterativ(a, n);
	Operation BSTotalR = BSort.createOperation("BSTotalR", n);
	bubbleSortRecursiv(b, n, BSTotalR);
	for (int i = 0; i < n; i++)
	{
		printf("%d ", a[i]);
	}
	puts(" ");
	for (int i = 0; i < n; i++)
	{
		printf("%d ", b[i]);
	}
}

void testBS(int order)
{
	int a[10000], b[10000], c[10000], d[10000];
	int n;

	for (n = STEP; n <= MAX; n += STEP)
	{
		for (int m = 1; m <= TESTS; m++) {
			FillRandomArray(a, n, 0, 10000, false, order);
			for (int i = 0; i < n; i++)
			{
				b[i] = a[i];
				c[i] = a[i];
				d[i] = a[i];
			}
			bubbleSortIterativ(a, n);
			Operation BSTotalR = BSort.createOperation("BSTotalR", n);
			bubbleSortRecursiv(b, n, BSTotalR);
		}

	}
	for (n = STEP; n <= MAX; n += STEP)
	{
		BSort.startTimer("bubbleSortIterativ", n);
		for (int m = 1; m <= TESTS; m++)
		{
			bubbleSortIterativ(c, n);
		}
		BSort.stopTimer("bubbleSortIterativ", n);
	}
	
	
	for (n = STEP; n <= MAX; n += STEP)
	{
		BSort.startTimer("bubbleSortRecursiv;", n);
		for (int m = 1; m <= TESTS; m++) {
			Operation BSTotalR1 = BSort.createOperation("BSTotalR", n);
			bubbleSortRecursiv(d, n, BSTotalR1);
		}
		BSort.stopTimer("bubbleSortRecursiv;", n);
	}

	BSort.divideValues("BSTotalR", TESTS);
	BSort.divideValues("BSTotalI", TESTS);
}

void test_BS()
{
	testBS(UNSORTED);

	BSort.reset("BEST");
	testBS(ASCENDING);

	BSort.reset("Worst");
	testBS(DESCENDING);

	BSort.showReport();
}

int main()
{
	demo();
	puts("");
	//demoBC();
	//testBC();
	demoBS();
	//test_hSqS();
	//test_BS();
	return 0;
}