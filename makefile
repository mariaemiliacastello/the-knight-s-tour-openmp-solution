CC = gcc -O3

all: passeiocavalo-paralelo passeiocavalo

passeiocavalo-paralelo: passeiocavalo-paralelo.c
	$(CC) -o passeiocavalo-paralelo -fopenmp passeiocavalo-paralelo.c 

passeiocavalo: passeiocavalo.c
	$(CC) -o passeiocavalo passeiocavalo.c 
run: passeiocavalo passeiocavalo-paralelo

clean:
	@rm -rf passeiocavalo passeiocavalo-paralelo
