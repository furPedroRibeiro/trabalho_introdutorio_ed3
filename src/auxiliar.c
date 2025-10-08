//Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221

#include "../headers/auxiliar.h"

//definindo que as raízes são nulas
registro* raizLista = NULL;
indice* raizListaIndice = NULL;

//definindo ultimo elemento = NULL

registro* ultimoElemento = NULL;

//definindo variáveis para o cabeçalho do arquivo de dados
//abriremos o arquivo com status consistente
char status = '1'; 
//contador da quantidade de pessoas presentes no arquivo de dados
int quantidadePessoas = 0;
//contador da quantidade de pessoas removidas no arquivo de dados
int quantidadeRemovidos = 0;
//definindo variável para o próximo byteoffset disponível
//pula o cabeçalho, por isso começa em 17
int64_t proxByteoffset = 17;

//essa função lê o arquivo csv, faz a inserção no arquivo de dados a cada linha lida do csv, e vai montando a lista duplamente encadeada para o índice ser inserido depois no arquivo de índice
void lerCSV(char *nomeArquivoEntrada, char *nomeArquivoDados, char *nomeArquivoIndice){
  //criando buffer para ler uma linha do arquivo
  char bufferLinha[1024];

  //vamos testar se o arquivo de dados será criado corretamente:
  char caminho[100] = "./";
  strcat(caminho, nomeArquivoDados);
  //cria e abre arquivo pra escrita
  FILE* arqDados = fopen(caminho, "wb");
  if(arqDados == NULL){
    puts("Falha no processamento do arquivo.");
    return;
  } else{
    fclose(arqDados);
  }
  //agora testando se o arquivo de índice existe:
  char caminho_1[100] = "./";
  strcat(caminho_1, nomeArquivoIndice);
  //cria e abre arquivo pra escrita
  FILE* arqIndice = fopen(caminho_1, "ab");
  if(arqIndice == NULL){
    puts("Falha no processamento do arquivo.");
    return;
  } else{
    fclose(arqIndice);
  }
  
  //precisamos abrir o arquivo csv para leitura
  char caminho_2[] = "./dados.csv";
  FILE *arqEntrada = fopen(caminho_2, "r");
  //se der erro no processamento do arquivo, aparece a seguinte mensagem:
  if(arqEntrada == NULL){
    puts("Falha no processamento do arquivo.");
    return;
  }

  //fgets para ignorar a primeira linha do arquivo csv, que contém os nomes dos campos
  fgets(bufferLinha, sizeof(bufferLinha), arqEntrada);

  //função para criar o arquivo de dados e definir o cabeçalho é chamada antes da leitura começar
  criaCabecalhoArquivoDados(nomeArquivoDados, status, quantidadePessoas, quantidadeRemovidos, proxByteoffset);

  //aqui começa a leitura a partir da segunda linha do arquivo de dados
  while(fgets(bufferLinha, sizeof(bufferLinha), arqEntrada) != NULL){
    //declarando variável para obter o byteoffset do registro que será inserido nessa iteração
    int64_t byteoffsetAtual = proxByteoffset;
    //contador de tamanho de registro, tem no mínimo 21 bytes por registro por causa dos campos de tamanho fixo
    int64_t tamRegistroBytes = 21;
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
    //inicializando cabeçalho do arquivo de dados:
    criaCabecalhoArquivoDados(nomeArquivoDados, status, quantidadePessoas, quantidadeRemovidos, proxByteoffset);
    //agora é preciso fazer uma inserção no arquivo de dados, para cada leitura é necessário fazer uma inserção no arquivo de dados
    insereRegistro(novoRegistro, nomeArquivoDados, quantidadePessoas, quantidadeRemovidos, proxByteoffset);

    //depois que o registro é inserido, podemos adicionar um nó para o índice na lista de índice, que vai ser criada inteira antes de adicionarmos ela para o arquivo de índice

    //criando ponteiro para um novo registro de indice
    indice* novoRegistroIndice = calloc(1, sizeof(indice));
    //INSERÇÃO NA LISTA DE ÍNDICE: chama função
    criarNoRegistroIndice(novoRegistroIndice, campoIdPessoa, byteoffsetAtual);
  }

  //acabou a leitura do arquivo e a inserção de registros no arquivo de dados, agora é necessário fazer a inserção no arquivo de índice. A lista de índices já estará ordenada e com os byteoffsets certos, já que fizemos uma inserção a cada leitura
  //o arquivo csv pode ser fechado:
  fclose(arqEntrada);

  insereRegistroIndice(raizListaIndice, nomeArquivoIndice);
}

//função que cria cabeçalho do arquivo de dados
void criaCabecalhoArquivoDados(char* nomeArquivoDados, char* status, int quantidadePessoas, int quantidadeRemovidos, int64_t proxByteoffset){
  //aqui o arquivo de dados é criado e o cabeçalho é definido com os dados iniciais
  char caminho[100] = "./";
  strcat(caminho, nomeArquivoDados);
  //cria e abre arquivo pra escrita
  FILE* arqDados = fopen(caminho, "wb");

  //escreve cabeçalho
  //variável para contar se o tamanho do cabeçalho ta certo:
  int tamCabecalho = 0;
  fwrite(&status, sizeof(char), 1, arqDados);
  tamCabecalho += sizeof(char);
  fwrite(&quantidadePessoas, sizeof(int), 1, arqDados);
  tamCabecalho += sizeof(int);
  fwrite(&quantidadeRemovidos, sizeof(int), 1, arqDados);
  tamCabecalho += sizeof(int);
  fwrite(&proxByteoffset, sizeof(int64_t), 1, arqDados);
  tamCabecalho += sizeof(int64_t);

  fclose(arqDados);
}

