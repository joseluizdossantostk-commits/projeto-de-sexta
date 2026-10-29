#ifndef PRODUTO_H
#define PRODUTO_H

#include "categoria.h"

#define MAX_PRODUTOS 50

typedef struct {
    int codigo;
    char nome[50];
    char prateleira;
    float preco;
    int quantidade;
    int disponivel;
    int categoria_codigo;   /* so o codigo; o nome vem da lista de categorias */
} Produto;

int buscarPorCodigo(Produto produtos[], int total, int codigo);
int buscarPorNome(Produto produtos[], int total, const char nome[]);
void exibirProduto(Produto p, Categoria categorias[], int totalCategorias);
int cadastrarProduto(Produto produtos[], int total,
                     Categoria categorias[], int totalCategorias);
void listarProdutos(Produto produtos[], int total,
                    Categoria categorias[], int totalCategorias);
void pesquisarPorCodigo(Produto produtos[], int total,
                        Categoria categorias[], int totalCategorias);
void pesquisarPorNome(Produto produtos[], int total,
                      Categoria categorias[], int totalCategorias);

/* Persistencia: arquivo de texto "produtos.txt" */
int carregarProdutos(Produto produtos[]);
void salvarProdutos(Produto produtos[], int total);

#endif
