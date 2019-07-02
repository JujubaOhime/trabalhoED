#ifndef BANCO_H
#define BANCO_H

#include <stdio.h>
#include "bm.h"
#include "TLSE.h"
#include <string.h>
#include <limits.h>

TABM *geraArv(int t, char *nome);
TABM *le(FILE *fp, int t, int booleano, char *c);
TABM *openF(char *c, int t);
TABM *completo(char *c, int t);
TABM *geraArvArq(int t);
void paraArq(char *c, TABM *arv, int t);
TABM *abreCond(char *c, int t, int indice);
void adicaoArq(TPizza *p);
TABM *adicao(int t, TPizza *p);
void remocaoArq(int indice);
void remocao(int t, int indice);
void imprimeArq();
TPizza *buscaPizza(int indice);
int imprimeIndice(int indice);
TLSE *imprimeCat(char *c);
void removeCat(TLSE *list, int T);
TPizza *getPizza();
void imprimefp2();
void merge(char *c, int t);

#endif