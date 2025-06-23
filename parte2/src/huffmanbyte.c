#include "../include/huffmanbyte.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void GeraPesos(TipoPesos p) {
  int i;
  for (i = 0; i < N; i++)
    p[i] = 1 + (unsigned int)rand();
}

TipoIndice h(TipoChave Chave, TipoPesos p) {
  int i;
  unsigned int Soma = 0;
  for (i = 0; i < strlen(Chave); i++)
    Soma += (unsigned int)Chave[i] * p[i];
  return (Soma % M);
}

void Inicializa(TipoDicionario T) {
  int i;
  for (i = 0; i < M; i++) {
    memcpy(T[i].Chave, VAZIO, N + 1);
    T[i].Freq = 0;
  }
}

TipoApontador Pesquisa(TipoChave Ch, TipoPesos p, TipoDicionario T) {
  unsigned int i = 0, Inicial = h(Ch, p);
  while (strcmp(T[(Inicial + i) % M].Chave, VAZIO) != 0 &&
         strcmp(T[(Inicial + i) % M].Chave, Ch) != 0 && i < M)
    i++;
  if (strcmp(T[(Inicial + i) % M].Chave, Ch) == 0)
    return ((Inicial + i) % M);
  else
    return M; /* Pesquisa sem sucesso */
}

void Insere(TipoItem *x, TipoPesos p, TipoDicionario T) {
  unsigned int i = 0, Inicial;
  if (Pesquisa(x->Chave, p, T) < M) {
    printf("Elemento ja esta presente\n");
    return;
  }
  Inicial = h(x->Chave, p);
  while (strcmp(T[(Inicial + i) % M].Chave, VAZIO) != 0 &&
         strcmp(T[(Inicial + i) % M].Chave, RETIRADO) != 0 && i < M)
    i++;
  if (i < M) {
    strcpy(T[(Inicial + i) % M].Chave, x->Chave);
    T[(Inicial + i) % M].Freq = x->Freq;
    T[(Inicial + i) % M].Ordem = x->Ordem;
  } else
    printf(" Tabela cheia\n");
}

/* Inicio dos procedimentos do Extrator */
void DefineAlfabeto(TipoAlfabeto Alfabeto, FILE *ArqAlf) {
  char Simbolos[MAXALFABETO + 1];
  int i;
  char *Temp;
  for (i = 0; i <= MAXALFABETO; i++)
    Alfabeto[i] = FALSE;
  fgets(Simbolos, MAXALFABETO + 1, ArqAlf);
  Temp = strchr(Simbolos, '\n');
  if (Temp != NULL)
    *Temp = 0;
  for (i = 0; i <= strlen(Simbolos) - 1; i++)
    Alfabeto[Simbolos[i] + 127] = TRUE;
  Alfabeto[0] = FALSE; /* caractere de codigo zero: separador */
}

void ExtraiProximaPalavra(TipoPalavra Result, int *TipoIndice, char *Linha,
                          FILE *ArqTxt, TipoAlfabeto Alfabeto) {
  short FimPalavra = FALSE, Aux = FALSE;
  Result[0] = '\0';
  if (*TipoIndice > strlen(Linha)) {
    if (fgets(Linha, MAXALFABETO + 1,
              ArqTxt)) { /* Coloca um delimitador em Linha */
      sprintf(Linha + strlen(Linha), "%c", (char)0);
      *TipoIndice = 1;
    } else {
      sprintf(Linha, "%c", (char)0);
      FimPalavra = TRUE;
    }
  }
  while (*TipoIndice <= strlen(Linha) && !FimPalavra) {
    if (Alfabeto[Linha[*TipoIndice - 1] + 127]) {
      sprintf(Result + strlen(Result), "%c", Linha[*TipoIndice - 1]);
      Aux = TRUE;
    } else {
      if (Aux) {
        if (Linha[*TipoIndice - 1] != (char)0)
          (*TipoIndice)--;
      } else {
        sprintf(Result + strlen(Result), "%c", Linha[*TipoIndice - 1]);
      }
      FimPalavra = TRUE;
    }
    (*TipoIndice)++;
  }
}

