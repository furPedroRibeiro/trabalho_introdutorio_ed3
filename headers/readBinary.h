//Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221

//esse arquivo de cabeçalho .h tem como essência definir as funções, variáveis, etc. que serão usadas e implementadas em functions.c
#ifndef READ_BINARY_H
#define READ_BINARY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura auxiliar para armazenar os campos de um registro
struct registro2 {
    int idPessoa, idadePessoa;
    int tamNomePessoa, tamNomeUsuario;
    char nomePessoa[100];
    char nomeUsuario[100];
} reg;

// Função para listar todos os registros (funcionalidade 3)
void listarRegistros(char *nomeArquivoEntrada);
// Função para buscar registros (funcionalidade 4)
void buscarregistros(char *nomeArquivoPessoa, char *nomeArquivoIndice, int n);
// Função auxiliar para imprimir um registro
void imprimirRegistro(int idPessoa, int idadePessoa, int tamNomePessoa, char *nomePessoa, int tamNomeUsuario, char *nomeUsuario);


#endif