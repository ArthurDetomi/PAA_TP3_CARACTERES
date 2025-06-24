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

  /*
    Percorre o texto, i representa o início onde o padrão está sendo alinhado
    com o texto.
  */
  i = 0;
  while (i <= n - m) {
    j = m - 1;

    while (j >= 0) {
      solucao->qtd_comparacoes++;

      // Como não casou deve saltar
      if (T[i + j] != P[j]) {
        break;
      }

      j--;
    }

    // Verifica se padrão casou completamente com o texto
    if (j < 0) {
      insereFim(solucao->posicoesEncontradas, i + 1);
      i++;
    } else {
      // Realizar salto seguro caso não casou
      i += d[(unsigned char)T[i + m - 1]];
    }
  }
}

/*
  Recebe o texto descomprimido e utiliza BMH para encontrar casamento exato e
  retorna a solução encontrada
*/
SolucaoCasamento *buscar_arquivo_descomprimido(TipoTexto texto,
                                               TipoPadrao padrao) {
  SolucaoCasamento *solucao = alocar_solucao_casamento(padrao);

  int n = strlen(texto);
  int m = strlen(padrao);

  bmh_texto_descomprimido(texto, n, padrao, m, solucao);

  return solucao;
}
