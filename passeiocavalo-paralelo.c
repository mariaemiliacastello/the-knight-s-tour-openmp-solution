#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>

//#define N 4
//#define M 8
int N, M, threads;
void print_tabuleiro(int tabuleiro[N][M]){
    int i, j;
    for (i=0; i < N; i++){
        for (j=0; j < M; j++)
            printf("%3d ",tabuleiro[i][j]);
        printf("\n");
    }
}

int jogada_valida(int x, int y, int tabuleiro[N][M]){
    if (x < 0  || x >= N || y < 0 || y >= M)
        return 0;
    if(tabuleiro[x][y] != 0)
        return 0;
    return 1;
}
int proximo_movimento_y(int y, int movimento){
    int valor = 1;
    if( movimento < 5 )
        valor = 2;
    if (movimento % 2 == 0) // se par, eh uma subtracao
        return y - valor;
    else
        return y + valor;
    
}
int proximo_movimento_x(int x, int movimento){
    if (movimento < 3)
        return x + 1;
    else if (movimento < 5)
        return x - 1;
    else if (movimento < 7)
        return x + 2;
    else 
        return x - 2;
}
int passeio_cavalo(int tabuleiro[N][M], int x, int y, int jogada){
    int x2, y2, i;
    if (jogada == N*M)
        return 1;

    for (i=1;i<9;i++){
        x2 = proximo_movimento_x(x,i);
        y2 = proximo_movimento_y(y,i);
        if (jogada_valida(x2,y2, tabuleiro)){
            tabuleiro[x2][y2] = jogada+1;
            if (passeio_cavalo(tabuleiro, x2,y2, jogada+1))
                return 1;
            tabuleiro[x2][y2] = 0;
        }
    }

    return 0;
}
int le_tamanho(int argc, char **argv) {
        for (int i = 0; i < argc - 1; i++)
                if (strcmp (argv[i], "-t") == 0)
                        return atoi(argv[i+1]);
        return 5;
}

int le_threads(int argc, char **argv) {
        for (int i = 0; i < argc - 1; i++)
                if (strcmp (argv[i], "-p") == 0)
                        return atoi(argv[i+1]);
        return 6;
}
int main(int argc, char *argv[]){
	int i, j;
    	N = le_tamanho(argc, argv);
    	M=N;
	threads = le_threads(argc, argv);
    	clock_t start, end;
    	double cpu_time_used;
	if(threads > N*M)
		threads = N*M;
    	start = clock();
    	printf("Resolvendo para N=%d e M=%d\n",N,M);
	#pragma omp set_dynamic(0) //desliga o ajuste dinamico de threads
    	#pragma omp parallel num_threads(threads) //definir o numero de threads
    	{
		int tabuleiro[N][M];
    		for (i=0; i < N; i++)
        		for (j=0; j < M; j++)
            			tabuleiro[i][j] = 0;
    		tabuleiro[(omp_get_thread_num()) % N][(omp_get_thread_num()) / N] = 1; //pegar coluna por coluna
		if (passeio_cavalo(tabuleiro, (omp_get_thread_num()) % N, (omp_get_thread_num()) / N, 1)){
			#pragma omp critical //imprime o primeiro que chegar
			{
        			print_tabuleiro(tabuleiro);
				end = clock();
        			cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        			printf("%f seconds\n",cpu_time_used);
    				exit(0);
			}
    		}
    		else{
			#pragma omp critical //imprime que nao tem uma solucao somente uma vez
			{
        			printf("Nao existe solucao\n");
				end = clock();
                                cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
                                printf("%f seconds\n",cpu_time_used);
				exit(0);
			}
		}
    	}
    	end = clock();
    	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    	printf("%f seconds\n",cpu_time_used);
}
