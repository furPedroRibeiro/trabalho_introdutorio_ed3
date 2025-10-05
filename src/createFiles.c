//Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221

//create_file.c tem como principal função usar e implementar funções e variáveis definidas em createFile.h e funcoesAuxiliares.h
#include "../headers/createFiles.h"
#include "../headers/auxiliar.h"

//função para criar o arquivo de indice
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
  //chama uma função para criar uma estrutura de dados com todos os registros do arquivo de entrada, e cria também a estrutura de dados do indice
  lerCSV(nomeArquivoEntrada);

  //aqui as raízes das listas são obtidas
  registro* raizLista = retornaRaizListaRegistro();
  indice* raizListaIndice = retornaRaizListaIndice();
  
  //mostra elementos da raiz
  // printf("Tamanho do registro: %d", raizLista->tamRegistro);
  // printf("Removido: %c", raizLista->removido);
  // printf("ID da pessoa 1: %d", raizLista->idPessoa);
  // printf("Idade da pessoa 1: %d", raizLista->idadePessoa);
  // printf("Nome da pessoa 1: %s", raizLista->nome);
  // printf("Tamanho nome da pessoa 1: %d", raizLista->tamanhoNomePessoa);
  // printf("Nome de usuário 1: %s", raizLista->nomeUsuario);
  // printf("Tamanho nome da usuário 1: %d", raizLista->tamanhoNomeUsuario);

  //com a estrutura de dados criada, o que resta é inserir nos arquivos de dados e índice os dados coletados de dados.csv

  //abrindo os arquivos:

  //fazendo com que o arquivo seja criado em ./bin
  char caminho_1[100] = "./bin/";
  strcat(caminho_1, nomeArquivoIndice);
  //o arquivo de indice precisa existir, então é aberto para append binary
  FILE* arqIndice = fopen(caminho_1, "ab");

  if(arqIndice == NULL){
    puts("Falha no processamento do arquivo.");
    return;
  }

  //fazendo com que o arquivo seja criado em ./bin
  char caminho[100] = "./bin/";
  strcat(caminho, nomeArquivoSaida);
  //o arquivo de dados é aberto de maneira que seja criado se não existir
  FILE* arqDadosSaida = fopen(caminho, "wb");
  if(arqDadosSaida == NULL){
    puts("Falha no processamento do arquivo.");
    return;
  }

  //INSERINDO TUDO NO ARQUIVO DE DADOS:
  //primeiro precisamos criar o cabeçalho:
  //informações do cabeçalho
  char status = '1';
  int quantidadePessoas = retornaQuantidadePessoas();
  int quantidadeRemovidos = 0;
  long int proxByteoffset = retornaProxByteOffset();

  //escreve status no cabeçalho:
  fwrite(&status, sizeof(char), 1, arqDadosSaida);
  fwrite(&quantidadePessoas, sizeof(int), 1, arqDadosSaida);
  fwrite(&quantidadeRemovidos, sizeof(int), 1, arqDadosSaida);
  fwrite(&proxByteoffset, sizeof(long int), 1, arqDadosSaida);
  //cria nó auxiliar
  registro* noAuxiliar = raizLista;
  //laço de repetição para inserir todos os nós da lista no arquivo
  while(noAuxiliar != NULL){
    //obtendo todos os dados
    int tamanhoRegistro = noAuxiliar->tamRegistro;
    int idPessoa = noAuxiliar->idPessoa;
    int idadePessoa = noAuxiliar->idadePessoa;
    int tamNomePessoa = noAuxiliar->tamanhoNomePessoa;
    char *nomePessoa = noAuxiliar->nome;
    int tamNomeUsuario = noAuxiliar->tamanhoNomeUsuario;
    char *nomeUsuario = noAuxiliar->nomeUsuario;
    
    //print nas variáveis pra ver se ta escrevendo certo
    printf("\n\nRemovido: %c Tamanho do Registro: %d Id pessoa: %d Idade pessoa: %d Tamanho do nome da pessoa: %d Nome da pessoa: %s Tamanho do nome de usuario: %d Nome de Usuario: %s", noAuxiliar->removido[0], tamanhoRegistro, idPessoa, idadePessoa, tamNomePessoa, nomePessoa, tamNomeUsuario, nomeUsuario);

    //fwrite pro arquivo
    fwrite(&noAuxiliar->removido[0], sizeof(char), 1, arqDadosSaida);
    fwrite(&tamanhoRegistro, sizeof(int), 1, arqDadosSaida);
    fwrite(&idPessoa, sizeof(int), 1, arqDadosSaida);
    fwrite(&idadePessoa, sizeof(int), 1, arqDadosSaida);
    fwrite(&tamNomePessoa, sizeof(int), 1, arqDadosSaida);
    fwrite(&nomePessoa, sizeof(char), strlen(nomePessoa), arqDadosSaida);
    fwrite(&tamNomeUsuario, sizeof(int), 1, arqDadosSaida);
    fwrite(&nomeUsuario, sizeof(char), strlen(nomeUsuario), arqDadosSaida);

    noAuxiliar = noAuxiliar->proxRegistro;
  }

  //INSERÇÃO NO ARQUIVO DE ÍNDICE:
  //o arquivo de indice já existe, portanto já tem cabeçalho existente
  //noAuxiliarIndice para receber a raíz da lista de registros de indice
  indice* noAuxiliarIndice = raizListaIndice;

  //loop para inserir todos os nós da lista no arquivo
  while(noAuxiliarIndice != NULL){
    //obtendo informações
    int idPessoa = noAuxiliarIndice->idPessoa;
    long int byteoffset = noAuxiliarIndice->byteOffset;

    //print pra ver se deu certo:
    printf("\n\nId pessoa: %d byteoffset: %ld", idPessoa, byteoffset);


    //escreve no arquivo de índice
    fwrite(&idPessoa, sizeof(int), 1, arqIndice);
    fwrite(&byteoffset, sizeof(long int), 1, arqIndice);

    //atualiza o nó atual
    noAuxiliarIndice = noAuxiliarIndice->proxIndice;
  }

  fclose(arqIndice);
  fclose(arqDadosSaida);

  puts("DEU CERTOOOOO");

}