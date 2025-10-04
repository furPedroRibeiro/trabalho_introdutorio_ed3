//Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221

//esse arquivo de cabeçalho .h tem como essência definir as funções, variáveis, etc. que podem ser usadas e implementadas por todos os arquivos .c, portanto são funções, estruturas de dados e variáveis auxiliares
#ifndef AUXILIAR_H
#define AUXILIAR_H

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

//definindo que a raiz da lista começa vazia
registro* raizLista = NULL;

typedef struct Indice{
  int idPessoa;
  long int byteOffset;
  struct indice *proxIndice;
  struct indice *antIndice;
} indice;

//definindo que a raiz da lista de indice começa vazia
indice* raizListaIndice = NULL;

//FUNÇÃO DE LER O ARQUIVO CSV

registro* lerCSV();
void criarNoRegistro(registro* novoRegistro, char campoIdPessoa, char campoIdadePessoa, char campoNomePessoa, char campoNomeUsuario, int tamRegistroBytes, long int byteoffset);

#endif











