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
    No *novoNo = criarNo(dest);
    novoNo->prox = grafo->adjLists[src];
    grafo->adjLists[src] = novoNo;
    novoNo = criarNo(src);
    novoNo->prox = grafo->adjLists[dest];
    grafo->adjLists[dest] = novoNo;
}
// lendo grafo do arquivo
void lerDoArquivo(const char *filename, Grafo **grafo)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    int vertices;
    fscanf(file, "%d", &vertices);

    *grafo = criarGrafo(vertices);

    int src, dest;
    while (fscanf(file, "%d %d", &src, &dest) != EOF)
    {
        adicionarAresta(*grafo, src - 1, dest - 1);
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
            printf("-> %d", temp->vertex + 1);
            temp = temp->prox;
        }
        printf("\n");
    }
}
// Imprimir o grafo como Matriz de Adjacências
void imprimirMatriz(Grafo *grafo)
{
    // Inicializando a matriz de adjacências
    int **matrizAdj = (int **)malloc(grafo->numVertices * sizeof(int *)); // alocando matriz
    for (int i = 0; i < grafo->numVertices; i++)
    {
        matrizAdj[i] = (int *)calloc(grafo->numVertices, sizeof(int)); // alocando vertices
    }

    for (int i = 0; i < grafo->numVertices; i++) // preenchendo as martrizes de adjacencia
    {
        No *aux = grafo->adjLists[i];
        while (aux)
        {
            matrizAdj[i][aux->vertex] = 1;
            aux = aux->prox;
        }
    }
    // Imprimindo a Matriz de Adjacências
    printf("Matriz de Adjacencias: \n");
    for (int i = 0; i < grafo->numVertices; i++)
    {
        for (int j = 0; j < grafo->numVertices; j++)
        {
            printf("%d ", matrizAdj[i][j]);
        }
        printf("\n");
    }
    // liberando matriz
    for (int i = 0; i < grafo->numVertices; i++)
    {
        free(matrizAdj[i]);
    }
    free(matrizAdj);
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
    No *lista = grafo->adjLists[verticeInicial];
    No *aux = lista;

    grafo->visitado[verticeInicial] = 1; // Marcando como visitado
    printf("Visitado %d\n", verticeInicial + 1);

    while (aux != NULL)
    {
        int verticeAdjacente = aux->vertex;

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
    free(grafo->adjLists);
    free(grafo->visitado);
    free(grafo);
}

void buscaPorLargura(Grafo *grafo, int verticeInicial)
{
    Fila *fila = criarFila(); // Função para criar a fila

    grafo->visitado[verticeInicial] = 1;
    enqueue(fila, verticeInicial);
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

// Implementação da busca por largura

// DISTANCIAS E DIAMETRO
// função para calulcar a distancia entre dois vertices do grafo
// Função para calcular o diâmetro do grafo
// int calcularDiametro(struct Graph* graph) {
//   int diametro = 0;

//   // Para cada vértice do grafo, calcula a distância máxima para todos os outros vértices
//   for (int i = 0; i < graph->numVertices; i++) {
//     // Array para armazenar as distâncias mínimas a partir do vértice i
//     int distancias[graph->numVertices];
//     for (int j = 0; j < graph->numVertices; j++) {
//       distancias[j] = -1; // -1 indica que o vértice não foi alcançado
//     }

//     // BFS a partir do vértice i para calcular as distâncias mínimas
//     struct queue* q = createQueue();
//     distancias[i] = 0; // A distância do vértice para si mesmo é zero
//     enqueue(q, i);

//     while (!isEmpty(q)) {
//       int currentVertex = dequeue(q);
//       struct node* temp = graph->adjLists[currentVertex];

//       while (temp) {
//         int adjVertex = temp->vertex;

//         if (distancias[adjVertex] == -1) { // Verifica se o vértice adjacente não foi visitado
//           distancias[adjVertex] = distancias[currentVertex] + 1;
//           enqueue(q, adjVertex);
//         }

//         temp = temp->next;
//       }
//     }

//     // Encontra a maior distância calculada para o vértice i
//     for (int j = 0; j < graph->numVertices; j++) {
//       if (distancias[j] > diametro) {
//         diametro = distancias[j];
//       }
//     }
//   }

//   return diametro;
// }

// Função para calcular a distância entre dois vértices específicos
int calcularDistancia(Grafo *grafo, int startVertex, int targetVertex)
{
    // Array para armazenar as distâncias mínimas a partir do vértice startVertex
    int num = numVertices(grafo);
    int distancias[5];
    for (int j = 0; j < grafo->numVertices; j++)
    {
        distancias[j] = -1; // -1 indica que o vértice não foi alcançado
    }

    // BFS a partir do vértice startVertex para calcular as distâncias mínimas
    struct Fila *q = criarFila();
    distancias[startVertex] = 0; // A distância de startVertex para si mesmo é zero
    enqueue(q, startVertex);

    while (!filaVazia(q))
    {
        int currentVertex = dequeue(q);
        struct No *temp = grafo->adjLists[currentVertex];

        while (temp)
        {
            int adjVertex = temp->vertex;

            if (distancias[adjVertex] == -1)
            { // Verifica se o vértice adjacente não foi visitado
                distancias[adjVertex] = distancias[currentVertex] + 1;
                enqueue(q, adjVertex);
            }

            // Se chegarmos ao vértice targetVertex, podemos retornar a distância
            if (adjVertex == targetVertex)
            {
                return distancias[targetVertex];
            }

            temp = temp->prox;
        }
    }

    // Se o targetVertex não foi alcançado, retorna -1 indicando que não há caminho
    return -1;
}
int main()
{
    const char *filename = "grafo_2.txt"; // Nome do arquivo de entrada
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
    imprimirMatriz(grafo);

    liberarGrafo(grafo);

    // Liberar memória (não mostrado aqui)
    return 0;
}
