#include "../include/strategydp.h"

#include <stdlib.h>
#include <string.h>

void alocar_matriz_dp(int ***matriz, int linhas, int colunas) {
  *matriz = malloc(sizeof(int *) * linhas);
  for (int i = 0; i < linhas; i++) {
    (*matriz)[i] = malloc(sizeof(int) * colunas);
  }
}

void liberar_matriz_dp(int **matriz, int linhas) {
  for (int i = 0; i < linhas; i++) {
    free(matriz[i]);
  }
  free(matriz);
}

int min(int v1, int v2) { return (v1 < v2) ? v1 : v2; }

/*
 Essa função implementa a busca aproximada usando distância de edição.
 Ela preenche a matriz dp e depois verifica em quais posições do texto o padrão
 aparece com até num_erros operações de edição (inserção, deleção ou
 substituição).
*/
void buscar_casamentos_aproximados_dp(char *texto, char *padrao, int num_erros,
                                      Lista *li, int **dp) {
  int tamanho_texto = strlen(texto);
  int tamanho_padrao = strlen(padrao);

  /*
    Caso base: Isso representa comparar o padrão vazio com os prefixos do
    texto. Como não há nada para "casar", a distância de edição sempre será zero
  */
  for (int i = 0; i <= tamanho_texto; i++) {
    dp[0][i] = 0;
  }

  /*
   Caso base: comparando os primeiros i caracteres do padrão com texto vazio.
   Para transformar os primeiros i caracteres do padrão em uma string vazia,
   são necessárias exatamente i deleções.
  */
  for (int i = 1; i <= tamanho_padrao; i++) {
    dp[i][0] = i;
  }

  for (int i = 1; i <= tamanho_padrao; i++) {
    for (int j = 1; j <= tamanho_texto; j++) {
      if (padrao[i - 1] == texto[j - 1]) {
        // Se os caracteres forem iguais, nenhuma operação é necessária.
        dp[i][j] = dp[i - 1][j - 1];
      } else {
        // Caso contrário, considera-se as três operações possíveis:
        // substituição (diagonal), remoção (linha acima) ou inserção (coluna à
        // esquerda).
        dp[i][j] = 1 + min(min(dp[i - 1][j - 1], dp[i - 1][j]), dp[i][j - 1]);
      }
    }
  }

  /*
    Verifica a última linha a qual representa todo o padrão sendo comparado com
    prefixos do texto. Se a distância for menor ou igual ao número de erros
    permitido, então houve um casamento aproximado nessa posição
  */
  for (int i = 1; i <= tamanho_texto; i++) {
    if (dp[tamanho_padrao][i] <= num_erros) {
      int pos_inicial = i - tamanho_padrao + 1;

      insereFim(li, pos_inicial);
    }
  }
}

/*
  Realiza a busca utilizando de casamento aproximado utilizando programação
  dinâmica e retorna uma struct com a solução encontrada
*/
SolucaoCasamento *encontrar_casamento_aproximados_dp(char *texto, char *padrao,
                                                     int num_erros) {
  int **dp;

  int tamanho_texto = strlen(texto);
  int tamanho_padrao = strlen(padrao);

  alocar_matriz_dp(&dp, tamanho_padrao + 1, tamanho_texto + 1);

  SolucaoCasamento *solucao = alocar_solucao_casamento(padrao);

  buscar_casamentos_aproximados_dp(texto, padrao, num_erros,
                                   solucao->posicoesEncontradas, dp);

  liberar_matriz_dp(dp, tamanho_padrao + 1);

  return solucao;
}