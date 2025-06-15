#ifndef RESPOSTA_H
#define RESPOSTA_H

#include "../include/LSE.h"
#include <stdio.h>

typedef struct SolucaoCasamento {
  char *padrao;
  Lista *posicoesEncontradas;
} SolucaoCasamento;

SolucaoCasamento *alocar_solucao_casamento(char *padrao);

void destroi_solucao_casamento(SolucaoCasamento **solucao);

void escrever_solucao_casamento_terminal(SolucaoCasamento *solucao);

void escrever_solucao_casamento_arquivo(SolucaoCasamento *solucao,
                                        FILE *output_fp);

#endif