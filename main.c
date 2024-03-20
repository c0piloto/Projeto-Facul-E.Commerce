/* O programa a seguir é uma simulação de um sistema de e-commerce, que permite ao usuario criar uma
conta, fazer login, comprar os produtos em catálogo, e avaliar suas compras.

by: Moises Marques de Oliveira, Joao Pedro Teixeira, Gustavo Biazi e Luis Henrique Gonçalves
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Uma representa o Windows, io.h, e a outra sistemas Unix, unistd.h. Elas são outras bibliotecas com
funçãos relacionadas a esses sistemas operacionais, usadas na função bancosDeDadosBaseExiste,
sendo responsaveis pelas funções de acesso a arquivo, que são diferentes pra cada sistema operacional.
*/
#include <io.h>
#include <unistd.h>


#include "TADGeral.h"
#include "TADnos.h"

/* Define uma constante PROGRAM que verifica a versão do sistema operacional sendo usada, isso é relevante
na função limparTela() pois pois cada sistema operacional tem uma sintaxe diferente pra limpeza.
(Ex: Linux usa system("clear"); e Windows system("cls");)
*/
#if defined(_WIN32) || defined(_WIN64)
#define PLATFORM "Windows"
#elif defined(__linux__)
#define PLATFORM "Linux"
#elif defined(__APPLE__)
#define PLATFORM "macOS"
#else
#define PLATFORM "Sistema operacional não suportado"
#endif

//Protipos
int lerProdutos(ListaProdutos *lp);
int lerUsuarios(ListaUsuario *lu);
void limparTela();
int bancosDeDadosBaseExiste(char *nome);
int criaBancosDeDadosBase(char *nome);
//--------------------------------------------------//

