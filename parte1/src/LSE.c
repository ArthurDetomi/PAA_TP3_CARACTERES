#include "../include/LSE.h"
#include <stdio.h>

Lista *criaLista() {
  Lista *li;
  li = (Lista *)malloc(sizeof(Lista));
  if (li != NULL) {
    *li = NULL;
  }
  return li;
}

int listaVazia(Lista *li) {
  if (li == NULL)
    return 1;
  if (*li == NULL)
    return 1; // True - Vazia!
  return 0;   // False - tem elemento!
}

NO *alocarNO() { return (NO *)malloc(sizeof(NO)); }

void liberarNO(NO *q) { free(q); }

int insereIni(Lista *li, int elem) {
  if (li == NULL)
    return 0;
  NO *novo = alocarNO();
  if (novo == NULL)
    return 0;
  novo->info = elem;
  novo->prox = *li;
  *li = novo;
  return 1;
}

int insereFim(Lista *li, int elem) {
  if (li == NULL)
    return 0;
  NO *novo = alocarNO();
  if (novo == NULL)
    return 0;
  novo->info = elem;
  novo->prox = NULL;
  if (listaVazia(li)) {
    *li = novo;
  } else {
    NO *aux = *li;
    while (aux->prox != NULL)
      aux = aux->prox;
    aux->prox = novo;
  }
  return 1;
}

int removeIni(Lista *li) {
  if (li == NULL)
    return 0;
  if (listaVazia(li))
    return 0;
  NO *aux = *li;
  *li = aux->prox;
  liberarNO(aux);
  return 1;
}

int removeFim(Lista *li) {
  if (li == NULL)
    return 0;
  if (listaVazia(li))
    return 0;
  NO *ant, *aux = *li;
  while (aux->prox != NULL) {
    ant = aux;
    aux = aux->prox;
  }
  if (aux == *li)
    *li = aux->prox;
  else
    ant->prox = aux->prox;
  liberarNO(aux);
  return 1;
}

void imprimeLista(Lista *li) {
  if (li == NULL) {
    printf("Lista nula\n");
    return;
  }
  if (listaVazia(li)) {
    printf("Lista vazia\n");

    return;
  }
  NO *aux = *li;

  int size = tamanho(li);

  int count = 0;
  while (aux != NULL) {
    printf("%d", aux->info);

    if (count != size - 1) {
      printf(" ");
    }

    aux = aux->prox;
    count++;
  }
  printf("\n");
}

void imprimeListaArquivo(Lista *li, FILE *output_fp) {
  if (li == NULL)
    return;
  if (listaVazia(li)) {
    return;
  }
  NO *aux = *li;

  int size = tamanho(li);

  int count = 0;
  while (aux != NULL) {
    fprintf(output_fp, "%d", aux->info);

    if (count != size - 1) {
      fprintf(output_fp, " ");
    }

    aux = aux->prox;
    count++;
  }
  fprintf(output_fp, "\n");
}

void recComplementar(NO *n) {
  if (n == NULL)
    return;
  recComplementar(n->prox);
  printf("%d ", n->info);
}

void imprimeRevRec(Lista *li) {
  if (li == NULL)
    return;
  if (listaVazia(li)) {
    printf("Lista Vazia!\n");
    return;
  }
  printf("Elementos:\n");
  recComplementar(*li);
  printf("\n");

  // imprimeRevRec(&(*li)->prox);
  // printf("%d ", (*li)->info);
}

void imprimeRev(Lista *li) {
  if (li == NULL)
    return;
  if (listaVazia(li)) {
    printf("Lista Vazia!\n");
    return;
  }
  printf("Elementos REV:\n");
  NO *ant, *aux;
  NO *fim = NULL;
  do {
    aux = *li;
    while (aux != fim) {
      ant = aux;
      aux = aux->prox;
    }
    printf("%d ", ant->info);
    fim = ant;
  } while (fim != *li);
  printf("\n");
}

int tamanho(Lista *li) {
  if (li == NULL)
    return -1;
  if (listaVazia(li)) {
    return 0;
  }

  NO *aux = *li;

  int tamanho = 0;

  while (aux != NULL) {
    aux = aux->prox;
    tamanho++;
  }

  return tamanho;
}

int procura(Lista *li, int x) {
  if (li == NULL || listaVazia(li)) {
    return -1;
  }

  NO *aux = *li;

  while (aux != NULL) {
    if (aux->info == x) {
      return 1;
    }

    aux = aux->prox;
  }

  return 0;
}

int removePrimeiraOcorrencia(Lista *li, int elem) {
  if (li == NULL || listaVazia(li)) {
    return 0;
  }

  NO *anterior = NULL, *atual = *li;

  while (atual != NULL && atual->info != elem) {
    anterior = atual;
    atual = atual->prox;
  }

  if (atual == NULL) {
    return 0;
  }

  if (anterior == NULL) {
    *li = atual->prox;
  } else {
    anterior->prox = atual->prox;
  }

  liberarNO(atual);

  return 1;
}

int insereOrdenado(Lista *li, int elem) {
  if (li == NULL) {
    return -1;
  }

  NO *anterior = NULL, *atual = *li;

  while (atual != NULL && atual->info < elem) {
    anterior = atual;
    atual = atual->prox;
  }

  if (anterior == NULL) {
    return insereIni(li, elem);
  }

  NO *novo_no = alocarNO();
  novo_no->info = elem;

  anterior->prox = novo_no;
  novo_no->prox = atual;

  return 1;
}

void destroiLista(Lista *li) {
  if (li != NULL) {
    NO *aux;
    while ((*li) != NULL) {
      aux = *li;
      *li = (*li)->prox;
      liberarNO(aux);
    }
    free(li);
  }
}
