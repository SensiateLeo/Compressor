#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "leitura_escrita.h"

//Leitura de Bitmap (exemplo FileHeader)
void leituraHeader(FILE *F, BMPFILEHEADER *H){
// F é o arquivo Bitmap que deve ter sido “lido” do disco
    fread(&H->bfType, sizeof(unsigned short),1,F);
    fread(&H->bfSize, sizeof(unsigned int),1,F);
    fread(&H->bfReserved1, sizeof(unsigned short),1,F);
    fread(&H->bfReserved2, sizeof(unsigned short),1,F);
    fread(&H->bfOffBits, sizeof(unsigned int),1,F);
    printf("\nFileHeader read done!");
}

void leituraInfo(FILE *F, BMPINFOHEADER *H){
// F é o arquivo Bitmap que deve ter sido “lido” do disco
    fread(&H->biSize, sizeof(unsigned int), 1, F);
    fread(&H->biWidth, sizeof(int), 1, F); // Width of image
    fread(&H->biHeight, sizeof(int),1,F); // Height of image
    fread(&H->biPlanes, sizeof(unsigned short),1,F); // Number of color planes
    fread(&H->biBitCount, sizeof(unsigned short),1,F); // Number of bits per pixel
    fread(&H->biCompression, sizeof(unsigned int), 1, F); // Type of compression to use
    fread(&H->biSizeImage, sizeof(unsigned int), 1, F);
    fread(&H->biXPelsPerMeter, sizeof(int), 1, F);
    fread(&H->biYPelsPerMeter, sizeof(int), 1, F);
    fread(&H->biClrUsed, sizeof(unsigned int), 1, F);
    fread(&H->biClrImportant, sizeof(unsigned int), 1, F);
    printf("\nInfoHeader read done!");
}

void exibeInformacoesCabecalhos(BMPFILEHEADER FH, BMPINFOHEADER IH){
    printf("\n\n===========================\n");
    printf("BMP File Header");
    printf("\n===========================");
    printf("\nbfType: %s", (char *)&FH.bfType);
    printf("\nbfSize: %d", FH.bfSize);
    printf("\nbfReserved1: %d", FH.bfReserved1);
    printf("\nbfReserved2: %d", FH.bfReserved2);
    printf("\nbfOffBits: %d\n", FH.bfOffBits);

    printf("\n\n===========================\n");
    printf("BMP Info Header");
    printf("\n===========================");
    printf("\nbiSize: %d", IH.biSize);
    printf("\nbiWidth: %d", IH.biWidth);
    printf("\nbiHeight: %d", IH.biHeight);
    printf("\nbiPlanes: %d", IH.biPlanes);
    printf("\nbiBitCount: %d", IH.biBitCount);
    printf("\nbiCompression: %d", IH.biCompression);
    printf("\nbiSizeImage: %d", IH.biSizeImage);
    printf("\nbiXPelsPerMeter: %d", IH.biXPelsPerMeter);
    printf("\nbiYPelsPerMeter: %d", IH.biYPelsPerMeter);
    printf("\nbiClrUsed: %d", IH.biClrUsed);
    printf("\nbiClrImportant: %d", IH.biClrImportant);
}

void leituraBits(FILE *F, unsigned char *B, int num_blocos){
    int i;
    for (i=0; i<num_blocos ;i++){
        fread(&B[i], sizeof(unsigned char), 1, F);
    }
}

void converte_string_binario(int n, int tamanho, char* vetor_aux, char *vetor){
    int r, i;

    // Utiliza um número de 32 bits como base para a conversão.
    for(int aux_i = 31; aux_i >= 0; aux_i--) {
        // Executa a operação shift right até a
        // última posição da direita para cada bit.
        r = n >> aux_i;

        // Por meio do "e" lógico ele compara se o valor
        // na posição mais à direita é 1 ou 0
        // e imprime na tela até reproduzir o número binário.
        if(r & 1) {
            vetor[aux_i] = '1';
            //printf("1");
        } else {
            vetor[aux_i] = '0';
            //printf("0");
        }

     }

     int aux = 0;
     for(i = (tamanho-1); i >= 0; i--){
        vetor_aux[aux] = vetor[i];
        aux += 1;
     }
     vetor_aux[tamanho] = '\0';
}

void gravaArquivoBmp(BMPFILEHEADER FH, BMPINFOHEADER IH, unsigned char **B,unsigned char **G,unsigned char **R, char* nome_imagem){
    FILE *F;
    F = fopen(nome_imagem, "wb");
    if (F == NULL)
    {
        fprintf(stderr, "\nErro na abertura do arquivo.\n");
        exit (1);
    }

    fwrite(&FH.bfType, sizeof(FH.bfType), 1, F);
    fwrite(&FH.bfSize, sizeof(FH.bfSize), 1, F);
    fwrite(&FH.bfReserved1, sizeof(FH.bfReserved1), 1, F);
    fwrite(&FH.bfReserved2, sizeof(FH.bfReserved2), 1, F);
    fwrite(&FH.bfOffBits, sizeof(FH.bfOffBits), 1, F);
    //fwrite(&bmpFileHeader, sizeof(BMPINFOHEADER), 1, outfile);
    fwrite(&IH.biSize, sizeof(IH.biSize), 1, F);
    fwrite(&IH.biWidth, sizeof(IH.biWidth), 1, F);
    fwrite(&IH.biHeight, sizeof(IH.biHeight), 1, F);
    fwrite(&IH.biPlanes, sizeof(IH.biPlanes), 1, F);
    fwrite(&IH.biBitCount, sizeof(IH.biBitCount), 1, F);
    fwrite(&IH.biCompression, sizeof(IH.biCompression), 1, F);
    fwrite(&IH.biSizeImage, sizeof(IH.biSizeImage), 1, F);
    fwrite(&IH.biXPelsPerMeter, sizeof(IH.biXPelsPerMeter), 1, F);
    fwrite(&IH.biYPelsPerMeter, sizeof(IH.biYPelsPerMeter), 1, F);
    fwrite(&IH.biClrUsed, sizeof(IH.biClrUsed), 1, F);
    fwrite(&IH.biClrImportant, sizeof(IH.biClrImportant), 1, F);

    int i , j;


    for (i=0; i<IH.biHeight ;i++){
        for(j=0;j<IH.biWidth;j++){
            fwrite(&B[i][j], sizeof(B[i][j]), 1, F);
            fwrite(&G[i][j], sizeof(G[i][j]), 1, F);
            fwrite(&R[i][j], sizeof(R[i][j]), 1, F);
        }
    }

    if(fwrite != 0)
        printf("\n\n%s escrito com sucesso!!\n", nome_imagem);
    else
        printf("\nErro ao escrever arquivo %s!\n", nome_imagem);

    fclose(F);
}

