#include "../include/entrada.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/LSE.h"

int shift_and(char *padrao, char *texto, int num_erros,
              Lista *posicoes_encontradas) {
  int tamanho_texto = strlen(texto);
  int tamanho_padrao = strlen(padrao);

  unsigned int mascaras_bits[256];

  for (int i = 0; i < 256; i++) {
    mascaras_bits[i] = 0;
  }

  for (int i = 0; i < tamanho_padrao; i++) {
    unsigned char c = padrao[i];

    mascaras_bits[c] |= (1 << i);
  }

  unsigned int estados[num_erros + 1];
  unsigned int Rnovo[num_erros + 1];

  estados[0] = 0;
  for (int j = 1; j <= num_erros; j++) {
    estados[j] = (1 << j) - 1;
  }

  for (int i = 0; i < tamanho_texto; i++) {
    unsigned char c = texto[i];

    Rnovo[0] = ((estados[0] << 1) | 1) & mascaras_bits[c];

    for (int j = 1; j <= num_erros; j++) {
      Rnovo[j] = ((estados[j] << 1) | 1) & mascaras_bits[c];
      Rnovo[j] |= (estados[j - 1] << 1);
      Rnovo[j] |= estados[j - 1];
      Rnovo[j] |= Rnovo[j - 1];
    }

    for (int j = 0; j <= num_erros; j++) {
      estados[j] = Rnovo[j];
    }

    if (estados[num_erros] & (1 << (tamanho_padrao - 1))) {
      int posicao_encontrada = (i - tamanho_padrao + 1) + 1;

      insereFim(posicoes_encontradas, posicao_encontrada);
    }
  }

  return 0;
}

int temp(int argc, char *argv[]) {
  // Verifica se os argumentos de linha de comando são válidos
  if (!is_argumentos_validos(argc, argv)) {
    printf("Erro: Parâmetros inválidos.\n");
    printf("Uso correto: ./tp3_parte1 {estrateǵia} -i input/texto "
           "input/padroes -o saida.txt\n");
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

  int estrategia_escolhida = atoi(argv[ESTRATEGIA_P]);

  // TODO:Fazer a lógica principal do programa ...

  // Fechar todos os arquivos utilizados
  fclose(input_text_fp);
  fclose(input_patterns_fp);
  fclose(output_fp);

  return 0;
}




int main() {
  Lista *resposta = criaLista();
  shift_and("palavras",
            "Texto exemplo, texto tem palavras, palavras exercem fascínio.", 0,
            resposta);

  imprimeLista(resposta);

  Lista *resposta2 = criaLista();
  solve_dp("Texto exemplo, texto tem palavras, palavras exercem fascínio.",
           "palavras", 0, resposta2);

  imprimeLista(resposta2);

  destroiLista(resposta);
  destroiLista(resposta2);
}