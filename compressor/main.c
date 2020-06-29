#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//personal modules
#include "dct.h"
#include "codificacao.h"
#include "leitura_escrita.h"
#include "visualizacao.h"
#include "ycbcr.h"
#include "quantizacao.h"
#include "gerencia_memoria.h"

int main(int argc, char* argv[])
{
    FILE *fp;
    BMPFILEHEADER bmpFileHeader;
    BMPINFOHEADER bmpInfoHeader;

    fp = fopen(argv[1], "rb");
    if (fp == NULL)
    {
        printf("\nCouldn't open file");
        return 1;
    }

    fseek(fp, 0, SEEK_END);

    double tamanho;
    tamanho = ftell(fp);

    printf("\nTamanho do arquivo original: %.2f bytes\n", tamanho);

    fclose(fp);

    fp = fopen(argv[1], "rb");
    if (fp == NULL)
    {
        printf("\nCouldn't open file");
        return 1;
    }

    leituraHeader(fp, &bmpFileHeader);

    leituraInfo(fp, &bmpInfoHeader);

    if (!(verificaArquivoBmp(bmpFileHeader, bmpInfoHeader))){
        fclose(fp);
    }

    exibeInformacoesCabecalhos(bmpFileHeader, bmpInfoHeader);

    unsigned char*** blocos_Y;
    unsigned char*** blocos_Cb;
    unsigned char*** blocos_Cr;

    int num_blocos = (int)(bmpInfoHeader.biWidth/8)*(int)(bmpInfoHeader.biHeight/8);

    int*** blocos_Y_quantizados;

    blocos_Y_quantizados = (int ***) calloc (num_blocos, sizeof(int **));
    if (blocos_Y_quantizados == NULL) {
     printf ("** Erro: Memoria Insuficiente **");
    }

    for (int i = 0; i < num_blocos; i++) {
        blocos_Y_quantizados[i] = (int**) calloc (8, sizeof(int *));
        if (blocos_Y_quantizados[i] == NULL) {
            printf ("** Erro: Memoria Insuficiente **");
        }
    }

    for (int i = 0; i < num_blocos; i++) {
      for (int j = 0; j < 8; j++){
        blocos_Y_quantizados[i][j] = (int*) calloc (8, sizeof(int));
        if (blocos_Y_quantizados[i][j] == NULL) {
            printf ("** Erro: Memoria Insuficiente **");
        }
      }
    }

    int ***blocos_Cb_quantizados;

    blocos_Cb_quantizados = (int ***) calloc (num_blocos/4, sizeof(int **));
    if (blocos_Cb_quantizados == NULL) {
     printf ("** Erro: Memoria Insuficiente **");
    }

    for (int i = 0; i < num_blocos/4; i++) {
        blocos_Cb_quantizados[i] = (int**) calloc (8, sizeof(int *));
        if (blocos_Cb_quantizados[i] == NULL) {
            printf ("** Erro: Memoria Insuficiente **");
        }
    }

    for (int i = 0; i < num_blocos/4; i++) {
      for (int j = 0; j < 8; j++){
        blocos_Cb_quantizados[i][j] = (int*) calloc (8, sizeof(int));
        if (blocos_Cb_quantizados[i][j] == NULL) {
            printf ("** Erro: Memoria Insuficiente **");
        }
      }
    }

    int ***blocos_Cr_quantizados;

    blocos_Cr_quantizados = (int ***) calloc (num_blocos/4, sizeof(int **));
    if (blocos_Cr_quantizados == NULL) {
     printf ("** Erro: Memoria Insuficiente **");
    }

    for (int i = 0; i < num_blocos/4; i++) {
        blocos_Cr_quantizados[i] = (int**) calloc (8, sizeof(int *));
        if (blocos_Cr_quantizados[i] == NULL) {
            printf ("** Erro: Memoria Insuficiente **");
        }
    }

    for (int i = 0; i < num_blocos/4; i++) {
      for (int j = 0; j < 8; j++){
        blocos_Cr_quantizados[i][j] = (int*) calloc (8, sizeof(int));
        if (blocos_Cr_quantizados[i][j] == NULL) {
            printf ("** Erro: Memoria Insuficiente **");
        }
      }
    }
    //allocate enough memory for the bitmap image data
    unsigned char** B;
    unsigned char** G;
    unsigned char** R;

    B = aloca_matriz(bmpInfoHeader.biHeight,bmpInfoHeader.biWidth);
    G = aloca_matriz(bmpInfoHeader.biHeight,bmpInfoHeader.biWidth);
    R = aloca_matriz(bmpInfoHeader.biHeight,bmpInfoHeader.biWidth);

    leituraBits(fp, B, G, R, bmpInfoHeader.biHeight, bmpInfoHeader.biWidth);

    fclose(fp);

    converte_YCbCr(B, G, R, B, G, R, bmpInfoHeader.biWidth, bmpInfoHeader.biHeight);
    /*
    B --> Y
    G --> Cb
    R --> Cr
    */
    //Amostrando componentes Cb e Cr
    unsigned char** Cb_amostr;
    unsigned char** Cr_amostr;

    Cb_amostr = aloca_matriz(bmpInfoHeader.biHeight/2,bmpInfoHeader.biWidth/2);
    Cr_amostr = aloca_matriz(bmpInfoHeader.biHeight/2,bmpInfoHeader.biWidth/2);

    amostra_YCbCr_4_2_0(Cb_amostr, Cr_amostr, G, R, bmpInfoHeader.biWidth, bmpInfoHeader.biHeight);

    /*
    //recuperando matriz por expansao da amostragem
    for (int i = 0; i < bmpInfoHeader.biHeight/2; i++){
        for (int j = 0; j < bmpInfoHeader.biWidth/2; j++){
            G[2*i][2*j] = Cb_amostr[i][j];
            G[2*i][2*j+1] = Cb_amostr[i][j];
            G[2*i+1][2*j] = Cb_amostr[i][j];
            G[2*i+1][2*j+1] = Cb_amostr[i][j];

            R[2*i][2*j] = Cr_amostr[i][j];
            R[2*i][2*j+1] = Cr_amostr[i][j];
            R[2*i+1][2*j] = Cr_amostr[i][j];
            R[2*i+1][2*j+1] = Cr_amostr[i][j];
        }
    }*/

    //Separando a mariz R em blocos 8x8
    blocos_Y = separa_blocos_8_x_8(B, bmpInfoHeader.biWidth, bmpInfoHeader.biHeight);
    blocos_Cb = separa_blocos_8_x_8(Cb_amostr, bmpInfoHeader.biWidth/2, bmpInfoHeader.biHeight/2);
    blocos_Cr = separa_blocos_8_x_8(Cr_amostr, bmpInfoHeader.biWidth/2, bmpInfoHeader.biHeight/2);

    /*
    int f,g;

    for(f = 0; f < bmpInfoHeader.biHeight; f++){
        free(B[f]);
    }
    free(B);

    for(f = 0; f < bmpInfoHeader.biHeight; f++){
        free(G[f]);
    }
    free(G);

    for(f = 0; f < bmpInfoHeader.biHeight; f++){
        free(R[f]);
    }
    free(R);

    for(f = 0; f < bmpInfoHeader.biHeight/2; f++){
        free(Cb_amostr[f]);
    }
    free(Cb_amostr);

    for(f = 0; f < bmpInfoHeader.biHeight/2; f++){
        free(Cr_amostr[f]);
    }
    free(Cr_amostr); */

    int *** blocos_Y_DCT;

    blocos_Y_DCT = (int ***) calloc (num_blocos, sizeof(int **));
    if (blocos_Y_DCT == NULL) {
     printf ("** Erro: Memoria Insuficiente **");
    }

    for (int i = 0; i < num_blocos; i++) {
        blocos_Y_DCT[i] = (int**) calloc (8, sizeof(int *));
        if (blocos_Y_DCT[i] == NULL) {
            printf ("** Erro: Memoria Insuficiente **");
        }
    }

    for (int i = 0; i < num_blocos; i++) {
      for (int j = 0; j < 8; j++){
        blocos_Y_DCT[i][j] = (int*) calloc (8, sizeof(int));
        if (blocos_Y_DCT[i][j] == NULL) {
            printf ("** Erro: Memoria Insuficiente **");
        }
      }
    }

    int *** blocos_Cb_DCT;

    blocos_Cb_DCT = (int ***) calloc (num_blocos/4, sizeof(int **));
    if (blocos_Cb_DCT == NULL) {
     printf ("** Erro: Memoria Insuficiente **");
    }

    for (int i = 0; i < num_blocos/4; i++) {
        blocos_Cb_DCT[i] = (int**) calloc (8, sizeof(int *));
        if (blocos_Cb_DCT[i] == NULL) {
            printf ("** Erro: Memoria Insuficiente **");
        }
    }

    for (int i = 0; i < num_blocos/4; i++) {
      for (int j = 0; j < 8; j++){
        blocos_Cb_DCT[i][j] = (int*) calloc (8, sizeof(int));
        if (blocos_Cb_DCT[i][j] == NULL) {
            printf ("** Erro: Memoria Insuficiente **");
        }
      }
    }

    int *** blocos_Cr_DCT;

    blocos_Cr_DCT = (int ***) calloc (num_blocos/4, sizeof(int **));
    if (blocos_Cr_DCT == NULL) {
     printf ("** Erro: Memoria Insuficiente **");
    }

    for (int i = 0; i < num_blocos/4; i++) {
        blocos_Cr_DCT[i] = (int**) calloc (8, sizeof(int *));
        if (blocos_Cr_DCT[i] == NULL) {
            printf ("** Erro: Memoria Insuficiente **");
        }
    }

    for (int i = 0; i < num_blocos/4; i++) {
      for (int j = 0; j < 8; j++){
        blocos_Cr_DCT[i][j] = (int*) calloc (8, sizeof(int));
        if (blocos_Cr_DCT[i][j] == NULL) {
            printf ("** Erro: Memoria Insuficiente **");
        }
      }
    }

    //Aplica��o da DCT nos bloquinhos
    //entrada eh unsigned char
    aplica_DCT_blocos(blocos_Y_DCT, blocos_Y, bmpInfoHeader.biWidth, bmpInfoHeader.biHeight);
    aplica_DCT_blocos(blocos_Cb_DCT, blocos_Cb, bmpInfoHeader.biWidth/2, bmpInfoHeader.biHeight/2);
    aplica_DCT_blocos(blocos_Cr_DCT, blocos_Cr, bmpInfoHeader.biWidth/2, bmpInfoHeader.biHeight/2);
    //saida eh int

    /*
    for(g = 0; g < num_blocos; g++){
        for(f = 0; f < 8; f++){
            free(blocos_Y[g][f]);
        }
        free(blocos_Y[g]);
    }
    free(blocos_Y);

    for(g = 0; g < num_blocos/4; g++){
        for(f = 0; f < 8; f++){
            free(blocos_Cb[g][f]);
        }
        free(blocos_Cb[g]);
    }
    free(blocos_Cb);

    for(g = 0; g < num_blocos/4; g++){
        for(f = 0; f < 8; f++){
            free(blocos_Cr[g][f]);
        }
        free(blocos_Cr[g]);
    }
    free(blocos_Cr); */

    //entrada eh int
    aplica_quantizacao(mat_quantizacao_luminancia, blocos_Y_DCT, bmpInfoHeader.biWidth, bmpInfoHeader.biHeight, blocos_Y_quantizados);
    aplica_quantizacao(mat_quantizacao_cromancia, blocos_Cb_DCT, bmpInfoHeader.biWidth/2, bmpInfoHeader.biHeight/2, blocos_Cb_quantizados);
    aplica_quantizacao(mat_quantizacao_cromancia, blocos_Cr_DCT, bmpInfoHeader.biWidth/2, bmpInfoHeader.biHeight/2, blocos_Cr_quantizados);
    //saida eh int

    /*
    for(g = 0; g < num_blocos; g++){
        for(f = 0; f < 8; f++){
            free(blocos_Y_DCT[g][f]);
        }
        free(blocos_Y_DCT[g]);
    }
    free(blocos_Y_DCT);

    for(g = 0; g < num_blocos/4; g++){
        for(f = 0; f < 8; f++){
            free(blocos_Cb_DCT[g][f]);
        }
        free(blocos_Cb_DCT[g]);
    }
    free(blocos_Cb_DCT);

    for(g = 0; g < num_blocos/4; g++){
        for(f = 0; f < 8; f++){
            free(blocos_Cr_DCT[g][f]);
        }
        free(blocos_Cr_DCT[g]);
    }
    free(blocos_Cr_DCT); */

    double **zigzag_block_Y;
    zigzag_block_Y = malloc(num_blocos*sizeof(double *));
    if (zigzag_block_Y == NULL) {
        printf ("** Erro: Memoria Insuficiente **");
    }

    for (int i = 0; i < num_blocos; i++) {
        zigzag_block_Y[i] = (double*)malloc(64*sizeof(double));
        if (zigzag_block_Y[i] == NULL) {
            printf ("** Erro: Memoria Insuficiente **");
        }
    }
    zigzag(zigzag_block_Y, blocos_Y_quantizados, bmpInfoHeader.biWidth, bmpInfoHeader.biHeight);

    /*
    for(g = 0; g < num_blocos; g++){
        for(f = 0; f < 8; f++){
            free(blocos_Y_quantizados[g][f]);
            blocos_Y_quantizados[g][f] = NULL;
        }
        free(blocos_Y_quantizados[g]);
        blocos_Y_quantizados[g] = NULL;
    }
    free(blocos_Y_quantizados);
    blocos_Y_quantizados = NULL; */

    int **dc_codes_Y;
    dc_codes_Y = entropyCoding(zigzag_block_Y, bmpInfoHeader.biWidth, bmpInfoHeader.biHeight);

    int ***pre_ac_codes_Y;
    pre_ac_codes_Y = (int ***) calloc (num_blocos, sizeof(int **));
    if (pre_ac_codes_Y == NULL) {
     printf ("** Erro: Memoria Insuficiente **");
    }

    for (int i = 0; i < num_blocos; i++) {
        pre_ac_codes_Y [i] = (int**) calloc (8, sizeof(int *));
        if (pre_ac_codes_Y [i] == NULL) {
            printf ("** Erro: Memoria Insuficiente **");
        }
    }

    for (int i = 0; i < num_blocos; i++) {
      for (int j = 0; j < 8; j++){
        pre_ac_codes_Y [i][j] = (int*) calloc (8, sizeof(int));
        if (pre_ac_codes_Y [i][j] == NULL) {
            printf ("** Erro: Memoria Insuficiente **");
        }
      }
    }
    pre_ac_codes_Y = pre_codifica_AC(zigzag_block_Y, bmpInfoHeader.biWidth, bmpInfoHeader.biHeight);

    int ***ac_codes_Y;
    ac_codes_Y = aloca_cubo_int(num_blocos,63,2);
    ac_codes_Y = codifica_AC(pre_ac_codes_Y, bmpInfoHeader.biWidth, bmpInfoHeader.biHeight);

    double **zigzag_block_Cb;
    zigzag_block_Cb = malloc(num_blocos*sizeof(double *));
    if (zigzag_block_Cb == NULL) {
        printf ("** Erro: Memoria Insuficiente **");
    }

    for (int i = 0; i < num_blocos; i++) {
        zigzag_block_Cb[i] = (double*)malloc(64*sizeof(double));
        if (zigzag_block_Cb[i] == NULL) {
            printf ("** Erro: Memoria Insuficiente **");
        }
    }
    zigzag(zigzag_block_Cb, blocos_Cb_quantizados, bmpInfoHeader.biWidth/2, bmpInfoHeader.biHeight/2);

    /*for(g = 0; g < num_blocos/4; g++){
        for(f = 0; f < 8; f++){
            free(blocos_Cb_quantizados[g][f]);
        }
        free(blocos_Cb_quantizados[g]);
    }
    free(blocos_Cb_quantizados); */

    int **dc_codes_Cb;
    dc_codes_Cb = entropyCoding(zigzag_block_Cb, bmpInfoHeader.biWidth/2, bmpInfoHeader.biHeight/2);

    int ***pre_ac_codes_Cb;
    pre_ac_codes_Cb = aloca_cubo_int(num_blocos,63,3);
    pre_ac_codes_Cb = pre_codifica_AC(zigzag_block_Cb, bmpInfoHeader.biWidth/2, bmpInfoHeader.biHeight/2);

    int ***ac_codes_Cb;
    ac_codes_Cb = aloca_cubo_int(num_blocos,63,2);
    ac_codes_Cb = codifica_AC(pre_ac_codes_Cb, bmpInfoHeader.biWidth/2, bmpInfoHeader.biHeight/2);


    double **zigzag_block_Cr;
    zigzag_block_Cr = malloc(num_blocos*sizeof(double *));
    if (zigzag_block_Cr == NULL) {
        printf ("** Erro: Memoria Insuficiente **");
    }

    for (int i = 0; i < num_blocos; i++) {
        zigzag_block_Cr[i] = (double*)malloc(64*sizeof(double));
        if (zigzag_block_Cr[i] == NULL) {
            printf ("** Erro: Memoria Insuficiente **");
        }
    }
    zigzag(zigzag_block_Cr, blocos_Cr_quantizados, bmpInfoHeader.biWidth/2, bmpInfoHeader.biHeight/2);

   /* for(g = 0; g < num_blocos/4; g++){
        for(f = 0; f < 8; f++){
            free(blocos_Cr_quantizados[g][f]);
        }
        free(blocos_Cr_quantizados[g]);
    }
    free(blocos_Cr_quantizados); */

    int **dc_codes_Cr;
    dc_codes_Cr = entropyCoding(zigzag_block_Cr, bmpInfoHeader.biWidth/2, bmpInfoHeader.biHeight/2);

    int ***pre_ac_codes_Cr;
    pre_ac_codes_Cr = aloca_cubo_int(num_blocos,63,3);
    pre_ac_codes_Cr = pre_codifica_AC(zigzag_block_Cr, bmpInfoHeader.biWidth/2, bmpInfoHeader.biHeight/2);

    int ***ac_codes_Cr;
    ac_codes_Cr = aloca_cubo_int(num_blocos,63,2);
    ac_codes_Cr = codifica_AC(pre_ac_codes_Cr, bmpInfoHeader.biWidth/2, bmpInfoHeader.biHeight/2);

    char nome_arquivo_saida[strlen(argv[1])];
    strncpy(nome_arquivo_saida, argv[1], strlen(argv[1]));
    nome_arquivo_saida[(strlen(argv[1])-4)] = '.';
    nome_arquivo_saida[(strlen(argv[1])-3)] = 'b';
    nome_arquivo_saida[(strlen(argv[1])-2)] = 'i';
    nome_arquivo_saida[(strlen(argv[1])-1)] = 'n';
    nome_arquivo_saida[(strlen(argv[1]))] = '\0';

    grava_result_compressao(bmpFileHeader, bmpInfoHeader, dc_codes_Y, ac_codes_Y, dc_codes_Cb, ac_codes_Cb, dc_codes_Cr, ac_codes_Cr, nome_arquivo_saida);

    fp = fopen(nome_arquivo_saida, "rb");
    if (fp == NULL)
    {
        printf("\nCouldn't open file");
        return 1;
    }

    fseek(fp, 0, SEEK_END);
    double tamanho_saida;
    tamanho_saida = ftell(fp);

    double taxa_compressao = tamanho_saida/tamanho;

    printf("\nTamanho do arquivo de saida: %.2f bytes\n", tamanho_saida);

    printf("\nTaxa de compressao: %.2f\n\n", taxa_compressao);

    fclose(fp);

    return 0;
}
