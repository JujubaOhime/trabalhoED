#include <stdio.h>
#include "bm.h"
#include "TLSE.h"
#include "banco.h"
#include <string.h>
#include <limits.h>
///func 1, dado um arquivo e o fator de ramificacao, gera a arvore;
// gcc main.c -o main bm.o pizza.o

///funcoes que geram arvore e da reboot nos dados indices

int main(void)
{
  int resposta = 0;
  int T = 0;
  int indice;
  TABM *util;
  TPizza *sup;
  TLSE *listaUtil;
  do
  {
    printf("\e[34;1;1mpor favor, insira o T desejado para a arvore\e[m\n");
    scanf("%d", &T);
  } while (T <= 0);
  FILE *fp = fopen("ARQ/raiz.dat", "rb");
  if (!fp)
  {
    util = geraArvArq(T);
    paraArq(NULL, util, T);
    libera(util);
  }
  else
  {
    int t;
    fread(&t, sizeof(int), 1, fp);
    fclose(fp);
    if (t != T)
    {
      printf("T diferente do registrado da arvore anterior\n");
      util = geraArvArq(T);
      paraArq(NULL, util, T);
      libera(util);
    }
  }
  while (resposta != -9)
  {
    printf("\e[34;1;1mT atual -> %d\e[m\n", T);
    printf("\e[34;1;1minsira: \e[m\n");
    printf("\e[33;1m1\e[m -> mudar o T\n\e[33;1m2\e[m -> adicionar ou atualizar pizza\n\e[33;1m3\e[m -> buscar ou remover elemento\n\e[33;1m4\e[m -> buscar ou remover categoria\n\e[33;1m5\e[m -> imprimir arvore\n\e[33;1m6\e[m -> imprimir catalogo\n\e[33;1m\e[33;1m7\e[m -> merge com outro arquivo\n\e[33;1m1000\e[m -> resetar catalogo e arvore\n\e[33;1m1111\e[m -> resetar somente arvore\n\e[33;1m-9\e[m -> para encerrar\n");

    scanf("%d", &resposta);
    switch (resposta)
    {
    ///case 1 completo
    case 1:
      do
      {
        printf("\e[34;1;1mpor favor, insira o T desejado para a arvore\e[m\n");
        scanf("%d", &T);
      } while (T <= 0);
      util = geraArvArq(T);
      paraArq(NULL, util, T);
      libera(util);
      break;

    ///case 2 completo
    case 2:
      sup = getPizza();
      util = adicao(T, sup);
      imprime(util, 0);
      libera(util);
      free(sup);
      break;

    ///case 3 falta opcao de remocao
    case 3:
      printf("\e[34;1;1minsira o indice desejado\e[m\n");
      scanf("%d", &indice);
      if (imprimeIndice(indice))
      {
        char aux[3];
        printf("\e[34;1;1mdeseja remover? s/n\e[m\n");
        scanf(" %s", aux);
        if (strcmp(aux, "s") == 0)
        {
          remocao(T, indice);
        }
      }
      break;

    ///falta remocao
    case 4:
      printf("\e[34;1;1minsira a categoria desejada\e[m\n");
      char tipo[20];
      scanf(" %[^\n]", tipo);
      listaUtil = imprimeCat(tipo);
      if (tamLista(listaUtil) != 0)
      {
        char aux[3];
        printf("\e[34;1;1mdeseja remover categoria? s/n\e[m\n");
        scanf(" %s", aux);
        if (strcmp(aux, "s") == 0)
        {
          removeCat(listaUtil, T);
        }
      }
      else
        printf("\e[34;1;1mcategoria nao encontrada \e[m\n");
      break;

    ///case 5 completo
    case 5:
      util = completo(NULL, T);
      imprime(util, 0);
      libera(util);

      break;

    ///case 6 completo
    case 6:
      imprimeArq();
      break;

    case 7:
      printf("\e[34;1;1minsira o nome do arquivo para merge\e[m\n");
      char nome[100];
      scanf(" %[^\n]", nome);
      merge(nome, T);
      break;

    case 8:
      imprimefp2();

    ///case 1000 completo
    case 1000:
      util = geraArv(T, NULL);
      paraArq(NULL, util, T);
      libera(util);
      break;

    ///case 1111 completo
    case 1111:
      util = geraArvArq(T);
      paraArq(NULL, util, T);
      libera(util);
      break;

    }
  }
  return 0;
}