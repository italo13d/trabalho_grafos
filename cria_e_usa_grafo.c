#include <stdio.h>
#include "Grafo.h"

int main()
{
    Grafo *g = cria_Grafo(16, 1);

    if(g == NULL) {
        printf("Erro na criação do grafo.\n");
        return 1;
    }

    printf("Grafo criado com sucesso!\n");
    printf("Limite de vertices: %d\n", getLimiteVertices(g));
    printf("Numero atual de vertices: %d\n", getNumeroVertices(g));
    


    libera_Grafo(g);
    return 0;
}