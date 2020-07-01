#include <stdio.h>
#include <math.h>

#include "gerencia_memoria.h"

//Função que aplica a quantização nos blocos
void aplica_quantizacao(const unsigned char mat_quantizacao[8][8], int ***blocosDCT, int num_lin, int num_col, int *** Blocos_Quantizados){

    int num_blocos = (int)(num_lin/8)*(int)(num_col/8);

    for (int i = 0; i < num_blocos; i++){

        for (int j = 0; j < 8; j++){
            for (int k = 0; k < 8; k++){
                    //mat_quantizacao esta definida no .h (baseada na tabela usada pelo JPEG)
                Blocos_Quantizados[i][j][k] = round(blocosDCT[i][j][k]/mat_quantizacao[j][k]);
            }
        }

    }
}
