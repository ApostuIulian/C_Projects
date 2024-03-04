#include <stdio.h>
#include <stdlib.h>

int r1[] = { 2, 7, 5, 2, 7, 7, -1, 5, 2 };
int n = sizeof(r1) / sizeof(int);

void pozR(int* poz)
{
	for (int i = 0; i < n; i++)
	{
		if (r1[i] == -1)
		{
			*poz = i;
		}
	}
}

typedef struct _node {
	int key;
	int count;
	_node** child;
}Node;

typedef struct _nodeB {
	int key;
	_nodeB* left;
	_nodeB* bro;
}NodeB;


Node* R1ToR2()
{
	Node* T = (Node*)malloc(n * sizeof(Node));
	for (int i = 0; i < n; i++)
	{
		T[i].count = 0;
		T[i].key = i + 1;
	}
	for (int i = 0; i < n; i++)
	{
		T[i].child = (Node**)malloc(sizeof(Node*));
	}
	for (int i = 0; i < n; i++)
	{
		if (r1[i] != -1)
		{
			T[r1[i] - 1].child[T[r1[i] - 1].count] = (Node*)malloc(sizeof(Node));
			T[r1[i] - 1].child[T[r1[i] - 1].count] = &T[i];
			T[r1[i] - 1].count++;
		}
	}
	return T;
}

void pPmW(Node* T, int n)
{
	for (int i = 0; i < n; i++)
		printf(" ");
	printf("%d  ", T->key);
	puts(" ");
	for (int i = 0; i < T->count; i++)
		pPmW(T->child[i], n);
}

NodeB* demoB(Node* T)
{
	NodeB* B = (NodeB*)malloc(n * n * sizeof(NodeB));
	int p = 0;
	int c = 0;
	for (int i = 0; i < n; i++)
	{
		p = 0;
		for (int j = 0; j < T[i].count; j++)
		{
			if (T[i].child[j]->key != NULL && p == 0)
			{
				B[c].key = T[i].key;
				c++;
				p++;
			}
			B[c].key = T[i].child[j]->key;
			c++;
		}
	}
	for (int i = 0; i < c; i++)
	{
		B[i].bro = NULL;
		B[i].left = NULL;
	}
	for (int i = 0; i < c; i += T[B[i].key - 1].count + 1)
	{
		B[i].left = &B[i + 1];
		for (int j = i + 1; j < i + T[B[i].key - 1].count; j++)
		{
			B[j].bro = &B[j + 1];
		}
	}
	for (int i = 0; i < c; i++)
	{
		for (int j = i + 1; j < c; j++)
		{
			if (B[i].key == B[j].key)
			{
				if (B[i].left == NULL) B[i].left = B[j].left;
				if (B[i].bro == NULL) B[i].bro = B[j].bro;
				if (B[j].left == NULL) B[j].left = B[i].left;
				if (B[j].bro == NULL) B[j].bro = B[i].bro;
			}
		}
	}
	int rad;
	for (int i = 0; i < n; i++)
	{
		if (r1[i] == -1)
		{
			rad = i + 1;
			break;
		}
	}
	return &B[rad];
}

void preorder(NodeB* x, int level) {
	if (x != NULL) {
		printf("\n\n");
		for (int i = 0; i < level; i++)
		{
			printf("  ");
		}
		printf("%d ", x->key);
		preorder(x->left, level + 1);
		preorder(x->bro, level + 1);
	}
}

int main()
{
	int radP;
	pozR(&radP);
	R1ToR2();
	printf("pp binar: ");
	preorder(demoB(R1ToR2()), 0);
	puts("");
	printf("pp multiw:\n");
	pPmW(&R1ToR2()[radP], n);
	return 0;
}