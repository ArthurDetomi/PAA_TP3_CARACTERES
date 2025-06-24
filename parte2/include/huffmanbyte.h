#ifndef HUFFMAN_BYTE
#define HUFFMAN_BYTE

#include "../include/resposta.h"

#include <stdio.h>

#define VAZIO "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
#define RETIRADO "***********************************"

#define M 67001
#define N 35            /* Tamanho da chave */
#define BASENUM 128     /* Base numerica que o algoritmo trabalha */
#define MAXALFABETO 255 /* Constante usada em ExtraiProximaPalavra */
#define MAXTAMVETORESDO 10
#define TRUE 1
#define FALSE 0
#define MAXTAMTEXTO 100000
#define MAXTAMPADRAO 100
#define MAXCHAR 256
#define ARQ_COMP_PATH "output/arq_comprimido"
#define ARQ_ALF_PATH "input/alfabeto.txt"

#define MAX_TEXTO_SIZE 100000

typedef char TipoTexto[MAXTAMTEXTO + 1];
typedef char TipoPadrao[MAXTAMPADRAO + 1];

typedef int TipoApontador;
typedef char TipoChave[N + 1];
typedef int TipoPesos[N + 1];

typedef struct TipoItem {
    TipoChave chave;
    int freq, ordem;
} TipoItem;

typedef int TipoIndice;
typedef TipoItem *TipoDicionario;
typedef short TipoAlfabeto[MAXALFABETO + 1];

typedef struct TipoBaseOffset {
    int base, offset;
} TipoBaseOffset;

typedef TipoBaseOffset *TipoVetoresBO;
typedef char TipoPalavra[256];
typedef TipoPalavra *TipoVetorPalavra;

/* Inicio dos procedimentos do Extrator */
void define_alfabeto(TipoAlfabeto alfabeto, FILE *arqAlf);

/* Procedimento para gravar um numero inteiro em um arquivo de bytes */
void grava_num_int(FILE *arqComprimido, int num);

/* Procedimento para ler um numero inteiro de um arquivo de bytes */
int le_num_int(FILE *arqComprimido);

int constroi_vetores(TipoVetoresBO vetoresBaseOffset, TipoDicionario vocabulario,
                     int n, FILE *arqComprimido);

void escreve(FILE *ArqComprimido, int *Codigo, int *c);

int codifica(TipoVetoresBO vetoresBaseOffset, int ordem, int *c,
             int maxCompCod);

void compressao(FILE *arqTxt, FILE *arqAlf, FILE *arqComprimido);

int le_vetores(FILE *arqComprimido, TipoBaseOffset *vetoresBaseOffset);

int le_vocabulario(FILE *arqComprimido, TipoVetorPalavra vocabulario);

void bmh_byte(TipoTexto T, int n, TipoPadrao P, int m,
              SolucaoCasamento *solucao);

void atribui(TipoPadrao P, int Codigo, int c);

SolucaoCasamento *processar_padrao(TipoPadrao padrao,
                                   TipoVetorPalavra vocabulario,
                                   int NumNodosFolhas,
                                   TipoVetoresBO vetoresBaseOffset,
                                   int MaxCompCod, TipoTexto T, int n);

void comprimir_arquivo_entrada(char *arq_input_name);

#endif
