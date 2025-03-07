#include <stdio.h>
#include <stdlib.h>
#include "Grafo.h"
#include <string.h>

typedef struct no
{
    int destino;
    float peso;
    struct no *prox;
} No;

struct grafo
{
    int eh_ponderado;
    int nro_vertices;
    int limite_de_vertices;
    char **labels;
    No **adj;
};

Grafo *cria_Grafo(int limite_inicial, int eh_ponderado)
{
    int i;
    Grafo *G = malloc(sizeof(Grafo));
    if (G == NULL)
    {
        printf("Erro ao alocar o grafo. \n");
        exit(1);
    }
    G->eh_ponderado = eh_ponderado;
    G->nro_vertices = 0;
    G->limite_de_vertices = limite_inicial;

    // Aloca o vetor de labels
    G->labels = malloc(limite_inicial * sizeof(char *));
    if (G->labels == NULL)
    {
        printf("Erro ao alocar o vetor de labels.\n");
        exit(1);
    }

    // Aloca o vetor de listas de adjacência
    G->adj = malloc(limite_inicial * sizeof(No *));
    if (G->adj == NULL)
    {
        printf("Erro ao alocar o vetor de adjacência.\n");
        exit(1);
    }
    for (i = 0; i < limite_inicial; i++)
    {
        G->adj[i] = NULL;
    }
    return G;
}

int getLimiteVertices(Grafo *G)
{
    return G->limite_de_vertices;
}

int getNumeroVertices(Grafo *G)
{
    return G->nro_vertices;
}

void libera_Grafo(Grafo *G)
{
    if (G == NULL)
    {
        return;
    }
    // Libera cada lista de adjacência e cada label
    for (int i = 0; i < G->limite_de_vertices; i++)
    {
        No *atual = G->adj[i];
        while (atual != NULL)
        {
            No *temp = atual;
            atual = atual->prox;
            free(temp);
        }
        free(G->labels[i]);
    }
    free(G->labels);
    free(G->adj);
    free(G);
    printf("Grafo liberado");
}

int insereVertice(Grafo *G, char *label)
{
    if (G->nro_vertices >= G->limite_de_vertices)
    {
        int novo_limite = G->limite_de_vertices + (G->limite_de_vertices / 2);
        char **novos_labels = realloc(G->labels, novo_limite * sizeof(char *));
        if (novos_labels == NULL)
        {
            printf("Erro ao realocar o vetor de labels.\n");
            return 0;
        }
        G->labels = novos_labels;

        No **nova_adj = realloc(G->adj, novo_limite * sizeof(No *));
        if (nova_adj == NULL)
        {
            printf("Falha ao realocar o vetor de adjacência.\n");
            return 0;
        }
        G->adj = nova_adj;

        // Inicializa as novas posições como NULL
        for (int i = G->nro_vertices; i < novo_limite; i++)
        {
            G->adj[i] = NULL;
        }
        G->limite_de_vertices = novo_limite;
    }
    // Armazena o label(duplica a string para não depender do buffer externo)
    G->labels[G->nro_vertices] = strdup(label);
    if (G->labels[G->nro_vertices] == NULL)
    {
        printf("Erro ao duplicar o label.\n");
        return 0;
    }
    G->nro_vertices++;
    return 1;
}

// Recebe um ponteiro para grafo, dois vértices e um peso, que pode ser 0
int insereAresta(Grafo *G, int u, int v, float w)
{
    if (u < 0 || u >= G->nro_vertices || v < 0 || v >= G->nro_vertices)
    {
        printf("Vértices inválidos.\n");
        return 0;
    }
    // Cria o novo nó para a aresta de u para v
    No *novo = malloc(sizeof(No));
    if (novo == NULL)
    {
        printf("Erro ao alocar nó para a aresta.\n");
        return 0;
    }
    novo->destino = v;
    novo->peso = w;

    novo->prox = G->adj[u];
    G->adj[u] = novo;

    return 1;
}
