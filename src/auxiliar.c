//Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221

#include "../headers/auxiliar.h"

//definindo que as raízes são nulas
registro* raizLista = NULL;
indice* raizListaIndice = NULL;

//definindo ultimo elemento = NULL

registro* ultimoElemento = NULL;

//quantidade de pessoas presentes no arquivo de dados
int quantidadePessoas = 0;
//contador de byteoffset para auxiliar na criação da lista de indice, o byteoffset deve levar em consideração que existe um cabeçalho no arquivo de dados, que ocupa 16 bytes, portanto o primeiro byteoffset livre é:
long int byteoffset = 17;

//essa função lê o arquivo csv e monta a lista ligada definida em auxiliar.h
void lerCSV(const char *nomeArquivoEntrada){
  //criando buffer para ler uma linha do arquivo
  char bufferLinha[1024];
  
  //precisamos abrir o arquivo csv para leitura
  char caminho[] = "./dados.csv";
  FILE *arqDados = fopen(caminho, "r");
  //se der erro no processamento do arquivo, aparece a seguinte mensagem:
  if(arqDados == NULL){
    puts("Falha no processamento do arquivo.");
    return;
  }

  //fgets para ignorar a primeira linha do arquivo csv, que contém os nomes dos campos
  fgets(bufferLinha, sizeof(bufferLinha), arqDados);

  //aqui começa a leitura a partir da segunda linha do arquivo de dados
  while(fgets(bufferLinha, sizeof(bufferLinha), arqDados) != NULL){
    //print pra ver se deu certo:
    //printf("%s", bufferLinha);
    //contador de tamanho de registro, tem no mínimo 21 bytes por registro por causa dos campos de tamanho fixo
    long int tamRegistroBytes = 21;
    //removendo \n do fim da string
    bufferLinha[strcspn(bufferLinha, "\n")] = '\0';

    //criando ponteiro que aponta para o buffer que armazena a linha
    char *str = bufferLinha; 

    //a partir daqui a leitura dos campos é feita usando a função meu_strsep, explicada e declarada em auxiliar.h
    char* campoIdPessoa = meu_strsep(&str, ",");  //a leitura de um campo é feita observando o conteúdo de str, ou seja, a string que está em buffer linha    
    char* campoNomePessoa = meu_strsep(&str, ",");    
    char* campoIdadePessoa = meu_strsep(&str, ",");   
    char* campoNomeUsuario = meu_strsep(&str, ",");

      
    //criando ponteiro para um novo registro
    registro* novoRegistro = calloc(1, sizeof(registro));
    //INSERÇÃO NA LISTA DE REGISTROS: chama função
    criarNoRegistro(novoRegistro, campoIdPessoa, campoIdadePessoa, campoNomePessoa, campoNomeUsuario, tamRegistroBytes);

    //criando ponteiro para um novo registro de indice
    indice* novoRegistroIndice = calloc(1, sizeof(indice));
    //INSERÇÃO NA LISTA DE ÍNDICE: chama função
    criarNoRegistroIndice(novoRegistroIndice, campoIdPessoa, byteoffset);

    //atualizando byteoffset de modo que o próximo byteoffset livre seja depois do registro que acabou de ser inserido 
    byteoffset += raizLista->tamRegistro;
    //a raíz da lista sempre vai ser o elemento que acabou de ser inserido, então faz sentido pegar o tamanho desse registro para atualizar byteoffset
  
    //atualiza a quantidade de pessoas
    quantidadePessoas++;
  }

  //acabou a leitura do arquivo e a criação das 2 listas, fecha o arquivo
  fclose(arqDados);
}

//essa função cria o nó e adiciona esse nó novo a lista encadeada de registros do arquivo de dados
void criarNoRegistro(registro* novoRegistro, char *campoIdPessoa, char *campoIdadePessoa, char *campoNomePessoa, char *campoNomeUsuario, int tamRegistroBytes){
  //definindo id
  novoRegistro->idPessoa = atoi(campoIdPessoa);
  
  //definindo idade da pessoa
  if(campoIdadePessoa == NULL || campoIdadePessoa[0] == '\0'){
    novoRegistro->idadePessoa = 0;
  } else{
    novoRegistro->idadePessoa = atoi(campoIdadePessoa);
  }

  //definindo nome da pessoa
  if(campoNomePessoa != NULL){
    strcpy(novoRegistro->nome, campoNomePessoa);
    //para esse campo, é necessário também armazenar o tamanho do nome da pessoa
    novoRegistro->tamanhoNomePessoa = strlen(campoNomePessoa);
    //incrementa os bytes do nome para o contador do tamanho do registro
    tamRegistroBytes += strlen(campoNomePessoa);
  } else{
    strcpy(novoRegistro->nome, "");
  }

  //definindo nome de usuário da pessoa
  if(campoNomeUsuario != NULL && campoNomeUsuario[0] != '\0'){
    strcpy(novoRegistro->nomeUsuario, campoNomeUsuario);
    //para esse campo, é necessário também armazenar o tamanho do nome do usuario
    novoRegistro->tamanhoNomeUsuario = strlen(campoNomeUsuario);
    //incrementa os bytes do nome para o contador do tamanho do registro
    tamRegistroBytes += strlen(campoNomeUsuario);
  } else{
    strcpy(novoRegistro->nomeUsuario, "");
  }

  //caso para a raiz da lista ser nula
  if(raizLista == NULL){
    novoRegistro->removido[0] = '0'; // 0 é não removido
    novoRegistro->tamRegistro = tamRegistroBytes;
    raizLista = novoRegistro;
    novoRegistro->proxRegistro = NULL;
    ultimoElemento = novoRegistro;

    return;
  }

  //definindo dados que não são lidos do csv
  novoRegistro->removido[0] = '0'; // 0 é não removido
  novoRegistro->tamRegistro = tamRegistroBytes; 
  novoRegistro->proxRegistro = NULL; //insere o novo registro na raíz da lista
  ultimoElemento->proxRegistro = novoRegistro;
  ultimoElemento = novoRegistro; 

}

