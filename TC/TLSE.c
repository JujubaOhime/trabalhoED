#include "TLSE.h"

TLSE* inserel(TLSE *l, int elem){
  TLSE *novo = (TLSE *) malloc(sizeof(TLSE));
  novo->prox = l;
  novo->info = elem;
  return novo;
}

void imprimel(TLSE *l){
  TLSE *p = l;
  while(p){
    printf("% d ", p->info);
    p = p->prox;
  } 
}

void liberal(TLSE *l){
  TLSE *p = l, *q;
  while(p){
    q = p;
    p = p->prox;
    free(q);
  } 
}

TLSE* retiral(TLSE *l, int elem){
  TLSE *p = l, *ant = NULL;
  while((p) && (p->info != elem)){
    ant = p;
    p = p->prox;
  }
  if(!p) return l;
  if(!ant) l = l->prox;
  else ant->prox = p->prox;
  free(p);
  return l;
}




TLSE* buscal(TLSE *l, int elem){
  TLSE *p = l;
  while((p) && (p->info != elem)) p = p->prox; 
  return p;
}


int buscaIndice(TLSE *l, int indice){
  int atual = 0;
  TLSE *aux = l;
  while(atual != indice){
    aux = aux->prox;
    atual ++;
  }
  return aux->info;
}


int tamLista(TLSE *l){
  TLSE *p = l;
  int tam = 0;
  while(p){
    tam = tam + 1;
    p = p->prox;
  } 
  return tam;
}



