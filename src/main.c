//Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221

#include "../headers/create_files.h"
#include "../headers/functions.h"

int main(){
  //variáveis que podem ser usadas:
  char *nomeArquivoIndice;
  //primeiro deve-se receber a entrada do usuário para saber a saída desejada e qual função executar
  //obs: o programa terá apenas 1 entrada inicial
  char bufferEntrada[100];
  char* parametro;
  int codFuncionalidade;
  if(fgets(bufferEntrada, sizeof(bufferEntrada), stdin)!=NULL){
    //removendo \n
    bufferEntrada[strcspn(bufferEntrada, "\n")] = '\0';
    parametro = strtok(bufferEntrada, " ");
    codFuncionalidade = atoi(parametro);
  } else{
    printf("Entrada invalida!!!");
    return 0;
  }

  switch(codFuncionalidade){
    case 1:
      nomeArquivoIndice = strtok(NULL, " ");
      createIndex(nomeArquivoIndice);

      break;
    case 2: 
      break;
    case 3: 
      break;
    case 4:
      break;
    default: 
      printf("Opcao invalida");
  }
  
  return 0;
}