char *Trim(char *str) {
  int i = 0, j, len;
  char *strtmp = malloc(sizeof(char) * strlen(str) + 1);
  strcpy(strtmp, str);
  len = strlen(strtmp);
  while ((i < len) && ((strtmp[i] == ' ') || (strtmp[i] == '\t') ||
                       /*(strtmp[i] == '\n') ||*/ (strtmp[i] == '\r')))
    i++;
  j = len - 1;
  while ((j >= 0) && ((strtmp[j] == ' ') || (strtmp[j] == '\t') ||
                      /*(strtmp[i] == '\n') ||*/ (strtmp[i] == '\r')))
    j--;

  if (j >= 0)
    str[j + 1] = '\0';
  if (i <= j) {
    memmove(strtmp, strtmp + i, strlen(strtmp + i) + 1);
    // strcpy(strtmp, strtmp + i);
  } else
    strcpy(strtmp, "");
  return strtmp;
}

/* Procedimentos da Compressao e Descompressao */
void PrimeiraEtapa(FILE *ArqTxt, TipoAlfabeto Alfabeto, int *TipoIndice,
                   TipoPalavra Palavra, char *Linha, TipoDicionario Vocabulario,
                   TipoPesos p) {
  TipoItem Elemento;
  int i;
  char *PalavraTrim = NULL;
  do {
    ExtraiProximaPalavra(Palavra, TipoIndice, Linha, ArqTxt, Alfabeto);
    memcpy(Elemento.Chave, Palavra, sizeof(TipoChave));
    Elemento.Freq = 1;
    if (*Palavra != '\0') {
      i = Pesquisa(Elemento.Chave, p, Vocabulario);
      if (i < M)
        Vocabulario[i].Freq++;
      else
        Insere(&Elemento, p, Vocabulario);
      do {
        ExtraiProximaPalavra(Palavra, TipoIndice, Linha, ArqTxt, Alfabeto);
        memcpy(Elemento.Chave, Palavra, sizeof(TipoChave));
        /* O primeiro espaco depois da palavra nao e codificado */
        if (PalavraTrim != NULL)
          free(PalavraTrim);
        PalavraTrim = Trim(Palavra);
        if (strcmp(PalavraTrim, "") && (*PalavraTrim) != (char)0) {
          i = Pesquisa(Elemento.Chave, p, Vocabulario);
          if (i < M)
            Vocabulario[i].Freq++;
          else
            Insere(&Elemento, p, Vocabulario);
        }
      } while (strcmp(Palavra, ""));
      if (PalavraTrim != NULL)
        free(PalavraTrim);
    }
  } while (Palavra[0] != '\0');
}

