#include "../include/resposta.h"

#include <stdlib.h>
#include <string.h>

SolucaoCasamento *alocar_solucao_casamento(char *padrao) {
  SolucaoCasamento *solucao =
      (SolucaoCasamento *)malloc(sizeof(SolucaoCasamento));

  solucao->padrao = (char *)malloc(strlen(padrao) + 1);
  solucao->qtd_comparacoes = 0;

  strcpy(solucao->padrao, padrao);
  solucao->posicoesEncontradas = criaLista();

  return solucao;
}

void destroi_solucao_casamento(SolucaoCasamento **solucao) {
  destroiLista((*solucao)->posicoesEncontradas);
  free((*solucao)->padrao);
  free(*solucao);
  *solucao = NULL;
}

void escrever_solucao_casamento_terminal(SolucaoCasamento *solucao) {
  printf("%s ", solucao->padrao);
  imprimeLista(solucao->posicoesEncontradas);

  printf("\nQtd de comparações realizadas:\n");
  printf("%d comparações\n", solucao->qtd_comparacoes);
}

void escrever_solucao_casamento_arquivo(SolucaoCasamento *solucao,
                                        FILE *output_fp) {
  fprintf(output_fp, "%s ", solucao->padrao);
  imprimeListaArquivo(solucao->posicoesEncontradas, output_fp);
}