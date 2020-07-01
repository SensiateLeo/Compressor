#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "desquantizacao.h"
#include "gerencia_memoria.h"

//Função que aplica a desquantização nos blocos
//Baseado na implementacao fornecida em aula pelo professor
void aplica_desquantizacao(const unsigned char mat_quantizacao[8][8], int ***blocosDCT, int num_blocos, int ***Blocos_Quantizados){

    for (int i = 0; i < num_blocos; i++){
        for (int j = 0; j < 8; j++){
            for (int k = 0; k < 8; k++){
                blocosDCT[i][j][k] = round(Blocos_Quantizados[i][j][k]*mat_quantizacao[j][k]);
            }
        }

    }
}

//Função para aplica a DCT inversa
//Baseado na implementacao fornecida em aula pelo professor
void idct(unsigned char** Matrix, int **DCTMatrix, int N, int M){
    double Ci=0.0, Cj=0.0, mat = 0.0;

    for(int x=0; x<N; ++x){
        for(int y=0;y<M;++y){
            mat = 0.0;
            for(int i=0;i<N;i++){
                for(int j=0;j<M;j++){
                    Ci = 1; Cj = 1;

                    if(i==0) Ci = (double)(1/sqrt(2.0));

                    if(j==0) Cj = (double)(1/sqrt(2.0));

                    mat += (double)Ci*Cj*DCTMatrix[i][j]*cos( ((2.0*x+1.0)*i*M_PI)/(2.0*M) ) * cos( ((2.0*y+1.0)*j*M_PI)/(2.0*N) );
                }
            }
            Matrix[x][y] = round((sqrt(2.0/M)*sqrt(2.0/N))*mat);
            //printf("\n%f\n", Matrix[x][y]);
        }
    }

    for(int i=0;i<N;i++){
        for(int j=0;j<M;j++){
           DCTMatrix[i][j] = DCTMatrix[i][j]+(unsigned char)128;
        }
    }
}

//Função que aplica a IDCT nos blocos 8x8 do canal
void aplica_IDCT_blocos(unsigned char ***blocos_Y, int ***blocos, int num_blocos){

    for(int i = 0; i < num_blocos; i++){
        idct(blocos_Y[i], blocos[i],8,8);
    }

}

//Une os blocos num_blocos 8x8 do canal em uma matriz de tamanho igual à da imagem original
void une_bloco_matriz(unsigned char **Matrix, unsigned char ***blocos, int num_lin, int num_col){

    int n = 8;
    int num_blocos = (int)(num_lin/8)*(int)(num_col/8) ;

    int aux_lin = 0, aux_col = 0;
    for (int i = 0; i < num_blocos; i++){
        if (((i)%((int)(num_col/n)) == 0) && (i!=0)){
            aux_lin += n;
            aux_col = 0;
        }
        for (int lin = 0; lin < n; lin++){
            for(int col = 0; col < n; col++){
                Matrix[lin+aux_lin][col+aux_col] = blocos[i][lin][col];
            }
        }
        aux_col += n;
    }
}
