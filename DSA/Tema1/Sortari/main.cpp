/*
Teste:
Am efectuat cate 5 teste pe vectori cu elemente de la 100 la 1000
pe cazurile best, average si worst;
In cazul best, vectorul este sortat crescator in timp ce in cazul worst acesta este sortat descrescator.
In cazul average am folosit functia FillRandomArray pentru a genera siruri cu elemente aleatoare

Complexitati:

Bubble Sort:
-Best:O(n)
-Average:O(n^2)
-Worst: O(n^2)

Selection Sort:
-Best/Average/Worst: O(n^2);

Insertion Sort:
-Best: O(n)
-Average: O(n^2)
-Worst: O(n^2)
*/
#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"
#define STEP 100
#define MAX 10000
#define TESTS 5

Profiler Sort("Average");

void bubbleSort(int* a, int n)
{
	Operation BSComp = Sort.createOperation("BSComp", n);
	Operation BSAtr = Sort.createOperation("BSAtr", n);
	int sortat = 0;
	for (int i = 0; i < n - 1; i++)
	{
		sortat = 1;
		for (int j = 0; j < n - 1 - i; j++)
		{
			BSComp.count();
			if (a[j] > a[j + 1])
			{
				int temp = a[j];
				a[j] = a[j + 1];
				a[j + 1] = temp;
				BSAtr.count(3);
				sortat = 0;
			}
		}
		if (sortat)
		{
			break;
		}
	}
}

void selectionSort(int* a, int n)
{
	Operation SSComp = Sort.createOperation("SSComp", n);
	Operation SSAtr = Sort.createOperation("SSAtr", n);
	int idMin;
	for (int i = 0; i < n; i++)
	{
		idMin = i;
		for (int j = i + 1; j < n; j++)
		{
			SSComp.count();
			if (a[j] < a[idMin])
			{
				idMin = j;
			}
		}
		if (idMin != i)
		{
			int temp = a[i];
			a[i] = a[idMin];
			a[idMin] = temp;
			SSAtr.count(3);
		}
	}
}

void insertionSort(int* a, int n)
{
	Operation ISComp = Sort.createOperation("ISComp", n);
	Operation ISAtr = Sort.createOperation("ISAtr", n);
	int k = 0, i = 0;
	for (int j = 1; j < n; j++)
	{
		k = a[j];
		ISAtr.count();
		i = j - 1;
		ISComp.count();
		while (i >= 0 && a[i] > k)
		{
			ISComp.count();
			a[i + 1] = a[i];
			ISAtr.count();
			i = i - 1;
		}
		a[i + 1] = k;
		ISAtr.count();
	}
}

void demo()
{
	int bS[] = { 1, 0, 3, 9, 0, 2, 23, 86, 998, 23, 666, 33, 101, 0, 1};
	int sS[] = { 1, 0, 3, 9, 0, 2, 23, 86, 998, 23, 666, 33, 101, 0, 1};
	int iS[] = { 1, 0, 3, 9, 0, 2, 23, 86, 998, 23, 666, 33, 101, 0, 1};
	int n = sizeof(bS) / sizeof(int);
	bubbleSort(bS, n);
	for (int i = 0; i < n; i++)
	{
		printf("%d ", bS[i]);
	}
	puts("");

	selectionSort(sS, n);
	for (int i = 0; i < n; i++)
	{
		printf("%d ", sS[i]);
	}
	puts("");
	insertionSort(iS, n);

	for (int i = 0; i < n; i++)
	{
		printf("%d ", iS[i]);
	}
}

void test(int order)
{
	int a[10000], b[10000], c[10000];
	for (int n = STEP; n <= MAX; n += STEP)
	{
		for (int test = 0; test < TESTS; test++)
		{
			FillRandomArray(a, n, 0, 10000, false, order);
			for (int i = 0; i < n; i++)
			{
				b[i] = a[i];
				c[i] = a[i];
			}
			bubbleSort(a, n);
			selectionSort(b, n);
			insertionSort(c, n);
		}
	}
	Sort.divideValues("BSComp", TESTS);
	Sort.divideValues("BSAtr", TESTS);
	Sort.addSeries("BSTotal", "BSComp", "BSAtr");

	Sort.divideValues("SSComp", TESTS);
	Sort.divideValues("SSAtr", TESTS);
	Sort.addSeries("SSTotal", "SSComp", "SSAtr");

	Sort.divideValues("ISComp", TESTS);
	Sort.divideValues("ISAtr", TESTS);
	Sort.addSeries("ISTotal", "ISComp", "ISAtr");

	Sort.createGroup("Atr", "BSAtr", "SSAtr", "ISAtr");
	Sort.createGroup("Comp", "BSComp", "SSComp", "ISComp");
	Sort.createGroup("Total", "BSTotal", "SSTotal", "ISTotal");
}

void test_all()
{
	test(UNSORTED);

	Sort.reset("Best");
	test(ASCENDING);

	Sort.reset("Worst");
	test(DESCENDING);

	Sort.showReport();
}

int main()
{
	//demo();
	test_all();
	return 0;
}