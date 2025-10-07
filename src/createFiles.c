//Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221

//create_file.c tem como principal função usar e implementar funções e variáveis definidas em createFile.h e funcoesAuxiliares.h
#include "../headers/createFiles.h"
#include "../headers/auxiliar.h"

//inclui utilidades.h para usar a função binário na tela
#include "../headers/utilidades.h"

//função para criar o arquivo de indice
void criarIndice(char *nomeArquivoIndice){
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

  //fecha o arquivo
  fclose(arqIndice);

  //usa binario na tela como especificado no trabalho
  binarioNaTela(nomeArquivoIndice);
}

void criarArquivoDados(const char *nomeArquivoEntrada, char *nomeArquivoSaida, char *nomeArquivoIndice){
  //testando existência do arquivo de índice:
  //fazendo com que o arquivo seja criado em ./bin
  char caminho_1[100] = "./bin/";
  strcat(caminho_1, nomeArquivoIndice);
  //o arquivo de indice precisa existir, por isso esse teste é realizado antes:
  FILE* testArqIndice = fopen(caminho_1, "rb");
  if(testArqIndice == NULL){
    puts("Falha no processamento do arquivo.");
    return;
  }

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

  //abrindo o arquivo:
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
  char status = '0';
  int quantidadePessoas = retornaQuantidadePessoas();
  int quantidadeRemovidos = 0;
  int64_t proxByteoffset = retornaProxByteOffset();

  //escreve cabeçalho
  //variável para contar se o tamanho do cabeçalho ta certo:
  int tamCabecalho = 0;
  fwrite(&status, sizeof(char), 1, arqDadosSaida);
  tamCabecalho += sizeof(char);
  fwrite(&quantidadePessoas, sizeof(int), 1, arqDadosSaida);
  tamCabecalho += sizeof(int);
  fwrite(&quantidadeRemovidos, sizeof(int), 1, arqDadosSaida);
  tamCabecalho += sizeof(int);
  fwrite(&proxByteoffset, sizeof(int64_t), 1, arqDadosSaida);
  tamCabecalho += sizeof(int64_t);

  //printa cabeçalho pra testar:
  // printf("\nStatus: (%c), Quantidade de pessoas: (%d), Quantidade de removidos: (%d), Próximo Byteoffset disponível: (%ld)", status, quantidadePessoas, quantidadeRemovidos, proxByteoffset);

  //cria nó auxiliar
  registro* noAuxiliar = raizLista;
  //laço de repetição para inserir todos os nós da lista no arquivo
  while(noAuxiliar != NULL){
    //tamanho do que foi escrito em cada registro:
    int tamanhoRegistroEscrito = 0;
    
    //obtendo todos os dados
    int tamanhoRegistro = noAuxiliar->tamRegistro;
    int idPessoa = noAuxiliar->idPessoa;
    int idadePessoa = noAuxiliar->idadePessoa;
    int tamNomePessoa = noAuxiliar->tamanhoNomePessoa;
    char *nomePessoa = noAuxiliar->nome;
    int tamNomeUsuario = noAuxiliar->tamanhoNomeUsuario;
    char *nomeUsuario = noAuxiliar->nomeUsuario;
    
    //print nas variáveis pra ver se ta escrevendo certo
    // printf("\n\nRemovido: (%c) Tamanho do Registro: (%d) Id pessoa: (%d) Idade pessoa: (%d) Tamanho do nome da pessoa: (%d) Nome da pessoa: (%s) Tamanho do nome de usuario: (%d) Nome de Usuario: (%s)", noAuxiliar->removido[0], tamanhoRegistro, idPessoa, idadePessoa, tamNomePessoa, nomePessoa, tamNomeUsuario, nomeUsuario);

    //fwrite pro arquivo
    //escrevendo se o registro está logicamente removido ou não
    if(fwrite(&noAuxiliar->removido[0], sizeof(char), 1, arqDadosSaida) != 1){
        //deu erro
        // puts("\n\nERRO AO ESCREVER LINHA\n\n");
    }
    tamanhoRegistroEscrito += sizeof(char);
    //escrevendo tamanho do registro da pessoa
    if(fwrite(&tamanhoRegistro, sizeof(int), 1, arqDadosSaida)
 != 1){
        //deu erro
        // puts("\n\nERRO AO ESCREVER LINHA\n\n");
    }
    tamanhoRegistroEscrito += sizeof(int);
    //escrevendo id pessoa
    if(fwrite(&idPessoa, sizeof(int), 1, arqDadosSaida)
 != 1){
        //deu erro
        // puts("\n\nERRO AO ESCREVER LINHA\n\n");
    }
    tamanhoRegistroEscrito += sizeof(int);
    //escrevendo idade da pessoa
    if(fwrite(&idadePessoa, sizeof(int), 1, arqDadosSaida)
 != 1){
        //deu erro
        // puts("\n\nERRO AO ESCREVER LINHA\n\n");
    }
    tamanhoRegistroEscrito += sizeof(int);
    //escrevendo tamanho do nome da pessoa pessoa
    //se o nome da pessoa for vazio, não inserimos nada no arquivo de dados
    fwrite(&tamNomePessoa, sizeof(int), 1, arqDadosSaida);
    tamanhoRegistroEscrito += sizeof(int);
    //escrevendo nome da pessoa
    if(tamNomePessoa == 0){
      // puts("NOME VAZIO TESTE");
      //não insere nome da pessoa no arquivo de dados, para nomes nulos(vazios), apenas identificamos que o tamanho do nome é 0 e sabemos que ele é vazio
    } 
    //se não for vazio, insere o nome da pessoa no arquivo de dados
    else if(tamNomePessoa > 0){
      fwrite(nomePessoa, sizeof(char), tamNomePessoa, arqDadosSaida);
    }
    tamanhoRegistroEscrito += (sizeof(char)*tamNomePessoa);

    //analogamente, fazemos isso para o nome de usuário
    //escrevendo tamanho do nome do usuário
    fwrite(&tamNomeUsuario, sizeof(int), 1, arqDadosSaida);
    tamanhoRegistroEscrito += sizeof(int);
    //escrevendo nome da pessoa
    if(tamNomeUsuario == 0){
      //não insere nome da pessoa no arquivo de dados, para nomes nulos(vazios), apenas identificamos que o tamanho do nome é 0 e sabemos que ele é vazio
    } 
    //se não for vazio, insere o nome da pessoa no arquivo de dados
    else if(tamNomeUsuario > 0){
      fwrite(nomeUsuario, sizeof(char), tamNomeUsuario, arqDadosSaida);
    }
    tamanhoRegistroEscrito += (sizeof(char)*tamNomeUsuario);
    
    //tamanho do registro escrito no arquivo de dados:
    // printf("Tamanho do registro escrito: (%d)", tamanhoRegistroEscrito);

    //atualizando nó auxiliar
    noAuxiliar = noAuxiliar->proxRegistro;
  }

  //definindo arquivo como consistente depois de escrever tudo
  char consistente = '1';
  fseek(arqDadosSaida, 0, SEEK_SET);
  fwrite(&consistente, sizeof(char), 1, arqDadosSaida);
  //fechando arquivo
  fclose(arqDadosSaida);




  //INSERÇÃO NO ARQUIVO DE ÍNDICE:

  //o arquivo de indice precisa existir, e escrevemos nele após o cabeçalho, então é aberto como append binary
  FILE* arqIndice = fopen(caminho_1, "ab");

  if(arqIndice == NULL){
    puts("Falha no processamento do arquivo.");
    return;
  }
  //o arquivo de indice já existe, portanto já tem cabeçalho existente
  //noAuxiliarIndice para receber a raíz da lista de registros de indice
  indice* noAuxiliarIndice = raizListaIndice;

  //loop para inserir todos os nós da lista no arquivo
  while(noAuxiliarIndice != NULL){
    //obtendo informações
    int idPessoa = noAuxiliarIndice->idPessoa;
    int64_t byteoffset = noAuxiliarIndice->byteOffset;

    //print pra ver se deu certo:
    // printf("\n\nId pessoa: %d byteoffset: %ld ", idPessoa, byteoffset);


    //escreve no arquivo de índice
    fwrite(&idPessoa, sizeof(int), 1, arqIndice);
    fwrite(&byteoffset, sizeof(int64_t), 1, arqIndice);

    //atualiza o nó atual
    noAuxiliarIndice = noAuxiliarIndice->proxIndice;
  }

  //fechando o arquivo
  fclose(arqIndice);

  //usa função binário na tela como especificado no trabalho
  
  binarioNaTela(nomeArquivoSaida);
  binarioNaTela(nomeArquivoIndice);
}