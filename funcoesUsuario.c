#include "TADGeral.h"
#include "TADnos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Estrutura da Lista de usuarios
typedef struct listaUsuario{
    NoUser* inicio;
}ListaUsuario;

//Fun��o usada para a cria��o de uma lista nova e vazia
ListaUsuario *criarUsuarios(){
    ListaUsuario *l = (ListaUsuario*) malloc (sizeof(ListaUsuario));
    l->inicio = NULL;
    return l;
}

//Fun��o para checar se a lista est� vazia, caso esteja vazia retorne 0 caso n�o esteja vazia retorna 1
int listaVaziaUsuarios(ListaUsuario *l){
    if(l->inicio == NULL)
        return 0;
    return 1;
}

//Fun��o para limpar a lista por completo at� ela ficar vazia
/*void limparUsuarios(ListaUsuario *l){
    while (listaVaziaUsuarios(l) != 0){
       removerUsuario(l);
    }
}*/

//Retorna o n�mero de elementos da lista percorrendo os n�s da lista e contando-os.
int tamanhoUsuarios(ListaUsuario *l){
    int contador = 0;
    NoUser *no = l->inicio;

    while(no != NULL){
        contador++;
        no = no->prox;
    }
    return contador;
}

//Respons�vel por percorrer a lista e exibir os itens.
void mostrarUsuarios(ListaUsuario *l){
    NoUser *no = l->inicio;

    while(no != NULL){
        //Exibe os campos do item no da lista
        printf("Nome: %s\n", no->user.nome);
        printf("Usuario: %s\n", no->user.usuario);
        printf("\n");

        //Move para o pr�ximo item da lista
        no = no->prox;
    }
}

//Insere um usuario no banco de dados
int criarNovaConta(Usuario novoUsuario) {
    FILE *bancoDeDadosUsuarios = fopen("bancoDeDadosUsuarios.txt", "a");
    if (bancoDeDadosUsuarios != NULL) {

        fprintf(bancoDeDadosUsuarios, "Nome: %s\n", novoUsuario.nome);
        fprintf(bancoDeDadosUsuarios, "Usu�rio: %s\n", novoUsuario.usuario);
        fprintf(bancoDeDadosUsuarios, "Senha: %s\n", novoUsuario.senha);
        fprintf(bancoDeDadosUsuarios, "Produtos relacionados:\n");

        for (int i = 0; i < 5; i++)
            fprintf(bancoDeDadosUsuarios, "   - %s\n", novoUsuario.produtos_relacionados[i]);

        fprintf(bancoDeDadosUsuarios, "\n");

        fclose(bancoDeDadosUsuarios);
        return 0;
    } else {
        fclose(bancoDeDadosUsuarios);
        printf("Falha ao abrir o banco de dados dos usuarios.\n");
        return 1; // Retornar 1 para indicar erro na abertura do arquivo
    }
}

//Verifica se as credenciais do usuario estao corretas e retorna um ponteiro para o n� dele na lista
int buscarConta(ListaUsuario *l, char *user, char *senha, NoUser **conta){
    if (l == NULL) return -2;
    if (listaVaziaUsuarios(l) == 0) return -1;

    NoUser *no = l->inicio;
    while (no != NULL) {
        if (strcmp(user, no->user.usuario) == 0) {
            if (strcmp(senha, no->user.senha) == 0) {
                *conta = no;
                return 0; // Usu�rio e senha correspondem, retornar ponteiro para a conta do usu�rio
            } else {
                return -7;
            }
        }
        no = no->prox;
    }
    return -8;
}

//Insere um usuario no inicio da lista
int inserirContaNova(ListaUsuario *l, Usuario it){
    if (l == NULL) return -2;
    NoUser *no = (NoUser*)malloc(sizeof(NoUser));
    no->user = it;
    no->prox = l->inicio;
    l->inicio = no;

    return 0;
}
