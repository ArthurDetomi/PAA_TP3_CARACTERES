#include "../include/entrada.h"

#include "../include/strategy_shiftand.h"
#include "../include/strategydp.h"
#include "../include/tempo.h"

#include <stdio.h>
#include <string.h>

#define MAX_TAM_TEXTO 100000
#define MAX_TAM_LINHA 2048
#define MAX_TAM_OUTPUT_PATH 80
#define MAX_TAM_PADRAO 120

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
  char output_path[MAX_TAM_OUTPUT_PATH];
  get_output_path(argc, argv, output_path);

  FILE *output_fp = fopen(output_path, "w");

  if (output_fp == NULL) {
    perror("Erro ao abrir arquivo de saida\n");
    return 1;
  }

  int qtd_erros = atoi(argv[QTD_ERROS_P]);

  int estrategia_escolhida = atoi(argv[ESTRATEGIA_P]);

  char linha[MAX_TAM_LINHA];
  char texto[MAX_TAM_TEXTO];
  texto[0] = '\0';

  while (fgets(linha, sizeof(linha), input_text_fp)) {
    strcat(texto, linha);
  }

  // Inicia o temporizador para medir o tempo total de execução
  Temporizador tempo_total;
  iniciarTemporizador(&tempo_total);

  int qtdPadroes = 0;

  char padrao[MAX_TAM_PADRAO];
  while (fscanf(input_patterns_fp, "%s", padrao) == 1) {
    Temporizador tempo_teste;
    iniciarTemporizador(&tempo_teste);

    SolucaoCasamento *solucao = NULL;
    switch (estrategia_escolhida) {
    case SHIFT_AND:
      solucao = encontrar_casamento_aproximados_sa(texto, padrao, qtd_erros);
      break;
    case PROGRAMACAO_DINAMICA:
      solucao = encontrar_casamento_aproximados_dp(texto, padrao, qtd_erros);
      break;
    }

    // Finaliza a medição de tempo para este teste
    finalizarTemporizador(&tempo_teste);

    // Escreve nas saidas respectivas
    printf("\tTeste com padrão %d\n", ++qtdPadroes);
    printf("Solução:\n");
    escrever_solucao_casamento_terminal(solucao);
    escrever_solucao_casamento_arquivo(solucao, output_fp);
    printf("\n");
    imprimirTempos(&tempo_teste);
    printf("------------------------\n");

    destroi_solucao_casamento(&solucao);
  }

  // Finaliza a medição do tempo total de execução
  finalizarTemporizador(&tempo_total);

  // Exibe o tempo total de execução do programa
  printf("Tempo total de execução:\n");
  imprimirTempos(&tempo_total);

  // Fechar todos os arquivos utilizados
  fclose(input_text_fp);
  fclose(input_patterns_fp);
  fclose(output_fp);

  return 0;
}