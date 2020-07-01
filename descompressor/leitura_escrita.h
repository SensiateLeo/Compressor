#ifndef LEITURA_ESCRITA_H
#define LEITURA_ESCRITA_H

#include <stdio.h>
#include "codificacao.h"

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
void leituraBits(FILE *F, unsigned char *B, int num_blocos);
void exibeInformacoesCabecalhos(BMPFILEHEADER FH, BMPINFOHEADER IH);
void converte_string_binario(int n, char* vetor);
void gravaArquivoBmp(BMPFILEHEADER FH, BMPINFOHEADER IH, unsigned char **B,unsigned char **G,unsigned char **R, char* nome_imagem);
void recupera_canal(FILE *fp, PREFIX prefix_table[11], PREFIX_AC **prefix_table_AC, int num_blocos, int ** blocos);

#endif // LEITURA_ESCRITA
