#include <stdio.h>
#include "bm.h"
#include "TLSE.h"
#include "banco.h"
#include <string.h>
#include <limits.h>

TABM *geraArv(int t, char *nome)
{ ///gera dos dados iniciais
  TABM *T = inicializa();
  system("exec rm -r ./ARQ/*");
  if (!nome)
    nome = "dados_iniciais.dat";
  FILE *fp = fopen(nome, "rb");
  FILE *fp2 = fopen("indices.dat", "wb");
  FILE *fp3 = fopen("pizzas.dat", "wb");
  if (!fp)
    exit(1);
  if (!fp2)
    exit(1);
  if (!fp3)
    exit(1);
  int aux;
  TPizza *p;
  do
  {
    p = le_pizza(fp);
    if (p)
    {
      T = insere(T, p, t);
      fseek(fp3, 0, SEEK_END);
      aux = p->cod;
      fwrite(&aux, sizeof(int), 1, fp2);
      aux = ftell(fp3);
      fwrite(&aux, sizeof(int), 1, fp2);
      salva_pizza(p, fp3);
      aux = 1;
      fwrite(&aux, sizeof(int), 1, fp2);
    }
  } while (p);
  fclose(fp);
  fclose(fp2);
  fclose(fp3);
  return T;
}

// le tenta abrir um arq
TABM *le(FILE *fp, int t, int booleano, char *c)
{
  rewind(fp);
  int aux;
  int aux2;
  fread(&aux, sizeof(int), 1, fp);
  fread(&aux2, sizeof(int), 1, fp);
  if (t != aux)
  {
    fclose(fp);
    if (booleano == 0)
      free(c);
    return NULL;
  }
  TABM *novo = cria(t);
  novo->folha = aux2;
  do
  {
    aux2 = fread(&aux, sizeof(int), 1, fp);
    if (aux2 > 0)
    {
      novo->chave[novo->nchaves] = aux;
      novo->nchaves++;
    }
  } while (aux2 > 0);
  for (int i = 0; i < (2 * t) - 1; i++)
    novo->filho[i] = NULL;
  return novo;
}

//gerencia a string do nome do arq
//abre só um e bota todos filhos como nulo a partir de le
TABM *openF(char *c, int t)
{
  int booleano = 1;

  if (!c)
  {
    c = (char *)malloc(100 * (sizeof(char)));
    strcpy(c, "");
    c = strcat(c, "ARQ/raiz");
    booleano = 0;
  }
  char *str = (char *)malloc(100 * sizeof(char));
  strcpy(str, c);
  int i;
  strcat(str, ".dat");
  FILE *fp = fopen(str, "rb");
  if (!fp)
    return NULL;
  TABM *novo = le(fp, t, booleano, c);
  fclose(fp);
  if (booleano == 0)
    free(c);
  return novo;
}

//le a arv em disco e coloca na mP
TABM *completo(char *c, int t)
{ ///le do arquivo das arvs
  int booleano = 1;
  if (!c)
  {
    c = (char *)malloc(100 * (sizeof(char)));
    strcpy(c, "");
    c = strcat(c, "ARQ/raiz");
    booleano = 0;
  }
  char *str = (char *)malloc(100 * sizeof(char));
  strcpy(str, c);
  strcat(str, ".dat");
  FILE *fp = fopen(str, "rb");
  if (!fp)
    return NULL;
  TABM *a = le(fp, t, booleano, c);
  if (!a)
    return a;
  for (int i = 0; i < 2 * t; i++)
  {
    char aux[10];
    sprintf(aux, "_%d", i);
    strcpy(str, c);
    strcat(str, aux);
    a->filho[i] = completo(str, t);
  }
  fclose(fp);
  free(str);
  if (booleano == 0)
    free(c);
  return a;
}

//pega o arquivo índice e vai gerar uma árvore do 0 a partir dele
TABM *geraArvArq(int t)
{ ///gera do arquivo indices
  FILE *fp2 = fopen("indices.dat", "rb");
  if (!fp2)
    exit(1);
  FILE *fp3 = fopen("pizzas.dat", "rb");
  if (!fp3)
    exit(1);
  system("exec rm -r ./ARQ/*");
  int control = 1;
  int indices[3];
  TABM *T = inicializa();
  TPizza *p;
  while (control > 0)
  {
    control = fread(indices, sizeof(int), 3, fp2);
    if (indices[2] != 0)
    {
      fseek(fp3, indices[1], SEEK_SET);
      p = le_pizza(fp3);
      if (p)
      {
        T = insere(T, p, t);
      }
    }
  }
  fclose(fp2);
  fclose(fp3);
  return T;
}

