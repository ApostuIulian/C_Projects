#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"

Profiler Sortari("Sort");

void bubbleSort(int* a, int n)
{
    Operation BSComp = Sortari.createOperation("BSComp", n);
    Operation BSAtr = Sortari.createOperation("BSAtr", n);
    int sorted;
    for (int i = 0; i < n - 1; i++)
    {
        sorted = 1;
            for (int j = 0; j < n - 1 - i; j++)
            {
                BSComp.count();
                if (a[j] > a[j + 1])
                {
                    int temp = a[j];
                    a[j] = a[j + 1];
                    a[j + 1] = temp;
                    BSAtr.count(3);
                    sorted = 0;
                }
            }
            if (sorted)
            {
                break;
            }
        }
}

void selectionSort(int a[], int n)
{
    Operation SSComp = Sortari.createOperation("SSComp", n);
    Operation SSAtr = Sortari.createOperation("SSAtr", n);
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

void insertionSort(int a[],  int n)
{
    Operation ISComp = Sortari.createOperation("ISComp", n);
    Operation ISAtr = Sortari.createOperation("ISAtr", n);
    int k = 0, i;
    for (int j = 1; j < n; j++)
    {
        ISAtr.count();
        k = a[j];
        i = j - 1;
        ISComp.count();
        while (i >= 0 && a[i] > k)
        {
            ISComp.count();
            ISAtr.count();
            a[i + 1] = a[i];
            i = i - 1;
        }
        ISAtr.count();
        a[i + 1] = k;
    }
}
void demoBS()
{
    int a[] = { 2, 5, 1, 0, 7, 4, 8, 2, 5 };
    int n = sizeof(a) / sizeof(int);
    bubbleSort(a, n);
    for (int i = 0; i < n; i++)
    {
        printf("%d ", a[i]);
    }
}

void demoSS()
{
    int a[] = { 9, 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 4, 8, 1, 12, 6, 0 };
    int n = sizeof(a) / sizeof(int);
    selectionSort(a, n);
    for (int i = 0; i < n; i++)
    {
        printf("%d ", a[i]);
    }
}

void demoIS()
{
    int a[] = { 9, 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 4, 8, 1, 12, 6, 0 };
    int n = sizeof(a) / sizeof(int);
    insertionSort(a, n);
    for (int i = 0; i < n; i++)
    {
        printf("%d ", a[i]);
    }
}

void perfBS(int order)
{
    int a[10000];
    for (int n = 100; n <= 1000; n += 100)
    {
        FillRandomArray(a, n, order);
        bubbleSort(a, n);
    }
    Sortari.addSeries("totalBS", "BSComp", "BSAtr");
}

void perfSS(int order)
{
    int a[10000];
    for (int n = 100; n <= 1000; n += 100)
    {
        FillRandomArray(a, n, 10, 50000, false, order);
        selectionSort(a, n);
    }
    Sortari.addSeries("totalSS", "SSComp", "SSAtr");
}

void perfIS(int order)
{
    int a[10000];
    for (int n = 100; n <= 1000; n += 100)
    {
        FillRandomArray(a, n);
        insertionSort(a, n);
    }
    Sortari.addSeries("totalIS", "ISComp", "ISAtr");
}

void perfTotal()
{
    perfBS(UNSORTED);
    perfSS(UNSORTED);
    perfIS(UNSORTED);
    Sortari.createGroup("Comp", "BSComp", "SSComp", "ISComp");
    Sortari.createGroup("Atr", "BSAtr", "SSAtr", "ISAtr");
    Sortari.createGroup("Total", "totalBS", "totalSS", "totalIS");
    Sortari.showReport();
}

int main()
{
	perfTotal();
    //demoIS();
	return 0;
}