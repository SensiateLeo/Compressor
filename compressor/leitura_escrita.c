#include <stdio.h>
#include <stdlib.h>

#include "leitura_escrita.h"
#include "codificacao.h"

//Leitura de Bitmap (exemplo FileHeader)
void leituraHeader(FILE *F, BMPFILEHEADER *H){
// F é o arquivo Bitmap que deve ter sido “lido” do disco
    fread(&H->bfType, sizeof(unsigned short),1,F);
    fread(&H->bfSize, sizeof(unsigned int),1,F);
    fread(&H->bfReserved1, sizeof(unsigned short),1,F);
    fread(&H->bfReserved2, sizeof(unsigned short),1,F);
    fread(&H->bfOffBits, sizeof(unsigned int),1,F);
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
}

//Leitura dos Bits nas matrizes de cores R, G e B --> Le B, G e R
void leituraBits(FILE *F, unsigned char **B, unsigned char **G, unsigned char **R, int m, int n){
    int i , j;

    for (i=0; i<m ;i++){
        for(j=0;j<n;j++){
            fread(&B[i][j], sizeof(unsigned char), 1, F);
            fread(&G[i][j], sizeof(unsigned char), 1, F);
            fread(&R[i][j], sizeof(unsigned char), 1, F);
        }
    }
}

