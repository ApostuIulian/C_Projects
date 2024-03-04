#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Profiler.h"
#define m 3000
#define n 10007
typedef struct _node {
	int id;
	char name[30];
	int ocupat;
}Node;

void print(Node* H)
{
	for (int i = 0; i < n; i++)
	{
		if (H[i].ocupat == 1)
		{
			printf("H[%d]: id= %d | nume= %s\n", i, H[i].id, H[i].name);
		}
		else
		{
			printf("H[%d]= FREE\n", i);
		}
	}
}

int hash(int key)
{
	return abs(key) % n;
}

int patratic(int key, int i)
{
	return (i * i + hash(key)) % n;
}

void insert(Node* H, int id1, const char nume[30])
{
	for (int i = 0; i < n; i++)
	{
		int poz = patratic(id1, i);
		if (H[poz].ocupat != 1) {
			H[poz].id = id1;
			strcpy(H[poz].name, nume);
			H[poz].ocupat = 1;
			break;
		}
	}
}

int searchIdName(Node* H, int id1, const char nume[30], float* efort)
{
	for (int i = 0; i < n; i++)
	{
		(*efort)++;
		int poz = patratic(id1, i);
		if (H[poz].id == id1 && strcmp(H[poz].name, nume) == 0 && H[poz].ocupat > 0)
		{
			return poz;
		}
	}
	return -1;
}

/*void deleteN(Node* H, int id1, const char nume[30], int n)
{
	int pozCurr = -1, idCurr, pozViit, idViit;
	if (searchIdName(H, id1, nume, n) >= 0)
	{
		pozCurr = searchIdName(H, id1, nume, n);
		H[pozCurr].ocupat = 0;
		pozViit = patratic(id1, n, pozCurr);
		int i = 0;
		do
		{
			pozViit = patratic(H[pozCurr].id, n, i);
			idViit = H[pozViit].id;
			H[pozCurr].id = idViit;
			strcpy(H[pozCurr].name, H[pozViit].name);
			H[pozCurr].ocupat = 1;
			pozCurr = pozViit;
			i++;
		} while (H[pozViit].ocupat != 0);
		H[pozCurr].ocupat = 0;
	}
	else
	{
		printf("Elementul de sters nu a putut fi gasit!\n");
		return;
	}
}
*/

void deleteN(Node* H, int id, const char nume[30])
{
	float efort = 0;
	if (searchIdName(H, id, nume, &efort) >= 0)
	{
		int poz = searchIdName(H, id, nume, &efort);
		H[poz].id = -1;
		H[poz].ocupat = 0;
	}
	else
	{
		return;
	}
}

void fillH(Node* H, int factor)
{
	int a[n];
	FillRandomArray(a, factor, 1, 50000, 1);
	for (int i = 0; i < factor; i++)
	{
		insert(H, a[i], "I");
	}
}

void searchM(Node* H)
{
	float efortMedGasite = 0;
	float efortMedNeGasite = 0;
	int a[m];
	FillRandomArray(a, m / 2, 50000, 100000, 1);
	for (int i = 0; i < m / 2; i++)
	{
		efortMedNeGasite = 0;
		searchIdName(H, a[i], "I", &efortMedNeGasite);
	}
	int i = 0;
	for(int i=0;i<m/2;i++)
	{
		int j = (rand() % (n - 0 + 1)) + 0;
		efortMedGasite = 0;
		searchIdName(H, H[j].id, "I", &efortMedGasite);
	}
	printf("Efort mediu gasite: %f, Efort mediu negasite: %f\n", efortMedGasite / (m / 2), efortMedNeGasite / (m / 2));
}

void deleteM(Node* H)
{
	int a[1000];
	FillRandomArray(a, 1000, 1, 10000);
	for (int i = 0; i < 1000; i++)
	{
		H[a[i]].id = -1;
		H[a[i]].ocupat = 0;
	}
}
void demoInsertEfort()
{
	Node* H = (Node*)malloc(n * sizeof(Node));
	insert(H, 1, "A");
	insert(H, 11, "A");
	insert(H, 21, "A");
	float efort = 0;
	searchIdName(H, 11, "A", &efort);
	deleteN(H, 1, "A");
	print(H);
	printf("%f ", efort);
}
int main()
{
	Node* H = (Node*)malloc(n * sizeof(Node));
	int a[] = { 80, 85, 90, 95, 99 };
	int factor;
	factor = n * a[0] / 100;
	fillH(H, factor);
	printf("factor= 0.%d: ", a[0]);
	searchM(H);
	factor = n * a[1] / 100;
	fillH(H, factor);
	printf("factor= 0.%d: ", a[1]);
	searchM(H);
	factor = n * a[2] / 100;
	fillH(H, factor);
	printf("factor= 0.%d: ", a[2]);
	searchM(H);
	factor = n * a[3] / 100;
	fillH(H, factor);
	printf("factor= 0.%d: ", a[3]);
	searchM(H);
	factor = n * a[4] / 100;
	fillH(H, factor);
	printf("factor= 0.%d: ", a[4]);
	searchM(H);
	puts("");
	//demoInsertEfort();
	return 0;
}
