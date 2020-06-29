#include <stdio.h>

#include "ycbcr.h"

void amplia_canal(unsigned char ** canal, unsigned char ** canal_amostr, int h, int w){
    for (int i = 0; i < h/2; i++){
        for (int j = 0; j < w/2; j++){
            canal[2*i][2*j] = canal_amostr[i][j];
            canal[2*i][2*j+1] = canal_amostr[i][j];
            canal[2*i+1][2*j] = canal_amostr[i][j];
            canal[2*i+1][2*j+1] = canal_amostr[i][j];
        }
    }
}

void converte_RGB(unsigned char **Y, unsigned char **Cb, unsigned char **Cr, unsigned char **B, unsigned char **G, unsigned char **R, int w, int h){
    int i , j;
    unsigned char aux;
    for (i=0; i < h; i++){
        for(j=0; j < w; j++){
            R[i][j] = (1.402*Cr[i][j]) + Y[i][j];
            aux = Y[i][j] + (Cb[i][j]*(1.772));
            G[i][j] = (Y[i][j])-(0.344*Cb[i][i])-(0.714*Cr[i][j]);
            //Y[i][j] = 0.299*R[i][j] + 0.587*G[i][j] + 0.114*aux;
            //Cb[i][j] = 0.564*(aux - Y[i][j]);
            B[i][j] = aux;
            //Cr[i][j] = 0.713*(R[i][j] - Y[i][j]);

        }
    }
}
