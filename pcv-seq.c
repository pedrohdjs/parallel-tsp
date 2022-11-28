/**
 * Utilize o comando "make seq" para compilar esse código
 * O código será compilado no binário pcv, que pode ser
 * executado com "./pcv N"
*/

#include <stdio.h>
#include <stdlib.h>

/*
****************** Constantes ******************
*/

#define MAX_COST 50 //Peso máximo de uma aresta

/*
*********** Utilidades para matrizes ***********
*/

/**
 * Aloca dinamicamente uma matriz
 * 
 * @param n o número de linhas da matriz
 * @param m o número de colunas da matriz
 * 
 * @return a matriz alocada
*/
int** new_matrix(int n, int m){
    int** matrix = (int**)malloc(n*sizeof(int*));
    for(int i = 0; i < n; i++){
        matrix[i] = (int*)malloc(m*sizeof(int));
    }

    return matrix;
}

/**
 * Aloca dinamicamente uma matriz quadrada
 * (mesmo número de linhas e colunas)
 * 
 * @param n o número de linhas e colunas
 * 
 * @returns a matriz alocada
*/
int** new_square_matrix(int n){
    return new_matrix(n,n);
}

/**
 * Libera o espaço utilizado por uma matriz
 * 
 * @param matrix a matriz a ser liberada
 * @param n o número de linhas da matriz
 * 
 * @returns void
*/
void delete_matrix(int** matrix, int n){
    for(int i = 0; i < n; i++){
        free(matrix[i]);
    }
    free(matrix);
}

/**
 * Imprime uma matriz, para fins de debug
 * 
 * @param matrix a matriz a ser impressa
 * @param n o número de linhas da matriz 
 * @param m o número de colunas da matriz
 * 
 * @returns void
*/
void print_matrix(int** matrix, int n, int m){
    for(int i = 0; i<n; i++){
        for(int j = 0; j<m; j++){
            printf("%02d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

/*
********* Funções do problema principal *********
*/

/**
 * Gera uma matriz de custos para o grafo
 * 
 * @param n o número de vértices do grafo
 * 
 * @returns a matriz de custos das arestas
*/
int** get_cost_matrix(int n){
    int** matrix = new_square_matrix(n);
    for(int i = 0; i<n; i++){
        for(int j = 0; j<n; j++){
            matrix[i][j] = rand()%(MAX_COST+1);
        }
        matrix[i][i] = 0;
    }

    return matrix;
}


int main(int argc, char** argv){
    if(argc < 2){
        printf("O número de cidades não foi especificado. Execute o programa com \"./pcv N\", onde N é o número de cidades.\n");
        return 1;
    }

    int n = atoi(argv[1]);
    int** costs = get_cost_matrix(n);
    print_matrix(costs,n,n);
    delete_matrix(costs,n);

    return 0;
}