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
  TipoChave Chave;
  /* outros componentes */
  int Freq, Ordem;
} TipoItem;
typedef int TipoIndice;
typedef TipoItem *TipoDicionario;
typedef short TipoAlfabeto[MAXALFABETO + 1];
typedef struct TipoBaseOffset {
  int Base, Offset;
} TipoBaseOffset;
typedef TipoBaseOffset *TipoVetoresBO;
typedef char TipoPalavra[256];
typedef TipoPalavra *TipoVetorPalavra;

/* Inicio dos procedimentos do Extrator */
void DefineAlfabeto(TipoAlfabeto Alfabeto, FILE *ArqAlf);

/* Procedimento para gravar um numero inteiro em um arquivo de bytes */
void GravaNumInt(FILE *ArqComprimido, int Num);

/* Procedimento para ler um numero inteiro de um arquivo de bytes */
int LeNumInt(FILE *ArqComprimido);

int ConstroiVetores(TipoVetoresBO VetoresBaseOffset, TipoDicionario Vocabulario,
                    int n, FILE *ArqComprimido);

void Escreve(FILE *ArqComprimido, int *Codigo, int *c);

int Codifica(TipoVetoresBO VetoresBaseOffset, int Ordem, int *c,
             int MaxCompCod);

void Compressao(FILE *ArqTxt, FILE *ArqAlf, FILE *ArqComprimido);

int LeVetores(FILE *ArqComprimido, TipoBaseOffset *VetoresBaseOffset);

int LeVocabulario(FILE *ArqComprimido, TipoVetorPalavra Vocabulario);

void bmh_byte(TipoTexto T, int n, TipoPadrao P, int m,
              SolucaoCasamento *solucao);

void Atribui(TipoPadrao P, int Codigo, int c);

SolucaoCasamento *processar_padrao(TipoPadrao padrao,
                                   TipoVetorPalavra Vocabulario,
                                   int NumNodosFolhas,
                                   TipoVetoresBO VetoresBaseOffset,
                                   int MaxCompCod, TipoTexto T, int n);

void comprimir_arquivo_entrada(char *arq_input_name);

#endif