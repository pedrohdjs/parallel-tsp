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
#define PATH_LIST_EMPTY -1

typedef struct _path { //Um caminho
    int nodes[MAX_PATH_SIZE]; //Os nós no caminho
    int cost; //Custo do caminho até então
    int size; //Número de nós no caminho
} path;

typedef struct _path_list{ //Uma lista de caminhos alocados dinamicamente. Os caminhos nessa lista sempre devem ter o mesmo custo.
    path** paths;
    int size;
    int _actual_size;
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
    matrix = NULL;
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
    p = NULL;
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

/**
 * Imprime um caminho
 * 
 * @param p o caminho
 * 
 * @returns void
*/
void print_path(path* p){
    for(int i = 0; i<p->size-1; i++){
        printf("%d -> ", p->nodes[i]);
    }
    printf("%d\n", p->nodes[p->size-1]);
}

/*
*********** Utilidades para path lists **********
*/

/**
 * Aloca uma nova path list dinamicamente
 * 
 * @returns uma path list nova e vazia, alocada dinamicamente
*/
path_list* new_path_list(){
    path_list* pl = (path_list*)malloc(1*sizeof(path_list));
    pl->paths = (path**)malloc(PATH_LIST_SIZE*sizeof(path*));
    pl->size = 0;
    pl->_actual_size = PATH_LIST_SIZE;
}


/**
 * Libera o espaço de uma path list, sem liberar
 * os paths dentro dela
 * 
 * @param pl a path list a ser liberada
 * 
 * @returns void
*/
void delete_path_list(path_list* pl){
    free(pl->paths);
    free(pl);
    pl = NULL;
}


/**
 * Libera os paths dentro de uma path list,
 * sem liberar a path list
 * 
 * @param pl a path list que terá os paths associados liberados
 * 
 * @returns void
*/
void delete_path_list_paths(path_list* pl){
    for(int i = 0; i < pl->size; i++){
        delete_path(pl->paths[i]);
    }
    pl->size = 0;
}

/**
 * Concatena um path a uma path list, alocando
 * mais memória para a path list se necessário
 * 
 * @param pl a path list a receber o path
 * @param p o path a ser incluso na path list
 * 
 * @returns void
*/
void concatenate_to_path_list(path_list* pl, path* p){
    if(pl->size == pl->_actual_size){ //Aloca espaço adicional se necessário
        int new_size = 2*pl->_actual_size;
        pl->paths = realloc(pl->paths, new_size*sizeof(path*));
        pl->_actual_size = new_size;
    }


    pl->paths[pl->size++] = p;
}

/**
 * Retorna o custo dos paths em uma path list ou
 * PATH_LIST_EMPTY caso a path list esteja vazia.
 * Vale ressaltar que uma path list deve apenas
 * armazenar paths com o mesmo exato custo.
 * 
 * @param pl a path list
 * @param adj a matriz de adjacências do grafo
 * 
 * @returns o custo do primeiro caminhoa na path list ou PATH_LIST_EMPTY, se a path list estiver vazia
*/
int get_path_list_paths_cost(path_list* pl, int** adj){
    if(pl->size == 0){
        return PATH_LIST_EMPTY;
    }

    return get_path_cost(pl->paths[0], adj);
}

/**
 * Junta duas path lists em uma
 * 
 * @param dest uma das path lists, onde o resultado ficará armazenado
 * @param other a outra path list, que terá seus paths concatenados à path_list dest e será liberada
 * 
 * @returns void
*/
void merge_path_lists(path_list* dest, path_list* other){
    for(int i = 0; i<other->size; i++){
        concatenate_to_path_list(dest, other->paths[i]);
    }
    delete_path_list(other);
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

/**
 * Resolve o problema para um dado n, uma lista de adjacências e o caminho inicial.
 * Esse algoritmo é uma busca em profundidade.
 * 
 * @param n o número de nós no grafo
 * @param adj a lista de adjacências do grafo, com os pesos
 * @param initial_path o caminho inicial. Esse caminho será deletado pela função após utilizado.
 * 
 * @returns uma lista de caminhos com o menor custo
*/
path_list* solve_problem(int n, int** adj, path* initial_path){
    //printf("Solve problem chamado para o caminho: ");
    //print_path(initial_path);
    if(initial_path->size == n){ //Caso base
        concatenate_to_path(initial_path, STARTING_NODE);
        path* res = copy_path(initial_path);
        path_list* pl = new_path_list();
        concatenate_to_path_list(pl,res);
        return pl;
    }
    else{
        //Lista de caminhos visitados
        int visited[MAX_GRAPH_SIZE];
        for(int i = 0; i < MAX_GRAPH_SIZE; i++){
            visited[i] = 0;
        }

        for(int i = 0; i<initial_path->size; i++){
            visited[initial_path->nodes[i]] = 1;
        }

        path_list** pll = (path_list**)malloc(n*sizeof(path_list*)); //Lista de todos os path lists gerados
        int min_cost = __INT_MAX__; //Custo mínimo dos caminhos

        for(int i = 0; i<n; i++){
            pll[i] = new_path_list();

            if(!visited[i]){ //Somente processar path lists geradas a partir de um nó não visitado
                path* p = copy_path(initial_path);
                concatenate_to_path(p,i);

                pll[i] = solve_problem(n, adj, p);
            }

            // Obtém o custo mínimo dos caminhos possíveis
            int cost = get_path_list_paths_cost(pll[i], adj);
            if(cost != PATH_LIST_EMPTY && cost < min_cost){
                min_cost = cost;
            }
        }

        // O caminho inicial não tem mais uso
        delete_path(initial_path);

        path_list* res = new_path_list(); //A path list que armazena a resposta

        for(int i = 0; i<n; i++){
            if(get_path_list_paths_cost(pll[i], adj) == min_cost){
                merge_path_lists(res,pll[i]);
                
            }
            else{
                delete_path_list_paths(pll[i]); //Esses caminhos não serão mais utilizados
                delete_path_list(pll[i]);
            }
        }

        free(pll);
        return res;
    }
}

/**
 * Imprime a resposta
 * 
 * @param pl a path list da solução
 * @param adj a matriz de adjacências do grafo
 * @param n a ordem do grafo do problema
 * 
 * @returns void
*/
void print_answer(path_list* pl, int** adj, int n){
    int cost = get_path_list_paths_cost(pl, adj);

    printf("Matriz de adjacências: \n");
    print_matrix(adj, n, n);
    
    printf("\nCaminhos encontrados, com custo %d: \n", cost);
    for(int i = 0; i<pl->size; i++){
        path* p = pl->paths[i];

        print_path(p);
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
    path* initial_path = new_path();
    concatenate_to_path(initial_path, STARTING_NODE);

    path_list* res = solve_problem(n,costs, initial_path);
    print_answer(res, costs, n);

    delete_path_list_paths(res);
    delete_path_list(res);
    delete_matrix(costs,n);

    return 0;
}