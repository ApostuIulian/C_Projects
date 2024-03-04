#include<stdio.h>
#include<stdlib.h>
typedef struct _node
{
	int val;
	int rank;
	_node *parent;
}Node;

Node* Make_Set(int x)
{
	Node* s = (Node*)malloc(sizeof(Node));
	s->val = x;
	s->rank = 0;
	s->parent = s;
	return s;
}
Node* Find_Set(Node* s)
{
	if (s != s->parent)
	{
		s->parent = Find_Set(s->parent);
	}
	return s->parent;
}
void Union(Node* s1, Node* s2)
{
	Node* rs1 = Find_Set(s1);
	Node* rs2 = Find_Set(s2);
	if (rs1 != rs2)
	{
		if (rs1->rank < rs2->rank)
		{
			rs1->parent = rs2;
		}
		else if (rs1->rank > rs2->rank)
		{
			rs2->parent = rs1;
		}
		else
		{
			rs2->parent = rs1;
			rs1->rank++;
		}
	}
}
int main()
{

	return 0;
}