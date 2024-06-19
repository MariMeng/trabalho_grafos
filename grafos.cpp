#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <vector>
using namespace std;
// Estrutura de fila

// Estrutura para o nó na lista de adjacência
typedef struct No
{
    int vertex;
    struct No *prox;
} No;

// Estrutura para o grafo
typedef struct Grafo
{
    int numVertices;
    int *visitado;
    int **matrizAdj;
    No **adjLists;
} Grafo;

// criando No
No *createNo(int vertex)
{
    No *novoNo = (No *)malloc(sizeof(No));
    novoNo->vertex = vertex;
    novoNo->prox = NULL;
    return novoNo;
}
// função para criar grafo
Grafo *creategrafo(int vertices)
{
    Grafo *grafo = (Grafo *)malloc(sizeof(Grafo));
    grafo->numVertices = vertices;

    // iniciando a lista de adjacencia:

    grafo->adjLists = (No **)malloc(vertices * sizeof(No *));
    grafo->visitado = (int *)malloc(vertices * sizeof(int));
    for (int i = 0; i < vertices; i++)
    {
        grafo->adjLists[i] = NULL;
        grafo->visitado[i] = 0;
    }
    // iniciando a matriz de adjacencia:

    /*grafo->matrizAdj = (int **)malloc(vertices * sizeof(int *));
    for (int i = 0; i < vertices; i++)
    {
        grafo->matrizAdj[i] = (int *)malloc(vertices * sizeof(int));
    }
    for (int i = 0; i < vertices; i++)
    {
        for (int j = 0; j < vertices; j++)
        {
            grafo->matrizAdj[i][j] = 0;
        }
    }*/

    return grafo;
}
void addEdge(Grafo *grafo, int src, int dest)
{
    // Lista de adjacências:
    //  Adiciona aresta de src para dest
    No *newNo = createNo(dest);
    newNo->prox = grafo->adjLists[src - 1]; // Ajustando para índice base 0
    grafo->adjLists[src - 1] = newNo;

    // Adiciona aresta de dest para src (para grafos não direcionados)
    newNo = createNo(src);
    newNo->prox = grafo->adjLists[dest - 1];
    grafo->adjLists[dest - 1] = newNo;

    // Matriz de adjacências:
    /*
    if(grafo->matrizAdj[src][dest] == 0)
    {
        grafo->matrizAdj[src][dest] = 1;  //   Adiciona aresta de src para dest
        grafo->matrizAdj[dest][src] = 1;  // Adiciona aresta de dest para src (para grafos não direcionados)
    }*/
}
// lendo grafo do arquivo
void readgrafoFromFile(const char *filename, Grafo **grafo)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    int vertices;
    fscanf(file, "%d", &vertices);

    *grafo = creategrafo(vertices);

    int src, dest;
    while (fscanf(file, "%d %d", &src, &dest) != EOF)
    {
        addEdge(*grafo, src, dest);
    }

    fclose(file);
}
// imprimindo  o grafo
void printgrafo(Grafo *grafo)
{
    for (int v = 0; v < grafo->numVertices; v++)
    {
        No *temp = grafo->adjLists[v];
        printf("\n Lista de adjacencia do vertice %d\n head ", v + 1);
        while (temp)
        {
            printf("-> %d", temp->vertex);
            temp = temp->prox;
        }
        printf("\n");
    }
}
// Imprimir o grafo como Matriz de Adjacências
void imprimirMatriz(Grafo *grafo, int src, int dest)
{
    // Inicializando a matriz de adjacências
    grafo->matrizAdj = (int **)malloc(grafo->numVertices * sizeof(int *));
    for (int i = 0; i < grafo->numVertices; i++)
    {
        grafo->matrizAdj[i] = (int *)malloc(grafo->numVertices * sizeof(int));
    }
    for (int i = 0; i < grafo->numVertices; i++)
    {
        for (int j = 0; j < grafo->numVertices; j++)
        {
            grafo->matrizAdj[i][j] = 0;
        }
    }
    // Atualizando as arestas da matriz
    if (grafo->matrizAdj[src][dest] == 0)
    {
        grafo->matrizAdj[src][dest] = 1; //   Adiciona aresta de src para dest
        grafo->matrizAdj[dest][src] = 1; // Adiciona aresta de dest para src (para grafos não direcionados)
    }
    // Imprimindo a Matriz de Adjacências
    printf("Matriz de Adjacências: \n");
    for (int i = 0; i < grafo->numVertices; i++)
    {
        for (int j = 0; j < grafo->numVertices; j++)
        {
            printf("%d ", grafo->matrizAdj[i][j]);
        }
        printf("\n");
    }
}

// grau maximo
int grauMaximo(Grafo *grafo)
{
    int grauMax = -1;                            // numero pequeno
    for (int i = 0; i < grafo->numVertices; i++) // for para percorrer todas as vertices do grafo
    {
        int grauAtual = 0; // soma do vértice da vez
        No *aux = grafo->adjLists[i];
        while (aux) // for para percorrer a lista de arestas de cada vertice, percorre enquanto houver na lista de adj
        {
            grauAtual++;     // somando as vértices
            aux = aux->prox; // andando para o próximo vértice da lista de adjacencia
        }
        if (grauAtual > grauMax) // comparação, se a soma do vértice for maior que o grauMax, então agora ele é o grauMax.
            grauMax = grauAtual;
        // retorna todo o loop;
    }
    return grauMax; // retorna o grau máximo
}
// grau medio
float grauMedio(Grafo *grafo)
{
    int somaVertices = 0;
    for (int i = 0; i < grafo->numVertices; i++)
    {
        No *aux = grafo->adjLists[i];
        while (aux)
        {
            somaVertices++;
            aux = aux->prox;
        }
    }
    float grauMed = somaVertices / grafo->numVertices;
    return grauMed;
}

