// Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
// Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221

//readBinary.c tem como principal função usar e implementar funções e variáveis definidas em readBinary.h e algumas outras funções e variáveis vindas de outros arquivos
//nesse arquivo estão as funcionalidades 3 e 4, que dizem respeito a busca nos arquivos de índice e de dados

//incluindo arquivos de cabeçalho utilizados nesse arquivo de implementação
#include "../headers/readBinary.h"
#include "../headers/auxiliar.h"
#include "../headers/utilidades.h"

// Definição da variável global
struct registro2 reg;

// Funcao auxiliar para imprimir um registro
void imprimirRegistro(int idPessoa, int idadePessoa, int tamNomePessoa, char *nomePessoa, int tamNomeUsuario, char *nomeUsuario){
    printf("Dados da pessoa de codigo %d\n", idPessoa);
    
    if (tamNomePessoa > 0) {
        printf("Nome: %s\n", nomePessoa);
    } else {
        printf("Nome: -\n");
    }
    
    if (idadePessoa == -1) {
        printf("Idade: -\n");
    } else {
        printf("Idade: %d\n", idadePessoa);
    }
    
    if (tamNomeUsuario > 0) {
        printf("Usuario: %s\n\n", nomeUsuario);
    } else {
        printf("Usuario: -\n\n");
    }
}

void listarRegistros(char *nomeArquivoEntrada){
    // abrindo o caminho em que o arquivo está
    char caminho_2[100] = "./";
    strcat(caminho_2, nomeArquivoEntrada);
    FILE *arqPessoa = fopen(caminho_2, "rb");

    if (arqPessoa == NULL){
        puts("Falha no processamento do arquivo.");
        return;
    }

    // Leitura do cabeçalho do arquivo binário
    //criação de variáveis para armazenar cabeçalho
    char status;
    int quantidadePessoas;
    int quantidadeRemovidos;
    long int proxByteoffset;

    // fread(buff, sizeof, qnt, FILE)
    //lendo status
    if (fread(&status, sizeof(status), 1, arqPessoa) != 1){
        // Se o status for diferente de 1 o arquivo de dados está inconsistente
        puts("Falha no processamento do arquivo");
        fclose(arqPessoa);
        return;
    }
    //lendo quantidade de pessoas
    fread(&quantidadePessoas, sizeof(int), 1, arqPessoa);
    //lendo quantidade de pessoas removidas
    fread(&quantidadeRemovidos, sizeof(int), 1, arqPessoa);
    //lendo próximo byteoffset livre para registro
    fread(&proxByteoffset, sizeof(long int), 1, arqPessoa);

    // Tamanho do arquivo
    fseek(arqPessoa, 0, SEEK_END);
    long size = ftell(arqPessoa);
    // Ajustar inicio de arquivo
    fseek(arqPessoa, 17, SEEK_SET); // pula o cabeçalho

    // Ler arquivo
    while (ftell(arqPessoa) < size){
        char removido;
        fread(&removido, sizeof(char), 1, arqPessoa);
        int tamRegistro;
        fread(&tamRegistro, sizeof(int), 1, arqPessoa);
        if(removido == '1'){
            fseek(arqPessoa, tamRegistro-5, SEEK_CUR);
            continue;
        }
        // Lê cada campo do registro
        fread(&reg.idPessoa, sizeof(int), 1, arqPessoa);
        fread(&reg.idadePessoa, sizeof(int), 1, arqPessoa);
        fread(&reg.tamNomePessoa, sizeof(int), 1, arqPessoa);
        fread(&reg.nomePessoa, sizeof(char), reg.tamNomePessoa, arqPessoa);
        reg.nomePessoa[reg.tamNomePessoa] = '\0';
        fread(&reg.tamNomeUsuario, sizeof(int), 1, arqPessoa);
        fread(&reg.nomeUsuario, sizeof(char), reg.tamNomeUsuario, arqPessoa);
        reg.nomeUsuario[reg.tamNomeUsuario]='\0';

        // Exibe o registro formatado
        // Usa função auxiliar para imprimir
        imprimirRegistro(reg.idPessoa, reg.idadePessoa, reg.tamNomePessoa, reg.nomePessoa, reg.tamNomeUsuario, reg.nomeUsuario);
    }
    //fechando arquivo
    fclose(arqPessoa);
}


// funcionalidade 4, funcao where

//aproveitando a struct que ja foi criada em auxiliar.h
typedef struct indice indice;

//busca binaria para encontrar o byteOffset do registro com o id buscado
int64_t buscaBinariaIndice(indice *vetor, int tamanho, int idBuscado){
    int begin = 0;
    int end = tamanho - 1;

    while (begin <= end){
        int meio = (begin + end)/2;

        if(vetor[meio].idPessoa == idBuscado){
            return vetor[meio].byteOffset;
        } else if (vetor[meio].idPessoa < idBuscado){
            begin = meio + 1;
        } else{
            end = meio - 1;
        }
    }
    //se nao encontrar o indice fornecido
    return -1;
}