//passa um nome, arv e gera todos arquivos da arvore
//raiz_0 raiz_1 raiz_2
//pega a arv da ram pra disco. SalvaArvore
void paraArq(char *c, TABM *arv, int t)
{
  int booleano = 1;
  if (!arv)
    return;
  if (!c)
  {
    c = (char *)malloc(100 * (sizeof(char)));
    strcpy(c, "");
    c = strcat(c, "ARQ/raiz");
    booleano = 0;
  }
  char *str = (char *)malloc(100 * sizeof(char));
  strcpy(str, c);
  strcat(str, ".dat");
  FILE *fp = fopen(str, "wb");
  if (!fp)
    exit(1);
  int i = arv->folha;
  fwrite(&t, sizeof(int), 1, fp);
  fwrite(&i, sizeof(int), 1, fp);
  fwrite(arv->chave, sizeof(int), arv->nchaves, fp);
  fclose(fp);
  if (arv->filho)
  {
    for (int j = 0; j < arv->nchaves; j++)
    {
      char aux[10];
      sprintf(aux, "_%d", j);
      strcpy(str, c);
      strcat(str, aux);
      paraArq(str, arv->filho[j], t);
    }
    char aux[10];
    sprintf(aux, "_%d", arv->nchaves);
    strcpy(str, c);
    strcat(str, aux);
    paraArq(str, arv->filho[arv->nchaves], t);
  }
  free(str);
  if (booleano == 0)
    free(c);
}

// gera uma arv. em mp a partir dos arqs a partir do indice
TABM *abreCond(char *c, int t, int indice)
{
  int booleano = 1;
  if (!c)
  {
    c = (char *)malloc(100 * (sizeof(char)));
    strcpy(c, "");
    c = strcat(c, "ARQ/raiz");
    booleano = 0;
  }
  char *str = (char *)malloc(100 * sizeof(char));
  strcpy(str, c);
  strcat(str, ".dat");
  FILE *fp = fopen(str, "rb");
  if (!fp)
    return NULL;
  TABM *a = le(fp, t, booleano, c);
  if (!a)
    return a;
  int i = 0;
  while ((i < a->nchaves) && (indice > a->chave[i]))
    i++;
  if (i == 0)
  { //filho que to tentando abrir é menor que a primeira chave, abrir primeiro filho
    printf("caso1\n e menor que %d\n", a->chave[0]);
    char aux[10];
    sprintf(aux, "_%d", i);
    strcpy(str, c);
    strcat(str, aux);
    a->filho[i] = abreCond(str, t, indice);
    sprintf(aux, "_%d", i + 1);
    strcpy(str, c);
    strcat(str, aux);
    a->filho[i + 1] = openF(str, t);
  }
  else if (i == a->nchaves)
  { // maior qua a ultima chave, acessa o último filho
    printf("caso2\ne maior que %d\n", a->chave[a->nchaves - 1]);
    char aux[10];
    sprintf(aux, "_%d", i);
    strcpy(str, c);
    strcat(str, aux);
    a->filho[i] = abreCond(str, t, indice);
    sprintf(aux, "_%d", i - 1);
    strcpy(str, c);
    strcat(str, aux);
    a->filho[i - 1] = openF(str, t);
  }
  else
  { //do meio
    printf("caso3\nesta entre %d e %d\n", a->chave[i], a->chave[i - 1]);
    char aux[10];
    sprintf(aux, "_%d", i);
    strcpy(str, c);
    strcat(str, aux);
    a->filho[i] = abreCond(str, t, indice);
    sprintf(aux, "_%d", i + 1);
    strcpy(str, c);
    strcat(str, aux);
    a->filho[i + 1] = openF(str, t);
    sprintf(aux, "_%d", i - 1);
    strcpy(str, c);
    strcat(str, aux);
    a->filho[i - 1] = openF(str, t);
  }
  free(str);
  fclose(fp);
  if (booleano == 0)
    free(c);
  return a;
}

