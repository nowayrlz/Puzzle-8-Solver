#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <locale.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define DOWN 0
#define UP 1
#define LEFT 2
#define RIGHT 3
#define H2

extern int notvalid1[4];
extern int notvalid2[4];
extern int notvalid3[4];
extern int applyparam[4];
extern int maxdepth;
extern char rep[4];




struct node
{
	unsigned char bit_var[5]; //Valid will be at bit_var[4]-last 4 bits or bit_var[0]-first 4 bits
	char pathcost;
	char totalcost;
	char* str;
	struct node* next;
};
typedef struct node Node;

void  SetBit(char A[], char k);//Seta um bit do veor de bits

void  ClearBit(char A[], char k);//Zera um bit do vetor de bits

int ReadBitVar(char* vector, char index);//Lê uma posição do vetor de bits

void Write_Vector(int* dest, char* source);//Transfere de um bit_char[5] para int[9]

//__inline void Write_Vector_inline(int* dest, char* source);//Transfere de um bit_char[5] para int[9]

void WriteToBitArray(char* vector, int index, int num);//Escreve um int em uma posição do vetor de bits

void StoreVector(char* dest, int* source);//Transfere de um vetor int[9] para um vetor bit_char[5]

void print_state(int* game_state);

int heur(int* block);

int check_solve(int* block);

void prepend(Node** Raiz, Node* new_node, Node* old_node, int op);

int goal(Node* no, int* g_block);

int notonqueue(Node* Raiz, Node* newnode);

/*inline Node* bestnodefromqueue(Node* Raiz)
{
	Node* t = Raiz;
	int min_totalpathcost = 1000;
	//int totalpathcost;
	Node* to_return = NULL;
	//int valid;
	while (t != NULL)
	{

		if (ReadBitVar(t->bit_var, 9) == 1 && t->totalcost < min_totalpathcost)
		{
			min_totalpathcost = t->totalcost;
			to_return = t;
		}
		t = t->next;
	}

	if (to_return != NULL) WriteToBitArray(to_return->bit_var, 9, 0);


	return to_return;
}*/

Node* bestnodefromqueue(Node* Raiz);

char apply(Node* newnode, Node* oldnode, int op);

Node* newelement();

int op(char x);

char to_char(int i);
void print_block(int* block);

void fprint_block(int* block, FILE* file);

//void ProcessQueue(Node* node, Node** top);