void CalculaCompCodigo(TipoDicionario A, int n) {
  int u = 0; /* Nodos internos usados */
  int h = 0; /* Altura da arvore */
  int NoInt; /* Numero de nodos internos */
  int Prox, Raiz, Folha;
  int Disp = 1;
  int x, Resto;
  if (n > BASENUM - 1) {
    Resto = 1 + ((n - BASENUM) % (BASENUM - 1));
    if (Resto < 2)
      Resto = BASENUM;
  } else
    Resto = n - 1;
  NoInt = 1 + ((n - Resto) / (BASENUM - 1));
  for (x = n - 1; x >= (n - Resto) + 1; x--)
    A[n].Freq += A[x].Freq;
  /* Primeira Fase */
  Raiz = n;
  Folha = n - Resto;
  for (Prox = n - 1; Prox >= (n - NoInt) + 1; Prox--) { /* Procura Posicao */
    if ((Folha < 1) ||
        ((Raiz > Prox) && (A[Raiz].Freq <= A[Folha].Freq))) { /* No interno */
      A[Prox].Freq = A[Raiz].Freq;
      A[Raiz].Freq = Prox;
      Raiz--;
    } else { /* No-folha */
      A[Prox].Freq = A[Folha].Freq;
      Folha--;
    }
    /* Atualiza Frequencias */
    for (x = 1; x <= BASENUM - 1; x++) {
      if ((Folha < 1) ||
          ((Raiz > Prox) && (A[Raiz].Freq <= A[Folha].Freq))) { /* No interno */
        A[Prox].Freq += A[Raiz].Freq;
        A[Raiz].Freq = Prox;
        Raiz--;
      } else { /* No-folha */
        A[Prox].Freq += A[Folha].Freq;
        Folha--;
      }
    }
  }
  /* Segunda Fase */
  A[Raiz].Freq = 0;
  for (Prox = Raiz + 1; Prox <= n; Prox++)
    A[Prox].Freq = A[A[Prox].Freq].Freq + 1;
  /* Terceira Fase */
  Prox = 1;
  while (Disp > 0) {
    while (Raiz <= n && A[Raiz].Freq == h) {
      u++;
      Raiz++;
    }
    while (Disp > u) {
      A[Prox].Freq = h;
      Prox++;
      Disp--;
      if (Prox > n) {
        u = 0;
        break;
      }
    }
    Disp = BASENUM * u;
    h++;
    u = 0;
  }
}

void Particao(TipoIndice Esq, TipoIndice Dir, TipoIndice *i, TipoIndice *j,
              TipoDicionario A) {
  TipoItem x, w;
  *i = Esq;
  *j = Dir;
  x = A[(*i + *j) / 2]; /* obtem o pivo x */
  do {
    while (x.Freq < A[*i].Freq)
      (*i)++;
    while (x.Freq > A[*j].Freq)
      (*j)--;
    if (*i <= *j) {
      w = A[*i];
      A[*i] = A[*j];
      A[*j] = w;
      (*i)++;
      (*j)--;
    }
  } while (*i <= *j);
}

void Ordena(TipoIndice Esq, TipoIndice Dir, TipoDicionario A) {
  TipoIndice i, j;
  Particao(Esq, Dir, &i, &j, A);
  if (Esq < j)
    Ordena(Esq, j, A);
  if (i < Dir)
    Ordena(i, Dir, A);
}

void QuickSort(TipoDicionario A, TipoIndice *n) { Ordena(1, *n, A); }

TipoIndice OrdenaPorFrequencia(TipoDicionario Vocabulario) {
  TipoIndice i;
  TipoIndice n = 1;
  TipoItem Item;
  Item = Vocabulario[1];
  for (i = 0; i <= M - 1; i++) {
    if (strcmp(Vocabulario[i].Chave, VAZIO)) {
      if (i != 1) {
        Vocabulario[n] = Vocabulario[i];
        n++;
      }
    }
  }
  if (strcmp(Item.Chave, VAZIO))
    Vocabulario[n] = Item;
  else
    n--;
  QuickSort(Vocabulario, &n);
  return n;
}

/* Procedimento para gravar um numero inteiro em um arquivo de bytes */
void GravaNumInt(FILE *ArqComprimido, int Num) {
  fwrite(&Num, sizeof(int), 1, ArqComprimido);
}

/* Procedimento para ler um numero inteiro de um arquivo de bytes */
int LeNumInt(FILE *ArqComprimido) {
  int Num;
  fread(&Num, sizeof(int), 1, ArqComprimido);
  return Num;
}