void recupera_canal(FILE *fp, PREFIX prefix_table[11], PREFIX_AC **prefix_table_AC, int num_blocos, int **blocos){

    printf("Entro recupera!");
    //Buffer para leitura do arquivo compactado, byte a byte
    unsigned char buffer = 0;

    char string_comparacao[16];
    string_comparacao[0] = '\0';
    char string_comparacao_aux[16];
    string_comparacao_aux[0] = '\0';
    char string_valor_DC[16];
    char string_valor_DC_aux[16];
    char string_valor_AC[16];
    char string_valor_AC_aux[16];

    char *vetor_binario;
    vetor_binario = (char *) malloc(8*sizeof(char));

    int tam = 8;
    int categoria;
    int num_zeros;
    int resto = 0;
    int EOB = 1;
    int tamanho;

    int leu_prefixo_DC = 0;
    int resto_valor_DC = 0;
    int resto_prefixo_DC = 0;

    int cont_DC = 0;
    int valor_DC;

    int leu_prefixo_AC = 0;
    int resto_valor_AC = 0;
    int resto_prefixo_AC = 0;

    int cont_AC = 0;
    int valor_AC;

    int resto_AC_bloco = 0;
    int AC_modificado = 0;
    int DC_modificado = 0;

    char *vetor;
    vetor = (char *) calloc (32, sizeof(char));

    //Para isso, armazenamos primeiro os DCS que est�o codificados:
    int *valores_DC;
    valores_DC = (int *) malloc(num_blocos*sizeof(int));

    int **valores_AC;
    valores_AC = malloc(num_blocos*sizeof(int *));
    if (valores_AC == NULL) {
        printf ("** Erro: Memoria Insuficiente **");
    }

    for (int i = 0; i < num_blocos; i++) {
        valores_AC[i] = (int*)malloc(63*sizeof(int));
        if (valores_AC[i] == NULL) {
            printf ("** Erro: Memoria Insuficiente **");
        }
    }

    int blocos_lidos = 0;
    int ext_zero = 0;

    while (blocos_lidos < num_blocos){

        if (EOB == 1){
            EOB = 0;
            do{
                if (resto == 0){
                    fread(&buffer, sizeof(unsigned char), 1, fp);
                    converte_string_binario(buffer, tam, vetor_binario, vetor);
                    vetor[0] = '\0';
                }

                if (resto > 0){
                    if (resto == 1){
                        strncpy(string_comparacao, vetor_binario+(8-resto), resto);
                        string_comparacao[resto] = '\0';

                        if(leu_prefixo_DC == 1){
                            resto_valor_DC = 1;
                        }
                        else{
                            resto_prefixo_DC = 1;
                        }
                        resto = 0;
                    }
                    else{
                        for (int i = 2; i <= resto; i++){
                            strncpy(string_comparacao, vetor_binario+(8-resto), i);
                            string_comparacao[i] = '\0';

                            for(int j = 0; j < 11; j++){
                                if(strcmp(string_comparacao,prefix_table[j].prefix_bits) == 0){
                                    categoria = j;
                                    leu_prefixo_DC = 1;
                                    resto_prefixo_DC = 0;

                                    resto = resto-i;
                                    break;
                                }
                            }
                            if(leu_prefixo_DC == 1){
                                resto_prefixo_DC = 0;
                                if ((categoria) <= resto){
                                    if(categoria > 0){
                                        strncpy(string_valor_DC, vetor_binario+(8-resto), categoria);
                                        string_valor_DC[categoria] = '\0';
                                    }
                                    else{
                                        string_valor_DC[0] = '0';
                                        string_valor_DC[1] = '\0';
                                    }
                                    resto = resto-categoria;
                                    resto_valor_DC = 0;
                                    leu_prefixo_DC = 0;
                                }
                                else{
                                    strncpy(string_valor_DC, vetor_binario+(8-resto), resto);
                                    string_valor_DC[resto] = '\0';

                                    resto_valor_DC = categoria - resto;
                                    resto = 0;
                                }
                                break;
                            }
                            else{
                                resto_prefixo_DC = 1;
                            }
                        }
                        if(resto_prefixo_DC == 1){
                            resto = 0;
                        }
                    }
                }
                else{
                    if (resto_valor_DC > 0){
                        if (resto_valor_DC <= 8){
                            strncpy(string_valor_DC_aux, vetor_binario, resto_valor_DC);
                            string_valor_DC_aux[resto_valor_DC] = '\0';

                            strcat(string_valor_DC, string_valor_DC_aux);

                            resto = 8-resto_valor_DC;
                            resto_valor_DC = 0;
                            leu_prefixo_DC = 0;
                        }
                        else{
                            strncpy(string_valor_DC_aux, vetor_binario, 8);
                            string_valor_DC_aux[resto] = '\0';

                            strcat(string_valor_DC, string_valor_DC_aux);
                            resto = 0;

                            resto_valor_DC = resto_valor_DC-8;
                        }
                    }
                    else{
                        if (resto_prefixo_DC == 1){
                            for(int i = 0; i < 8; i++){
                                strncpy(string_comparacao_aux, vetor_binario+i, 1);
                                string_comparacao_aux[1] = '\0';

                                strcat(string_comparacao, string_comparacao_aux);

                                for(int j = 0; j < 11; j++){
                                    if(strcmp(string_comparacao,prefix_table[j].prefix_bits) == 0){
                                        categoria = j;
                                        leu_prefixo_DC = 1;
                                        resto_prefixo_DC = 0;

                                        break;
                                    }
                                }
                                if(leu_prefixo_DC == 1){
                                    resto_prefixo_DC = 0;
                                    if (((i+1)+categoria) <= 8){
                                        if(categoria > 0){
                                            strncpy(string_valor_DC, vetor_binario+(i+1), categoria);
                                            string_valor_DC[categoria] = '\0';
                                            resto = 8 - ((i+1)+categoria);
                                        }
                                        else{
                                            string_valor_DC[0] = '0';
                                            string_valor_DC[1] = '\0';
                                            resto = 8-(i+1);
                                        }

                                        resto_valor_DC = 0;
                                        leu_prefixo_DC = 0;
                                    }
                                    else{
                                        strncpy(string_valor_DC, vetor_binario+(i+1), (8-(i+1)));
                                        string_valor_DC[8-(i+1)] = '\0';

                                        resto_valor_DC = ((i+1)+categoria) - 8;
                                        resto =0;
                                    }
                                    break;
                                }
                                else{
                                    resto_prefixo_DC = 1;
                                }
                            }

                        }
                        else{

                            for(int i = 0; i < 8; i ++){
                                if(i == 0){
                                    strncpy(string_comparacao, vetor_binario, 1);
                                    string_comparacao[1] = '\0';
                                }
                                else{
                                    strncpy(string_comparacao_aux, vetor_binario+i, 1);
                                    string_comparacao_aux[1] = '\0';

                                    strcat(string_comparacao, string_comparacao_aux);
                                }

                                if (strlen(string_comparacao) >= 2){
                                    for(int j = 0; j < 11; j++){
                                        if(strcmp(string_comparacao,prefix_table[j].prefix_bits) == 0){

                                            categoria = j;
                                            leu_prefixo_DC = 1;
                                            resto_prefixo_DC = 0;

                                            break;
                                        }
                                    }
                                    if(leu_prefixo_DC == 1){
                                        if (((i+1)+categoria) <= 8){
                                            if(categoria > 0){
                                                strncpy(string_valor_DC, vetor_binario+(i+1), categoria);
                                                string_valor_DC[categoria] = '\0';
                                                resto = 8 - ((i+1)+categoria);
                                            }
                                            else{
                                                string_valor_DC[0] = '0';
                                                string_valor_DC[1] = '\0';
                                                resto = 5;
                                            }

                                            resto_valor_DC = 0;
                                            leu_prefixo_DC = 0;
                                        }
                                        else{
                                            strncpy(string_valor_DC, vetor_binario+(i+1), (8-(i+1)));
                                            string_valor_DC[(8-(i+1))] = '\0';

                                            resto_valor_DC = ((i+1)+categoria) - 8;
                                        }
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }while((resto_prefixo_DC != 0) || (resto_valor_DC != 0));

            valor_DC = 0;
            tamanho = strlen(string_valor_DC);

            for (int k = 0; k < tamanho; k++){
                if(string_valor_DC[k] == '1')
                    valor_DC += pow(2,((tamanho-1)-k));
            }

            if (categoria == 1){
                if (valor_DC < 1){
                    valor_DC = -1;
                }
            }
            else if (categoria == 2){
                if (valor_DC < 2){
                    for (int aux_compl=0; aux_compl < strlen(string_valor_DC); aux_compl++){
                        if (string_valor_DC[aux_compl] == '1'){
                            string_valor_DC[aux_compl] = '0';
                        }else{
                            string_valor_DC[aux_compl] = '1';
                        }
                    }
                    DC_modificado = 1;
                }
            }else if (categoria == 3){
                if (valor_DC < 4){
                    for (int aux_compl=0; aux_compl < strlen(string_valor_DC); aux_compl++){
                        if (string_valor_DC[aux_compl] == '1'){
                            string_valor_DC[aux_compl] = '0';
                        }else{
                            string_valor_DC[aux_compl] = '1';
                        }
                    }
                    DC_modificado = 1;
                }
            }else if (categoria == 4){
                if (valor_DC < 8){
                    for (int aux_compl=0; aux_compl < strlen(string_valor_DC); aux_compl++){
                        if (string_valor_DC[aux_compl] == '1'){
                            string_valor_DC[aux_compl] = '0';
                        }else{
                            string_valor_DC[aux_compl] = '1';
                        }
                    }
                    DC_modificado = 1;
                }
            }else if (categoria == 5){
                if (valor_DC < 16){
                    for (int aux_compl=0; aux_compl < strlen(string_valor_DC); aux_compl++){
                        if (string_valor_DC[aux_compl] == '1'){
                            string_valor_DC[aux_compl] = '0';
                        }else{
                            string_valor_DC[aux_compl] = '1';
                        }
                    }
                    DC_modificado = 1;
                }
            }else if (categoria == 6){
                if (valor_DC < 32){
                    for (int aux_compl=0; aux_compl < strlen(string_valor_DC); aux_compl++){
                        if (string_valor_DC[aux_compl] == '1'){
                            string_valor_DC[aux_compl] = '0';
                        }else{
                            string_valor_DC[aux_compl] = '1';
                        }
                    }
                    DC_modificado = 1;
                }
            }else if (categoria == 7){
                if (valor_DC < 64){
                    for (int aux_compl=0; aux_compl < strlen(string_valor_DC); aux_compl++){
                        if (string_valor_DC[aux_compl] == '1'){
                            string_valor_DC[aux_compl] = '0';
                        }else{
                            string_valor_DC[aux_compl] = '1';
                        }
                    }
                    DC_modificado = 1;
                }
            }else if (categoria == 8){
                if (valor_DC < 128){
                    for (int aux_compl=0; aux_compl < strlen(string_valor_DC); aux_compl++){
                        if (string_valor_DC[aux_compl] == '1'){
                            string_valor_DC[aux_compl] = '0';
                        }else{
                            string_valor_DC[aux_compl] = '1';
                        }
                    }
                    DC_modificado = 1;
                }
            }else if (categoria == 9){
                if (valor_DC < 256){
                    for (int aux_compl=0; aux_compl < strlen(string_valor_DC); aux_compl++){
                        if (string_valor_DC[aux_compl] == '1'){
                            string_valor_DC[aux_compl] = '0';
                        }else{
                            string_valor_DC[aux_compl] = '1';
                        }
                    }
                    DC_modificado = 1;
                }
            }else if (categoria == 10){
                if (valor_DC < 512){
                    for (int aux_compl=0; aux_compl < strlen(string_valor_DC); aux_compl++){
                        if (string_valor_DC[aux_compl] == '1'){
                            string_valor_DC[aux_compl] = '0';
                        }else{
                            string_valor_DC[aux_compl] = '1';
                        }
                    }
                    DC_modificado = 1;
                }
            }else if (categoria == 11){
                if (valor_DC < 1024){
                    for (int aux_compl=0; aux_compl < strlen(string_valor_DC); aux_compl++){
                        if (string_valor_DC[aux_compl] == '1'){
                            string_valor_DC[aux_compl] = '0';
                        }else{
                            string_valor_DC[aux_compl] = '1';
                        }
                    }
                    DC_modificado = 1;
                }
            }
            if (DC_modificado == 1){
                valor_DC = 0;
                for (int k = 0; k < tamanho; k++){
                    if(string_valor_DC[k] == '1')
                        valor_DC += pow(2,((tamanho-1)-k));
                }
                DC_modificado = 0;
                valor_DC = (-1)*valor_DC;
            }

            valores_DC[cont_DC] = valor_DC;
            cont_DC++;
        }

        //Aqui comecariam as leitutas dos ACS
        //Lembrando que os ACS devem ser armazenados na matriz "blocos"
        //Existem, no pior caso, 63 ACS

        //Quando encontrar um c�digo AC 0/0, preenche as demais linhas de "blocos" com 0
        // e come�a o pr�ximo bloco
        do{
            if (resto == 0){
                fread(&buffer, sizeof(unsigned char), 1, fp);
                converte_string_binario(buffer, tam, vetor_binario, vetor);
                vetor[0] = '\0';
            }

            if (resto > 0){
                if (resto == 1){
                    strncpy(string_comparacao, vetor_binario+(8-resto), resto);
                    string_comparacao[resto] = '\0';

                    if(leu_prefixo_AC == 1){
                        resto_valor_AC = 1;
                    }
                    else{
                        resto_prefixo_AC = 1;
                    }
                    resto = 0;
                }
                else{
                    for (int i = 2; i <= resto; i++){
                        strncpy(string_comparacao, vetor_binario+(8-resto), i);
                        string_comparacao[i] = '\0';
                        //Recuperando numero de zeros e categoria
                        if(strlen(string_comparacao) == 2){
                            for(int j = 0; j < 2; j++){
                                if(strcmp(string_comparacao,prefix_table_AC[coef_AC_2[j][0]][coef_AC_2[j][1]].prefix_bits) == 0){

                                    num_zeros = coef_AC_2[j][0];
                                    categoria = coef_AC_2[j][1];
                                    leu_prefixo_AC = 1;
                                    resto_prefixo_AC = 0;
                                    break;
                                }
                            }
                        }
                        else if(strlen(string_comparacao) == 3){
                            if(strcmp(string_comparacao,prefix_table_AC[coef_AC_3[0][0]][coef_AC_3[0][1]].prefix_bits) == 0){

                                num_zeros = coef_AC_3[0][0];
                                categoria = coef_AC_3[0][1];
                                leu_prefixo_AC = 1;
                                resto_prefixo_AC = 0;
                            }
                        }
                        else if(strlen(string_comparacao) == 4){
                            for(int j = 0; j < 3; j++){
                                if(strcmp(string_comparacao,prefix_table_AC[coef_AC_4[j][0]][coef_AC_4[j][1]].prefix_bits) == 0){

                                    num_zeros = coef_AC_4[j][0];
                                    categoria = coef_AC_4[j][1];
                                    leu_prefixo_AC = 1;
                                    resto_prefixo_AC = 0;

                                    //Lembrar que se j == 0 --> fim da linha
                                    if(j == 0){
                                        EOB = 1; //preenche as linhas do AC e parte para pr�ximo DC
                                    }
                                    break;
                                }
                            }
                        }
                        else if(strlen(string_comparacao) == 5){
                            for(int j = 0; j < 2; j++){
                                if(strcmp(string_comparacao,prefix_table_AC[coef_AC_5[j][0]][coef_AC_5[j][1]].prefix_bits) == 0){

                                    num_zeros = coef_AC_5[j][0];
                                    categoria = coef_AC_5[j][1];
                                    leu_prefixo_AC = 1;
                                    resto_prefixo_AC = 0;
                                    break;

                                }
                            }
                        }
                        else if(strlen(string_comparacao) == 6){
                            for(int j = 0; j < 4; j++){
                                if(strcmp(string_comparacao,prefix_table_AC[coef_AC_6[j][0]][coef_AC_6[j][1]].prefix_bits) == 0){

                                    num_zeros = coef_AC_6[j][0];
                                    categoria = coef_AC_6[j][1];
                                    leu_prefixo_AC = 1;
                                    resto_prefixo_AC = 0;
                                    break;

                                }
                            }
                        }
                        else if(strlen(string_comparacao) == 7){
                            for(int j = 0; j < 4; j++){
                                if(strcmp(string_comparacao,prefix_table_AC[coef_AC_7[j][0]][coef_AC_7[j][1]].prefix_bits) == 0){

                                    num_zeros = coef_AC_7[j][0];
                                    categoria = coef_AC_7[j][1];
                                    leu_prefixo_AC = 1;
                                    resto_prefixo_AC = 0;
                                    break;

                                }
                            }
                        }
                        else if(strlen(string_comparacao) == 8){
                            for(int j = 0; j < 3; j++){
                                if(strcmp(string_comparacao,prefix_table_AC[coef_AC_8[j][0]][coef_AC_8[j][1]].prefix_bits) == 0){

                                    num_zeros = coef_AC_8[j][0];
                                    categoria = coef_AC_8[j][1];
                                    leu_prefixo_AC = 1;
                                    resto_prefixo_AC = 0;
                                    break;

                                }
                            }
                        }
                        else if(strlen(string_comparacao) == 9){
                            for(int j = 0; j < 5; j++){
                                if(strcmp(string_comparacao,prefix_table_AC[coef_AC_9[j][0]][coef_AC_9[j][1]].prefix_bits) == 0){

                                    num_zeros = coef_AC_9[j][0];
                                    categoria = coef_AC_9[j][1];
                                    leu_prefixo_AC = 1;
                                    resto_prefixo_AC = 0;
                                    break;

                                }
                            }
                        }
                        else if(strlen(string_comparacao) == 10){
                            for(int j = 0; j < 5; j++){
                                if(strcmp(string_comparacao,prefix_table_AC[coef_AC_10[j][0]][coef_AC_10[j][1]].prefix_bits) == 0){

                                    num_zeros = coef_AC_10[j][0];
                                    categoria = coef_AC_10[j][1];
                                    leu_prefixo_AC = 1;
                                    resto_prefixo_AC = 0;
                                    break;

                                }
                            }
                        }
                        else if(strlen(string_comparacao) == 11){
                            for(int j = 0; j < 5; j++){
                                if(strcmp(string_comparacao,prefix_table_AC[coef_AC_11[j][0]][coef_AC_11[j][1]].prefix_bits) == 0){

                                    num_zeros = coef_AC_11[j][0];
                                    categoria = coef_AC_11[j][1];
                                    leu_prefixo_AC = 1;
                                    resto_prefixo_AC = 0;
                                    break;

                                }
                            }
                        }
                        else if(strlen(string_comparacao) == 12){
                            for(int j = 0; j < 2; j++){
                                if(strcmp(string_comparacao,prefix_table_AC[coef_AC_12[j][0]][coef_AC_12[j][1]].prefix_bits) == 0){

                                    num_zeros = coef_AC_12[j][0];
                                    categoria = coef_AC_12[j][1];
                                    leu_prefixo_AC = 1;
                                    resto_prefixo_AC = 0;

                                    if(j == 1){
                                        num_zeros += 1;
                                        ext_zero = 1;
                                    }
                                    break;

                                }
                            }
                        }
                        else if((strlen(string_comparacao) >= 15)){
                            for(int j = 0; j < 126; j++){
                                if(strcmp(string_comparacao,prefix_table_AC[coef_AC_15[j][0]][coef_AC_15[j][1]].prefix_bits) == 0){

                                    num_zeros = coef_AC_15[j][0];
                                    categoria = coef_AC_15[j][1];
                                    leu_prefixo_AC = 1;
                                    resto_prefixo_AC = 0;
                                    break;

                                }
                            }
                        }
                        if(EOB == 0){
                            if(ext_zero == 1){
                                resto_prefixo_AC = 0;
                                resto_valor_AC = 0;
                                leu_prefixo_AC = 0;
                                resto = resto - i;
                                string_valor_AC[0] = '0';
                                string_valor_AC[1] = '\0';
                                break;
                            }
                            else if(leu_prefixo_AC == 1){
                                resto_prefixo_AC = 0;
                                resto = resto - i;
                                if ((categoria) <= resto){
                                    if(categoria > 0){
                                        strncpy(string_valor_AC, vetor_binario+(8-resto), categoria);
                                        string_valor_AC[categoria] = '\0';

                                    }
                                    else{
                                        string_valor_AC[0] = '0';
                                        string_valor_AC[1] = '\0';
                                    }
                                    resto = resto-(categoria);
                                    resto_valor_AC = 0;
                                    leu_prefixo_AC = 0;
                                }
                                else{
                                    strncpy(string_valor_AC, vetor_binario+(8-resto),resto);
                                    string_valor_AC[resto] = '\0';

                                    resto_valor_AC = categoria-(resto);
                                    resto = 0;
                                }
                                break;
                            }
                            else if(leu_prefixo_AC == 0){
                                resto_prefixo_AC = 1;
                            }
                        }
                        else{
                            resto_prefixo_AC = 0;
                            resto_valor_AC = 0;
                            leu_prefixo_AC = 0;
                            resto = resto - i;
                            break;
                        }
                    }
                    if(resto_prefixo_AC == 1){
                        resto = 0;
                    }
                }
            }
            else{
                if (resto_valor_AC > 0){
                    if (resto_valor_AC <= 8){
                        strncpy(string_valor_AC_aux, vetor_binario, resto_valor_AC);
                        string_valor_AC_aux[resto_valor_AC] = '\0';

                        strcat(string_valor_AC, string_valor_AC_aux);

                        resto = 8-resto_valor_AC;
                        resto_valor_AC = 0;
                        leu_prefixo_AC = 0;
                    }
                    else{
                        strncpy(string_valor_AC_aux, vetor_binario, 8);
                        string_valor_AC_aux[resto] = '\0';

                        strcat(string_valor_AC, string_valor_AC_aux);
                        resto = 0;

                        resto_valor_AC = resto_valor_AC-8;
                    }
                }
                else{
                    if (resto_prefixo_AC == 1){
                        for(int i = 0; i < 8; i++){
                            strncpy(string_comparacao_aux, vetor_binario+i, 1);
                            string_comparacao_aux[1] = '\0';

                            strcat(string_comparacao, string_comparacao_aux);

                            //Recuperando numero de zeros e categoria
                        if(strlen(string_comparacao) == 2){
                            for(int j = 0; j < 2; j++){
                                if(strcmp(string_comparacao,prefix_table_AC[coef_AC_2[j][0]][coef_AC_2[j][1]].prefix_bits) == 0){

                                    num_zeros = coef_AC_2[j][0];
                                    categoria = coef_AC_2[j][1];
                                    leu_prefixo_AC = 1;
                                    resto_prefixo_AC = 0;
                                    break;
                                }
                            }
                        }
                        else if(strlen(string_comparacao) == 3){
                                if(strcmp(string_comparacao,prefix_table_AC[coef_AC_3[0][0]][coef_AC_3[0][1]].prefix_bits) == 0){

                                    num_zeros = coef_AC_3[0][0];
                                    categoria = coef_AC_3[0][1];
                                    leu_prefixo_AC = 1;
                                    resto_prefixo_AC = 0;
                                }
                            }
                            else if(strlen(string_comparacao) == 4){
                                for(int j = 0; j < 3; j++){
                                    if(strcmp(string_comparacao,prefix_table_AC[coef_AC_4[j][0]][coef_AC_4[j][1]].prefix_bits) == 0){

                                        num_zeros = coef_AC_4[j][0];
                                        categoria = coef_AC_4[j][1];
                                        leu_prefixo_AC = 1;
                                        resto_prefixo_AC = 0;

                                        //Lembrar que se j == 0 --> fim da linha
                                        if(j == 0){
                                            EOB = 1; //preenche as linhas do AC e parte para pr�ximo DC
                                        }
                                        break;
                                    }
                                }
                            }
                            else if(strlen(string_comparacao) == 5){
                                for(int j = 0; j < 2; j++){
                                    if(strcmp(string_comparacao,prefix_table_AC[coef_AC_5[j][0]][coef_AC_5[j][1]].prefix_bits) == 0){

                                        num_zeros = coef_AC_5[j][0];
                                        categoria = coef_AC_5[j][1];
                                        leu_prefixo_AC = 1;
                                        resto_prefixo_AC = 0;
                                        break;

                                    }
                                }
                            }
                            else if(strlen(string_comparacao) == 6){
                                for(int j = 0; j < 4; j++){
                                    if(strcmp(string_comparacao,prefix_table_AC[coef_AC_6[j][0]][coef_AC_6[j][1]].prefix_bits) == 0){

                                        num_zeros = coef_AC_6[j][0];
                                        categoria = coef_AC_6[j][1];
                                        leu_prefixo_AC = 1;
                                        resto_prefixo_AC = 0;
                                        break;

                                    }
                                }
                            }
                            else if(strlen(string_comparacao) == 7){
                                for(int j = 0; j < 4; j++){
                                    if(strcmp(string_comparacao,prefix_table_AC[coef_AC_7[j][0]][coef_AC_7[j][1]].prefix_bits) == 0){

                                        num_zeros = coef_AC_7[j][0];
                                        categoria = coef_AC_7[j][1];
                                        leu_prefixo_AC = 1;
                                        resto_prefixo_AC = 0;
                                        break;

                                    }
                                }
                            }
                            else if(strlen(string_comparacao) == 8){
                                for(int j = 0; j < 3; j++){
                                    if(strcmp(string_comparacao,prefix_table_AC[coef_AC_8[j][0]][coef_AC_8[j][1]].prefix_bits) == 0){

                                        num_zeros = coef_AC_8[j][0];
                                        categoria = coef_AC_8[j][1];
                                        leu_prefixo_AC = 1;
                                        resto_prefixo_AC = 0;
                                        break;

                                    }
                                }
                            }
                            else if(strlen(string_comparacao) == 9){
                                for(int j = 0; j < 5; j++){
                                    if(strcmp(string_comparacao,prefix_table_AC[coef_AC_9[j][0]][coef_AC_9[j][1]].prefix_bits) == 0){

                                        num_zeros = coef_AC_9[j][0];
                                        categoria = coef_AC_9[j][1];
                                        leu_prefixo_AC = 1;
                                        resto_prefixo_AC = 0;
                                        break;

                                    }
                                }
                            }
                            else if(strlen(string_comparacao) == 10){
                                for(int j = 0; j < 5; j++){
                                    if(strcmp(string_comparacao,prefix_table_AC[coef_AC_10[j][0]][coef_AC_10[j][1]].prefix_bits) == 0){

                                        num_zeros = coef_AC_10[j][0];
                                        categoria = coef_AC_10[j][1];
                                        leu_prefixo_AC = 1;
                                        resto_prefixo_AC = 0;
                                        break;

                                    }
                                }
                            }
                            else if(strlen(string_comparacao) == 11){
                                for(int j = 0; j < 5; j++){
                                    if(strcmp(string_comparacao,prefix_table_AC[coef_AC_11[j][0]][coef_AC_11[j][1]].prefix_bits) == 0){

                                        num_zeros = coef_AC_11[j][0];
                                        categoria = coef_AC_11[j][1];
                                        leu_prefixo_AC = 1;
                                        resto_prefixo_AC = 0;
                                        break;

                                    }
                                }
                            }
                            else if(strlen(string_comparacao) == 12){
                                for(int j = 0; j < 2; j++){
                                    if(strcmp(string_comparacao,prefix_table_AC[coef_AC_12[j][0]][coef_AC_12[j][1]].prefix_bits) == 0){

                                        num_zeros = coef_AC_12[j][0];
                                        categoria = coef_AC_12[j][1];
                                        leu_prefixo_AC = 1;
                                        resto_prefixo_AC = 0;

                                        if(j == 1){
                                            ext_zero = 1;
                                            num_zeros += 1;
                                        }
                                        break;

                                    }
                                }
                            }
                            else if(strlen(string_comparacao) >= 15){
                                for(int j = 0; j < 126; j++){
                                    if(strcmp(string_comparacao,prefix_table_AC[coef_AC_15[j][0]][coef_AC_15[j][1]].prefix_bits) == 0){

                                        num_zeros = coef_AC_15[j][0];
                                        categoria = coef_AC_15[j][1];
                                        leu_prefixo_AC = 1;
                                        resto_prefixo_AC = 0;
                                        break;
                                    }
                                }
                            }
                            if(EOB == 0){
                                if(ext_zero == 1){
                                    resto_prefixo_AC = 0;
                                    resto_valor_AC = 0;
                                    leu_prefixo_AC = 0;
                                    string_valor_AC[0] = '0';
                                    string_valor_AC[1] = '\0';
                                    if((i+1)<= 8){
                                        resto = 8-(i+1);
                                    }
                                    else{
                                        resto = 0;
                                    }
                                    break;
                                }
                                else if(leu_prefixo_AC == 1){
                                    resto_prefixo_AC = 0;
                                    if (((i+1)+categoria) <= 8){
                                        if(categoria > 0){
                                            strncpy(string_valor_AC, vetor_binario+(i+1), categoria);
                                            string_valor_AC[categoria] = '\0';
                                            resto = 8 - ((i+1)+categoria);
                                        }
                                        else{
                                            string_valor_AC[0] = '0';
                                            string_valor_AC[1] = '\0';
                                        }

                                        resto_valor_AC = 0;
                                        leu_prefixo_AC = 0;
                                    }
                                    else{
                                        strncpy(string_valor_AC, vetor_binario+(i+1), (8-(i+1)));
                                        string_valor_AC[8-(i+1)] = '\0';

                                        resto_valor_AC = ((i+1)+categoria) - 8;
                                        resto =0;
                                    }
                                    break;
                                }
                                else if(leu_prefixo_AC == 0){
                                    resto_prefixo_AC = 1;
                                }
                            }
                            else{
                                resto = 8 - (i+1);
                                leu_prefixo_AC = 0;
                                resto_prefixo_AC = 0;
                                resto_valor_AC = 0;
                                break;
                            }
                        }

                    }
                    else{
                        for(int i = 0; i < 8; i ++){
                            if(i == 0){
                                strncpy(string_comparacao, vetor_binario, 1);
                                string_comparacao[1] = '\0';
                            }
                            else{
                                strncpy(string_comparacao_aux, vetor_binario+i, 1);
                                string_comparacao_aux[1] = '\0';

                                strcat(string_comparacao, string_comparacao_aux);
                            }

                            if (strlen(string_comparacao) >= 2){
                                if(strlen(string_comparacao) == 2){
                                    for(int j = 0; j < 2; j++){
                                        if(strcmp(string_comparacao,prefix_table_AC[coef_AC_2[j][0]][coef_AC_2[j][1]].prefix_bits) == 0){

                                            num_zeros = coef_AC_2[j][0];
                                            categoria = coef_AC_2[j][1];
                                            leu_prefixo_AC = 1;
                                            resto_prefixo_AC = 0;
                                            break;
                                        }
                                    }
                                }
                                else if(strlen(string_comparacao) == 3){
                                    if(strcmp(string_comparacao,prefix_table_AC[coef_AC_3[0][0]][coef_AC_3[0][1]].prefix_bits) == 0){

                                        num_zeros = coef_AC_3[0][0];
                                        categoria = coef_AC_3[0][1];
                                        leu_prefixo_AC = 1;
                                        resto_prefixo_AC = 0;
                                    }
                                }
                                else if(strlen(string_comparacao) == 4){
                                    for(int j = 0; j < 3; j++){
                                        if(strcmp(string_comparacao,prefix_table_AC[coef_AC_4[j][0]][coef_AC_4[j][1]].prefix_bits) == 0){

                                            num_zeros = coef_AC_4[j][0];
                                            categoria = coef_AC_4[j][1];
                                            leu_prefixo_AC = 1;
                                            resto_prefixo_AC = 0;

                                            //Lembrar que se j == 0 --> fim da linha
                                            if(j == 0){
                                                EOB = 1; //preenche as linhas do AC e parte para pr�ximo DC
                                                resto = 4;
                                            }
                                            break;
                                        }
                                    }
                                }
                                else if(strlen(string_comparacao) == 5){
                                    for(int j = 0; j < 2; j++){
                                        if(strcmp(string_comparacao,prefix_table_AC[coef_AC_5[j][0]][coef_AC_5[j][1]].prefix_bits) == 0){

                                            num_zeros = coef_AC_5[j][0];
                                            categoria = coef_AC_5[j][1];
                                            leu_prefixo_AC = 1;
                                            resto_prefixo_AC = 0;
                                            break;

                                        }
                                    }
                                }
                                else if(strlen(string_comparacao) == 6){
                                    for(int j = 0; j < 4; j++){
                                        if(strcmp(string_comparacao,prefix_table_AC[coef_AC_6[j][0]][coef_AC_6[j][1]].prefix_bits) == 0){

                                            num_zeros = coef_AC_6[j][0];
                                            categoria = coef_AC_6[j][1];
                                            leu_prefixo_AC = 1;
                                            resto_prefixo_AC = 0;
                                            break;

                                        }
                                    }
                                }
                                else if(strlen(string_comparacao) == 7){
                                    for(int j = 0; j < 4; j++){
                                        if(strcmp(string_comparacao,prefix_table_AC[coef_AC_7[j][0]][coef_AC_7[j][1]].prefix_bits) == 0){

                                            num_zeros = coef_AC_7[j][0];
                                            categoria = coef_AC_7[j][1];
                                            leu_prefixo_AC = 1;
                                            resto_prefixo_AC = 0;
                                            break;

                                        }
                                    }
                                }
                                else if(strlen(string_comparacao) == 8){
                                    for(int j = 0; j < 3; j++){
                                        if(strcmp(string_comparacao,prefix_table_AC[coef_AC_8[j][0]][coef_AC_8[j][1]].prefix_bits) == 0){

                                            num_zeros = coef_AC_8[j][0];
                                            categoria = coef_AC_8[j][1];
                                            leu_prefixo_AC = 1;
                                            resto_prefixo_AC = 0;
                                            break;

                                        }
                                    }
                                }
                                else if(strlen(string_comparacao) == 9){
                                    for(int j = 0; j < 5; j++){
                                        if(strcmp(string_comparacao,prefix_table_AC[coef_AC_9[j][0]][coef_AC_9[j][1]].prefix_bits) == 0){

                                            num_zeros = coef_AC_9[j][0];
                                            categoria = coef_AC_9[j][1];
                                            leu_prefixo_AC = 1;
                                            resto_prefixo_AC = 0;
                                            break;

                                        }
                                    }
                                }
                                else if(strlen(string_comparacao) == 10){
                                    for(int j = 0; j < 5; j++){
                                        if(strcmp(string_comparacao,prefix_table_AC[coef_AC_10[j][0]][coef_AC_10[j][1]].prefix_bits) == 0){

                                            num_zeros = coef_AC_10[j][0];
                                            categoria = coef_AC_10[j][1];
                                            leu_prefixo_AC = 1;
                                            resto_prefixo_AC = 0;
                                            break;

                                        }
                                    }
                                }
                                else if(strlen(string_comparacao) == 11){
                                    for(int j = 0; j < 5; j++){
                                        if(strcmp(string_comparacao,prefix_table_AC[coef_AC_11[j][0]][coef_AC_11[j][1]].prefix_bits) == 0){

                                            num_zeros = coef_AC_11[j][0];
                                            categoria = coef_AC_11[j][1];
                                            leu_prefixo_AC = 1;
                                            resto_prefixo_AC = 0;
                                            break;

                                        }
                                    }
                                }
                                else if(strlen(string_comparacao) == 12){
                                    for(int j = 0; j < 2; j++){
                                        if(strcmp(string_comparacao,prefix_table_AC[coef_AC_12[j][0]][coef_AC_12[j][1]].prefix_bits) == 0){

                                            num_zeros = coef_AC_12[j][0];
                                            categoria = coef_AC_12[j][1];
                                            leu_prefixo_AC = 1;
                                            resto_prefixo_AC = 0;

                                            if(j == 1){
                                                ext_zero = 1;
                                                num_zeros+= 1;
                                            }
                                            break;

                                        }
                                    }
                                }
                                else if(strlen(string_comparacao) >= 15){
                                    for(int j = 0; j < 126; j++){
                                        if(strcmp(string_comparacao,prefix_table_AC[coef_AC_15[j][0]][coef_AC_15[j][1]].prefix_bits) == 0){

                                            num_zeros = coef_AC_15[j][0];
                                            categoria = coef_AC_15[j][1];
                                            leu_prefixo_AC = 1;
                                            resto_prefixo_AC = 0;
                                            break;

                                        }
                                    }
                                }
                                if(EOB == 0){
                                    if(ext_zero == 1){
                                        resto_prefixo_AC = 0;
                                        resto_valor_AC = 0;
                                        leu_prefixo_AC = 0;
                                        string_valor_AC[0] = '0';
                                        string_valor_AC[1] = '\0';

                                        if((i+1) <= 8){
                                            resto = 8 - (i+1);
                                        }
                                        else{
                                            resto = 0;
                                        }
                                        break;
                                    }
                                    else if(leu_prefixo_AC == 1){
                                        if (((i+1)+categoria) <= 8){
                                            if(categoria > 0){
                                                strncpy(string_valor_AC, vetor_binario+(i+1), categoria);
                                                string_valor_AC[categoria] = '\0';
                                                resto = 8 - ((i+1)+categoria);
                                            }
                                            else{
                                                string_valor_AC[0] = '0';
                                                string_valor_AC[1] = '\0';
                                            }

                                            resto_valor_AC = 0;
                                            leu_prefixo_AC = 0;
                                        }
                                        else{
                                            strncpy(string_valor_AC, vetor_binario+(i+1), (8-(i+1)));
                                            string_valor_AC[(8-(i+1))] = '\0';

                                            resto_valor_AC = ((i+1)+categoria) - 8;
                                            resto = 0;
                                        }
                                        break;
                                    }
                                    else if(leu_prefixo_AC == 0){
                                        resto_prefixo_AC = 1;
                                    }
                                }
                                else{
                                    leu_prefixo_AC = 0;
                                    resto_prefixo_AC = 0;
                                    resto_valor_AC = 0;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }while((resto_prefixo_AC != 0) || (resto_valor_AC != 0));

        if (EOB == 1){
            resto_AC_bloco = 63-cont_AC;
            for (int aux =0; aux < resto_AC_bloco; aux++){
                valores_AC[cont_DC-1][cont_AC+aux] = 0;
                cont_AC++;
            }
            cont_AC = 0;
            valor_AC = 0;
            num_zeros = resto_AC_bloco;
            //string_comparacao[0] = '\0';
            string_valor_AC[0] = '\0';
            string_comparacao_aux[0] = '\0';
            string_valor_AC_aux[0] = '\0';
            blocos_lidos++;
        }
        else{
            //logica para gravar os zeros
            if(ext_zero == 1){
                for (int aux =0; aux < num_zeros; aux++){
                    valores_AC[cont_DC-1][cont_AC] = 0;
                    cont_AC++;
                }
                num_zeros = 0;
                ext_zero = 0;
                string_comparacao[0] = '\0';
            }
            else{

                for (int aux =0; aux < num_zeros; aux++){
                    valores_AC[cont_DC-1][cont_AC] = 0;
                    cont_AC++;
                }

                valor_AC = 0;
                tamanho = strlen(string_valor_AC);

                for (int k = 0; k < tamanho; k++){
                    if(string_valor_AC[k] == '1')
                        valor_AC += pow(2,((tamanho-1)-k));
                }

                if (categoria == 1){
                    if (valor_AC < 1){
                        valor_AC = -1;
                    }
                }
                else if (categoria == 2){
                    if (valor_AC < 2){
                        for (int aux_compl=0; aux_compl < strlen(string_valor_AC); aux_compl++){
                            if (string_valor_AC[aux_compl] == '1'){
                                string_valor_AC[aux_compl] = '0';
                            }else{
                                string_valor_AC[aux_compl] = '1';
                            }
                        }
                        AC_modificado = 1;
                    }
                }else if (categoria == 3){
                    if (valor_AC < 4){
                        for (int aux_compl=0; aux_compl < strlen(string_valor_AC); aux_compl++){
                            if (string_valor_AC[aux_compl] == '1'){
                                string_valor_AC[aux_compl] = '0';
                            }else{
                                string_valor_AC[aux_compl] = '1';
                            }
                        }
                        AC_modificado = 1;
                    }
                }else if (categoria == 4){
                    if (valor_AC < 8){
                        for (int aux_compl=0; aux_compl < strlen(string_valor_AC); aux_compl++){
                            if (string_valor_AC[aux_compl] == '1'){
                                string_valor_AC[aux_compl] = '0';
                            }else{
                                string_valor_AC[aux_compl] = '1';
                            }
                        }
                        AC_modificado = 1;
                    }
                }else if (categoria == 5){
                    if (valor_AC < 16){
                        for (int aux_compl=0; aux_compl < strlen(string_valor_AC); aux_compl++){
                            if (string_valor_AC[aux_compl] == '1'){
                                string_valor_AC[aux_compl] = '0';
                            }else{
                                string_valor_AC[aux_compl] = '1';
                            }
                        }
                        AC_modificado = 1;
                    }
                }else if (categoria == 6){
                    if (valor_AC < 32){
                        for (int aux_compl=0; aux_compl < strlen(string_valor_AC); aux_compl++){
                            if (string_valor_AC[aux_compl] == '1'){
                                string_valor_AC[aux_compl] = '0';
                            }else{
                                string_valor_AC[aux_compl] = '1';
                            }
                        }
                        AC_modificado = 1;
                    }
                }else if (categoria == 7){
                    if (valor_AC < 64){
                        for (int aux_compl=0; aux_compl < strlen(string_valor_AC); aux_compl++){
                            if (string_valor_AC[aux_compl] == '1'){
                                string_valor_AC[aux_compl] = '0';
                            }else{
                                string_valor_AC[aux_compl] = '1';
                            }
                        }
                        AC_modificado = 1;
                    }
                }else if (categoria == 8){
                    if (valor_AC < 128){
                        for (int aux_compl=0; aux_compl < strlen(string_valor_AC); aux_compl++){
                            if (string_valor_AC[aux_compl] == '1'){
                                string_valor_AC[aux_compl] = '0';
                            }else{
                                string_valor_AC[aux_compl] = '1';
                            }
                        }
                        AC_modificado = 1;
                    }
                }else if (categoria == 9){
                    if (valor_AC < 256){
                        for (int aux_compl=0; aux_compl < strlen(string_valor_AC); aux_compl++){
                            if (string_valor_AC[aux_compl] == '1'){
                                string_valor_AC[aux_compl] = '0';
                            }else{
                                string_valor_AC[aux_compl] = '1';
                            }
                        }
                        AC_modificado = 1;
                    }
                }else if (categoria == 10){
                    if (valor_AC < 512){
                        for (int aux_compl=0; aux_compl < strlen(string_valor_AC); aux_compl++){
                            if (string_valor_AC[aux_compl] == '1'){
                                string_valor_AC[aux_compl] = '0';
                            }else{
                                string_valor_AC[aux_compl] = '1';
                            }
                        }
                        AC_modificado = 1;
                    }
                }else if (categoria == 11){
                    if (valor_AC < 1024){
                        for (int aux_compl=0; aux_compl < strlen(string_valor_AC); aux_compl++){
                            if (string_valor_AC[aux_compl] == '1'){
                                string_valor_AC[aux_compl] = '0';
                            }else{
                                string_valor_AC[aux_compl] = '1';
                            }
                        }
                        AC_modificado = 1;
                    }
                }

                if (AC_modificado == 1){
                    valor_AC = 0;
                    for (int k = 0; k < tamanho; k++){
                        if(string_valor_AC[k] == '1')
                            valor_AC += pow(2,((tamanho-1)-k));
                    }
                    AC_modificado = 0;
                    valor_AC = (-1)*valor_AC;
                }

                valores_AC[cont_DC-1][cont_AC] = valor_AC;
                cont_AC++;
            }
        }
    }
    printf("Saiu leitura recupera!");

    blocos[0][0] = valores_DC[0];
    for(int k = 1; k < num_blocos; k++){
        blocos[k][0] = valores_DC[k] + blocos[k-1][0];
    }
    for(int x = 0; x < num_blocos; x++){
        for(int y = 0; y < 63; y++)
            blocos[x][y+1] = valores_AC[x][y];
    }

    printf("\nRecuperou bloco!\n");
}
