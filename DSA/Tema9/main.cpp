#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"
Profiler QS("Total");
Profiler In("Total");
typedef struct _node
{
	int key;
	_node* left;
	_node* right;
	_node* parent;
	int size;
}Node;

Node* BuildTree(int a, int b)
{
	if (a <= b)
	{
		Node* root = (Node*)malloc(sizeof(Node));
		int m = (b + a) / 2;
		root->key = m;
		root->size = 1;
		root->left = BuildTree(a, root->key - 1);
		root->right = BuildTree(root->key + 1, b);
		if (root->left != NULL)
		{
			root->size += root->left->size;
		}
		if (root->right != NULL)
		{
			root->right->parent = root;
			root->size += root->right->size;
		}
		return root;
	}
	else
	{
		return NULL;
	}
}

void inorderRec(Node* r, Operation IRTotal)
{
	IRTotal.count();
	if (r != NULL)
	{
		inorderRec(r->left, IRTotal);
		//printf("%d\n", r->key);
		//IRTotal.count();
		inorderRec(r->right, IRTotal);
	}
}

void inorderRecDemo(Node* r)
{
	if (r != NULL)
	{
		inorderRecDemo(r->left);
		printf("%d\n", r->key);
		inorderRecDemo(r->right);
	}
}

void inorderIter(Node* r, Operation IITotal)
{
	Node* curr, * predecesor;
	IITotal.count();
	if (r == NULL)
	{
		return;
	}
	IITotal.count();
	curr = r;
	IITotal.count();
	while (curr)
	{
		IITotal.count();
		IITotal.count();
		if (curr->left == NULL)//daca nu are copil de stanga
		{
			//printf("%d ", curr->key);//il vom afisa
			IITotal.count();
			curr = curr->right;
		}
		else
		{
			IITotal.count();
			predecesor = curr->left;
			IITotal.count();
			while (predecesor->right != NULL && predecesor->right != curr)
			{
				IITotal.count();
				IITotal.count();
				predecesor = predecesor->right;
			}
			IITotal.count();
			if (predecesor->right == NULL)
			{
				IITotal.count(2);
				predecesor->right = curr;
				curr = curr->left;
			}
			else
			{
				IITotal.count(2);
				predecesor->right = NULL;
				//printf("%d ", curr->key);
				curr = curr->right;
			}
		}
	}
}

void inorderIterDemo(Node* r)
{
	Node* curr, * predecesor;
	if (r == NULL)
	{
		return;
	}
	curr = r;
	while (curr)
	{
		if (curr->left == NULL)//daca nu are copil de stanga
		{
			printf("%d ", curr->key);//il vom afisa
			curr = curr->right;
		}
		else
		{
			predecesor = curr->left;
			while (predecesor->right != NULL && predecesor->right != curr)
			{
				predecesor = predecesor->right;
			}
			if (predecesor->right == NULL)
			{
				predecesor->right = curr;
				curr = curr->left;
			}
			else
			{
				predecesor->right = NULL;
				printf("%d ", curr->key);
				curr = curr->right;
			}
		}
	}
}


void swap(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

void insertionSort(int* a, int l, int h, Operation QSTotal)
{
	for (int i = l + 1; i <= h; i++)
	{
		QSTotal.count();
		int val = a[i];
		int j = i;
		while (j > l && a[j - 1] > val)
		{
			QSTotal.count();
			a[j] = a[j - 1];
			j -= 1;
		}
		QSTotal.count();
		a[j] = val;
	}
}

int partition(int*a, int p, int r, Operation QSTotal)
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

void hybridQS(int* a, int l, int h, Operation QSTotal, int prag)
{
	while (l < h)
	{
		if (h - l < prag)
		{
			insertionSort(a, l, h, QSTotal);
			break;
		}
		else
		{
			int pivot = partition(a, l, h, QSTotal);
			if (pivot - l < h - pivot)
			{
				hybridQS(a, l, pivot-1, QSTotal, prag);
				l = pivot + 1;
			}
			else
			{
				hybridQS(a, pivot + 1, h, QSTotal, prag);
					h = pivot - 1;
			}
		}
    }
}


void testQS(int i)
{
	Operation QSTotal = QS.createOperation("QSTotal", i);
	int a[10000];
	FillRandomArray(a, i, 0, 50000, false, UNSORTED);
	hybridQS(a, 0, i-1, QSTotal, 10);
}

void testSelect()
{
	for (int i = 100; i <= 10000; i += 100)
	{
		for (int m = 1; m <= 1000; m += 100)
		{
			testQS(i);
		}
	}
	for (int i = 100; i <= 10000; i += 100)
	{
		QS.startTimer("QS", i);
		for (int m = 1; m <= 1000; m += 100)
		{
			testQS(i);
		}
	    QS.stopTimer("QS", i);
	}
	QS.showReport();
}

void test(int i)
{
	Operation IRTotal = In.createOperation("IRTotal", i);
	Operation IITotal = In.createOperation("IITotal", i);
	Node* r = BuildTree(1, i);
	inorderRec(r, IRTotal);
	inorderIter(r, IITotal);
}

void testIn()
{
	for (int i = 100; i <= 10000; i += 100)
	{
		test(i);
	}
	In.showReport();
}

void testQSPrag(int i)
{
	Operation QSPTotal = QS.createOperation("QSPTotal", i);
	int a[10000];
	FillRandomArray(a, 10000, 0, 50000, false, UNSORTED);
	hybridQS(a, 0, 9999, QSPTotal, i);
}

void testQP()
{
	for (int i = 1; i <= 1000; i++)
	{
			testQSPrag(i);
	}
	QS.showReport();
}
void demoQSH()
{
	int a[] = { 5, 23, 7, 0, 4, 3, 46, 123, 123, 1 };
	int n = sizeof(a) / sizeof(int);
	Operation QST = QS.createOperation("QS", 0);
	hybridQS(a, 0, n - 1, QST, 10);
	for (int i = 0; i < n; i++)
	{
		printf("%d ", a[i]);
	}
}
int main()
{
	Node* r = BuildTree(1, 11);
	inorderRecDemo(r);
	inorderIterDemo(r);
	puts("");
	demoQSH();
	//testSelect();
	//testIn();
	//testQP();
	return 0;
}