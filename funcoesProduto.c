#include "TADGeral.h"
#include "TADnos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct listaProdutos{
    NoProdutos *inicio;
}ListaProdutos;

//Função usada para a criação de uma lista nova e vazia
ListaProdutos *criarProdutos(){
    ListaProdutos *l = (ListaProdutos*)malloc(sizeof(ListaProdutos));
    l->inicio = NULL;
    return l;
}

//Função para checar se a lista está vazia, caso esteja vazia retorne 0 caso não esteja vazia retorna 1
int listaVaziaProdutos(ListaProdutos *l){
    if(l->inicio == NULL)
        return 0;
    return 1;
}

//Função para limpar a lista por completo até ela ficar vazia
void limparProdutos(ListaProdutos *l){
    while(listaVaziaProdutos(l) != 0)
        removerProduto(l);
}

//Mostra todos os produtos
void mostrarProdutos(ListaProdutos *l){
    if (l == NULL) return -2;

    NoProdutos *no = l->inicio;
    int indice = 0;

    while(no != NULL){
        printf("Indice: %d\n", indice);
        printf("Nome: %s\n", no->item.nome);
        printf("Descricao: %s\n", no->item.descricao);
        printf("Valor Unitario: %.2f\n", no->item.valor_unitario);
        printf("Quantidade em Estoque: %d\n", no->item.quantidade_estoque);
        printf("Nota de Avaliacao: %.2f\n", no->item.nota_avaliacao);
        printf("Categoria: %s\n", no->item.categoria);
        printf("Vendas: %d\n", no->item.vendas);
        printf("\n");

        no = no->prox;
        indice++;
    }
}

//Exibe na tela todos os produtos com um certo nome
int mostrarProdutosNome(ListaProdutos *l, char *nome){
    if (l == NULL) return -2;

    NoProdutos *no = l->inicio;
    int indice = 0;
    int encontrado = 0;

    while (no != NULL) {
        if (strcmp(nome, no->item.nome) == 0) {
            printf("Indice: %d\n", indice);
            printf("Nome: %s\n", no->item.nome);
            printf("Descricao: %s\n", no->item.descricao);
            printf("Valor Unitario: %.2f\n", no->item.valor_unitario);
            printf("Quantidade em Estoque: %d\n", no->item.quantidade_estoque);
            printf("Nota de Avaliacao: %.2f\n", no->item.nota_avaliacao);
            printf("Categoria: %s\n", no->item.categoria);
            printf("\n");

            encontrado++;
        }
        no = no->prox;
        indice++;
    }

    if(encontrado == 0)
        return 1;
    else
        return 0;
}

//Exibe na tela todos os produtos com uma determinada categoria
int mostrarProdutosCategoria(ListaProdutos *l, char *categoria){
    if (l == NULL) return -2;

    NoProdutos *no = l->inicio;
    int indice = 0;
    int encontrado = 0;

    while(no != NULL){
        if(strcmp(categoria, no->item.categoria) == 0){
            printf("Indice: %d\n", indice);
            printf("Nome: %s\n", no->item.nome);
            printf("Descricao: %s\n", no->item.descricao);
            printf("Valor Unitario: %.2f\n", no->item.valor_unitario);
            printf("Quantidade em Estoque: %d\n", no->item.quantidade_estoque);
            printf("Nota de Avaliacao: %.2f\n", no->item.nota_avaliacao);
            printf("Categoria: %s\n", no->item.categoria);
            printf("\n");
            encontrado++;
            }
        no = no->prox;
        indice++;
        }
    if(encontrado == 0)
        return 1;
    else
        return 0;
}

//Verifica se a categoria digitada pelo usuario existe
int categoriaValida(char *categoria) {
    if (
        strcmp(categoria, "Vestuario") != 0 &&
        strcmp(categoria, "Utensilios") != 0 &&
        strcmp(categoria, "Eletronicos") != 0 &&
        strcmp(categoria, "Livros") != 0 &&
        strcmp(categoria, "Acessorios") != 0 &&
        strcmp(categoria, "Moveis") != 0 &&
        strcmp(categoria, "Esporte") != 0 &&
        strcmp(categoria, "Beleza") != 0 &&
        strcmp(categoria, "Papelaria") != 0 &&
        strcmp(categoria, "Decoracao") != 0 &&
        strcmp(categoria, "Higiene") != 0
    ) {
        return 1;
    } else {
        return 0;
    }
}

