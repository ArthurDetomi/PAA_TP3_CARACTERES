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
#define NUM_MIN_ARGS_ESPERADOS 6

// Define o maximo de argumentos que pode se passar para executar o programa
#define NUM_MAX_ARGS_ESPERADOS 8

enum argumento_posicao {
  ESTRATEGIA_P = 1,
  QTD_ERROS_P = 2,
  FLAG_INPUT_P = 3,
  ARQUIVO_ENTRADA_TEXTO_P = 4,
  ARQUIVO_ENTRADA_PADRAO_P = 5,
  FLAG_OUTPUT_P = 6,
  ARQUIVO_SAIDA_P = 7,
};

enum estrategias { PROGRAMACAO_DINAMICA = 1, SHIFT_AND = 2 };

// Verifica se os argumentos de linha de comando são válidos
bool is_argumentos_validos(int argc, char *argv[]);

// Obtem o arquivo de saída especificado nos argumentos do programa
void *get_output_path(int argc, char *argv[], char *output_path);

#endif