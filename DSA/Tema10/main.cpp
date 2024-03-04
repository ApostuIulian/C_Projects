#include<stdio.h>
#include<stdlib.h>
#include "Profiler.h";

Profiler Kr("Total");

typedef struct _node
{
	int val;
	int rank;
	_node* parent;
}Node;

typedef struct edge
{
	Node *u;
	Node *v;
	int w;
}Edge;

Node* Make_Set(int x)
{
	Node* s = (Node*)malloc(sizeof(Node));
	s->val = x;
	s->rank = 0;
	s->parent = s;
	return s;
}
Node* Find_Set(Node* s, Operation KRTotal)
{
	KRTotal.count();
	if (s != s->parent)
	{
		KRTotal.count();
		s->parent = Find_Set(s->parent, KRTotal);
	}
	return s->parent;
}
void Union(Node* s1, Node* s2, Operation KRTotal)
{
	Node* rs1 = Find_Set(s1, KRTotal);
	Node* rs2 = Find_Set(s2, KRTotal);
	KRTotal.count();
	if (rs1 != rs2)
	{
		KRTotal.count();
		if (rs1->rank < rs2->rank)
		{
			KRTotal.count();
			rs1->parent = rs2;
		}
		else if (rs1->rank > rs2->rank)
		{
			KRTotal.count(2);
			rs2->parent = rs1;
		}
		else
		{
			KRTotal.count();
			rs2->parent = rs1;
			rs1->rank++;
		}
	}
}

int comparaEdges(const void* a, const void* b) {
	const Edge* x = (const Edge*)a;
	const Edge* y = (const Edge*)b;
	return x->w - y->w;
}

void comparaMuchii(Edge* v, int n, Operation KRTOtal)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			if (v[i].w > v[j].w)
			{
				Edge temp = v[i];
				v[i] = v[j];
				v[j] = temp;
			}
		}
	}
}
void kruskal(Edge* v, int n, Operation KRTotal)
{
	Node* v1=NULL;
	Node* v2=NULL;
	//comparaMuchii(v, n);
	qsort(v, n, sizeof(Edge), comparaEdges);
	int mst = 0;
	for (int i = 0; i < n; i++)
	{
		KRTotal.count();
		if (Find_Set(v[i].u, KRTotal) != Find_Set(v[i].v, KRTotal))
		{
			mst = mst + v[i].w;
			KRTotal.count();
			Union(Find_Set(v[i].u, KRTotal), Find_Set(v[i].v, KRTotal), KRTotal);
			printf("%d - %d = %d\n", v[i].u->val, v[i].v->val, v[i].w);
		}
	}
	printf("mst: %d\n", mst);
}

void demo()
{
	Node* nod[4];
	for (int i = 0; i < 7; i++) {
		nod[i] = Make_Set(i+1);
	}
	int n = 7;
	Edge* v = (Edge*)malloc(n * sizeof(Edge));
	v[0].u = nod[0];
	v[0].v = nod[1];
	v[0].w = 1;
	v[1].u = nod[0];
	v[1].v = nod[2];
	v[1].w = 7;
	v[2].u = nod[0];
	v[2].v = nod[3];
	v[2].w = 10;
	v[3].u = nod[0];
	v[3].v = nod[4];
	v[3].w = 5;
	v[4].u = nod[1];
	v[4].v = nod[2];
	v[4].w = 3;
	v[5].u = nod[2];
	v[5].v = nod[3];
	v[5].w = 4;
	v[6].u = nod[3];
	v[6].v = nod[4];
	v[6].w = 2;

	Operation KRTotal = Kr.createOperation("KRTotal", 0);
	kruskal(v, n, KRTotal);
}

void test(int i)
{
	Operation KRTotal = Kr.createOperation("KRTotal", i);
	Node* nod[10000];
	for (int j = 0; j < i; j++)
	{
		nod[j] = Make_Set(j);
	}
	Edge* v = (Edge*)malloc(i * sizeof(Edge));
	for (int j = 0; j < i-1; j++)
	{
			v[j].u = nod[j+1];
			v[j].v = nod[j+2];
		v[j].w= (rand() % (50 - 1 + 1)) + 1;
	}
	kruskal(v, i, KRTotal);
}

void testKr()
{
	for (int i = 100; i <= 10000; i += 100)
	{
		test(i);
	}
	Kr.showReport();
}
int main()
{
//	testKr();
  demo();
}