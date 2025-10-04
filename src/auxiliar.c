//Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221

#include "auxiliar.h"

//essa função lê o arquivo csv e monta a lista ligada definida em auxiliar.h
registro* lerCSV(char *nomeArquivoEntrada){
  //contador de byteoffset para auxiliar na criação da lista de indice
  int byteoffset = 0;
  
  //criando ponteiro para um novo registro e um novo registro de indice
  registro* novoRegistro = calloc(1, sizeof(registro));
  indice* novoRegistroIndice = calloc(1, sizeof(indice));
 
  //criando buffer para ler uma linha do arquivo
  char bufferLinha[1024];
  
  //precisamos abrir o arquivo csv para leitura
  char caminho[] = "./dados.csv";
  FILE *arqDados = fopen(caminho, "r");
  //se der erro no processamento do arquivo, aparece a seguinte mensagem:
  if(arqDados == NULL){
    puts("Falha no processamento do arquivo.");

  }

  //fgets para ignorar a primeira linha do arquivo csv, que contém os nomes dos campos
  fgets(bufferLinha, sizeof(bufferLinha), arqDados);

  //aqui começa a leitura a partir da segunda linha do arquivo de dados
  while(fgets(bufferLinha, sizeof(bufferLinha), arqDados) != NULL){
    //contador de tamanho de registro, tem no mínimo 21 bytes por registro por causa dos campos de tamanho fixo
    long int tamRegistroBytes = 21;
    //removendo \n do fim da string
    bufferLinha[strcspn(bufferLinha, '\n')] = '\0';
    
    //a partir daqui a leitura dos campos é feita
    char campoIdPessoa[] = strsep(bufferLinha, ',');
    char campoIdadePessoa[] = strsep(bufferLinha, ',');
    char campoNomePessoa[] = strsep(bufferLinha, ',');
    char campoNomeUsuario[] = strsep(bufferLinha, ',');

    //INSERÇÃO NA LISTA DE REGISTROS:
    criarNoRegistro(novoRegistro, campoIdPessoa, campoIdadePessoa, campoNomePessoa, campoNomeUsuario, tamRegistroBytes, byteoffset);

    //INSERÇÃO NA LISTA DE ÍNDICE
    criarNoRegistroIndice(novoRegistroIndice, campoIdPessoa, byteoffset);
  }
}

//essa função cria o nó de um registro e retorna o byteoffset dele
long int criarNoRegistro(registro* novoRegistro, char campoIdPessoa, char campoIdadePessoa, char campoNomePessoa, char campoNomeUsuario, int tamRegistroBytes, long int byteoffset){
  

  //definindo id
  novoRegistro->idPessoa = atoi(campoIdPessoa);
  
  //definindo idade da pessoa
  if(atoi(campoIdadePessoa) == '\0'){
    novoRegistro->idadePessoa = 0;
  } else{
    novoRegistro->idadePessoa = atoi(campoIdadePessoa);
  }

  //definindo nome da pessoa
  if(campoNomePessoa != '\0'){
    strcpy(novoRegistro->nome, campoNomePessoa);
    //para esse campo, é necessário também armazenar o tamanho do nome da pessoa
    novoRegistro->tamanhoNomePessoa = strlen(campoNomePessoa);
    //incrementa os bytes do nome para o contador do tamanho do registro
    tamRegistroBytes += strlen(campoNomePessoa);
  } else{
    strcpy(novoRegistro->nome, '\0');
  }

  //definindo nome de usuário da pessoa
  if(campoNomeUsuario != '\0'){
    strcpy(novoRegistro->nomeUsuario, campoNomeUsuario);
    //para esse campo, é necessário também armazenar o tamanho do nome do usuario
    novoRegistro->tamanhoNomeUsuario = strlen(campoNomeUsuario);
    //incrementa os bytes do nome para o contador do tamanho do registro
    tamRegistroBytes += strlen(campoNomePessoa);
  } else{
    strcpy(novoRegistro->nomeUsuario, '\0');
  }

  //definindo dados que não são lidos do csv
  novoRegistro->removido = '0'; // 0 é não removido
  novoRegistro->tamRegistro = tamRegistroBytes; 
  novoRegistro->proxRegistro = raizLista; //insere o novo registro na raíz da lista
  raizLista = novoRegistro; //atualiza a raiz, que agora é o próprio novo registro

  //atualizando byteoffset para o byteoffset atual ser inserido corretamente na lista 
  byteoffset += tamRegistroBytes;
}

