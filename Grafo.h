#ifndef GRAFO_H
#define GRAFO_H

typedef struct grafo Grafo;
Grafo *cria_Grafo(int limite_de_vertices, int eh_ponderado);
void libera_Grafo(Grafo *G);

int insereVertice(Grafo *G, char *label);

// u = origem, v = destino e w = peso se for ponderado
int insereAresta(Grafo *G, int u, int v, float w);
int removeAresta(Grafo *G, int u, int v);
int grauMedio(Grafo *G);
int grauMax(Grafo *G, int *v);
int grau(Grafo *G, int v);

int getLimiteVertices(Grafo *G);
int getNumeroVertices(Grafo *G);
int contaArestas(Grafo *G);
int verificaConexaBfs(Grafo *G, int start, int *visitado);
int validaConexas(Grafo *G, int *tamMax);
int validaMenorCaminhoBfs(Grafo *G, int start, int *dist);
#endif