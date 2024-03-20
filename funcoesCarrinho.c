#include "TADGeral.h"
#include "TADnos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct listaCarrinho{
    NoCarrinho* inicio;
    Usuario usuario;
}ListaCarrinho;

//Função usada para a criação de uma lista nova e vazia
ListaCarrinho *criarCarrinho(){
    ListaCarrinho *l = (ListaCarrinho*) malloc (sizeof(ListaCarrinho));
    l->inicio = NULL;
    return l;
}

//Função para checar se a lista está vazia, caso esteja vazia retorne 0 caso não esteja vazia retorna 1
int listaVaziaCarrinho(ListaCarrinho *l){
    if(l->inicio == NULL)
        return 0;
    return 1;
}

//Função para limpar a lista por completo até ela ficar vazia
void limparCarrinho(ListaCarrinho *l){
    while (listaVaziaCarrinho(l) != 0)
        removerPosicaoCarrinho(l,0);
}

//Função para mostrar todos os produtos da lista
void mostrarCarrinho(ListaCarrinho *l){
    if (l == NULL) return;

    NoProdutos *no = l->inicio;

    int indice = 0;

    while(no != NULL){
        printf("Indice: %d\n", indice);
        printf("Nome: %s\n", no->item.nome);
        printf("Descricao: %s\n", no->item.descricao);
        printf("Valor Unitario: %.2f\n", no->item.valor_unitario);
        printf("Quantidade no carrinho: %d\n", no->item.quantidade_carrinho);
        printf("Nota de Avaliacao: %.2f\n", no->item.nota_avaliacao);
        printf("Categoria: %s\n", no->item.categoria);
        printf("\n");

        no = no->prox;
        indice++;
    }
}


//Função que retorna o tamanho da lista de produtos
int tamanhoCarrinho(ListaCarrinho *l){
    int contador = 0;
    NoCarrinho *no = l->inicio;

    while(no != NULL){
        contador++;
        no = no->prox;
    }
    return contador;
}

//Verifica se um item existe no carrinho e retorna sua posição
int contemItemCarrinho(ListaCarrinho *l, Produto it){
    if (l == NULL) return -2; //Checa se a lista existe
    if (listaVaziaCarrinho(l) == 0) return -1; //Checa se a lista é vazia

    NoCarrinho *no = l->inicio;
    int contador = 0;
    while (no != NULL){
        if(strcmp(no->item.nome,it.nome) == 0){
            return contador;
        } else {
            contador++;
            no = no->prox;
        }
    }
    return -1;
}

//Insere um nó novo no inicio da lista
int inserirCarrinho(ListaCarrinho *l,Produto it,int qnt){
    if (l == NULL) return 2; //Checagem se a lista existe

    if (qnt <= it.quantidade_estoque){
        if (contemItemCarrinho(l,it) >= 0){
            NoCarrinho *no = l->inicio;
            int contador = 0;
            while (contador < contemItemCarrinho(l,it)){
                no = no->prox;
                contador++;
            }
            no->item.quantidade_carrinho += qnt;
            return 0;
        } else {
            NoCarrinho *no = (NoCarrinho*) malloc (sizeof(NoCarrinho)); //Aloca memória o novo nó
            no->item = it;
            no->item.quantidade_carrinho = qnt;
            no->prox = l->inicio;
            l->inicio = no;
        }
    } else
        printf("Quantidade invalida.\n");
    return 0;
}

//Remove um item da lista de produtos baseado na sua posição
int removerPosicaoCarrinho(ListaCarrinho *l,int pos){
    if(pos < 0 || pos >= tamanhoCarrinho(l)) return 2; //Índice inválido
    if (l == NULL) return -2; //Checa se a lista existe
    if (listaVaziaCarrinho(l) == 0) return -1; //Checa se a lista é vazia


    NoCarrinho *no = l->inicio;
    NoCarrinho *aux = NULL;
    int p = 0;
    while(p < pos){
        aux = no;
        no = no->prox;
        p++;
    }

    if(aux == NULL)
        l->inicio = no->prox;
    else
        aux->prox = no->prox;

    free(no);

    return 0;
}

//Calcula o valor total do carrinho ao fazer checkout
float totalCarrinho(ListaCarrinho *l){
    if (l == NULL) return -2;

    NoCarrinho *no = l->inicio;
    float total = 0;

    while(no != NULL){
        total += (no->item.valor_unitario * no->item.quantidade_carrinho);
        no = no->prox;
    }

    return total;
}

//Retorna o nome do produto do inicio da lista do carrinho
char *retornaNomeCheckout(ListaCarrinho *l){
    if (l == NULL) return "Ponteiro para lista na existe";
    if (listaVaziaCarrinho(l) == 0) return "Carrinho vazio";

    NoCarrinho *no = l->inicio;

    return no->item.nome;
}

//Retonrna a quantidade do produto do inicio da lista do carrinho
int retornaQuantidadeCheckout(ListaCarrinho *l){
    if (l == NULL) return -2;
    if (listaVaziaCarrinho(l) == 0) return -1;

    NoCarrinho *no = l->inicio;

    return no->item.quantidade_carrinho;
}

//Remove item do carrinho
int removerItemCarrinho(ListaCarrinho *l, int indice, int qtd){
    if (l == NULL) return -2;
    if (listaVaziaCarrinho(l) == 0) return -1;

    NoCarrinho *no = l->inicio;
    int contador = 0;

    while(contador < indice){
        no = no->prox;
    }

    if ((no->item.quantidade_carrinho - qtd) > 0)
        no->item.quantidade_carrinho = no->item.quantidade_carrinho - qtd;
    else
        removerPosicaoCarrinho(l, indice);

    return 0;
}

//Atualiza as categorias que serao recomendadas para o usuario baseado em suas ultimas compras
int recomendaProdutos(ListaCarrinho *l,NoUser *conta){
    if (l == NULL) return -2;
    if (conta == NULL) return -3;
    if (listaVaziaCarrinho(l) == 0) return -1;

    int i = 0;
    NoCarrinho *no = l->inicio;

    while(i < 5){
        strcpy(&conta->user.produtos_relacionados[i],no->item.categoria);
        i++;
        if(no->prox != NULL){
            no = no->prox;
        }
    }

    return 0;
}

