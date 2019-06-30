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

TABM *remover(TABM *arv, int ch, int t)
{
	if (!arv) return arv;
	int i;
	// se o numero de chaves for maior que o num da posição e o elem for igual a chave da posição:

	if(arv->folha)
	{// caso 1
		for (i = 0; i < arv->nchaves && arv->chave[i] < ch; i++); // acha posição nó onde deve estar o que quer remover
		printf("\nCASO 1\n");
		int j;
		for (j = i; j < arv->nchaves - 1; j++) arv->chave[j] = arv->chave[j + 1];
		arv->nchaves--;
		return arv;
	}

	for (i = 0; i < arv->nchaves && arv->chave[i] <= ch; i++); // acha posição nó onde deve estar o que quer remover
	
	TABM *y = arv->filho[i], *z = NULL;

	if (y->nchaves == t - 1)
	{ //CASOS 3A e 3B
		if ((i < arv->nchaves) && (arv->filho[i + 1]->nchaves >= t))
		{ //CASO 3A
			printf("CASO 3A: i menor que nchaves");
			printf("  i = %d", i);
			z = arv->filho[i + 1]; // z = 10-29 e y = 2
			int j;
			if(y->folha){
				y->chave[t - 1] = arv->chave[i];
				y->nchaves++;
				int elem = z->chave[0];
				for(j = 0; j<z->nchaves -1; j++){
					z->chave[j] = z->chave[j+1];
				}
				z->nchaves--;
				y->chave[y->nchaves-1] = elem;
				arv->chave[i] = z->chave[0];
				arv->filho[i] = remover(y, ch, t);
				return arv;
			}
			y->chave[t - 1] = arv->chave[i]; //dar a y a chave i da arv; y = 2-10
			y->nchaves++;
			arv->chave[i] = z->chave[0]; //dar a arv uma chave de z //10 por 10
			for (j = 0; j < z->nchaves - 1; j++) //ajustar chaves de z
				z->chave[j] = z->chave[j + 1];
			//z->chave[j] = 0; Rosseti
			y->filho[y->nchaves] = z->filho[0]; //enviar ponteiro menor de z para o novo elemento em y
			for (j = 0; j < z->nchaves; j++)	//ajustar filhos de z
				z->filho[j] = z->filho[j + 1]; // z = 29
			z->nchaves--;
			arv->filho[i] = remover(arv->filho[i], ch, t);
			return arv;
		}
		if ((i > 0) && (!z) && (arv->filho[i - 1]->nchaves >= t))
		{ //CASO 3A
			printf("\nCASO 3A: i igual a nchaves\n"); // i = 3
			z = arv->filho[i - 1];
			int j;
			if (y->folha){
				for (j = y->nchaves; j > 0; j--) //encaixar lugar da nova chave
					y->chave[j] = y->chave[j - 1];
				y->nchaves++;
				int elem = z->chave[z->nchaves-1];
				z->nchaves--;
				y->chave[0] = elem;
				arv->chave[i-1] = elem;
				arv->filho[i] = remover(y, ch, t);
				return arv;
			}
			for (j = y->nchaves; j > 0; j--) //encaixar lugar da nova chave
				y->chave[j] = y->chave[j - 1];
			for (j = y->nchaves + 1; j > 0; j--) //encaixar lugar dos filhos da nova chave
				y->filho[j] = y->filho[j - 1];
			y->chave[0] = arv->chave[i - 1]; //dar a y a chave i da arv
			y->nchaves++;
			arv->chave[i - 1] = z->chave[z->nchaves - 1]; //dar a arv uma chave de z
			y->filho[0] = z->filho[z->nchaves];			  //enviar ponteiro de z para o novo elemento em y
			z->nchaves--;
			arv->filho[i] = remover(y, ch, t);
			return arv;
		}
		if (!z)
		{ //CASO 3B
			if (i < arv->nchaves && arv->filho[i + 1]->nchaves == t - 1)
			{	
				printf("\nCASO 3B: i menor que nchaves\n");
				printf("  i = %d", i);
				z = arv->filho[i + 1]; 
				int j;
				if (y->folha){
					for (j = 0; j < t - 1; j++)
					{
						y->chave[t -1 + j] = z->chave[j]; //passar filho[i+1] para filho[i]
						y->nchaves++;
					}
					for (j = i; j < arv->nchaves - 1; j++)
					{ //limpar referências de i
						arv->chave[j] = arv->chave[j + 1];
						arv->filho[j + 1] = arv->filho[j + 2];
					}
					arv->nchaves--;
					arv = remover(arv, ch, t);
					return arv;

				}
				y->chave[t - 1] = arv->chave[i]; //pegar chave [i] e coloca ao final de filho[i]
				y->nchaves++;
				for (j = 0; j < t - 1; j++)
				{
					y->chave[t + j] = z->chave[j]; //passar filho[i+1] para filho[i]
					y->nchaves++;
				}
				if (!y->folha)
				{
					for (j = 0; j < t; j++)
					{
						y->filho[t + j] = z->filho[j];
					}
				}
				for (j = i; j < arv->nchaves - 1; j++)
				{ //limpar referências de i
					arv->chave[j] = arv->chave[j + 1];
					arv->filho[j + 1] = arv->filho[j + 2];
				}
				arv->nchaves--;
				arv = remover(arv, ch, t);
				return arv;
			}
			if ((i > 0) && (arv->filho[i - 1]->nchaves == t - 1))
			{
				z = arv->filho[i - 1];
				int j;
				printf("\nCASO 3B: i igual a nchaves\n");
				if (y->folha){
					for (j = 0; j < t - 1; j++)
					{
						z->chave[t + j] = y->chave[j]; //passar filho[i+1] para filho[i]
						z->nchaves++;
					}
					arv->nchaves--;
					//arv->filho[i - 1] = z;
					arv = remover(arv, ch, t);
					return arv;
				}
				if (i == arv->nchaves)
					z->chave[t - 1] = arv->chave[i - 1]; //pegar chave[i] e poe ao final de filho[i-1]
				else
					z->chave[t - 1] = arv->chave[i]; //pegar chave [i] e poe ao final de filho[i-1]
				z->nchaves++;
				for (j = 0; j < t - 1; j++)
				{
					z->chave[t + j] = y->chave[j]; //passar filho[i+1] para filho[i]
					z->nchaves++;
				}
				if (!z->folha)
				{
					for (j = 0; j < t; j++)
					{
						z->filho[t + j] = y->filho[j];
					}
				}
				arv->nchaves--;
				arv->filho[i - 1] = z;
				arv = remover(arv, ch, t);
				return arv;
			}
		}
	}
	arv->filho[i] = remover(arv->filho[i], ch, t);
	return arv;
}
