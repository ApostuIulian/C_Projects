#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"
#define STEP 100
#define MAX 10000
#define STEP 100
#define MAX 10000
int k = 7;
Profiler kL("kListe");

int n;
void nRand(int* n)
{
	srand(time(0));
	(*n) = rand() % (20 - k + 1) + k;
}

typedef struct _Node
{
	int key;
	_Node* next;
}Node;

void init(Node** first)
{
	*first = NULL;
}

Node* newNode(int key)
{
	Node* q = (Node*)malloc(sizeof(Node));
	q->key = key;
	q->next = NULL;
	return q;
}

void insertLast(Node** first, Node** last, int key)
{
	Node* q = newNode(key);
	if (*last == NULL)
	{
		*first = *last = q;
	}
	else
	{
		(*last)->next = q;
		*last = q;
	}
}

void print(Node* first)
{
	for (Node* a = first; a != NULL; a = a->next)
	{
		printf("%d ", a->key);
	}
}

void printAll(Node* a[])
{
	for (int i = 0; i < k; i++)
	{
		print(a[i]);
		puts("");
	}
}

void fillK(Node* aL[])
{
	int a[10000];
	int elem = n;
	int rest = 0;
	for (int j = 0; j < k; j++)
	{
		if (elem % k != 0)
		{
			rest = elem % k;
		}
		elem -= elem % k;
		FillRandomArray(a, elem / k + rest, 0, 10000, false, ASCENDING);
		Node* first;
		Node* last;
		init(&first);
		init(&last);
		for (int i = 0; i < elem / k + rest; i++)
		{
			insertLast(&first, &last, a[i]);
		}
		rest = 0;
		aL[j] = first;
	}
}

void fill2(Node* aL[])
{
	int a[10000];
	int elem = n, rest = 0;
	for (int j = 0; j < 2; j++)
	{
		if (elem % 2 != 0)
		{
			rest = elem % 2;
		}
		elem -= elem % 2;
		FillRandomArray(a, elem / 2 + rest, 0, 10000, false, ASCENDING);
		Node* first;
		Node* last;
		init(&first);
		init(&last);
		for (int i = 0; i < elem / 2 + rest; i++)
		{
			insertLast(&first, &last, a[i]);
		}
		rest = 0;
		aL[j] = first;
	}
}

Node* interclasare(Node* a, Node* a1)
{
	Operation kLTotal = kL.createOperation("kLTotal", n);
	Node* b = newNode(-1);
	Node* c = b;

	while (a != NULL && a1 != NULL)
	{
		kLTotal.count();
		if (a->key < a1->key)
		{
			kLTotal.count(2);
			b->next = a;
			a = a->next;
		}
		else
		{
			kLTotal.count(2);
			b->next = a1;
			a1 = a1->next;
		}
		kLTotal.count();
		b = b->next;
	}
	kLTotal.count();
	if (a != NULL)
	{
		kLTotal.count();
		b->next = a;
	}
	kLTotal.count();
	if (a1 != NULL)
	{
		kLTotal.count();
		b->next = a1;
	}
	return c->next; // sa nu afiseza si -1
}

Node* interclasareK(Node* a[])
{
	Node* res;
	init(&res);
	res = interclasare(a[0], a[1]);
	for (int i = 2; i < k; i++)
	{
		res = interclasare(res, a[i]);
	}
	return res;
}

void Heapify(int* H, int i, int m, Operation HTotal)
{
	int largest = i;
	int l = 2 * i + 1;
	int r = 2 * i + 2;
	HTotal.count();
	if (l < m && H[l] <= H[largest])
	{
		largest = l;
	}
	HTotal.count();
	if (r < m && H[r] <= H[largest])
	{
		largest = r;
	}
	if (largest != i)
	{
		HTotal.count(3);
		int temp = H[i];
		H[i] = H[largest];
		H[largest] = temp;
		Heapify(H, largest, m, HTotal);
	}
}

