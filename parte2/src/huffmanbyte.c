#include "../include/huffmanbyte.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Gera pesos aleatórios para cada posição de caracter
void gera_pesos(TipoPesos p) {
  int i;
  for (i = 0; i < N; i++)
    p[i] = 1 + (unsigned int)rand();
}

TipoIndice h(TipoChave chave, TipoPesos p) {
  int i;
  unsigned int soma = 0;
  for (i = 0; i < strlen(chave); i++)
    soma += (unsigned int)chave[i] * p[i];
  return (soma % M);
}

// Inicializa a tabela hash
void inicializa(TipoDicionario T) {
  int i;
  for (i = 0; i < M; i++) {
    memcpy(T[i].chave, VAZIO, N + 1);
    T[i].freq = 0;
  }
}

// Retorna a posição se encontrar, ou M se a chave não existir na tabela.
TipoApontador pesquisa(TipoChave chave, TipoPesos p, TipoDicionario T) {
  unsigned int i = 0, inicial = h(chave, p);
  while (strcmp(T[(inicial + i) % M].chave, VAZIO) != 0 &&
         strcmp(T[(inicial + i) % M].chave, chave) != 0 && i < M)
    i++;
  if (strcmp(T[(inicial + i) % M].chave, chave) == 0)
    return ((inicial + i) % M);

  /* Pesquisa sem sucesso */
  return M;
}

void insere(TipoItem *x, TipoPesos p, TipoDicionario T) {
  unsigned int i = 0;
  if (pesquisa(x->chave, p, T) < M) {
    printf("Elemento ja esta presente\n");
    return;
  }
  unsigned int inicial = h(x->chave, p);
  while (strcmp(T[(inicial + i) % M].chave, VAZIO) != 0 &&
         strcmp(T[(inicial + i) % M].chave, RETIRADO) != 0 && i < M)
    i++;
  if (i < M) {
    strcpy(T[(inicial + i) % M].chave, x->chave);
    T[(inicial + i) % M].freq = x->freq;
    T[(inicial + i) % M].ordem = x->ordem;
  } else
    printf(" Tabela cheia\n");
}

// Marca quais caracteres são considerados válidos para formar palavras.
void define_alfabeto(TipoAlfabeto alfabeto, FILE *arqAlf) {
  char simbolos[MAXALFABETO + 1];
  int i;
  char *temp;
  for (i = 0; i <= MAXALFABETO; i++)
    alfabeto[i] = FALSE;
  fgets(simbolos, MAXALFABETO + 1, arqAlf);
  temp = strchr(simbolos, '\n');
  if (temp != NULL)
    *temp = 0;
  for (i = 0; i <= strlen(simbolos) - 1; i++)
    alfabeto[simbolos[i] + 127] = TRUE;
  alfabeto[0] = FALSE; /* caractere de codigo zero: separador */
}

// Extrai próxima palavra da linha atual. Se linha acabar, lê outra do arquivo.
// Usa a tabela do alfabeto para identificar separadores.
void extrai_proxima_palavra(TipoPalavra result, int *tipo_indice, char *linha,
                            FILE *arq_txt, TipoAlfabeto alfabeto) {
  short fim_palavra = FALSE, aux = FALSE;
  result[0] = '\0';
  if (*tipo_indice > strlen(linha)) {
    if (fgets(linha, MAXALFABETO + 1,
              arq_txt)) { /* Coloca um delimitador em Linha */
      sprintf(linha + strlen(linha), "%c", (char)0);
      *tipo_indice = 1;
    } else {
      sprintf(linha, "%c", (char)0);
      fim_palavra = TRUE;
    }
  }
  while (*tipo_indice <= strlen(linha) && !fim_palavra) {
    if (alfabeto[linha[*tipo_indice - 1] + 127]) {
      sprintf(result + strlen(result), "%c", linha[*tipo_indice - 1]);
      aux = TRUE;
    } else {
      if (aux) {
        if (linha[*tipo_indice - 1] != (char)0)
          (*tipo_indice)--;
      } else {
        sprintf(result + strlen(result), "%c", linha[*tipo_indice - 1]);
      }
      fim_palavra = TRUE;
    }
    (*tipo_indice)++;
  }
}

