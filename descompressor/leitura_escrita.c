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

//Exibindo informacoes obtidas do cabeçalho lido
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

//Função que le o arquivo de entrada, byte a byte
void leituraBits(FILE *F, unsigned char *B, int num_blocos){
    int i;
    for (i=0; i<num_blocos ;i++){
        fread(&B[i], sizeof(unsigned char), 1, F);
    }
}

//Função para converter um inteiro em uma string com seu correspondente valor binário
void converte_string_binario(int n, char* vetor){
    int i;
    int pos_inicial;

    char vetor_aux[9];

    if(n >= 128){
      pos_inicial = 7;
    }
    else if (n >= 64){
      pos_inicial = 6;
    }
    else if (n >= 32){
      pos_inicial = 5;
    }
    else if (n >= 16){
      pos_inicial = 4;
    }
    else if (n >= 8){
      pos_inicial = 3;
    }
    else if (n >= 4){
      pos_inicial = 2;
    }
    else if (n >= 2){
      pos_inicial = 1;
    }
    else{
      pos_inicial = 0;
    }

    for(i = 0; i < 8; i++){
      if(i > pos_inicial){
        vetor_aux[i] = '0';
      }
      else{
        if(n % 2 == 0){
          vetor_aux[i] = '0';
        }
        else{
          vetor_aux[i] = '1';
        }
        n = n/2;
      }
    }

    int pos = 7;
    for(i = 0; i < 8; i++){
      vetor[pos] = vetor_aux[i];
      pos--;
    }
    vetor[8] = '\0';
}

//Função para gravar o cabeçalho e os canais após a decdificação
//Retornando assim à imagem original
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

