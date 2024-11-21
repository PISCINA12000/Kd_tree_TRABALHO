#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "codigos_auxiliares/TAD_PilhaEnd.h"

#define K 2
#define TF 10

// STRUCTS -------------------------------------------------------------------------------------------------------
struct listaPontos{
	int ponto[K];
	struct listaPontos *prox;
};
typedef listaPontos ListaPontos;

// ORDENAR - EXIBIR - GERAR --> PONTOS ----------------------------------------------------------------------------
void ordenarPontos(int pontos[TF][K], int ini, int fim, int d)
{
    int menorX, menorY, menorPos;
    // INI E FIM SAO AS POSICOES DO MEU VETOR
    // POIS TEM MOMENTOS NO MEU CODIGO QUE QUERO APENAS ORDENAR UMA PARCELA DOS PONTOS
    // **FIM SEMPRE VEM COM UMA POSICAO A MAIS, PARA QUE NAO PAREMOS DE ORDENAR UMA CASA ANTES
    // **INI VEM NORMALMENTE

    // D EH O DISCRIMINANTE, OU SEJA, SE VOU ORDENAR PELO X OU PELO Y
    if (d == 0) // VOU ORDENAR PELO X
    {
        for (int i=ini; i<fim; i++)
        {
            menorX = pontos[i][0];
            menorY = pontos[i][1];
            menorPos = i;
            for (int j=i+1; j<fim; j++)
            {
                if (pontos[j][0] < menorX)
                {
                    menorX = pontos[j][0];
                    menorY = pontos[j][1];
                    menorPos = j;
                }
                else // NAO EH MENOR, MAS PODE SER IGUAL
                {
                    if (pontos[j][0] == menorX) // SAO IGUAIS
                    {
                        if(pontos[j][1] < menorY) // VOU VERIFICAR O Y
                        {
                            menorY = pontos[j][1];
                            menorPos = j;
                        }
                    }
                }
            }
            pontos[menorPos][0] = pontos[i][0];
            pontos[menorPos][1] = pontos[i][1];
            pontos[i][0] = menorX;
            pontos[i][1] = menorY;
        }
    }
    else // VOU ORDENAR PELO Y
    {
        for (int i=ini; i<fim; i++)
        {
            menorX = pontos[i][0];
            menorY = pontos[i][1];
            menorPos = i;
            for (int j=i+1; j<fim; j++)
            {
                if (pontos[j][1] < menorY)
                {
                    menorX = pontos[j][0];
                    menorY = pontos[j][1];
                    menorPos = j;
                }
                else // NAO EH MENOR, MAS PODE SER IGUAL
                {
                    if (pontos[j][1] == menorY) // SAO IGUAIS
                    {
                        if(pontos[j][0] < menorX) // VOU VERIFICAR O Y
                        {
                            menorX = pontos[j][0];
                            menorPos = j;
                        }
                    }
                }
            }
            pontos[menorPos][0] = pontos[i][0];
            pontos[menorPos][1] = pontos[i][1];
            pontos[i][0] = menorX;
            pontos[i][1] = menorY;
        }
    }
}
void gerarPontos(int pontos[TF][K])
{
    // for (i = 0; i < 10; i++)
    // {
    // 	/* gerando valores aleatórios entre zero e 100 */
    // 	printf("%d ", rand() % 100);
    // }

    for (int i = 0; i < TF; i++)
    {
        pontos[i][0] = 1 + (rand() % 100);
        pontos[i][1] = 1 + (rand() % 100);
    }
}
void exibirPontos(int pontos[TF][K])
{
    printf("Pontos gerados aleatoriamente:\n");
    for (int i = 0; i < TF; i++)
    {
        printf("Ponto %d: (%d,%d)\n", i + 1, pontos[i][0], pontos[i][1]);
    }
    printf("\n\n");
}

