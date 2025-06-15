#ifndef STRATEGYDP
#define STRATEGYDP

#include "../include/LSE.h"

#include "../include/resposta.h"

SolucaoCasamento *encontrar_casamento_aproximados_dp(char *texto, char *padrao,
                                                     int num_erros);

#endif