int ConstroiVetores(TipoVetoresBO VetoresBaseOffset, TipoDicionario Vocabulario,
                    int n, FILE *ArqComprimido) {
  int Wcs[MAXTAMVETORESDO + 1];
  int i, MaxCompCod;
  MaxCompCod = Vocabulario[n].Freq;
  for (i = 1; i <= MaxCompCod; i++) {
    Wcs[i] = 0;
    VetoresBaseOffset[i].Offset = 0;
  }
  for (i = 1; i <= n; i++) {
    Wcs[Vocabulario[i].Freq]++;
    VetoresBaseOffset[Vocabulario[i].Freq].Offset =
        i - Wcs[Vocabulario[i].Freq] + 1;
  }
  VetoresBaseOffset[1].Base = 0;
  for (i = 2; i <= MaxCompCod; i++) {
    VetoresBaseOffset[i].Base =
        BASENUM * (VetoresBaseOffset[i - 1].Base + Wcs[i - 1]);
    if (VetoresBaseOffset[i].Offset == 0)
      VetoresBaseOffset[i].Offset = VetoresBaseOffset[i - 1].Offset;
  }
  /* Salvando as tabelas em disco */
  GravaNumInt(ArqComprimido, MaxCompCod);
  for (i = 1; i <= MaxCompCod; i++) {
    GravaNumInt(ArqComprimido, VetoresBaseOffset[i].Base);
    GravaNumInt(ArqComprimido, VetoresBaseOffset[i].Offset);
  }
  return MaxCompCod;
}

int SegundaEtapa(TipoDicionario Vocabulario, TipoVetoresBO VetoresBaseOffset,
                 TipoPesos p, FILE *ArqComprimido) {
  int Result, i, j, NumNodosFolhas, PosArq;
  TipoItem Elemento;
  char Ch;
  TipoPalavra Palavra;
  NumNodosFolhas = OrdenaPorFrequencia(Vocabulario);
  CalculaCompCodigo(Vocabulario, NumNodosFolhas);
  Result = ConstroiVetores(VetoresBaseOffset, Vocabulario, NumNodosFolhas,
                           ArqComprimido);
  /* Grava Vocabulario */
  GravaNumInt(ArqComprimido, NumNodosFolhas);
  PosArq = ftell(ArqComprimido);
  for (i = 1; i <= NumNodosFolhas; i++) {
    j = strlen(Vocabulario[i].Chave);
    fwrite(Vocabulario[i].Chave, sizeof(char), j + 1, ArqComprimido);
  }
  /* Le e reconstroi a condicao de hash no vetor que contem o vocabulario */
  fseek(ArqComprimido, PosArq, SEEK_SET);
  Inicializa(Vocabulario);
  for (i = 1; i <= NumNodosFolhas; i++) {
    *Palavra = '\0';
    do {
      fread(&Ch, sizeof(char), 1, ArqComprimido);
      if (Ch != (char)0)
        sprintf(Palavra + strlen(Palavra), "%c", Ch);
    } while (Ch != (char)0);
    memcpy(Elemento.Chave, Palavra, sizeof(TipoChave));
    Elemento.Ordem = i;
    j = Pesquisa(Elemento.Chave, p, Vocabulario);
    if (j >= M)
      Insere(&Elemento, p, Vocabulario);
  }
  return Result;
}

void Escreve(FILE *ArqComprimido, int *Codigo, int *c) {
  unsigned char Saida[MAXTAMVETORESDO + 1];
  int i = 1, cTmp;
  int LogBase2 = (int)round(log(BASENUM) / log(2.0));
  int Mask = (int)pow(2, LogBase2) - 1;
  cTmp = *c;
  Saida[i] = ((unsigned)(*Codigo)) >> ((*c - 1) * LogBase2);
  if (LogBase2 == 7)
    Saida[i] = Saida[i] | 0x80;
  i++;
  (*c)--;
  while (*c > 0) {
    Saida[i] = (((unsigned)(*Codigo)) >> ((*c - 1) * LogBase2)) & Mask;
    i++;
    (*c)--;
  }
  for (i = 1; i <= cTmp; i++)
    fwrite(&Saida[i], sizeof(unsigned char), 1, ArqComprimido);
}