//essa função cria e adiciona um nó novo a lista duplamente encadeada de registros do arquivo de índice
void criarNoRegistroIndice(indice* novoRegistroIndice, char *campoIdPessoa, long int byteoffset){
  novoRegistroIndice->idPessoa = atoi(campoIdPessoa);
  novoRegistroIndice->byteOffset = byteoffset;
  //a inserção será ordenada, então começamos com um ponteiro auxiliar que aponta para a raiz da lista
  indice* ponteiroAuxiliar = raizListaIndice;

  //se a lista for nula:
  if(raizListaIndice == NULL){
    novoRegistroIndice->antIndice = NULL;
    novoRegistroIndice->proxIndice = NULL;
    novoRegistroIndice->idPessoa = atoi(campoIdPessoa);
    novoRegistroIndice->byteOffset = byteoffset;
    raizListaIndice = novoRegistroIndice;
    return;
  }

  //enquanto o ponteiro auxiliar não for o último, esse loop estará ativo
  while(ponteiroAuxiliar != NULL){
    //declarando e criando ponteiro para o nó anterior e para o próximo nó do ponteiro auxiliar, já que a lista de indice é uma lista duplamente encadeada
    indice *ponteiroAuxiliarProx = ponteiroAuxiliar->proxIndice;
    indice *ponteiroAuxiliarAnt = ponteiroAuxiliar->antIndice;

    //nesse primeiro condicional, se encontra o caso em que o ponteiro é a raíz da lista e o que queremos inserir é menor que a raíz
    if(ponteiroAuxiliarAnt == NULL){
      if(novoRegistroIndice->idPessoa < ponteiroAuxiliar->idPessoa){
        novoRegistroIndice->idPessoa = atoi(campoIdPessoa);
        novoRegistroIndice->byteOffset = byteoffset;
        
        novoRegistroIndice->proxIndice = ponteiroAuxiliar;
        novoRegistroIndice->antIndice = NULL;
        ponteiroAuxiliar->antIndice = novoRegistroIndice;
        raizListaIndice = novoRegistroIndice;
        //a função acaba quando ocorre a inserção
        return;
      } else { 
      }
    }
    //no segundo condicional, se não for a raíz, verificamos se o nó que queremos inserir tem id menor que o nó atual procurado, se sim insere antes disso, se não vai pro próximo; além disso, se for menor, o nó se insere no meio de dois nós, de modo que os ponteiro se ajeitem
    else if(novoRegistroIndice->idPessoa < ponteiroAuxiliar->idPessoa){
      novoRegistroIndice->idPessoa = atoi(campoIdPessoa);
      novoRegistroIndice->byteOffset = byteoffset;
      
      novoRegistroIndice->proxIndice = ponteiroAuxiliar;
      novoRegistroIndice->antIndice = ponteiroAuxiliarAnt;
      ponteiroAuxiliarAnt->proxIndice = novoRegistroIndice;    ponteiroAuxiliar->antIndice = novoRegistroIndice;
      //a função acaba quando ocorre inserção
      return;
    }
    //esse condicional verifica se é o último nó da lista
    else if(ponteiroAuxiliarProx == NULL){
      novoRegistroIndice->idPessoa = atoi(campoIdPessoa);
      novoRegistroIndice->byteOffset = byteoffset;

      novoRegistroIndice->antIndice = ponteiroAuxiliar;
      novoRegistroIndice->proxIndice = NULL;
      ponteiroAuxiliar->proxIndice = novoRegistroIndice;
      //retorna quando ocorre a inserção
    }
    //se não for uma dessas opções, o ponteiro auxiliar passa pro próximo
    ponteiroAuxiliar = ponteiroAuxiliarProx;
  }
}

char* meu_strsep(char** buffer, const char* delim) {
    char* inicio = *buffer;
    char* p;
    
    if (inicio == NULL) {
        return NULL;
    }
    
    p = strpbrk(inicio, delim);
    
    if (p) {
        *p = '\0';
        *buffer = p + 1;
    } else {
        *buffer = NULL;
    }
    
    return inicio;
}

//função que retorna a raíz da lista de registros de dados
registro* retornaRaizListaRegistro(){
  return raizLista;
}

//função que retorna a raíz da lista de registros de índice
indice* retornaRaizListaIndice(){
  return raizListaIndice;
}

//função que retorna a quantidade de pessoas armazenadas
int retornaQuantidadePessoas(){
  return quantidadePessoas;
}

//função que retorna o próximo byteoffset disponível
long int retornaProxByteOffset(){
  return byteoffset;
}