// LISTA DE PONTOS
ListaPontos *criaNoLista(int x, int y)
{
	ListaPontos *NC;
	NC = (ListaPontos*)malloc(sizeof(ListaPontos));
	NC->prox = NULL;
	NC->ponto[0] = x;
	NC->ponto[1] = y;
	
	return NC;
}

// CRIAR ARVORE ---------------------------------------------------------------------------------------------------
KdTree* criaNoKd(int ponto[K])
{
    KdTree *nova = (KdTree*)malloc(sizeof(KdTree));

    nova->x = ponto[0];
    nova->y = ponto[1];
    nova->esq = NULL;
    nova->dir = NULL;

    return nova;
}
void insereBalanceadaR(KdTree **raiz, int pontos[TF][K], int ini, int fim , int n)
{
        int d, meio;

        if (ini <= fim)
        {

                d = n%K;
                meio = (ini+fim)/2;

                ordenarPontos(pontos, ini, fim+1, d);

                *raiz = criaNoKd(pontos[meio]);

                                                         //ini   //fim   //n
                insereBalanceadaR(&(*raiz)->esq, pontos, ini,    meio-1, n+1);
                insereBalanceadaR(&(*raiz)->dir, pontos, meio+1, fim,    n+1);
        }
}
void exibeArvore(KdTree *arvore)
{
    static int n = -1;
    if (arvore == NULL)
        return;
    n++;
    exibeArvore(arvore->dir);
    for (int i = 0; i < 5 * n; i++)
        printf(" ");
    printf("(%d,%d)\n",arvore->x,arvore->y);
    exibeArvore(arvore->esq);
    n--;
}
void buscarPontos(KdTree *raiz, int coordenada[K], int raio, ListaPontos **L)
{

	int distancia;
	ListaPontos *aux ,*NC, *ant;
	Pilha *P;
	init(&P);
	push(&P, raiz);

	while(!isEmpty(P))
	{
		if (raiz != NULL)
		{
			pop(&P, &raiz);
			while(raiz != NULL)
			{
				distancia = sqrt(pow(coordenada[0]- raiz->x,2) + pow(coordenada[1] - raiz->y,2));
				if (distancia <= raio) // esta dentro do raio
				{
					NC = criaNoLista(raiz->x, raiz->y);
					
					if (*L == NULL)
						*L = NC;
					else
					{
						aux = *L;
						while(aux != NULL && aux->ponto[0] != raiz->x && aux->ponto[1] != raiz->y)
						{
							ant = aux;
							aux = aux->prox;
						}
						if (aux == NULL)
						{
							ant->prox = NC;
						}
					}
					// insere na lista
				}
				push(&P, raiz);
				raiz = raiz->dir;
			}
		}
		pop(&P, &raiz);
		raiz = raiz->esq;
		if (raiz != NULL)
			push(&P, raiz);
	}
}

int main()
{
    int pontos[TF][K];
    int coord[K];
    KdTree *raiz = NULL;
    ListaPontos *lista = NULL;

    //DEFININDO A COORDENADA QUE USAREI COMO PROVA REAL
    coord[0] = 50;
    coord[1] = 50;

    gerarPontos(pontos);
    exibirPontos(pontos);

    // ORDENAR E EXIBIR EM FUNCAO DO X
    ordenarPontos(pontos, 0, 10, 0);
    exibirPontos(pontos);

    // ORDENAR E EXIBIR EM FUNCAO DO Y
    ordenarPontos(pontos, 0, 10, 1);
    exibirPontos(pontos);

    //CRIAR A ARVORE HUFFMAN COM OS PONTOS FORNECIDOS
    insereBalanceadaR(&raiz, pontos, 0, 9, 0);
    exibeArvore(raiz);

    //BUSCAR OS PONTOS QUE ME INTERESSAM
    buscarPontos(raiz, coord, 15, &lista);

    //EXIBIR OS PONTOS ENCONTRADOS
    printf("Pontos dentro da distancia:\n");
    while(lista)
    {
        printf("(%d,%d)\n", lista->ponto[0], lista->ponto[1]);
    }
    printf("\n\n");

    return 0;
}