//Mostra 5 produtos relacionados as ultimas compras do usuario, caso ele nunca tenha feito compras, recomenda 5 produtos aleatorios
int mostrarRelacionados(ListaProdutos *l, NoUser *conta) {
    if (l == NULL) return -2;
    if (listaVaziaProdutos(l) == 0) return -1;

    int i = 0;
    int indice = 0;
    NoProdutos *no = l->inicio;

    if (categoriaValida(&conta->user.produtos_relacionados[i]) != 0) {
        while (i < 5) {
            printf("Indice: %d\n", indice);
            printf("Nome: %s\n", no->item.nome);
            printf("Descricao: %s\n", no->item.descricao);
            printf("Valor Unitario: %.2f\n", no->item.valor_unitario);
            printf("Quantidade em Estoque: %d\n", no->item.quantidade_estoque);
            printf("Nota de Avaliacao: %.2f\n", no->item.nota_avaliacao);
            printf("Categoria: %s\n", no->item.categoria);
            printf("\n");

            no = no->prox;
            indice++;
            i++;
        }
    } else {
        while (i < 5) {
            if (strcmp(&conta->user.produtos_relacionados[i], no->item.categoria) == 0) {
                printf("Indice: %d\n", indice);
                printf("Nome: %s\n", no->item.nome);
                printf("Descricao: %s\n", no->item.descricao);
                printf("Valor Unitario: %.2f\n", no->item.valor_unitario);
                printf("Quantidade em Estoque: %d\n", no->item.quantidade_estoque);
                printf("Nota de Avaliacao: %.2f\n", no->item.nota_avaliacao);
                printf("Categoria: %s\n", no->item.categoria);
                printf("\n");
                i++;
                no = l->inicio;
                indice = 0;
            }
            no = no->prox;
            indice++;
        }
    }
    return 0;
}

//Função que retorna o tamanho da lista de produtos
int tamanhoProdutos(ListaProdutos *l){
    if (l == NULL) return -2;
    if (listaVaziaProdutos(l) == 0) return -1;

    int p = 0;
    NoProdutos *no = l->inicio;

    while(no != NULL){
        p++;
        no = no->prox;
    }
    return p;
}

//Insere um nó novo no inicio da lista
int inserirProduto(ListaProdutos *l, Produto it){
    if(l == NULL) return -2;

    NoProdutos *no = (NoProdutos*) malloc (sizeof(NoProdutos));

    no->item = it;
    no->prox = l->inicio;
    l->inicio = no;
    return 0;
}

//Remove o primeiro item da lista de produtos
int removerProduto(ListaProdutos *l){
    if(l == NULL) return 2;
    if(listaVaziaProdutos(l) == 0) return 1;

    NoProdutos *no = l->inicio;
    l->inicio = no->prox;
    free(no);
    return 0;
}

//Remove o produto em determinada posicao
int removerProdutoPos(ListaProdutos *l, int pos){
    if (pos < 0 || pos >= tamanhoProdutos(l)) return -3;
    if (l == NULL) return -2;
    if (listaVaziaProdutos(l) == 0) return -1;

    NoProdutos *no = l->inicio;
    NoProdutos *aux = NULL;
    int p = 0;

    while(p < pos){
        aux = no;
        no = no->prox;
        p++;
    }
    if(aux == NULL)
        l->inicio->prox = no->prox;
    else
        aux->prox = no->prox;
    free(no);

    return 0;
}

