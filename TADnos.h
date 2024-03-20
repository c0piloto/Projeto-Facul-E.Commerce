#ifndef TADNOS_H_INCLUDED
#define TADNOS_H_INCLUDED

#include "TADGeral.h"

typedef struct noCarrinho{
    Produto item;
    struct noCarrinho *prox;
}NoCarrinho;

typedef struct noProdutos{
    Produto item;
    struct noProdutos *prox;
}NoProdutos;

typedef struct noUser{
    Usuario user;
    struct noUser *prox;
}NoUser;

#endif // TADNOS_H_INCLUDED
