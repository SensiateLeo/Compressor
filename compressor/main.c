#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//Módulos criados para o trabalho
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

    //Abrindo o arquivo para leitura
    fp = fopen(argv[1], "rb");
    if (fp == NULL)
    {
        printf("\nCouldn't open file");
        return 1;
    }

    //Calculando tamanho do arquivo
    fseek(fp, 0, SEEK_END);

    double tamanho;
    tamanho = ftell(fp);

    fclose(fp);

    //Abrindo arquivo indicado pelo usuário
    fp = fopen(argv[1], "rb");
    if (fp == NULL)
    {
        printf("\nCouldn't open file");
        return 1;
    }

    //Leitura do File Header
    leituraHeader(fp, &bmpFileHeader);
    //Leitura do Info Header
    leituraInfo(fp, &bmpInfoHeader);

    //Verificando se o arquivo atende à todos os requisistos
    if (!(verificaArquivoBmp(bmpFileHeader, bmpInfoHeader))){
        //Se o arquivo não corresponder aos requisistos, o programa é encerrado!
        fclose(fp);
    }

    //Exibindo informações de cabeçalho para o usuário
    exibeInformacoesCabecalhos(bmpFileHeader, bmpInfoHeader);

    unsigned char*** blocos_Y;
    unsigned char*** blocos_Cb;
    unsigned char*** blocos_Cr;

    //Calculando o número de blocos 8x8 necessários para codificar a imagem
    int num_blocos = (int)(bmpInfoHeader.biWidth/8)*(int)(bmpInfoHeader.biHeight/8);

    //Declaração dos vetores que receberão os blocos após a quantização
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

    //Matrizes B, G e R correspondentes à imagem original
    B = aloca_matriz(bmpInfoHeader.biHeight,bmpInfoHeader.biWidth);
    G = aloca_matriz(bmpInfoHeader.biHeight,bmpInfoHeader.biWidth);
    R = aloca_matriz(bmpInfoHeader.biHeight,bmpInfoHeader.biWidth);

    //Lê todos os bits da imagem, e coloca nos correspondentes canais (B G e R)
    leituraBits(fp, B, G, R, bmpInfoHeader.biHeight, bmpInfoHeader.biWidth);

    fclose(fp);

    //Convertendo os canais originais para Y, Cb e Cr
    converte_YCbCr(B, G, R, B, G, R, bmpInfoHeader.biWidth, bmpInfoHeader.biHeight);

    //Após a conversão, temos o seguinte formato:
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

    //Realizando amostragem nos canais Cb e Cr
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

    //Separando as matrizes em blocos 8x8 --> num_blocos com matrizes 8x8
    blocos_Y = separa_blocos_8_x_8(B, bmpInfoHeader.biWidth, bmpInfoHeader.biHeight);
    blocos_Cb = separa_blocos_8_x_8(Cb_amostr, bmpInfoHeader.biWidth/2, bmpInfoHeader.biHeight/2);
    blocos_Cr = separa_blocos_8_x_8(Cr_amostr, bmpInfoHeader.biWidth/2, bmpInfoHeader.biHeight/2);

    //Alocando os blocos que receberão o resultado da DCT
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

    //Aplicação da DCT nos blocos
    //entrada eh unsigned char
    //blocos --> DCT --> blocos_DCT
    aplica_DCT_blocos(blocos_Y_DCT, blocos_Y, bmpInfoHeader.biWidth, bmpInfoHeader.biHeight);
    aplica_DCT_blocos(blocos_Cb_DCT, blocos_Cb, bmpInfoHeader.biWidth/2, bmpInfoHeader.biHeight/2);
    aplica_DCT_blocos(blocos_Cr_DCT, blocos_Cr, bmpInfoHeader.biWidth/2, bmpInfoHeader.biHeight/2);
    //saida eh int


    //Aplicando a quantização nos blocos, após a DCT
    //entrada eh int
    aplica_quantizacao(mat_quantizacao_luminancia, blocos_Y_DCT, bmpInfoHeader.biWidth, bmpInfoHeader.biHeight, blocos_Y_quantizados);
    aplica_quantizacao(mat_quantizacao_cromancia, blocos_Cb_DCT, bmpInfoHeader.biWidth/2, bmpInfoHeader.biHeight/2, blocos_Cb_quantizados);
    aplica_quantizacao(mat_quantizacao_cromancia, blocos_Cr_DCT, bmpInfoHeader.biWidth/2, bmpInfoHeader.biHeight/2, blocos_Cr_quantizados);
    //saida eh int

    //Alocando a matriz que receberá o bloco após o "zig zag"
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

    //Realiza o "zig zag" nos blocos quantizados
    //Dessa forma, as matrizes 8x8 são transformadas em vetores de 64 posições
    //blocos 8x8 --> vetores[64]
    zigzag(zigzag_block_Y, blocos_Y_quantizados, bmpInfoHeader.biWidth, bmpInfoHeader.biHeight);

    //Amtriz que receberá os códigos DC do canal Y
    //Os códigos recebidos tem categoria/valor
    //Matriz de num_blocos linhas por 2 colunas
    int **dc_codes_Y;
    dc_codes_Y = entropyCoding(zigzag_block_Y, bmpInfoHeader.biWidth, bmpInfoHeader.biHeight);
    //A função retorna o código DC e o tamanho total do código
    //A codificação dos coeficientes DC é feita através da codificação por diferenças

    //Estrutura que rece uma pré- codificação dos coeficientes AC
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
    //Pré-codifica os coeficientes AC presentes no vetor após o zig zag
    //Retorna um cubo de num_blocos x 63 x 3
    //cada bloco, pode conter até 63 coeficientes AC
    //Cada coeficiente tem a informação de número de zeros/categoria/valor
    pre_ac_codes_Y = pre_codifica_AC(zigzag_block_Y, bmpInfoHeader.biWidth, bmpInfoHeader.biHeight);

    int ***ac_codes_Y;
    ac_codes_Y = aloca_cubo_int(num_blocos,63,2);

    //A estrutura num_zeros/categoria/valor é passada para a codificação
    //Que codifica de acordo com a tabela pré determinada
    ac_codes_Y = codifica_AC(pre_ac_codes_Y, bmpInfoHeader.biWidth, bmpInfoHeader.biHeight);

    //A função retorna os coeficientes AC codificados
    //retorna código/valor

     //A codificação dos coeficientes AC é feita através da codificação por entropia


    //O mesmo processo descrito acima é realizado para os canais Cb e Cr
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
    //Aplicação do zig zag nos blocos
    zigzag(zigzag_block_Cb, blocos_Cb_quantizados, bmpInfoHeader.biWidth/2, bmpInfoHeader.biHeight/2);

    int **dc_codes_Cb;
    //Codificando coeficientes DC
    dc_codes_Cb = entropyCoding(zigzag_block_Cb, bmpInfoHeader.biWidth/2, bmpInfoHeader.biHeight/2);

    int ***pre_ac_codes_Cb;
    //Pr-e- codifica os ACs
    pre_ac_codes_Cb = aloca_cubo_int(num_blocos,63,3);
    pre_ac_codes_Cb = pre_codifica_AC(zigzag_block_Cb, bmpInfoHeader.biWidth/2, bmpInfoHeader.biHeight/2);

    int ***ac_codes_Cb;
    ac_codes_Cb = aloca_cubo_int(num_blocos,63,2);
    //Codifica os ACs
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
    //APlica zig zag nos blocos
    zigzag(zigzag_block_Cr, blocos_Cr_quantizados, bmpInfoHeader.biWidth/2, bmpInfoHeader.biHeight/2);

    int **dc_codes_Cr;
    //Codifica os coeficientes DC
    dc_codes_Cr = entropyCoding(zigzag_block_Cr, bmpInfoHeader.biWidth/2, bmpInfoHeader.biHeight/2);

    int ***pre_ac_codes_Cr;
    pre_ac_codes_Cr = aloca_cubo_int(num_blocos,63,3);
    //Pr-e- codifica os coeficientes AC
    pre_ac_codes_Cr = pre_codifica_AC(zigzag_block_Cr, bmpInfoHeader.biWidth/2, bmpInfoHeader.biHeight/2);

    int ***ac_codes_Cr;
    ac_codes_Cr = aloca_cubo_int(num_blocos,63,2);
    //Codifica os coeficientes AC
    ac_codes_Cr = codifica_AC(pre_ac_codes_Cr, bmpInfoHeader.biWidth/2, bmpInfoHeader.biHeight/2);

    //Criando um novo arquivo de saída, no formato binário
    //O arquivo possui o mesmo nome do arquivo de entrada
    char nome_arquivo_saida[strlen(argv[1])];
    strncpy(nome_arquivo_saida, argv[1], strlen(argv[1]));
    nome_arquivo_saida[(strlen(argv[1])-4)] = '.';
    nome_arquivo_saida[(strlen(argv[1])-3)] = 'b';
    nome_arquivo_saida[(strlen(argv[1])-2)] = 'i';
    nome_arquivo_saida[(strlen(argv[1])-1)] = 'n';
    nome_arquivo_saida[(strlen(argv[1]))] = '\0';

    //Gravando resultado da compressão
    //Cabeçalho e códigos DC e AC dos canais Y, Cb e Cr
    grava_result_compressao(bmpFileHeader, bmpInfoHeader, dc_codes_Y, ac_codes_Y, dc_codes_Cb, ac_codes_Cb, dc_codes_Cr, ac_codes_Cr, nome_arquivo_saida);

    fp = fopen(nome_arquivo_saida, "rb");
    if (fp == NULL)
    {
        printf("\nCouldn't open file");
        return 1;
    }

    //Calculando o tamanho do arquivo de saída
    fseek(fp, 0, SEEK_END);
    double tamanho_saida;
    tamanho_saida = ftell(fp);

//Calculando a taxa de compressão
    double taxa_compressao = 1 - (tamanho_saida/tamanho);


//Exibindo informações
    printf("\n\nTamanho do arquivo original: %.2f bytes\n", tamanho);
    printf("\nTamanho do arquivo de saida: %.2f bytes\n", tamanho_saida);

    printf("\nTaxa de compressao: %.2f\n\n", taxa_compressao);

    fclose(fp);

    return 0;
}