int main(){
    NoUser *conta;
    int op;
    ListaUsuario *lista_usuarios = criarUsuarios();
    ListaCarrinho *lista_carrinho = criarCarrinho();
    ListaProdutos *lista_produtos = criarProdutos();

    //Verifica se o banco de dados existe, e caso não existir cria um
    if(bancosDeDadosBaseExiste("bancoDeDadosProdutos.txt") != 0)
        criaBancosDeDadosBase("bancoDeDadosProdutos.txt");

    if(bancosDeDadosBaseExiste("bancoDeDadosUsuarios.txt") != 0)
        criaBancosDeDadosBase("bancoDeDadosUsuarios.txt");

    //Lê os bancos de dados e os insere em listas
    lerProdutos(lista_produtos);
    lerUsuarios(lista_usuarios);

    while(op != 1){
        printf("\n1 - Fazer login\n2 - Criar uma conta nova.\n\n");
        fflush(stdin);
        scanf("%d", &op);
        switch (op){
            //Faz login
            case 1: {
                limparTela();

                printf ("Conta para testes:\nusuario: admin\nsenha: admin\n\n");

                char user[20];
                char senha[20];
                printf("\nDigite o seu usuario\n\n");
                fflush(stdin);
                fgets(user, 20, stdin);
                printf("\nDigite a sua senha\n\n");
                fflush(stdin);
                fgets(senha, 20, stdin);
                fflush(stdin);
                int tamUser = strlen(user);
                int tamSenha = strlen(senha);
                if (user[tamUser - 1] == '\n') { //Substitui o "Enter" digitado pelo usuario da string por '\0', para poder compara-la e salva-la no banco de dados
                    user[tamUser - 1] = '\0';
                }
                if (senha[tamSenha - 1] == '\n') { //Substitui o "Enter" digitado pelo usuario da string por '\0', para poder compara-la e salva-la no banco de dados
                    senha[tamSenha - 1] = '\0';
                }

                int retorno = buscarConta(lista_usuarios, user, senha, &conta); //Verifica se a conta existe, se a senha está correta e retorna um ponteiro para o nó da conta logada

                if (retorno == -7){
                    op = 0;

                    limparTela();

                    printf("\nSenha incorreta para o usuario '%s'.\n\n", user);
                }
                else if (retorno == -8){
                    op = 0;

                    limparTela();

                    printf("\nUsuario nao existe, crie uma nova conta ou verifique se digitou corretamente.\n\n");
                }

                break;
            }
            //Cadastra um novo usuário
            case 2: {

                limparTela();

                Usuario novo;

                printf("\nDigite o seu nome\n\n");
                fflush(stdin);
                fgets(novo.nome, 50, stdin);
                int tamNome = strlen(novo.nome);
                if (novo.nome[tamNome - 1] == '\n') { //Substitui o "Enter" digitado pelo usuario da string por '\0', para poder compara-la e salva-la no banco de dados
                    novo.nome[tamNome - 1] = '\0';
                }
                printf("\nDigite o seu usuario\n\n");
                fflush(stdin);
                fgets(novo.usuario, 20, stdin);
                int tamUser = strlen(novo.usuario);
                if (novo.usuario[tamUser - 1] == '\n') { //Substitui o "Enter" digitado pelo usuario da string por '\0', para poder compara-la e salva-la no banco de dados
                    novo.usuario[tamUser - 1] = '\0';
                }
                printf("\nDigite a sua senha\n\n");
                fgets(novo.senha, 20, stdin);
                int tamSenha = strlen(novo.senha);
                if (novo.senha[tamSenha - 1] == '\n') { //Substitui o "Enter" digitado pelo usuario da string por '\0', para poder compara-la e salva-la no banco de dados
                    novo.senha[tamSenha - 1] = '\0';
                }
                inserirContaNova(lista_usuarios, novo); //Insere a conta na lista
                criarNovaConta(novo); //Insere a conta no banco de dados
                break;
            }
            default:
                limparTela();

                printf("\nDigite uma opcao valida!\n\n");
                break;
        }
    }

    limparTela();
    printf("\nProdutos recomendados baseados em suas ultimas compras:\n\n");  //Recomenda 5 produtos relacionados ao usuario, e verifica se o ele quer colocar algum no carrinho
    mostrarRelacionados(lista_produtos, conta);
    printf("\nDeseja colocar algum dos itens no carrinho? s(sim)/n(nao)\n\n");
    char confirma;
    fflush(stdin);
    scanf("%c", &confirma);
    if (confirma == 's' || confirma == 'S'){
        printf("\nQual? (Digite o indice)\n\n");
        int indice;
        scanf("%d", &indice);
        printf("\nQuantos?\n\n");
        int quantidade;
        scanf("%d", &quantidade);
        Produto produtoret;
        buscarPosicao(lista_produtos, indice, &produtoret);
        inserirCarrinho(lista_carrinho, produtoret, quantidade);
        limparTela();
    }
    else if (confirma == 'n' || confirma == 'N'){
        limparTela();
    }
    op = 0;

    while(op != 6){
        printf("\nMenu Principal\n");
        printf("\n1 - Mostrar todos os produtos\n2 - Procurar produto pelo nome\n3 - Procurar produto pela categoria\n4 - Ver produtos relacionados\n5 - Mostrar carrinho\n6 - Sair\n\n");
        fflush(stdin);
        scanf("%d", &op);
        switch (op){
            case 1:{ //Exibe todos os produtos na tela e verifica se o usuario quer colocar algum no carrinho

                limparTela();
                mostrarProdutos(lista_produtos);
                printf("\nDeseja colocar algum dos itens no carrinho? s(sim)/n(nao)\n\n");
                fflush(stdin);
                scanf("%c", &confirma);
                if (confirma == 's' || confirma == 'S'){
                    printf("\nQual? (Digite o indice)\n\n");
                    int indice;
                    scanf("%d", &indice);
                    printf("\nQuantos?\n\n");
                    int quantidade;
                    scanf("%d", &quantidade);
                    Produto produtoret;
                    buscarPosicao(lista_produtos, indice, &produtoret);
                    inserirCarrinho(lista_carrinho, produtoret, quantidade);

                    limparTela();
                }
                else if (confirma == 'n' || confirma == 'N'){
                    limparTela();
                    printf("\nVoltando ao menu principal\n\n");
                    op = 0;
                }
                else{
                    limparTela();
                    printf("\nOpcao invalida! Voltando ao menu principal\n\n");
                    op = 0;
                }
                break;
            }

            case 2:{ //Permite que o usuario faça uma busca por nome da lista de produtos

                limparTela();
                char nomeproduto[20];

                printf("\nDigite o nome do produto:\n\n");
                fflush(stdin);
                fgets(nomeproduto, 20, stdin);
                int tamNomeP = strlen(nomeproduto);
                if (nomeproduto[tamNomeP - 1] == '\n') {
                    nomeproduto[tamNomeP - 1] = '\0';
                }

                limparTela();

                if(mostrarProdutosNome(lista_produtos, nomeproduto) == 1){
                    printf("\nNao encontramos nenhum produto com esse nome, tente novamente\n\n");
                    break;
                }
                printf("\nDeseja colocar algum dos itens no carrinho? s(sim)/n(nao)\n\n");
                fflush(stdin);
                scanf("%c", &confirma);
                if (confirma == 's' || confirma == 'S'){
                    printf("\nQual? (Digite o indice)\n\n");
                    int indice;
                    scanf("%d", &indice);
                    printf("\nQuantos?\n\n");
                    int quantidade;
                    scanf("%d", &quantidade);
                    Produto produtoret;
                    buscarPosicao(lista_produtos, indice, &produtoret);
                    inserirCarrinho(lista_carrinho, produtoret, quantidade);
                }
                else if (confirma == 'n' || confirma == 'N'){
                    limparTela();
                    printf("\nVoltando ao menu principal\n\n");
                    op = 0;
                }
                else{
                    limparTela();
                    printf("\nOpcao invalida! Voltando ao menu principal\n\n");
                    op = 0;
                }
                break;
            }

            case 3:{ //Permite que o usuario faça uma busca por categoria da lista de produtos

                limparTela();
                char categoriaproduto[15];
                printf("\nCategorias existentes:");
                printf("\nVestuario\nUtensilios\nEletronicos\nLivros\nAcessorios\nMoveis\nEsporte\nBeleza\nPapelaria\nDecoracao\nHigiene\n");
                printf("\nDigite a categoria do produto:\n\n");
                fflush(stdin);
                fgets(categoriaproduto, 15, stdin);
                int tamCategoriaP = strlen(categoriaproduto);
                if (categoriaproduto[tamCategoriaP - 1] == '\n') {
                    categoriaproduto[tamCategoriaP - 1] = '\0';
                }
                if(mostrarProdutosCategoria(lista_produtos, categoriaproduto) == 1){
                    limparTela();
                    printf("\nNao encontramos nenhum produto com essa categoria, tente novamente\n\n");
                    break;
                }
                printf("\nDeseja colocar algum dos itens no carrinho? s(sim)/n(nao)\n\n");
                fflush(stdin);
                scanf("%c", &confirma);
                if (confirma == 's' || confirma == 'S'){
                    printf("\nQual? (Digite o indice)\n\n");
                    int indice;
                    scanf("%d", &indice);
                    printf("\nQuantos?\n\n");
                    int qtd;
                    scanf("%d", &qtd);
                    Produto produtoret;
                    buscarPosicao(lista_produtos, indice, &produtoret);
                    inserirCarrinho(lista_carrinho, produtoret, qtd);
                }
                else if (confirma == 'n' || confirma == 'N'){
                    limparTela();
                    printf("\nVoltando ao menu principal\n\n");
                    op = 0;
                }
                else{
                    limparTela();
                    printf("\nOpcao invalida! Voltando ao menu principal\n\n");
                    op = 0;
                }
                limparTela();

                printf("\nProduto adicionado ao carrinho com sucesso, retornando ao menu!\n\n");

                break;
            }
            case 4:{ //Recomenda novamente 5 produtos relacionados ao usuario
                limparTela();

                printf("\nProdutos recomendados baseados nas suas ultimas compras:\n\n");
                mostrarRelacionados(lista_produtos, conta);
                printf("\nDeseja colocar algum dos itens no carrinho? s(sim)/n(nao)\n\n");
                char confirma;
                fflush(stdin);
                scanf("%c", &confirma);
                if (confirma == 's' || confirma == 'S'){
                    printf("\nQual? (Digite o indice)\n\n");
                    int indice;
                    scanf("%d", &indice);
                    printf("\nQuantos?\n\n");
                    int quantidade;
                    scanf("%d", &quantidade);
                    Produto produtoret;
                    buscarPosicao(lista_produtos, indice, &produtoret);
                    inserirCarrinho(lista_carrinho, produtoret, quantidade);
                    limparTela();
                }
                else if (confirma == 'n' || confirma == 'N'){
                    limparTela();
                }
                op = 0;
                break;
            }
            case 5:{ //Acessa o carrinho, permitindo deletar um produto ou fazer checkout
                limparTela();

                mostrarCarrinho(lista_carrinho);
                printf("\nD - Deletar produto do carrinho\nC - Checkout\nS - Sair do carrinho\n\n");
                char opcaocarrinho;
                fflush(stdin);
                scanf("%c", &opcaocarrinho);
                if (opcaocarrinho == 'd' || opcaocarrinho == 'D'){
                    printf("\nQual? (Digite o indice\n\n)");
                    int indice;
                    scanf("%d", &indice);
                    printf("\nQuantos?\n\n");
                    int qtd;
                    scanf("%d", &qtd);
                    removerItemCarrinho(lista_carrinho,indice, qtd);

                    limparTela();

                    break;
                }
                if (opcaocarrinho == 'c' || opcaocarrinho == 'C'){
                    printf("\nR$ %.2f", totalCarrinho(lista_carrinho));
                    printf("\nDeseja finalizar a compra? s(sim)/n(nao)\n\n");
                    fflush(stdin);
                    char opcaocheckout;
                    scanf("%c", &opcaocheckout);
                    if(opcaocheckout == 's' || opcaocheckout == 'S'){
                        printf("Muito obrigado pela compra.\n\n");
                        recomendaProdutos(lista_carrinho, conta);
                        float nota;
                        while(listaVaziaCarrinho(lista_carrinho) != 0){
                            char *produtoRemover = retornaNomeCheckout(lista_carrinho);
                            int qtdCarrinho = retornaQuantidadeCheckout(lista_carrinho);
                            printf("\nAvalie o produto %s comprado de 0.0 a 5.0:\n\n", produtoRemover);
                            fflush(stdin);
                            scanf("%f",&nota);
                            removerEstoque(lista_produtos, produtoRemover, nota, qtdCarrinho);
                            removerPosicaoCarrinho(lista_carrinho,0);
                        }

                        limparTela();
                        break;
                    }
                    else if (opcaocheckout == 'n' || opcaocheckout == 'N'){
                        limparTela();
                        printf("\nVoltando ao menu principal\n\n");
                        op = 0;
                        break;
                    }
                    else{
                        limparTela();
                        printf("\nOpcao invalida! Voltando ao menu principal\n\n");
                        op = 0;
                        break;
                    }
                }
                if (opcaocarrinho == 's' || opcaocarrinho == 'S'){
                    limparTela();
                    printf("\nVoltando ao menu principal\n\n");
                    op = 0;
                    break;
                }
                printf("\nOpcao invalida! Voltando ao menu principal\n\n");
                op = 0;

            }
            case 6:{ // Sai do programa
                printf("\nSaindo...\n");
                break;
            }
            default:
                limparTela();
                printf("\nDigite uma opcao valida!\n\n");
                break;
        }
    }
    salvarListaProdutos(lista_produtos); //Atualiza o banco de dados
    return 0;
}

