#include <stdio.h>
#include <stdlib.h>
#define MAX 100
#define n1 5

typedef struct _nod
{
	int val;
	struct _nod* next;
}Node;

typedef struct
{
	int n;
	Node** l;
	int* parinti;
	int* d;
	int* f;
	int* culoare;
}Graph;

enum {
	WHITE, GRAY, BLACK
};

void insertFirst(Node** head, int key)
{
	Node* p = (Node*)malloc(sizeof(Node));
	p->val = key;
	p->next = *head;
	*head = p;
}

int index;
int nrComponente;
int stack[MAX];
int top;
bool onStack[MAX];
int componenta[MAX];

void push(int v)
{
	stack[top++] = v;
	onStack[v] = true;
}

int pop()
{
	int v = stack[--top];
	onStack[v] = false;
	return v;
}

int min(int a, int b)
{
	return  a < b ? a : b;
}

void dfsVisit(Graph* G, int v, int* timp, int level, int* list, int* id)
{
	(*timp)++;
	G->d[v] = *timp;
	G->culoare[v] = GRAY;
	for (int i = 0; i < level; i++)
	{
		printf(" ");
	}
	printf("%d\n", v);
	Node* a = G->l[v];
	while (a != NULL)
	{
		int k = a->val;
		if (G->culoare[k] == WHITE)
		{
			G->parinti[k] = v;
			dfsVisit(G, k, timp, level + 1, list, id);
		}
		a = a->next;
	}
	G->culoare[v] = BLACK;
	list[*id] = v;
	(*id)--;
	(*timp)++;
	G->f[v] = *timp;
}

void dfs(Graph* G, int* list, int* id)
{
	for (int i = 0; i < G->n; i++)
	{
		G->culoare[i] = WHITE;
		G->parinti[i] = NULL;
	}
	int timp = 0;
	for (int i = 0; i < G->n; i++)
	{
		if (G->culoare[i] == WHITE)
		{
			dfsVisit(G, i, &timp, 0, list, id);
		}
	}
}

void topological(Graph G)
{
	int topological[n1];
	int id = 2;
	dfs(&G, topological, &id);
	for (int i = 0; i < G.n; i++)
	{
		printf("%d ", topological[i]);
	}
}

void SCC(Graph G, int id)
{
	G.d[id] = G.f[id] = index;
	index++;
	push(id);
	Node* a = G.l[id];
	while (a != NULL)
	{
		int v = a->val;
		if (G.d[v] == -1)
		{
			G.parinti[v] = id;
			SCC(G, v);
			G.f[id] = min(G.f[id], G.f[v]);
		}
		else if (onStack[v])
		{
			G.f[id] = min(G.f[id], G.d[v]);
		}
		a = a->next;
	}
	if (G.d[id] == G.f[id])
	{
		nrComponente++;
		int v;
		do {
			v = pop();
			componenta[v] = nrComponente;
		} while (id != v);
	}
}

void Tarjan(Graph G)
{
	index = 0;
	nrComponente = 0;
	for (int i = 0;i < G.n; i++)
	{
		G.d[i] = G.f[i] = -1;
		onStack[i] = false;
		componenta[i] = 0;
	}
	for (int i = 0; i < G.n; i++)
	{
		if (G.d[i] == -1)
		{
			SCC(G, i);
		}
	}
	for (int i = 1; i <= nrComponente; i++)
	{
		printf("Componenta %d: {", i);
		for (int j = 0; j < G.n; j++)
		{
			if (componenta[j] == i)
			{
				printf("%d ", j);
			}
		}
		printf("}\n");
	}
}

void demoSCC()
{
	Graph G;
	G.n = n1;
	G.l = (Node**)malloc(G.n*sizeof(Node*));
	for (int i = 0; i < G.n; i++)
	{
		G.l[i] = NULL;
	}
	G.parinti = (int*)malloc(G.n * sizeof(int));
	G.d = (int*)malloc(G.n * sizeof(int));
	G.f = (int*)malloc(G.n * sizeof(int));
	G.culoare = (int*)malloc(G.n * sizeof(int));
	//0->1
	Node* v = (Node*)malloc(sizeof(Node));
	v->val = 1;
	v->next = G.l[0];
	G.l[0] = v;
	//0->2
    v = (Node*)malloc(sizeof(Node));
	v->val = 2;
	v->next = G.l[0];
	G.l[0] = v;
	//1->3
    v = (Node*)malloc(sizeof(Node));
	v->val = 3;
	v->next = G.l[1];
	G.l[1] = v;
	//2->4
	v = (Node*)malloc(sizeof(Node));
	v->val = 4;
	v->next = G.l[2];
	G.l[2] = v;
	//4->0
	v = (Node*)malloc(sizeof(Node));
	v->val = 0;
	v->next = G.l[4];
	G.l[4] = v;
	Tarjan(G);
}

void demoTopological()
{
	Graph G;
	G.n = n1;
	G.l = (Node**)malloc(G.n * sizeof(Node*));
	for (int i = 0; i < G.n; i++)
	{
		G.l[i] = NULL;
	}
	G.parinti = (int*)malloc(G.n * sizeof(int));
	G.d = (int*)malloc(G.n * sizeof(int));
	G.f = (int*)malloc(G.n * sizeof(int));
	G.culoare = (int*)malloc(G.n * sizeof(int));
	//0->1
	Node* v = (Node*)malloc(sizeof(Node));
	v->val = 1;
	v->next = G.l[0];
	G.l[0] = v;
	//0->2
	v = (Node*)malloc(sizeof(Node));
	v->val = 2;
	v->next = G.l[0];
	G.l[0] = v;
	//1->3
	v = (Node*)malloc(sizeof(Node));
	v->val = 3;
	v->next = G.l[1];
	G.l[1] = v;
	//2->4
	v = (Node*)malloc(sizeof(Node));
	v->val = 4;
	v->next = G.l[2];
	G.l[2] = v;
	//4->0
	v = (Node*)malloc(sizeof(Node));
	v->val = 0;
	v->next = G.l[4];
	G.l[4] = v;
	topological(G);
}

int main()
{
	demoTopological();
	puts("");
	demoSCC();
	return 0;
}