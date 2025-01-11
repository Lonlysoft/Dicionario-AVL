#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct NoAvl{
	char palavra[50];
	char significado[200];
	int altura;
	struct NoAvl* esq;
	struct NoAvl* dir;
}NoAvl;

int AVL__altura(NoAvl* n){
	return n == NULL ? 0 : n->altura;
}

int BSC__maximo(int a, int b){
	return(a > b) ? a : b;
}

// Criação de um novo nó
NoAvl* AVL__novoNo(char* palavra, char* significado){
	NoAvl* n = (NoAvl* )malloc(sizeof(NoAvl));
	strcpy(n->palavra, palavra);
	strcpy(n->significado, significado);
	n->esq = n->dir = NULL;
	n->altura = 1;
	return n;
}

NoAvl* AVL__LL(NoAvl* y){
	NoAvl* x = y->esq;
	NoAvl* T2 = x->dir;

	x->dir = y;
	y->esq = T2;

	y->altura = BSC__maximo(AVL__altura(y->esq), AVL__altura(y->dir)) + 1;
	x->altura = BSC__maximo(AVL__altura(x->esq), AVL__altura(x->dir)) + 1;

	return x;
}

NoAvl* AVL__RR(NoAvl* x){
	NoAvl* y = x->dir;
	NoAvl* T2 = y->esq;

	y->esq = x;
	x->dir = T2;

	x->altura = BSC__maximo(AVL__altura(x->esq), AVL__altura(x->dir)) + 1;
	y->altura = BSC__maximo(AVL__altura(y->esq), AVL__altura(y->dir)) + 1;

	return y;
}

int AVL__calcularBalanceamento(NoAvl* n){
	return n == NULL ? 0 : AVL__altura(n->esq) - AVL__altura(n->dir);
}

NoAvl* AVL__inserirItem(NoAvl* no, char* palavra, char* significado){
	if(no == NULL)
		return AVL__novoNo(palavra, significado);

	if(strcmp(palavra, no->palavra) < 0)
		no->esq = AVL__inserirItem(no->esq, palavra, significado);
	else if(strcmp(palavra, no->palavra) > 0)
		no->dir = AVL__inserirItem(no->dir, palavra, significado);
	else
		return no;

	no->altura = 1 + BSC__maximo(AVL__altura(no->esq), AVL__altura(no->dir));
	int balance = AVL__calcularBalanceamento(no);

	//Rotação LL
	if(balance > 1 && strcmp(palavra, no->esq->palavra) < 0)
		return AVL__LL(no);

	//Rotação RR
	if(balance < -1 && strcmp(palavra, no->dir->palavra) > 0)
		return AVL__RR(no);

	//Rotação LR
	if(balance > 1 && strcmp(palavra, no->esq->palavra) > 0){
		no->esq = AVL__RR(no->esq);
		return AVL__LL(no);
	}

	//Rotação RL
	if(balance < -1 && strcmp(palavra, no->dir->palavra) < 0){
		no->dir = AVL__LL(no->dir);
		return AVL__RR(no);
	}
	return no;
}

NoAvl* AVL__encontrarMinimo(NoAvl* node){
	NoAvl* noAtual = node;
	while(noAtual->esq != NULL)
		noAtual = noAtual->esq;
	return noAtual;
}

