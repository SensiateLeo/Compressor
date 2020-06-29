#ifndef LEITURA_ESCRITA_H
#define LEITURA_ESCRITA_H

#include <stdio.h>

typedef struct /**** BMP file header structure ****/
 {
    unsigned short bfType; /* Magic number for file */
    unsigned int bfSize; /* Size of file */
    unsigned short bfReserved1; /* Reserved */
    unsigned short bfReserved2; /* ... */
    unsigned int bfOffBits; /* Offset to bitmap data */
 } BMPFILEHEADER;
/* bfType deve ser = "MB" */

typedef struct /**** BMP file info structure ****/
 {
    unsigned int biSize; /* Size of info header */
    int biWidth; /* Width of image */
    int biHeight; /* Height of image */
    unsigned short biPlanes; /* Number of color planes */
    unsigned short biBitCount; /* Number of bits per pixel */
    unsigned int biCompression; /* Type of compression to use */
    unsigned int biSizeImage; /* Size of image data */
    int biXPelsPerMeter; /* X pixels per meter */
    int biYPelsPerMeter; /* Y pixels per meter */
    unsigned int biClrUsed; /* Number of colors used */
    unsigned int biClrImportant; /* Number of important colors */
 } BMPINFOHEADER;

void leituraHeader(FILE *F, BMPFILEHEADER *H);
void leituraInfo(FILE *F, BMPINFOHEADER *H);
void leituraBits(FILE *F, unsigned char **B, unsigned char **G, unsigned char **R, int n, int m);
void gravaArquivoBmp(BMPFILEHEADER FH, BMPINFOHEADER IH, unsigned char **B,unsigned char **G,unsigned char **R, char* nome_imagem);
int verificaArquivoBmp(BMPFILEHEADER FH, BMPINFOHEADER IH);
void converte_string_binario(int n, int tamanho, char* vetor_aux);
void grava_result_compressao(BMPFILEHEADER FH, BMPINFOHEADER IH, int **dc_codes_Y, int ***ac_codes_Y, int **dc_codes_Cb, int ***ac_codes_Cb, int **dc_codes_Cr, int ***ac_codes_Cr, char* nome_imagem);


#endif // LEITURA_ESCRITA