char *trim(char *str) {
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
void primeira_etapa(FILE *ArqTxt, TipoAlfabeto Alfabeto, int *TipoIndice,
                    TipoPalavra Palavra, char *Linha,
                    TipoDicionario Vocabulario, TipoPesos p) {
  TipoItem Elemento;
  int i;
  char *PalavraTrim = NULL;
  do {
    extrai_proxima_palavra(Palavra, TipoIndice, Linha, ArqTxt, Alfabeto);
    memcpy(Elemento.chave, Palavra, sizeof(TipoChave));
    Elemento.freq = 1;
    if (*Palavra != '\0') {
      i = pesquisa(Elemento.chave, p, Vocabulario);
      if (i < M)
        Vocabulario[i].freq++;
      else
        insere(&Elemento, p, Vocabulario);
      do {
        extrai_proxima_palavra(Palavra, TipoIndice, Linha, ArqTxt, Alfabeto);
        memcpy(Elemento.chave, Palavra, sizeof(TipoChave));
        /* O primeiro espaco depois da palavra nao e codificado */
        if (PalavraTrim != NULL)
          free(PalavraTrim);
        PalavraTrim = trim(Palavra);
        if (strcmp(PalavraTrim, "") && (*PalavraTrim) != (char)0) {
          i = pesquisa(Elemento.chave, p, Vocabulario);
          if (i < M)
            Vocabulario[i].freq++;
          else
            insere(&Elemento, p, Vocabulario);
        }
      } while (strcmp(Palavra, ""));
      if (PalavraTrim != NULL)
        free(PalavraTrim);
    }
  } while (Palavra[0] != '\0');
}

/*
  Constrói a árvore de Huffman generalizada.

  Fase 1: cria nodos internos somando frequências.
  Fase 2: calcula profundidade da árvore.
  Fase 3: define comprimento dos códigos com base na profundidade.
*/
void calcula_comp_codigo(TipoDicionario A, int n) {
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
    A[n].freq += A[x].freq;
  /* Primeira Fase */
  Raiz = n;
  Folha = n - Resto;
  for (Prox = n - 1; Prox >= (n - NoInt) + 1; Prox--) { /* Procura Posicao */
    if ((Folha < 1) ||
        ((Raiz > Prox) && (A[Raiz].freq <= A[Folha].freq))) { /* No interno */
      A[Prox].freq = A[Raiz].freq;
      A[Raiz].freq = Prox;
      Raiz--;
    } else { /* No-folha */
      A[Prox].freq = A[Folha].freq;
      Folha--;
    }
    /* Atualiza Frequencias */
    for (x = 1; x <= BASENUM - 1; x++) {
      if ((Folha < 1) ||
          ((Raiz > Prox) && (A[Raiz].freq <= A[Folha].freq))) { /* No interno */
        A[Prox].freq += A[Raiz].freq;
        A[Raiz].freq = Prox;
        Raiz--;
      } else { /* No-folha */
        A[Prox].freq += A[Folha].freq;
        Folha--;
      }
    }
  }
  /* Segunda Fase */
  A[Raiz].freq = 0;
  for (Prox = Raiz + 1; Prox <= n; Prox++)
    A[Prox].freq = A[A[Prox].freq].freq + 1;
  /* Terceira Fase */
  Prox = 1;
  while (Disp > 0) {
    while (Raiz <= n && A[Raiz].freq == h) {
      u++;
      Raiz++;
    }
    while (Disp > u) {
      A[Prox].freq = h;
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

void particao(TipoIndice Esq, TipoIndice Dir, TipoIndice *i, TipoIndice *j,
              TipoDicionario A) {
  TipoItem x, w;
  *i = Esq;
  *j = Dir;
  x = A[(*i + *j) / 2]; /* obtem o pivo x */
  do {
    while (x.freq < A[*i].freq)
      (*i)++;
    while (x.freq > A[*j].freq)
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

void ordena(TipoIndice Esq, TipoIndice Dir, TipoDicionario A) {
  TipoIndice i, j;
  particao(Esq, Dir, &i, &j, A);
  if (Esq < j)
    ordena(Esq, j, A);
  if (i < Dir)
    ordena(i, Dir, A);
}

void QuickSort(TipoDicionario A, TipoIndice *n) { ordena(1, *n, A); }

TipoIndice ordena_por_frequencia(TipoDicionario vocabulario) {
  TipoIndice i;
  TipoIndice n = 1;
  TipoItem item;
  item = vocabulario[1];
  for (i = 0; i <= M - 1; i++) {
    if (strcmp(vocabulario[i].chave, VAZIO)) {
      if (i != 1) {
        vocabulario[n] = vocabulario[i];
        n++;
      }
    }
  }
  if (strcmp(item.chave, VAZIO))
    vocabulario[n] = item;
  else
    n--;
  QuickSort(vocabulario, &n);
  return n;
}

/* Procedimento para gravar um numero inteiro em um arquivo de bytes */
void grava_num_int(FILE *arqComprimido, int num) {
  fwrite(&num, sizeof(int), 1, arqComprimido);
}

/* Procedimento para ler um numero inteiro de um arquivo de bytes */
int le_num_int(FILE *arqComprimido) {
  int Num;
  fread(&Num, sizeof(int), 1, arqComprimido);
  return Num;
}

int constroi_vetores(TipoVetoresBO vetoresBaseOffset,
                     TipoDicionario vocabulario, int n, FILE *arqComprimido) {
  int Wcs[MAXTAMVETORESDO + 1];
  int i;
  int max_comp_cod = vocabulario[n].freq;
  for (i = 1; i <= max_comp_cod; i++) {
    Wcs[i] = 0;
    vetoresBaseOffset[i].offset = 0;
  }
  for (i = 1; i <= n; i++) {
    Wcs[vocabulario[i].freq]++;
    vetoresBaseOffset[vocabulario[i].freq].offset =
        i - Wcs[vocabulario[i].freq] + 1;
  }
  vetoresBaseOffset[1].base = 0;
  for (i = 2; i <= max_comp_cod; i++) {
    vetoresBaseOffset[i].base =
        BASENUM * (vetoresBaseOffset[i - 1].base + Wcs[i - 1]);
    if (vetoresBaseOffset[i].offset == 0)
      vetoresBaseOffset[i].offset = vetoresBaseOffset[i - 1].offset;
  }
  /* Salvando as tabelas em disco */
  grava_num_int(arqComprimido, max_comp_cod);
  for (i = 1; i <= max_comp_cod; i++) {
    grava_num_int(arqComprimido, vetoresBaseOffset[i].base);
    grava_num_int(arqComprimido, vetoresBaseOffset[i].offset);
  }
  return max_comp_cod;
}

/*Segunda etapa da compressão:
// Ordena o vocabulário por frequência, calcula os comprimentos de código,
// e grava o vocabulário e os vetores em disco.
// Em seguida, reconstroi a tabela de hash a partir do vocabulário serializado.
*/
int segunda_etapa(TipoDicionario Vocabulario, TipoVetoresBO VetoresBaseOffset,
                  TipoPesos p, FILE *ArqComprimido) {
  int Result, i, j, num_nodos_folhas, PosArq;
  TipoItem elemento;
  char Ch;
  TipoPalavra palavra;
  num_nodos_folhas = ordena_por_frequencia(Vocabulario);
  calcula_comp_codigo(Vocabulario, num_nodos_folhas);
  Result = constroi_vetores(VetoresBaseOffset, Vocabulario, num_nodos_folhas,
                            ArqComprimido);
  /* Grava Vocabulario */
  grava_num_int(ArqComprimido, num_nodos_folhas);
  PosArq = ftell(ArqComprimido);
  for (i = 1; i <= num_nodos_folhas; i++) {
    j = strlen(Vocabulario[i].chave);
    fwrite(Vocabulario[i].chave, sizeof(char), j + 1, ArqComprimido);
  }
  /* Le e reconstroi a condicao de hash no vetor que contem o vocabulario */
  fseek(ArqComprimido, PosArq, SEEK_SET);
  inicializa(Vocabulario);
  for (i = 1; i <= num_nodos_folhas; i++) {
    *palavra = '\0';
    do {
      fread(&Ch, sizeof(char), 1, ArqComprimido);
      if (Ch != (char)0)
        sprintf(palavra + strlen(palavra), "%c", Ch);
    } while (Ch != (char)0);
    memcpy(elemento.chave, palavra, sizeof(TipoChave));
    elemento.ordem = i;
    j = pesquisa(elemento.chave, p, Vocabulario);
    if (j >= M)
      insere(&elemento, p, Vocabulario);
  }
  return Result;
}

void escreve(FILE *ArqComprimido, int *Codigo, int *c) {
  unsigned char saida[MAXTAMVETORESDO + 1];
  int i = 1, cTmp;
  int log_base2 = (int)round(log(BASENUM) / log(2.0));
  int mask = (int)pow(2, log_base2) - 1;
  cTmp = *c;
  saida[i] = ((unsigned)(*Codigo)) >> ((*c - 1) * log_base2);
  if (log_base2 == 7)
    saida[i] = saida[i] | 0x80;
  i++;
  (*c)--;
  while (*c > 0) {
    saida[i] = (((unsigned)(*Codigo)) >> ((*c - 1) * log_base2)) & mask;
    i++;
    (*c)--;
  }
  for (i = 1; i <= cTmp; i++)
    fwrite(&saida[i], sizeof(unsigned char), 1, ArqComprimido);
}

int codifica(TipoVetoresBO vetoresBaseOffset, int ordem, int *c,
             int maxCompCod) {
  *c = 1;
  while (ordem >= vetoresBaseOffset[*c + 1].offset && *c + 1 <= maxCompCod)
    (*c)++;
  return (ordem - vetoresBaseOffset[*c].offset + vetoresBaseOffset[*c].base);
}

/*
  Terceira etapa da compressão:
  Lê o texto novamente e escreve os códigos compactados no arquivo.
  Usa o vocabulário e os vetores base-offset para converter palavras em
  códigos.
*/
void terceira_etapa(FILE *ArqTxt, TipoAlfabeto Alfabeto, int *TipoIndice,
                    TipoPalavra Palavra, char *Linha,
                    TipoDicionario Vocabulario, TipoPesos p,
                    TipoVetoresBO VetoresBaseOffset, FILE *ArqComprimido,
                    int MaxCompCod) {
  TipoApontador Pos;
  TipoChave Chave;
  char *PalavraTrim = NULL;
  int Codigo, c;
  do {
    extrai_proxima_palavra(Palavra, TipoIndice, Linha, ArqTxt, Alfabeto);
    memcpy(Chave, Palavra, sizeof(TipoChave));
    if (*Palavra != '\0') {
      Pos = pesquisa(Chave, p, Vocabulario);
      Codigo =
          codifica(VetoresBaseOffset, Vocabulario[Pos].ordem, &c, MaxCompCod);
      escreve(ArqComprimido, &Codigo, &c);
      do {
        extrai_proxima_palavra(Palavra, TipoIndice, Linha, ArqTxt, Alfabeto);
        /* O primeiro espaco depois da palavra nao e codificado */
        if (PalavraTrim != NULL) {
          free(PalavraTrim);
          PalavraTrim = NULL;
        }

        PalavraTrim = trim(Palavra);

        if (strcmp(PalavraTrim, "") && (*PalavraTrim) != (char)0) {
          memcpy(Chave, Palavra, sizeof(TipoChave));
          Pos = pesquisa(Chave, p, Vocabulario);
          Codigo = codifica(VetoresBaseOffset, Vocabulario[Pos].ordem, &c,
                            MaxCompCod);
          escreve(ArqComprimido, &Codigo, &c);
        }
      } while (strcmp(Palavra, ""));
    }
  } while (*Palavra != '\0');

  if (PalavraTrim != NULL) {
    free(PalavraTrim);
    PalavraTrim = NULL;
  }
}

/*
  Realiza a compressão como um todo:
  Executa as três etapas (vocabulário, codificação, escrita).
*/
void compressao(FILE *arqTxt, FILE *arqAlf, FILE *arqComprimido) {
  TipoAlfabeto Alfabeto;
  TipoPalavra Palavra, Linha;
  int Ind = 1, MaxCompCod;
  TipoPesos p;
  TipoDicionario Vocabulario = calloc(M + 1, sizeof(TipoItem));
  TipoVetoresBO VetoresBaseOffset =
      calloc(MAXTAMVETORESDO + 1, sizeof(TipoBaseOffset));
  /* Inicializacao do Alfabeto */
  define_alfabeto(Alfabeto, arqAlf); /* Le o alfabeto definido em arquivo */
  *Linha = '\0';
  inicializa(Vocabulario);
  gera_pesos(p);
  primeira_etapa(arqTxt, Alfabeto, &Ind, Palavra, Linha, Vocabulario, p);
  MaxCompCod = segunda_etapa(Vocabulario, VetoresBaseOffset, p, arqComprimido);
  fseek(arqTxt, 0, SEEK_SET); /* Move cursor para inicio do arquivo*/
  Ind = 1;
  *Linha = '\0';
  terceira_etapa(arqTxt, Alfabeto, &Ind, Palavra, Linha, Vocabulario, p,
                 VetoresBaseOffset, arqComprimido, MaxCompCod);
  free(Vocabulario);
  free(VetoresBaseOffset);
}

/*Procedimentos da Descompressao*/
int le_vetores(FILE *arqComprimido, TipoBaseOffset *vetoresBaseOffset) {
  int MaxCompCod, i;
  MaxCompCod = le_num_int(arqComprimido);
  for (i = 1; i <= MaxCompCod; i++) {
    vetoresBaseOffset[i].base = le_num_int(arqComprimido);
    vetoresBaseOffset[i].offset = le_num_int(arqComprimido);
  }
  return MaxCompCod;
}

int le_vocabulario(FILE *arqComprimido, TipoVetorPalavra vocabulario) {
  int i;
  TipoPalavra Palavra;
  char Ch;
  int num_nodos_folhas = le_num_int(arqComprimido);
  for (i = 1; i <= num_nodos_folhas; i++) {
    *Palavra = '\0';
    do {
      fread(&Ch, sizeof(unsigned char), 1, arqComprimido);
      if (Ch != (char)0) /* As palavras estao separadas pelo caratere 0 */
        sprintf(Palavra + strlen(Palavra), "%c", Ch);
    } while (Ch != (char)0);
    strcpy(vocabulario[i], Palavra);
  }
  return num_nodos_folhas;
}

// Versão do Boyer-Moore-Horspool adaptada para padrões em forma compactada
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

void atribui(TipoPadrao P, int Codigo, int c) {
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

// Realiza busca no texto já comprimido buscando casamento exato com o BMH
// adaptado
SolucaoCasamento *processar_padrao(TipoPadrao padrao,
                                   TipoVetorPalavra vocabulario,
                                   int NumNodosFolhas,
                                   TipoVetoresBO vetoresBaseOffset,
                                   int MaxCompCod, TipoTexto T, int n) {
  SolucaoCasamento *solucao = alocar_solucao_casamento(padrao);

  int Ind, Ord, c;

  for (Ind = 1; Ind <= NumNodosFolhas; Ind++) {
    if (!strcmp(vocabulario[Ind], padrao)) {
      Ord = Ind;
      break;
    }
  }

  if (Ind == NumNodosFolhas + 1) {
    printf("Padrao:%s nao encontrado\n", padrao);
    return solucao;
  }

  int codigo = codifica(vetoresBaseOffset, Ord, &c, MaxCompCod);
  atribui(padrao, codigo, c);
  bmh_byte(T, n, padrao, c, solucao);

  return solucao;
}

// Comprimi o arquivo de entrada
void comprimir_arquivo_entrada(char *arq_input_name) {
  FILE *arq_comprimido = fopen(ARQ_COMP_PATH, "w+b");
  FILE *arq_alfabeto = fopen(ARQ_ALF_PATH, "r");
  FILE *input_text_fp = fopen(arq_input_name, "r");

  if (arq_comprimido == NULL || arq_alfabeto == NULL) {
    perror("Erro ao abrir arquivos de dependência\n");
    exit(1);
  }

  compressao(input_text_fp, arq_alfabeto, arq_comprimido);

  fclose(arq_comprimido);
  arq_comprimido = NULL;
  fclose(arq_alfabeto);
  arq_alfabeto = NULL;
  fclose(input_text_fp);
  input_text_fp = NULL;
}