#include "Functions.h"
#define CLEARBIT(A, k) (A[k / 8] &= ~(1 << (k % 8)))
#define SETBIT(A, k) (A[k / 8] |= 1 << (k % 8))
int notvalid1[4] = { 6, 0, 0, 2 };
int notvalid2[4] = { 7, 1, 3, 5 };
int notvalid3[4] = { 8, 2, 6, 8 };
int applyparam[4] = { +3, -3, -1, +1 };
int maxdepth = 35;
char rep[] = "dulr";

void  ClearBit(char A[], char k)
{
	A[k / 8] &= ~(1 << (k % 8));
}

void  SetBit(char A[], char k)
{
	A[k / 8] |= 1 << (k % 8);  // Set the bit at the k-th position in A[i]
}

int ReadBitVar(char* vector, char index)//index 1 = primeira posição, index 2 = segunda
{
	int index2 = index / 2;
	int index3 = index % 2;
	if (index3 == 0)
	{
		return (vector[index2] & 0x0f);
	}
	if (index3 == 1)
	{
		return (vector[index2] & 0xf0) >> 4;
	}

	return 999;
	
}


void WriteToBitArray(char* vector, int index, int num)
{
	int i;
	num = num & 0x0f;
	int bit[4];
	index = index * 4;
	bit[0] = num & 1;
	bit[1] = (num & 2) >> 1;
	bit[2] = (num & 4) >> 2;
	bit[3] = (num & 8) >> 3;
	//int aux;
	for (i = 0; i < 4; i++)
	{
		//aux = index + i;
		//printf("Index: %d\n", index+i);
		if (bit[i] == 1)
			SetBit(vector, index+(i));
			//SETBIT(vector, aux);
		else
			ClearBit(vector, index+(i));
			//CLEARBIT(vector, aux);
	}

}


void Write_Vector(int* dest, char* source)
{
	int i, j;
	char tmp;
	for (i = 0; i < 9; i++)
	{
		j = i / 2;
		tmp = source[j];
		if (i % 2 == 0)
			dest[i] = tmp & 0x0f;
		else
			dest[i] = (tmp & 0xf0) >> 4;
		
		
		//dest[i] = ReadBitVar(source, i);
	}
	//print_block(dest);

}

void StoreVector(char* dest, int* source)
{
	int i;
	for (i = 0; i < 9; i++)
	{
		WriteToBitArray(dest, i, source[i]);
	}
}

void print_state(int* game_state)
{
	int i;
	for (i = 0; i < 9; i++)
	{
		if(game_state[i]==9)
			printf("_ ");
		else
			printf("%d ", game_state[i]);
		if ((i + 1) % 3 == 0)
			printf("\n");
	}
}

int heur(int* block)
{
//#ifdef H2
	int to_return = 0;

	for (int i = 0; i<9; i++)
	{
		to_return += abs((i / 3) - (block[i] / 3));
		to_return += abs((i % 3) - (block[i] % 3));
	}
	return to_return;

/*#else
	int to_return = 0;

	for (int i = 0; i<9; i++)
	{
		if (block[i] != i) to_return++;
	}
	return to_return;


#endif
*/

}

int check_solve(int* block)
{
	int i, j, count;
	count = 0;
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			if (block[i] > block[j])
			{
				count++;
			}
		}
	}
	return count%2;
}

void prepend(Node** Raiz, Node* new_node, Node* old_node, int op)
{
	int vector[9];
	Write_Vector(vector, new_node->bit_var);
	new_node->next = *Raiz;
	*Raiz = new_node;

	strcpy(new_node->str, old_node->str);
	new_node->str[old_node->pathcost] = rep[op];
	new_node->str[old_node->pathcost + 1] = 0;

	new_node->pathcost = old_node->pathcost + 1;
	new_node->totalcost = new_node->pathcost + heur(vector);
	if (new_node->totalcost < old_node->totalcost)
		new_node->totalcost = old_node->totalcost;
}

int goal(Node* no, int* g_block)
{
	if (no == NULL)
		return 0;
	
	int i;
	int block[9];
	Write_Vector(block, no->bit_var);
	for (i = 0; i < 9; i++)
	{
		if (block[i] != g_block[i])
			return 0;
	}

	return 1;
}

int notonqueue(Node* t, Node* newnode)
{
	//int block[9];
	//Write_Vector(block, newnode->bit_var);
	//int i;
	//int vector[9];
	//Node* t = Raiz;
	char tmp1[5], tmp2[5];
	//char tmp1[10];
	memcpy(tmp1, newnode->bit_var, 5);
	tmp1[4] = (tmp1[4] & 0x0f);
	while (t != NULL)
	{
		memcpy(tmp2, t->bit_var, 5);
		//memcpy((tmp1 + 5), t->bit_var, 5);
		tmp2[4] = (tmp2[4] & 0x0f);
		//tmp1[9] = (tmp1[9] & 0x0f);
		
		
		if (!memcmp((void*)tmp1, (void*)tmp2, 5)) {
		//if(!memcmp((void*)tmp1,tmp2, 5)){
			return 1;
		}
		/*
		Write_Vector(vector, t->bit_var);
		for (i = 0; i<9; i++)
			if (vector[i] != block[i]) 
				break;

		
		if (i == 9)	
			return 0;
		*/
			
		
		

		t = t->next;
	}
	
	return 0;
}

