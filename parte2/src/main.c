#include "../include/entrada.h"
#include "../include/huffmanbyte.h"
#include "../include/solucao_arq_descomp.h"
#include "../include/tempo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TAM_LINHA 2048

int main(int argc, char *argv[]) {
  // Verifica se os argumentos de linha de comando são válidos
  if (!is_argumentos_validos(argc, argv)) {
    printf("Erro: Parâmetros inválidos.\n");
    printf("Uso correto: ./tp3_parte2 -i input/texto input/padroes -o "
           "saida.txt\n");
    printf("Onde '-i' indica os arquivos de entrada.\n");
    printf("Onde '-o' indica o arquivo de saída (Opcional)\n");

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

  int option_escolhida = atoi(argv[FLAG_OPTION_P]);

  if (option_escolhida == ARQ_COMPRIMIDO_TEST) {
    // Teste com arquivo comprimido
    comprimir_arquivo_entrada(input_text_path);

    TipoAlfabeto alfabeto;
    int maxCompCod, numNodosFolhas;
    TipoVetorPalavra vocabulario =
        (TipoVetorPalavra)calloc(M + 1, sizeof(TipoPalavra));
    TipoVetoresBO vetoresBaseOffset =
        (TipoVetoresBO)calloc(MAXTAMVETORESDO + 1, sizeof(TipoBaseOffset));
    TipoPalavra p;
    TipoTexto T;
    memset(T, 0, sizeof T);

    int n = 1;

    FILE *arq_comprimido = fopen(ARQ_COMP_PATH, "r+b");

    if (arq_comprimido == NULL) {
      printf("Erro ao abrir arquivo comprimido\n");
      exit(1);
    }

    FILE *arq_alfabeto = fopen(ARQ_ALF_PATH, "r");

    DefineAlfabeto(alfabeto, arq_alfabeto); /*Le alfabeto definido em arquivo*/
    maxCompCod = LeVetores(arq_comprimido, vetoresBaseOffset);
    numNodosFolhas = LeVocabulario(arq_comprimido, vocabulario);

    while (fread(&T[n], sizeof(char), 1, arq_comprimido))
      n++;

    // Inicia o temporizador para medir o tempo total de execução
    Temporizador tempo_total;
    iniciarTemporizador(&tempo_total);
    int qtdPadroes = 0;

    TipoPadrao padrao;

    FILE *output_comp_fp = fopen("output/teste_arq_comp", "w");

    while (fscanf(input_patterns_fp, "%s", padrao) == 1) {
      Temporizador tempo_teste;
      iniciarTemporizador(&tempo_teste);

      SolucaoCasamento *solucao =
          processar_padrao(padrao, vocabulario, numNodosFolhas,
                           vetoresBaseOffset, maxCompCod, T, n);
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

    free(vocabulario);
    free(vetoresBaseOffset);

    // Finaliza a medição do tempo total de execução
    finalizarTemporizador(&tempo_total);

    // Exibe o tempo total de execução do programa
    printf("Tempo total de execução:\n");
    imprimirTempos(&tempo_total);

    fclose(output_comp_fp);
    fclose(arq_alfabeto);
    fclose(arq_comprimido);
  } else {
    // Teste sem comprimir o texto
    TipoTexto texto;
    texto[0] = '\0';

    char linha[MAX_TAM_LINHA];

    while (fgets(linha, sizeof(linha), input_text_fp)) {
      strcat(texto, linha);
    }

    // Inicia o temporizador para medir o tempo total de execução
    Temporizador tempo_total;
    iniciarTemporizador(&tempo_total);

    int qtdPadroes = 0;

    TipoPadrao padrao;
    while (fscanf(input_patterns_fp, "%s", padrao) == 1) {
      Temporizador tempo_teste;
      iniciarTemporizador(&tempo_teste);

      SolucaoCasamento *solucao = buscar_arquivo_descomprimido(texto, padrao);

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
  }

  // Fechar todos os arquivos utilizados
  fclose(input_text_fp);
  fclose(input_patterns_fp);
  fclose(output_fp);

  return 0;
}