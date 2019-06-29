#include <stdio.h>
#include <stdlib.h>
#include "pizza.h"

typedef struct arvbm{
  int nchaves, folha, *chave;
  struct arvbm **filho, *prox;
  TPizza **pizzas;
}TABM;

TABM *cria(int t);
TABM *inicializa(void);
void libera(TABM *a);
TABM *busca(TABM *a, int mat);
void imprime(TABM *a, int andar);
TABM *divisao(TABM *x, int i, TABM* y, int t);
TABM *insere_nao_completo(TABM *x, TPizza *p, int t);
TABM *insere(TABM *T, TPizza *p, int t);
TABM *remover(TABM *arv, int ch, int t);