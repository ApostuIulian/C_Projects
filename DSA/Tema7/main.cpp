#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"
Profiler OS("Total");
typedef struct _node
{
	int key;
	_node* left;
	_node* right;
	_node* parent;
	int size;
}Node;

Node* OS_Select(Node* r, int s, Operation STotal)
{
	int g = 1;
	STotal.count();
	if (r->left == NULL)
	{
		g = 1;
	}
	else
	{
		STotal.count();
		g = r->left->size + 1;
	}
	STotal.count();
	if (s == g)
	{
		return r;
	}
	else if (s < g)
	{
		STotal.count();
		return OS_Select(r->left, s, STotal);
	}
	else if (s > g)
	{
		STotal.count();
		return OS_Select(r->right, s - g, STotal);
	}
	return NULL;
}

Node* succesor(Node* r, Operation DTotal)
{
	DTotal.count();
	while (r->left != NULL)
	{
		DTotal.count();
		r = r->left;
	}
	return r;
}

void OS_Delete(Node* r, int n, Operation DTotal)
{
	DTotal.count();
	Node* d = OS_Select(r, n, DTotal);
	DTotal.count();
	Node* p = d;
	DTotal.count();
	if (d == NULL)
	{
		return;
	}
	DTotal.count();
	if (d->left == NULL && d->right==NULL)
	{
		DTotal.count();
		if (d != r)
		{
			DTotal.count();
			if (d->parent->left == d)
			{
				DTotal.count();
				d->parent->left = NULL;
			}
			else
			{
				DTotal.count();
				d->parent->right = NULL;
			}
		}
		else
		{
			DTotal.count();
			r == NULL;
		}
	}
	else if(d->left != NULL && d->right != NULL)
	{
		DTotal.count();
		Node* suc = succesor(d->right, DTotal);
		DTotal.count();
		int dK = suc->key;
		DTotal.count();
		OS_Delete(r, suc->key, DTotal);
		DTotal.count();
		d->key = dK;
	}
	else
	{
		DTotal.count();
		if (d->parent->left == d)
		{
			DTotal.count();
			if (d->right == NULL)
			{
				DTotal.count();
				d->parent->left = d->left;
			}
			else
			{
				DTotal.count();
				d->parent->left = d->right;
			}
		}else if (d->parent->right == d)
		{
			DTotal.count();
			DTotal.count();
			if (d->right == NULL)
			{
				DTotal.count();
				d->parent->right = d->left;
			}
			else
			{
				DTotal.count();
				d->parent->right = d->right;
			}
		}
		DTotal.count();
		while (p->parent != r)
		{
			DTotal.count(2);
			p->parent->size--;
			p = p->parent;
		}
		DTotal.count();
		r->size--;
		free(d);
	}
}

Node* BuildTree(int a, int b, Operation BTotal)
{
	if (a <= b)
	{
		Node* root = (Node*)malloc(sizeof(Node));
		int m = (b + a) / 2;
		BTotal.count(4);
		root->key = m;
		root->size = 1;
		root->left = BuildTree(a, root->key - 1, BTotal);
		root->right = BuildTree(root->key + 1, b, BTotal);
		BTotal.count();
		if (root->left != NULL)
		{
			BTotal.count(2);
			root->left->parent = root;
			root->size += root->left->size;
		}
		BTotal.count();
		if (root->right != NULL)
		{
			BTotal.count(2);
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

void print(Node* r)
{
	if (r != NULL)
	{
		print(r->left);
		printf("%d size: %d\n", r->key, r->size);
		print(r->right);
	}
}

void testS(int i)
{
	Operation STotal = OS.createOperation("SelectTotal", i);
	Operation BTotal = OS.createOperation("BuildTotal", i);
	Operation DTotal = OS.createOperation("DeleteTotal", i);
	Node* r = BuildTree(1, i, BTotal);
	OS_Select(r, rand() % (i + 1 - 1) + 1, STotal);
	OS_Delete(r, rand() % (i + 1 - 1) + 1, DTotal);
}


void testSelect()
{
	for (int i = 100; i <= 10000; i+=100)
	{
		testS(i);
	}
	OS.showReport();
}

int main()
{
	Operation STotal = OS.createOperation("SelectTotal", 0);
	Node* r = BuildTree(1, 11, STotal);
	print(r);
	OS_Delete(r, 4, STotal);
	printf("After delete:\n");
	print(r);
	testSelect();
	//testDelete();
	printf("%d ", OS_Select(r, 4, STotal)->key);
	return 0;
}