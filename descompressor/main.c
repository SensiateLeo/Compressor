#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//personal modules
#include "leitura_escrita.h"
#include "desquantizacao.h"
#include "ycbcr.h"


int main(int argc, char* argv[]){

    FILE *fp;
    BMPFILEHEADER bmpFileHeader;
    BMPINFOHEADER bmpInfoHeader;

    //Abrindo o arquivo para leitura
    fp = fopen(argv[1], "rb");
    if (fp == NULL)
    {
        printf("\nCouldn't open file");
        return 1;
    }

    //Definindo a tabela de prefixos (Huffman)
    //para decodificacao dos valores DC
    PREFIX prefix_table[11];
    define_PrefixTables(prefix_table);


    //Definindo a tabela de prefixos
    //para decodificacao dos valores AC
    PREFIX_AC **prefix_table_AC;

    /* aloca as linhas da matriz */
    prefix_table_AC = (PREFIX_AC **) calloc (16, sizeof(PREFIX_AC *));
    if (prefix_table_AC == NULL) {
        printf ("** Erro: Memoria Insuficiente **");
    }
    /* aloca as colunas da matriz */
    for (int i = 0; i < 16; i++ ) {
        prefix_table_AC[i] = (PREFIX_AC *) calloc (11, sizeof(PREFIX_AC));
        if (prefix_table_AC[i] == NULL) {
            printf ("** Erro: Memoria Insuficiente **");
        }
    }

    //Tabela de prefixos AC
    define_PrefixTablesAC(prefix_table_AC);

    //Lendo cabecalhos
    leituraHeader(fp, &bmpFileHeader);
    leituraInfo(fp, &bmpInfoHeader);

    //Exibindo informacoes dos cabecalhos para conferir se
    //foram recuperados corretamente
    exibeInformacoesCabecalhos(bmpFileHeader, bmpInfoHeader);

    //Buffer para leitura do arquivo compactado, byte a byte
    int num_blocos_total = (int)(bmpInfoHeader.biWidth/8)*(int)(bmpInfoHeader.biHeight/8);

    //Número de blocos dos canais Y, Cb e Cr
    int num_blocos[3];
    num_blocos[0] = num_blocos_total;
    num_blocos[1] = num_blocos_total/4;
    num_blocos[2] = num_blocos_total/4;

    //Estruturas necessárias para decodificação

    //Cubo que receberá os canais Y, Cb e Cr
    unsigned char ***canais;
    canais = (unsigned char***) calloc (3, sizeof(unsigned char **));

    if (canais == NULL) {
     printf ("** Erro: Memoria Insuficiente **");
    }

    for (int i = 0; i < 3; i++) {
        canais[i] = (unsigned char**) calloc (bmpInfoHeader.biHeight, sizeof(unsigned char*));
        if (canais[i] == NULL) {
            printf ("** Erro: Memoria Insuficiente **");
        }
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < bmpInfoHeader.biHeight; j++) {
            canais[i][j] = (unsigned char*) calloc (bmpInfoHeader.biWidth, sizeof(unsigned char));
            if (canais[i][j] == NULL) {
                printf ("** Erro: Memoria Insuficiente **");
            }
        }
    }

    //Matriz de blocos, que receberá os blocos de 64 posições
    //Correspondentes aos blocos codificados
    int **blocos;
    blocos = calloc(num_blocos[0], sizeof(int *));
    if (blocos == NULL) {
        printf ("** Erro: Memoria Insuficiente **");
    }
    printf("\nPrimeira alocada!");
    for (int i = 0; i < num_blocos[0]; i++) {
        blocos[i] = (int*)calloc(64,sizeof(int));
        if (blocos[i] == NULL) {
            printf ("** Erro: Memoria Insuficiente **");
        }
    }

    //Estrutura que receberá os blocos após a desquantização
    int*** blocos_DCT;
    blocos_DCT = (int***) calloc (num_blocos[0], sizeof(int **));
    if (blocos_DCT == NULL) {
     printf ("** Erro: Memoria Insuficiente **");
    }

    for (int i = 0; i < num_blocos[0]; i++) {
        blocos_DCT[i] = (int**) calloc (8, sizeof(int *));
        if (blocos_DCT[i] == NULL) {
            printf ("** Erro: Memoria Insuficiente **");
        }
    }

    for (int i = 0; i < num_blocos[0]; i++) {
      for (int j = 0; j < 8; j++){
        blocos_DCT[i][j] = (int*) calloc (8, sizeof(int));
        if (blocos_DCT[i][j] == NULL) {
            printf ("** Erro: Memoria Insuficiente **");
        }
      }
    }

    //Estrutura que receberá os blocos após a IDCT
    unsigned char ***blocos_IDCT;
    blocos_IDCT = (unsigned char***) calloc (num_blocos[0], sizeof(unsigned char **));
    if (blocos_IDCT == NULL) {
     printf ("** Erro: Memoria Insuficiente **");
    }

    for (int i = 0; i < num_blocos[0]; i++) {
        blocos_IDCT[i] = (unsigned char**) calloc (8, sizeof(unsigned char *));
        if (blocos_IDCT[i] == NULL) {
            printf ("** Erro: Memoria Insuficiente **");
        }
    }

    for (int i = 0; i < num_blocos[0]; i++) {
      for (int j = 0; j < 8; j++){
        blocos_IDCT[i][j] = (unsigned char*) calloc (8, sizeof(unsigned char));
        if (blocos_IDCT[i][j] == NULL) {
            printf ("** Erro: Memoria Insuficiente **");
        }
      }
    }

    //Sub canal criado para a expansão dos canais amostrados Cb e Cr
    unsigned char *** sub_canal;
    sub_canal = (unsigned char***) calloc (2, sizeof(unsigned char **));
    if (sub_canal == NULL) {
     printf ("** Erro: Memoria Insuficiente **");
    }

    for (int i = 0; i < 2; i++) {
        sub_canal[i] = (unsigned char**) calloc (bmpInfoHeader.biHeight/2, sizeof(unsigned char *));
        if (sub_canal[i] == NULL) {
            printf ("** Erro: Memoria Insuficiente **");
        }
    }

    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < bmpInfoHeader.biHeight/2; j++){
        sub_canal[i][j] = (unsigned char*) calloc (bmpInfoHeader.biWidth/2, sizeof(unsigned char));
        if (sub_canal[i][j] == NULL) {
            printf ("** Erro: Memoria Insuficiente **");
        }
      }
    }

    //Para os três canais, é feito:
    for (int a = 0; a < 3; a++){

        //Recupera um canal de num_blocos do arquivo binario
        recupera_canal(fp, prefix_table, prefix_table_AC, num_blocos[a], blocos);

        //Desfaz o zigzag dos blocos
        //vetores de 64 posições --> blocos 8x8
        desfaz_zigzag(blocos,num_blocos[a],blocos_DCT);

        //Desquantizacao ok
        if(a == 0){
            //Se o canal é Y, desquantiza com a matriz de luminância
            aplica_desquantizacao(mat_quantizacao_luminancia, blocos_DCT, num_blocos[a], blocos_DCT);
        }
        else{
            //Se são os canais Cb e Cr, desquantiza com a matriz de cromância
            aplica_desquantizacao(mat_quantizacao_cromancia, blocos_DCT, num_blocos[a], blocos_DCT);
        }

        //Aplica a DCT Inversa nos blocos 8x8
        aplica_IDCT_blocos(blocos_IDCT, blocos_DCT, num_blocos[a]);

        if (a > 0){
            //Se são os canais Cb e Cr:

            //Une os blocos 8x8 em uma matriz
            une_bloco_matriz(sub_canal[a-1], blocos_IDCT, bmpInfoHeader.biHeight/2,bmpInfoHeader.biWidth/2);
            //Expande o sub_canal para uma matriz do tamanho da imagem original
            amplia_canal(canais[a], sub_canal[a-1], bmpInfoHeader.biHeight,bmpInfoHeader.biWidth);

        }
        else{
            //Une os blocos em uma matriz, do tamanho da imagem original
            une_bloco_matriz(canais[a], blocos_IDCT, bmpInfoHeader.biHeight,bmpInfoHeader.biWidth);
        }

    }

    //Converte os canais Y, Cb e Cr para B, G e R novamente
    converte_RGB(canais[0],canais[1],canais[2],canais[0],canais[1],canais[2],bmpInfoHeader.biWidth,bmpInfoHeader.biHeight);

    //Criando um novo arquivo de saída, no formato binário
    //O arquivo possui o mesmo nome do arquivo de entrada
    char nome_arquivo_saida[strlen(argv[1])];
    strncpy(nome_arquivo_saida, argv[1], strlen(argv[1]));
    nome_arquivo_saida[(strlen(argv[1])-4)] = '.';
    nome_arquivo_saida[(strlen(argv[1])-3)] = 'b';
    nome_arquivo_saida[(strlen(argv[1])-2)] = 'm';
    nome_arquivo_saida[(strlen(argv[1])-1)] = 'p';
    nome_arquivo_saida[(strlen(argv[1]))] = '\0';

    //Grava esses canais, junto ao cabeçalho no arquivo de saída
    //Com isso, recupera- se a imagem original
    gravaArquivoBmp(bmpFileHeader,bmpInfoHeader, canais[0],canais[1],canais[2], nome_arquivo_saida);

    printf("Arquivo recuperado com sucesso!");

    fclose(fp);
    
    //free() nos canais recuperados
    for (int b=0; b < 3; b++){
        for (int c=0; c < bmpInfoHeader.biHeight; c++){
            free(canais[b][c]);
            canais[b][c] = NULL;
        }
        free(canais[b]);
        canais[b] = NULL;
    }
    free(canais);
    canais = NULL;

    printf("\n\n========================\nArquivo descomprimido com sucesso!\n========================\n");


    return 0;
}
