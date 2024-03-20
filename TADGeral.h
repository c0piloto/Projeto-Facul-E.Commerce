#ifndef TADGERAL_H_INCLUDED
#define TADGERAL_H_INCLUDED

typedef struct produto
{
    char nome[20];
    char descricao[100];
    char categoria[15];
    float valor_unitario;
    float nota_avaliacao;
    int quantidade_estoque;
    int quantidade_carrinho;
    int vendas;
    char vendedor[20];
}Produto;

typedef struct usuario
{
    char nome[50];
    char usuario[20];
    char senha[20];
    char produtos_relacionados[5][20];
}Usuario;

typedef struct pedido
{
    Produto item;
    char login[20];
} Pedido;

typedef struct listaProdutos ListaProdutos;
typedef struct listaCarrinho ListaCarrinho;
typedef struct listaUsuario ListaUsuario;
typedef struct listaAnuncio ListaAnuncio;
typedef struct noProdutos NoProdutos;
typedef struct noCarrinho NoCarrinho;
typedef struct noUser NoUser;
typedef struct noAnuncio NoAnuncio;

ListaProdutos *criarProdutos();
int listaVaziaProdutos(ListaProdutos *l);
void limparProdutos(ListaProdutos *l);
void mostrarProdutos(ListaProdutos *l);
int criarNovoProduto(Produto it);
int mostrarProdutosNome(ListaProdutos *l, char *nome);
int mostrarProdutosCategoria(ListaProdutos *l, char *categoria);
int tamanhoProdutos(ListaProdutos *l);
int inserirProduto(ListaProdutos *l, Produto it);
int removerProduto(ListaProdutos *l);
int removerProdutoPos(ListaProdutos *l,int pos);
int buscarCategoria(ListaProdutos *l, char* categoriaP, Produto *retorno);
int buscarProduto(ListaProdutos *l, char* nomeP, Produto *retorno);
int buscarPosicao(ListaProdutos *l, int posicao, Produto *retorno);
int contemItem(ListaProdutos *l, char *it);
int removerEstoque(ListaProdutos *l, char *it, float nota, int qtd);
int salvarListaProdutos(ListaProdutos *l);
int categoriaValida(char *categoria);

ListaUsuario *criarUsuarios();
int criarNovaConta(Usuario novoUsuario);
int listaVaziaUsuarios(ListaUsuario *l);
void limparUsuarios(ListaUsuario *l);
void mostrarUsuarios(ListaUsuario *l);
int tamanhoUsuarios(ListaUsuario *l);
int contemUsuario(ListaUsuario *l, Usuario *it);
int inserirContaNova(ListaUsuario *l, Usuario it);
int inserirAnuncio(NoUser *user, Produto it);
int removerAnuncio(NoUser *user, int pos);
int removerUsuario(ListaUsuario *l);
int buscarConta(ListaUsuario *user, char *usuario, char *senha,NoUser **conta);
int salvarListaUsuarios(ListaUsuario *l);

ListaCarrinho *criarCarrinho();
int listaVaziaCarrinho(ListaCarrinho *l);
void limparCarrinho(ListaCarrinho *l);
void mostrarCarrinho(ListaCarrinho *l);
int tamanhoCarrinho(ListaCarrinho *l);
int contemItemCarrinho(ListaCarrinho *l, Produto it);
int inserirCarrinho(ListaCarrinho *l, Produto it,int qnt);
int removerPosicaoCarrinho(ListaCarrinho *l,int pos);
float totalCarrinho(ListaCarrinho *l);
char *retornaNomeCheckout(ListaCarrinho *l);
int retornaQuantidadeCheckout(ListaCarrinho *l);
int recomendaProdutos(ListaCarrinho *l,NoUser *conta);

#endif // TADGERAL_H_INCLUDED