//Função que decodifica os bytes lidos, retornando as infromações aos canais Y, Cb e Cr após a quantização
//Retorna num_blocos vetores de 64 posições, com seus correspondentes valores DC (posição 0) e ACs
void recupera_canal(FILE *fp, PREFIX prefix_table[11], PREFIX_AC **prefix_table_AC, int num_blocos, int **blocos){

    //Buffer para leitura do arquivo compactado, byte a byte
    unsigned char buffer = 0;

    //Variáveis de controle e manipulação
    char string_comparacao[17];
    string_comparacao[0] = '\0';
    char string_comparacao_aux[17];
    string_comparacao_aux[0] = '\0';
    char string_valor_DC[17];
    char string_valor_DC_aux[17];
    char string_valor_AC[17];
    char string_valor_AC_aux[17];

    char *vetor_binario;
    vetor_binario = (char *) malloc(9*sizeof(char));

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

    //Vetor para armazenar os valores DC
    int *valores_DC;
    valores_DC = (int *) malloc(num_blocos*sizeof(int));

    //Matriz para guqrdar os valores AC
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

    //Pelo tamanho da imagem, sabemos quanto blocos correspondem a cada canal
    //Sendo assim, enquanto não lemos todos os blocos, ficamos nesse loop
    while (blocos_lidos < num_blocos){

        //Caso identificado um fim de bloco --> inicia decodificação DC
        if (EOB == 1){
            EOB = 0;
            do{
                //Resto corresponde ao que falta ser lido no buffer
                //Se o resto é zero, lê o próximo buffer
                if (resto == 0){
                    fread(&buffer, sizeof(unsigned char), 1, fp);
                    converte_string_binario((int)buffer, vetor_binario);
                }
                //Se o resto for maior que zero
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

                            //Compara a sequência lida com os prefixos DC possíveis
                            for(int j = 0; j < 11; j++){
                                if(strcmp(string_comparacao,prefix_table[j].prefix_bits) == 0){
                                    categoria = j;
                                    leu_prefixo_DC = 1;
                                    resto_prefixo_DC = 0;

                                    resto = resto-i;
                                    break;
                                }
                            }
                            //Se leu um prefixo DC
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
                    //Se ainda falta ler um valor DC
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
                            string_valor_DC_aux[8] = '\0';

                            strcat(string_valor_DC, string_valor_DC_aux);
                            resto = 0;

                            resto_valor_DC = resto_valor_DC-8;
                        }
                    }
                    else{
                        //Se está no meio da leitura de um prefixo
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
                            //Se está começando a leitura de um buffer
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
                                    //Compara com os posíveis prefixos DC
                                    for(int j = 0; j < 11; j++){
                                        if(strcmp(string_comparacao,prefix_table[j].prefix_bits) == 0){

                                            categoria = j;
                                            leu_prefixo_DC = 1;
                                            resto_prefixo_DC = 0;

                                            break;
                                        }
                                    }
                                    //Se leu o prefixo, lê a categoria
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
            //Faz a rotina acima enquanto falat ler prefixo e valor DC
            }while((resto_prefixo_DC != 0) || (resto_valor_DC != 0));

            //Aqui, temos o valor DC lido
            valor_DC = 0;
            tamanho = strlen(string_valor_DC);

            //Calculamos o valor DC
            for (int k = 0; k < tamanho; k++){
                if(string_valor_DC[k] == '1')
                    valor_DC += pow(2,((tamanho-1)-k));
            }

            //Verificamos a categoria
            //Identificar se o número é positivo ou negativo
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

            //Coloca o valor no vetor de DCs
            valores_DC[cont_DC] = valor_DC;
            cont_DC++;
        }

        //Aqui comecariam as leitutas dos ACS
        //Lembrando que os ACS devem ser armazenados na matriz "blocos"
        //Existem, no pior caso, 63 ACS

        //Quando encontrar um c�digo AC 0/0, preenche as demais linhas de "blocos" com 0
        // e come�a o pr�ximo bloco
        do{
            //Se já leu todo o buffer, lê um novo
            if (resto == 0){
                fread(&buffer, sizeof(unsigned char), 1, fp);
                converte_string_binario((int)buffer, vetor_binario);
            }

            //Se ainda há oq ser lido no buffer
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
                        //cof_AC_num foi criado para comparar somente com os prefixos do tamanho atual da string

                        //Aqui, realiza as comparações com os prefixos da tabela, de acordo com o tamanho da sequência lida
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

                                    if(j == 1){ //Se identificou uma extensão de zeros
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
                        if(EOB == 0){ //Se o prefixo lido não identificou fim do bloco
                            if(ext_zero == 1){
                                //Se identificou uma extensão de zeros
                                resto_prefixo_AC = 0;
                                resto_valor_AC = 0;
                                leu_prefixo_AC = 0;
                                resto = resto - i;
                                string_valor_AC[0] = '0';
                                string_valor_AC[1] = '\0';
                                break;
                            }
                            else if(leu_prefixo_AC == 1){
                                //Se leu um prefixo AC
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
                            //Se ainda não leu o prefixo, indica que ainda falat ler o mesmo
                            else if(leu_prefixo_AC == 0){
                                resto_prefixo_AC = 1;
                            }
                        }
                        //Se identificou fim de bloco...
                        else{
                            resto_prefixo_AC = 0;
                            resto_valor_AC = 0;
                            leu_prefixo_AC = 0;
                            resto = resto - i;
                            break;
                        }
                    }
                    //Se não leu o prefixo e passou por todo o buffer
                    if(resto_prefixo_AC == 1){
                        resto = 0;
                    }
                }
            } //Se resto == 0 --> novo buffer
            else{
                if (resto_valor_AC > 0){
                    //Se esta no meio de uma leitura de valor AC
                    if (resto_valor_AC <= 8){
                        //Se o valor esta dentro do buffer
                        strncpy(string_valor_AC_aux, vetor_binario, resto_valor_AC);
                        string_valor_AC_aux[resto_valor_AC] = '\0';

                        strcat(string_valor_AC, string_valor_AC_aux);

                        resto = 8-resto_valor_AC;
                        resto_valor_AC = 0;
                        leu_prefixo_AC = 0;
                    }
                    else{
                        //Se o valor a ser lido é maior que o buffer
                        strncpy(string_valor_AC_aux, vetor_binario, 8);
                        string_valor_AC_aux[8] = '\0';

                        strcat(string_valor_AC, string_valor_AC_aux);
                        resto = 0;

                        resto_valor_AC = resto_valor_AC-8;
                    }
                }
                else{
                    //Se ainda falta ler um prefixo AC
                    if (resto_prefixo_AC == 1){
                        //Se está no meio da leitura de um prefixo...
                        for(int i = 0; i < 8; i++){
                            strncpy(string_comparacao_aux, vetor_binario+i, 1);
                            string_comparacao_aux[1] = '\0';

                            strcat(string_comparacao, string_comparacao_aux);

                        //Recuperando numero de zeros e categoria
                        //Comparação de acordo com o tamanho da sequência lida
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

                                        if(j == 1){ //Se identificou uma extensão de zeros
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
                                //Se não identificou fim de bloco
                                if(ext_zero == 1){ //Se identificou extensão de zeros...
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
                                //Se conseguiu ler o prefixo nesse buffer
                                else if(leu_prefixo_AC == 1){
                                    resto_prefixo_AC = 0;
                                    if (((i+1)+categoria) <= 8){ //Verifica se o valor DC a ser lido esta dentro do buffer atual
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
                                        //Se o valor AC não esta somente nesse buffer
                                        //le o buffer todo e indica quanto ainda resta para a leitura
                                        strncpy(string_valor_AC, vetor_binario+(i+1), (8-(i+1)));
                                        string_valor_AC[8-(i+1)] = '\0';

                                        resto_valor_AC = ((i+1)+categoria) - 8;
                                        resto =0;
                                    }
                                    break;
                                }
                                //Se não leu prefixo, indica que falat um resto de prefixo
                                else if(leu_prefixo_AC == 0){
                                    resto_prefixo_AC = 1;
                                }
                            }
                            //Se idenificou fim de bloco
                            else{
                                resto = 8 - (i+1);
                                leu_prefixo_AC = 0;
                                resto_prefixo_AC = 0;
                                resto_valor_AC = 0;
                                break;
                            }
                        }

                    }
                    else{ //Se está começando a ler um nove prefixo
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
                            //Se a seuqnecia do prefixo tem mais que 2 bits
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
                                if(EOB == 0){ //Se não é fim de bloco...
                                    if(ext_zero == 1){ //Se viu extensão de zeros
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
                                        //Se leu prefixo
                                        if (((i+1)+categoria) <= 8){ //Verifica se o valor AC está dentro do buffer atual
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
                                        else{ //Lê todo o buffer e indica que ainda falta ler o valor AC
                                            strncpy(string_valor_AC, vetor_binario+(i+1), (8-(i+1)));
                                            string_valor_AC[(8-(i+1))] = '\0';

                                            resto_valor_AC = ((i+1)+categoria) - 8;
                                            resto = 0;
                                        }
                                        break;
                                    }
                                    else if(leu_prefixo_AC == 0){
                                            //Se não leu prefixo, indica que há um resto de prefixo AC
                                        resto_prefixo_AC = 1;
                                    }
                                }
                                else{
                                    //Se identificou um fim de bloco...
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
        //Enquanto não leu um prefixo e um valor AC, faz a rotina acima
        }while((resto_prefixo_AC != 0) || (resto_valor_AC != 0));

        if (EOB == 1){
            //Se identificou um fim de bloco
            //É necessário preencher o restante do vetor com zeros!
            resto_AC_bloco = 63-cont_AC;
            for (int aux =0; aux < resto_AC_bloco; aux++){
                valores_AC[cont_DC-1][cont_AC+aux] = 0;
            }
            cont_AC = 0;
            valor_AC = 0;
            num_zeros = resto_AC_bloco;
            string_valor_AC[0] = '\0';
            string_comparacao_aux[0] = '\0';
            string_valor_AC_aux[0] = '\0';
            blocos_lidos++;
        }
        else{
            //Se não é fim de bloco...

            //logica para gravar a extensão de zeros
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
                //Grava o total de zeros identificado
                for (int aux =0; aux < num_zeros; aux++){
                    valores_AC[cont_DC-1][cont_AC] = 0;
                    cont_AC++;
                }

                //Começamos a gravação do valor AC
                valor_AC = 0;
                tamanho = strlen(string_valor_AC);

                //Calcula o valor do AC
                for (int k = 0; k < tamanho; k++){
                    if(string_valor_AC[k] == '1')
                        valor_AC += pow(2,((tamanho-1)-k));
                }

                //Compara com a categoria identificada
                //Determinar se o valor é positivo ou negativo
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
                //Coloca o valor AC no vetor de ACs do bloco correspondente
                valores_AC[cont_DC-1][cont_AC] = valor_AC;
                cont_AC++;
            }
        }
    }

    //Aqui, já lemos todos os coeficientes DC e AC de todos os blocos do canal

    //Gravamos os valores DC
    //Desfazendo a codificaão por diferenças
    blocos[0][0] = valores_DC[0];
    for(int k = 1; k < num_blocos; k++){
        blocos[k][0] = valores_DC[k] + blocos[k-1][0];
    }
    //Gravamos os valores AC nos blocos
    for(int x = 0; x < num_blocos; x++){
        for(int y = 0; y < 63; y++)
            blocos[x][y+1] = valores_AC[x][y];
    }

    //Ao final desta função, blocos tem num_blocos vetores de 64 posições
    //que correspondem as num_blocos matrizes 8x8 que compoem o canal
}
