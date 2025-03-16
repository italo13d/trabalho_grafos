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

int insereAresta(Grafo *G, int u, int v, float w)
{
    // Valida se os vértices estão no intervalo 1..G->nro_vertices
    if (u < 1 || u > G->nro_vertices || v < 1 || v > G->nro_vertices)
    {
        printf("Vértices inválidos.\n");
        return 0;
    }

    // Converte os vértices de 1-based para 0-based
    int u_index = u - 1;
    int v_index = v - 1;

    // Insere a aresta (u, v)
    No *novo = malloc(sizeof(No));
    if (novo == NULL)
    {
        printf("Erro ao alocar nó para a aresta.\n");
        return 0;
    }
    novo->destino = v_index;
    novo->peso = w;
    novo->prox = G->adj[u_index];
    G->adj[u_index] = novo;

    No *novoReverso = malloc(sizeof(No));
    if (novoReverso == NULL)
    {
        printf("Erro ao alocar nó para a aresta reversa.\n");
        return 0;
    }
    novoReverso->destino = u_index;
    novoReverso->peso = w;
    novoReverso->prox = G->adj[v_index];
    G->adj[v_index] = novoReverso;

    return 1;
}

int contaArestas(Grafo *G)
{
    int total = 0;

    for (int i = 0; i < G->nro_vertices; i++)
    {
        No *temp = G->adj[i];
        while (temp != NULL)
        {
            total++;
            temp = temp->prox;
        }
    }

    // Como o grafo é não-direcionado, o número real de arestas é divido por 2
    return total / 2;
}

int removeAresta(Grafo *G, int u, int v)
{
    // Validação dos vértices com base em 1 ao invés de 0 do array original
    if (u < 1 || u > G->nro_vertices || v < 1 || v > G->nro_vertices)
    {
        printf("Vértices inválidos.\n");
        return 0;
    }

    // Converte para índices baseado em 0 do array original
    int u_index = u - 1;
    int v_index = v - 1;

    // Remover aresta(u, v)
    No *prev = NULL, *curr = G->adj[u_index];
    int removido = 0;
    while (curr != NULL)
    {
        if (curr->destino == v_index)
        {
            // Encontrou a aresta e a remove
            if (prev == NULL)
            {
                G->adj[u_index] = curr->prox; // Achou o primeiro nó
            }
            else
            {
                prev->prox = curr->prox;
            }
            free(curr);
            removido = 1;
            break;
        }
        prev = curr;
        curr = curr->prox;
    }

    // Se não encontrar a aresta (u, v), retorna 0
    if (!removido)
    {
        return 0;
    }

    // Removendo a aresta simétrica(v,u) pelo grafo não ser dígrafo(dirigido ou direcionado)
    prev = NULL;
    curr = G->adj[v_index];
    removido = 0;
    while (curr != NULL)
    {
        if (curr->destino == u_index)
        {
            if (prev == NULL)
            {
                G->adj[v_index] = curr->prox;
            }
            else
            {
                prev->prox = curr->prox;
            }
            free(curr);
            removido = 1;
            break;
        }
        prev = curr;
        curr = curr->prox;
    }

    // Avisa se a aresta reversa não for encontrada( a rede deve estar consistente)
    if (!removido)
    {
        printf("Aresta reversa não encontrada.\n");
        return 0;
    }
    return 1;
}

int grau(Grafo *G, int v)
{
    if (v < 1 || v > G->nro_vertices)
        return -1;

    // Converte o índice baseado em 0
    int v_index = v - 1;

    int cont = 0;
    No *atual = G->adj[v_index];
    while (atual != NULL)
    {
        cont++;
        atual = atual->prox;
    }
    return cont;
}

int grauMedio(Grafo *G)
{
    // Verifica se o grafo é válido e possui vértices
    if (G == NULL || G->nro_vertices == 0)
    {
        return -1;
    }

    int somaGraus = 0;

    // Percorre cada lista de adjacência para cada vértice
    for (int i = 0; i < G->nro_vertices; i++)
    {
        No *atual = G->adj[i];
        while (atual != NULL)
        {
            somaGraus++;
            atual = atual->prox;
        }
    }

    // Retorna a média dos graus (divisão inteira)
    return somaGraus / G->nro_vertices;
}

int grauMax(Grafo *G, int *v)
{
    // Verifica se o grafo é válido e possui vértices
    if (G == NULL || G->nro_vertices == 0)
    {
        return -1;
    }

    int grauMax = -1;
    int indiceMaiorVertice = -1;

    // Percorre todas as listas de adjacência, como na função de grau médio
    for (int i = 0; i < G->nro_vertices; i++)
    {
        int contador = 0;
        No *atual = G->adj[i];
        while (atual != NULL)
        {
            contador++;
            atual = atual->prox;
        }
        // Se o grau deste vértice for maior que o máximo atual, atualiza os valores
        if (contador > grauMax)
        {
            grauMax = contador;
            indiceMaiorVertice = i;
        }
    }

    // Armazena em *v o vértice com maior grau (convertendo da numeração de arrays para baseada em 1)
    if (v != NULL)
    {
        *v = indiceMaiorVertice + 1;
    }

    return grauMax;
}

int verificaConexaBfs(Grafo *G, int start, int *visitado)
{
    int n = G->nro_vertices;
    int *fila = malloc(n * sizeof(int));
    if (fila == NULL)
    {
        printf("Erro ao alocar memória para a fila.\n");
        exit(1);
    }
    int front = 0, rear = 0;

    fila[rear++] = start;
    visitado[start] = 1;
    int compSize = 1;

    while (front < rear)
    {
        int u = fila[front++];
        No *node = G->adj[u];
        while (node != NULL)
        {
            int v = node->destino;
            if (visitado[v] == 0)
            {
                visitado[v] = 1;
                fila[rear++] = v;
                compSize++;
            }
            node = node->prox;
        }
    }
    free(fila);
    return compSize;
}

int validaConexas(Grafo *G, int *tamMax)
{
    if (G == NULL || G->nro_vertices == 0)
    {
        if (tamMax)
            *tamMax = 0;
        return 0;
    }

    int n = G->nro_vertices;
    int *visitado = (int *)calloc(n, sizeof(int));
    if (visitado == NULL)
    {
        printf("Erro ao alocar memória para visitado.\n");
        exit(1);
    }

    int compCont = 0;
    int maxTam = 0;
    for (int i = 0; i < n; i++)
    {
        if (visitado[i] == 0)
        {
            int compTam = verificaConexaBfs(G, i, visitado);
            compCont++;
            if (compTam > maxTam)
                maxTam = compTam;
        }
    }
    free(visitado);
    if (tamMax)
        *tamMax = maxTam;
    return compCont;
}

int validaMenorCaminhoBfs(Grafo *G, int start, int *dist)
{
    int n = G->nro_vertices;
    // Inicializa todas as distâncias com -1 (não alcançado)
    for (int i = 0; i < n; i++)
    {
        dist[i] = -1;
    }

    int *fila = malloc(n * sizeof(int));
    if (fila == NULL)
    {
        printf("Erro ao alocar memória para a fila.\n");
        exit(1);
    }
    int front = 0, rear = 0;

    fila[rear++] = start;
    dist[start] = 0;

    while (front < rear)
    {
        int u = fila[front++];
        No *node = G->adj[u];
        while (node != NULL)
        {
            int v = node->destino;
            if (dist[v] == -1)
            {
                dist[v] = dist[u] + 1;
                fila[rear++] = v;
            }
            node = node->prox;
        }
    }

    free(fila);
    return 1;
}