//mexe no arq de índices
//arq indices: info 1:indice da pizza info 2:ponteiro da pizza em pizza.dat info 3:is alive
void adicaoArq(TPizza *p)
{
  FILE *fp2 = fopen("indices.dat", "rb+");
  if (!fp2)
    exit(8);
  rewind(fp2);
  FILE *fp3 = fopen("pizzas.dat", "rb+");
  if (!fp3)
    exit(9);
  rewind(fp3);
  int indices[3];
  int controle = 1;
  indices[0] = -1;
  indices[1] = -1;
  indices[2] = -1;
  while (indices[0] < p->cod && controle > 0)
    controle = fread(indices, sizeof(int), 3, fp2);
  if (indices[0] == p->cod)
  {
    indices[2] = 1;
    fseek(fp2, -3 * sizeof(int), SEEK_CUR);
    fwrite(indices, sizeof(int), 3, fp2);
    fseek(fp3, indices[1], SEEK_SET);
    salva_pizza(p, fp3);
  }
  else if (controle <= 0)
  {
    fseek(fp2, 0, SEEK_END);
    fseek(fp3, 0, SEEK_END);
    indices[0] = p->cod;
    indices[1] = ftell(fp3);
    indices[2] = 1;
    salva_pizza(p, fp3);
    fwrite(indices, sizeof(int), 3, fp2);
  }
  else
  {
    int posicao = ftell(fp2) - 3 * sizeof(int);
    fseek(fp2, 0, SEEK_END);
    while (ftell(fp2) != posicao)
    {
      fseek(fp2, -3 * sizeof(int), SEEK_CUR);
      fread(indices, sizeof(int), 3, fp2);
      fwrite(indices, sizeof(int), 3, fp2);
      fseek(fp2, -6 * sizeof(int), SEEK_CUR);
    }
    fseek(fp3, 0, SEEK_END);
    indices[0] = p->cod;
    indices[1] = ftell(fp3);
    indices[2] = 1;
    salva_pizza(p, fp3);
    fwrite(indices, sizeof(int), 3, fp2);
  }

  fclose(fp2);
  fclose(fp3);
}

//adiciona uma pizza na arv na mp e no arq
TABM *adicao(int t, TPizza *p)
{
  //TABM *T = completo(NULL, t);
  //TABM *T = abrecond(NULL, t, p->cod);
  TABM *T = completo(NULL, t);
  if (T)
  {
    if (busca(T, p->cod))
    {
      adicaoArq(p);
      return T;
    }
    T = insere(T, p, t);
    paraArq(NULL, T, t);
    adicaoArq(p);
    return T;
  }
  return NULL;
}

void remocaoArq(int indice)
{
  FILE *fp2 = fopen("indices.dat", "rb+");
  if (!fp2)
    exit(1);
  rewind(fp2);
  int indices[3];
  int controle = 1;
  indices[0] = -1;
  indices[1] = -1;
  indices[2] = -1;
  while (indices[0] < indice && controle > 0)
    controle = fread(indices, sizeof(int), 3, fp2);
  if (indices[0] == indice)
  {
    int aux = ftell(fp2);
    aux = aux - 3 * sizeof(int);
    fseek(fp2, aux, SEEK_SET);
    indices[2] = 0;
    fwrite(indices, sizeof(int), 3, fp2);
  }
  fclose(fp2);
  return;
}

void remocao(int t, int indice)
{
  TABM *T = completo(NULL, t);
  if (busca(T, indice))
  {
    T = remover(T, indice, t);
    remocaoArq(indice);
  }
  paraArq(NULL, T, t);
  libera(T);
}

void imprimeArq()
{
  FILE *fp2 = fopen("indices.dat", "rb");
  if (!fp2)
    exit(1);
  FILE *fp3 = fopen("pizzas.dat", "rb");
  if (!fp3)
    exit(1);
  int control, indices[3];
  TPizza *p = (TPizza *)malloc(tamanho_pizza_bytes());
  control = 2;
  while (control > 1)
  {
    control = fread(indices, sizeof(int), 3, fp2);
    if (control <= 1)
      break;
    if (indices[2] != 0)
    {
      fseek(fp3, indices[1], SEEK_SET);
      p = le_pizza(fp3);
      imprime_pizza(p);
    }
  }
  free(p);
  fclose(fp2);
  fclose(fp3);
}