int Codifica(TipoVetoresBO VetoresBaseOffset, int Ordem, int *c,
             int MaxCompCod) {
  *c = 1;
  while (Ordem >= VetoresBaseOffset[*c + 1].Offset && *c + 1 <= MaxCompCod)
    (*c)++;
  return (Ordem - VetoresBaseOffset[*c].Offset + VetoresBaseOffset[*c].Base);
}

void TerceiraEtapa(FILE *ArqTxt, TipoAlfabeto Alfabeto, int *TipoIndice,
                   TipoPalavra Palavra, char *Linha, TipoDicionario Vocabulario,
                   TipoPesos p, TipoVetoresBO VetoresBaseOffset,
                   FILE *ArqComprimido, int MaxCompCod) {
  TipoApontador Pos;
  TipoChave Chave;
  char *PalavraTrim = NULL;
  int Codigo, c;
  do {
    ExtraiProximaPalavra(Palavra, TipoIndice, Linha, ArqTxt, Alfabeto);
    memcpy(Chave, Palavra, sizeof(TipoChave));
    if (*Palavra != '\0') {
      Pos = Pesquisa(Chave, p, Vocabulario);
      Codigo =
          Codifica(VetoresBaseOffset, Vocabulario[Pos].Ordem, &c, MaxCompCod);
      Escreve(ArqComprimido, &Codigo, &c);
      do {
        ExtraiProximaPalavra(Palavra, TipoIndice, Linha, ArqTxt, Alfabeto);
        /* O primeiro espaco depois da palavra nao e codificado */
        if (PalavraTrim != NULL) {
          free(PalavraTrim);
          PalavraTrim = NULL;
        }

        PalavraTrim = Trim(Palavra);

        if (strcmp(PalavraTrim, "") && (*PalavraTrim) != (char)0) {
          memcpy(Chave, Palavra, sizeof(TipoChave));
          Pos = Pesquisa(Chave, p, Vocabulario);
          Codigo = Codifica(VetoresBaseOffset, Vocabulario[Pos].Ordem, &c,
                            MaxCompCod);
          Escreve(ArqComprimido, &Codigo, &c);
        }
      } while (strcmp(Palavra, ""));
    }
  } while (*Palavra != '\0');

  if (PalavraTrim != NULL) {
    free(PalavraTrim);
    PalavraTrim = NULL;
  }
}

void Compressao(FILE *ArqTxt, FILE *ArqAlf, FILE *ArqComprimido) {
  TipoAlfabeto Alfabeto;
  TipoPalavra Palavra, Linha;
  int Ind = 1, MaxCompCod;
  TipoPesos p;
  TipoDicionario Vocabulario = (TipoDicionario)calloc(M + 1, sizeof(TipoItem));
  TipoVetoresBO VetoresBaseOffset =
      (TipoVetoresBO)calloc(MAXTAMVETORESDO + 1, sizeof(TipoBaseOffset));
  /* Inicializacao do Alfabeto */
  DefineAlfabeto(Alfabeto, ArqAlf); /* Le o alfabeto definido em arquivo */
  *Linha = '\0';
  Inicializa(Vocabulario);
  GeraPesos(p);
  PrimeiraEtapa(ArqTxt, Alfabeto, &Ind, Palavra, Linha, Vocabulario, p);
  MaxCompCod = SegundaEtapa(Vocabulario, VetoresBaseOffset, p, ArqComprimido);
  fseek(ArqTxt, 0, SEEK_SET); /* Move cursor para inicio do arquivo*/
  Ind = 1;
  *Linha = '\0';
  TerceiraEtapa(ArqTxt, Alfabeto, &Ind, Palavra, Linha, Vocabulario, p,
                VetoresBaseOffset, ArqComprimido, MaxCompCod);
  free(Vocabulario);
  free(VetoresBaseOffset);
}

