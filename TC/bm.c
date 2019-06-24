#include <stdio.h>
#include <stdlib.h>
#include "bm.h"
#include <string.h>



TABM *cria(int t){
  TABM* novo = (TABM*)malloc(sizeof(TABM));
  novo->nchaves = 0;
  novo->chave =(int*)malloc(sizeof(int)*((t*2)-1));
  novo->pizzas =(TPizza**)malloc(sizeof(TPizza*)*((t*2)-1));
  novo->folha = 1;
  novo->filho = (TABM**)malloc(sizeof(TABM*)*t*2);
  novo->prox = NULL;
  int i;
  for(i=0; i<(t*2); i++)novo->filho[i] = NULL;
  for(i=0; i<(t*2)-1; i++)novo->pizzas[i] = NULL;
  return novo;
}


TABM *inicializa(void){
  return NULL;
}

void libera(TABM *a){
  if(a){
    if(!a->folha){
      int i;
      for(i = 0; i <= a->nchaves; i++) libera(a->filho[i]);
    }
    free(a->chave);
    free(a);
  }
}

TABM *busca(TABM *a, int mat){
  if (!a) return NULL;
  int i = 0;
  while ((i < a->nchaves) && (mat > a->chave[i])) i++;
  if ((i < a->nchaves) && (a->folha) && (mat == a->chave[i])) return a;
  if (a-> folha) return NULL;
  if (a->chave[i] == mat) i++;
  return busca(a->filho[i], mat);
}

void imprime(TABM *a, int andar){
  if(a){
    int i,j;
    for(i=0; i<=a->nchaves-1; i++){
      imprime(a->filho[i],andar+1);
      for(j=0; j<=andar; j++) printf("   ");
      printf("%d\n", a->chave[i]);
    }
    imprime(a->filho[i],andar+1);
  }
}

TABM *divisao(TABM *x, int i, TABM* y, int t){
  TABM *z = cria(t);
  z->folha = y->folha;
  int j;
  if(!y->folha){
    z->nchaves = t-1;
    for(j=0;j<t-1;j++)z->chave[j] = y->chave[j+t];
    for(j=0;j<t;j++){
      z->filho[j] = y->filho[j+t];
      y->filho[j+t] = NULL;
    }
  }
  else {
    z->nchaves = t; //z possuir� uma chave a mais que y se for folha
    for(j=0;j < t;j++){
      z->chave[j] = y->chave[j+t-1];//Caso em que y � folha, temos q passar a info para o n� da direita
      z->pizzas[j] = y->pizzas[j+t-1];
    }
    y->prox = z;
  }
  y->nchaves = t-1;
  for(j=x->nchaves; j>=i; j--)x->filho[j+1]=x->filho[j];
  x->filho[i] = z;
  for(j=x->nchaves; j>=i; j--) x->chave[j] = x->chave[j-1];
  x->chave[i-1] = y->chave[t-1];
  x->nchaves++;
  return x;
}


TABM *insere_nao_completo(TABM *x, TPizza *p, int t){
  int i = x->nchaves-1;
  if(x->folha){
    while((i>=0) && (p->cod < x->chave[i])){
      x->chave[i+1] = x->chave[i];
      i--;
    }
    x->chave[i+1] = p->cod;
    x->pizzas[i+1] = p;
    x->nchaves++;
    return x;
  }
  while((i>=0) && (p->cod < x->chave[i])) i--;
  i++;
  if(x->filho[i]->nchaves == ((2*t)-1)){
    x = divisao(x, (i+1), x->filho[i], t);
    if(p->cod > x->chave[i]) i++;
  }
  x->filho[i] = insere_nao_completo(x->filho[i], p, t);
  return x;
}

TABM *insere(TABM *T, TPizza *p, int t){
  if(busca(T, p->cod)) return T;
  if(!T){
    T=cria(t);
    T->chave[0] = p->cod;
    T->nchaves=1;
    T->pizzas[0] = p;
    return T;
  }
  if(T->nchaves == (2*t)-1){
    TABM *S = cria(t);
    S->nchaves=0;
    S->folha = 0;
    S->filho[0] = T;
    S = divisao(S,1,T,t);
    S = insere_nao_completo(S, p, t);
    return S;
  }
  T = insere_nao_completo(T, p, t);
  return T;
}