//Lê o banco de dados e passa todos os usuarios para uma lista
int lerUsuarios(ListaUsuario *lu) {
    FILE *bancoDeDadosUsuarios = fopen("bancoDeDadosUsuarios.txt", "r");
    if (bancoDeDadosUsuarios == NULL)
        return -5;

    Usuario user;

    while (fscanf(bancoDeDadosUsuarios, "Nome: %s\n", user.nome) == 1) {
        fscanf(bancoDeDadosUsuarios, "Usuário: %s\n", user.usuario);
        fscanf(bancoDeDadosUsuarios, "Senha: %s\n", user.senha);
        fscanf(bancoDeDadosUsuarios, "Produtos relacionados:\n");

        for (int i = 0; i < 5; i++) {
            fscanf(bancoDeDadosUsuarios, "   - %s\n", user.produtos_relacionados[i]);
        }
        fscanf(bancoDeDadosUsuarios, "\n");

        inserirContaNova(lu, user);
    }

    fclose(bancoDeDadosUsuarios);
    return 0;
}

//Lê o banco de dados e passa todos os produtos para uma lista
int lerProdutos(ListaProdutos *lp) {
    FILE *bancoDeDadosProdutos = fopen("bancoDeDadosProdutos.txt", "r");
    if (bancoDeDadosProdutos == NULL)
        return -6;

    Produto item;
    char buffer[100]; // Buffer para ler as linhas do arquivo

    while (fgets(buffer, sizeof(buffer), bancoDeDadosProdutos) != NULL) {
        if (sscanf(buffer, "Nome: %s\n", item.nome) == 1 &&
            fgets(buffer, sizeof(buffer), bancoDeDadosProdutos) != NULL &&
            sscanf(buffer, "Descrição: %s\n", item.descricao) == 1 &&
            fgets(buffer, sizeof(buffer), bancoDeDadosProdutos) != NULL &&
            sscanf(buffer, "Categoria: %s\n", item.categoria) == 1 &&
            fgets(buffer, sizeof(buffer), bancoDeDadosProdutos) != NULL &&
            sscanf(buffer, "Valor Unitário: %f\n", &item.valor_unitario) == 1 &&
            fgets(buffer, sizeof(buffer), bancoDeDadosProdutos) != NULL &&
            sscanf(buffer, "Nota de Avaliação: %f\n", &item.nota_avaliacao) == 1 &&
            fgets(buffer, sizeof(buffer), bancoDeDadosProdutos) != NULL &&
            sscanf(buffer, "Quantidade em Estoque: %d\n", &item.quantidade_estoque) == 1 &&
            fgets(buffer, sizeof(buffer), bancoDeDadosProdutos) != NULL &&
            sscanf(buffer, "Quantidade no Carrinho: %d\n", &item.quantidade_carrinho) == 1 &&
            fgets(buffer, sizeof(buffer), bancoDeDadosProdutos) != NULL &&
            sscanf(buffer, "Vendas: %d\n", &item.vendas) == 1 &&
            fgets(buffer, sizeof(buffer), bancoDeDadosProdutos) != NULL &&
            sscanf(buffer, "Vendedor: %s\n", item.vendedor) == 1) {
            // Leitura bem-sucedida, insira o produto na lista
            inserirProduto(lp, item);
        }
    }

    fclose(bancoDeDadosProdutos);
    return 0;
}

