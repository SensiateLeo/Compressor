#ifndef DCT_H
#define DCT_H
void dct(int **DCTMatrix, unsigned char **Matrix, int N, int M);

unsigned char ***separa_blocos_8_x_8 (unsigned char **matriz, int num_col, int num_lin);

void aplica_DCT_blocos(int ***DCT_Matrix, unsigned char ***blocos, int num_col, int num_lin);

#endif // DCT
