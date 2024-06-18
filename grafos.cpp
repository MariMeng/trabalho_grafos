#include <stdio.h>
#include <stdlib.h>

// Estrutura para o nó na lista de adjacência
typedef struct Node
{
    int vertex;
    struct Node *next;
} Node;

// Estrutura para o grafo
typedef struct Graph
{
    int numVertices;
    Node **adjLists;
} Graph;

Node *createNode(int vertex)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->vertex = vertex;
    newNode->next = NULL;
    return newNode;
}
Graph *createGraph(int vertices)
{
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->numVertices = vertices;

    graph->adjLists = (Node **)malloc(vertices * sizeof(Node *));

    for (int i = 0; i < vertices; i++)
        graph->adjLists[i] = NULL;

    return graph;
}
void addEdge(Graph *graph, int src, int dest)
{
    // Adiciona aresta de src para dest
    Node *newNode = createNode(dest);
    newNode->next = graph->adjLists[src - 1]; // Ajustando para índice base 0
    graph->adjLists[src - 1] = newNode;

    // Adiciona aresta de dest para src (para grafos não direcionados)
    newNode = createNode(src);
    newNode->next = graph->adjLists[dest - 1];
    graph->adjLists[dest - 1] = newNode;
}
void readGraphFromFile(const char *filename, Graph **graph)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    int vertices;
    fscanf(file, "%d", &vertices);

    *graph = createGraph(vertices);

    int src, dest;
    while (fscanf(file, "%d %d", &src, &dest) != EOF)
    {
        addEdge(*graph, src, dest);
    }

    fclose(file);
}
void printGraph(Graph *graph)
{
    for (int v = 0; v < graph->numVertices; v++)
    {
        Node *temp = graph->adjLists[v];
        printf("\n Lista de adjacencia do vertice %d\n head ", v + 1);
        while (temp)
        {
            printf("-> %d", temp->vertex);
            temp = temp->next;
        }
        printf("\n");
    }
}
int main()
{
    const char *filename = "grafo_1.txt"; // Nome do arquivo de entrada
    Graph *graph = NULL;

    readGraphFromFile(filename, &graph);
    printGraph(graph);

    // Liberar memória (não mostrado aqui)
    return 0;
}