Node* bestnodefromqueue(Node* t)
{
	//Node* t = Raiz;
	int min_totalpathcost = maxdepth;
	//int totalpathcost;
	Node* to_return = NULL;
	//int valid;

	while (t != NULL)
	{
		
		//if (ReadBitVar(t->bit_var, 9) == 1 && t->totalcost < min_totalpathcost)
		if (((t->bit_var[4] & 0xf0) >> 4)) {
			if (t->totalcost < min_totalpathcost)
			{
				min_totalpathcost = t->totalcost;

				to_return = t;
			}
		}
		t = t->next;
	}

	//if (to_return != NULL) WriteToBitArray(to_return->bit_var, 9, 0);

	if (to_return)
		to_return->bit_var[4] &= ~(1 << (4));
		//ClearBit(to_return->bit_var, 36);
		//CLEARBIT(to_return->bit_var, 36);
	
	

	return to_return;
}

char apply(Node* newnode, Node* oldnode, int op)
{
	if (oldnode == NULL)
		return 1;
	int j;
	int blank;
	int oldstate[9], newstate[9];
	
	Write_Vector(oldstate, oldnode->bit_var);
	for (j = 0; j < 9; j++)
	{
		if (oldstate[j] == 8)
		{
			blank = j;
			break;
		}
	}

	
	if (blank == notvalid1[op] || blank == notvalid2[op] || blank == notvalid3[op])
		return 1;

	for (j = 0; j<9; j++)
		newstate[j] = oldstate[j];

	newstate[blank] = newstate[blank + applyparam[op]];
	newstate[blank + applyparam[op]] = 8;
	StoreVector(newnode->bit_var, newstate);
	return 0;
}

Node* newelement()
{
	//Node* t = (Node*)calloc(1,sizeof(Node));
	Node* t = (Node*)malloc(sizeof(Node));
	if (t == NULL) return NULL;
	WriteToBitArray(t->bit_var, 9, 1);

	t->str = (char*)calloc(1,(maxdepth + 1));
	
	if (t->str == NULL) return NULL;
	//int i;
	//for (i = 0; i < maxdepth + 1; i++)
		//t->str[i] = '\0';
	

	t->pathcost = 0;
	t->totalcost = 0;
	t->next = NULL;
	return t;
}

int op(char x)
{
	switch (x)
	{
	case 'd': return 0;
	case 'u': return 1;
	case 'l': return 2;
	case 'r': return 3;
	default: printf("ERROR!"); return -1;
	}

}

char to_char(int i)
{
	if (i >= 0 && i <= 7) return i + '1';
	else if (i == 8) return 'x';
	else { printf("ERROR in Program!"); return -1; }

}

void print_block(int* block)
{
	printf("\n");
	printf("\n-------");
	printf("\n|%c|%c|%c|", to_char(block[0]), to_char(block[1]), to_char(block[2]));
	printf("\n-------");
	printf("\n|%c|%c|%c|", to_char(block[3]), to_char(block[4]), to_char(block[5]));
	printf("\n-------");
	printf("\n|%c|%c|%c|", to_char(block[6]), to_char(block[7]), to_char(block[8]));
	printf("\n-------");
}

void fprint_block(int* block, FILE* file)
{
	int i;
	for (i = 0; i < 9; i++)
		fprintf_s(file, "%c ", to_char(block[i]));

	fprintf_s(file, "\n");
}

/*void ProcessQueue(Node* node, Node** top)
{
	Node* t = *top;
	Node* newnode = newelement();
	Node *new1, *new2, *new3, *new4;
	char bit1[5], bit2[5], bit3[5], bit4[5], tmp[5];
	int ret1;
	int ret2;
	int ret3;
	int ret4;
	//Inicializando:
	if (!apply(newnode, node, 0))
	{
		memcpy(bit1, newnode->bit_var, 5);
		bit1[4] = (bit1[4] & 0x0f);
		new1 = newnode;
		newnode = newelement();
		ret1 = 0;
	}
	else
		ret1 = 1;

	if (!apply(newnode, node, 1))
	{
		memcpy(bit2, newnode->bit_var, 5);
		bit2[4] = (bit2[4] & 0x0f);
		new2 = newnode;
		newnode = newelement();
		ret2 = 0;
	}
	else
		ret2 = 1;

	if (!apply(newnode, node, 2))
	{
		memcpy(bit3, newnode->bit_var, 5);
		bit3[4] = (bit3[4] & 0x0f);
		new3 = newnode;
		newnode = newelement();
		ret3 = 0;
	}
	else
		ret3 = 1;

	if (!apply(newnode, node, 3))
	{
		memcpy(bit4, newnode->bit_var, 5);
		bit4[4] = (bit4[4] & 0x0f);
		new4 = newnode;
		ret4 = 0;
	}
	else
		ret4 = 1;

	//Processando:
	while (t != NULL)
	{
		memcpy(tmp, t->bit_var, 5);
		tmp[4] = (tmp[4] & 0x0f);
		if (!ret1 && !memcmp((void*)tmp, (void*)bit1, 5))
			ret1 = 1;
		if (!ret2 && !memcmp((void*)tmp, (void*)bit2, 5))
			ret2 = 1;
		if (!ret3 && !memcmp((void*)tmp, (void*)bit3, 5))
			ret3 = 1;
		if (!ret4 && !memcmp((void*)tmp, (void*)bit4, 5))
			ret4 = 1;

		t = t->next;
	}

	//Adicionando na fila:
	if (!ret1)
		prepend(top, new1, node, 0);
	if (!ret2)
		prepend(top, new2, node, 1);
	if (!ret3)
		prepend(top, new3, node, 2);
	if (!ret4)
		prepend(top, new4, node, 3);

}*/