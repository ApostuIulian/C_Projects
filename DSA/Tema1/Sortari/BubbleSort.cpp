#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h";

Profiler BS("Bubble Sort");

void bubbleSort(int* a, int n)
{
    Operation Comp = BS.createOperation("BBComp", n);
    Operation Atr = BS.createOperation("BBAtr", n);
    int sorted;
    for (int i = 0; i < n - 1; i++)
    {
        sorted = 1;
        {
            for (int j = 0; j < n - 1 - i; j++)
            {
                Comp.count();
                if (a[j] > a[j + 1])
                {
                    int temp = a[j];
                    a[j] = a[j + 1];
                    a[j + 1] = temp;
                    Atr.count(3);
                    sorted = 0;
                }
            }
            if (sorted)
            {
                break;
            }
        }
    }
}
void perf()
{
    int a[10000];
    for (int n = 100; n <= 1000; n += 100)
    {
        FillRandomArray(a, n);
        bubbleSort(a, n);
    }
    BS.addSeries("total", "BBComp", "BBAtr");
    BS.showReport();
}
void demo()
{
    int a[] = {2, 5, 1, 0, 7, 4, 8, 2, 5};
    int n = sizeof(a) / sizeof(int);
    bubbleSort(a, n);
    for (int i = 0; i < n; i++)
    {
        printf("%d ", a[i]);
    }
}
int main()
{
    //demo();
    perf();
    return 0;
}
