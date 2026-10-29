#include <stdio.h>
#include <string.h>
#include "produto.h"
#include "util.h"

#define ARQUIVO_PRODUTOS "produtos.txt"

int buscarPorCodigo(Produto produtos[], int total, int codigo) {
    int i;
    for (i = 0; i < total; i++)
        if (produtos[i].codigo == codigo) return i;
    return -1;
}

int buscarPorNome(Produto produtos[], int total, const char nome[]) {
    int i;
    for (i = 0; i < total; i++)
        if (strcmp(produtos[i].nome, nome) == 0) return i;
    return -1;
}

void exibirProduto(Produto p, Categoria categorias[], int totalCategorias) {
    int pos;
    printf("Codigo: %d\n", p.codigo);
    printf("Nome: %s\n", p.nome);
    printf("Prateleira: %c\n", p.prateleira);
    printf("Preco unitario: R$ %.2f\n", p.preco);
    printf("Quantidade em estoque: %d\n", p.quantidade);
    printf("Disponivel para venda: %s\n", p.disponivel ? "Sim" : "Nao");
    pos = buscarCategoriaPorCodigo(categorias, totalCategorias, p.categoria_codigo);
    if (pos != -1) printf("Categoria: %s\n", categorias[pos].nome);
    else printf("Categoria: desconhecida\n");
}

int cadastrarProduto(Produto produtos[], int total,
                     Categoria categorias[], int totalCategorias) {
    Produto novo;
    if (total >= MAX_PRODUTOS) {
        printf("\nLista cheia! Nao e possivel cadastrar mais produtos.\n");
        return total;
    }
    if (totalCategorias == 0) {
        printf("\nNenhuma categoria cadastrada! Cadastre ao menos uma categoria\n"
               "(menu 4 - Categorias) antes de cadastrar um produto.\n");
        return total;
    }
    printf("\n===== CADASTRO =====\n");
    while (1) {
        novo.codigo = lerInteiro("Digite o codigo: ",
            "Codigo invalido! Digite um numero inteiro maior que zero: ", 1);
        if (buscarPorCodigo(produtos, total, novo.codigo) != -1)
            printf("Ja existe um produto com esse codigo! Tente outro.\n");
        else break;
    }
    lerNome("Digite o nome: ", novo.nome, sizeof(novo.nome));
    novo.prateleira = lerPrateleira();
    novo.preco = lerPreco();
    novo.quantidade = lerInteiro("Digite a quantidade em estoque: ",
        "Quantidade invalida! Digite um numero inteiro maior ou igual a zero: ", 0);
    novo.disponivel = novo.quantidade > 0;

    listarCategorias(categorias, totalCategorias);
    printf("\n");
    while (1) {
        novo.categoria_codigo = lerInteiro("Digite o codigo da categoria: ",
            "Codigo invalido! Digite um numero inteiro maior que zero: ", 1);
        if (buscarCategoriaPorCodigo(categorias, totalCategorias,
                                     novo.categoria_codigo) == -1)
            printf("Essa categoria nao existe! Escolha um codigo da lista.\n");
        else break;
    }

    produtos[total] = novo;
    printf("\nProduto cadastrado com sucesso!\n\n");
    exibirProduto(produtos[total], categorias, totalCategorias);
    return total + 1;
}

void listarProdutos(Produto produtos[], int total,
                    Categoria categorias[], int totalCategorias) {
    int i;
    printf("\n===== PRODUTOS CADASTRADOS =====\n");
    if (total == 0) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }
    for (i = 0; i < total; i++) {
        printf("\n--- Produto %d ---\n", i + 1);
        exibirProduto(produtos[i], categorias, totalCategorias);
    }
}

void pesquisarPorCodigo(Produto produtos[], int total,
                        Categoria categorias[], int totalCategorias) {
    int codigo, pos;
    printf("\n===== PESQUISAR POR CODIGO =====\n");
    codigo = lerInteiro("Digite o codigo: ",
        "Codigo invalido! Digite um numero inteiro maior que zero: ", 1);
    pos = buscarPorCodigo(produtos, total, codigo);
    if (pos == -1) printf("\nNenhum produto encontrado.\n");
    else {
        printf("\nProduto encontrado!\n");
        exibirProduto(produtos[pos], categorias, totalCategorias);
    }
}

void pesquisarPorNome(Produto produtos[], int total,
                      Categoria categorias[], int totalCategorias) {
    char nome[50];
    int pos;
    printf("\n===== PESQUISAR POR NOME =====\n");
    lerNome("Digite o nome: ", nome, sizeof(nome));
    pos = buscarPorNome(produtos, total, nome);
    if (pos == -1) printf("\nNenhum produto encontrado.\n");
    else {
        printf("\nProduto encontrado!\n");
        exibirProduto(produtos[pos], categorias, totalCategorias);
    }
}

/* Formato do arquivo: um produto por linha ->
   codigo;prateleira;preco;quantidade;disponivel;categoria_codigo;nome
   (o nome fica por ultimo porque pode ter espacos).
   Devolve quantos produtos foram lidos (0 se o arquivo nao existe). */
int carregarProdutos(Produto produtos[]) {
    FILE *f = fopen(ARQUIVO_PRODUTOS, "r");
    int total = 0;
    Produto p;
    if (f == NULL) return 0;               /* primeira execucao: lista vazia */
    while (total < MAX_PRODUTOS &&
           fscanf(f, "%d;%c;%f;%d;%d;%d;%49[^\n]\n",
                  &p.codigo, &p.prateleira, &p.preco, &p.quantidade,
                  &p.disponivel, &p.categoria_codigo, p.nome) == 7) {
        produtos[total++] = p;
    }
    fclose(f);
    return total;
}

void salvarProdutos(Produto produtos[], int total) {
    int i;
    FILE *f = fopen(ARQUIVO_PRODUTOS, "w");
    if (f == NULL) {
        printf("\nErro: nao foi possivel salvar %s!\n", ARQUIVO_PRODUTOS);
        return;
    }
    for (i = 0; i < total; i++)
        fprintf(f, "%d;%c;%.2f;%d;%d;%d;%s\n",
                produtos[i].codigo, produtos[i].prateleira, produtos[i].preco,
                produtos[i].quantidade, produtos[i].disponivel,
                produtos[i].categoria_codigo, produtos[i].nome);
    fclose(f);
}
