#include <string.h>
#include <stdlib.h>
#include "pizza.h"

void imprime_pizza(TPizza *p)
{
	printf("%d, %s (%s), R$ %.2f\n", p->cod, p->nome, p->descricao, p->preco);
}


TPizza *pizza(int cod, char *nome, char *descricao, float preco)
{
	TPizza *p = (TPizza *) malloc(sizeof(TPizza));
	if (p) memset(p, 0, sizeof(TPizza));
	p->cod = cod;
	strcpy(p->nome, nome);
	strcpy(p->descricao, descricao);
	p->preco = preco;
	return p;
}


void salva_pizza(TPizza *p, FILE *out)
{
	fwrite(&p->cod, sizeof(int), 1, out);
	fwrite(p->nome, sizeof(char), sizeof(p->nome), out);
	fwrite(p->descricao, sizeof(char), sizeof(p->descricao), out);
	fwrite(&p->preco, sizeof(float), 1, out);
}


TPizza *le_pizza(FILE *in)
{
	TPizza *p = (TPizza *) malloc(sizeof(TPizza));
	if (0 >= fread(&p->cod, sizeof(int), 1, in)) {
		free(p);
		return NULL;
	}
	fread(p->nome, sizeof(char), sizeof(p->nome), in);
	fread(p->descricao, sizeof(char), sizeof(p->descricao), in);
	fread(&p->preco, sizeof(float), 1, in);
	return p;
}


int cmp_pizza(TPizza *p1, TPizza *p2)
{
	if (p1 == NULL) {
		return (p2 == NULL);
	}
	if (p1->cod != p2->cod) {
		return 0;
	}
	if (strcmp(p1->nome, p2->nome) != 0) {
		return 0;
	}
	if (strcmp(p1->descricao, p2->descricao) != 0) {
		return 0;
	}
	if (p1->preco != p2->preco) {
		return 0;
	}
	return 1;
}


int tamanho_pizza_bytes()
{
	return sizeof(int) + // cod
		sizeof(char) * 50 + // nome
		sizeof(char) * 20 + // categoria
		sizeof(float); // preço
}