TPizza *buscaPizza(int indice)
{
  FILE *fp2 = fopen("indices.dat", "rb+");
  if (!fp2)
    exit(57);
  rewind(fp2);
  FILE *fp3 = fopen("pizzas.dat", "rb+");
  if (!fp3)
    exit(57);
  int indices[3];
  int controle = 1;
  TPizza *p = (TPizza *)malloc(tamanho_pizza_bytes());
  indices[0] = INT_MIN;
  indices[1] = INT_MIN;
  indices[2] = INT_MIN;
  while (indices[0] < indice && controle > 0)
  {
    controle = fread(indices, sizeof(int), 3, fp2);
  }
  if (indices[0] == indice && indices[2])
  {
    fseek(fp3, indices[1], SEEK_SET);
    p = le_pizza(fp3);
  }
  else
  {
    free(p);
    p = NULL;
  }
  fclose(fp2);
  fclose(fp3);
  return p;
}

int imprimeIndice(int indice)
{
  TPizza *p = buscaPizza(indice);
  if (p)
  {
    imprime_pizza(p);
    free(p);
    return 1;
  }
  else
    printf("pizza nao encontrada >:)\n");
  return 0;
}

TLSE *buscat(char *c)
{
  FILE *fp2 = fopen("indices.dat", "rb+");
  if (!fp2)
    exit(1);
  rewind(fp2);
  FILE *fp3 = fopen("pizzas.dat", "rb+");
  if (!fp3)
    exit(1);
  int indices[3];
  int controle = 1;
  TLSE *cat = NULL;
  TPizza *p = (TPizza *)malloc(tamanho_pizza_bytes());
  indices[0] = -1;
  indices[1] = -1;
  indices[2] = -1;
  while (controle > 0)
  {
    controle = fread(indices, sizeof(int), 3, fp2);
    if (controle > 0)
    {
      if (indices[2] != 0)
      {
        fseek(fp3, indices[1], SEEK_SET);
        p = le_pizza(fp3);
        if (strcmp(p->descricao, c) == 0)
          cat = inserel(cat, p->cod);
      }
    }
  }
  free(p);
  p = NULL;
  fclose(fp2);
  fclose(fp3);
  return cat;
}

TLSE *imprimeCat(char *c)
{
  TLSE *T = buscat(c);
  printf("\n");
  for (int i = 0; i < tamLista(T); i++)
  {
    TPizza *p = buscaPizza(buscaIndice(T, i));
    imprime_pizza(p);
  }
  return T;
}

void removeCat(TLSE *list, int T)
{
  for (int i = 0; i < tamLista(list); i++)
  {
    TPizza *p = buscaPizza(buscaIndice(list, i));
    if (p)
      remocao(T, p->cod);
  }
}

TPizza *getPizza()
{
  char nome[50];
  char tipo[20];
  int cod;
  float preco;
  printf("\e[34;1;1mQual o nome da pizza?\e[m\n");
  scanf(" %[^\n]", nome);
  printf("\e[34;1;1mQual o tipo da pizza?\e[m\n");
  scanf(" %[^\n]", tipo);
  printf("\e[34;1;1mEscreva seu preco\e[m\n");
  scanf(" %f", &preco);
  printf("\e[34;1;1mEscreva o código da pizza\e[m\n");
  scanf(" %d", &cod);
  return pizza(cod, nome, tipo, preco);
}

void imprimefp2()
{
  FILE *fp2 = fopen("indices.dat", "rb+");
  if (!fp2)
    exit(13);
  rewind(fp2);
  int controle = 1;
  int indices[3];
  while (controle > 0)
  {
    controle = fread(indices, sizeof(int), 3, fp2);
    if (controle > 0)
      printf("%d %d %d\n", indices[0], indices[1], indices[2]);
  }
  fclose(fp2);
}

void merge(char *c, int t)
{
  FILE *fp = fopen(c, "rb");
  if (!fp)
    exit(1);
  rewind(fp);
  TPizza *p = NULL;
  TABM *T = NULL;
  do
  {
    p = le_pizza(fp);
    ;
    if (p)
    {
      {
        T = adicao(t, p);
        printf("tentando add ");
        imprime_pizza(p);
      }
      if (T)
        libera(T);
    }
  } while (p);
  fclose(fp);
}

