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

//função de listar todos os registros, é a funcionalidade 3 do trabalho e tem como objetivo listar todos os registros do arquivo de dados
void listarRegistros(char *nomeArquivoEntrada);


#endif