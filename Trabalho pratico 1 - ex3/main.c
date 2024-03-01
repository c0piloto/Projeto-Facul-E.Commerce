/*
    O seguinte programa consiste em criar e testar as funcionalidades de uma fila de carros em um posto de gasolina.

    Grupo:

    **Nome 1: ** **Matricula: **

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struct dos carros, tamanho da placa é baseado no tamanho atual brasileiro, tamanho de modelo é um valor arbitrário para
// que não gaste muita memoria atoa para a demonstracao.
typedef struct {
    char placa[8];
    char modelo[11];
    int anoDeFabricacao;
} Carro;

// Struct da fila utilizada, maxtam define o tamanho maximo e tam define o tamanho atual.
typedef struct {
    Carro* carros;
    int tam;
    int maxtam;
    int fim;
    int inicio;
} Fila;

// Verifica se a fila está vazia.
int filaVazia(Fila* f) {
    if (f == NULL) return -1;
    if (f->tam == 0) return 0;
    else return 1;
}

// Verifica se a fila esta cheia.
int filaCheia(Fila* f) {
    if (f->tam == f->maxtam) return 0;
    else return 1;
}

// Cria a fila nova com o tamanho passado pelo programa.
int criaFila(Fila* f, int maxtam) {
    f->maxtam = maxtam;
    f->carros = (Carro*)malloc(maxtam * sizeof(Carro));
    if (f->carros == NULL) return -1; // Verifica alocacão de memória
    f->tam = 0; // Tamanho da fila sem nenhum item.
    f->inicio = 0; // Inicio da fila no espaco 0 do vetor.
    f->fim = -1; // Indicador de fila vazia
    return 0;
}

// Funcao que adiociona um carro na fila.
int adicionarNaFila(Fila* f, Carro carro) {
    if (f == NULL) return -1;
    if (filaCheia(f) == 0) return -2;

    f->fim = (f->fim + 1) % f->maxtam;
    f->carros[f->fim] = carro;
    f->tam++;

    return 0;
}

// Essa funcao remove da fila, ela é a função que determina o atendimento de um cliente, retorna o carro atendido para uso em outras funcoes.
int removerDaFila(Fila* f, Carro* carroAtendido) {
    if (f == NULL) return -1;
    if (filaVazia(f) == 0) return -3;

    *carroAtendido = f->carros[f->inicio];
    f->inicio = (f->inicio + 1) % f->maxtam;
    f->tam--;

    return 0;
}

// Procura um carro na fila, essa funcao foi propositalmente feita quebrando as regras de fila e acessando todos os elementos para demonstracao dessa possibilidade.
int buscarPlaca(Fila* f, char* placa, Carro* carroEncontrado) {
    if (f == NULL) return -1;
    if (filaVazia(f) == 0) return -3;

    for (int i = 0; i < f->tam; i++)
        if (strcmp(f->carros[(f->inicio + i) % f->maxtam].placa, placa) == 0) {
            *carroEncontrado = f->carros[(f->inicio + i) % f->maxtam];
            return 5;
        }

    return 0;
}

// Essa funcao mostra a fila inteira, foi propositalmente feita MANTENDO as regras de fila, que eh a regra de que apenas o primeiro elemento esta disponivel
// dessa forma ela retira o item e o coloca no final da fila novamente, dessa forma ela percorre a fila inteira sem infringir as regras.
int mostrarFila (Fila *f){
    if (f == NULL) return -1;
    if (filaVazia(f) == 0) return -3;

    Carro carroParaAtender;
    for(int i = 0; i < f->tam; i++){
        removerDaFila(f, &carroParaAtender);
        printf("Placa: %s, Modelo: %s, Ano de Fabricacao: %d\n", carroParaAtender.placa, carroParaAtender.modelo, carroParaAtender.anoDeFabricacao);
        adicionarNaFila(f, carroParaAtender);
    }
    return 0;
}

// Funcao para aumentar o tamanho da fila caso necessário.
int aumentarFila(Fila* f, int aumentarEm) {
    if (f == NULL) return -1;

    Carro* aumentado = realloc(f->carros, (f->maxtam + aumentarEm) * sizeof(Carro));
    if (aumentado == NULL) return -4;
    f->carros = aumentado;
    f->maxtam += aumentarEm;

    return 0;
}

// Main, onde as funcoes acima são testadas.
int main() {
    Fila fila;
    if (criaFila(&fila, 5) != 0) {
        printf("Erro ao criar a fila.\n");
        return 1;
    }

    // Adicionando alguns carros na fila
    Carro carro1 = {"ABC1D23", "Gol", 2010};
    Carro carro2 = {"XYZ4E56", "Civic", 2015};
    Carro carro3 = {"MNO7F89", "Corolla", 2018};

    adicionarNaFila(&fila, carro1);
    adicionarNaFila(&fila, carro2);
    adicionarNaFila(&fila, carro3);

    // Exibindo os carros esperando atendimento
    printf("Carros esperando atendimento:\n");
    mostrarFila(&fila);

    // Verificando se um carro está presente na fila
    Carro carroEncontrado;
    char placaParaBuscar[] = "MNO7F89";
    if (buscarPlaca(&fila, placaParaBuscar, &carroEncontrado)) {
        printf("\nO carro com a placa %s esta na fila.\n", placaParaBuscar);
        printf("Modelo: %s, Ano de Fabricacao: %d\n", carroEncontrado.modelo, carroEncontrado.anoDeFabricacao);
    } else {
        printf("\nO carro com a placa %s nao esta na fila.\n", placaParaBuscar);
    }

    // Verificando um carro que NAO esta na fila.
    char placaParaBuscar2[] = "AAA1A11";
    if (buscarPlaca(&fila, placaParaBuscar2, &carroEncontrado)) {
        printf("\nO carro com a placa %s esta na fila.\n", placaParaBuscar2);
        printf("Modelo: %s, Ano de Fabricacao: %d\n", carroEncontrado.modelo, carroEncontrado.anoDeFabricacao);
    } else
        printf("\nO carro com a placa %s nao esta na fila.\n", placaParaBuscar2);

    Carro carroAtendido;
    if (removerDaFila(&fila, &carroAtendido) == 0)
        printf("\nO carro com placa %s foi atendido, ele eh do modelo %s.\n", carroAtendido.placa, carroAtendido.modelo);
    else
        printf("\nAlgum erro aconteceu!\n");


    free(fila.carros); // Liberando memória alocada para a fila

    return 0;
}
