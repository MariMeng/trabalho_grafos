#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#define TAM 10000
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

typedef struct Fila
{
    int itens[TAM];
    int frente;
    int final;
} Fila;

// fila vazia
int filaVazia(Fila *fila)
{
    return (fila->final == -1);
}
// criando fila
Fila *criarFila()
{
    Fila *fila = (Fila *)malloc(sizeof(Fila));
    fila->final = -1;
    fila->frente = -1;
    return fila;
}
// remover elementos da fila
int dequeue(Fila *fila)
{
    if (filaVazia(fila))
    {
        printf("Fila está vazia\n");
        return -1; // Indicando erro
    }
    else
    {
        int item = fila->itens[fila->frente];
        fila->frente++;
        if (fila->frente > fila->final)
        {
            fila->frente = fila->final = -1;
        }
        return item;
    }
}
// adicionar elementos na fila
void enqueue(Fila *fila, int valor)
{
    if (fila->final == TAM - 1)
    {
        printf("Fila cheia\n");
    }
    else
    {
        if (fila->frente == -1)
        {
            fila->frente = 0;
        }
        fila->final++;
        fila->itens[fila->final] = valor;
    }
}

void liberarfila(Fila *fila)
{
    while (!filaVazia(fila))
    {
        dequeue(fila);
    }
    free(fila);
}
// criando No
No *criarNo(int vertex)
{
    No *novoNo = (No *)malloc(sizeof(No));
    novoNo->vertex = vertex;
    novoNo->prox = NULL;
    return novoNo;
}
// função para criar grafo
Grafo *criarGrafo(int vertices)
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
    return grafo;
}

void adicionarAresta(Grafo *grafo, int src, int dest)
{
    No *novoNo = criarNo(dest - 1);
    novoNo->prox = grafo->adjLists[src - 1];
    grafo->adjLists[src - 1] = novoNo;

    novoNo = criarNo(src - 1);
    novoNo->prox = grafo->adjLists[dest - 1];
    grafo->adjLists[dest - 1] = novoNo;
}
// lendo grafo do arfilauivo
void lerDoArquivo(const char *filename, Grafo **grafo)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Erro ao abrir o arfilauivo");
        exit(EXIT_FAILURE);
    }

    int vertices;
    fscanf(file, "%d", &vertices);

    *grafo = criarGrafo(vertices);

    int src, dest;
    while (fscanf(file, "%d %d", &src, &dest) != EOF)
    {
        adicionarAresta(*grafo, src, dest);
    }

    fclose(file);
}
// imprimindo  o grafo
void printgrafo(Grafo *grafo)
{
    for (int v = 0; v < grafo->numVertices; v++)
    {
        No *aux = grafo->adjLists[v];
        printf("\n Lista de adjacencia do vertice %d\n head ", v + 1);
        while (aux)
        {
            printf("-> %d", aux->vertex + 1);
            aux = aux->prox;
        }
        printf("\n");
    }
}
// Imprimir o grafo como Matriz de Adjacências
void imprimirMatriz(Grafo *grafo)
{
    // Verificações de segurança
    if (grafo == NULL || grafo->numVertices <= 0 || grafo->adjLists == NULL)
    {
        printf("Grafo inválido\n");
        return;
    }

    // Alocação da matriz de adjacências
    int **matrizAdj = (int **)malloc(grafo->numVertices * sizeof(int *));
    if (matrizAdj == NULL)
    {
        perror("Erro ao alocar matriz de adjacências");
        return;
    }

    for (int i = 0; i < grafo->numVertices; i++)
    {
        matrizAdj[i] = (int *)calloc(grafo->numVertices, sizeof(int));
        if (matrizAdj[i] == NULL)
        {
            perror("Erro ao alocar vetor da matriz de adjacências");
            // Liberar memória alocada até agora
            for (int j = 0; j < i; j++)
            {
                free(matrizAdj[j]);
            }
            free(matrizAdj);
            return;
        }
    }

    // Preenchimento da matriz de adjacências
    for (int i = 0; i < grafo->numVertices; i++)
    {
        No *aux = grafo->adjLists[i];
        while (aux != NULL)
        {
            matrizAdj[i][aux->vertex] = 1;
            aux = aux->prox;
        }
    }

    // Impressão da Matriz de Adjacências
    printf("Matriz de Adjacências:\n");
    for (int i = 0; i < grafo->numVertices; i++)
    {
        for (int j = 0; j < grafo->numVertices; j++)
        {
            printf("%d ", matrizAdj[i][j]);
        }
        printf("\n");
    }

    // Liberação da memória alocada para a matriz de adjacências
    for (int i = 0; i < grafo->numVertices; i++)
    {
        free(matrizAdj[i]);
    }
    free(matrizAdj);
}

