nome_executavel = teste.exe
codigo_fonte = bm.c pizza.c TLSE.c main.c
compilador = gcc

run:
	$(compilador) $(codigo_fonte) -o $(nome_executavel)
	@ echo "\033c\n"
	@ ./$(nome_executavel)