//função para inserir um registro no arquivo de dados
void insereRegistro(registro* novoRegistro, char* nomeArquivoDados, int quantidadePessoas, int quantidadeRemovidos, int64_t proxByteoffset){
  //a cada inserção, o cabeçalho é atualizado com a quantidade de pessoas, o número de pessoas removidas e o próximo byte offset disponível, além disso, o status é definido pra 1 no começo da inserção para indicar que o arquivo está inconsistente e depois da inserção ele é 0 para mostrar que está consistente

  //o arquivo já foi criado com o cabeçalho, então nos resta apenas abrir para escrita no fim do arquivo
  char caminho[100] = "./";
  strcat(caminho, nomeArquivoDados);
  FILE* arqDados = fopen(caminho, "ab");

  //primeiro já vamos reescrever o status para 0
  char statusInconsistente = '0';
  fseek(arqDados, 0, SEEK_SET);
  fwrite(&status, sizeof(char), 1, arqDados);

  //agora reposicionamos o ponteiro do arquivo para escrever o registro, aproveitando que proxByteOffset ainda não foi atualizado
  fseek(arqDados, proxByteoffset, SEEK_SET);

  //inserção:
  int tamanhoRegistro = novoRegistro->tamRegistro;
  int idPessoa = novoRegistro->idPessoa;
  int idadePessoa = novoRegistro->idadePessoa;
  int tamNomePessoa = novoRegistro->tamanhoNomePessoa;
  char *nomePessoa = novoRegistro->nome;
  int tamNomeUsuario = novoRegistro->tamanhoNomeUsuario;
  char *nomeUsuario = novoRegistro->nomeUsuario;

  fwrite(&novoRegistro->removido[0], sizeof(char), 1, arqDados);
  fwrite(&tamanhoRegistro, sizeof(int), 1, arqDados);
  fwrite(&idPessoa, sizeof(int), 1, arqDados);
  fwrite(&idadePessoa, sizeof(int), 1, arqDados);
  fwrite(&tamNomePessoa, sizeof(int), 1, arqDados);
  if(tamNomePessoa == 0){
    //não escreve nome da pessoa
  } else{
    fwrite(nomePessoa, sizeof(char), tamNomePessoa, arqDados);
  }
  fwrite(&tamNomeUsuario, sizeof(int), 1, arqDados);
  if(tamNomeUsuario == 0){
    //não escreve nome do usuário
  } else{
    fwrite(nomeUsuario, sizeof(char), tamNomeUsuario, arqDados);
  }

  //atualizando byteoffset de modo que o próximo byteoffset livre seja depois do registro que acabou de ser inserido 
  proxByteoffset += novoRegistro->tamRegistro;
  //agora devemos atualizar o registro de cabeçalho:
  quantidadePessoas++;
  quantidadeRemovidos = 0;
  //movendo ponteiro do arquivo para o byte 1:
  fseek(arqDados, 1, SEEK_SET);
  //para atualizar a quantidade de pessoas, vamos escrever um inteiro
  fwrite(&quantidadePessoas, sizeof(int), 1, arqDados);
  fwrite(&quantidadeRemovidos, sizeof(int), 1, arqDados);
  fwrite(&proxByteoffset, sizeof(int64_t), 1, arqDados);
  //com o cabeçalho todo reescrito e o registro novo cadastrado, basta atualizar o status novamente para 1 e depois posicionar o ponteiro do arquivo para a próxima inserção:
  char statusConsistente = '1';
  fseek(arqDados, 0, SEEK_SET);
  fwrite(&statusConsistente, sizeof(char), 1, arqDados);
  fseek(arqDados, proxByteoffset, SEEK_SET);

  fclose(arqDados);
}

void insereRegistroIndice(indice* raizListaIndice, char* nomeArquivoIndice){
  //é necessário abrir o arquivo de índice para escrita a partir do final, porque o arquivo já vai existir
  char caminho_1[100] = "./";
  strcat(caminho_1, nomeArquivoIndice);
  FILE* arqIndice = fopen(caminho_1, "ab");

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
}

//essa função cria o nó e adiciona esse nó novo a lista encadeada de registros do arquivo de dados
void criarNoRegistro(registro* novoRegistro, char *campoIdPessoa, char *campoIdadePessoa, char *campoNomePessoa, char *campoNomeUsuario, int tamRegistroBytes){
  //definindo id, não precisa de tratamento pois id não pode ser nulo
  novoRegistro->idPessoa = atoi(campoIdPessoa);
  
  //definindo idade da pessoa
  if(campoIdadePessoa == NULL || campoIdadePessoa[0] == '\0'){
    novoRegistro->idadePessoa = -1;
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
    novoRegistro->tamanhoNomePessoa = 0;
    tamRegistroBytes += 0;
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
    novoRegistro->tamanhoNomeUsuario = 0;
    tamRegistroBytes += 0;
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
void criarNoRegistroIndice(indice* novoRegistroIndice, char *campoIdPessoa, int64_t byteoffset){
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