NoAvl* AVL__removerItem(NoAvl* raiz, char* palavra){
	if(raiz == NULL)
		return raiz;
	//remocao de uma ABB
	if(strcmp(palavra, raiz->palavra) < 0)
		raiz->esq = AVL__removerItem(raiz->esq, palavra);
	else if(strcmp(palavra, raiz->palavra) > 0)
		raiz->dir = AVL__removerItem(raiz->dir, palavra);
	else{
		if((raiz->esq == NULL) || (raiz->dir == NULL)){
			NoAvl* temp = raiz->esq ? raiz->esq : raiz->dir;
			if(temp == NULL){
				temp = raiz;
				raiz = NULL;
			}else
				*raiz = *temp;

			free(temp);
		}else{
			NoAvl* temp = AVL__encontrarMinimo(raiz->dir);
			strcpy(raiz->palavra, temp->palavra);
			strcpy(raiz->significado, temp->significado);
			raiz->dir = AVL__removerItem(raiz->dir, temp->palavra);
		}
	}
	if(raiz == NULL){
		return raiz;
	}
	//parte do balanceamento
	raiz->altura = 1 + BSC__maximo(AVL__altura(raiz->esq), AVL__altura(raiz->dir));
	int balance = AVL__calcularBalanceamento(raiz);

	if(balance > 1 && AVL__calcularBalanceamento(raiz->esq) >= 0)
		return AVL__LL(raiz);

	if(balance > 1 && AVL__calcularBalanceamento(raiz->esq) < 0){
		raiz->esq = AVL__RR(raiz->esq);
		return AVL__LL(raiz);
	}

	if(balance < -1 && AVL__calcularBalanceamento(raiz->dir) <= 0)
		return AVL__RR(raiz);

	if(balance < -1 && AVL__calcularBalanceamento(raiz->dir) > 0){
		raiz->dir = AVL__LL(raiz->dir);
		return AVL__RR(raiz);
	}

	return raiz;
}

NoAvl* AVL__acharItem(NoAvl* raiz, char* palavra){
	if(raiz == NULL){
		return raiz;
	}
	if(strcmp(raiz->palavra, palavra) == 0)
		return raiz;

	if(strcmp(palavra, raiz->palavra) < 0){
		return AVL__acharItem(raiz->esq, palavra);
	}
	return AVL__acharItem(raiz->dir, palavra);
}

void AVL__printf(NoAvl* raiz){
	if(raiz != NULL){
		AVL__printf(raiz->esq);
		printf("%s h = %d; ", raiz->palavra, raiz->altura);
		AVL__printf(raiz->dir);
	}
}

int AVL__temNaArvore(NoAvl* a, char* p){
    if (a == NULL)
        return 0;

    if (strcmp(p, a->palavra) == 0)
        return 1;
    else if (strcmp(p, a->palavra) < 0)
        return AVL__temNaArvore(a->esq, p);
    else
        return AVL__temNaArvore(a->dir, p);
}

int main(){
	NoAvl* raiz = NULL;
	NoAvl* n;
	int opcao;
	char palavra[50], significado[200];

	do{
		scanf("%d", &opcao);

		switch(opcao){
			case 1:
				printf("Árvores criadas sucesso.\n\n");
				raiz = NULL;
				break;

			case 2: 
				scanf("%s", palavra);
				if(AVL__temNaArvore(raiz, palavra) == 1){
					raiz = AVL__removerItem(raiz, palavra);
					printf("Remoção de %s com sucesso\n\n", palavra);
				}
				else{
					printf("Operação de remoção da palavra %s inválida\n\n", palavra);
				}
				break;

			case 3:
				scanf("%s", palavra);
				getchar();
				fgets(significado, sizeof(significado), stdin);
				significado[strcspn(significado, "\n")] = '\0';
				
				if(AVL__temNaArvore(raiz, palavra) == 0)
					raiz = AVL__inserirItem(raiz, palavra, significado);
					printf("Inserção de %s com sucesso\n\n", palavra);
				}
				else{
					printf("%s já se encontra no dicionário\n\n", palavra);
				}
				break;
			
			//buscar palavra
			case 4:
				scanf("%s", palavra);
				n = AVL__acharItem(raiz, palavra);
				if(n != NULL){
					printf("%s encontrado no dicionario h = %d: \n %s\n\n", n->palavra, n->altura, n->significado);
				}
				else
					printf("%s não encontrado no dicionário\n\n", palavra);
				break;
				
			case 5:
				AVL__printf(raiz);
				printf("\n\n");
				break;
		}
	}while(opcao != 6);
	return 0;
}