void BottomUp(int* H, int m, Operation HTotal)
{
	for (int i = m / 2 - 1; i >= 0; i--)
	{
		Heapify(H, i, m, HTotal);
	}
}

Node* sortKheap(Node* a[])
{
	Operation HTotal = kL.createOperation("HTotal", n);
	Node* first;
	init(&first);
	Node* last;
	init(&last);
	int v[10000];
	int minId;
	int id = 0;
	while (id < n)
	{
		for (int i = 0; i < k; i++)
		{
			HTotal.count();
			v[i] = a[i]->key;
		}
		BottomUp(v, k, HTotal);
		for (int i = 0; i < k; i++)
		{
			HTotal.count();
			if (v[0] == a[i]->key)
			{
				minId = i;
				break;
			}
		}
		HTotal.count();
		if (a[minId]->next != NULL)
		{
			HTotal.count();
			a[minId] = a[minId]->next;
		}
		else
		{
			HTotal.count();
			a[minId]->key = 2000000;
		}
		id++;
		insertLast(&first, &last, v[0]);
	}
	return first;
}
void demoHeap()
{
	Node* a[500];
	Node* b;
	init(&b);
	fillK(a);
	printAll(a);
	print(sortKheap(a));
}

void demoI2()
{
	Node* a[2];
	fill2(a);
	print(a[0]);
	puts("");
	print(a[1]);
	puts("");
	Node* res = interclasare(a[0], a[1]);
	print(res);
}

void demoIK()
{
	Node* a[500];
	fillK(a);
	printAll(a);
	Node* res = interclasareK(a);
	print(res);
}

void NtestIK()
{
	Node* aL[500];
	k = 50;
	for (n = STEP; n <= MAX; n += STEP)
	{
		fillK(aL);
		interclasareK(aL);
	}
	kL.showReport();
}

void fillK1(Node* aL[])
{
	int a[10000];
	int elem = n, rest = 0;
	for (int j = 0; j < k; j++)
	{
		if (elem % k != 0)
		{
			rest = elem % k;
		}
		elem -= elem % k;
		FillRandomArray(a, elem / k + rest, 0, 10000, false, ASCENDING);
		Node* first;
		Node* last;
		init(&first);
		init(&last);
		for (int i = 0; i < elem / k + rest; i++)
		{
			insertLast(&first, &last, a[i]);
		}
		rest = 0;
		aL[j] = first;
	}
}

Node* sortKheapK1(Node* a[])
{
	Operation HTotal = kL.createOperation("HTotal", k);
	Node* first;
	init(&first);
	Node* last;
	init(&last);
	int v[10000];
	int minId;
	int id = 0;
	while (id < n)
	{
		for (int i = 0; i < k; i++)
		{
			v[i] = a[i]->key;
		}
		BottomUp(v, k, HTotal);
		for (int i = 0; i < k; i++)
		{
			if (v[0] == a[i]->key)
			{
				minId = i;
				break;
			}
		}
		HTotal.count();
		if (a[minId]->next != NULL)
		{
			HTotal.count();
			a[minId] = a[minId]->next;
		}
		else
		{
			HTotal.count();
			a[minId]->key = 2000000;
		}
		id++;
		insertLast(&first, &last, v[0]);
	}
	return first;
}

void testHeap()
{
	Node* aL[500];
	k = 100;
	for (n = STEP; n <= MAX; n += STEP)
	{
		fillK(aL);
		sortKheap(aL);
	}
	kL.showReport();
}

void testHeapK()
{
	Node* aL[500];
	n = 10000;
	for (k = 10; k <= 500; k += 10)
	{
		fillK1(aL);
		sortKheapK1(aL);
	}
	kL.showReport();
}

int main()
{
	nRand(&n);
	printf("%d\n", n);
	demoI2();
	puts("");
	puts("");
	demoIK();
	puts("");
	puts("");
	demoHeap();
	puts("");
	puts("");
	//testHeap();
	//testHeapK();
	return 0;
}