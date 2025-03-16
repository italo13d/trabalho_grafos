#include <stdio.h>
#include "grafo.h"
#include <locale.h>
#include <wchar.h>
#include <stdlib.h>

int main()
{
    setlocale(LC_ALL, "Portuguese_Brazil");
    Grafo *g = cria_Grafo(16, 1);
    int verticeMax;

    if (g == NULL)
    {
        printf("Erro na criação do grafo.\n");
        return 1;
    }

    char *labels[16] = {"1", "2", "3", "4",
                        "5", "6", "7", "8",
                        "9", "10", "11", "12",
                        "13", "14", "15", "16"};

    // Insere os 16 vértices do grafo( cada vértice é representado por sua posição, começando em 1 e indo até 16)
    for (int i = 0; i < 16; i++)
    {
        insereVertice(g, labels[i]);
    }

    // Insere as 58 arestas, mapeando as relações entre os vértices
    insereAresta(g, 1, 2, 1);
    insereAresta(g, 1, 3, -1);
    insereAresta(g, 2, 3, -1);
    insereAresta(g, 1, 4, -1);
    insereAresta(g, 3, 4, 1);
    insereAresta(g, 1, 5, -1);
    insereAresta(g, 2, 5, -1);
    insereAresta(g, 1, 6, -1);
    insereAresta(g, 2, 6, -1);
    insereAresta(g, 3, 6, 1);
    insereAresta(g, 3, 7, 1);
    insereAresta(g, 5, 7, 1);
    insereAresta(g, 6, 7, 1);
    insereAresta(g, 3, 8, 1);
    insereAresta(g, 4, 8, 1);
    insereAresta(g, 6, 8, 1);
    insereAresta(g, 7, 8, 1);
    insereAresta(g, 2, 9, -1);
    insereAresta(g, 5, 9, 1);
    insereAresta(g, 6, 9, -1);
    insereAresta(g, 2, 10, -1);
    insereAresta(g, 9, 10, 1);
    insereAresta(g, 6, 11, 1);
    insereAresta(g, 7, 11, 1);
    insereAresta(g, 8, 11, 1);
    insereAresta(g, 9, 11, -1);
    insereAresta(g, 10, 11, -1);
    insereAresta(g, 1, 12, -1);
    insereAresta(g, 6, 12, 1);
    insereAresta(g, 7, 12, 1);
    insereAresta(g, 8, 12, 1);
    insereAresta(g, 11, 12, 1);
    insereAresta(g, 6, 13, -1);
    insereAresta(g, 7, 13, 1);
    insereAresta(g, 9, 13, 1);
    insereAresta(g, 10, 13, 1);
    insereAresta(g, 11, 13, -1);
    insereAresta(g, 5, 14, 1);
    insereAresta(g, 8, 14, -1);
    insereAresta(g, 12, 14, -1);
    insereAresta(g, 13, 14, 1);
    insereAresta(g, 1, 15, 1);
    insereAresta(g, 2, 15, 1);
    insereAresta(g, 5, 15, -1);
    insereAresta(g, 9, 15, -1);
    insereAresta(g, 10, 15, -1);
    insereAresta(g, 11, 15, -1);
    insereAresta(g, 12, 15, -1);
    insereAresta(g, 13, 15, -1);
    insereAresta(g, 1, 16, 1);
    insereAresta(g, 2, 16, 1);
    insereAresta(g, 5, 16, -1);
    insereAresta(g, 6, 16, -1);
    insereAresta(g, 11, 16, -1);
    insereAresta(g, 12, 16, -1);
    insereAresta(g, 13, 16, -1);
    insereAresta(g, 14, 16, -1);
    insereAresta(g, 15, 16, 1);

    int maxGrau = grauMax(g, &verticeMax);

    wprintf(L"Limite de vértices: %d\n", getLimiteVertices(g));
    wprintf(L"Numero atual de vértices: %d\n", getNumeroVertices(g));
    wprintf(L"Número atual de arestas: %d\n", contaArestas(g));
    wprintf(L"Grau do vértice: %d\n", grau(g, 8));
    wprintf(L"Grau médio do grafo: %d\n", grauMedio(g));
    wprintf(L"O maior grau é %d e está no vértice %d\n", maxGrau, verticeMax);
    int tamMax;
    int numComp = validaConexas(g, &tamMax);
    wprintf(L"Número de componentes conexas: %d\n", numComp);
    wprintf(L"Tamanho da maior componente: %d\n", tamMax);

    int *dist = malloc(getNumeroVertices(g) * sizeof(int));
    if (dist == NULL)
    {
        wprintf(L"Erro ao alocar memória para as distâncias.\n");
        libera_Grafo(g);
        return 1;
    }
    if (validaMenorCaminhoBfs(g, 0, dist))
    {
        wprintf(L"Distâncias a partir do vértice 1 ( em número de arestas):\n");
        for (int i = 0; i < getNumeroVertices(g); i++)
        {
            wprintf(L"Vértice %d: %d\n", i + 1, dist[i]);
        }
    }
    else
    {
        wprintf(L"Erro ao calcular o menor caminho.\n");
    }
    free(dist);

    libera_Grafo(g);
    return 0;
}