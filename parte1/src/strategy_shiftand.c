#include "../include/resposta.h"
#include <string.h>

int shift_and(char *padrao, char *texto, int num_erros,
              Lista *posicoes_encontradas) {
  int tamanho_texto = strlen(texto);
  int tamanho_padrao = strlen(padrao);

  unsigned int mascaras_bits[256];

  /*
    Inicializa a tabela de máscaras para cada caractere ASCII
    Cada bit da máscara representa a presença do caractere em uma posição do
    padrão
  */
  for (int i = 0; i < 256; i++) {
    mascaras_bits[i] = 0;
  }

  for (int i = 0; i < tamanho_padrao; i++) {
    unsigned char c = padrao[i];

    // Bit i indica ocorrência de 'c' na posição i do padrão
    mascaras_bits[c] |= (1 << i);
  }

  unsigned int estados[num_erros + 1];
  unsigned int estados_novo[num_erros + 1];

  estados[0] = 0;
  for (int j = 1; j <= num_erros; j++) {
    estados[j] = (1 << j) - 1;
  }

  for (int i = 0; i < tamanho_texto; i++) {
    unsigned char c = texto[i];

    // Atualiza o estado com zero erro
    estados_novo[0] = ((estados[0] << 1) | 1) & mascaras_bits[c];

    // Atualiza os estados com até j erros permitidos
    for (int j = 1; j <= num_erros; j++) {
      // substituição
      estados_novo[j] = ((estados[j] << 1) | 1) & mascaras_bits[c];
      // inserção
      estados_novo[j] |= (estados[j - 1] << 1);
      // deleção
      estados_novo[j] |= estados[j - 1];
      // erro acumulado
      estados_novo[j] |= estados_novo[j - 1];
    }

    // Copia os novos estados para os antigos para a próxima iteração
    for (int j = 0; j <= num_erros; j++) {
      estados[j] = estados_novo[j];
    }

    // Verifica se houve casamento com até num_erros
    if (estados[num_erros] & (1 << (tamanho_padrao - 1))) {
      int posicao_encontrada = (i - tamanho_padrao + 1) + 1;
      insereFim(posicoes_encontradas, posicao_encontrada);
    }
  }

  return 0;
}

// Retorna a struct com todas as posições onde o padrão com até num_erros de
// tolerância foi encontrado
SolucaoCasamento *encontrar_casamento_aproximados_sa(char *texto, char *padrao,
                                                     int num_erros) {
  SolucaoCasamento *solucao = alocar_solucao_casamento(padrao);

  shift_and(padrao, texto, num_erros, solucao->posicoesEncontradas);

  return solucao;
}