//Função que grava o resultado em um arquivo
void gravaArquivoBmp(BMPFILEHEADER FH, BMPINFOHEADER IH, unsigned char **B,unsigned char **G,unsigned char **R, char* nome_imagem){
    FILE *F;
    F = fopen(nome_imagem, "wb");
    if (F == NULL)
    {
        fprintf(stderr, "\nErro na abertura do arquivo.\n");
        exit (1);
    }

    //Gravaão do cabeçalho
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

    //Gravação dos canais B, G e R
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

//Verifica se o arquivo recebido atende as especificações
//Especificações definidas no requisto do trabalho
int verificaArquivoBmp(BMPFILEHEADER FH, BMPINFOHEADER IH){
    if (FH.bfType !=0x4D42)
    {
        printf("O arquivo nao eh do tipo BMP.");
        printf("\nEncerrando compressão");
        return 0;
    }
    else{
        printf("\nO arquivo recebido eh do formato Bitmap.");
    }

    //Verifica se o arquivo já contém compressão
    if ((int)IH.biCompression != 0)
    {
        printf("O arquivo BMP já apresenta compressao.");
        printf("\nEncerrando compressão");
        return 0;
    }
    else{
        printf("\nArquivo recebido nao possui compressao");
    }

    //Verificando se o arquivo possui 24 bits para cor
    if((int)IH.biBitCount != 24){
        printf("O modelo de imagem utilizado nao possui 24 bits para cor (Possui %d bits)",IH.biBitCount);
        printf("\nEncerrando compressao");
        return 0;
    }
    else{
        printf("\nO modelo de imagem utilizado possui 24 bits para cor");
    }

    //Verificando se as dimensoes da imagem são múltiplos de 8
    if((int)IH.biWidth%8 != 0 || (int)IH.biHeight%8 != 0){
        printf("\nAs dimensoes da imagem nao sao multiplas de 8");
        printf("\nEncerrando compressao");
        return 0;
    }
    else{
        printf("\nAs dimensoes da imagem sao multiplos de 8");
    }

    //Verificando as dimensões máximas/ mínimas da imagem
    if((int)IH.biWidth < 8 && (int)IH.biWidth > 1280 && (int)IH.biHeight < 8 && (int)IH.biHeight > 800){
        printf("\nAs dimensoes da imagem nao atendem aos requisitos de dimensao (MIN:8x8 e MX:1280x800)");
        printf("\nEncerrando compressao");
        return 0;
    }
    else{
        printf("\nAs dimensoes da imagem atendem aos requisitos de tamanho\n");
    }

    return 1;
}

//Função que converte um inteiro para um string de binários
void converte_string_binario(int n, int tamanho, char* vetor_aux){
    int r, i;
    char *vetor;
    vetor = (char *) calloc (32, sizeof(char));


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

     free(vetor);
     vetor = NULL;
}

//Função que grava somente o cabeçalho da imagem (Teste)
void gravaCabecalhoBinario(BMPFILEHEADER FH, BMPINFOHEADER IH, FILE *F){

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

}

//Função que grava os códigos Dc e AC de cada bloco codificado
//Os códigos são gravados em buffers de 1 byte por vez
void gravaBlocos(int num_blocos, int **dc_codes, int ***ac_codes, FILE *F){

    //Gravando resultado da compactacao
    //Baseado na implementa��o fornecida em aula
    unsigned char buffer = 0;   //00000000
    unsigned char buffer_aux = 255; //11111111
    unsigned char buffer_resto = 0; //00000000

    int tam = 0;
    int resto = 0;
    char *vetor_binario;
    vetor_binario = (char *) calloc (32, sizeof(char));

    char *vetor_resto;
    vetor_resto = (char *) calloc (32, sizeof(char));

    char *vetor_binario_AC;
    vetor_binario_AC = (char *) calloc (32, sizeof(char));

    int aux_1 = 0;
    int aux_2 = 0;
    int aux_3 = 0;

    int EOB = 0;

    int pos_DC = 0;
    int pos_AC = 0;

    int gravacoes = 0;

    for (int i = 0; i < num_blocos; i ++){
        EOB = 0;
        //Primeiro vamos salvar o codigo referente a informacao DC
        converte_string_binario(dc_codes[i][0], dc_codes[i][1], vetor_binario);
        tam = dc_codes[i][1];
        pos_DC = 0;

        //Se ainda houver espaco no buffer a preencher
        if (resto > 0){
            //Se o tamanho do novo codigo dc for maior que o espaco a preencher no buffer
            if(tam - resto >= 0){
                //Preenche o buffer
                for (int aux=0; aux < resto; aux ++){
                    buffer = (buffer<<1) | (vetor_binario[aux]=='1');
                }
                //Escreve o conteudo do buffer no arquivo .bin
                fwrite(&buffer, sizeof(buffer), 1, F);

                //onde parou no codigo dc
                pos_DC = resto;

                resto = 0;
                buffer = 0;
            }
            else{
                //Se o tamanho do novo codigo dc for insuficiente para preencher o buffer
                for (int aux=0; aux < tam; aux ++){
                    buffer = (buffer<<1) | (vetor_binario[aux]=='1');
                }
                //Atualiza o espaco a preencher no buffer
                resto = resto - tam;
                pos_DC = 0;
                tam = 0;
            }
        }

        if (tam > 0){
            //Como o tamanho eh maior q 0, sabemos que o buffer esta vazio --> retomamos a rotina normal
            for (aux_1 = pos_DC; aux_1 < tam; aux_1 += 8){
                if ((tam-aux_1) >= 8){
                    for (int aux_2 = 0; aux_2 < 8; aux_2++){
                        buffer = (buffer<<1) | (vetor_binario[aux_1+aux_2]=='1');
                    }
                    fwrite(&buffer, sizeof(buffer), 1, F);

                    resto = 0;
                    buffer = 0;
                }
                else{
                    resto = 8 - (tam-aux_1);
                    for (aux_2 = 0; aux_2 < (tam-aux_1); aux_2++){
                        buffer = (buffer<<1) | (vetor_binario[aux_1+aux_2]=='1');
                    }
                }
            }
        }

        //Posteriormente, salvamos os codigos referentes a informacao AC
        for (int j = 0; j < 63; j++){

            if(EOB == 1)
                continue;

            //Convertendo valores inteiros para string
            converte_string_binario(ac_codes[i][j][0], ac_codes[i][j][1], vetor_binario_AC);
            tam = ac_codes[i][j][1];
            pos_AC = 0;

            if(tam == 4 && vetor_binario_AC[0] == '1' && vetor_binario_AC[1] == '0' && vetor_binario_AC[2] == '1' && vetor_binario_AC[3] == '0'){
                EOB = 1;
            }

            //Se ainda houver espaco no buffer a preencher
            if (resto > 0){
                //Se o tamanho do novo codigo dc for maior que o espaco a preencher no buffer
                if(tam - resto >= 0){
                    //Preenche o buffer
                    for (int aux=0; aux < resto; aux ++){
                        buffer = (buffer<<1) | (vetor_binario_AC[aux]=='1');
                    }
                    //Escreve o conteudo do buffer no arquivo .bin
                    fwrite(&buffer, sizeof(buffer), 1, F);

                    //onde parou no codigo dc
                    pos_AC = resto;

                    resto = 0;
                    buffer = 0;
                }
                else{
                    //Se o tamanho do novo codigo dc for insuficiente para preencher o buffer
                    for (int aux=0; aux < tam; aux ++){
                        buffer = (buffer<<1) | (vetor_binario_AC[aux]=='1');
                    }
                    //Atualiza o espaco a preencher no buffer
                    resto = resto - tam;
                    pos_AC = 0;
                    tam = 0;
                }
            }

            if (tam > 0){
                //Como o tamanho eh maior q 0, sabemos que o buffer esta vazio --> retomamos a rotina normal
                for (aux_1 = pos_AC; aux_1 < tam; aux_1 += 8){
                    if ((tam-aux_1) >= 8){
                        for (int aux_2 = 0; aux_2 < 8; aux_2++){
                            buffer = (buffer<<1) | (vetor_binario_AC[aux_1+aux_2]=='1');
                        }
                        fwrite(&buffer, sizeof(buffer), 1, F);

                        resto = 0;
                        buffer = 0;
                    }
                    else{
                        resto = 8 - (tam-aux_1);
                        for (aux_2 = 0; aux_2 < (tam-aux_1); aux_2++){
                            buffer = (buffer<<1) | (vetor_binario_AC[aux_1+aux_2]=='1');
                        }
                    }
                }
            }
        }
    }

    if(resto > 0){
        //Caso o buffer final não tenha sido completo
        //Faz um left shift de resto posições
        //Realiza a gravação

        //Isso permite que a seuqência seja mantida
        //O que restar no buffer, será ignorado na decodificação
        converte_string_binario(buffer, 8, vetor_binario);
        for (aux_3 = 0; aux_3 < resto; aux_3++){
            buffer = (buffer<<1);
        }
        fwrite(&buffer, sizeof(buffer), 1, F);
        converte_string_binario(buffer, 8, vetor_binario);
    }

    resto = 0;
    buffer_resto = 0;
}

//Função que grava o resultado da compressao no arquivo binario
void grava_result_compressao(BMPFILEHEADER FH, BMPINFOHEADER IH, int **dc_codes_Y, int ***ac_codes_Y, int **dc_codes_Cb, int ***ac_codes_Cb, int **dc_codes_Cr, int ***ac_codes_Cr, char* nome_imagem){
    FILE *F;

    F = fopen(nome_imagem, "wb");
    if (F == NULL)
    {
        fprintf(stderr, "\nErro na abertura do arquivo.\n");
        exit (1);
    }

    //Gravando Cabecalhos
    gravaCabecalhoBinario(FH, IH, F);

    int num_blocos = (int)(IH.biWidth/8)*(int)(IH.biHeight/8);

    int num_buffers_Y;
    int num_buffers_Cb;
    int num_buffers_Cr;

    //Gravando blocos Y, Cb e Cr
    gravaBlocos(num_blocos, dc_codes_Y, ac_codes_Y, F);
    gravaBlocos(num_blocos/4, dc_codes_Cb, ac_codes_Cb, F);
    gravaBlocos(num_blocos/4, dc_codes_Cr, ac_codes_Cr, F);

    printf("\n\n====================\nFim da compressao do arquivo\n====================\n");

    //Fechando arquivos
    fclose(F);
}
