/**
 * Utilize o comando "make seq" para compilar esse código
 * O código será compilado no binário pcv, que pode ser
 * executado com "./pcv N"
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
****** Constantes e definições de tipos ********
*/

#define MAX_COST 50 //Peso máximo de uma aresta
#define MAX_GRAPH_SIZE 32 //Tamanho maximo do grafo
#define MAX_PATH_SIZE (MAX_GRAPH_SIZE+1) //Tamanho máximo de um caminho
#define STARTING_NODE 0
#define COST_NOT_COMPUTED 0
#define PATH_LIST_SIZE 32

typedef struct _path { //Um caminho
    int nodes[MAX_PATH_SIZE]; //Os nós no caminho
    int cost; //Custo do caminho até então
    int size; //Número de nós no caminho
} path;

typedef struct _path_list{ //Uma lista de caminhos alocados dinamicamente. Os caminhos nessa lista sempre devem ter o mesmo custo.
    path** paths;
} path_list;

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
************* Utilidades para paths *************
*/

/**
 * Cria um novo path vazio
 * 
 * @returns um path alocado dinamicamente
*/
path* new_path(){
    path* p = (path*)malloc(1*sizeof(path));
    for(int i = 0; i<MAX_PATH_SIZE; i++){
        p->nodes[i] = -1;
    }
    p->cost = COST_NOT_COMPUTED;
    p->size = 0;
    return p;
}

/**
 * Libera a memória alocada para p
 * 
 * @param p o path a ser desalocado
*/
void delete_path(path* p){
    free(p);
}

/**
 * Cria uma cópia de um path alocada dinamicamente
 * 
 * @param original o path a ser copiado
 * @param n o número máximo de nós no caminho
 * 
 * @returns uma cópia de original
*/
path* copy_path(path* original){
    path* p = new_path();
    memcpy(p->nodes, original->nodes, MAX_PATH_SIZE*sizeof(int));
    p->size = original->size;
    p->cost = original->cost;
    return p;
}

/**
 * Adiciona o nó n ao fim da lista de nós do path p
 * O tamanho da lista (p->size) é atualizado, mas
 * o custo (p->cost) não.
 * 
 * @param p o caminho
 * @param n o vértice a ser concatenado à lista
 * 
 * @returns void
 * 
*/
void concatenate_to_path(path* p, int n){
    p->nodes[p->size++] = n;
}

/**
 * Obtém o custo de um path com base na matriz
 * de adjacências passada. Se o custo já tiver
 * sido calculado, ele apenas é retornado.
 * 
 * @param p o path
 * @param adj a matriz de adjacências
 * 
 * @returns o custo do caminho p, de acordo com adj
*/
int get_path_cost(path* p, int** adj){
    if(p->cost == COST_NOT_COMPUTED){
        for(int i = 0; i < (p->size-1); i++){
            p->cost += adj[p->nodes[i]][p->nodes[i+1]];
        }
    }

    return p->cost;
}

/*
*********** Utilidades para path lists **********
*/

/*
TODO:
- criar path list
- liberar path list
- criar path list com um nó só
- obter o custo de todos os elementos na path list
- fundir duas path lists
*/


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

/**
 * Resolve o problema para um dado n, uma lista de adjacências e o caminho inicial
 * 
 * @param n o número de nós no grafo
 * @param adj a lista de adjacências do grafo, com os pesos
 * @param initial_path o caminho inicial
 * 
 * @returns uma lista de caminhos com o menor custo
 * 
*/
path_list solve_problem(int n, int** adj, path* initial_path){
    if(initial_path->size == n){ //Caso base
        concatenate_to_path(initial_path, STARTING_NODE);
        //TODO: retorna lista com só o caminho inicial
    }
    else{
        //Lista de dados
        int visited[MAX_GRAPH_SIZE];
        for(int i = 0; i < MAX_GRAPH_SIZE; i++){
            visited[i] = 0;
        }

        for(int i = 0; i<initial_path->size; i++){
            visited[initial_path->nodes[i]] = 1;
        }

        //TODO: adicionar todos não visitados
        //e então obter a lista de todos com o
        //menor custo, liberar as path lists
        //de caminhos mais caros que esse menor custo,
        //mergear as restantes e retornar
    }
}

int main(int argc, char** argv){
    if(argc < 2){
        printf("O número de cidades não foi especificado. Execute o programa com \"./pcv N\", onde N é o número de cidades.\n");
        return 1;
    }

    int n = atoi(argv[1]);

    if(n + 1 > MAX_GRAPH_SIZE){
        printf("O N passado é maior que o limite do programa. Isso pode ser resolvido se modificando o macro MAX_GRAPH_SIZE, na linha 16, e recompilando o programa. \n");
        return 1;
    }

    int** costs = get_cost_matrix(n);
    path* initial_path = new_path;
    concatenate_to_path(initial_path, STARTING_NODE);

    solve_problem(n,costs, initial_path);

    delete_path(initial_path);
    delete_matrix(costs,n);

    return 0;
}