// Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
// Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221

//readBinary.c tem como principal função usar e implementar funções e variáveis definidas em readBinary.h e algumas outras funções e variáveis vindas de outros arquivos
//nesse arquivo estão as funcionalidades 3 e 4, que dizem respeito a busca nos arquivos de índice e de dados

//incluindo arquivos de cabeçalho utilizados nesse arquivo de implementação
#include "../headers/readBinary.h"
#include "../headers/auxiliar.h"

void listarRegistros(char *nomeArquivoEntrada){
    // abrindo o caminho em que o arquivo está
    char caminho_2[100] = "./bin/";
    strcat(caminho_2, nomeArquivoEntrada);
    // o arquivo pessoa precisa existir, então é aberto para append binary
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
    };
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
        //exibindo id
        printf("Dados da pessoa de codigo %d\n", reg.idPessoa);
        //exibindo nome
        if (reg.tamNomePessoa > 0){
            printf("Nome: %s\n", reg.nomePessoa);
        } else{
            printf("Nome: -\n");
        }
        //exibindo idade
        if (reg.idadePessoa == -1){
            printf("Idade: -\n");
        } else{
            printf("Idade: %d\n", reg.idadePessoa);
        }
        //exibindo nome de usuário
        if (reg.tamNomeUsuario > 0){
            printf("Usuario: %s\n\n", reg.nomeUsuario);
        } else{
            printf("Usuario: -\n\n");
        }
    }
    //fechando arquivo
    fclose(arqPessoa);
}