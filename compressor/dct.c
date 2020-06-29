#include <stdlib.h>
#include <stdio.h>
#include <math.h>

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


unsigned char ***separa_blocos_8_x_8 (unsigned char **matriz, int num_col, int num_lin){
    //dimensao dos blocos
    int n = 8;
    int num_blocos, i, j, lin, col;

    //numero de blocos em que a imagem sera dividida
    num_blocos = (int)(num_lin/n)*(int)(num_col/n);

    /*
    printf("\n\n============================\n");
    printf("Extracao de blocos da imagem\n");
    printf("============================\n\n");
    printf("===========\n");
    printf("Informacoes\n");
    printf("=============================================\n");
    printf("Dimensoes da imagem: %d x %d\n", num_lin, num_col);
    printf("Dimensoes dos blocos a serem extraidos: %d x %d\n", n, n);
    printf("Numero de blocos a serem extraidos: %d\n", num_blocos);
    printf("=============================================\n\n");
    */

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

void aplica_DCT_blocos(int ***DCT_Matrix, unsigned char ***blocos, int num_col, int num_lin){
    int n = 8;
    int num_blocos;

    num_blocos = (int)(num_lin/n)*(int)(num_col/n);

    for(int i = 0; i < num_blocos; i++){
        dct(DCT_Matrix[i], blocos[i], 8, 8);
    }
}