// grau maximo
int grauMaximo(Grafo *grafo)
{
    int grauMax = -1;                            // numero pefilaueno
    for (int i = 0; i < grafo->numVertices; i++) // for para percorrer todas as vertices do grafo
    {
        int grauAtual = 0; // soma do vértice da vez
        No *aux = grafo->adjLists[i];
        while (aux) // for para percorrer a lista de arestas de cada vertice, percorre enfilauanto houver na lista de adj
        {
            grauAtual++;     // somando as vértices
            aux = aux->prox; // andando para o próximo vértice da lista de adjacencia
        }
        if (grauAtual > grauMax) // comparação, se a soma do vértice for maior filaue o grauMax, então agora ele é o grauMax.
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

// função para comparar inteiros usada na ordenação fila sort
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
            somaVertice++; // iterando a filauantidade de cada vertice da aresta
            aux = aux->prox;
        }
        grausArray[i] = somaVertice; // colocando o grau do vértice do array;
    }
    qsort(grausArray, grafo->numVertices, sizeof(int), comparar); // ordenando o array com fila sort
    printf("Testando se array esta ordenado: \n");                // teste, apagar depois;
    for (int i = 0; i < num; i++)
    {
        printf(" %d ", grausArray[i]);
    }

    double medianaGrau;
    int meio = num / 2;
    if (num % 2 == 0)

        medianaGrau = ((grausArray[meio - 1]) + grausArray[meio]) / 2;
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
        while (aux) // for para percorrer a lista de arestas de cada vertice, percorre enfilauanto houver na lista de adj
        {
            grauAtual++;     // iterando
            aux = aux->prox; // andando para o próximo vértice da lista de adjacencia
        }
        if (grauAtual < grauMin) // comparação, se a soma do numero de vértices for menor filaue o grauMin, então agora ele é o grauMin.
            grauMin = grauAtual;
        // retorna todo o loop;
    }
    return grauMin; // retorna o grau mínimo
}

// Representação dos grafos

//    BUSCAS EM PROFUNDIDADE E LARGURA

// Implementação da busca em profundidade
void resetarVisitados(Grafo *grafo)
{
    for (int i = 0; i < grafo->numVertices; i++)
    {
        grafo->visitado[i] = 0;
    }
}
void buscaEmProfundidade(Grafo *grafo, int verticeInicial)
{
    No *lista = grafo->adjLists[verticeInicial - 1];
    No *aux = lista;

    grafo->visitado[verticeInicial - 1] = 1; // Marcando como visitado
    printf("Visitado %d\n", verticeInicial);

    while (aux != NULL)
    {
        int verticeAdjacente = aux->vertex;

        if (grafo->visitado[verticeAdjacente] == 0)
        {
            buscaEmProfundidade(grafo, verticeAdjacente + 1);
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
            while (atual != NULL)
            {
                No *aux = atual;
                atual = atual->prox;
                free(aux);
            }
        }
    }
    free(grafo->adjLists);
    free(grafo->visitado);
    free(grafo);
}

