#include "../include/entrada.h"

#include <stdbool.h>

#include <stdlib.h>
#include <string.h>

bool is_argumentos_validos(int argc, char *argv[]) {
  // A entrada do programa deve possuir 5 argumentos ou 7 argumentos
  if (argc != NUM_MIN_ARGS_ESPERADOS && argc != NUM_MAX_ARGS_ESPERADOS) {
    return false;
  }

  int estrategia = (int)atoi(argv[ESTRATEGIA_P]);

  if (estrategia != PROGRAMACAO_DINAMICA && estrategia != SHIFT_AND) {
    return false;
  }

  // Se não possui a flag -i como argumento considera-se invalido
  if (strcmp(argv[FLAG_INPUT_P], FLAG_INPUT) != 0) {
    return false;
  }

  /*
    Se possui o número minimo de argumentos a validação já feita é suficiente
    portanto é válido os argumentos
  */
  if (argc == NUM_MIN_ARGS_ESPERADOS) {
    return true;
  }

  /*
   Caso tenha 7 argumentos o argumento de indice 5 deve ser obrigatoriamente o
   -o
  */
  return strcmp(argv[FLAG_OUTPUT_P], FLAG_OUTPUT) == 0;
}

void *get_output_path(int argc, char *argv[], char *output_path) {
  strcpy(output_path, "output/");
  strcat(output_path, (NUM_MAX_ARGS_ESPERADOS == argc) ? argv[ARQUIVO_SAIDA_P]
                                                       : "output.txt");

  return output_path;
}