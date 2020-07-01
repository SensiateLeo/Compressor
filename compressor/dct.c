#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//Implementação da função que aplica a DCT nas matrizes 8x8
//Baseada na implementação fornecida em aula
void dct(int **DCTMatrix, unsigned char **Matrix, int N, int M){
    double Ci = 0.0, Cj = 0.0;
    double mat=0.0;

    for (int i = 0; i < M; ++i){
        for (int j = 0; j < N; ++j){
            Matrix[i][j] = Matrix[i][j]-(unsigned char)128;
        }
    }

    for (int i = 0; i < M; ++i){
        for (int j = 0; j < N; ++j){
            Ci = 1.0; Cj = 1.0;
            if (i==0)
                Ci = (double) (1/sqrt(2.0));
            if (j==0)
                Cj = (double) (1/sqrt(2.0));

            mat = 0.0;

            for (int x = 0; x < M; x++){
                for (int y = 0; y < N; y++){
                    mat += (double)Matrix[x][y]*cos(((2.0*x+1.0)*i*M_PI)/(2.0*M))*cos(((2.0*y+1.0)*j*M_PI)/(2.0*N));
                }
            }
            double f9 = 0.0;
            f9 = (sqrt(2.0/M)*sqrt(2.0/N));
            DCTMatrix[i][j] = round(mat*Ci*Cj*f9);
        }
    }
}

//Função para separar a matriz da imagem em blocos 8x8
//Retorna um "cubo" de n_blocos x 8 x 8
unsigned char ***separa_blocos_8_x_8 (unsigned char **matriz, int num_col, int num_lin){
    //dimensao dos blocos
    int n = 8;
    int num_blocos, i, j, lin, col;

    //numero de blocos em que a imagem sera dividida
    num_blocos = (int)(num_lin/n)*(int)(num_col/n);

    unsigned char*** blocos;

    blocos = (unsigned char ***) calloc (num_blocos, sizeof(unsigned char **));
    if (blocos == NULL) {
     printf ("** Erro: Memoria Insuficiente **");
     return (NULL);
    }

    for (i = 0; i < num_blocos; i++) {
        blocos[i] = (unsigned char**) calloc (n, sizeof(unsigned char *));
        if (blocos[i] == NULL) {
            printf ("** Erro: Memoria Insuficiente **");
            return (NULL);
        }
    }

    for (i = 0; i < num_blocos; i++) {
      for (j = 0; j < n; j++){
        blocos[i][j] = (unsigned char*) calloc (n, sizeof(unsigned char));
        if (blocos[i][j] == NULL) {
            printf ("** Erro: Memoria Insuficiente **");
            return (NULL);
        }
      }
    }

    int aux_lin = 0, aux_col = 0;
    //Dividinso os blocos 8x8
    for (i = 0; i < num_blocos; i++){
        if (((i)%((int)(num_col/n)) == 0) && (i!=0)){
            aux_lin += n;
            aux_col = 0;
        }
        for (lin = 0; lin < n; lin++){
            for(col = 0; col < n; col++){
                blocos[i][lin][col] = matriz[lin+aux_lin][col+aux_col];
            }
        }
        aux_col += n;
    }

    return blocos;
}

//Função que aplica a DCT nos blocos 8x8 do cubo
void aplica_DCT_blocos(int ***DCT_Matrix, unsigned char ***blocos, int num_col, int num_lin){
    int n = 8;
    int num_blocos;

    num_blocos = (int)(num_lin/n)*(int)(num_col/n);

    for(int i = 0; i < num_blocos; i++){
        dct(DCT_Matrix[i], blocos[i], 8, 8);
    }
}