// num de vertice
int numVertices(Grafo *grafo)
{
    return grafo->numVertices;
}

// função para comparar inteiros usada na ordenação q sort
int comparar(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b); // retornando a -b
}
// mediana do grau
float medianaDoGrau(Grafo *grafo)
{
    int num = numVertices(grafo);
    int *grausArray = (int *)malloc(grafo->numVertices * sizeof(int));

    for (int i = 0; i < num; i++) // percorrendo os vertices
    {
        int somaVertice = 0;
        No *aux = grafo->adjLists[i];
        while (aux) // percorrendo as arestas dos vertices
        {
            somaVertice++; // iterando a quantidade de cada vertice da aresta
            aux = aux->prox;
        }
        grausArray[i] = somaVertice; // colocando o grau do vértice do array;
    }
    qsort(grausArray, grafo->numVertices, sizeof(int), comparar); // ordenando o array com q sort
    printf("Testando se array esta ordenado: \n");                // teste, apagar depois;
    for (int i = 0; i < num; i++)
    {
        printf(" %d ", grausArray[i]);
    }

    double medianaGrau;
    int meio = num / 2;
    if (num % 2 == 0)

        medianaGrau = ((grausArray[meio] - 1) + grausArray[meio]) / 2;
    else
        medianaGrau = grausArray[meio];

    free(grausArray);
    return medianaGrau;
}

// num de aresta
int numArestas(Grafo *grafo)
{
    int soma = 0;
    for (int i = 0; i < grafo->numVertices; i++)
    {
        No *aux = grafo->adjLists[i];
        while (aux)
        {
            soma++;
            aux = aux->prox;
        }
    }
    return soma;
}

// grau minimo
int grauMinimo(Grafo *grafo)
{
    int grauMin = 100000;                        // Número grande
    for (int i = 0; i < grafo->numVertices; i++) // for para percorrer todas as vértices do grafo
    {
        int grauAtual = 0; // soma do grau do vértice da vez
        No *aux = grafo->adjLists[i];
        while (aux) // for para percorrer a lista de arestas de cada vertice, percorre enquanto houver na lista de adj
        {
            grauAtual++;     // iterando
            aux = aux->prox; // andando para o próximo vértice da lista de adjacencia
        }
        if (grauAtual < grauMin) // comparação, se a soma do numero de vértices for menor que o grauMin, então agora ele é o grauMin.
            grauMin = grauAtual;
        // retorna todo o loop;
    }
    return grauMin; // retorna o grau mínimo
}

// Representação dos grafos
// ja tem a lista de adjacencia pronta, só falta a matriz
// te mandei um video q achei de o cra fazendo, se te ajudar

//    BUSCAS EM PROFUNDIDADE E LARGURA

// Implementação da busca em profundidade

void buscaEmProfundidade(Grafo *grafo, int verticeInicial)
{
    No *lista = grafo->adjLists[verticeInicial];
    No *aux = lista;

    grafo->visitado[verticeInicial] = 1; // Marcando como visitado
    printf("Visitado %d\n", verticeInicial);

    while (aux != NULL)
    {
        int verticeAdjacente = aux->vertex - 1;

        if (grafo->visitado[verticeAdjacente] == 0)
        {
            buscaEmProfundidade(grafo, verticeAdjacente);
        }
        aux = aux->prox;
    }
}

void liberarGrafo(Grafo *grafo)
{
    if (grafo != NULL)
    {
        for (int i = 0; i < grafo->numVertices; i++)
        {
            No *atual = grafo->adjLists[i];
            while (atual)
            {
                No *aux = atual;
                free(aux);
                aux = aux->prox;
            }
        }
    }
    free(grafo);
}
// Implementação da busca por largura

// DISTANCIAS E DIAMETRO
// função para calulcar a distancia entre dois vertices do grafo

void bfsEntreDoisVertices(Grafo *grafo, int vert1, int vert2)
{
}

int main()
{
    const char *filename = "grafo_2.txt"; // Nome do arquivo de entrada
    Grafo *grafo = NULL;

    readgrafoFromFile(filename, &grafo);
    printgrafo(grafo);
    printf("Numero de arestas: %d", numArestas(grafo));
    printf("\nResultado do grau minimo: %d\n", grauMinimo(grafo));
    printf("Resultado do grau max %d\n", grauMaximo(grafo));
    printf("Resutado grau medio %f\n", grauMedio(grafo));
    printf("Resultado mediana do grau %f\n", medianaDoGrau(grafo));
    printf("Busca em profundidade: \n");
    buscaEmProfundidade(grafo, 3);
    printf("\nGrafo em matriz:\n");
    // imprimirMatriz(grafo);
    liberarGrafo(grafo);
    // Liberar memória (não mostrado aqui)
    return 0;
}
