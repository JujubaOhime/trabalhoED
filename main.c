#include <stdio.h>
#include "TC/bm.h"
#include <string.h>

///func 1, dado um arquivo e o fator de ramificacao, gera a arvore;



///funcoes que geram arvore
TABM * geraarv(int t, char*nome){///gera dos dados iniciais
    TABM * T = inicializa();
    if(!nome) nome = "TC/dados_iniciais.dat";
    FILE *fp = fopen(nome, "rb");
    FILE *fp2 = fopen("indices.dat", "wb");
    FILE *fp3 = fopen("pizzas.dat", "wb");
    if(!fp) exit(1);
    int aux;
    TPizza * p;
    do{
      p = le_pizza(fp);
      if(p){
        T = insere(T, p, t);
        fseek(fp3, 0, SEEK_END);
        aux = p->cod;
        fwrite(&aux,sizeof(int), 1, fp2);
        aux =  ftell(fp3);
        fwrite(&aux,sizeof(int), 1, fp2);
        salva_pizza(p, fp3);
        aux = 1;
        fwrite(&aux,sizeof(int), 1, fp2);
        
      }
    }while(p);
    fclose(fp);
    fclose(fp2);
    fclose(fp3);
    return T;
}

TABM * le(FILE *fp, int t, int b, char *c){
  rewind(fp);
  int aux;
  int aux2;
  fread(&aux, sizeof(int), 1, fp);
  fread(&aux2, sizeof(int), 1, fp);
  if(t != aux){
    fclose(fp);
    if(b == 0)free(c);
    return NULL;}
    TABM* novo = cria(t);
    novo->folha = aux2;
  do{
    aux2 = fread(&aux, sizeof(int), 1, fp);
    if(aux2>0){
      novo->chave[novo->nchaves] = aux;
      novo->nchaves++; 
    } 
  }while(aux2>0);
  for(int i = 0; i<(2*t)-1;i++)novo->filho[i] = NULL;
  return novo;
}

TABM * openf(char *c,int t){
  int b = 1;
  
  if(!c){
    c = (char*)malloc(100*(sizeof(char)));
    strcpy(c, "");
    c = strcat(c, "ARQ/raiz");
    int b = 0;
    }
  char *str =(char *)malloc(100*sizeof(char));
  strcpy(str, c);
  int i;
  strcat(str, ".dat");
  FILE *fp = fopen(str, "rb");
  if(!fp) return NULL;
  TABM * novo = le(fp, t, b, c);
  fclose(fp);
  if(b == 0)free(c);
  return novo;
}

TABM * completo(char *c, int t){///gera do arquivo das arvs
  int b = 1;
  if(!c){
    c = (char*)malloc(100*(sizeof(char)));
    strcpy(c, "");
    c = strcat(c, "ARQ/raiz");
    int b = 0;
    }
  char *str =(char *)malloc(100*sizeof(char));
  strcpy(str, c);
  strcat(str, ".dat");
  FILE *fp = fopen(str, "rb");
  if(!fp) return NULL;
  TABM * a = le(fp, t, b, c);
  if(!a)return a;
  for(int i = 0; i<2*t; i++){
    char aux[10];
    sprintf(aux, ">%d", i); 
    strcpy(str, c);    
    strcat(str, aux);
    a->filho[i] = completo(str, t);
  }
  free(str);
  if(b == 0)free(c);
  return a;
}



TABM * geraarvarq(int t){///gera do arquivo indices
  FILE *fp2 = fopen("indices.dat", "rb");
  if(!fp2) exit(1);
  FILE *fp3 = fopen("pizzas.dat", "rb");
  if(!fp3) exit(1);
  int control = 1;
  int indices[3];
  TABM * T = inicializa();
  TPizza * p;
  while(control > 0){
    control = fread(indices, sizeof(int),3,fp2);
    if(indices[2]){
      fseek(fp3, indices[1], SEEK_SET);
      p = le_pizza(fp3);
      if(p){
        T = insere(T, p, t);
      }
    }
  }
  fclose(fp2);
  fclose(fp3);
  return T;
}

