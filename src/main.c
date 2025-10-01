//Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221

#include "../headers/createFiles.h"
#include "../headers/functions.h"
#include "../headers/auxiliar.h"

int main(){
  //variáveis que podem ser usadas para chamadas e execução de funções:
  //guarda o ponteiro para o nome de arquivo de entrada
  char *nomeArquivoEntrada;
  //guarda o ponteiro para o nome de arquivo de índice
  char *nomeArquivoIndice;
  //guarda o ponteiro para o nome de arquivo de saída
  char *nomeArquivoSaida;

  //primeiro deve-se receber a entrada do usuário para saber a saída desejada e qual função executar

  //obs: o programa terá apenas 1 entrada inicial
  char bufferEntrada[100];
  
  //variável responsável por armazenar o parâmetro inicial no primeiro uso de strtok
  char* parametro;
  int codFuncionalidade;
  //faz a leitura da entrada através de stdin
  fgets(bufferEntrada, sizeof(bufferEntrada), stdin);
  //removendo \n
  bufferEntrada[strcspn(bufferEntrada, "\n")] = '\0';
  //salvando primeiro parâmetro da entrada
  parametro = strtok(bufferEntrada, " ");
  //atribuindo o código da funcionalidade como inteiro para codFuncionalidade
  codFuncionalidade = atoi(parametro);

  //switch para executar a funcionalidade correta requisitada pelo usuário
  switch(codFuncionalidade){
    //funcionalidade 1: criar arquivo de index apenas com cabeçalho
    case 1:
      //obtem o nome do arquivo de indice passado como parâmetro
      nomeArquivoIndice = strtok(NULL, " ");
      criarIndice(nomeArquivoIndice);

      break;
    case 2:
      //obtem o nome do arquivo de entrada passado como parametro
      nomeArquivoEntrada = strtok(NULL, " ");
      //obtem o nome do arquivo de saida passado como parametro
      nomeArquivoSaida = strtok(NULL, " ");
      //obtem o nome do arquivo de índice passado como parametro
      nomeArquivoIndice = strtok(NULL, " ");

      //chama a funcionalidade 2
      criarArquivoDados(nomeArquivoEntrada, nomeArquivoSaida, nomeArquivoIndice);

      break;
    case 3: 
      break;
    case 4:
      break;
    //caso o usuário digite alguma opção que não é de 1 a 4
    default: 
      printf("Opcao invalida");
  }
  
  return 0;
}