void buscaPorLargura(Grafo *grafo, int verticeInicial)
{
    Fila *fila = criarFila(); // Função para criar a fila

    grafo->visitado[verticeInicial - 1] = 1;
    enqueue(fila, verticeInicial - 1);
    while (!filaVazia(fila))
    {
        int verticeAtual = dequeue(fila);
        printf("Visitado %d\n", verticeAtual + 1);

        No *aux = grafo->adjLists[verticeAtual];

        while (aux)
        {
            int verticeAdj = aux->vertex;
            if (grafo->visitado[verticeAdj] == 0)
            {
                grafo->visitado[verticeAdj] = 1;
                enqueue(fila, verticeAdj);
            }
            aux = aux->prox;
        }
    }
    liberarfila(fila); // Função para liberar a fila
}

// Função para calcular a distância entre dois vértices específicos
int calcularDistancia(Grafo *grafo, int verticeInicial, int verticeFinal)
{
    int origem = verticeInicial - 1;
    int destino = verticeFinal - 1;
    if (origem < 0 || origem >= grafo->numVertices || destino < 0 || destino >= grafo->numVertices)
    {
        return -1;
    }
    Fila *fila = criarFila();
    int *distancia = (int *)malloc(grafo->numVertices * sizeof(int));
    for (int i = 0; i < grafo->numVertices; i++)
    {
        distancia[i] = -1; // -1 indica filaue o vértice não foi alcançado
    }
    grafo->visitado[origem] = 1;
    distancia[origem] = 0; // A distância de origem para si mesmo é zero
    enqueue(fila, origem);

    while (!filaVazia(fila))
    {
        int verticeAtual = dequeue(fila);
        No *aux = grafo->adjLists[verticeAtual];

        while (aux)
        {
            int adjVertex = aux->vertex;

            if (distancia[adjVertex] == -1)
            { // Verifica se o vértice adjacente não foi visitado
                grafo->visitado[adjVertex] = 1;
                distancia[adjVertex] = distancia[verticeAtual] + 1;
                enqueue(fila, adjVertex);
            }
            aux = aux->prox;
        }
    }

    // Se o destino não foi alcançado, retorna -1 indicando filaue não há caminho
    liberarfila(fila);
    resetarVisitados(grafo);
    return distancia[destino];
}

int calcularDiamentro(Grafo *grafo)
{
    int resultado = 0;
    for (int i = 0; i < grafo->numVertices; i++)
    {
        for (int j = 0; j < grafo->numVertices; j++)
        {
            int distancia = calcularDistancia(grafo, i, j);
            if (distancia > resultado)
                resultado = distancia;
        }
    }
    return resultado;
}

int main()
{
    const char *filename = "grafo_2.txt"; // Nome do arfilauivo de entrada
    Grafo *grafo = NULL;

    lerDoArquivo(filename, &grafo);
    printgrafo(grafo);

    printf("Numero de arestas: %d", numArestas(grafo));
    printf("\nResultado do grau minimo: %d\n", grauMinimo(grafo));
    printf("Resultado do grau max %d\n", grauMaximo(grafo));
    printf("Resutado grau medio %f\n", grauMedio(grafo));
    printf("Resultado mediana do grau %f\n", medianaDoGrau(grafo));
    printgrafo(grafo);
    printf("Buca por Largura:\n");
    buscaPorLargura(grafo, 3);
    resetarVisitados(grafo);
    printf("Busca em profundidade: \n");
    buscaEmProfundidade(grafo, 3);
    printf("\nGrafo em matriz:\n");
    printf("Dsitancia entre duas vertices no grafo: %d\n", calcularDistancia(grafo, 1, 6));
    printf("Diametro do grafo: %d", calcularDiamentro(grafo));

    // imprimirMatriz(grafo);

    liberarGrafo(grafo);

    // Liberar memória (não mostrado afilaui)
    return 0;
}
