#include "../include/entrada.h"

#include "../include/strategy_shiftand.h"
#include "../include/strategydp.h"

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
  // Verifica se os argumentos de linha de comando são válidos
  if (!is_argumentos_validos(argc, argv)) {
    printf("Erro: Parâmetros inválidos.\n");
    printf("Uso correto: ./tp3_parte1 {estrategia} {qtd_erros} -i input/texto "
           "input/padroes -o saida.txt\n");
    printf("Onde {qtdErros} deve ser menor que o tamanho do padrao\n");
    printf("Onde '-i' indica os arquivos de entrada.\n");
    printf("Onde '-o' indica o arquivo de saída (Opcional)\n");
    printf(
        "Estratégia aceita somente 1(Programação Dinâmica) e 2(Shift And)\n");

    return 1;
  }

  char *input_text_path = argv[ARQUIVO_ENTRADA_TEXTO_P];
  char *input_patterns_path = argv[ARQUIVO_ENTRADA_PADRAO_P];

  FILE *input_text_fp = fopen(input_text_path, "r");
  FILE *input_patterns_fp = fopen(input_patterns_path, "r");

  if (input_text_fp == NULL || input_patterns_fp == NULL) {
    perror("Erro ao abrir os arquivos de entrada\n");
    return 1;
  }

  // Obtém o caminho de arquivo de saída através dos argumentos
  char output_path[80];
  get_output_path(argc, argv, output_path);

  FILE *output_fp = fopen(output_path, "w");

  if (output_fp == NULL) {
    perror("Erro ao abrir arquivo de saida\n");
    return 1;
  }

  int qtd_erros = atoi(argv[QTD_ERROS_P]);

  int estrategia_escolhida = atoi(argv[ESTRATEGIA_P]);

  char linha[1024];
  char texto[10000];
  texto[0] = '\0';

  while (fgets(linha, sizeof(linha), input_text_fp)) {
    strcat(texto, linha);
  }

  char padrao[100];
  while (fscanf(input_patterns_fp, "%s", padrao) == 1) {
    SolucaoCasamento *solucao = NULL;
    switch (estrategia_escolhida) {
    case SHIFT_AND:
      solucao = encontrar_casamento_aproximados_sa(texto, padrao, qtd_erros);
      break;
    case PROGRAMACAO_DINAMICA:
      solucao = encontrar_casamento_aproximados_dp(texto, padrao, qtd_erros);
      break;
    }

    escrever_solucao_casamento_terminal(solucao);
    escrever_solucao_casamento_arquivo(solucao, output_fp);

    destroi_solucao_casamento(&solucao);
  }

  // Fechar todos os arquivos utilizados
  fclose(input_text_fp);
  fclose(input_patterns_fp);
  fclose(output_fp);

  return 0;
}