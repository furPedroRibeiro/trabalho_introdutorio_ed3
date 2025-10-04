//Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221

//create_file.c tem como principal função usar e implementar funções e variáveis definidas em createFile.h e funcoesAuxiliares.h
#include "../headers/createFiles.h"
#include "../headers/auxiliar.h"

//função para criar o arquivo de index
void criarIndice(const char *nomeArquivoIndice){
  //caminho onde os binário devem ser criados:
  char caminho[100] = "./bin/";
  //concatena o caminho com o nome do arquivo
  strcat(caminho, nomeArquivoIndice);
  //o que deve ser feito aqui é a abertura(criação) do arquivo de índice, que nesse momento deve conter apenas o cabeçalho
  //o arquivo em sua criação, tem status e ele está consistente
  char status = '1'; 
  //devemos adicionar o lixo ao cabeçalho também
  char lixo[] = "$$$$$$$$$$$";

  //cria o arquivo com o caminho e nome concatenados em strcat
  FILE *arqIndice = fopen(caminho, "wb");
  if(arqIndice == NULL){
    printf("Falha no processamento do arquivo.");
    return;
  }

  int numero = 05;
  //escreve o cabeçalho no arquivo
  fwrite(&status, sizeof(char), 1, arqIndice);
  fwrite(lixo, sizeof(char), strlen(lixo), arqIndice);

  fclose(arqIndice);
}

void criarArquivoDados(const char *nomeArquivoEntrada, const char *nomeArquivoSaida, const char *nomeArquivoIndice){
  //chama uma função para criar uma estrutura de dados com todos os registros do arquivo de entrada
  lerCSV(nomeArquivoEntrada);
}