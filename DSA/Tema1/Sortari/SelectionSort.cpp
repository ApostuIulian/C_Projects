#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h";

Profiler SS("Selection Sort");

void selectionSort(int a[], int n)
{
	Operation Comp = SS.createOperation("SSComp", n);
	Operation Atr = SS.createOperation("SSAtr", n);
	int idMin;
	for (int i = 0; i < n; i++)
	{
		idMin = i;
		for (int j = i + 1; j < n; j++)
		{
			Comp.count();
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
			Atr.count(3);
		}
	}
}
void perf()
{
	int a[10000];
	for (int n = 100; n <= 1000; n += 100)
	{
		FillRandomArray(a, n);
		selectionSort(a, n);
	}
	SS.addSeries("total", "SSComp", "SSAtr");
	SS.showReport();
}
void demo()
{
	int a[] = { 9, 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 4, 8, 1, 12, 6, 0};
	int n = sizeof(a) / sizeof(int);
	selectionSort(a, n);
	for(int i=0;i<n;i++)
	{
		printf("%d ", a[i]);
	}
}
int main()
{
	//demo();
	perf();
}
