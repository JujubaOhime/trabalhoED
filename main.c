#include <stdio.h>
#include "TC/bm.h"
#include "TC/TLSE.h"
#include <string.h>
#include <limits.h>

///func 1, dado um arquivo e o fator de ramificacao, gera a arvore;
// gcc main.c -o main bm.o pizza.o

///funcoes que geram arvore e da reboot nos dados indices
TABM * geraArv(int t, char*nome){///gera dos dados iniciais
    TABM * T = inicializa();
    if(!nome) nome = "TC/dados_iniciais.dat";
    FILE *fp = fopen(nome, "rb");
    FILE *fp2 = fopen("indices.dat", "wb");
    FILE *fp3 = fopen("pizzas.dat", "wb");
    if(!fp) exit(1);
    if(!fp2) exit(1);
    if(!fp3) exit(1);
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

// le tenta abrir um arq 
TABM * le(FILE *fp, int t, int booleano, char *c){
  rewind(fp);
  int aux;
  int aux2;
  fread(&aux, sizeof(int), 1, fp);
  fread(&aux2, sizeof(int), 1, fp);
  if(t != aux){
    fclose(fp);
    if(booleano == 0)free(c);
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

// t param. arvore, boolean , c é o nome, 
//1->T 2->BOOLEAN FOLHA 3->....N->chaves
//gerencia a string do nome do arq
//abre só um e bota todos filhos como nulo a partir de le
TABM * openf(char *c,int t){
  int booleano = 1;
  
  if(!c){
    c = (char*)malloc(100*(sizeof(char)));
    strcpy(c, "");
    c = strcat(c, "ARQ/raiz");
    booleano = 0;
    }
  char *str =(char *)malloc(100*sizeof(char));
  strcpy(str, c);
  int i;
  strcat(str, ".dat");
  FILE *fp = fopen(str, "rb");
  if(!fp) return NULL;
  TABM * novo = le(fp, t, booleano, c);
  fclose(fp);
  if(booleano == 0)free(c);
  return novo;
}

//pega arq por arq e transforma em árvore
//raiz>0>1.dat
/* 
  4

3 
    1
  2
    0

    T = 2   ate 4 filhos
    raiz>0
    raiz>1
    raiz>2
    raiz>3
*/

//le a arv em disco e coloca na mP
TABM * completo(char *c, int t){///le do arquivo das arvs
  int booleano = 1;
  if(!c){
    c = (char*)malloc(100*(sizeof(char)));
    strcpy(c, "");
    c = strcat(c, "ARQ/raiz");
    booleano = 0;
    }
  char *str =(char *)malloc(100*sizeof(char));
  strcpy(str, c);
  strcat(str, ".dat");
  FILE *fp = fopen(str, "rb");
  if(!fp) return NULL;
  TABM * a = le(fp, t, booleano, c);
  if(!a)return a;
  for(int i = 0; i<2*t; i++){
    char aux[10];
    sprintf(aux, "_%d", i); 
    strcpy(str, c);    
    strcat(str, aux);
    a->filho[i] = completo(str, t);
  }
  free(str);
  if(booleano == 0)free(c);
  return a;
}


//pega o arquivo índice e vai gerar uma árvore do 0 a partir dele
TABM * geraArvArq(int t){///gera do arquivo indices
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
//tentava achar um filho que era folha, e ia chamando pelo while
//em desuso por poder achar um filho sem folha
void imprimeLista(TABM *T, int t){
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
  T = 
  
  T->prox;
  }
}

//igual imprimelist mas ele coloca em arq.
//em desuso
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

//passa um nome, arv e gera todos arquivos da arvore
//raiz_0 raiz_1 raiz_2
//pega a arv da ram pra disco. SalvaArvore
void paraArq(char*c, TABM* arv, int t){
  int booleano = 1;
  if(!arv)return;
  if(!c){
    c = (char*)malloc(100*(sizeof(char)));
    strcpy(c, "");
    c = strcat(c, "ARQ/raiz");
    booleano = 0;
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
  if(booleano == 0)free(c);
}


// gera uma arv. em mp a partir dos arqs a partir do indice 
TABM * abrecond(char *c, int t, int indice){
  int booleano = 1;
  if(!c){
    c = (char*)malloc(100*(sizeof(char)));
    strcpy(c, "");
    c = strcat(c, "ARQ/raiz");
    booleano = 0;
    }
  char *str =(char *)malloc(100*sizeof(char));
  strcpy(str, c);
  strcat(str, ".dat");
  FILE *fp = fopen(str, "rb");
  if(!fp) return NULL;
  TABM * a = le(fp, t, booleano, c);
  if(!a)return a;
  int i = 0;
  while ((i < a->nchaves) && (indice > a->chave[i])) i++;
  if(i == 0){ //filho que to tentando abrir é menor que a primeira chave, abrir primeiro filho
    printf("caso1\n e menor que %d\n", a->chave[0]);
    char aux[10];
    sprintf(aux, "_%d", i); 
    strcpy(str, c);    
    strcat(str, aux);
    a->filho[i] = abrecond(str,t,indice);
    sprintf(aux, "_%d", i+1); 
    strcpy(str, c);    
    strcat(str, aux);
    a->filho[i+1] = openf(str,t);
  }
  else if(i == a->nchaves){ // maior qua a ultima chave, acessa o último filho
    printf("caso2\ne maior que %d\n", a->chave[a->nchaves-1]);
    char aux[10];
    sprintf(aux, "_%d", i); 
    strcpy(str, c);    
    strcat(str, aux);
    a->filho[i] =abrecond(str,t,indice);
    sprintf(aux, "_%d", i-1); 
    strcpy(str, c);    
    strcat(str, aux);
    a->filho[i-1] = openf(str,t);
  }
  else{ //do meio
    printf("caso3\nesta entre %d e %d\n", a->chave[i], a->chave[i-1]);
    char aux[10];
    sprintf(aux, "_%d", i); 
    strcpy(str, c);    
    strcat(str, aux);
    a->filho[i] = abrecond(str,t,indice);
    sprintf(aux, "_%d", i+1); 
    strcpy(str, c);    
    strcat(str, aux);
    a->filho[i+1] = openf(str,t);
    sprintf(aux, "_%d", i-1); 
    strcpy(str, c);    
    strcat(str, aux);
    a->filho[i-1] = openf(str,t);
  }
  free(str);
  if(booleano == 0)free(c);
  return a;
}

//mexe no arq de índices
//arq indices: info 1:indice da pizza info 2:ponteiro da pizza em pizza.dat info 3:is alive
void adicaoarq(TPizza *p){
  FILE *fp2 = fopen("indices.dat", "rb+");
  if(!fp2) exit(1);
  rewind(fp2);
  FILE *fp3 = fopen("pizzas.dat", "rb+");
  if(!fp3) exit(1);
  int indices[3];
  int controle = 1;
  indices[0] = -1; indices[1] = -1;indices[2] = -1;
    while(indices[0]< p->cod && controle>0)controle = fread(indices, sizeof(int), 3, fp2);
    if(indices[0] == p->cod){
      fseek(fp2, -3*sizeof(int), SEEK_CUR);
      indices[2] = 1;
      fwrite(fp2, sizeof(int), 3, fp2);
      fseek(fp3, indices[1], SEEK_SET);
      salva_pizza(p, fp3);
      fclose(fp2);
      fclose(fp3);
      return;
    }
    int indicesaux[3];
    fseek(fp3, 0, SEEK_END);
    indicesaux[0] = p->cod; indicesaux[1] = ftell(fp3); indicesaux[2] = 1;
    salva_pizza(p, fp3);
    fseek(fp2, -3*sizeof(int), SEEK_CUR);
    fwrite(indicesaux, sizeof(int), 3, fp2);
    indicesaux[0] = indices[0]; indicesaux[1] = indices[1]; indicesaux[2] = indices[2];
    while(controle > 0){
      controle = fread(indices, sizeof(int), 3, fp2);
      fseek(fp2, -3*sizeof(int), SEEK_CUR);
      fwrite(indicesaux, sizeof(int), 3, fp2);
      if(controle == 3){indicesaux[0] = indices[0]; indicesaux[1] = indices[1]; indicesaux[2] = indices[2];}
    }
  fclose(fp2);
  fclose(fp3);
}



//adiciona uma pizza na arv na mp e no arq
TABM * adicao(int t, TPizza *p){
   TABM *T = abrecond(NULL, t, p->cod);
   printf("\n\n\n");

   if(busca(T, p->cod)){
     adicaoarq(p);
     return T;
   }
   T = insere(T, p, t);
   paraArq(NULL, T, t);
   adicaoarq(p);
   return T;
}


//3 int 1 indice 2 ponteiro 3 is alive
//fseek(fp, ponteiro, SEEK_SET)
//le_pizza
/*135, Milho com Bacon (Salgada), R$ 32.00
137, Frango, Milho e Palmito (Salgada), R$ 27.00
*/
void imprimeArq(){
   FILE *fp2 = fopen("indices.dat", "rb");
   if(!fp2) exit(1);
   FILE *fp3 = fopen("pizzas.dat", "rb");
   if(!fp3) exit(1);
   int control, indices[3];
   TPizza *p = (TPizza*)malloc(tamanho_pizza_bytes());;
   control = 2;
   while(control>1){
     control = fread(indices, sizeof(int), 3, fp2);
     if (control<=1) break;
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



TPizza * buscaPizza(int indice){
  FILE *fp2 = fopen("indices.dat", "rb+");
  if(!fp2) exit(1);
  rewind(fp2);
  FILE *fp3 = fopen("pizzas.dat", "rb+");
  if(!fp3) exit(1);
  int indices[3];
  int controle = 1;
  TPizza *p = (TPizza *)malloc(tamanho_pizza_bytes());
  indices[0] = INT_MIN; indices[1] = INT_MIN;indices[2] = INT_MIN;
  while(indices[0]< indice && controle>0){controle = fread(indices, sizeof(int), 3, fp2);
  }
  if(indices[0] == indice && indices[2]){
    fseek(fp3, indices[1], SEEK_SET);
    p = le_pizza(fp3);
  }
  else{
     free(p);
     p = NULL;
  }
  fclose(fp2);
  fclose(fp3);
  return p;
}

void imprimeIndice(int indice){
  TPizza * p = buscaPizza(indice);
  if(p){imprime_pizza(p);
  free(p);}
  else printf("pizza nao encontrada >:)\n");
  
}


TLSE * buscat(char * c){
  FILE *fp2 = fopen("indices.dat", "rb+");
  if(!fp2) exit(1);
  rewind(fp2);
  FILE *fp3 = fopen("pizzas.dat", "rb+");
  if(!fp3) exit(1);
  int indices[3];
  int controle = 1;
  TLSE *cat = NULL;
  TPizza *p = (TPizza *)malloc(tamanho_pizza_bytes());
  indices[0] = INT_MIN; indices[1] = INT_MIN;indices[2] = INT_MIN;
  while(controle>0){
    controle = fread(indices, sizeof(int), 3, fp2);
    if(controle>0){
      fseek(fp3, indices[1], SEEK_SET);
      p = le_pizza(fp3);
      if(strcmp(p->descricao,c) == 0)cat = inserel(cat, p->cod);
    }
  }
  free(p);
  p = NULL;
  fclose(fp2);
  fclose(fp3);
  return cat;
}



void imprimeCat(char *c){
  TLSE *T = buscat(c);
  printf("\n\n");
  for(int i = 0; i < tamLista(T) ;i++){
  TPizza *p = buscaPizza(buscaIndice(T, i));
  imprime_pizza(p);
  }
}




int main(void) {
  TABM *T = geraArv(2, NULL);
  imprimeArq();
  imprime(T,0);
  paraArq(NULL, T, 2);
  libera(T);
  
  while(1){
    char aux[3];
    printf("Deseja adicionar alguma pizza?(s/n) \n");
    scanf(" %s", aux);
    if(strcmp(aux, "n") == 0) break;
    char nome[50];
    char tipo[20];
    int cod;
    float preco;
    printf("Qual o nome da pizza?\n");
    scanf(" %[^\n]", nome);
    printf("Qual o tipo da pizza?\n");
    scanf(" %[^\n]", tipo);
    printf("Escreva seu preco\n");
    scanf(" %f",&preco);
    printf("Escreva o código da pizza \n");
    scanf(" %d",&cod);
    TPizza *p = pizza(cod,nome,tipo,preco);
    T = adicao(2, p);
    imprime(T, 0);
  }

  imprimeArq();

  return 0;
}