void criarNoRegistroIndice(indice* novoRegistroIndice,char campoIdPessoa, long int byteoffset){
  novoRegistroIndice->idPessoa = atoi(campoIdPessoa);
  novoRegistroIndice->byteOffset = byteoffset;
  //a inserção será ordenada, então começamos com um ponteiro auxiliar que aponta para a raiz da lista
  indice* ponteiroAuxiliar = raizListaIndice;

  //se a lista for nula:
  if(raizListaIndice == NULL){
    novoRegistroIndice->antIndice = NULL;
    novoRegistroIndice->proxIndice = NULL;
    raizListaIndice = novoRegistroIndice;
    return;
  }

  //enquanto o ponteiro auxiliar não for o último, esse loop estará ativo
  while(ponteiroAuxiliar->proxIndice != NULL){
    //declarando e criando ponteiro para o nó anterior e para o próximo nó do ponteiro auxiliar, já que a lista de indice é uma lista duplamente encadeada
    indice *ponteiroAuxiliarProx = ponteiroAuxiliar->proxIndice;
    indice *ponteiroAuxiliarAnt = ponteiroAuxiliar->antIndice;

    //nesse primeiro condicional, se encontra o caso em que o ponteiro é a raíz da lista e o que queremos inserir é menor que a raíz
    if(ponteiroAuxiliarAnt == NULL){
      if(novoRegistroIndice->idPessoa < ponteiroAuxiliar->idPessoa){
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
      novoRegistroIndice->proxIndice = ponteiroAuxiliar;
      novoRegistroIndice->antIndice = ponteiroAuxiliarAnt;
      ponteiroAuxiliarAnt->proxIndice = novoRegistroIndice;    ponteiroAuxiliar->antIndice = novoRegistroIndice;

      //a função acaba quando ocorre inserção
      return;
    } else{
      //se não for a raíz, e o id for maior que o id do ponteiro atual, pula pro próximo, garantindo assim, que se o nó novo tiver id maior que todos os nós da lista, ele só será inserido no fim da lista. Se ele for menor que algum nó da lista, em algum momento ele será inserido na posição correta
    }
    //se não for uma dessas opções, o ponteiro auxiliar passa pro próximo
    ponteiroAuxiliar = ponteiroAuxiliarProx;
  }
  //se sair do while e chegar aqui sem passar pelo while de novo, significa que chegamos no último nó, agora basta verificar se é maior ou menor e inserir
  //declarando e criando ponteiro para o nó anterior e para o próximo nó do ponteiro auxiliar, já que a lista de indice é uma lista duplamente encadeada
  indice *ponteiroAuxiliarProx = ponteiroAuxiliar->proxIndice;
  indice *ponteiroAuxiliarAnt = ponteiroAuxiliar->antIndice;
  //se for menor
  if(novoRegistroIndice->idPessoa < ponteiroAuxiliar->idPessoa){
    novoRegistroIndice->antIndice = ponteiroAuxiliarAnt;
    novoRegistroIndice->proxIndice = ponteiroAuxiliar;
    ponteiroAuxiliar->antIndice = novoRegistroIndice;
    ponteiroAuxiliarAnt->proxIndice = novoRegistroIndice;
    return;
  } else{
    novoRegistroIndice->antIndice = ponteiroAuxiliar;
    novoRegistroIndice->proxIndice = NULL;
    ponteiroAuxiliar->proxIndice = novoRegistroIndice;
  }
}