#ifndef RESPOSTA_H
#define RESPOSTA_H

#include "../include/LSE.h"

typedef struct Solucao {
  char *padrao;
  Lista *posicoesEncontradas;
} SolucaoCasamento;

SolucaoCasamento *alocar_solucao_casamento();

#endif