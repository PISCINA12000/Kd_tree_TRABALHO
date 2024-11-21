#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct kdtree
{
    int x, y;
    struct kdtree *esq, *dir;
};
typedef struct kdtree KdTree;

struct pilha{
    KdTree *arvore;
    struct pilha *prox;
};
typedef struct pilha Pilha;

// Inicializa a pilha (define o topo como NULL).
void init(Pilha** P) {
    *P = NULL;
}

// Verifica se a pilha está vazia.
char isEmpty(Pilha* P) {
    return P == NULL;
}

// Empilha um ponteiro para uma estrutura Tree na pilha.
void push(Pilha** P, KdTree *x) {
    Pilha *nova = (Pilha*) malloc(sizeof(Pilha));
    nova->arvore = x;
    nova->prox = *P;
    *P = nova;
}

// Retorna o ponteiro para a estrutura Tree no topo da pilha.
void top(Pilha* P, KdTree** arvoreHuf) {
    if (!isEmpty(P)) {
        *arvoreHuf =  P->arvore;
    }
    *arvoreHuf = NULL;
}

// Desempilha um ponteiro para uma estrutura Tree e o retorna via referência.
void pop(Pilha** P, KdTree** arvoreHuf) {
    Pilha* aux;
    KdTree *auxTree;
    if (!isEmpty(*P)) {
        auxTree = (*P)->arvore;
        aux = *P;
        *P = (*P)->prox;
        free(aux);
    } else {
        auxTree = NULL;
    }
    *arvoreHuf = auxTree;
}

// Retorna o tamanho da pilha.
int tamanho(Pilha* P) {
    int cont = 0;
    while (P != NULL) {
        cont++;
        P = P->prox;
    }
    return cont;
}