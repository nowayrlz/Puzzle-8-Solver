#include "Functions.h"





int main()
{
	setlocale(LC_ALL, "");
	int goal_block[9] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 }; //8 indicates no tile
	int i;
	int block[9];// = { 0, 3, 1, 2, 8, 5, 6, 7, 4 }; //Example
	//int block[9] = { 7, 5, 6, 1, 4, 3, 2, 8, 0 }; //Hardest possible - 31 moves
	//int block[9] = { 0, 7, 4, 8, 1, 3, 2, 5, 6 }; //Hard - 29moves

	
	printf("\nQuebra-cabeça do 8!\n");

	printf("\nPor favor, entre com o estado inicial do jogo:\n"
		" [Represente as posições com números de 1 a 8, e o espaço livre com 'x'.\n"
		" Comece escrevendo da esquerda para a direita de cada linha.\n"
		" Sua string final vai parecer com isso: '1 4 2 3 x 6 7 8 5'.\n"
		" Não se esqueceça de colocar espaço entre os caracteres]\n");

	i = 0;
	
	while (i<9)
	
	{
		char chr;
		chr = fgetc(stdin);
		if (chr == 32) continue;
		if (chr == 'x') block[i] = 8;
		else if (chr >= '1' && chr <= '9') block[i] = chr - '1';
		else { 
			printf("String invalida. Exemplo de string valida: 2 1 3 4 7 5 6 8 x."); 
			return 1; 
		}
		i++;
	}

	fgetc(stdin); //flush out the end of line character

	printf("\n Agora entre com o estado objetivo: (Geralmente: 1 2 3 4 5 6 7 8 x) ");

	i = 0;
	while (i<9)
	{
		char chr;
		chr = fgetc(stdin);
		if (chr == 32) continue;
		if (chr == 'x') goal_block[i] = 8;
		else if (chr >= '1' && chr <= '9') goal_block[i] = chr - '1';
		else { 
			printf("chr=%d. Entrada invalida. Exemplo de entrada valida...2 1 3 4 7 5 6 8 x.", (int)chr); 
			getch(stdin);
			return 1; 

		}
		
		i++;
	}

	printf("Entre com o limite de nivel: (<25 é resolvido rapidamente): ");
	scanf_s("%d", &maxdepth);
	
	printf("\nTrabalhando...");

	Node* top = newelement();
	StoreVector(top->bit_var, block);
	top->totalcost = heur(block);

	Node* newnode = newelement();
	int contagem = 0;
	Node* node;
	
	float startTime = (float)clock() / CLOCKS_PER_SEC;
	float timeElapsed;
	while (1)
	{
		

		node = bestnodefromqueue(top);
		//printf("Contagem: %d", contagem);
		contagem++;
		/*
		int game_state[9];
		printf("Debug INFO %d: \n", contagem);
		printf("Matriz:\n");
		Write_Vector(game_state, node->bit_var);
		print_block(game_state);
		printf("\nValid: %d\n", ReadBitVar(node->bit_var, 9));
		//getch(stdin);
		//fprint_block(game_state, fp);
		*/
		if (node == NULL) {
			float endTime = (float)clock() / 1000;

			float timeElapsed = endTime - startTime;
			
			
			printf("pronto!\nNão há solução até o nivel %d.\nTente aumentar o nivel, se não houver solução e o nivel for >31 o padrão não é resolvivel.\n\n", maxdepth);
			
			printf("Time to execute: %f s\n", timeElapsed);
			break;
		}
		else if (goal(node, goal_block)) {
			float endTime = (float)clock() / CLOCKS_PER_SEC;

			float timeElapsed = endTime - startTime;

			char chr[15];
			printf("pronto. \nA solução com menor número de jogadas é (%d).", node->pathcost);
			printf("\nTime to execute: %f s", timeElapsed);
			printf("\nQuer que mostre o passo a passo? (Y/N)?");
			scanf("%s", chr);
			
			if (chr[0] == 'n' || chr[0] == 'N') {
				printf("\n (Move Blank u=up, d=down, l=left, r=right)\n");
				printf(node->str);
				printf("\n");
				break;
			}

			int block2[9];
			int j = 0;
			Node* solution = newelement();
			Node* original_state = newelement();
			
			
			for (i = 0; i<node->pathcost; i++)
			{
				printf("\n\n\nPasso %d:\n", i);
				print_block(block);
				
				StoreVector(original_state->bit_var, block);
				
				apply(solution, original_state, op(node->str[i]));
				Write_Vector(block2, solution->bit_var);
				
				for (j = 0; j < 9; j++)
					block[j] = block2[j];
				getch(stdin);
			}
			printf("\n\n\nPasso %d:\n", i);
			print_block(block);

			printf("\nPasso a passo completo.\nOs passos feitos foram: (Mover a peça livre: u=up, d=down, l-left, r=right)\n");
			printf(node->str);
			printf("\n");
			break;

		}

		if (node->totalcost > maxdepth) continue;

		if(!apply(newnode,node, 0))
			if (!notonqueue(top, newnode)) {
				prepend(&top, newnode, node, 0);
				newnode = newelement();
			}
		if (!apply(newnode, node, 1))
			if (!notonqueue(top, newnode)) {
				prepend(&top, newnode, node, 1);
				newnode = newelement();
			}
		if (!apply(newnode, node, 2))
			if (!notonqueue(top, newnode)) {
				prepend(&top, newnode, node, 2);
				newnode = newelement();
			}
		if (!apply(newnode, node, 3))
			if (!notonqueue(top, newnode)) {
				prepend(&top, newnode, node, 3);
				newnode = newelement();
			}
		
		//ProcessQueue(node, &top);
		//newnode = newelement();
		/*for (i = 0; i < 4; i++) {
			if (apply(newnode, node, i) == -1)
				continue;

			if (notonqueue(top, newnode)) {
				prepend(&top, newnode, node, i);
				newnode = newelement();
				//if (newnode == NULL) { printf("ERROR!! insufficient memory!! Try decreasing depth!"); return 1; }
			}
		}*/

	}
	
	float endTime = (float)clock() / 1000;
	timeElapsed = endTime - startTime;
	printf("Processing time: %f s\n", timeElapsed);
	printf("Iterações do while: %d", contagem);
	char h = getch(stdin);
	//exit(0);
	return 0;
}