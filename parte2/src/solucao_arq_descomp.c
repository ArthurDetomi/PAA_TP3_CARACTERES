#include "../include/solucao_arq_descomp.h"

#include <string.h>

void bmh_texto_descomprimido(TipoTexto T, int n, TipoPadrao P, int m,
                             SolucaoCasamento *solucao) {
  int i, j, k;
  int d[MAXCHAR + 1];

  for (j = 0; j <= MAXCHAR; j++) {
    d[j] = m;
  }

  for (j = 0; j < m - 1; j++) {
    d[(unsigned char)P[j]] = m - j - 1;
  }

  i = 0;
  while (i <= n - m) {
    j = m - 1;

    while (j >= 0) {
      solucao->qtd_comparacoes++;

      if (T[i + j] != P[j]) {
        break;
      }

      j--;
    }

    if (j < 0) {
      insereFim(solucao->posicoesEncontradas, i + 1);
      i++;
    } else {
      i += d[(unsigned char)T[i + m - 1]];
    }
  }
}

SolucaoCasamento *buscar_arquivo_descomprimido(TipoTexto texto,
                                               TipoPadrao padrao) {
  SolucaoCasamento *solucao = alocar_solucao_casamento(padrao);

  int n = strlen(texto);
  int m = strlen(padrao);

  bmh_texto_descomprimido(texto, n, padrao, m, solucao);

  return solucao;
}
