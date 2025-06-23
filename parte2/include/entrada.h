#ifndef ENTRADA_H
#define ENTRADA_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// Define a flag esperada para especificar o arquivo de entrada
#define FLAG_INPUT "-i"

// Define a flag esperada para especificar o arquivo de saída
#define FLAG_OUTPUT "-o"

// Define o minimo de argumentos que deve se passar para executar o programa
#define NUM_MIN_ARGS_ESPERADOS 5

// Define o maximo de argumentos que pode se passar para executar o programa
#define NUM_MAX_ARGS_ESPERADOS 7

enum argumento_posicao {
  FLAG_OPTION_P = 1,
  FLAG_INPUT_P = 2,
  ARQUIVO_ENTRADA_TEXTO_P = 3,
  ARQUIVO_ENTRADA_PADRAO_P = 4,
  FLAG_OUTPUT_P = 5,
  ARQUIVO_SAIDA_P = 6,
};

enum options { ARQ_COMPRIMIDO_TEST = 1, ARQ_DESCOMPRIMIDO_TEST = 2 };

// Verifica se os argumentos de linha de comando são válidos
bool is_argumentos_validos(int argc, char *argv[]);

// Obtem o arquivo de saída especificado nos argumentos do programa
void *get_output_path(int argc, char *argv[], char *output_path);

#endif