/*Procedimentos da Descompressao*/
int LeVetores(FILE *ArqComprimido, TipoBaseOffset *VetoresBaseOffset) {
  int MaxCompCod, i;
  MaxCompCod = LeNumInt(ArqComprimido);
  for (i = 1; i <= MaxCompCod; i++) {
    VetoresBaseOffset[i].Base = LeNumInt(ArqComprimido);
    VetoresBaseOffset[i].Offset = LeNumInt(ArqComprimido);
  }
  return MaxCompCod;
}

int LeVocabulario(FILE *ArqComprimido, TipoVetorPalavra Vocabulario) {
  int NumNodosFolhas, i;
  TipoPalavra Palavra;
  char Ch;
  NumNodosFolhas = LeNumInt(ArqComprimido);
  for (i = 1; i <= NumNodosFolhas; i++) {
    *Palavra = '\0';
    do {
      fread(&Ch, sizeof(unsigned char), 1, ArqComprimido);
      if (Ch != (char)0) /* As palavras estao separadas pelo caratere 0 */
        sprintf(Palavra + strlen(Palavra), "%c", Ch);
    } while (Ch != (char)0);
    strcpy(Vocabulario[i], Palavra);
  }
  return NumNodosFolhas;
}

void bmh_byte(TipoTexto T, int n, TipoPadrao P, int m,
              SolucaoCasamento *solucao) {
  int i, j, k, d[MAXCHAR + 1];
  for (j = 0; j <= MAXCHAR; j++) {
    d[j] = m;
  }

  for (j = 1; j <= m - 1; j++) {
    d[P[j] + 128] = m - j;
  }

  i = m;
  while (i <= n) {
    k = i;
    j = m;
    while (j >= 0) {
      solucao->qtd_comparacoes++;

      if (T[i + j] != P[j]) {
        break;
      }

      j--;
    }
    if (j == 0) {
      insereFim(solucao->posicoesEncontradas, k + 1);
    }
    i += d[T[i] + 128];
  }
}

void Atribui(TipoPadrao P, int Codigo, int c) {
  int i = 1;
  P[i] = (char)((Codigo >> ((c - 1) * 7)) | 0x80);
  i++;
  c--;
  while (c > 0) {
    P[i] = (char)((Codigo >> ((c - 1) * 7)) & 127);
    i++;
    c--;
  }
}

SolucaoCasamento *processar_padrao(TipoPadrao padrao,
                                   TipoVetorPalavra Vocabulario,
                                   int NumNodosFolhas,
                                   TipoVetoresBO VetoresBaseOffset,
                                   int MaxCompCod, TipoTexto T, int n) {
  SolucaoCasamento *solucao = alocar_solucao_casamento(padrao);

  int Ind, Ord, c, Codigo;

  for (Ind = 1; Ind <= NumNodosFolhas; Ind++) {
    if (!strcmp(Vocabulario[Ind], padrao)) {
      Ord = Ind;
      break;
    }
  }

  if (Ind == NumNodosFolhas + 1) {
    printf("Padrao:%s nao encontrado\n", padrao);
    return solucao;
  }

  Codigo = Codifica(VetoresBaseOffset, Ord, &c, MaxCompCod);
  Atribui(padrao, Codigo, c);
  bmh_byte(T, n, padrao, c, solucao);

  return solucao;
}

void comprimir_arquivo_entrada(char *arq_input_name) {
  FILE *arq_comprimido = fopen(ARQ_COMP_PATH, "w+b");
  FILE *arq_alfabeto = fopen(ARQ_ALF_PATH, "r");
  FILE *input_text_fp = fopen(arq_input_name, "r");

  if (arq_comprimido == NULL || arq_alfabeto == NULL) {
    perror("Erro ao abrir arquivos de dependência\n");
    exit(1);
  }

  Compressao(input_text_fp, arq_alfabeto, arq_comprimido);

  fclose(arq_comprimido);
  arq_comprimido = NULL;
  fclose(arq_alfabeto);
  arq_alfabeto = NULL;
  fclose(input_text_fp);
  input_text_fp = NULL;
}