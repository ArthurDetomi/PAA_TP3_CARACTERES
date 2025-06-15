/*----------------- File: LSE.h  ----------------------+
|Lista Simplesmente Encadeada                           |
| | | | | Implementado por Guilherme C. Pena em 14/09/2023      |
+-------------------------------------------------------+ */

#ifndef LISTASE_H
#define LISTASE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct NO {
  int info;
  struct NO *prox;
} NO;

typedef struct NO *Lista;

Lista *criaLista();

int listaVazia(Lista *li);

NO *alocarNO();

void liberarNO(NO *q);

int insereIni(Lista *li, int elem);

int insereFim(Lista *li, int elem);

int removeIni(Lista *li);

int removeFim(Lista *li);

void imprimeLista(Lista *li);

void recComplementar(NO *n);

void imprimeRevRec(Lista *li);

void imprimeRev(Lista *li);

void destroiLista(Lista *li);

/* Exercício 2.2 */
int tamanho(Lista *li);
int procura(Lista *li, int x);
int insereOrdenado(Lista *li, int elem);
int removePrimeiraOcorrencia(Lista *li, int elem);

#endif