//imprimir registro encontrado por byteOffset
void imprimirRegistroPorByteOffset(FILE *arqPessoa, int64_t byteOffset){
    fseek(arqPessoa, byteOffset, SEEK_SET);

    char removido;
    fread(&removido, sizeof(char), 1, arqPessoa);
    if (removido == '1'){
        return; // registro marcado como removido
    }

    int tamRegistro;
    fread(&tamRegistro, sizeof(int), 1, arqPessoa);

    //lê cada campo do registro
    fread(&reg.idPessoa, sizeof(int), 1, arqPessoa);
    fread(&reg.idadePessoa, sizeof(int), 1, arqPessoa);
    fread(&reg.tamNomePessoa, sizeof(int), 1, arqPessoa);
    fread(&reg.nomePessoa, sizeof(char), reg.tamNomePessoa, arqPessoa);
    reg.nomePessoa[reg.tamNomePessoa] = '\0';
    fread(&reg.tamNomeUsuario, sizeof(int), 1, arqPessoa);
    fread(&reg.nomeUsuario, sizeof(char), reg.tamNomeUsuario, arqPessoa);
    reg.nomeUsuario[reg.tamNomeUsuario] = '\0';

    //usa função auxiliar para imprimir
    imprimirRegistro(reg.idPessoa, reg.idadePessoa, reg.tamNomePessoa, reg.nomePessoa, reg.tamNomeUsuario, reg.nomeUsuario);
}

//função principal da funcionalidade 4
void buscarregistros(char *nomeArquivoPessoa, char *nomeArquivoIndice, int n){
    // Abertura dos arquivos
    char caminho_2[100] = "./";
    strcat(caminho_2, nomeArquivoPessoa);
    FILE *arqPessoa = fopen(caminho_2, "rb");

    char caminho_3[100] = "./";
    strcat(caminho_3, nomeArquivoIndice);
    FILE *arquivoIndice = fopen(caminho_3, "rb");

    if(arqPessoa == NULL || arquivoIndice == NULL){
        puts("Falha no processamento do arquivo.");
        return;
    }

    //carrega o arquivo de índice em um vetor
    fseek(arquivoIndice, 0, SEEK_END);
    long sizeIndice = ftell(arquivoIndice) - 12; // remove os bytes do cabeçalho

    //calcula quantos registros de índice existem no arquivo
    int qtdIndice = sizeIndice / (sizeof(int) + sizeof(int64_t));

    //posiciona o ponteiro depois do cabeçalho
    fseek(arquivoIndice, 12, SEEK_SET);

    //aloca memória
    indice *vetorIndice = malloc(qtdIndice * sizeof(indice));
    
    //carrega o índice completo no vetor
    for(int i = 0; i < qtdIndice; i++){
        fread(&vetorIndice[i].idPessoa, sizeof(int), 1, arquivoIndice);
        fread(&vetorIndice[i].byteOffset, sizeof(int64_t), 1, arquivoIndice);
    }
    fclose(arquivoIndice);

    //obtem o tamanho do arquivo de dados
    fseek(arqPessoa, 0, SEEK_END);
    long sizeDados = ftell(arqPessoa);

    //loop de buscas
    for(int i = 0; i < n; i++){
        int entrada;
        char nomeCampo[100], valorCampo[100];
        int find = 0;

        // lê a linha de busca no formato: número nomeCampo=valorCampo
        scanf("%d", &entrada);
        scanf("%[^=]", nomeCampo);
        getchar(); // consome '='
        scan_quote_string(valorCampo);

        //caso 1:busca por idPessoa usando indice
        if(strcmp(nomeCampo, "idPessoa") == 0){
            int idBusca = atoi(valorCampo);
            int64_t offset = buscaBinariaIndice(vetorIndice, qtdIndice, idBusca);
            if(offset != -1){
                imprimirRegistroPorByteOffset(arqPessoa, offset);
                find = 1;
            }
        } 
        //caso 2:busca sequencial por outros campos
        else {
            fseek(arqPessoa, 17, SEEK_SET); // pula o cabeçalho

            while(ftell(arqPessoa) < sizeDados){
                char removido;
                fread(&removido, sizeof(char), 1, arqPessoa);
                
                int tamRegistro;
                fread(&tamRegistro, sizeof(int), 1, arqPessoa);
                
                //pula os registros removidos
                if(removido == '1'){
                    fseek(arqPessoa, tamRegistro - 5, SEEK_CUR);
                    continue;
                }

                //leitura dos campos do registro
                fread(&reg.idPessoa, sizeof(int), 1, arqPessoa);
                fread(&reg.idadePessoa, sizeof(int), 1, arqPessoa);
                fread(&reg.tamNomePessoa, sizeof(int), 1, arqPessoa);
                fread(reg.nomePessoa, sizeof(char), reg.tamNomePessoa, arqPessoa);
                reg.nomePessoa[reg.tamNomePessoa] = '\0';
                fread(&reg.tamNomeUsuario, sizeof(int), 1, arqPessoa);
                fread(reg.nomeUsuario, sizeof(char), reg.tamNomeUsuario, arqPessoa);
                reg.nomeUsuario[reg.tamNomeUsuario] = '\0';

                if ((strcmp(nomeCampo, "idade") == 0 && reg.idadePessoa == atoi(valorCampo)) ||
                    (strcmp(nomeCampo, "nomePessoa") == 0 && strcmp(reg.nomePessoa, valorCampo) == 0) ||
                    (strcmp(nomeCampo, "nomeUsuario") == 0 && strcmp(reg.nomeUsuario, valorCampo) == 0)) {
                    
                    imprimirRegistro(reg.idPessoa, reg.idadePessoa, reg.tamNomePessoa, reg.nomePessoa, reg.tamNomeUsuario, reg.nomeUsuario);
                    find = 1;
                }
            }
        }
    }

    free(vetorIndice);
    fclose(arqPessoa);
}