void imprimelista(TABM *T, int t){
  int i;
  while(T && !T->folha){
    i = 0;
    while(!T->filho[i] && i<=T->nchaves)i++;
    T = T->filho[i];
  }
  while(T){
  for(i = 0; i<T->nchaves; i++){
    if(T->pizzas[i]) imprime_pizza(T->pizzas[i]);
  }
  T = T->prox;
  }
}

void geralistb(TABM *T, int t, char*nome){
  int i;
  FILE *fp = fopen(nome, "wb");
  if(!fp) exit(1);
  while(T && !T->folha){
    i = 0;
    while(!T->filho[i] && i<=T->nchaves)i++;
    T = T->filho[i];
  }
  while(T){
  for(i = 0; i<T->nchaves; i++){
    if(T->pizzas[i])salva_pizza(T->pizzas[i], fp);
  }
  T = T->prox;
  }
  fclose(fp);
}

void ToArq(char*c, TABM* arv, int t){
  int b = 1;
  if(!arv)return;
  if(!c){
    c = (char*)malloc(100*(sizeof(char)));
    strcpy(c, "");
    c = strcat(c, "ARQ/raiz");
    int b = 0;
    }
  char *str =(char *)malloc(100*sizeof(char));
  strcpy(str, c);
  strcat(str, ".dat");
  FILE *fp = fopen(str, "wb");
  if(!fp) exit(1);
  int i = arv->folha;
  fwrite (&t, sizeof(int), 1, fp);
  fwrite (&i, sizeof(int), 1, fp);
  fwrite (arv->chave, sizeof(int), arv->nchaves, fp);
  fclose(fp);
  if(arv->filho){
  for(int j = 0; j<arv->nchaves; j++){
    char aux[10];
    sprintf(aux, ">%d", j); 
    strcpy(str, c);    
    strcat(str, aux);
    ToArq(str, arv->filho[j], t);
  }
  char aux[10];
  sprintf(aux, ">%d", arv->nchaves); 
  strcpy(str, c);    
  strcat(str, aux);
  ToArq(str, arv->filho[arv->nchaves], t);
}
  free(str);
  if(b == 0)free(c);
}




TABM * abrecond(char *c, int t, int indice){
  int b = 1;
  if(!c){
    c = (char*)malloc(100*(sizeof(char)));
    strcpy(c, "");
    c = strcat(c, "ARQ/raiz");
    int b = 0;
    }
  char *str =(char *)malloc(100*sizeof(char));
  strcpy(str, c);
  strcat(str, ".dat");
  FILE *fp = fopen(str, "rb");
  if(!fp) return NULL;
  TABM * a = le(fp, t, b, c);
  if(!a)return a;
  int i = 0;
  while ((i < a->nchaves) && (indice > a->chave[i])) i++;
  if(i == 0){
    printf("caso1\n e menor que %d\n", a->chave[0]);
    char aux[10];
    sprintf(aux, ">%d", i); 
    strcpy(str, c);    
    strcat(str, aux);
    a->filho[i] = abrecond(str,t,indice);
    sprintf(aux, ">%d", i+1); 
    strcpy(str, c);    
    strcat(str, aux);
    a->filho[i+1] = openf(str,t);
  }
  else if(i == a->nchaves){
    printf("caso2\ne maior que %d\n", a->chave[a->nchaves-1]);
    char aux[10];
    sprintf(aux, ">%d", i); 
    strcpy(str, c);    
    strcat(str, aux);
    a->filho[i] =abrecond(str,t,indice);
    sprintf(aux, ">%d", i-1); 
    strcpy(str, c);    
    strcat(str, aux);
    a->filho[i-1] = openf(str,t);
  }
  else{
    printf("caso3\nesta entre %d e %d\n", a->chave[i], a->chave[i-1]);
    char aux[10];
    sprintf(aux, ">%d", i); 
    strcpy(str, c);    
    strcat(str, aux);
    a->filho[i] = abrecond(str,t,indice);
    sprintf(aux, ">%d", i+1); 
    strcpy(str, c);    
    strcat(str, aux);
    a->filho[i+1] = openf(str,t);
    sprintf(aux, ">%d", i-1); 
    strcpy(str, c);    
    strcat(str, aux);
    a->filho[i-1] = openf(str,t);
  }
  free(str);
  if(b == 0)free(c);
  return a;
}