//Limpa o console
void limparTela(){
    if (strcmp(PLATFORM, "Windows") == 0)
        system("cls");
    else if (strcmp(PLATFORM, "Linux") == 0 || strcmp(PLATFORM, "macOS") == 0)
        system("clear");
}

//Verifica se um arquivo com nome igual ao banco de dados existe
int bancosDeDadosBaseExiste(char *nome){
    if (strcmp(PLATFORM, "Windows") == 0){
        char *nomeDoArquivo = nome;

        if (_access(nomeDoArquivo, 0) == 0)
            return 0;
        else
            return 1;

    }
    else if (strcmp(PLATFORM, "Linux") == 0 || strcmp(PLATFORM, "macOS") == 0){
        char *nomeDoArquivo = nome;

        if (access(nomeDoArquivo, F_OK) == 0)
            return 0;
        else
            return 1;
    }
}

//Função que cria um banco de dados novo
int criaBancosDeDadosBase(char *nome){
    if(strcmp(nome, "bancoDeDadosProdutos.txt") == 0){
        srand(time(NULL));

        Produto produto1 = {"Camiseta", "Camiseta de algodao", "Vestuario", 25.0, 4.7, 50, 0, rand() % 100 + 1, 'A'};
        Produto produto2 = {"Caneca", "Caneca de porcelana", "Utensilios", 10.5, 4.2, 100, 0, rand() % 100 + 1, 'B'};
        Produto produto3 = {"Celular", "Smartphone Android", "Eletronicos", 399.99, 4.8, 30, 0, rand() % 100 + 1, 'C'};
        Produto produto4 = {"Sapato", "Sapato social masculino", "Vestuario", 55.0, 4.6, 40, 0, rand() % 100 + 1, 'D'};
        Produto produto5 = {"Livro", "Romance contemporaneo", "Livros", 15.5, 4.9, 70, 0, rand() % 100 + 1, 'E'};
        Produto produto6 = {"Fone de ouvido", "Fone Bluetooth", "Eletronicos", 79.99, 4.5, 60, 0, rand() % 100 + 1, 'A'};
        Produto produto7 = {"Mochila", "Mochila Escolar", "Acessorios", 30.0, 4.2, 90, 0, rand() % 100 + 1, 'B'};
        Produto produto8 = {"Cadeira", "Cadeira de Escritorio", "Moveis", 120.0, 4.6, 20, 0, rand() % 100 + 1, 'C'};
        Produto produto9 = {"Mesa", "Mesa de Jantar", "Moveis", 199.5, 4.8, 35, 0, rand() % 100 + 1, 'D'};
        Produto produto10 = {"Shorts", "Shorts Esportivo", "Vestuario", 18.0, 4.0, 55, 0, rand() % 100 + 1, 'E'};
        Produto produto11 = {"Perfume", "Perfume Floral", "Beleza", 50.0, 4.7, 40, 0, rand() % 100 + 1, 'A'};
        Produto produto12 = {"Relogio", "Relogio de Pulso", "Acessorios", 99.0, 4.3, 75, 0, rand() % 100 + 1, 'B'};
        Produto produto13 = {"Colar", "Colar de Prata", "Acessorios", 35.5, 4.5, 65, 0, rand() % 100 + 1, 'C'};
        Produto produto14 = {"Tenis", "Tenis Esportivo", "Vestuario", 70.0, 4.8, 30, 0, rand() % 100 + 1, 'D'};
        Produto produto15 = {"Bolsa", "Bolsa Feminina", "Acessorios", 40.0, 4.0, 85, 0, rand() % 100 + 1, 'E'};
        Produto produto16 = {"Oculos", "Oculos de Sol", "Acessorios", 60.0, 4.6, 50, 0, rand() % 100 + 1, 'A'};
        Produto produto17 = {"Monitor", "Monitor LED", "Eletronicos", 199.0, 4.7, 25, 0, rand() % 100 + 1, 'B'};
        Produto produto18 = {"Notebook", "Notebook Intel i5", "Eletronicos", 799.99, 4.9, 15, 0, rand() % 100 + 1, 'C'};
        Produto produto19 = {"Camera", "Camera Digital", "Eletronicos", 299.0, 4.4, 20, 0, rand() % 100 + 1, 'D'};
        Produto produto20 = {"Pulseira", "Pulseira de Ouro", "Acessorios", 45.0, 4.8, 60, 0, rand() % 100 + 1, 'E'};
        Produto produto21 = {"Chapeu", "Chapeu de Palha", "Acessorios", 20.0, 4.2, 75, 0, rand() % 100 + 1, 'A'};
        Produto produto22 = {"Jaqueta", "Jaqueta de Couro", "Vestuario", 120.0, 4.7, 30, 0, rand() % 100 + 1, 'B'};
        Produto produto23 = {"Bicicleta", "Bicicleta MTB", "Esporte", 299.0, 4.8, 10, 0, rand() % 100 + 1, 'C'};
        Produto produto24 = {"Laptop", "Laptop Intel i7", "Eletronicos", 1199.0, 4.9, 5, 0, rand() % 100 + 1, 'D'};
        Produto produto25 = {"Cadeado", "Cadeado de Segurança", "Utilidades", 15.0, 4.5, 70, 0, rand() % 100 + 1, 'E'};
        Produto produto26 = {"Guarda-chuva", "Guarda-chuva Transparente", "Acessorios", 25.0, 4.3, 85, 0, rand() % 100 + 1, 'A'};
        Produto produto27 = {"Lapis", "Lapis HB", "Papelaria", 0.5, 4.0, 200, 0, rand() % 100 + 1, 'B'};
        Produto produto28 = {"Pincel", "Pincel de Pintura", "Papelaria", 2.0, 4.6, 120, 0, rand() % 100 + 1, 'C'};
        Produto produto29 = {"Caneta", "Caneta Esferografica", "Papelaria", 1.0, 4.8, 150, 0, rand() % 100 + 1, 'D'};
        Produto produto30 = {"Livro de Comedia", "Comedia contemporanea", "Livros", 10.5, 4.2, 40, 0, rand() % 100 + 1, 'B'};
        Produto produto31 = {"Agenda", "Agenda de Couro", "Papelaria", 8.0, 4.7, 80, 0, rand() % 100 + 1, 'A'};
        Produto produto32 = {"Regua", "Regua de Plastico", "Papelaria", 1.5, 4.2, 110, 0, rand() % 100 + 1, 'B'};
        Produto produto33 = {"Borracha", "Borracha Escolar", "Papelaria", 0.75, 4.4, 130, 0, rand() % 100 + 1, 'C'};
        Produto produto34 = {"Apontador", "Apontador de Lapis", "Papelaria", 1.0, 4.0, 90, 0, rand() % 100 + 1, 'D'};
        Produto produto35 = {"Mouse", "Mouse Sem Fio", "Eletronicos", 20.0, 4.6, 65, 0, rand() % 100 + 1, 'E'};
        Produto produto36 = {"Teclado", "Teclado Mecanico", "Eletronicos", 40.0, 4.8, 55, 0, rand() % 100 + 1, 'A'};
        Produto produto37 = {"Monitor", "Monitor Curvo", "Eletronicos", 250.0, 4.7, 45, 0, rand() % 100 + 1, 'B'};
        Produto produto38 = {"Vaso", "Vaso de Flores", "Decoração", 15.0, 4.2, 75, 0, rand() % 100 + 1, 'C'};
        Produto produto39 = {"Espelho", "Espelho de Parede", "Decoração", 30.0, 4.6, 60, 0, rand() % 100 + 1, 'D'};
        Produto produto40 = {"Pente", "Pente de Madeira", "Beleza", 5.0, 4.8, 100, 0, rand() % 100 + 1, 'E'};
        Produto produto41 = {"Creme", "Creme Hidratante", "Beleza", 15.0, 4.7, 85, 0, rand() % 100 + 1, 'A'};
        Produto produto42 = {"Sabonete", "Sabonete Liquido", "Higiene", 2.5, 4.5, 120, 0, rand() % 100 + 1, 'B'};
        Produto produto43 = {"Shampoo", "Shampoo Revitalizante", "Higiene", 10.0, 4.3, 90, 0, rand() % 100 + 1, 'C'};
        Produto produto44 = {"Condicionador", "Condicionador Nutritivo", "Higiene", 12.0, 4.0, 80, 0, rand() % 100 + 1, 'D'};
        Produto produto45 = {"Toalha", "Toalha de Banho", "Higiene", 8.0, 4.6, 110, 0, rand() % 100 + 1, 'E'};
        Produto produto46 = {"Copo", "Copo de Vidro", "Utensilios", 3.0, 4.8, 150, 0, rand() % 100 + 1, 'A'};
        Produto produto47 = {"Prato", "Prato de Porcelana", "Utensilios", 5.0, 4.7, 140, 0, rand() % 100 + 1, 'B'};
        Produto produto48 = {"Garfo", "Garfo de Inox", "Utensilios", 2.0, 4.5, 160, 0, rand() % 100 + 1, 'C'};
        Produto produto49 = {"Faca", "Faca de Cozinha", "Utensilios", 4.0, 4.3, 130, 0, rand() % 100 + 1, 'D'};
        Produto produto50 = {"Chaveiro", "Chaveiro de Couro", "Acessorios", 6.0, 4.6, 170, 0, rand() % 100 + 1, 'E'};

        criarNovoProduto(produto1);
        criarNovoProduto(produto2);
        criarNovoProduto(produto3);
        criarNovoProduto(produto4);
        criarNovoProduto(produto5);
        criarNovoProduto(produto6);
        criarNovoProduto(produto7);
        criarNovoProduto(produto8);
        criarNovoProduto(produto9);
        criarNovoProduto(produto10);
        criarNovoProduto(produto11);
        criarNovoProduto(produto12);
        criarNovoProduto(produto13);
        criarNovoProduto(produto14);
        criarNovoProduto(produto15);
        criarNovoProduto(produto16);
        criarNovoProduto(produto17);
        criarNovoProduto(produto18);
        criarNovoProduto(produto19);
        criarNovoProduto(produto20);
        criarNovoProduto(produto21);
        criarNovoProduto(produto22);
        criarNovoProduto(produto23);
        criarNovoProduto(produto24);
        criarNovoProduto(produto25);
        criarNovoProduto(produto26);
        criarNovoProduto(produto27);
        criarNovoProduto(produto28);
        criarNovoProduto(produto29);
        criarNovoProduto(produto30);
        criarNovoProduto(produto31);
        criarNovoProduto(produto32);
        criarNovoProduto(produto33);
        criarNovoProduto(produto34);
        criarNovoProduto(produto35);
        criarNovoProduto(produto36);
        criarNovoProduto(produto37);
        criarNovoProduto(produto38);
        criarNovoProduto(produto39);
        criarNovoProduto(produto40);
        criarNovoProduto(produto41);
        criarNovoProduto(produto42);
        criarNovoProduto(produto43);
        criarNovoProduto(produto44);
        criarNovoProduto(produto45);
        criarNovoProduto(produto46);
        criarNovoProduto(produto47);
        criarNovoProduto(produto48);
        criarNovoProduto(produto49);
        criarNovoProduto(produto50);
    }
    else if (strcmp(nome, "bancoDeDadosUsuarios.txt") == 0){
        Usuario novoUsuario;

        // Inicialização dos campos do novo usuário
        strcpy(novoUsuario.nome, "admin");
        strcpy(novoUsuario.usuario, "admin");
        strcpy(novoUsuario.senha, "admin");

        // Inicialização dos produtos relacionados (exemplo com três produtos)
        strcpy(novoUsuario.produtos_relacionados[0], "Eletronicos");
        strcpy(novoUsuario.produtos_relacionados[1], "Higiene");
        strcpy(novoUsuario.produtos_relacionados[2], "Vestuario");
        strcpy(novoUsuario.produtos_relacionados[3], "Livros");
        strcpy(novoUsuario.produtos_relacionados[4], "Moveis");

        criarNovaConta(novoUsuario);
    }else
        return -1;
    return 0;
}
