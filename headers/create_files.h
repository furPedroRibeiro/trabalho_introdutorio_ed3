//Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221

//esse arquivo de cabeçalho .h tem como essência definir as funções, variáveis, etc. que serão usadas e implementadas em create_file.c
#ifndef CREATE_FILE_H
#define CREATE_FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Registro{
  char removido;
  int tamRegistro;
  int idPessoa;
  int idadePessoa;
  int tamanhoNomePessoa;
  char *nome;
  int tamanhoNomeUsuario;
  char *nomeUsuario;
  struct registro *proxRegistro;
} registro;

typedef struct Index{
  int idPessoa;
  long int byteOffset;
  struct index *proxIndex;
} index;

void createIndex(const char *nomeArquivoIndice);


#endif