void adicaoarq(TPizza *p, int b){
  FILE *fp2 = fopen("indices.dat", "rb+");
  if(!fp2) exit(1);
  rewind(fp2);
  FILE *fp3 = fopen("pizzas.dat", "rb+");
  if(!fp3) exit(1);
  int indices[3];
  int controle = 1;
  indices[0] = -1; indices[1] = -1;indices[2] = -1;
  if(!b){
    while(indices[0] != p->cod)fread(indices, sizeof(int), 3, fp2);
    fseek(fp3, indices[1], SEEK_SET);
    salva_pizza(p, fp3);
  }
  if(b){
    while(indices[0]< p->cod && controle>0)controle = fread(indices, sizeof(int), 3, fp2);
    int indicesaux[3];
    fseek(fp3, 0, SEEK_END);
    indicesaux[0] = p->cod; indicesaux[1] = ftell(fp3); indicesaux[2] = 1;
    fwrite(p, tamanho_pizza_bytes(),1, fp3);
    fseek(fp2, -3*sizeof(int), SEEK_CUR);
    fwrite(indicesaux, sizeof(int), 3, fp2);
    indicesaux[0] = indices[0]; indicesaux[1] = indices[1]; indicesaux[2] = indices[2];
    while(controle > 0){
      controle = fread(indices, sizeof(int), 3, fp2);
      fseek(fp2, -3*sizeof(int), SEEK_CUR);
      fwrite(indicesaux, sizeof(int), 3, fp2);
      if(controle == 3){indicesaux[0] = indices[0]; indicesaux[1] = indices[1]; indicesaux[2] = indices[2];}
    }
  }
  fclose(fp2);
  fclose(fp3);
}


void adicao(int t, TPizza *p){
   TABM *T = abrecond(NULL, t, p->cod);
   imprime(T, 0);
   printf("\n\n\n");

   if(busca(T, p->cod)){
     adicaoarq(p, 0);
     libera(T);
     return;
   }
   T = insere(T, p, t);
   imprime(T, 0);
   ToArq(NULL, T, t);
   adicaoarq(p, 1);
   libera(T);

}




void imprimearq(){
   FILE *fp2 = fopen("indices.dat", "rb");
   if(!fp2) exit(1);
   FILE *fp3 = fopen("pizzas.dat", "rb");
   if(!fp3) exit(1);
   int control, indices[3];
   TPizza *p = (TPizza*)malloc(tamanho_pizza_bytes());;
   control = 2;
   while(control>1){
     control = fread(indices, sizeof(int), 3, fp2);
     if(indices[2]){
     printf("indice -> %d posicao -> %d\n", indices[0], indices[1]);
     fseek(fp3, indices[1], SEEK_SET);
     p = le_pizza(fp3);
     imprime_pizza(p);}
     }
  free(p);
  fclose(fp2);
  fclose(fp3);
}




int main(void) {
  TABM *T = geraarv(2, NULL);
  ToArq(NULL, T, 2);
  TPizza *p = pizza(9,"????? tchau", "salgada", 10.0);
  adicao(2, p);
  p = pizza(134,"????? meio daskjldha", "salgada", 10.0);
  adicao(2, p);
  p = pizza(150,"????? oi", "salgada", 10.0);
  adicao(2, p);
  imprimearq();
  /*imprime(T,0);
  ToArq(NULL, T, 2);
  libera(T);
  TPizza *p = pizza(11,"calabresa", "salgada", 10.0);
  adicao(2, p);
  T = completo(NULL, 2);
  imprime(T, 0);*/


  printf("\nacabou");


  return 0;
}