//Insere um produto no banco de dados
int criarNovoProduto(Produto novoProduto) {

    FILE *bancoDeDadosProdutos = fopen("bancoDeDadosProdutos.txt", "a");
    if (bancoDeDadosProdutos != NULL) {

        fprintf(bancoDeDadosProdutos, "Nome: %s\n", novoProduto.nome);
        fprintf(bancoDeDadosProdutos, "Descrição: %s\n", novoProduto.descricao);
        fprintf(bancoDeDadosProdutos, "Categoria: %s\n", novoProduto.categoria);
        fprintf(bancoDeDadosProdutos, "Valor Unitário: %.2f\n", novoProduto.valor_unitario);
        fprintf(bancoDeDadosProdutos, "Nota de Avaliação: %.2f\n", novoProduto.nota_avaliacao);
        fprintf(bancoDeDadosProdutos, "Quantidade em Estoque: %d\n", novoProduto.quantidade_estoque);
        fprintf(bancoDeDadosProdutos, "Quantidade no Carrinho: %d\n", novoProduto.quantidade_carrinho);
        fprintf(bancoDeDadosProdutos, "Vendas: %d\n", novoProduto.vendas);
        fprintf(bancoDeDadosProdutos, "Vendedor: %s\n", novoProduto.vendedor);
        fprintf(bancoDeDadosProdutos, "\n");

        fclose(bancoDeDadosProdutos);
        return 0;
    } else {

        fclose(bancoDeDadosProdutos);
        printf("Falha ao abrir o banco de dados dos produtos.\n");
        return 1; // Retornar 1 para indicar erro na abertura do arquivo
    }
}

//Atualiza o banco de dados dos produtos com as compras realizadas
int salvarListaProdutos(ListaProdutos *l) {
    if (l == NULL) return -2;
    if (listaVaziaProdutos(l) == 0) return -1;

    remove("bancoDeDadosProdutos.txt");

    FILE *bancoDeDadosProdutos = fopen("bancoDeDadosProdutos.txt", "w");

    if (bancoDeDadosProdutos != NULL) {
        NoProdutos *no = l->inicio;

        while (no != NULL) {
            fprintf(bancoDeDadosProdutos, "Nome: %s\n", no->item.nome);
            fprintf(bancoDeDadosProdutos, "Descrição: %s\n", no->item.descricao);
            fprintf(bancoDeDadosProdutos, "Categoria: %s\n", no->item.categoria);
            fprintf(bancoDeDadosProdutos, "Valor Unitário: %.2f\n", no->item.valor_unitario);
            fprintf(bancoDeDadosProdutos, "Nota de Avaliação: %.2f\n", no->item.nota_avaliacao);
            fprintf(bancoDeDadosProdutos, "Quantidade em Estoque: %d\n", no->item.quantidade_estoque);
            fprintf(bancoDeDadosProdutos, "Quantidade no Carrinho: %d\n", no->item.quantidade_carrinho);
            fprintf(bancoDeDadosProdutos, "Vendas: %d\n", no->item.vendas);
            fprintf(bancoDeDadosProdutos, "Vendedor: %s\n", no->item.vendedor);
            fprintf(bancoDeDadosProdutos, "\n");

            no = no->prox;
        }

        fclose(bancoDeDadosProdutos);
    } else {
        return 1;
    }
    return 0;
}

//Verifica se um item específico está na lista.
int contemItem(ListaProdutos *l, char *item){
    if (l == NULL) return -2;
    if (listaVaziaProdutos(l) == 0) return -1;

    NoProdutos *no = l->inicio;

    while(no != NULL){
        if(strcmp(no->item.nome, item) == 0)
            return 0; //Produto encontrado na lista
        no = no->prox;
    }
    return 1; //Produto não encontrado na lista
}

//Retorna um produto em determinada posicao
int buscarPosicao(ListaProdutos *l, int posicao, Produto *retorno){
    if (l == NULL) return -2;
    if (listaVaziaProdutos(l) == 0) return -1;

    NoProdutos *no = l->inicio;
    int contador = 0;

    while(contador < posicao){
        no = no->prox;
        contador++;
    }

    *retorno = no->item;
    return 0;
}

//Atualiza a quantidade em estoque, a nota do produto e numero de vendas de um produto
int removerEstoque(ListaProdutos *l, char *it, float nota, int qtd){
    if (l == NULL) return -2;

    NoProdutos *no = l->inicio;

    while(strcmp(it, no->item.nome) != 0){
        no = no->prox;
    }

    no->item.quantidade_estoque -= qtd;

    no->item.nota_avaliacao = ((no->item.vendas * no->item.nota_avaliacao) + nota) / (no->item.vendas + 1);

    no->item.vendas += qtd;


    return 0;
}



