#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "codificacao.h"
#include "leitura_escrita.h"
#include "gerencia_memoria.h"

//Função para fazer o zig zag nos blocos
//blocos 8x8 --> vetores de 64 posições
void zigzag(double **blocos, int ***DCTBlocks, int num_lin, int num_col){

    int i, j;
    int num_blocos = (int)(num_lin/8)*(int)(num_col/8);

    for(i = 0; i < num_blocos; i++){
        for(j=0; j<64; j++){
            blocos[i][j] = DCTBlocks[i][SCAN[j][0]][SCAN[j][1]];
        }
    }
}

//Tabela de prefixos DC --> seguindo modelo JPEG
void define_PrefixTables(PREFIX *prefix_table){
    prefix_table[0].prefix_bits[0] = '0';
    prefix_table[0].prefix_bits[1] = '1';
    prefix_table[0].prefix_bits[2] = '0';
    prefix_table[0].prefix_bits[3] = '\0';
    prefix_table[0].tam_total = 3;

    prefix_table[1].prefix_bits[0] = '0';
    prefix_table[1].prefix_bits[1] = '1';
    prefix_table[1].prefix_bits[2] = '1';
    prefix_table[1].prefix_bits[3] = '\0';
    prefix_table[1].tam_total = 4;

    prefix_table[2].prefix_bits[0] = '1';
    prefix_table[2].prefix_bits[1] = '0';
    prefix_table[2].prefix_bits[2] = '0';
    prefix_table[2].prefix_bits[3] = '\0';
    prefix_table[2].tam_total = 5;

    prefix_table[3].prefix_bits[0] = '0';
    prefix_table[3].prefix_bits[1] = '0';
    prefix_table[3].prefix_bits[2] = '\0';
    prefix_table[3].tam_total = 5;

    prefix_table[4].prefix_bits[0] = '1';
    prefix_table[4].prefix_bits[1] = '0';
    prefix_table[4].prefix_bits[2] = '1';
    prefix_table[4].prefix_bits[3] = '\0';
    prefix_table[4].tam_total = 7;

    prefix_table[5].prefix_bits[0] = '1';
    prefix_table[5].prefix_bits[1] = '1';
    prefix_table[5].prefix_bits[2] = '0';
    prefix_table[5].prefix_bits[3] = '\0';
    prefix_table[5].tam_total = 8;

    prefix_table[6].prefix_bits[0] = '1';
    prefix_table[6].prefix_bits[1] = '1';
    prefix_table[6].prefix_bits[2] = '1';
    prefix_table[6].prefix_bits[3] = '0';
    prefix_table[6].prefix_bits[4] = '\0';
    prefix_table[6].tam_total = 10;

    prefix_table[7].prefix_bits[0] = '1';
    prefix_table[7].prefix_bits[1] = '1';
    prefix_table[7].prefix_bits[2] = '1';
    prefix_table[7].prefix_bits[3] = '1';
    prefix_table[7].prefix_bits[4] = '0';
    prefix_table[7].prefix_bits[5] = '\0';
    prefix_table[7].tam_total = 12;

    prefix_table[8].prefix_bits[0] = '1';
    prefix_table[8].prefix_bits[1] = '1';
    prefix_table[8].prefix_bits[2] = '1';
    prefix_table[8].prefix_bits[3] = '1';
    prefix_table[8].prefix_bits[4] = '1';
    prefix_table[8].prefix_bits[5] = '0';
    prefix_table[8].prefix_bits[6] = '\0';
    prefix_table[8].tam_total = 14;

    prefix_table[9].prefix_bits[0] = '1';
    prefix_table[9].prefix_bits[1] = '1';
    prefix_table[9].prefix_bits[2] = '1';
    prefix_table[9].prefix_bits[3] = '1';
    prefix_table[9].prefix_bits[4] = '1';
    prefix_table[9].prefix_bits[5] = '1';
    prefix_table[9].prefix_bits[6] = '0';
    prefix_table[9].prefix_bits[7] = '\0';
    prefix_table[9].tam_total = 16;

    prefix_table[10].prefix_bits[0] = '1';
    prefix_table[10].prefix_bits[1] = '1';
    prefix_table[10].prefix_bits[2] = '1';
    prefix_table[10].prefix_bits[3] = '1';
    prefix_table[10].prefix_bits[4] = '1';
    prefix_table[10].prefix_bits[5] = '1';
    prefix_table[10].prefix_bits[6] = '1';
    prefix_table[10].prefix_bits[7] = '0';
    prefix_table[10].prefix_bits[8] = '\0';
    prefix_table[10].tam_total = 18;
}

//Tabela de prefixos AC --> seguindo modelo JPEG
void define_PrefixTablesAC(PREFIX_AC **prefix_table){
    //zeros/categoria --> bits e tamanho
    prefix_table[0][0].prefix_bits[0] = '1';
    prefix_table[0][0].prefix_bits[1] = '0';
    prefix_table[0][0].prefix_bits[2] = '1';
    prefix_table[0][0].prefix_bits[3] = '0';
    prefix_table[0][0].prefix_bits[4] = '\0';
    prefix_table[0][0].tam_total = 4;

    prefix_table[0][1].prefix_bits[0] = '0';
    prefix_table[0][1].prefix_bits[1] = '0';
    prefix_table[0][1].prefix_bits[2] = '\0';
    prefix_table[0][1].tam_total = 3;

    prefix_table[0][2].prefix_bits[0] = '0';
    prefix_table[0][2].prefix_bits[1] = '1';
    prefix_table[0][2].prefix_bits[2] = '\0';
    prefix_table[0][2].tam_total = 4;

    prefix_table[0][3].prefix_bits[0] = '1';
    prefix_table[0][3].prefix_bits[1] = '0';
    prefix_table[0][3].prefix_bits[2] = '0';
    prefix_table[0][3].prefix_bits[3] = '\0';
    prefix_table[0][3].tam_total = 6;

    prefix_table[0][4].prefix_bits[0] = '1';
    prefix_table[0][4].prefix_bits[1] = '0';
    prefix_table[0][4].prefix_bits[2] = '1';
    prefix_table[0][4].prefix_bits[3] = '1';
    prefix_table[0][4].prefix_bits[4] = '\0';
    prefix_table[0][4].tam_total = 8;

    prefix_table[0][5].prefix_bits[0] = '1';
    prefix_table[0][5].prefix_bits[1] = '1';
    prefix_table[0][5].prefix_bits[2] = '0';
    prefix_table[0][5].prefix_bits[3] = '1';
    prefix_table[0][5].prefix_bits[4] = '0';
    prefix_table[0][5].prefix_bits[5] = '\0';
    prefix_table[0][5].tam_total = 10;

    prefix_table[0][6].prefix_bits[0] = '1';
    prefix_table[0][6].prefix_bits[1] = '1';
    prefix_table[0][6].prefix_bits[2] = '1';
    prefix_table[0][6].prefix_bits[3] = '0';
    prefix_table[0][6].prefix_bits[4] = '0';
    prefix_table[0][6].prefix_bits[5] = '0';
    prefix_table[0][6].prefix_bits[6] = '\0';
    prefix_table[0][6].tam_total = 12;

    prefix_table[0][7].prefix_bits[0] = '1';
    prefix_table[0][7].prefix_bits[1] = '1';
    prefix_table[0][7].prefix_bits[2] = '1';
    prefix_table[0][7].prefix_bits[3] = '1';
    prefix_table[0][7].prefix_bits[4] = '0';
    prefix_table[0][7].prefix_bits[5] = '0';
    prefix_table[0][7].prefix_bits[6] = '0';
    prefix_table[0][7].prefix_bits[7] = '\0';
    prefix_table[0][7].tam_total = 14;

    prefix_table[0][8].prefix_bits[0] = '1';
    prefix_table[0][8].prefix_bits[1] = '1';
    prefix_table[0][8].prefix_bits[2] = '1';
    prefix_table[0][8].prefix_bits[3] = '1';
    prefix_table[0][8].prefix_bits[4] = '1';
    prefix_table[0][8].prefix_bits[5] = '1';
    prefix_table[0][8].prefix_bits[6] = '0';
    prefix_table[0][8].prefix_bits[7] = '1';
    prefix_table[0][8].prefix_bits[8] = '1';
    prefix_table[0][8].prefix_bits[9] = '0';
    prefix_table[0][8].prefix_bits[10] = '\0';
    prefix_table[0][8].tam_total = 18;

    prefix_table[0][9].prefix_bits[0] = '1';
    prefix_table[0][9].prefix_bits[1] = '1';
    prefix_table[0][9].prefix_bits[2] = '1';
    prefix_table[0][9].prefix_bits[3] = '1';
    prefix_table[0][9].prefix_bits[4] = '1';
    prefix_table[0][9].prefix_bits[5] = '1';
    prefix_table[0][9].prefix_bits[6] = '1';
    prefix_table[0][9].prefix_bits[7] = '1';
    prefix_table[0][9].prefix_bits[8] = '1';
    prefix_table[0][9].prefix_bits[9] = '0';
    prefix_table[0][9].prefix_bits[10] = '0';
    prefix_table[0][9].prefix_bits[11] = '0';
    prefix_table[0][9].prefix_bits[12] = '0';
    prefix_table[0][9].prefix_bits[13] = '0';
    prefix_table[0][9].prefix_bits[14] = '1';
    prefix_table[0][9].prefix_bits[15] = '0';
    prefix_table[0][9].prefix_bits[16] = '\0';
    prefix_table[0][9].tam_total = 25;

    prefix_table[0][10].prefix_bits[0] = '1';
    prefix_table[0][10].prefix_bits[1] = '1';
    prefix_table[0][10].prefix_bits[2] = '1';
    prefix_table[0][10].prefix_bits[3] = '1';
    prefix_table[0][10].prefix_bits[4] = '1';
    prefix_table[0][10].prefix_bits[5] = '1';
    prefix_table[0][10].prefix_bits[6] = '1';
    prefix_table[0][10].prefix_bits[7] = '1';
    prefix_table[0][10].prefix_bits[8] = '1';
    prefix_table[0][10].prefix_bits[9] = '0';
    prefix_table[0][10].prefix_bits[10] = '0';
    prefix_table[0][10].prefix_bits[11] = '0';
    prefix_table[0][10].prefix_bits[12] = '0';
    prefix_table[0][10].prefix_bits[13] = '0';
    prefix_table[0][10].prefix_bits[14] = '1';
    prefix_table[0][10].prefix_bits[15] = '1';
    prefix_table[0][10].prefix_bits[16] = '\0';
    prefix_table[0][10].tam_total = 26;

    prefix_table[1][1].prefix_bits[0] = '1';
    prefix_table[1][1].prefix_bits[1] = '1';
    prefix_table[1][1].prefix_bits[2] = '0';
    prefix_table[1][1].prefix_bits[3] = '0';
    prefix_table[1][1].prefix_bits[4] = '\0';
    prefix_table[1][1].tam_total = 5;

    prefix_table[1][2].prefix_bits[0] = '1';
    prefix_table[1][2].prefix_bits[1] = '1';
    prefix_table[1][2].prefix_bits[2] = '1';
    prefix_table[1][2].prefix_bits[3] = '0';
    prefix_table[1][2].prefix_bits[4] = '0';
    prefix_table[1][2].prefix_bits[5] = '1';
    prefix_table[1][2].prefix_bits[6] = '\0';
    prefix_table[1][2].tam_total = 8;

    prefix_table[1][3].prefix_bits[0] = '1';
    prefix_table[1][3].prefix_bits[1] = '1';
    prefix_table[1][3].prefix_bits[2] = '1';
    prefix_table[1][3].prefix_bits[3] = '1';
    prefix_table[1][3].prefix_bits[4] = '0';
    prefix_table[1][3].prefix_bits[5] = '0';
    prefix_table[1][3].prefix_bits[6] = '1';
    prefix_table[1][3].prefix_bits[7] = '\0';
    prefix_table[1][3].tam_total = 10;

    prefix_table[1][4].prefix_bits[0] = '1';
    prefix_table[1][4].prefix_bits[1] = '1';
    prefix_table[1][4].prefix_bits[2] = '1';
    prefix_table[1][4].prefix_bits[3] = '1';
    prefix_table[1][4].prefix_bits[4] = '1';
    prefix_table[1][4].prefix_bits[5] = '0';
    prefix_table[1][4].prefix_bits[6] = '1';
    prefix_table[1][4].prefix_bits[7] = '1';
    prefix_table[1][4].prefix_bits[8] = '0';
    prefix_table[1][4].prefix_bits[9] = '\0';
    prefix_table[1][4].tam_total = 13;

    prefix_table[1][5].prefix_bits[0] = '1';
    prefix_table[1][5].prefix_bits[1] = '1';
    prefix_table[1][5].prefix_bits[2] = '1';
    prefix_table[1][5].prefix_bits[3] = '1';
    prefix_table[1][5].prefix_bits[4] = '1';
    prefix_table[1][5].prefix_bits[5] = '1';
    prefix_table[1][5].prefix_bits[6] = '1';
    prefix_table[1][5].prefix_bits[7] = '0';
    prefix_table[1][5].prefix_bits[8] = '1';
    prefix_table[1][5].prefix_bits[9] = '1';
    prefix_table[1][5].prefix_bits[10] = '0';
    prefix_table[1][5].prefix_bits[11] = '\0';
    prefix_table[1][5].tam_total = 16;

    prefix_table[1][6].prefix_bits[0] = '1';
    prefix_table[1][6].prefix_bits[1] = '1';
    prefix_table[1][6].prefix_bits[2] = '1';
    prefix_table[1][6].prefix_bits[3] = '1';
    prefix_table[1][6].prefix_bits[4] = '1';
    prefix_table[1][6].prefix_bits[5] = '1';
    prefix_table[1][6].prefix_bits[6] = '1';
    prefix_table[1][6].prefix_bits[7] = '1';
    prefix_table[1][6].prefix_bits[8] = '1';
    prefix_table[1][6].prefix_bits[9] = '0';
    prefix_table[1][6].prefix_bits[10] = '0';
    prefix_table[1][6].prefix_bits[11] = '0';
    prefix_table[1][6].prefix_bits[12] = '0';
    prefix_table[1][6].prefix_bits[13] = '1';
    prefix_table[1][6].prefix_bits[14] = '0';
    prefix_table[1][6].prefix_bits[15] = '0';
    prefix_table[1][6].prefix_bits[16] = '\0';
    prefix_table[1][6].tam_total = 22;

    prefix_table[1][7].prefix_bits[0] = '1';
    prefix_table[1][7].prefix_bits[1] = '1';
    prefix_table[1][7].prefix_bits[2] = '1';
    prefix_table[1][7].prefix_bits[3] = '1';
    prefix_table[1][7].prefix_bits[4] = '1';
    prefix_table[1][7].prefix_bits[5] = '1';
    prefix_table[1][7].prefix_bits[6] = '1';
    prefix_table[1][7].prefix_bits[7] = '1';
    prefix_table[1][7].prefix_bits[8] = '1';
    prefix_table[1][7].prefix_bits[9] = '0';
    prefix_table[1][7].prefix_bits[10] = '0';
    prefix_table[1][7].prefix_bits[11] = '0';
    prefix_table[1][7].prefix_bits[12] = '0';
    prefix_table[1][7].prefix_bits[13] = '1';
    prefix_table[1][7].prefix_bits[14] = '0';
    prefix_table[1][7].prefix_bits[15] = '1';
    prefix_table[1][7].prefix_bits[16] = '\0';
    prefix_table[1][7].tam_total = 23;

    prefix_table[1][8].prefix_bits[0] = '1';
    prefix_table[1][8].prefix_bits[1] = '1';
    prefix_table[1][8].prefix_bits[2] = '1';
    prefix_table[1][8].prefix_bits[3] = '1';
    prefix_table[1][8].prefix_bits[4] = '1';
    prefix_table[1][8].prefix_bits[5] = '1';
    prefix_table[1][8].prefix_bits[6] = '1';
    prefix_table[1][8].prefix_bits[7] = '1';
    prefix_table[1][8].prefix_bits[8] = '1';
    prefix_table[1][8].prefix_bits[9] = '0';
    prefix_table[1][8].prefix_bits[10] = '0';
    prefix_table[1][8].prefix_bits[11] = '0';
    prefix_table[1][8].prefix_bits[12] = '0';
    prefix_table[1][8].prefix_bits[13] = '1';
    prefix_table[1][8].prefix_bits[14] = '1';
    prefix_table[1][8].prefix_bits[15] = '0';
    prefix_table[1][8].prefix_bits[16] = '\0';
    prefix_table[1][8].tam_total = 24;

    prefix_table[1][9].prefix_bits[0] = '1';
    prefix_table[1][9].prefix_bits[1] = '1';
    prefix_table[1][9].prefix_bits[2] = '1';
    prefix_table[1][9].prefix_bits[3] = '1';
    prefix_table[1][9].prefix_bits[4] = '1';
    prefix_table[1][9].prefix_bits[5] = '1';
    prefix_table[1][9].prefix_bits[6] = '1';
    prefix_table[1][9].prefix_bits[7] = '1';
    prefix_table[1][9].prefix_bits[8] = '1';
    prefix_table[1][9].prefix_bits[9] = '0';
    prefix_table[1][9].prefix_bits[10] = '0';
    prefix_table[1][9].prefix_bits[11] = '0';
    prefix_table[1][9].prefix_bits[12] = '0';
    prefix_table[1][9].prefix_bits[13] = '1';
    prefix_table[1][9].prefix_bits[14] = '1';
    prefix_table[1][9].prefix_bits[15] = '1';
    prefix_table[1][9].prefix_bits[16] = '\0';
    prefix_table[1][9].tam_total = 25;

    prefix_table[1][10].prefix_bits[0] = '1';
    prefix_table[1][10].prefix_bits[1] = '1';
    prefix_table[1][10].prefix_bits[2] = '1';
    prefix_table[1][10].prefix_bits[3] = '1';
    prefix_table[1][10].prefix_bits[4] = '1';
    prefix_table[1][10].prefix_bits[5] = '1';
    prefix_table[1][10].prefix_bits[6] = '1';
    prefix_table[1][10].prefix_bits[7] = '1';
    prefix_table[1][10].prefix_bits[8] = '1';
    prefix_table[1][10].prefix_bits[9] = '0';
    prefix_table[1][10].prefix_bits[10] = '0';
    prefix_table[1][10].prefix_bits[11] = '0';
    prefix_table[1][10].prefix_bits[12] = '1';
    prefix_table[1][10].prefix_bits[13] = '0';
    prefix_table[1][10].prefix_bits[14] = '0';
    prefix_table[1][10].prefix_bits[15] = '0';
    prefix_table[1][10].prefix_bits[16] = '\0';
    prefix_table[1][10].tam_total = 26;

    prefix_table[2][1].prefix_bits[0] = '1';
    prefix_table[2][1].prefix_bits[1] = '1';
    prefix_table[2][1].prefix_bits[2] = '0';
    prefix_table[2][1].prefix_bits[3] = '1';
    prefix_table[2][1].prefix_bits[4] = '1';
    prefix_table[2][1].prefix_bits[5] = '\0';
    prefix_table[2][1].tam_total = 6;

    prefix_table[2][2].prefix_bits[0] = '1';
    prefix_table[2][2].prefix_bits[1] = '1';
    prefix_table[2][2].prefix_bits[2] = '1';
    prefix_table[2][2].prefix_bits[3] = '1';
    prefix_table[2][2].prefix_bits[4] = '1';
    prefix_table[2][2].prefix_bits[5] = '0';
    prefix_table[2][2].prefix_bits[6] = '0';
    prefix_table[2][2].prefix_bits[7] = '0';
    prefix_table[2][2].prefix_bits[8] = '\0';
    prefix_table[2][2].tam_total = 10;

    prefix_table[2][3].prefix_bits[0] = '1';
    prefix_table[2][3].prefix_bits[1] = '1';
    prefix_table[2][3].prefix_bits[2] = '1';
    prefix_table[2][3].prefix_bits[3] = '1';
    prefix_table[2][3].prefix_bits[4] = '1';
    prefix_table[2][3].prefix_bits[5] = '1';
    prefix_table[2][3].prefix_bits[6] = '0';
    prefix_table[2][3].prefix_bits[7] = '1';
    prefix_table[2][3].prefix_bits[8] = '1';
    prefix_table[2][3].prefix_bits[9] = '1';
    prefix_table[2][3].prefix_bits[10] = '\0';
    prefix_table[2][3].tam_total = 13;

    prefix_table[2][4].prefix_bits[0] = '1';
    prefix_table[2][4].prefix_bits[1] = '1';
    prefix_table[2][4].prefix_bits[2] = '1';
    prefix_table[2][4].prefix_bits[3] = '1';
    prefix_table[2][4].prefix_bits[4] = '1';
    prefix_table[2][4].prefix_bits[5] = '1';
    prefix_table[2][4].prefix_bits[6] = '1';
    prefix_table[2][4].prefix_bits[7] = '1';
    prefix_table[2][4].prefix_bits[8] = '1';
    prefix_table[2][4].prefix_bits[9] = '0';
    prefix_table[2][4].prefix_bits[10] = '0';
    prefix_table[2][4].prefix_bits[11] = '0';
    prefix_table[2][4].prefix_bits[12] = '1';
    prefix_table[2][4].prefix_bits[13] = '0';
    prefix_table[2][4].prefix_bits[14] = '0';
    prefix_table[2][4].prefix_bits[15] = '1';
    prefix_table[2][4].prefix_bits[16] = '\0';
    prefix_table[2][4].tam_total = 20;

    prefix_table[2][5].prefix_bits[0] = '1';
    prefix_table[2][5].prefix_bits[1] = '1';
    prefix_table[2][5].prefix_bits[2] = '1';
    prefix_table[2][5].prefix_bits[3] = '1';
    prefix_table[2][5].prefix_bits[4] = '1';
    prefix_table[2][5].prefix_bits[5] = '1';
    prefix_table[2][5].prefix_bits[6] = '1';
    prefix_table[2][5].prefix_bits[7] = '1';
    prefix_table[2][5].prefix_bits[8] = '1';
    prefix_table[2][5].prefix_bits[9] = '0';
    prefix_table[2][5].prefix_bits[10] = '0';
    prefix_table[2][5].prefix_bits[11] = '0';
    prefix_table[2][5].prefix_bits[12] = '1';
    prefix_table[2][5].prefix_bits[13] = '0';
    prefix_table[2][5].prefix_bits[14] = '1';
    prefix_table[2][5].prefix_bits[15] = '0';
    prefix_table[2][5].prefix_bits[16] = '\0';
    prefix_table[2][5].tam_total = 21;

    prefix_table[2][6].prefix_bits[0] = '1';
    prefix_table[2][6].prefix_bits[1] = '1';
    prefix_table[2][6].prefix_bits[2] = '1';
    prefix_table[2][6].prefix_bits[3] = '1';
    prefix_table[2][6].prefix_bits[4] = '1';
    prefix_table[2][6].prefix_bits[5] = '1';
    prefix_table[2][6].prefix_bits[6] = '1';
    prefix_table[2][6].prefix_bits[7] = '1';
    prefix_table[2][6].prefix_bits[8] = '1';
    prefix_table[2][6].prefix_bits[9] = '0';
    prefix_table[2][6].prefix_bits[10] = '0';
    prefix_table[2][6].prefix_bits[11] = '0';
    prefix_table[2][6].prefix_bits[12] = '1';
    prefix_table[2][6].prefix_bits[13] = '0';
    prefix_table[2][6].prefix_bits[14] = '1';
    prefix_table[2][6].prefix_bits[15] = '1';
    prefix_table[2][6].prefix_bits[16] = '\0';
    prefix_table[2][6].tam_total = 22;

    prefix_table[2][7].prefix_bits[0] = '1';
    prefix_table[2][7].prefix_bits[1] = '1';
    prefix_table[2][7].prefix_bits[2] = '1';
    prefix_table[2][7].prefix_bits[3] = '1';
    prefix_table[2][7].prefix_bits[4] = '1';
    prefix_table[2][7].prefix_bits[5] = '1';
    prefix_table[2][7].prefix_bits[6] = '1';
    prefix_table[2][7].prefix_bits[7] = '1';
    prefix_table[2][7].prefix_bits[8] = '1';
    prefix_table[2][7].prefix_bits[9] = '0';
    prefix_table[2][7].prefix_bits[10] = '0';
    prefix_table[2][7].prefix_bits[11] = '0';
    prefix_table[2][7].prefix_bits[12] = '1';
    prefix_table[2][7].prefix_bits[13] = '1';
    prefix_table[2][7].prefix_bits[14] = '0';
    prefix_table[2][7].prefix_bits[15] = '0';
    prefix_table[2][7].prefix_bits[16] = '\0';
    prefix_table[2][7].tam_total = 23;

    prefix_table[2][8].prefix_bits[0] = '1';
    prefix_table[2][8].prefix_bits[1] = '1';
    prefix_table[2][8].prefix_bits[2] = '1';
    prefix_table[2][8].prefix_bits[3] = '1';
    prefix_table[2][8].prefix_bits[4] = '1';
    prefix_table[2][8].prefix_bits[5] = '1';
    prefix_table[2][8].prefix_bits[6] = '1';
    prefix_table[2][8].prefix_bits[7] = '1';
    prefix_table[2][8].prefix_bits[8] = '1';
    prefix_table[2][8].prefix_bits[9] = '0';
    prefix_table[2][8].prefix_bits[10] = '0';
    prefix_table[2][8].prefix_bits[11] = '0';
    prefix_table[2][8].prefix_bits[12] = '1';
    prefix_table[2][8].prefix_bits[13] = '1';
    prefix_table[2][8].prefix_bits[14] = '0';
    prefix_table[2][8].prefix_bits[15] = '1';
    prefix_table[2][8].prefix_bits[16] = '\0';
    prefix_table[2][8].tam_total = 24;

    prefix_table[2][9].prefix_bits[0] = '1';
    prefix_table[2][9].prefix_bits[1] = '1';
    prefix_table[2][9].prefix_bits[2] = '1';
    prefix_table[2][9].prefix_bits[3] = '1';
    prefix_table[2][9].prefix_bits[4] = '1';
    prefix_table[2][9].prefix_bits[5] = '1';
    prefix_table[2][9].prefix_bits[6] = '1';
    prefix_table[2][9].prefix_bits[7] = '1';
    prefix_table[2][9].prefix_bits[8] = '1';
    prefix_table[2][9].prefix_bits[9] = '0';
    prefix_table[2][9].prefix_bits[10] = '0';
    prefix_table[2][9].prefix_bits[11] = '0';
    prefix_table[2][9].prefix_bits[12] = '1';
    prefix_table[2][9].prefix_bits[13] = '1';
    prefix_table[2][9].prefix_bits[14] = '1';
    prefix_table[2][9].prefix_bits[15] = '0';
    prefix_table[2][9].prefix_bits[16] = '\0';
    prefix_table[2][9].tam_total = 25;

    prefix_table[2][10].prefix_bits[0] = '1';
    prefix_table[2][10].prefix_bits[1] = '1';
    prefix_table[2][10].prefix_bits[2] = '1';
    prefix_table[2][10].prefix_bits[3] = '1';
    prefix_table[2][10].prefix_bits[4] = '1';
    prefix_table[2][10].prefix_bits[5] = '1';
    prefix_table[2][10].prefix_bits[6] = '1';
    prefix_table[2][10].prefix_bits[7] = '1';
    prefix_table[2][10].prefix_bits[8] = '1';
    prefix_table[2][10].prefix_bits[9] = '0';
    prefix_table[2][10].prefix_bits[10] = '0';
    prefix_table[2][10].prefix_bits[11] = '0';
    prefix_table[2][10].prefix_bits[12] = '1';
    prefix_table[2][10].prefix_bits[13] = '1';
    prefix_table[2][10].prefix_bits[14] = '1';
    prefix_table[2][10].prefix_bits[15] = '1';
    prefix_table[2][10].prefix_bits[16] = '\0';
    prefix_table[2][10].tam_total = 26;

    prefix_table[3][1].prefix_bits[0] = '1';
    prefix_table[3][1].prefix_bits[1] = '1';
    prefix_table[3][1].prefix_bits[2] = '1';
    prefix_table[3][1].prefix_bits[3] = '0';
    prefix_table[3][1].prefix_bits[4] = '1';
    prefix_table[3][1].prefix_bits[5] = '0';
    prefix_table[3][1].prefix_bits[6] = '\0';
    prefix_table[3][1].tam_total = 7;

    prefix_table[3][2].prefix_bits[0] = '1';
    prefix_table[3][2].prefix_bits[1] = '1';
    prefix_table[3][2].prefix_bits[2] = '1';
    prefix_table[3][2].prefix_bits[3] = '1';
    prefix_table[3][2].prefix_bits[4] = '1';
    prefix_table[3][2].prefix_bits[5] = '0';
    prefix_table[3][2].prefix_bits[6] = '1';
    prefix_table[3][2].prefix_bits[7] = '1';
    prefix_table[3][2].prefix_bits[8] = '1';
    prefix_table[3][2].prefix_bits[9] = '\0';
    prefix_table[3][2].tam_total = 11;

    prefix_table[3][3].prefix_bits[0] = '1';
    prefix_table[3][3].prefix_bits[1] = '1';
    prefix_table[3][3].prefix_bits[2] = '1';
    prefix_table[3][3].prefix_bits[3] = '1';
    prefix_table[3][3].prefix_bits[4] = '1';
    prefix_table[3][3].prefix_bits[5] = '1';
    prefix_table[3][3].prefix_bits[6] = '1';
    prefix_table[3][3].prefix_bits[7] = '0';
    prefix_table[3][3].prefix_bits[8] = '1';
    prefix_table[3][3].prefix_bits[9] = '1';
    prefix_table[3][3].prefix_bits[10] = '1';
    prefix_table[3][3].prefix_bits[11] = '\0';
    prefix_table[3][3].tam_total = 14;

    prefix_table[3][4].prefix_bits[0] = '1';
    prefix_table[3][4].prefix_bits[1] = '1';
    prefix_table[3][4].prefix_bits[2] = '1';
    prefix_table[3][4].prefix_bits[3] = '1';
    prefix_table[3][4].prefix_bits[4] = '1';
    prefix_table[3][4].prefix_bits[5] = '1';
    prefix_table[3][4].prefix_bits[6] = '1';
    prefix_table[3][4].prefix_bits[7] = '1';
    prefix_table[3][4].prefix_bits[8] = '1';
    prefix_table[3][4].prefix_bits[9] = '0';
    prefix_table[3][4].prefix_bits[10] = '0';
    prefix_table[3][4].prefix_bits[11] = '1';
    prefix_table[3][4].prefix_bits[12] = '0';
    prefix_table[3][4].prefix_bits[13] = '0';
    prefix_table[3][4].prefix_bits[14] = '0';
    prefix_table[3][4].prefix_bits[15] = '0';
    prefix_table[3][4].prefix_bits[16] = '\0';
    prefix_table[3][4].tam_total = 20;

    prefix_table[3][5].prefix_bits[0] = '1';
    prefix_table[3][5].prefix_bits[1] = '1';
    prefix_table[3][5].prefix_bits[2] = '1';
    prefix_table[3][5].prefix_bits[3] = '1';
    prefix_table[3][5].prefix_bits[4] = '1';
    prefix_table[3][5].prefix_bits[5] = '1';
    prefix_table[3][5].prefix_bits[6] = '1';
    prefix_table[3][5].prefix_bits[7] = '1';
    prefix_table[3][5].prefix_bits[8] = '1';
    prefix_table[3][5].prefix_bits[9] = '0';
    prefix_table[3][5].prefix_bits[10] = '0';
    prefix_table[3][5].prefix_bits[11] = '1';
    prefix_table[3][5].prefix_bits[12] = '0';
    prefix_table[3][5].prefix_bits[13] = '0';
    prefix_table[3][5].prefix_bits[14] = '0';
    prefix_table[3][5].prefix_bits[15] = '1';
    prefix_table[3][5].prefix_bits[16] = '\0';
    prefix_table[3][5].tam_total = 21;

    prefix_table[3][6].prefix_bits[0] = '1';
    prefix_table[3][6].prefix_bits[1] = '1';
    prefix_table[3][6].prefix_bits[2] = '1';
    prefix_table[3][6].prefix_bits[3] = '1';
    prefix_table[3][6].prefix_bits[4] = '1';
    prefix_table[3][6].prefix_bits[5] = '1';
    prefix_table[3][6].prefix_bits[6] = '1';
    prefix_table[3][6].prefix_bits[7] = '1';
    prefix_table[3][6].prefix_bits[8] = '1';
    prefix_table[3][6].prefix_bits[9] = '0';
    prefix_table[3][6].prefix_bits[10] = '0';
    prefix_table[3][6].prefix_bits[11] = '1';
    prefix_table[3][6].prefix_bits[12] = '0';
    prefix_table[3][6].prefix_bits[13] = '0';
    prefix_table[3][6].prefix_bits[14] = '1';
    prefix_table[3][6].prefix_bits[15] = '0';
    prefix_table[3][6].prefix_bits[16] = '\0';
    prefix_table[3][6].tam_total = 22;

    prefix_table[3][7].prefix_bits[0] = '1';
    prefix_table[3][7].prefix_bits[1] = '1';
    prefix_table[3][7].prefix_bits[2] = '1';
    prefix_table[3][7].prefix_bits[3] = '1';
    prefix_table[3][7].prefix_bits[4] = '1';
    prefix_table[3][7].prefix_bits[5] = '1';
    prefix_table[3][7].prefix_bits[6] = '1';
    prefix_table[3][7].prefix_bits[7] = '1';
    prefix_table[3][7].prefix_bits[8] = '1';
    prefix_table[3][7].prefix_bits[9] = '0';
    prefix_table[3][7].prefix_bits[10] = '0';
    prefix_table[3][7].prefix_bits[11] = '1';
    prefix_table[3][7].prefix_bits[12] = '0';
    prefix_table[3][7].prefix_bits[13] = '0';
    prefix_table[3][7].prefix_bits[14] = '1';
    prefix_table[3][7].prefix_bits[15] = '1';
    prefix_table[3][7].prefix_bits[16] = '\0';
    prefix_table[3][7].tam_total = 23;

    prefix_table[3][8].prefix_bits[0] = '1';
    prefix_table[3][8].prefix_bits[1] = '1';
    prefix_table[3][8].prefix_bits[2] = '1';
    prefix_table[3][8].prefix_bits[3] = '1';
    prefix_table[3][8].prefix_bits[4] = '1';
    prefix_table[3][8].prefix_bits[5] = '1';
    prefix_table[3][8].prefix_bits[6] = '1';
    prefix_table[3][8].prefix_bits[7] = '1';
    prefix_table[3][8].prefix_bits[8] = '1';
    prefix_table[3][8].prefix_bits[9] = '0';
    prefix_table[3][8].prefix_bits[10] = '0';
    prefix_table[3][8].prefix_bits[11] = '1';
    prefix_table[3][8].prefix_bits[12] = '0';
    prefix_table[3][8].prefix_bits[13] = '1';
    prefix_table[3][8].prefix_bits[14] = '0';
    prefix_table[3][8].prefix_bits[15] = '0';
    prefix_table[3][8].prefix_bits[16] = '\0';
    prefix_table[3][8].tam_total = 24;

    prefix_table[3][9].prefix_bits[0] = '1';
    prefix_table[3][9].prefix_bits[1] = '1';
    prefix_table[3][9].prefix_bits[2] = '1';
    prefix_table[3][9].prefix_bits[3] = '1';
    prefix_table[3][9].prefix_bits[4] = '1';
    prefix_table[3][9].prefix_bits[5] = '1';
    prefix_table[3][9].prefix_bits[6] = '1';
    prefix_table[3][9].prefix_bits[7] = '1';
    prefix_table[3][9].prefix_bits[8] = '1';
    prefix_table[3][9].prefix_bits[9] = '0';
    prefix_table[3][9].prefix_bits[10] = '0';
    prefix_table[3][9].prefix_bits[11] = '1';
    prefix_table[3][9].prefix_bits[12] = '0';
    prefix_table[3][9].prefix_bits[13] = '1';
    prefix_table[3][9].prefix_bits[14] = '0';
    prefix_table[3][9].prefix_bits[15] = '1';
    prefix_table[3][9].prefix_bits[16] = '\0';
    prefix_table[3][9].tam_total = 25;

    prefix_table[3][10].prefix_bits[0] = '1';
    prefix_table[3][10].prefix_bits[1] = '1';
    prefix_table[3][10].prefix_bits[2] = '1';
    prefix_table[3][10].prefix_bits[3] = '1';
    prefix_table[3][10].prefix_bits[4] = '1';
    prefix_table[3][10].prefix_bits[5] = '1';
    prefix_table[3][10].prefix_bits[6] = '1';
    prefix_table[3][10].prefix_bits[7] = '1';
    prefix_table[3][10].prefix_bits[8] = '1';
    prefix_table[3][10].prefix_bits[9] = '0';
    prefix_table[3][10].prefix_bits[10] = '0';
    prefix_table[3][10].prefix_bits[11] = '1';
    prefix_table[3][10].prefix_bits[12] = '0';
    prefix_table[3][10].prefix_bits[13] = '1';
    prefix_table[3][10].prefix_bits[14] = '1';
    prefix_table[3][10].prefix_bits[15] = '0';
    prefix_table[3][10].prefix_bits[16] = '\0';
    prefix_table[3][10].tam_total = 26;

    prefix_table[4][1].prefix_bits[0] = '1';
    prefix_table[4][1].prefix_bits[1] = '1';
    prefix_table[4][1].prefix_bits[2] = '1';
    prefix_table[4][1].prefix_bits[3] = '0';
    prefix_table[4][1].prefix_bits[4] = '1';
    prefix_table[4][1].prefix_bits[5] = '1';
    prefix_table[4][1].prefix_bits[6] = '\0';
    prefix_table[4][1].tam_total = 7;

    prefix_table[4][2].prefix_bits[0] = '1';
    prefix_table[4][2].prefix_bits[1] = '1';
    prefix_table[4][2].prefix_bits[2] = '1';
    prefix_table[4][2].prefix_bits[3] = '1';
    prefix_table[4][2].prefix_bits[4] = '1';
    prefix_table[4][2].prefix_bits[5] = '1';
    prefix_table[4][2].prefix_bits[6] = '1';
    prefix_table[4][2].prefix_bits[7] = '0';
    prefix_table[4][2].prefix_bits[8] = '0';
    prefix_table[4][2].prefix_bits[9] = '0';
    prefix_table[4][2].prefix_bits[10] = '\0';
    prefix_table[4][2].tam_total = 12;

    prefix_table[4][3].prefix_bits[0] = '1';
    prefix_table[4][3].prefix_bits[1] = '1';
    prefix_table[4][3].prefix_bits[2] = '1';
    prefix_table[4][3].prefix_bits[3] = '1';
    prefix_table[4][3].prefix_bits[4] = '1';
    prefix_table[4][3].prefix_bits[5] = '1';
    prefix_table[4][3].prefix_bits[6] = '1';
    prefix_table[4][3].prefix_bits[7] = '1';
    prefix_table[4][3].prefix_bits[8] = '1';
    prefix_table[4][3].prefix_bits[9] = '0';
    prefix_table[4][3].prefix_bits[10] = '0';
    prefix_table[4][3].prefix_bits[11] = '1';
    prefix_table[4][3].prefix_bits[12] = '0';
    prefix_table[4][3].prefix_bits[13] = '1';
    prefix_table[4][3].prefix_bits[14] = '1';
    prefix_table[4][3].prefix_bits[15] = '1';
    prefix_table[4][3].prefix_bits[16] = '\0';
    prefix_table[4][3].tam_total = 19;

    prefix_table[4][4].prefix_bits[0] = '1';
    prefix_table[4][4].prefix_bits[1] = '1';
    prefix_table[4][4].prefix_bits[2] = '1';
    prefix_table[4][4].prefix_bits[3] = '1';
    prefix_table[4][4].prefix_bits[4] = '1';
    prefix_table[4][4].prefix_bits[5] = '1';
    prefix_table[4][4].prefix_bits[6] = '1';
    prefix_table[4][4].prefix_bits[7] = '1';
    prefix_table[4][4].prefix_bits[8] = '1';
    prefix_table[4][4].prefix_bits[9] = '0';
    prefix_table[4][4].prefix_bits[10] = '0';
    prefix_table[4][4].prefix_bits[11] = '1';
    prefix_table[4][4].prefix_bits[12] = '1';
    prefix_table[4][4].prefix_bits[13] = '0';
    prefix_table[4][4].prefix_bits[14] = '0';
    prefix_table[4][4].prefix_bits[15] = '0';
    prefix_table[4][4].prefix_bits[16] = '\0';
    prefix_table[4][4].tam_total = 20;

    prefix_table[4][5].prefix_bits[0] = '1';
    prefix_table[4][5].prefix_bits[1] = '1';
    prefix_table[4][5].prefix_bits[2] = '1';
    prefix_table[4][5].prefix_bits[3] = '1';
    prefix_table[4][5].prefix_bits[4] = '1';
    prefix_table[4][5].prefix_bits[5] = '1';
    prefix_table[4][5].prefix_bits[6] = '1';
    prefix_table[4][5].prefix_bits[7] = '1';
    prefix_table[4][5].prefix_bits[8] = '1';
    prefix_table[4][5].prefix_bits[9] = '0';
    prefix_table[4][5].prefix_bits[10] = '0';
    prefix_table[4][5].prefix_bits[11] = '1';
    prefix_table[4][5].prefix_bits[12] = '1';
    prefix_table[4][5].prefix_bits[13] = '0';
    prefix_table[4][5].prefix_bits[14] = '0';
    prefix_table[4][5].prefix_bits[15] = '1';
    prefix_table[4][5].prefix_bits[16] = '\0';
    prefix_table[4][5].tam_total = 21;

    prefix_table[4][6].prefix_bits[0] = '1';
    prefix_table[4][6].prefix_bits[1] = '1';
    prefix_table[4][6].prefix_bits[2] = '1';
    prefix_table[4][6].prefix_bits[3] = '1';
    prefix_table[4][6].prefix_bits[4] = '1';
    prefix_table[4][6].prefix_bits[5] = '1';
    prefix_table[4][6].prefix_bits[6] = '1';
    prefix_table[4][6].prefix_bits[7] = '1';
    prefix_table[4][6].prefix_bits[8] = '1';
    prefix_table[4][6].prefix_bits[9] = '0';
    prefix_table[4][6].prefix_bits[10] = '0';
    prefix_table[4][6].prefix_bits[11] = '1';
    prefix_table[4][6].prefix_bits[12] = '1';
    prefix_table[4][6].prefix_bits[13] = '0';
    prefix_table[4][6].prefix_bits[14] = '1';
    prefix_table[4][6].prefix_bits[15] = '0';
    prefix_table[4][6].prefix_bits[16] = '\0';
    prefix_table[4][6].tam_total = 22;

    prefix_table[4][7].prefix_bits[0] = '1';
    prefix_table[4][7].prefix_bits[1] = '1';
    prefix_table[4][7].prefix_bits[2] = '1';
    prefix_table[4][7].prefix_bits[3] = '1';
    prefix_table[4][7].prefix_bits[4] = '1';
    prefix_table[4][7].prefix_bits[5] = '1';
    prefix_table[4][7].prefix_bits[6] = '1';
    prefix_table[4][7].prefix_bits[7] = '1';
    prefix_table[4][7].prefix_bits[8] = '1';
    prefix_table[4][7].prefix_bits[9] = '0';
    prefix_table[4][7].prefix_bits[10] = '0';
    prefix_table[4][7].prefix_bits[11] = '1';
    prefix_table[4][7].prefix_bits[12] = '1';
    prefix_table[4][7].prefix_bits[13] = '0';
    prefix_table[4][7].prefix_bits[14] = '1';
    prefix_table[4][7].prefix_bits[15] = '1';
    prefix_table[4][7].prefix_bits[16] = '\0';
    prefix_table[4][7].tam_total = 23;

    prefix_table[4][8].prefix_bits[0] = '1';
    prefix_table[4][8].prefix_bits[1] = '1';
    prefix_table[4][8].prefix_bits[2] = '1';
    prefix_table[4][8].prefix_bits[3] = '1';
    prefix_table[4][8].prefix_bits[4] = '1';
    prefix_table[4][8].prefix_bits[5] = '1';
    prefix_table[4][8].prefix_bits[6] = '1';
    prefix_table[4][8].prefix_bits[7] = '1';
    prefix_table[4][8].prefix_bits[8] = '1';
    prefix_table[4][8].prefix_bits[9] = '0';
    prefix_table[4][8].prefix_bits[10] = '0';
    prefix_table[4][8].prefix_bits[11] = '1';
    prefix_table[4][8].prefix_bits[12] = '1';
    prefix_table[4][8].prefix_bits[13] = '1';
    prefix_table[4][8].prefix_bits[14] = '0';
    prefix_table[4][8].prefix_bits[15] = '0';
    prefix_table[4][8].prefix_bits[16] = '\0';
    prefix_table[4][8].tam_total = 24;

    prefix_table[4][9].prefix_bits[0] = '1';
    prefix_table[4][9].prefix_bits[1] = '1';
    prefix_table[4][9].prefix_bits[2] = '1';
    prefix_table[4][9].prefix_bits[3] = '1';
    prefix_table[4][9].prefix_bits[4] = '1';
    prefix_table[4][9].prefix_bits[5] = '1';
    prefix_table[4][9].prefix_bits[6] = '1';
    prefix_table[4][9].prefix_bits[7] = '1';
    prefix_table[4][9].prefix_bits[8] = '1';
    prefix_table[4][9].prefix_bits[9] = '0';
    prefix_table[4][9].prefix_bits[10] = '0';
    prefix_table[4][9].prefix_bits[11] = '1';
    prefix_table[4][9].prefix_bits[12] = '1';
    prefix_table[4][9].prefix_bits[13] = '1';
    prefix_table[4][9].prefix_bits[14] = '0';
    prefix_table[4][9].prefix_bits[15] = '1';
    prefix_table[4][9].prefix_bits[16] = '\0';
    prefix_table[4][9].tam_total = 25;

    prefix_table[4][10].prefix_bits[0] = '1';
    prefix_table[4][10].prefix_bits[1] = '1';
    prefix_table[4][10].prefix_bits[2] = '1';
    prefix_table[4][10].prefix_bits[3] = '1';
    prefix_table[4][10].prefix_bits[4] = '1';
    prefix_table[4][10].prefix_bits[5] = '1';
    prefix_table[4][10].prefix_bits[6] = '1';
    prefix_table[4][10].prefix_bits[7] = '1';
    prefix_table[4][10].prefix_bits[8] = '1';
    prefix_table[4][10].prefix_bits[9] = '0';
    prefix_table[4][10].prefix_bits[10] = '0';
    prefix_table[4][10].prefix_bits[11] = '1';
    prefix_table[4][10].prefix_bits[12] = '1';
    prefix_table[4][10].prefix_bits[13] = '1';
    prefix_table[4][10].prefix_bits[14] = '1';
    prefix_table[4][10].prefix_bits[15] = '0';
    prefix_table[4][10].prefix_bits[16] = '\0';
    prefix_table[4][10].tam_total = 26;

    prefix_table[5][1].prefix_bits[0] = '1';
    prefix_table[5][1].prefix_bits[1] = '1';
    prefix_table[5][1].prefix_bits[2] = '1';
    prefix_table[5][1].prefix_bits[3] = '1';
    prefix_table[5][1].prefix_bits[4] = '0';
    prefix_table[5][1].prefix_bits[5] = '1';
    prefix_table[5][1].prefix_bits[6] = '0';
    prefix_table[5][1].prefix_bits[7] = '\0';
    prefix_table[5][1].tam_total = 8;

    prefix_table[5][2].prefix_bits[0] = '1';
    prefix_table[5][2].prefix_bits[1] = '1';
    prefix_table[5][2].prefix_bits[2] = '1';
    prefix_table[5][2].prefix_bits[3] = '1';
    prefix_table[5][2].prefix_bits[4] = '1';
    prefix_table[5][2].prefix_bits[5] = '1';
    prefix_table[5][2].prefix_bits[6] = '1';
    prefix_table[5][2].prefix_bits[7] = '0';
    prefix_table[5][2].prefix_bits[8] = '0';
    prefix_table[5][2].prefix_bits[9] = '1';
    prefix_table[5][2].prefix_bits[10] = '\0';
    prefix_table[5][2].tam_total = 12;

    prefix_table[5][3].prefix_bits[0] = '1';
    prefix_table[5][3].prefix_bits[1] = '1';
    prefix_table[5][3].prefix_bits[2] = '1';
    prefix_table[5][3].prefix_bits[3] = '1';
    prefix_table[5][3].prefix_bits[4] = '1';
    prefix_table[5][3].prefix_bits[5] = '1';
    prefix_table[5][3].prefix_bits[6] = '1';
    prefix_table[5][3].prefix_bits[7] = '1';
    prefix_table[5][3].prefix_bits[8] = '1';
    prefix_table[5][3].prefix_bits[9] = '0';
    prefix_table[5][3].prefix_bits[10] = '0';
    prefix_table[5][3].prefix_bits[11] = '1';
    prefix_table[5][3].prefix_bits[12] = '1';
    prefix_table[5][3].prefix_bits[13] = '1';
    prefix_table[5][3].prefix_bits[14] = '1';
    prefix_table[5][3].prefix_bits[15] = '1';
    prefix_table[5][3].prefix_bits[16] = '\0';
    prefix_table[5][3].tam_total = 19;

    prefix_table[5][4].prefix_bits[0] = '1';
    prefix_table[5][4].prefix_bits[1] = '1';
    prefix_table[5][4].prefix_bits[2] = '1';
    prefix_table[5][4].prefix_bits[3] = '1';
    prefix_table[5][4].prefix_bits[4] = '1';
    prefix_table[5][4].prefix_bits[5] = '1';
    prefix_table[5][4].prefix_bits[6] = '1';
    prefix_table[5][4].prefix_bits[7] = '1';
    prefix_table[5][4].prefix_bits[8] = '1';
    prefix_table[5][4].prefix_bits[9] = '0';
    prefix_table[5][4].prefix_bits[10] = '1';
    prefix_table[5][4].prefix_bits[11] = '0';
    prefix_table[5][4].prefix_bits[12] = '0';
    prefix_table[5][4].prefix_bits[13] = '0';
    prefix_table[5][4].prefix_bits[14] = '0';
    prefix_table[5][4].prefix_bits[15] = '0';
    prefix_table[5][4].prefix_bits[16] = '\0';
    prefix_table[5][4].tam_total =20;

    prefix_table[5][5].prefix_bits[0] = '1';
    prefix_table[5][5].prefix_bits[1] = '1';
    prefix_table[5][5].prefix_bits[2] = '1';
    prefix_table[5][5].prefix_bits[3] = '1';
    prefix_table[5][5].prefix_bits[4] = '1';
    prefix_table[5][5].prefix_bits[5] = '1';
    prefix_table[5][5].prefix_bits[6] = '1';
    prefix_table[5][5].prefix_bits[7] = '1';
    prefix_table[5][5].prefix_bits[8] = '1';
    prefix_table[5][5].prefix_bits[9] = '0';
    prefix_table[5][5].prefix_bits[10] = '1';
    prefix_table[5][5].prefix_bits[11] = '0';
    prefix_table[5][5].prefix_bits[12] = '0';
    prefix_table[5][5].prefix_bits[13] = '0';
    prefix_table[5][5].prefix_bits[14] = '0';
    prefix_table[5][5].prefix_bits[15] = '1';
    prefix_table[5][5].prefix_bits[16] = '\0';
    prefix_table[5][5].tam_total = 21;

    prefix_table[5][6].prefix_bits[0] = '1';
    prefix_table[5][6].prefix_bits[1] = '1';
    prefix_table[5][6].prefix_bits[2] = '1';
    prefix_table[5][6].prefix_bits[3] = '1';
    prefix_table[5][6].prefix_bits[4] = '1';
    prefix_table[5][6].prefix_bits[5] = '1';
    prefix_table[5][6].prefix_bits[6] = '1';
    prefix_table[5][6].prefix_bits[7] = '1';
    prefix_table[5][6].prefix_bits[8] = '1';
    prefix_table[5][6].prefix_bits[9] = '0';
    prefix_table[5][6].prefix_bits[10] = '1';
    prefix_table[5][6].prefix_bits[11] = '0';
    prefix_table[5][6].prefix_bits[12] = '0';
    prefix_table[5][6].prefix_bits[13] = '0';
    prefix_table[5][6].prefix_bits[14] = '1';
    prefix_table[5][6].prefix_bits[15] = '0';
    prefix_table[5][6].prefix_bits[16] = '\0';
    prefix_table[5][6].tam_total = 22;

    prefix_table[5][7].prefix_bits[0] = '1';
    prefix_table[5][7].prefix_bits[1] = '1';
    prefix_table[5][7].prefix_bits[2] = '1';
    prefix_table[5][7].prefix_bits[3] = '1';
    prefix_table[5][7].prefix_bits[4] = '1';
    prefix_table[5][7].prefix_bits[5] = '1';
    prefix_table[5][7].prefix_bits[6] = '1';
    prefix_table[5][7].prefix_bits[7] = '1';
    prefix_table[5][7].prefix_bits[8] = '1';
    prefix_table[5][7].prefix_bits[9] = '0';
    prefix_table[5][7].prefix_bits[10] = '1';
    prefix_table[5][7].prefix_bits[11] = '0';
    prefix_table[5][7].prefix_bits[12] = '0';
    prefix_table[5][7].prefix_bits[13] = '0';
    prefix_table[5][7].prefix_bits[14] = '1';
    prefix_table[5][7].prefix_bits[15] = '1';
    prefix_table[5][7].prefix_bits[16] = '\0';
    prefix_table[5][7].tam_total = 23;

    prefix_table[5][8].prefix_bits[0] = '1';
    prefix_table[5][8].prefix_bits[1] = '1';
    prefix_table[5][8].prefix_bits[2] = '1';
    prefix_table[5][8].prefix_bits[3] = '1';
    prefix_table[5][8].prefix_bits[4] = '1';
    prefix_table[5][8].prefix_bits[5] = '1';
    prefix_table[5][8].prefix_bits[6] = '1';
    prefix_table[5][8].prefix_bits[7] = '1';
    prefix_table[5][8].prefix_bits[8] = '1';
    prefix_table[5][8].prefix_bits[9] = '0';
    prefix_table[5][8].prefix_bits[10] = '1';
    prefix_table[5][8].prefix_bits[11] = '0';
    prefix_table[5][8].prefix_bits[12] = '0';
    prefix_table[5][8].prefix_bits[13] = '1';
    prefix_table[5][8].prefix_bits[14] = '0';
    prefix_table[5][8].prefix_bits[15] = '0';
    prefix_table[5][8].prefix_bits[16] = '\0';
    prefix_table[5][8].tam_total = 24;

    prefix_table[5][9].prefix_bits[0] = '1';
    prefix_table[5][9].prefix_bits[1] = '1';
    prefix_table[5][9].prefix_bits[2] = '1';
    prefix_table[5][9].prefix_bits[3] = '1';
    prefix_table[5][9].prefix_bits[4] = '1';
    prefix_table[5][9].prefix_bits[5] = '1';
    prefix_table[5][9].prefix_bits[6] = '1';
    prefix_table[5][9].prefix_bits[7] = '1';
    prefix_table[5][9].prefix_bits[8] = '1';
    prefix_table[5][9].prefix_bits[9] = '0';
    prefix_table[5][9].prefix_bits[10] = '1';
    prefix_table[5][9].prefix_bits[11] = '0';
    prefix_table[5][9].prefix_bits[12] = '0';
    prefix_table[5][9].prefix_bits[13] = '1';
    prefix_table[5][9].prefix_bits[14] = '0';
    prefix_table[5][9].prefix_bits[15] = '1';
    prefix_table[5][9].prefix_bits[16] = '\0';
    prefix_table[5][9].tam_total = 25;

    prefix_table[5][10].prefix_bits[0] = '1';
    prefix_table[5][10].prefix_bits[1] = '1';
    prefix_table[5][10].prefix_bits[2] = '1';
    prefix_table[5][10].prefix_bits[3] = '1';
    prefix_table[5][10].prefix_bits[4] = '1';
    prefix_table[5][10].prefix_bits[5] = '1';
    prefix_table[5][10].prefix_bits[6] = '1';
    prefix_table[5][10].prefix_bits[7] = '1';
    prefix_table[5][10].prefix_bits[8] = '1';
    prefix_table[5][10].prefix_bits[9] = '0';
    prefix_table[5][10].prefix_bits[10] = '1';
    prefix_table[5][10].prefix_bits[11] = '0';
    prefix_table[5][10].prefix_bits[12] = '0';
    prefix_table[5][10].prefix_bits[13] = '1';
    prefix_table[5][10].prefix_bits[14] = '1';
    prefix_table[5][10].prefix_bits[15] = '0';
    prefix_table[5][10].prefix_bits[16] = '\0';
    prefix_table[5][10].tam_total = 26;

    prefix_table[6][1].prefix_bits[0] = '1';
    prefix_table[6][1].prefix_bits[1] = '1';
    prefix_table[6][1].prefix_bits[2] = '1';
    prefix_table[6][1].prefix_bits[3] = '1';
    prefix_table[6][1].prefix_bits[4] = '0';
    prefix_table[6][1].prefix_bits[5] = '1';
    prefix_table[6][1].prefix_bits[6] = '1';
    prefix_table[6][1].prefix_bits[7] = '\0';
    prefix_table[6][1].tam_total = 8;

    prefix_table[6][2].prefix_bits[0] = '1';
    prefix_table[6][2].prefix_bits[1] = '1';
    prefix_table[6][2].prefix_bits[2] = '1';
    prefix_table[6][2].prefix_bits[3] = '1';
    prefix_table[6][2].prefix_bits[4] = '1';
    prefix_table[6][2].prefix_bits[5] = '1';
    prefix_table[6][2].prefix_bits[6] = '1';
    prefix_table[6][2].prefix_bits[7] = '1';
    prefix_table[6][2].prefix_bits[8] = '0';
    prefix_table[6][2].prefix_bits[9] = '0';
    prefix_table[6][2].prefix_bits[10] = '0';
    prefix_table[6][2].prefix_bits[11] = '\0';
    prefix_table[6][2].tam_total = 13;

    prefix_table[6][3].prefix_bits[0] = '1';
    prefix_table[6][3].prefix_bits[1] = '1';
    prefix_table[6][3].prefix_bits[2] = '1';
    prefix_table[6][3].prefix_bits[3] = '1';
    prefix_table[6][3].prefix_bits[4] = '1';
    prefix_table[6][3].prefix_bits[5] = '1';
    prefix_table[6][3].prefix_bits[6] = '1';
    prefix_table[6][3].prefix_bits[7] = '1';
    prefix_table[6][3].prefix_bits[8] = '1';
    prefix_table[6][3].prefix_bits[9] = '0';
    prefix_table[6][3].prefix_bits[10] = '1';
    prefix_table[6][3].prefix_bits[11] = '0';
    prefix_table[6][3].prefix_bits[12] = '0';
    prefix_table[6][3].prefix_bits[13] = '1';
    prefix_table[6][3].prefix_bits[14] = '1';
    prefix_table[6][3].prefix_bits[15] = '1';
    prefix_table[6][3].prefix_bits[16] = '\0';
    prefix_table[6][3].tam_total = 19;

    prefix_table[6][4].prefix_bits[0] = '1';
    prefix_table[6][4].prefix_bits[1] = '1';
    prefix_table[6][4].prefix_bits[2] = '1';
    prefix_table[6][4].prefix_bits[3] = '1';
    prefix_table[6][4].prefix_bits[4] = '1';
    prefix_table[6][4].prefix_bits[5] = '1';
    prefix_table[6][4].prefix_bits[6] = '1';
    prefix_table[6][4].prefix_bits[7] = '1';
    prefix_table[6][4].prefix_bits[8] = '1';
    prefix_table[6][4].prefix_bits[9] = '0';
    prefix_table[6][4].prefix_bits[10] = '1';
    prefix_table[6][4].prefix_bits[11] = '0';
    prefix_table[6][4].prefix_bits[12] = '1';
    prefix_table[6][4].prefix_bits[13] = '0';
    prefix_table[6][4].prefix_bits[14] = '0';
    prefix_table[6][4].prefix_bits[15] = '0';
    prefix_table[6][4].prefix_bits[16] = '\0';
    prefix_table[6][4].tam_total = 20;

    prefix_table[6][5].prefix_bits[0] = '1';
    prefix_table[6][5].prefix_bits[1] = '1';
    prefix_table[6][5].prefix_bits[2] = '1';
    prefix_table[6][5].prefix_bits[3] = '1';
    prefix_table[6][5].prefix_bits[4] = '1';
    prefix_table[6][5].prefix_bits[5] = '1';
    prefix_table[6][5].prefix_bits[6] = '1';
    prefix_table[6][5].prefix_bits[7] = '1';
    prefix_table[6][5].prefix_bits[8] = '1';
    prefix_table[6][5].prefix_bits[9] = '0';
    prefix_table[6][5].prefix_bits[10] = '1';
    prefix_table[6][5].prefix_bits[11] = '0';
    prefix_table[6][5].prefix_bits[12] = '1';
    prefix_table[6][5].prefix_bits[13] = '0';
    prefix_table[6][5].prefix_bits[14] = '0';
    prefix_table[6][5].prefix_bits[15] = '1';
    prefix_table[6][5].prefix_bits[16] = '\0';
    prefix_table[6][5].tam_total = 21;

    prefix_table[6][6].prefix_bits[0] = '1';
    prefix_table[6][6].prefix_bits[1] = '1';
    prefix_table[6][6].prefix_bits[2] = '1';
    prefix_table[6][6].prefix_bits[3] = '1';
    prefix_table[6][6].prefix_bits[4] = '1';
    prefix_table[6][6].prefix_bits[5] = '1';
    prefix_table[6][6].prefix_bits[6] = '1';
    prefix_table[6][6].prefix_bits[7] = '1';
    prefix_table[6][6].prefix_bits[8] = '1';
    prefix_table[6][6].prefix_bits[9] = '0';
    prefix_table[6][6].prefix_bits[10] = '1';
    prefix_table[6][6].prefix_bits[11] = '0';
    prefix_table[6][6].prefix_bits[12] = '1';
    prefix_table[6][6].prefix_bits[13] = '0';
    prefix_table[6][6].prefix_bits[14] = '1';
    prefix_table[6][6].prefix_bits[15] = '0';
    prefix_table[6][6].prefix_bits[16] = '\0';
    prefix_table[6][6].tam_total = 22;

    prefix_table[6][7].prefix_bits[0] = '1';
    prefix_table[6][7].prefix_bits[1] = '1';
    prefix_table[6][7].prefix_bits[2] = '1';
    prefix_table[6][7].prefix_bits[3] = '1';
    prefix_table[6][7].prefix_bits[4] = '1';
    prefix_table[6][7].prefix_bits[5] = '1';
    prefix_table[6][7].prefix_bits[6] = '1';
    prefix_table[6][7].prefix_bits[7] = '1';
    prefix_table[6][7].prefix_bits[8] = '1';
    prefix_table[6][7].prefix_bits[9] = '0';
    prefix_table[6][7].prefix_bits[10] = '1';
    prefix_table[6][7].prefix_bits[11] = '0';
    prefix_table[6][7].prefix_bits[12] = '1';
    prefix_table[6][7].prefix_bits[13] = '0';
    prefix_table[6][7].prefix_bits[14] = '1';
    prefix_table[6][7].prefix_bits[15] = '1';
    prefix_table[6][7].prefix_bits[16] = '\0';
    prefix_table[6][7].tam_total = 23;

    prefix_table[6][8].prefix_bits[0] = '1';
    prefix_table[6][8].prefix_bits[1] = '1';
    prefix_table[6][8].prefix_bits[2] = '1';
    prefix_table[6][8].prefix_bits[3] = '1';
    prefix_table[6][8].prefix_bits[4] = '1';
    prefix_table[6][8].prefix_bits[5] = '1';
    prefix_table[6][8].prefix_bits[6] = '1';
    prefix_table[6][8].prefix_bits[7] = '1';
    prefix_table[6][8].prefix_bits[8] = '1';
    prefix_table[6][8].prefix_bits[9] = '0';
    prefix_table[6][8].prefix_bits[10] = '1';
    prefix_table[6][8].prefix_bits[11] = '0';
    prefix_table[6][8].prefix_bits[12] = '1';
    prefix_table[6][8].prefix_bits[13] = '1';
    prefix_table[6][8].prefix_bits[14] = '0';
    prefix_table[6][8].prefix_bits[15] = '0';
    prefix_table[6][8].prefix_bits[16] = '\0';
    prefix_table[6][8].tam_total = 24;

    prefix_table[6][9].prefix_bits[0] = '1';
    prefix_table[6][9].prefix_bits[1] = '1';
    prefix_table[6][9].prefix_bits[2] = '1';
    prefix_table[6][9].prefix_bits[3] = '1';
    prefix_table[6][9].prefix_bits[4] = '1';
    prefix_table[6][9].prefix_bits[5] = '1';
    prefix_table[6][9].prefix_bits[6] = '1';
    prefix_table[6][9].prefix_bits[7] = '1';
    prefix_table[6][9].prefix_bits[8] = '1';
    prefix_table[6][9].prefix_bits[9] = '0';
    prefix_table[6][9].prefix_bits[10] = '1';
    prefix_table[6][9].prefix_bits[11] = '0';
    prefix_table[6][9].prefix_bits[12] = '1';
    prefix_table[6][9].prefix_bits[13] = '1';
    prefix_table[6][9].prefix_bits[14] = '0';
    prefix_table[6][9].prefix_bits[15] = '1';
    prefix_table[6][9].prefix_bits[16] = '\0';
    prefix_table[6][9].tam_total = 25;

    prefix_table[6][10].prefix_bits[0] = '1';
    prefix_table[6][10].prefix_bits[1] = '1';
    prefix_table[6][10].prefix_bits[2] = '1';
    prefix_table[6][10].prefix_bits[3] = '1';
    prefix_table[6][10].prefix_bits[4] = '1';
    prefix_table[6][10].prefix_bits[5] = '1';
    prefix_table[6][10].prefix_bits[6] = '1';
    prefix_table[6][10].prefix_bits[7] = '1';
    prefix_table[6][10].prefix_bits[8] = '1';
    prefix_table[6][10].prefix_bits[9] = '0';
    prefix_table[6][10].prefix_bits[10] = '1';
    prefix_table[6][10].prefix_bits[11] = '0';
    prefix_table[6][10].prefix_bits[12] = '1';
    prefix_table[6][10].prefix_bits[13] = '1';
    prefix_table[6][10].prefix_bits[14] = '1';
    prefix_table[6][10].prefix_bits[15] = '0';
    prefix_table[6][10].prefix_bits[16] = '\0';
    prefix_table[6][10].tam_total = 26;

    prefix_table[7][1].prefix_bits[0] = '1';
    prefix_table[7][1].prefix_bits[1] = '1';
    prefix_table[7][1].prefix_bits[2] = '1';
    prefix_table[7][1].prefix_bits[3] = '1';
    prefix_table[7][1].prefix_bits[4] = '1';
    prefix_table[7][1].prefix_bits[5] = '0';
    prefix_table[7][1].prefix_bits[6] = '0';
    prefix_table[7][1].prefix_bits[7] = '1';
    prefix_table[7][1].prefix_bits[8] = '\0';
    prefix_table[7][1].tam_total = 9;

    prefix_table[7][2].prefix_bits[0] = '1';
    prefix_table[7][2].prefix_bits[1] = '1';
    prefix_table[7][2].prefix_bits[2] = '1';
    prefix_table[7][2].prefix_bits[3] = '1';
    prefix_table[7][2].prefix_bits[4] = '1';
    prefix_table[7][2].prefix_bits[5] = '1';
    prefix_table[7][2].prefix_bits[6] = '1';
    prefix_table[7][2].prefix_bits[7] = '1';
    prefix_table[7][2].prefix_bits[8] = '0';
    prefix_table[7][2].prefix_bits[9] = '0';
    prefix_table[7][2].prefix_bits[10] = '1';
    prefix_table[7][2].prefix_bits[11] = '\0';
    prefix_table[7][2].tam_total = 13;

    prefix_table[7][3].prefix_bits[0] = '1';
    prefix_table[7][3].prefix_bits[1] = '1';
    prefix_table[7][3].prefix_bits[2] = '1';
    prefix_table[7][3].prefix_bits[3] = '1';
    prefix_table[7][3].prefix_bits[4] = '1';
    prefix_table[7][3].prefix_bits[5] = '1';
    prefix_table[7][3].prefix_bits[6] = '1';
    prefix_table[7][3].prefix_bits[7] = '1';
    prefix_table[7][3].prefix_bits[8] = '1';
    prefix_table[7][3].prefix_bits[9] = '0';
    prefix_table[7][3].prefix_bits[10] = '1';
    prefix_table[7][3].prefix_bits[11] = '0';
    prefix_table[7][3].prefix_bits[12] = '1';
    prefix_table[7][3].prefix_bits[13] = '1';
    prefix_table[7][3].prefix_bits[14] = '1';
    prefix_table[7][3].prefix_bits[15] = '1';
    prefix_table[7][3].prefix_bits[16] = '\0';
    prefix_table[7][3].tam_total = 19;

    prefix_table[7][4].prefix_bits[0] = '1';
    prefix_table[7][4].prefix_bits[1] = '1';
    prefix_table[7][4].prefix_bits[2] = '1';
    prefix_table[7][4].prefix_bits[3] = '1';
    prefix_table[7][4].prefix_bits[4] = '1';
    prefix_table[7][4].prefix_bits[5] = '1';
    prefix_table[7][4].prefix_bits[6] = '1';
    prefix_table[7][4].prefix_bits[7] = '1';
    prefix_table[7][4].prefix_bits[8] = '1';
    prefix_table[7][4].prefix_bits[9] = '0';
    prefix_table[7][4].prefix_bits[10] = '1';
    prefix_table[7][4].prefix_bits[11] = '1';
    prefix_table[7][4].prefix_bits[12] = '0';
    prefix_table[7][4].prefix_bits[13] = '0';
    prefix_table[7][4].prefix_bits[14] = '0';
    prefix_table[7][4].prefix_bits[15] = '0';
    prefix_table[7][4].prefix_bits[16] = '\0';
    prefix_table[7][4].tam_total = 20;

    prefix_table[7][5].prefix_bits[0] = '1';
    prefix_table[7][5].prefix_bits[1] = '1';
    prefix_table[7][5].prefix_bits[2] = '1';
    prefix_table[7][5].prefix_bits[3] = '1';
    prefix_table[7][5].prefix_bits[4] = '1';
    prefix_table[7][5].prefix_bits[5] = '1';
    prefix_table[7][5].prefix_bits[6] = '1';
    prefix_table[7][5].prefix_bits[7] = '1';
    prefix_table[7][5].prefix_bits[8] = '1';
    prefix_table[7][5].prefix_bits[9] = '0';
    prefix_table[7][5].prefix_bits[10] = '1';
    prefix_table[7][5].prefix_bits[11] = '1';
    prefix_table[7][5].prefix_bits[12] = '0';
    prefix_table[7][5].prefix_bits[13] = '0';
    prefix_table[7][5].prefix_bits[14] = '0';
    prefix_table[7][5].prefix_bits[15] = '1';
    prefix_table[7][5].prefix_bits[16] = '\0';
    prefix_table[7][5].tam_total = 21;

    prefix_table[7][6].prefix_bits[0] = '1';
    prefix_table[7][6].prefix_bits[1] = '1';
    prefix_table[7][6].prefix_bits[2] = '1';
    prefix_table[7][6].prefix_bits[3] = '1';
    prefix_table[7][6].prefix_bits[4] = '1';
    prefix_table[7][6].prefix_bits[5] = '1';
    prefix_table[7][6].prefix_bits[6] = '1';
    prefix_table[7][6].prefix_bits[7] = '1';
    prefix_table[7][6].prefix_bits[8] = '1';
    prefix_table[7][6].prefix_bits[9] = '0';
    prefix_table[7][6].prefix_bits[10] = '1';
    prefix_table[7][6].prefix_bits[11] = '1';
    prefix_table[7][6].prefix_bits[12] = '0';
    prefix_table[7][6].prefix_bits[13] = '0';
    prefix_table[7][6].prefix_bits[14] = '1';
    prefix_table[7][6].prefix_bits[15] = '0';
    prefix_table[7][6].prefix_bits[16] = '\0';
    prefix_table[7][6].tam_total = 22;

    prefix_table[7][7].prefix_bits[0] = '1';
    prefix_table[7][7].prefix_bits[1] = '1';
    prefix_table[7][7].prefix_bits[2] = '1';
    prefix_table[7][7].prefix_bits[3] = '1';
    prefix_table[7][7].prefix_bits[4] = '1';
    prefix_table[7][7].prefix_bits[5] = '1';
    prefix_table[7][7].prefix_bits[6] = '1';
    prefix_table[7][7].prefix_bits[7] = '1';
    prefix_table[7][7].prefix_bits[8] = '1';
    prefix_table[7][7].prefix_bits[9] = '0';
    prefix_table[7][7].prefix_bits[10] = '1';
    prefix_table[7][7].prefix_bits[11] = '1';
    prefix_table[7][7].prefix_bits[12] = '0';
    prefix_table[7][7].prefix_bits[13] = '0';
    prefix_table[7][7].prefix_bits[14] = '1';
    prefix_table[7][7].prefix_bits[15] = '1';
    prefix_table[7][7].prefix_bits[16] = '\0';
    prefix_table[7][7].tam_total = 23;

    prefix_table[7][8].prefix_bits[0] = '1';
    prefix_table[7][8].prefix_bits[1] = '1';
    prefix_table[7][8].prefix_bits[2] = '1';
    prefix_table[7][8].prefix_bits[3] = '1';
    prefix_table[7][8].prefix_bits[4] = '1';
    prefix_table[7][8].prefix_bits[5] = '1';
    prefix_table[7][8].prefix_bits[6] = '1';
    prefix_table[7][8].prefix_bits[7] = '1';
    prefix_table[7][8].prefix_bits[8] = '1';
    prefix_table[7][8].prefix_bits[9] = '0';
    prefix_table[7][8].prefix_bits[10] = '1';
    prefix_table[7][8].prefix_bits[11] = '1';
    prefix_table[7][8].prefix_bits[12] = '0';
    prefix_table[7][8].prefix_bits[13] = '1';
    prefix_table[7][8].prefix_bits[14] = '0';
    prefix_table[7][8].prefix_bits[15] = '0';
    prefix_table[7][8].prefix_bits[16] = '\0';
    prefix_table[7][8].tam_total = 24;

    prefix_table[7][9].prefix_bits[0] = '1';
    prefix_table[7][9].prefix_bits[1] = '1';
    prefix_table[7][9].prefix_bits[2] = '1';
    prefix_table[7][9].prefix_bits[3] = '1';
    prefix_table[7][9].prefix_bits[4] = '1';
    prefix_table[7][9].prefix_bits[5] = '1';
    prefix_table[7][9].prefix_bits[6] = '1';
    prefix_table[7][9].prefix_bits[7] = '1';
    prefix_table[7][9].prefix_bits[8] = '1';
    prefix_table[7][9].prefix_bits[9] = '0';
    prefix_table[7][9].prefix_bits[10] = '1';
    prefix_table[7][9].prefix_bits[11] = '1';
    prefix_table[7][9].prefix_bits[12] = '0';
    prefix_table[7][9].prefix_bits[13] = '1';
    prefix_table[7][9].prefix_bits[14] = '0';
    prefix_table[7][9].prefix_bits[15] = '1';
    prefix_table[7][9].prefix_bits[16] = '\0';
    prefix_table[7][9].tam_total = 25;

    prefix_table[7][10].prefix_bits[0] = '1';
    prefix_table[7][10].prefix_bits[1] = '1';
    prefix_table[7][10].prefix_bits[2] = '1';
    prefix_table[7][10].prefix_bits[3] = '1';
    prefix_table[7][10].prefix_bits[4] = '1';
    prefix_table[7][10].prefix_bits[5] = '1';
    prefix_table[7][10].prefix_bits[6] = '1';
    prefix_table[7][10].prefix_bits[7] = '1';
    prefix_table[7][10].prefix_bits[8] = '1';
    prefix_table[7][10].prefix_bits[9] = '0';
    prefix_table[7][10].prefix_bits[10] = '1';
    prefix_table[7][10].prefix_bits[11] = '1';
    prefix_table[7][10].prefix_bits[12] = '0';
    prefix_table[7][10].prefix_bits[13] = '1';
    prefix_table[7][10].prefix_bits[14] = '1';
    prefix_table[7][10].prefix_bits[15] = '0';
    prefix_table[7][10].prefix_bits[16] = '\0';
    prefix_table[7][10].tam_total = 26;

    prefix_table[8][1].prefix_bits[0] = '1';
    prefix_table[8][1].prefix_bits[1] = '1';
    prefix_table[8][1].prefix_bits[2] = '1';
    prefix_table[8][1].prefix_bits[3] = '1';
    prefix_table[8][1].prefix_bits[4] = '1';
    prefix_table[8][1].prefix_bits[5] = '0';
    prefix_table[8][1].prefix_bits[6] = '1';
    prefix_table[8][1].prefix_bits[7] = '0';
    prefix_table[8][1].prefix_bits[8] = '\0';
    prefix_table[8][1].tam_total = 9;

    prefix_table[8][2].prefix_bits[0] = '1';
    prefix_table[8][2].prefix_bits[1] = '1';
    prefix_table[8][2].prefix_bits[2] = '1';
    prefix_table[8][2].prefix_bits[3] = '1';
    prefix_table[8][2].prefix_bits[4] = '1';
    prefix_table[8][2].prefix_bits[5] = '1';
    prefix_table[8][2].prefix_bits[6] = '1';
    prefix_table[8][2].prefix_bits[7] = '1';
    prefix_table[8][2].prefix_bits[8] = '1';
    prefix_table[8][2].prefix_bits[9] = '0';
    prefix_table[8][2].prefix_bits[10] = '0';
    prefix_table[8][2].prefix_bits[11] = '0';
    prefix_table[8][2].prefix_bits[12] = '0';
    prefix_table[8][2].prefix_bits[13] = '0';
    prefix_table[8][2].prefix_bits[14] = '0';
    prefix_table[8][2].prefix_bits[15] = '\0';
    prefix_table[8][2].tam_total = 17;

    prefix_table[8][3].prefix_bits[0] = '1';
    prefix_table[8][3].prefix_bits[1] = '1';
    prefix_table[8][3].prefix_bits[2] = '1';
    prefix_table[8][3].prefix_bits[3] = '1';
    prefix_table[8][3].prefix_bits[4] = '1';
    prefix_table[8][3].prefix_bits[5] = '1';
    prefix_table[8][3].prefix_bits[6] = '1';
    prefix_table[8][3].prefix_bits[7] = '1';
    prefix_table[8][3].prefix_bits[8] = '1';
    prefix_table[8][3].prefix_bits[9] = '0';
    prefix_table[8][3].prefix_bits[10] = '1';
    prefix_table[8][3].prefix_bits[11] = '1';
    prefix_table[8][3].prefix_bits[12] = '0';
    prefix_table[8][3].prefix_bits[13] = '1';
    prefix_table[8][3].prefix_bits[14] = '1';
    prefix_table[8][3].prefix_bits[15] = '1';
    prefix_table[8][3].prefix_bits[16] = '\0';
    prefix_table[8][3].tam_total = 19;

    prefix_table[8][4].prefix_bits[0] = '1';
    prefix_table[8][4].prefix_bits[1] = '1';
    prefix_table[8][4].prefix_bits[2] = '1';
    prefix_table[8][4].prefix_bits[3] = '1';
    prefix_table[8][4].prefix_bits[4] = '1';
    prefix_table[8][4].prefix_bits[5] = '1';
    prefix_table[8][4].prefix_bits[6] = '1';
    prefix_table[8][4].prefix_bits[7] = '1';
    prefix_table[8][4].prefix_bits[8] = '1';
    prefix_table[8][4].prefix_bits[9] = '0';
    prefix_table[8][4].prefix_bits[10] = '1';
    prefix_table[8][4].prefix_bits[11] = '1';
    prefix_table[8][4].prefix_bits[12] = '1';
    prefix_table[8][4].prefix_bits[13] = '0';
    prefix_table[8][4].prefix_bits[14] = '0';
    prefix_table[8][4].prefix_bits[15] = '0';
    prefix_table[8][4].prefix_bits[16] = '\0';
    prefix_table[8][4].tam_total = 20;

    prefix_table[8][5].prefix_bits[0] = '1';
    prefix_table[8][5].prefix_bits[1] = '1';
    prefix_table[8][5].prefix_bits[2] = '1';
    prefix_table[8][5].prefix_bits[3] = '1';
    prefix_table[8][5].prefix_bits[4] = '1';
    prefix_table[8][5].prefix_bits[5] = '1';
    prefix_table[8][5].prefix_bits[6] = '1';
    prefix_table[8][5].prefix_bits[7] = '1';
    prefix_table[8][5].prefix_bits[8] = '1';
    prefix_table[8][5].prefix_bits[9] = '0';
    prefix_table[8][5].prefix_bits[10] = '1';
    prefix_table[8][5].prefix_bits[11] = '1';
    prefix_table[8][5].prefix_bits[12] = '1';
    prefix_table[8][5].prefix_bits[13] = '0';
    prefix_table[8][5].prefix_bits[14] = '0';
    prefix_table[8][5].prefix_bits[15] = '1';
    prefix_table[8][5].prefix_bits[16] = '\0';
    prefix_table[8][5].tam_total = 21;

    prefix_table[8][6].prefix_bits[0] = '1';
    prefix_table[8][6].prefix_bits[1] = '1';
    prefix_table[8][6].prefix_bits[2] = '1';
    prefix_table[8][6].prefix_bits[3] = '1';
    prefix_table[8][6].prefix_bits[4] = '1';
    prefix_table[8][6].prefix_bits[5] = '1';
    prefix_table[8][6].prefix_bits[6] = '1';
    prefix_table[8][6].prefix_bits[7] = '1';
    prefix_table[8][6].prefix_bits[8] = '1';
    prefix_table[8][6].prefix_bits[9] = '0';
    prefix_table[8][6].prefix_bits[10] = '1';
    prefix_table[8][6].prefix_bits[11] = '1';
    prefix_table[8][6].prefix_bits[12] = '1';
    prefix_table[8][6].prefix_bits[13] = '0';
    prefix_table[8][6].prefix_bits[14] = '1';
    prefix_table[8][6].prefix_bits[15] = '0';
    prefix_table[8][6].prefix_bits[16] = '\0';
    prefix_table[8][6].tam_total = 22;

    prefix_table[8][7].prefix_bits[0] = '1';
    prefix_table[8][7].prefix_bits[1] = '1';
    prefix_table[8][7].prefix_bits[2] = '1';
    prefix_table[8][7].prefix_bits[3] = '1';
    prefix_table[8][7].prefix_bits[4] = '1';
    prefix_table[8][7].prefix_bits[5] = '1';
    prefix_table[8][7].prefix_bits[6] = '1';
    prefix_table[8][7].prefix_bits[7] = '1';
    prefix_table[8][7].prefix_bits[8] = '1';
    prefix_table[8][7].prefix_bits[9] = '0';
    prefix_table[8][7].prefix_bits[10] = '1';
    prefix_table[8][7].prefix_bits[11] = '1';
    prefix_table[8][7].prefix_bits[12] = '1';
    prefix_table[8][7].prefix_bits[13] = '0';
    prefix_table[8][7].prefix_bits[14] = '1';
    prefix_table[8][7].prefix_bits[15] = '1';
    prefix_table[8][7].prefix_bits[16] = '\0';
    prefix_table[8][7].tam_total = 23;

    prefix_table[8][8].prefix_bits[0] = '1';
    prefix_table[8][8].prefix_bits[1] = '1';
    prefix_table[8][8].prefix_bits[2] = '1';
    prefix_table[8][8].prefix_bits[3] = '1';
    prefix_table[8][8].prefix_bits[4] = '1';
    prefix_table[8][8].prefix_bits[5] = '1';
    prefix_table[8][8].prefix_bits[6] = '1';
    prefix_table[8][8].prefix_bits[7] = '1';
    prefix_table[8][8].prefix_bits[8] = '1';
    prefix_table[8][8].prefix_bits[9] = '0';
    prefix_table[8][8].prefix_bits[10] = '1';
    prefix_table[8][8].prefix_bits[11] = '1';
    prefix_table[8][8].prefix_bits[12] = '1';
    prefix_table[8][8].prefix_bits[13] = '1';
    prefix_table[8][8].prefix_bits[14] = '0';
    prefix_table[8][8].prefix_bits[15] = '0';
    prefix_table[8][8].prefix_bits[16] = '\0';
    prefix_table[8][8].tam_total = 24;

    prefix_table[8][9].prefix_bits[0] = '1';
    prefix_table[8][9].prefix_bits[1] = '1';
    prefix_table[8][9].prefix_bits[2] = '1';
    prefix_table[8][9].prefix_bits[3] = '1';
    prefix_table[8][9].prefix_bits[4] = '1';
    prefix_table[8][9].prefix_bits[5] = '1';
    prefix_table[8][9].prefix_bits[6] = '1';
    prefix_table[8][9].prefix_bits[7] = '1';
    prefix_table[8][9].prefix_bits[8] = '1';
    prefix_table[8][9].prefix_bits[9] = '0';
    prefix_table[8][9].prefix_bits[10] = '1';
    prefix_table[8][9].prefix_bits[11] = '1';
    prefix_table[8][9].prefix_bits[12] = '1';
    prefix_table[8][9].prefix_bits[13] = '1';
    prefix_table[8][9].prefix_bits[14] = '0';
    prefix_table[8][9].prefix_bits[15] = '1';
    prefix_table[8][9].prefix_bits[16] = '\0';
    prefix_table[8][9].tam_total = 25;

    prefix_table[8][10].prefix_bits[0] = '1';
    prefix_table[8][10].prefix_bits[1] = '1';
    prefix_table[8][10].prefix_bits[2] = '1';
    prefix_table[8][10].prefix_bits[3] = '1';
    prefix_table[8][10].prefix_bits[4] = '1';
    prefix_table[8][10].prefix_bits[5] = '1';
    prefix_table[8][10].prefix_bits[6] = '1';
    prefix_table[8][10].prefix_bits[7] = '1';
    prefix_table[8][10].prefix_bits[8] = '1';
    prefix_table[8][10].prefix_bits[9] = '0';
    prefix_table[8][10].prefix_bits[10] = '1';
    prefix_table[8][10].prefix_bits[11] = '1';
    prefix_table[8][10].prefix_bits[12] = '1';
    prefix_table[8][10].prefix_bits[13] = '1';
    prefix_table[8][10].prefix_bits[14] = '1';
    prefix_table[8][10].prefix_bits[15] = '0';
    prefix_table[8][10].prefix_bits[16] = '\0';
    prefix_table[8][10].tam_total = 26;

    prefix_table[9][1].prefix_bits[0] = '1';
    prefix_table[9][1].prefix_bits[1] = '1';
    prefix_table[9][1].prefix_bits[2] = '1';
    prefix_table[9][1].prefix_bits[3] = '1';
    prefix_table[9][1].prefix_bits[4] = '1';
    prefix_table[9][1].prefix_bits[5] = '1';
    prefix_table[9][1].prefix_bits[6] = '0';
    prefix_table[9][1].prefix_bits[7] = '0';
    prefix_table[9][1].prefix_bits[8] = '0';
    prefix_table[9][1].prefix_bits[9] = '\0';
    prefix_table[9][1].tam_total = 10;

    prefix_table[9][2].prefix_bits[0] = '1';
    prefix_table[9][2].prefix_bits[1] = '1';
    prefix_table[9][2].prefix_bits[2] = '1';
    prefix_table[9][2].prefix_bits[3] = '1';
    prefix_table[9][2].prefix_bits[4] = '1';
    prefix_table[9][2].prefix_bits[5] = '1';
    prefix_table[9][2].prefix_bits[6] = '1';
    prefix_table[9][2].prefix_bits[7] = '1';
    prefix_table[9][2].prefix_bits[8] = '1';
    prefix_table[9][2].prefix_bits[9] = '0';
    prefix_table[9][2].prefix_bits[10] = '1';
    prefix_table[9][2].prefix_bits[11] = '1';
    prefix_table[9][2].prefix_bits[12] = '1';
    prefix_table[9][2].prefix_bits[13] = '1';
    prefix_table[9][2].prefix_bits[14] = '1';
    prefix_table[9][2].prefix_bits[15] = '1';
    prefix_table[9][2].prefix_bits[16] = '\0';
    prefix_table[9][2].tam_total = 18;

    prefix_table[9][3].prefix_bits[0] = '1';
    prefix_table[9][3].prefix_bits[1] = '1';
    prefix_table[9][3].prefix_bits[2] = '1';
    prefix_table[9][3].prefix_bits[3] = '1';
    prefix_table[9][3].prefix_bits[4] = '1';
    prefix_table[9][3].prefix_bits[5] = '1';
    prefix_table[9][3].prefix_bits[6] = '1';
    prefix_table[9][3].prefix_bits[7] = '1';
    prefix_table[9][3].prefix_bits[8] = '1';
    prefix_table[9][3].prefix_bits[9] = '1';
    prefix_table[9][3].prefix_bits[10] = '0';
    prefix_table[9][3].prefix_bits[11] = '0';
    prefix_table[9][3].prefix_bits[12] = '0';
    prefix_table[9][3].prefix_bits[13] = '0';
    prefix_table[9][3].prefix_bits[14] = '0';
    prefix_table[9][3].prefix_bits[15] = '0';
    prefix_table[9][3].prefix_bits[16] = '\0';
    prefix_table[9][3].tam_total = 19;

    prefix_table[9][4].prefix_bits[0] = '1';
    prefix_table[9][4].prefix_bits[1] = '1';
    prefix_table[9][4].prefix_bits[2] = '1';
    prefix_table[9][4].prefix_bits[3] = '1';
    prefix_table[9][4].prefix_bits[4] = '1';
    prefix_table[9][4].prefix_bits[5] = '1';
    prefix_table[9][4].prefix_bits[6] = '1';
    prefix_table[9][4].prefix_bits[7] = '1';
    prefix_table[9][4].prefix_bits[8] = '1';
    prefix_table[9][4].prefix_bits[9] = '1';
    prefix_table[9][4].prefix_bits[10] = '0';
    prefix_table[9][4].prefix_bits[11] = '0';
    prefix_table[9][4].prefix_bits[12] = '0';
    prefix_table[9][4].prefix_bits[13] = '0';
    prefix_table[9][4].prefix_bits[14] = '0';
    prefix_table[9][4].prefix_bits[15] = '1';
    prefix_table[9][4].prefix_bits[16] = '\0';
    prefix_table[9][4].tam_total = 20;

    prefix_table[9][5].prefix_bits[0] = '1';
    prefix_table[9][5].prefix_bits[1] = '1';
    prefix_table[9][5].prefix_bits[2] = '1';
    prefix_table[9][5].prefix_bits[3] = '1';
    prefix_table[9][5].prefix_bits[4] = '1';
    prefix_table[9][5].prefix_bits[5] = '1';
    prefix_table[9][5].prefix_bits[6] = '1';
    prefix_table[9][5].prefix_bits[7] = '1';
    prefix_table[9][5].prefix_bits[8] = '1';
    prefix_table[9][5].prefix_bits[9] = '1';
    prefix_table[9][5].prefix_bits[10] = '0';
    prefix_table[9][5].prefix_bits[11] = '0';
    prefix_table[9][5].prefix_bits[12] = '0';
    prefix_table[9][5].prefix_bits[13] = '0';
    prefix_table[9][5].prefix_bits[14] = '1';
    prefix_table[9][5].prefix_bits[15] = '0';
    prefix_table[9][5].prefix_bits[16] = '\0';
    prefix_table[9][5].tam_total = 21;

    prefix_table[9][6].prefix_bits[0] = '1';
    prefix_table[9][6].prefix_bits[1] = '1';
    prefix_table[9][6].prefix_bits[2] = '1';
    prefix_table[9][6].prefix_bits[3] = '1';
    prefix_table[9][6].prefix_bits[4] = '1';
    prefix_table[9][6].prefix_bits[5] = '1';
    prefix_table[9][6].prefix_bits[6] = '1';
    prefix_table[9][6].prefix_bits[7] = '1';
    prefix_table[9][6].prefix_bits[8] = '1';
    prefix_table[9][6].prefix_bits[9] = '1';
    prefix_table[9][6].prefix_bits[10] = '0';
    prefix_table[9][6].prefix_bits[11] = '0';
    prefix_table[9][6].prefix_bits[12] = '0';
    prefix_table[9][6].prefix_bits[13] = '0';
    prefix_table[9][6].prefix_bits[14] = '1';
    prefix_table[9][6].prefix_bits[15] = '1';
    prefix_table[9][6].prefix_bits[16] = '\0';
    prefix_table[9][6].tam_total = 22;

    prefix_table[9][7].prefix_bits[0] = '1';
    prefix_table[9][7].prefix_bits[1] = '1';
    prefix_table[9][7].prefix_bits[2] = '1';
    prefix_table[9][7].prefix_bits[3] = '1';
    prefix_table[9][7].prefix_bits[4] = '1';
    prefix_table[9][7].prefix_bits[5] = '1';
    prefix_table[9][7].prefix_bits[6] = '1';
    prefix_table[9][7].prefix_bits[7] = '1';
    prefix_table[9][7].prefix_bits[8] = '1';
    prefix_table[9][7].prefix_bits[9] = '1';
    prefix_table[9][7].prefix_bits[10] = '0';
    prefix_table[9][7].prefix_bits[11] = '0';
    prefix_table[9][7].prefix_bits[12] = '0';
    prefix_table[9][7].prefix_bits[13] = '1';
    prefix_table[9][7].prefix_bits[14] = '0';
    prefix_table[9][7].prefix_bits[15] = '0';
    prefix_table[9][7].prefix_bits[16] = '\0';
    prefix_table[9][7].tam_total = 23;

    prefix_table[9][8].prefix_bits[0] = '1';
    prefix_table[9][8].prefix_bits[1] = '1';
    prefix_table[9][8].prefix_bits[2] = '1';
    prefix_table[9][8].prefix_bits[3] = '1';
    prefix_table[9][8].prefix_bits[4] = '1';
    prefix_table[9][8].prefix_bits[5] = '1';
    prefix_table[9][8].prefix_bits[6] = '1';
    prefix_table[9][8].prefix_bits[7] = '1';
    prefix_table[9][8].prefix_bits[8] = '1';
    prefix_table[9][8].prefix_bits[9] = '1';
    prefix_table[9][8].prefix_bits[10] = '0';
    prefix_table[9][8].prefix_bits[11] = '0';
    prefix_table[9][8].prefix_bits[12] = '0';
    prefix_table[9][8].prefix_bits[13] = '1';
    prefix_table[9][8].prefix_bits[14] = '0';
    prefix_table[9][8].prefix_bits[15] = '1';
    prefix_table[9][8].prefix_bits[16] = '\0';
    prefix_table[9][8].tam_total = 24;

    prefix_table[9][9].prefix_bits[0] = '1';
    prefix_table[9][9].prefix_bits[1] = '1';
    prefix_table[9][9].prefix_bits[2] = '1';
    prefix_table[9][9].prefix_bits[3] = '1';
    prefix_table[9][9].prefix_bits[4] = '1';
    prefix_table[9][9].prefix_bits[5] = '1';
    prefix_table[9][9].prefix_bits[6] = '1';
    prefix_table[9][9].prefix_bits[7] = '1';
    prefix_table[9][9].prefix_bits[8] = '1';
    prefix_table[9][9].prefix_bits[9] = '1';
    prefix_table[9][9].prefix_bits[10] = '0';
    prefix_table[9][9].prefix_bits[11] = '0';
    prefix_table[9][9].prefix_bits[12] = '0';
    prefix_table[9][9].prefix_bits[13] = '1';
    prefix_table[9][9].prefix_bits[14] = '1';
    prefix_table[9][9].prefix_bits[15] = '0';
    prefix_table[9][9].prefix_bits[16] = '\0';
    prefix_table[9][9].tam_total = 25;

    prefix_table[9][10].prefix_bits[0] = '1';
    prefix_table[9][10].prefix_bits[1] = '1';
    prefix_table[9][10].prefix_bits[2] = '1';
    prefix_table[9][10].prefix_bits[3] = '1';
    prefix_table[9][10].prefix_bits[4] = '1';
    prefix_table[9][10].prefix_bits[5] = '1';
    prefix_table[9][10].prefix_bits[6] = '1';
    prefix_table[9][10].prefix_bits[7] = '1';
    prefix_table[9][10].prefix_bits[8] = '1';
    prefix_table[9][10].prefix_bits[9] = '1';
    prefix_table[9][10].prefix_bits[10] = '0';
    prefix_table[9][10].prefix_bits[11] = '0';
    prefix_table[9][10].prefix_bits[12] = '0';
    prefix_table[9][10].prefix_bits[13] = '1';
    prefix_table[9][10].prefix_bits[14] = '1';
    prefix_table[9][10].prefix_bits[15] = '1';
    prefix_table[9][10].prefix_bits[16] = '\0';
    prefix_table[9][10].tam_total = 26;

    prefix_table[10][1].prefix_bits[0] = '1';
    prefix_table[10][1].prefix_bits[1] = '1';
    prefix_table[10][1].prefix_bits[2] = '1';
    prefix_table[10][1].prefix_bits[3] = '1';
    prefix_table[10][1].prefix_bits[4] = '1';
    prefix_table[10][1].prefix_bits[5] = '1';
    prefix_table[10][1].prefix_bits[6] = '0';
    prefix_table[10][1].prefix_bits[7] = '0';
    prefix_table[10][1].prefix_bits[8] = '1';
    prefix_table[10][1].prefix_bits[9] = '\0';
    prefix_table[10][1].tam_total = 10;

    prefix_table[10][2].prefix_bits[0] = '1';
    prefix_table[10][2].prefix_bits[1] = '1';
    prefix_table[10][2].prefix_bits[2] = '1';
    prefix_table[10][2].prefix_bits[3] = '1';
    prefix_table[10][2].prefix_bits[4] = '1';
    prefix_table[10][2].prefix_bits[5] = '1';
    prefix_table[10][2].prefix_bits[6] = '1';
    prefix_table[10][2].prefix_bits[7] = '1';
    prefix_table[10][2].prefix_bits[8] = '1';
    prefix_table[10][2].prefix_bits[9] = '1';
    prefix_table[10][2].prefix_bits[10] = '0';
    prefix_table[10][2].prefix_bits[11] = '0';
    prefix_table[10][2].prefix_bits[12] = '1';
    prefix_table[10][2].prefix_bits[13] = '0';
    prefix_table[10][2].prefix_bits[14] = '0';
    prefix_table[10][2].prefix_bits[15] = '0';
    prefix_table[10][2].prefix_bits[16] = '\0';
    prefix_table[10][2].tam_total = 18;

    prefix_table[10][3].prefix_bits[0] = '1';
    prefix_table[10][3].prefix_bits[1] = '1';
    prefix_table[10][3].prefix_bits[2] = '1';
    prefix_table[10][3].prefix_bits[3] = '1';
    prefix_table[10][3].prefix_bits[4] = '1';
    prefix_table[10][3].prefix_bits[5] = '1';
    prefix_table[10][3].prefix_bits[6] = '1';
    prefix_table[10][3].prefix_bits[7] = '1';
    prefix_table[10][3].prefix_bits[8] = '1';
    prefix_table[10][3].prefix_bits[9] = '1';
    prefix_table[10][3].prefix_bits[10] = '0';
    prefix_table[10][3].prefix_bits[11] = '0';
    prefix_table[10][3].prefix_bits[12] = '1';
    prefix_table[10][3].prefix_bits[13] = '0';
    prefix_table[10][3].prefix_bits[14] = '0';
    prefix_table[10][3].prefix_bits[15] = '1';
    prefix_table[10][3].prefix_bits[16] = '\0';
    prefix_table[10][3].tam_total = 19;

    prefix_table[10][4].prefix_bits[0] = '1';
    prefix_table[10][4].prefix_bits[1] = '1';
    prefix_table[10][4].prefix_bits[2] = '1';
    prefix_table[10][4].prefix_bits[3] = '1';
    prefix_table[10][4].prefix_bits[4] = '1';
    prefix_table[10][4].prefix_bits[5] = '1';
    prefix_table[10][4].prefix_bits[6] = '1';
    prefix_table[10][4].prefix_bits[7] = '1';
    prefix_table[10][4].prefix_bits[8] = '1';
    prefix_table[10][4].prefix_bits[9] = '1';
    prefix_table[10][4].prefix_bits[10] = '0';
    prefix_table[10][4].prefix_bits[11] = '0';
    prefix_table[10][4].prefix_bits[12] = '1';
    prefix_table[10][4].prefix_bits[13] = '0';
    prefix_table[10][4].prefix_bits[14] = '1';
    prefix_table[10][4].prefix_bits[15] = '0';
    prefix_table[10][4].prefix_bits[16] = '\0';
    prefix_table[10][4].tam_total = 20;

    prefix_table[10][5].prefix_bits[0] = '1';
    prefix_table[10][5].prefix_bits[1] = '1';
    prefix_table[10][5].prefix_bits[2] = '1';
    prefix_table[10][5].prefix_bits[3] = '1';
    prefix_table[10][5].prefix_bits[4] = '1';
    prefix_table[10][5].prefix_bits[5] = '1';
    prefix_table[10][5].prefix_bits[6] = '1';
    prefix_table[10][5].prefix_bits[7] = '1';
    prefix_table[10][5].prefix_bits[8] = '1';
    prefix_table[10][5].prefix_bits[9] = '1';
    prefix_table[10][5].prefix_bits[10] = '0';
    prefix_table[10][5].prefix_bits[11] = '0';
    prefix_table[10][5].prefix_bits[12] = '1';
    prefix_table[10][5].prefix_bits[13] = '0';
    prefix_table[10][5].prefix_bits[14] = '1';
    prefix_table[10][5].prefix_bits[15] = '1';
    prefix_table[10][5].prefix_bits[16] = '\0';
    prefix_table[10][5].tam_total = 21;

    prefix_table[10][6].prefix_bits[0] = '1';
    prefix_table[10][6].prefix_bits[1] = '1';
    prefix_table[10][6].prefix_bits[2] = '1';
    prefix_table[10][6].prefix_bits[3] = '1';
    prefix_table[10][6].prefix_bits[4] = '1';
    prefix_table[10][6].prefix_bits[5] = '1';
    prefix_table[10][6].prefix_bits[6] = '1';
    prefix_table[10][6].prefix_bits[7] = '1';
    prefix_table[10][6].prefix_bits[8] = '1';
    prefix_table[10][6].prefix_bits[9] = '1';
    prefix_table[10][6].prefix_bits[10] = '0';
    prefix_table[10][6].prefix_bits[11] = '0';
    prefix_table[10][6].prefix_bits[12] = '1';
    prefix_table[10][6].prefix_bits[13] = '1';
    prefix_table[10][6].prefix_bits[14] = '0';
    prefix_table[10][6].prefix_bits[15] = '0';
    prefix_table[10][6].prefix_bits[16] = '\0';
    prefix_table[10][6].tam_total = 22;

    prefix_table[10][7].prefix_bits[0] = '1';
    prefix_table[10][7].prefix_bits[1] = '1';
    prefix_table[10][7].prefix_bits[2] = '1';
    prefix_table[10][7].prefix_bits[3] = '1';
    prefix_table[10][7].prefix_bits[4] = '1';
    prefix_table[10][7].prefix_bits[5] = '1';
    prefix_table[10][7].prefix_bits[6] = '1';
    prefix_table[10][7].prefix_bits[7] = '1';
    prefix_table[10][7].prefix_bits[8] = '1';
    prefix_table[10][7].prefix_bits[9] = '1';
    prefix_table[10][7].prefix_bits[10] = '0';
    prefix_table[10][7].prefix_bits[11] = '0';
    prefix_table[10][7].prefix_bits[12] = '1';
    prefix_table[10][7].prefix_bits[13] = '1';
    prefix_table[10][7].prefix_bits[14] = '0';
    prefix_table[10][7].prefix_bits[15] = '1';
    prefix_table[10][7].prefix_bits[16] = '\0';
    prefix_table[10][7].tam_total = 23;

    prefix_table[10][8].prefix_bits[0] = '1';
    prefix_table[10][8].prefix_bits[1] = '1';
    prefix_table[10][8].prefix_bits[2] = '1';
    prefix_table[10][8].prefix_bits[3] = '1';
    prefix_table[10][8].prefix_bits[4] = '1';
    prefix_table[10][8].prefix_bits[5] = '1';
    prefix_table[10][8].prefix_bits[6] = '1';
    prefix_table[10][8].prefix_bits[7] = '1';
    prefix_table[10][8].prefix_bits[8] = '1';
    prefix_table[10][8].prefix_bits[9] = '1';
    prefix_table[10][8].prefix_bits[10] = '0';
    prefix_table[10][8].prefix_bits[11] = '0';
    prefix_table[10][8].prefix_bits[12] = '1';
    prefix_table[10][8].prefix_bits[13] = '1';
    prefix_table[10][8].prefix_bits[14] = '1';
    prefix_table[10][8].prefix_bits[15] = '0';
    prefix_table[10][8].prefix_bits[16] = '\0';
    prefix_table[10][8].tam_total = 24;

    prefix_table[10][9].prefix_bits[0] = '1';
    prefix_table[10][9].prefix_bits[1] = '1';
    prefix_table[10][9].prefix_bits[2] = '1';
    prefix_table[10][9].prefix_bits[3] = '1';
    prefix_table[10][9].prefix_bits[4] = '1';
    prefix_table[10][9].prefix_bits[5] = '1';
    prefix_table[10][9].prefix_bits[6] = '1';
    prefix_table[10][9].prefix_bits[7] = '1';
    prefix_table[10][9].prefix_bits[8] = '1';
    prefix_table[10][9].prefix_bits[9] = '1';
    prefix_table[10][9].prefix_bits[10] = '0';
    prefix_table[10][9].prefix_bits[11] = '0';
    prefix_table[10][9].prefix_bits[12] = '1';
    prefix_table[10][9].prefix_bits[13] = '1';
    prefix_table[10][9].prefix_bits[14] = '1';
    prefix_table[10][9].prefix_bits[15] = '1';
    prefix_table[10][9].prefix_bits[16] = '\0';
    prefix_table[10][9].tam_total = 25;

    prefix_table[10][10].prefix_bits[0] = '1';
    prefix_table[10][10].prefix_bits[1] = '1';
    prefix_table[10][10].prefix_bits[2] = '1';
    prefix_table[10][10].prefix_bits[3] = '1';
    prefix_table[10][10].prefix_bits[4] = '1';
    prefix_table[10][10].prefix_bits[5] = '1';
    prefix_table[10][10].prefix_bits[6] = '1';
    prefix_table[10][10].prefix_bits[7] = '1';
    prefix_table[10][10].prefix_bits[8] = '1';
    prefix_table[10][10].prefix_bits[9] = '1';
    prefix_table[10][10].prefix_bits[10] = '0';
    prefix_table[10][10].prefix_bits[11] = '1';
    prefix_table[10][10].prefix_bits[12] = '0';
    prefix_table[10][10].prefix_bits[13] = '0';
    prefix_table[10][10].prefix_bits[14] = '0';
    prefix_table[10][10].prefix_bits[15] = '0';
    prefix_table[10][10].prefix_bits[16] = '\0';
    prefix_table[10][10].tam_total = 26;

    prefix_table[11][1].prefix_bits[0] = '1';
    prefix_table[11][1].prefix_bits[1] = '1';
    prefix_table[11][1].prefix_bits[2] = '1';
    prefix_table[11][1].prefix_bits[3] = '1';
    prefix_table[11][1].prefix_bits[4] = '1';
    prefix_table[11][1].prefix_bits[5] = '1';
    prefix_table[11][1].prefix_bits[6] = '0';
    prefix_table[11][1].prefix_bits[7] = '1';
    prefix_table[11][1].prefix_bits[8] = '0';
    prefix_table[11][1].prefix_bits[9] = '\0';
    prefix_table[11][1].tam_total = 10;

    prefix_table[11][2].prefix_bits[0] = '1';
    prefix_table[11][2].prefix_bits[1] = '1';
    prefix_table[11][2].prefix_bits[2] = '1';
    prefix_table[11][2].prefix_bits[3] = '1';
    prefix_table[11][2].prefix_bits[4] = '1';
    prefix_table[11][2].prefix_bits[5] = '1';
    prefix_table[11][2].prefix_bits[6] = '1';
    prefix_table[11][2].prefix_bits[7] = '1';
    prefix_table[11][2].prefix_bits[8] = '1';
    prefix_table[11][2].prefix_bits[9] = '1';
    prefix_table[11][2].prefix_bits[10] = '0';
    prefix_table[11][2].prefix_bits[11] = '1';
    prefix_table[11][2].prefix_bits[12] = '0';
    prefix_table[11][2].prefix_bits[13] = '0';
    prefix_table[11][2].prefix_bits[14] = '0';
    prefix_table[11][2].prefix_bits[15] = '1';
    prefix_table[11][2].prefix_bits[16] = '\0';
    prefix_table[11][2].tam_total = 18;

    prefix_table[11][3].prefix_bits[0] = '1';
    prefix_table[11][3].prefix_bits[1] = '1';
    prefix_table[11][3].prefix_bits[2] = '1';
    prefix_table[11][3].prefix_bits[3] = '1';
    prefix_table[11][3].prefix_bits[4] = '1';
    prefix_table[11][3].prefix_bits[5] = '1';
    prefix_table[11][3].prefix_bits[6] = '1';
    prefix_table[11][3].prefix_bits[7] = '1';
    prefix_table[11][3].prefix_bits[8] = '1';
    prefix_table[11][3].prefix_bits[9] = '1';
    prefix_table[11][3].prefix_bits[10] = '0';
    prefix_table[11][3].prefix_bits[11] = '1';
    prefix_table[11][3].prefix_bits[12] = '0';
    prefix_table[11][3].prefix_bits[13] = '0';
    prefix_table[11][3].prefix_bits[14] = '1';
    prefix_table[11][3].prefix_bits[15] = '0';
    prefix_table[11][3].prefix_bits[16] = '\0';
    prefix_table[11][3].tam_total = 19;

    prefix_table[11][4].prefix_bits[0] = '1';
    prefix_table[11][4].prefix_bits[1] = '1';
    prefix_table[11][4].prefix_bits[2] = '1';
    prefix_table[11][4].prefix_bits[3] = '1';
    prefix_table[11][4].prefix_bits[4] = '1';
    prefix_table[11][4].prefix_bits[5] = '1';
    prefix_table[11][4].prefix_bits[6] = '1';
    prefix_table[11][4].prefix_bits[7] = '1';
    prefix_table[11][4].prefix_bits[8] = '1';
    prefix_table[11][4].prefix_bits[9] = '1';
    prefix_table[11][4].prefix_bits[10] = '0';
    prefix_table[11][4].prefix_bits[11] = '1';
    prefix_table[11][4].prefix_bits[12] = '0';
    prefix_table[11][4].prefix_bits[13] = '0';
    prefix_table[11][4].prefix_bits[14] = '1';
    prefix_table[11][4].prefix_bits[15] = '1';
    prefix_table[11][4].prefix_bits[16] = '\0';
    prefix_table[11][4].tam_total = 20;

    prefix_table[11][5].prefix_bits[0] = '1';
    prefix_table[11][5].prefix_bits[1] = '1';
    prefix_table[11][5].prefix_bits[2] = '1';
    prefix_table[11][5].prefix_bits[3] = '1';
    prefix_table[11][5].prefix_bits[4] = '1';
    prefix_table[11][5].prefix_bits[5] = '1';
    prefix_table[11][5].prefix_bits[6] = '1';
    prefix_table[11][5].prefix_bits[7] = '1';
    prefix_table[11][5].prefix_bits[8] = '1';
    prefix_table[11][5].prefix_bits[9] = '1';
    prefix_table[11][5].prefix_bits[10] = '0';
    prefix_table[11][5].prefix_bits[11] = '1';
    prefix_table[11][5].prefix_bits[12] = '0';
    prefix_table[11][5].prefix_bits[13] = '1';
    prefix_table[11][5].prefix_bits[14] = '0';
    prefix_table[11][5].prefix_bits[15] = '0';
    prefix_table[11][5].prefix_bits[16] = '\0';
    prefix_table[11][5].tam_total = 21;

    prefix_table[11][6].prefix_bits[0] = '1';
    prefix_table[11][6].prefix_bits[1] = '1';
    prefix_table[11][6].prefix_bits[2] = '1';
    prefix_table[11][6].prefix_bits[3] = '1';
    prefix_table[11][6].prefix_bits[4] = '1';
    prefix_table[11][6].prefix_bits[5] = '1';
    prefix_table[11][6].prefix_bits[6] = '1';
    prefix_table[11][6].prefix_bits[7] = '1';
    prefix_table[11][6].prefix_bits[8] = '1';
    prefix_table[11][6].prefix_bits[9] = '1';
    prefix_table[11][6].prefix_bits[10] = '0';
    prefix_table[11][6].prefix_bits[11] = '1';
    prefix_table[11][6].prefix_bits[12] = '0';
    prefix_table[11][6].prefix_bits[13] = '1';
    prefix_table[11][6].prefix_bits[14] = '0';
    prefix_table[11][6].prefix_bits[15] = '1';
    prefix_table[11][6].prefix_bits[16] = '\0';
    prefix_table[11][6].tam_total = 22;

    prefix_table[11][7].prefix_bits[0] = '1';
    prefix_table[11][7].prefix_bits[1] = '1';
    prefix_table[11][7].prefix_bits[2] = '1';
    prefix_table[11][7].prefix_bits[3] = '1';
    prefix_table[11][7].prefix_bits[4] = '1';
    prefix_table[11][7].prefix_bits[5] = '1';
    prefix_table[11][7].prefix_bits[6] = '1';
    prefix_table[11][7].prefix_bits[7] = '1';
    prefix_table[11][7].prefix_bits[8] = '1';
    prefix_table[11][7].prefix_bits[9] = '1';
    prefix_table[11][7].prefix_bits[10] = '0';
    prefix_table[11][7].prefix_bits[11] = '1';
    prefix_table[11][7].prefix_bits[12] = '0';
    prefix_table[11][7].prefix_bits[13] = '1';
    prefix_table[11][7].prefix_bits[14] = '1';
    prefix_table[11][7].prefix_bits[15] = '0';
    prefix_table[11][7].prefix_bits[16] = '\0';
    prefix_table[11][7].tam_total = 23;

    prefix_table[11][8].prefix_bits[0] = '1';
    prefix_table[11][8].prefix_bits[1] = '1';
    prefix_table[11][8].prefix_bits[2] = '1';
    prefix_table[11][8].prefix_bits[3] = '1';
    prefix_table[11][8].prefix_bits[4] = '1';
    prefix_table[11][8].prefix_bits[5] = '1';
    prefix_table[11][8].prefix_bits[6] = '1';
    prefix_table[11][8].prefix_bits[7] = '1';
    prefix_table[11][8].prefix_bits[8] = '1';
    prefix_table[11][8].prefix_bits[9] = '1';
    prefix_table[11][8].prefix_bits[10] = '0';
    prefix_table[11][8].prefix_bits[11] = '1';
    prefix_table[11][8].prefix_bits[12] = '0';
    prefix_table[11][8].prefix_bits[13] = '1';
    prefix_table[11][8].prefix_bits[14] = '1';
    prefix_table[11][8].prefix_bits[15] = '1';
    prefix_table[11][8].prefix_bits[16] = '\0';
    prefix_table[11][8].tam_total = 24;

    prefix_table[11][9].prefix_bits[0] = '1';
    prefix_table[11][9].prefix_bits[1] = '1';
    prefix_table[11][9].prefix_bits[2] = '1';
    prefix_table[11][9].prefix_bits[3] = '1';
    prefix_table[11][9].prefix_bits[4] = '1';
    prefix_table[11][9].prefix_bits[5] = '1';
    prefix_table[11][9].prefix_bits[6] = '1';
    prefix_table[11][9].prefix_bits[7] = '1';
    prefix_table[11][9].prefix_bits[8] = '1';
    prefix_table[11][9].prefix_bits[9] = '1';
    prefix_table[11][9].prefix_bits[10] = '0';
    prefix_table[11][9].prefix_bits[11] = '1';
    prefix_table[11][9].prefix_bits[12] = '1';
    prefix_table[11][9].prefix_bits[13] = '0';
    prefix_table[11][9].prefix_bits[14] = '0';
    prefix_table[11][9].prefix_bits[15] = '0';
    prefix_table[11][9].prefix_bits[16] = '\0';
    prefix_table[11][9].tam_total = 25;

    prefix_table[11][10].prefix_bits[0] = '1';
    prefix_table[11][10].prefix_bits[1] = '1';
    prefix_table[11][10].prefix_bits[2] = '1';
    prefix_table[11][10].prefix_bits[3] = '1';
    prefix_table[11][10].prefix_bits[4] = '1';
    prefix_table[11][10].prefix_bits[5] = '1';
    prefix_table[11][10].prefix_bits[6] = '1';
    prefix_table[11][10].prefix_bits[7] = '1';
    prefix_table[11][10].prefix_bits[8] = '1';
    prefix_table[11][10].prefix_bits[9] = '1';
    prefix_table[11][10].prefix_bits[10] = '0';
    prefix_table[11][10].prefix_bits[11] = '1';
    prefix_table[11][10].prefix_bits[12] = '1';
    prefix_table[11][10].prefix_bits[13] = '0';
    prefix_table[11][10].prefix_bits[14] = '0';
    prefix_table[11][10].prefix_bits[15] = '1';
    prefix_table[11][10].prefix_bits[16] = '\0';
    prefix_table[11][10].tam_total = 26;

    prefix_table[12][1].prefix_bits[0] = '1';
    prefix_table[12][1].prefix_bits[1] = '1';
    prefix_table[12][1].prefix_bits[2] = '1';
    prefix_table[12][1].prefix_bits[3] = '1';
    prefix_table[12][1].prefix_bits[4] = '1';
    prefix_table[12][1].prefix_bits[5] = '1';
    prefix_table[12][1].prefix_bits[6] = '1';
    prefix_table[12][1].prefix_bits[7] = '0';
    prefix_table[12][1].prefix_bits[8] = '1';
    prefix_table[12][1].prefix_bits[9] = '0';
    prefix_table[12][1].prefix_bits[10] = '\0';
    prefix_table[12][1].tam_total = 11;

    prefix_table[12][2].prefix_bits[0] = '1';
    prefix_table[12][2].prefix_bits[1] = '1';
    prefix_table[12][2].prefix_bits[2] = '1';
    prefix_table[12][2].prefix_bits[3] = '1';
    prefix_table[12][2].prefix_bits[4] = '1';
    prefix_table[12][2].prefix_bits[5] = '1';
    prefix_table[12][2].prefix_bits[6] = '1';
    prefix_table[12][2].prefix_bits[7] = '1';
    prefix_table[12][2].prefix_bits[8] = '1';
    prefix_table[12][2].prefix_bits[9] = '1';
    prefix_table[12][2].prefix_bits[10] = '0';
    prefix_table[12][2].prefix_bits[11] = '1';
    prefix_table[12][2].prefix_bits[12] = '1';
    prefix_table[12][2].prefix_bits[13] = '0';
    prefix_table[12][2].prefix_bits[14] = '1';
    prefix_table[12][2].prefix_bits[15] = '0';
    prefix_table[12][2].prefix_bits[16] = '\0';
    prefix_table[12][2].tam_total = 18;

    prefix_table[12][3].prefix_bits[0] = '1';
    prefix_table[12][3].prefix_bits[1] = '1';
    prefix_table[12][3].prefix_bits[2] = '1';
    prefix_table[12][3].prefix_bits[3] = '1';
    prefix_table[12][3].prefix_bits[4] = '1';
    prefix_table[12][3].prefix_bits[5] = '1';
    prefix_table[12][3].prefix_bits[6] = '1';
    prefix_table[12][3].prefix_bits[7] = '1';
    prefix_table[12][3].prefix_bits[8] = '1';
    prefix_table[12][3].prefix_bits[9] = '1';
    prefix_table[12][3].prefix_bits[10] = '0';
    prefix_table[12][3].prefix_bits[11] = '1';
    prefix_table[12][3].prefix_bits[12] = '1';
    prefix_table[12][3].prefix_bits[13] = '0';
    prefix_table[12][3].prefix_bits[14] = '1';
    prefix_table[12][3].prefix_bits[15] = '1';
    prefix_table[12][3].prefix_bits[16] = '\0';
    prefix_table[12][3].tam_total = 19;

    prefix_table[12][4].prefix_bits[0] = '1';
    prefix_table[12][4].prefix_bits[1] = '1';
    prefix_table[12][4].prefix_bits[2] = '1';
    prefix_table[12][4].prefix_bits[3] = '1';
    prefix_table[12][4].prefix_bits[4] = '1';
    prefix_table[12][4].prefix_bits[5] = '1';
    prefix_table[12][4].prefix_bits[6] = '1';
    prefix_table[12][4].prefix_bits[7] = '1';
    prefix_table[12][4].prefix_bits[8] = '1';
    prefix_table[12][4].prefix_bits[9] = '1';
    prefix_table[12][4].prefix_bits[10] = '0';
    prefix_table[12][4].prefix_bits[11] = '1';
    prefix_table[12][4].prefix_bits[12] = '1';
    prefix_table[12][4].prefix_bits[13] = '1';
    prefix_table[12][4].prefix_bits[14] = '0';
    prefix_table[12][4].prefix_bits[15] = '0';
    prefix_table[12][4].prefix_bits[16] = '\0';
    prefix_table[12][4].tam_total = 20;

    prefix_table[12][5].prefix_bits[0] = '1';
    prefix_table[12][5].prefix_bits[1] = '1';
    prefix_table[12][5].prefix_bits[2] = '1';
    prefix_table[12][5].prefix_bits[3] = '1';
    prefix_table[12][5].prefix_bits[4] = '1';
    prefix_table[12][5].prefix_bits[5] = '1';
    prefix_table[12][5].prefix_bits[6] = '1';
    prefix_table[12][5].prefix_bits[7] = '1';
    prefix_table[12][5].prefix_bits[8] = '1';
    prefix_table[12][5].prefix_bits[9] = '1';
    prefix_table[12][5].prefix_bits[10] = '0';
    prefix_table[12][5].prefix_bits[11] = '1';
    prefix_table[12][5].prefix_bits[12] = '1';
    prefix_table[12][5].prefix_bits[13] = '1';
    prefix_table[12][5].prefix_bits[14] = '0';
    prefix_table[12][5].prefix_bits[15] = '1';
    prefix_table[12][5].prefix_bits[16] = '\0';
    prefix_table[12][5].tam_total = 21;

    prefix_table[12][6].prefix_bits[0] = '1';
    prefix_table[12][6].prefix_bits[1] = '1';
    prefix_table[12][6].prefix_bits[2] = '1';
    prefix_table[12][6].prefix_bits[3] = '1';
    prefix_table[12][6].prefix_bits[4] = '1';
    prefix_table[12][6].prefix_bits[5] = '1';
    prefix_table[12][6].prefix_bits[6] = '1';
    prefix_table[12][6].prefix_bits[7] = '1';
    prefix_table[12][6].prefix_bits[8] = '1';
    prefix_table[12][6].prefix_bits[9] = '1';
    prefix_table[12][6].prefix_bits[10] = '0';
    prefix_table[12][6].prefix_bits[11] = '1';
    prefix_table[12][6].prefix_bits[12] = '1';
    prefix_table[12][6].prefix_bits[13] = '1';
    prefix_table[12][6].prefix_bits[14] = '1';
    prefix_table[12][6].prefix_bits[15] = '0';
    prefix_table[12][6].prefix_bits[16] = '\0';
    prefix_table[12][6].tam_total = 22;

    prefix_table[12][7].prefix_bits[0] = '1';
    prefix_table[12][7].prefix_bits[1] = '1';
    prefix_table[12][7].prefix_bits[2] = '1';
    prefix_table[12][7].prefix_bits[3] = '1';
    prefix_table[12][7].prefix_bits[4] = '1';
    prefix_table[12][7].prefix_bits[5] = '1';
    prefix_table[12][7].prefix_bits[6] = '1';
    prefix_table[12][7].prefix_bits[7] = '1';
    prefix_table[12][7].prefix_bits[8] = '1';
    prefix_table[12][7].prefix_bits[9] = '1';
    prefix_table[12][7].prefix_bits[10] = '0';
    prefix_table[12][7].prefix_bits[11] = '1';
    prefix_table[12][7].prefix_bits[12] = '1';
    prefix_table[12][7].prefix_bits[13] = '1';
    prefix_table[12][7].prefix_bits[14] = '1';
    prefix_table[12][7].prefix_bits[15] = '1';
    prefix_table[12][7].prefix_bits[16] = '\0';
    prefix_table[12][7].tam_total = 23;

    prefix_table[12][8].prefix_bits[0] = '1';
    prefix_table[12][8].prefix_bits[1] = '1';
    prefix_table[12][8].prefix_bits[2] = '1';
    prefix_table[12][8].prefix_bits[3] = '1';
    prefix_table[12][8].prefix_bits[4] = '1';
    prefix_table[12][8].prefix_bits[5] = '1';
    prefix_table[12][8].prefix_bits[6] = '1';
    prefix_table[12][8].prefix_bits[7] = '1';
    prefix_table[12][8].prefix_bits[8] = '1';
    prefix_table[12][8].prefix_bits[9] = '1';
    prefix_table[12][8].prefix_bits[10] = '1';
    prefix_table[12][8].prefix_bits[11] = '0';
    prefix_table[12][8].prefix_bits[12] = '0';
    prefix_table[12][8].prefix_bits[13] = '0';
    prefix_table[12][8].prefix_bits[14] = '0';
    prefix_table[12][8].prefix_bits[15] = '0';
    prefix_table[12][8].prefix_bits[16] = '\0';
    prefix_table[12][8].tam_total = 24;

    prefix_table[12][9].prefix_bits[0] = '1';
    prefix_table[12][9].prefix_bits[1] = '1';
    prefix_table[12][9].prefix_bits[2] = '1';
    prefix_table[12][9].prefix_bits[3] = '1';
    prefix_table[12][9].prefix_bits[4] = '1';
    prefix_table[12][9].prefix_bits[5] = '1';
    prefix_table[12][9].prefix_bits[6] = '1';
    prefix_table[12][9].prefix_bits[7] = '1';
    prefix_table[12][9].prefix_bits[8] = '1';
    prefix_table[12][9].prefix_bits[9] = '1';
    prefix_table[12][9].prefix_bits[10] = '1';
    prefix_table[12][9].prefix_bits[11] = '0';
    prefix_table[12][9].prefix_bits[12] = '0';
    prefix_table[12][9].prefix_bits[13] = '0';
    prefix_table[12][9].prefix_bits[14] = '0';
    prefix_table[12][9].prefix_bits[15] = '1';
    prefix_table[12][9].prefix_bits[16] = '\0';
    prefix_table[12][9].tam_total = 25;

    prefix_table[12][10].prefix_bits[0] = '1';
    prefix_table[12][10].prefix_bits[1] = '1';
    prefix_table[12][10].prefix_bits[2] = '1';
    prefix_table[12][10].prefix_bits[3] = '1';
    prefix_table[12][10].prefix_bits[4] = '1';
    prefix_table[12][10].prefix_bits[5] = '1';
    prefix_table[12][10].prefix_bits[6] = '1';
    prefix_table[12][10].prefix_bits[7] = '1';
    prefix_table[12][10].prefix_bits[8] = '1';
    prefix_table[12][10].prefix_bits[9] = '1';
    prefix_table[12][10].prefix_bits[10] = '1';
    prefix_table[12][10].prefix_bits[11] = '0';
    prefix_table[12][10].prefix_bits[12] = '0';
    prefix_table[12][10].prefix_bits[13] = '0';
    prefix_table[12][10].prefix_bits[14] = '1';
    prefix_table[12][10].prefix_bits[15] = '0';
    prefix_table[12][10].prefix_bits[16] = '\0';
    prefix_table[12][10].tam_total = 26;

    prefix_table[13][1].prefix_bits[0] = '1';
    prefix_table[13][1].prefix_bits[1] = '1';
    prefix_table[13][1].prefix_bits[2] = '1';
    prefix_table[13][1].prefix_bits[3] = '1';
    prefix_table[13][1].prefix_bits[4] = '1';
    prefix_table[13][1].prefix_bits[5] = '1';
    prefix_table[13][1].prefix_bits[6] = '1';
    prefix_table[13][1].prefix_bits[7] = '1';
    prefix_table[13][1].prefix_bits[8] = '0';
    prefix_table[13][1].prefix_bits[9] = '1';
    prefix_table[13][1].prefix_bits[10] = '0';
    prefix_table[13][1].prefix_bits[11] = '\0';
    prefix_table[13][1].tam_total = 12;

    prefix_table[13][2].prefix_bits[0] = '1';
    prefix_table[13][2].prefix_bits[1] = '1';
    prefix_table[13][2].prefix_bits[2] = '1';
    prefix_table[13][2].prefix_bits[3] = '1';
    prefix_table[13][2].prefix_bits[4] = '1';
    prefix_table[13][2].prefix_bits[5] = '1';
    prefix_table[13][2].prefix_bits[6] = '1';
    prefix_table[13][2].prefix_bits[7] = '1';
    prefix_table[13][2].prefix_bits[8] = '1';
    prefix_table[13][2].prefix_bits[9] = '1';
    prefix_table[13][2].prefix_bits[10] = '1';
    prefix_table[13][2].prefix_bits[11] = '0';
    prefix_table[13][2].prefix_bits[12] = '0';
    prefix_table[13][2].prefix_bits[13] = '0';
    prefix_table[13][2].prefix_bits[14] = '1';
    prefix_table[13][2].prefix_bits[15] = '1';
    prefix_table[13][2].prefix_bits[16] = '\0';
    prefix_table[13][2].tam_total = 18;

    prefix_table[13][3].prefix_bits[0] = '1';
    prefix_table[13][3].prefix_bits[1] = '1';
    prefix_table[13][3].prefix_bits[2] = '1';
    prefix_table[13][3].prefix_bits[3] = '1';
    prefix_table[13][3].prefix_bits[4] = '1';
    prefix_table[13][3].prefix_bits[5] = '1';
    prefix_table[13][3].prefix_bits[6] = '1';
    prefix_table[13][3].prefix_bits[7] = '1';
    prefix_table[13][3].prefix_bits[8] = '1';
    prefix_table[13][3].prefix_bits[9] = '1';
    prefix_table[13][3].prefix_bits[10] = '1';
    prefix_table[13][3].prefix_bits[11] = '0';
    prefix_table[13][3].prefix_bits[12] = '0';
    prefix_table[13][3].prefix_bits[13] = '1';
    prefix_table[13][3].prefix_bits[14] = '0';
    prefix_table[13][3].prefix_bits[15] = '0';
    prefix_table[13][3].prefix_bits[16] = '\0';
    prefix_table[13][3].tam_total = 19;

    prefix_table[13][4].prefix_bits[0] = '1';
    prefix_table[13][4].prefix_bits[1] = '1';
    prefix_table[13][4].prefix_bits[2] = '1';
    prefix_table[13][4].prefix_bits[3] = '1';
    prefix_table[13][4].prefix_bits[4] = '1';
    prefix_table[13][4].prefix_bits[5] = '1';
    prefix_table[13][4].prefix_bits[6] = '1';
    prefix_table[13][4].prefix_bits[7] = '1';
    prefix_table[13][4].prefix_bits[8] = '1';
    prefix_table[13][4].prefix_bits[9] = '1';
    prefix_table[13][4].prefix_bits[10] = '1';
    prefix_table[13][4].prefix_bits[11] = '0';
    prefix_table[13][4].prefix_bits[12] = '0';
    prefix_table[13][4].prefix_bits[13] = '1';
    prefix_table[13][4].prefix_bits[14] = '0';
    prefix_table[13][4].prefix_bits[15] = '1';
    prefix_table[13][4].prefix_bits[16] = '\0';
    prefix_table[13][4].tam_total = 20;

    prefix_table[13][5].prefix_bits[0] = '1';
    prefix_table[13][5].prefix_bits[1] = '1';
    prefix_table[13][5].prefix_bits[2] = '1';
    prefix_table[13][5].prefix_bits[3] = '1';
    prefix_table[13][5].prefix_bits[4] = '1';
    prefix_table[13][5].prefix_bits[5] = '1';
    prefix_table[13][5].prefix_bits[6] = '1';
    prefix_table[13][5].prefix_bits[7] = '1';
    prefix_table[13][5].prefix_bits[8] = '1';
    prefix_table[13][5].prefix_bits[9] = '1';
    prefix_table[13][5].prefix_bits[10] = '1';
    prefix_table[13][5].prefix_bits[11] = '0';
    prefix_table[13][5].prefix_bits[12] = '0';
    prefix_table[13][5].prefix_bits[13] = '1';
    prefix_table[13][5].prefix_bits[14] = '1';
    prefix_table[13][5].prefix_bits[15] = '0';
    prefix_table[13][5].prefix_bits[16] = '\0';
    prefix_table[13][5].tam_total = 21;

    prefix_table[13][6].prefix_bits[0] = '1';
    prefix_table[13][6].prefix_bits[1] = '1';
    prefix_table[13][6].prefix_bits[2] = '1';
    prefix_table[13][6].prefix_bits[3] = '1';
    prefix_table[13][6].prefix_bits[4] = '1';
    prefix_table[13][6].prefix_bits[5] = '1';
    prefix_table[13][6].prefix_bits[6] = '1';
    prefix_table[13][6].prefix_bits[7] = '1';
    prefix_table[13][6].prefix_bits[8] = '1';
    prefix_table[13][6].prefix_bits[9] = '1';
    prefix_table[13][6].prefix_bits[10] = '1';
    prefix_table[13][6].prefix_bits[11] = '0';
    prefix_table[13][6].prefix_bits[12] = '0';
    prefix_table[13][6].prefix_bits[13] = '1';
    prefix_table[13][6].prefix_bits[14] = '1';
    prefix_table[13][6].prefix_bits[15] = '1';
    prefix_table[13][6].prefix_bits[16] = '\0';
    prefix_table[13][6].tam_total = 22;

    prefix_table[13][7].prefix_bits[0] = '1';
    prefix_table[13][7].prefix_bits[1] = '1';
    prefix_table[13][7].prefix_bits[2] = '1';
    prefix_table[13][7].prefix_bits[3] = '1';
    prefix_table[13][7].prefix_bits[4] = '1';
    prefix_table[13][7].prefix_bits[5] = '1';
    prefix_table[13][7].prefix_bits[6] = '1';
    prefix_table[13][7].prefix_bits[7] = '1';
    prefix_table[13][7].prefix_bits[8] = '1';
    prefix_table[13][7].prefix_bits[9] = '1';
    prefix_table[13][7].prefix_bits[10] = '1';
    prefix_table[13][7].prefix_bits[11] = '0';
    prefix_table[13][7].prefix_bits[12] = '1';
    prefix_table[13][7].prefix_bits[13] = '0';
    prefix_table[13][7].prefix_bits[14] = '0';
    prefix_table[13][7].prefix_bits[15] = '0';
    prefix_table[13][7].prefix_bits[16] = '\0';
    prefix_table[13][7].tam_total = 23;

    prefix_table[13][8].prefix_bits[0] = '1';
    prefix_table[13][8].prefix_bits[1] = '1';
    prefix_table[13][8].prefix_bits[2] = '1';
    prefix_table[13][8].prefix_bits[3] = '1';
    prefix_table[13][8].prefix_bits[4] = '1';
    prefix_table[13][8].prefix_bits[5] = '1';
    prefix_table[13][8].prefix_bits[6] = '1';
    prefix_table[13][8].prefix_bits[7] = '1';
    prefix_table[13][8].prefix_bits[8] = '1';
    prefix_table[13][8].prefix_bits[9] = '1';
    prefix_table[13][8].prefix_bits[10] = '1';
    prefix_table[13][8].prefix_bits[11] = '0';
    prefix_table[13][8].prefix_bits[12] = '1';
    prefix_table[13][8].prefix_bits[13] = '0';
    prefix_table[13][8].prefix_bits[14] = '0';
    prefix_table[13][8].prefix_bits[15] = '1';
    prefix_table[13][8].prefix_bits[16] = '\0';
    prefix_table[13][8].tam_total = 24;

    prefix_table[13][9].prefix_bits[0] = '1';
    prefix_table[13][9].prefix_bits[1] = '1';
    prefix_table[13][9].prefix_bits[2] = '1';
    prefix_table[13][9].prefix_bits[3] = '1';
    prefix_table[13][9].prefix_bits[4] = '1';
    prefix_table[13][9].prefix_bits[5] = '1';
    prefix_table[13][9].prefix_bits[6] = '1';
    prefix_table[13][9].prefix_bits[7] = '1';
    prefix_table[13][9].prefix_bits[8] = '1';
    prefix_table[13][9].prefix_bits[9] = '1';
    prefix_table[13][9].prefix_bits[10] = '1';
    prefix_table[13][9].prefix_bits[11] = '0';
    prefix_table[13][9].prefix_bits[12] = '1';
    prefix_table[13][9].prefix_bits[13] = '0';
    prefix_table[13][9].prefix_bits[14] = '1';
    prefix_table[13][9].prefix_bits[15] = '0';
    prefix_table[13][9].prefix_bits[16] = '\0';
    prefix_table[13][9].tam_total = 25;

    prefix_table[13][10].prefix_bits[0] = '1';
    prefix_table[13][10].prefix_bits[1] = '1';
    prefix_table[13][10].prefix_bits[2] = '1';
    prefix_table[13][10].prefix_bits[3] = '1';
    prefix_table[13][10].prefix_bits[4] = '1';
    prefix_table[13][10].prefix_bits[5] = '1';
    prefix_table[13][10].prefix_bits[6] = '1';
    prefix_table[13][10].prefix_bits[7] = '1';
    prefix_table[13][10].prefix_bits[8] = '1';
    prefix_table[13][10].prefix_bits[9] = '1';
    prefix_table[13][10].prefix_bits[10] = '1';
    prefix_table[13][10].prefix_bits[11] = '0';
    prefix_table[13][10].prefix_bits[12] = '1';
    prefix_table[13][10].prefix_bits[13] = '0';
    prefix_table[13][10].prefix_bits[14] = '1';
    prefix_table[13][10].prefix_bits[15] = '1';
    prefix_table[13][10].prefix_bits[16] = '\0';
    prefix_table[13][10].tam_total = 26;

    prefix_table[14][1].prefix_bits[0] = '1';
    prefix_table[14][1].prefix_bits[1] = '1';
    prefix_table[14][1].prefix_bits[2] = '1';
    prefix_table[14][1].prefix_bits[3] = '1';
    prefix_table[14][1].prefix_bits[4] = '1';
    prefix_table[14][1].prefix_bits[5] = '1';
    prefix_table[14][1].prefix_bits[6] = '1';
    prefix_table[14][1].prefix_bits[7] = '1';
    prefix_table[14][1].prefix_bits[8] = '0';
    prefix_table[14][1].prefix_bits[9] = '1';
    prefix_table[14][1].prefix_bits[10] = '1';
    prefix_table[14][1].prefix_bits[11] = '0';
    prefix_table[14][1].prefix_bits[12] = '\0';
    prefix_table[14][1].tam_total = 13;

    prefix_table[14][2].prefix_bits[0] = '1';
    prefix_table[14][2].prefix_bits[1] = '1';
    prefix_table[14][2].prefix_bits[2] = '1';
    prefix_table[14][2].prefix_bits[3] = '1';
    prefix_table[14][2].prefix_bits[4] = '1';
    prefix_table[14][2].prefix_bits[5] = '1';
    prefix_table[14][2].prefix_bits[6] = '1';
    prefix_table[14][2].prefix_bits[7] = '1';
    prefix_table[14][2].prefix_bits[8] = '1';
    prefix_table[14][2].prefix_bits[9] = '1';
    prefix_table[14][2].prefix_bits[10] = '1';
    prefix_table[14][2].prefix_bits[11] = '0';
    prefix_table[14][2].prefix_bits[12] = '1';
    prefix_table[14][2].prefix_bits[13] = '1';
    prefix_table[14][2].prefix_bits[14] = '0';
    prefix_table[14][2].prefix_bits[15] = '0';
    prefix_table[14][2].prefix_bits[16] = '\0';
    prefix_table[14][2].tam_total = 18;

    prefix_table[14][3].prefix_bits[0] = '1';
    prefix_table[14][3].prefix_bits[1] = '1';
    prefix_table[14][3].prefix_bits[2] = '1';
    prefix_table[14][3].prefix_bits[3] = '1';
    prefix_table[14][3].prefix_bits[4] = '1';
    prefix_table[14][3].prefix_bits[5] = '1';
    prefix_table[14][3].prefix_bits[6] = '1';
    prefix_table[14][3].prefix_bits[7] = '1';
    prefix_table[14][3].prefix_bits[8] = '1';
    prefix_table[14][3].prefix_bits[9] = '1';
    prefix_table[14][3].prefix_bits[10] = '1';
    prefix_table[14][3].prefix_bits[11] = '0';
    prefix_table[14][3].prefix_bits[12] = '1';
    prefix_table[14][3].prefix_bits[13] = '1';
    prefix_table[14][3].prefix_bits[14] = '0';
    prefix_table[14][3].prefix_bits[15] = '1';
    prefix_table[14][3].prefix_bits[16] = '\0';
    prefix_table[14][3].tam_total = 19;

    prefix_table[14][4].prefix_bits[0] = '1';
    prefix_table[14][4].prefix_bits[1] = '1';
    prefix_table[14][4].prefix_bits[2] = '1';
    prefix_table[14][4].prefix_bits[3] = '1';
    prefix_table[14][4].prefix_bits[4] = '1';
    prefix_table[14][4].prefix_bits[5] = '1';
    prefix_table[14][4].prefix_bits[6] = '1';
    prefix_table[14][4].prefix_bits[7] = '1';
    prefix_table[14][4].prefix_bits[8] = '1';
    prefix_table[14][4].prefix_bits[9] = '1';
    prefix_table[14][4].prefix_bits[10] = '1';
    prefix_table[14][4].prefix_bits[11] = '0';
    prefix_table[14][4].prefix_bits[12] = '1';
    prefix_table[14][4].prefix_bits[13] = '1';
    prefix_table[14][4].prefix_bits[14] = '1';
    prefix_table[14][4].prefix_bits[15] = '0';
    prefix_table[14][4].prefix_bits[16] = '\0';
    prefix_table[14][4].tam_total = 20;

    prefix_table[14][5].prefix_bits[0] = '1';
    prefix_table[14][5].prefix_bits[1] = '1';
    prefix_table[14][5].prefix_bits[2] = '1';
    prefix_table[14][5].prefix_bits[3] = '1';
    prefix_table[14][5].prefix_bits[4] = '1';
    prefix_table[14][5].prefix_bits[5] = '1';
    prefix_table[14][5].prefix_bits[6] = '1';
    prefix_table[14][5].prefix_bits[7] = '1';
    prefix_table[14][5].prefix_bits[8] = '1';
    prefix_table[14][5].prefix_bits[9] = '1';
    prefix_table[14][5].prefix_bits[10] = '1';
    prefix_table[14][5].prefix_bits[11] = '0';
    prefix_table[14][5].prefix_bits[12] = '1';
    prefix_table[14][5].prefix_bits[13] = '1';
    prefix_table[14][5].prefix_bits[14] = '1';
    prefix_table[14][5].prefix_bits[15] = '1';
    prefix_table[14][5].prefix_bits[16] = '\0';
    prefix_table[14][5].tam_total = 21;

    prefix_table[14][6].prefix_bits[0] = '1';
    prefix_table[14][6].prefix_bits[1] = '1';
    prefix_table[14][6].prefix_bits[2] = '1';
    prefix_table[14][6].prefix_bits[3] = '1';
    prefix_table[14][6].prefix_bits[4] = '1';
    prefix_table[14][6].prefix_bits[5] = '1';
    prefix_table[14][6].prefix_bits[6] = '1';
    prefix_table[14][6].prefix_bits[7] = '1';
    prefix_table[14][6].prefix_bits[8] = '1';
    prefix_table[14][6].prefix_bits[9] = '1';
    prefix_table[14][6].prefix_bits[10] = '1';
    prefix_table[14][6].prefix_bits[11] = '1';
    prefix_table[14][6].prefix_bits[12] = '0';
    prefix_table[14][6].prefix_bits[13] = '0';
    prefix_table[14][6].prefix_bits[14] = '0';
    prefix_table[14][6].prefix_bits[15] = '0';
    prefix_table[14][6].prefix_bits[16] = '\0';
    prefix_table[14][6].tam_total = 22;

    prefix_table[14][7].prefix_bits[0] = '1';
    prefix_table[14][7].prefix_bits[1] = '1';
    prefix_table[14][7].prefix_bits[2] = '1';
    prefix_table[14][7].prefix_bits[3] = '1';
    prefix_table[14][7].prefix_bits[4] = '1';
    prefix_table[14][7].prefix_bits[5] = '1';
    prefix_table[14][7].prefix_bits[6] = '1';
    prefix_table[14][7].prefix_bits[7] = '1';
    prefix_table[14][7].prefix_bits[8] = '1';
    prefix_table[14][7].prefix_bits[9] = '1';
    prefix_table[14][7].prefix_bits[10] = '1';
    prefix_table[14][7].prefix_bits[11] = '1';
    prefix_table[14][7].prefix_bits[12] = '0';
    prefix_table[14][7].prefix_bits[13] = '0';
    prefix_table[14][7].prefix_bits[14] = '0';
    prefix_table[14][7].prefix_bits[15] = '1';
    prefix_table[14][7].prefix_bits[16] = '\0';
    prefix_table[14][7].tam_total = 23;

    prefix_table[14][8].prefix_bits[0] = '1';
    prefix_table[14][8].prefix_bits[1] = '1';
    prefix_table[14][8].prefix_bits[2] = '1';
    prefix_table[14][8].prefix_bits[3] = '1';
    prefix_table[14][8].prefix_bits[4] = '1';
    prefix_table[14][8].prefix_bits[5] = '1';
    prefix_table[14][8].prefix_bits[6] = '1';
    prefix_table[14][8].prefix_bits[7] = '1';
    prefix_table[14][8].prefix_bits[8] = '1';
    prefix_table[14][8].prefix_bits[9] = '1';
    prefix_table[14][8].prefix_bits[10] = '1';
    prefix_table[14][8].prefix_bits[11] = '1';
    prefix_table[14][8].prefix_bits[12] = '0';
    prefix_table[14][8].prefix_bits[13] = '0';
    prefix_table[14][8].prefix_bits[14] = '1';
    prefix_table[14][8].prefix_bits[15] = '0';
    prefix_table[14][8].prefix_bits[16] = '\0';
    prefix_table[14][8].tam_total = 24;

    prefix_table[14][9].prefix_bits[0] = '1';
    prefix_table[14][9].prefix_bits[1] = '1';
    prefix_table[14][9].prefix_bits[2] = '1';
    prefix_table[14][9].prefix_bits[3] = '1';
    prefix_table[14][9].prefix_bits[4] = '1';
    prefix_table[14][9].prefix_bits[5] = '1';
    prefix_table[14][9].prefix_bits[6] = '1';
    prefix_table[14][9].prefix_bits[7] = '1';
    prefix_table[14][9].prefix_bits[8] = '1';
    prefix_table[14][9].prefix_bits[9] = '1';
    prefix_table[14][9].prefix_bits[10] = '1';
    prefix_table[14][9].prefix_bits[11] = '1';
    prefix_table[14][9].prefix_bits[12] = '0';
    prefix_table[14][9].prefix_bits[13] = '0';
    prefix_table[14][9].prefix_bits[14] = '1';
    prefix_table[14][9].prefix_bits[15] = '1';
    prefix_table[14][9].prefix_bits[16] = '\0';
    prefix_table[14][9].tam_total = 25;

    prefix_table[14][10].prefix_bits[0] = '1';
    prefix_table[14][10].prefix_bits[1] = '1';
    prefix_table[14][10].prefix_bits[2] = '1';
    prefix_table[14][10].prefix_bits[3] = '1';
    prefix_table[14][10].prefix_bits[4] = '1';
    prefix_table[14][10].prefix_bits[5] = '1';
    prefix_table[14][10].prefix_bits[6] = '1';
    prefix_table[14][10].prefix_bits[7] = '1';
    prefix_table[14][10].prefix_bits[8] = '1';
    prefix_table[14][10].prefix_bits[9] = '1';
    prefix_table[14][10].prefix_bits[10] = '1';
    prefix_table[14][10].prefix_bits[11] = '1';
    prefix_table[14][10].prefix_bits[12] = '0';
    prefix_table[14][10].prefix_bits[13] = '1';
    prefix_table[14][10].prefix_bits[14] = '0';
    prefix_table[14][10].prefix_bits[15] = '0';
    prefix_table[14][10].prefix_bits[16] = '\0';
    prefix_table[14][10].tam_total = 26;

    //Fim do arquivo!
    prefix_table[15][0].prefix_bits[0] = '1';
    prefix_table[15][0].prefix_bits[1] = '1';
    prefix_table[15][0].prefix_bits[2] = '1';
    prefix_table[15][0].prefix_bits[3] = '1';
    prefix_table[15][0].prefix_bits[4] = '1';
    prefix_table[15][0].prefix_bits[5] = '1';
    prefix_table[15][0].prefix_bits[6] = '1';
    prefix_table[15][0].prefix_bits[7] = '1';
    prefix_table[15][0].prefix_bits[8] = '0';
    prefix_table[15][0].prefix_bits[9] = '1';
    prefix_table[15][0].prefix_bits[10] = '1';
    prefix_table[15][0].prefix_bits[11] = '1';
    prefix_table[15][0].prefix_bits[12] = '\0';
    prefix_table[15][0].tam_total = 12;

    prefix_table[15][1].prefix_bits[0] = '1';
    prefix_table[15][1].prefix_bits[1] = '1';
    prefix_table[15][1].prefix_bits[2] = '1';
    prefix_table[15][1].prefix_bits[3] = '1';
    prefix_table[15][1].prefix_bits[4] = '1';
    prefix_table[15][1].prefix_bits[5] = '1';
    prefix_table[15][1].prefix_bits[6] = '1';
    prefix_table[15][1].prefix_bits[7] = '1';
    prefix_table[15][1].prefix_bits[8] = '1';
    prefix_table[15][1].prefix_bits[9] = '1';
    prefix_table[15][1].prefix_bits[10] = '1';
    prefix_table[15][1].prefix_bits[11] = '1';
    prefix_table[15][1].prefix_bits[12] = '0';
    prefix_table[15][1].prefix_bits[13] = '1';
    prefix_table[15][1].prefix_bits[14] = '0';
    prefix_table[15][1].prefix_bits[15] = '1';
    prefix_table[15][1].prefix_bits[16] = '\0';
    prefix_table[15][1].tam_total = 17;

    prefix_table[15][2].prefix_bits[0] = '1';
    prefix_table[15][2].prefix_bits[1] = '1';
    prefix_table[15][2].prefix_bits[2] = '1';
    prefix_table[15][2].prefix_bits[3] = '1';
    prefix_table[15][2].prefix_bits[4] = '1';
    prefix_table[15][2].prefix_bits[5] = '1';
    prefix_table[15][2].prefix_bits[6] = '1';
    prefix_table[15][2].prefix_bits[7] = '1';
    prefix_table[15][2].prefix_bits[8] = '1';
    prefix_table[15][2].prefix_bits[9] = '1';
    prefix_table[15][2].prefix_bits[10] = '1';
    prefix_table[15][2].prefix_bits[11] = '1';
    prefix_table[15][2].prefix_bits[12] = '0';
    prefix_table[15][2].prefix_bits[13] = '1';
    prefix_table[15][2].prefix_bits[14] = '1';
    prefix_table[15][2].prefix_bits[15] = '0';
    prefix_table[15][2].prefix_bits[16] = '\0';
    prefix_table[15][2].tam_total = 18;

    prefix_table[15][3].prefix_bits[0] = '1';
    prefix_table[15][3].prefix_bits[1] = '1';
    prefix_table[15][3].prefix_bits[2] = '1';
    prefix_table[15][3].prefix_bits[3] = '1';
    prefix_table[15][3].prefix_bits[4] = '1';
    prefix_table[15][3].prefix_bits[5] = '1';
    prefix_table[15][3].prefix_bits[6] = '1';
    prefix_table[15][3].prefix_bits[7] = '1';
    prefix_table[15][3].prefix_bits[8] = '1';
    prefix_table[15][3].prefix_bits[9] = '1';
    prefix_table[15][3].prefix_bits[10] = '1';
    prefix_table[15][3].prefix_bits[11] = '1';
    prefix_table[15][3].prefix_bits[12] = '0';
    prefix_table[15][3].prefix_bits[13] = '1';
    prefix_table[15][3].prefix_bits[14] = '1';
    prefix_table[15][3].prefix_bits[15] = '1';
    prefix_table[15][3].prefix_bits[16] = '\0';
    prefix_table[15][3].tam_total = 19;

    prefix_table[15][4].prefix_bits[0] = '1';
    prefix_table[15][4].prefix_bits[1] = '1';
    prefix_table[15][4].prefix_bits[2] = '1';
    prefix_table[15][4].prefix_bits[3] = '1';
    prefix_table[15][4].prefix_bits[4] = '1';
    prefix_table[15][4].prefix_bits[5] = '1';
    prefix_table[15][4].prefix_bits[6] = '1';
    prefix_table[15][4].prefix_bits[7] = '1';
    prefix_table[15][4].prefix_bits[8] = '1';
    prefix_table[15][4].prefix_bits[9] = '1';
    prefix_table[15][4].prefix_bits[10] = '1';
    prefix_table[15][4].prefix_bits[11] = '1';
    prefix_table[15][4].prefix_bits[12] = '1';
    prefix_table[15][4].prefix_bits[13] = '0';
    prefix_table[15][4].prefix_bits[14] = '0';
    prefix_table[15][4].prefix_bits[15] = '0';
    prefix_table[15][4].prefix_bits[16] = '\0';
    prefix_table[15][4].tam_total = 20;

    prefix_table[15][5].prefix_bits[0] = '1';
    prefix_table[15][5].prefix_bits[1] = '1';
    prefix_table[15][5].prefix_bits[2] = '1';
    prefix_table[15][5].prefix_bits[3] = '1';
    prefix_table[15][5].prefix_bits[4] = '1';
    prefix_table[15][5].prefix_bits[5] = '1';
    prefix_table[15][5].prefix_bits[6] = '1';
    prefix_table[15][5].prefix_bits[7] = '1';
    prefix_table[15][5].prefix_bits[8] = '1';
    prefix_table[15][5].prefix_bits[9] = '1';
    prefix_table[15][5].prefix_bits[10] = '1';
    prefix_table[15][5].prefix_bits[11] = '1';
    prefix_table[15][5].prefix_bits[12] = '1';
    prefix_table[15][5].prefix_bits[13] = '0';
    prefix_table[15][5].prefix_bits[14] = '0';
    prefix_table[15][5].prefix_bits[15] = '1';
    prefix_table[15][5].prefix_bits[16] = '\0';
    prefix_table[15][5].tam_total = 21;

    prefix_table[15][6].prefix_bits[0] = '1';
    prefix_table[15][6].prefix_bits[1] = '1';
    prefix_table[15][6].prefix_bits[2] = '1';
    prefix_table[15][6].prefix_bits[3] = '1';
    prefix_table[15][6].prefix_bits[4] = '1';
    prefix_table[15][6].prefix_bits[5] = '1';
    prefix_table[15][6].prefix_bits[6] = '1';
    prefix_table[15][6].prefix_bits[7] = '1';
    prefix_table[15][6].prefix_bits[8] = '1';
    prefix_table[15][6].prefix_bits[9] = '1';
    prefix_table[15][6].prefix_bits[10] = '1';
    prefix_table[15][6].prefix_bits[11] = '1';
    prefix_table[15][6].prefix_bits[12] = '1';
    prefix_table[15][6].prefix_bits[13] = '0';
    prefix_table[15][6].prefix_bits[14] = '1';
    prefix_table[15][6].prefix_bits[15] = '0';
    prefix_table[15][6].prefix_bits[16] = '\0';
    prefix_table[15][6].tam_total = 22;

    prefix_table[15][7].prefix_bits[0] = '1';
    prefix_table[15][7].prefix_bits[1] = '1';
    prefix_table[15][7].prefix_bits[2] = '1';
    prefix_table[15][7].prefix_bits[3] = '1';
    prefix_table[15][7].prefix_bits[4] = '1';
    prefix_table[15][7].prefix_bits[5] = '1';
    prefix_table[15][7].prefix_bits[6] = '1';
    prefix_table[15][7].prefix_bits[7] = '1';
    prefix_table[15][7].prefix_bits[8] = '1';
    prefix_table[15][7].prefix_bits[9] = '1';
    prefix_table[15][7].prefix_bits[10] = '1';
    prefix_table[15][7].prefix_bits[11] = '1';
    prefix_table[15][7].prefix_bits[12] = '1';
    prefix_table[15][7].prefix_bits[13] = '0';
    prefix_table[15][7].prefix_bits[14] = '1';
    prefix_table[15][7].prefix_bits[15] = '1';
    prefix_table[15][7].prefix_bits[16] = '\0';
    prefix_table[15][7].tam_total = 23;

    prefix_table[15][8].prefix_bits[0] = '1';
    prefix_table[15][8].prefix_bits[1] = '1';
    prefix_table[15][8].prefix_bits[2] = '1';
    prefix_table[15][8].prefix_bits[3] = '1';
    prefix_table[15][8].prefix_bits[4] = '1';
    prefix_table[15][8].prefix_bits[5] = '1';
    prefix_table[15][8].prefix_bits[6] = '1';
    prefix_table[15][8].prefix_bits[7] = '1';
    prefix_table[15][8].prefix_bits[8] = '1';
    prefix_table[15][8].prefix_bits[9] = '1';
    prefix_table[15][8].prefix_bits[10] = '1';
    prefix_table[15][8].prefix_bits[11] = '1';
    prefix_table[15][8].prefix_bits[12] = '1';
    prefix_table[15][8].prefix_bits[13] = '1';
    prefix_table[15][8].prefix_bits[14] = '0';
    prefix_table[15][8].prefix_bits[15] = '0';
    prefix_table[15][8].prefix_bits[16] = '\0';
    prefix_table[15][8].tam_total = 24;

    prefix_table[15][9].prefix_bits[0] = '1';
    prefix_table[15][9].prefix_bits[1] = '1';
    prefix_table[15][9].prefix_bits[2] = '1';
    prefix_table[15][9].prefix_bits[3] = '1';
    prefix_table[15][9].prefix_bits[4] = '1';
    prefix_table[15][9].prefix_bits[5] = '1';
    prefix_table[15][9].prefix_bits[6] = '1';
    prefix_table[15][9].prefix_bits[7] = '1';
    prefix_table[15][9].prefix_bits[8] = '1';
    prefix_table[15][9].prefix_bits[9] = '1';
    prefix_table[15][9].prefix_bits[10] = '1';
    prefix_table[15][9].prefix_bits[11] = '1';
    prefix_table[15][9].prefix_bits[12] = '1';
    prefix_table[15][9].prefix_bits[13] = '1';
    prefix_table[15][9].prefix_bits[14] = '0';
    prefix_table[15][9].prefix_bits[15] = '1';
    prefix_table[15][9].prefix_bits[16] = '\0';
    prefix_table[15][9].tam_total = 25;

    prefix_table[15][10].prefix_bits[0] = '1';
    prefix_table[15][10].prefix_bits[1] = '1';
    prefix_table[15][10].prefix_bits[2] = '1';
    prefix_table[15][10].prefix_bits[3] = '1';
    prefix_table[15][10].prefix_bits[4] = '1';
    prefix_table[15][10].prefix_bits[5] = '1';
    prefix_table[15][10].prefix_bits[6] = '1';
    prefix_table[15][10].prefix_bits[7] = '1';
    prefix_table[15][10].prefix_bits[8] = '1';
    prefix_table[15][10].prefix_bits[9] = '1';
    prefix_table[15][10].prefix_bits[10] = '1';
    prefix_table[15][10].prefix_bits[11] = '1';
    prefix_table[15][10].prefix_bits[12] = '1';
    prefix_table[15][10].prefix_bits[13] = '1';
    prefix_table[15][10].prefix_bits[14] = '1';
    prefix_table[15][10].prefix_bits[15] = '0';
    prefix_table[15][10].prefix_bits[16] = '\0';
    prefix_table[15][10].tam_total = 26;
}

//Implementação baseada no exemplo fornecido em aula pelo professor
//Concatenação do códio com categoria --> coeficientes DC
int concat_Code(PREFIX prefix, unsigned int codigo, unsigned char categoria){
    unsigned int cod, mask=0;
    unsigned char tam = strlen(prefix.prefix_bits);

    cod = codigo;

    for (int i = (tam-1); i >= 0; i--){
        mask = 0;
        //a partir de qual posicao tenho q colocar o prefixo
        //lembrar que 'categoria' eh referente ao numero de bits na mantissa
        mask = pow(2, categoria);
        if (prefix.prefix_bits[i] == '1'){
            cod = cod | mask;
        }
        else{
            mask = ~mask;
            cod = cod&mask;
        }
        categoria++;
    }
    return(cod);
}

//Concatenação do código para coeficiente AC
int concat_Code_AC(PREFIX_AC prefix, unsigned int codigo, unsigned char categoria){
    unsigned int cod, mask=0;
    unsigned char tam = strlen(prefix.prefix_bits);

    cod = codigo;

    for (int i = (tam-1); i >= 0; i--){
        mask = 0;
        //a partir de qual posicao tenho q colocar o prefixo
        //lembrar que 'categoria' eh referente ao numero de bits na mantissa
        mask = pow(2, categoria);
        if (prefix.prefix_bits[i] == '1'){
            cod = cod | mask;
        }
        else{
            mask = ~mask;
            cod = cod&mask;
        }
        categoria++;
    }
    return(cod);
}

//Função para exibir um número inteiro no formato binário
void exibe_binario(int n, int tamanho){
    int r;
    int vetor[32];
    int i;

    // Utiliza um número de 32 bits como base para a conversão.
    for(int aux_i = 31; aux_i >= 0; aux_i--) {
        // Executa a operação shift right até a
        // última posição da direita para cada bit.
        r = n >> aux_i;

        // Por meio do "e" lógico ele compara se o valor
        // na posição mais à direita é 1 ou 0
        // e imprime na tela até reproduzir o número binário.
        if(r & 1) {
            vetor[aux_i] = 1;
            //printf("1");
        } else {
            vetor[aux_i] = 0;
            //printf("0");
        }
     }

     for(i = (tamanho-1); i >= 0; i--){
        printf("%d", vetor[i]);
     }

      printf("\n\n");
}

//Implementação baseada no exemplo fornecido em aula pelo professor
//Codificação por diferenças nos coeficientes DC
int **entropyCoding(double **dif, int num_lin, int num_col){

    int i, j;
    int num_blocos = (int)(num_lin/8)*(int)(num_col/8);
    int **codigos;

    codigos = (int **) calloc (num_blocos, sizeof(int *));
    if (codigos == NULL) {
        printf (" Erro: Memoria Insuficiente ");
    }
    /* aloca as colunas da matriz */
    for (int i = 0; i < num_blocos; i++ ) {
        codigos[i] = (int *) calloc (2, sizeof(int));
        if (codigos[i] == NULL) {
            printf (" Erro: Memoria Insuficiente ");
        }
    }

    int *diferencas;
    unsigned char categoria;
    PREFIX prefix_table[12];

    //Carrega tabela de prefixos
    define_PrefixTables(prefix_table);

    diferencas = (int *) malloc(num_blocos*sizeof(int));

    //Calculando as diferenças
    diferencas[0] = dif[0][0];
    //Calcula as diferenças para o Diferential Coding dos DCs
    for(i = 1; i < num_blocos; i++){
        diferencas[i] = (int)dif[i][0] - (int)dif[i-1][0];
    }

    for(i = 0; i < num_blocos; i++){

        //Identificando a categoria do número
        if(diferencas[i] == 0) categoria = 0;
        else if ((diferencas[i] == -1) || (diferencas[i] == 1))
            categoria = 1;
        else if ((diferencas[i] >= -3) && (diferencas[i] <= 3))
            categoria = 2;
        else if ((diferencas[i] >= -7) && (diferencas[i] <= 7))
            categoria = 3;
        else if ((diferencas[i] >= -15) && (diferencas[i] <= 15))
            categoria = 4;
        else if ((diferencas[i] >= -31) && (diferencas[i] <= 31))
            categoria = 5;
        else if ((diferencas[i] >= -63) && (diferencas[i] <= 63))
            categoria = 6;
        else if ((diferencas[i] >= -127) && (diferencas[i] <= 127))
            categoria = 7;
        else if ((diferencas[i] >= -255) && (diferencas[i] <= 255))
            categoria = 8;
        else if ((diferencas[i] >= -511) && (diferencas[i] <= 511))
            categoria = 9;
        else if ((diferencas[i] >= -1023) && (diferencas[i] <= 1023))
            categoria = 10;
        else if ((diferencas[i] >= -2047) && (diferencas[i] <= 2047))
            categoria = 11;

        int modulo = diferencas[i];
        //printf("\ndif: %d ", diferencas[i]);
        if(diferencas[i] < 0)
            modulo *= -1;

        unsigned int modulo2 = modulo;
        unsigned int codigo = 0;
        unsigned int mask = 1;

        //Obter o código em complemento de 1
        //Ver aula 18/05 para explicação
        for(j = 0; j < categoria; j++){
            codigo = modulo2 & mask;
            mask = (mask << 1) + 1;
        }
        if(diferencas[i] < 0)
            codigo = ~codigo;

            //Concatenando o código com a categoria
        codigos[i][0] = concat_Code(prefix_table[categoria], codigo, categoria);
        //pegando o tamanho total do código
        codigos[i][1] = (int)prefix_table[categoria].tam_total;

    }

    free(diferencas);
    diferencas = NULL;
    //São retornados o código do valor DC e o tamanho total desse código
    return codigos;
}

//Função para pré-codificar os coeficientes AC
//Identifica o número de zeros/categoria/valor do coeficiente
int ***pre_codifica_AC(double **diferencas, int num_lin, int num_col){

    int i, j;
    int num_blocos = (int)(num_lin/8)*(int)(num_col/8);

    int ***codigos;
    codigos = aloca_cubo_int(num_blocos, 63, 3);

    int categoria;
    int conta_zeros = 0;

    int aux_j = 0;
    //diferenças tem 64 colunas
    //codigos tem 63 pois a primeira posição do diferenças é o DC
    for(i = 0; i < num_blocos; i++){
        aux_j = 0;
        for(j=1; j < 64; j++){

            //Identificando o número de zeros da sequência
            if((int)diferencas[i][j] == 0){
                conta_zeros += 1;

                //Se chegou no fim da linha com zeros...
                if(j == 63){
                    codigos[i][aux_j][0] = 0;
                    codigos[i][aux_j][1] = 0;
                    codigos[i][aux_j][2] = (int)diferencas[i][j]; //Não faz diferença esse valor
                    conta_zeros = 0;
                }

            }
            else{
                //Enquanto contagem de zeros > 16 --> codifica extensão de zeros
                while(conta_zeros >= 16){
                    codigos[i][aux_j][0] = 15;
                    codigos[i][aux_j][1] = 0;
                    codigos[i][aux_j][2] = 0;
                    conta_zeros -= 16;
                    aux_j += 1;
                }

                //Pega categoria do valor
                int valor = (int) diferencas[i][j];
                if ((valor == -1) || (valor == 1)){
                     categoria = 1;
                }
                else if ((valor >= -3) && (valor <= 3)){
                    categoria = 2;
                }
                else if ((valor >= -7) && (valor <= 7)){
                    categoria = 3;
                }
                else if ((valor >= -15) && (valor <= 15)){
                    categoria = 4;
                }
                else if ((valor >= -31) && (valor <= 31)){
                    categoria = 5;
                }
                else if ((valor >= -63) && (valor <= 63)){
                    categoria = 6;
                }
                else if ((valor >= -127) && (valor <= 127)){
                    categoria = 7;
                }
                else if ((valor >= -255) && (valor <= 255)){
                    categoria = 8;
                }
                else if ((valor >= -511) && (valor <= 511)){
                    categoria = 9;
                }
                else if ((valor >= -1023) && (valor <= 1023)){
                    categoria = 10;
                }

                //Coloca num_zeros/categoria/valor na estrutura retornada
                codigos[i][aux_j][0] = conta_zeros;
                codigos[i][aux_j][1] = categoria;
                codigos[i][aux_j][2] = valor;
                conta_zeros = 0;
                aux_j += 1;
            }
        }
    }

    return codigos;
}

//Codificação AC
//Recebe zeros/categoria/valor ==> retorna codigo/tamanho
int ***codifica_AC(int ***codigos, int num_lin, int num_col){

    int i, j, k;
    int num_blocos = (int)(num_lin/8)*(int)(num_col/8);

    int ***codigos_bin;
    codigos_bin = aloca_cubo_int (num_blocos, 63, 2);

    //Matriz de prefixos AC
    PREFIX_AC **prefix_table;

    /* aloca as linhas da matriz */
    prefix_table = (PREFIX_AC **) calloc (16, sizeof(PREFIX_AC *));
    if (prefix_table == NULL) {
        printf ("** Erro: Memoria Insuficiente **");
    }
    /* aloca as colunas da matriz */
    for (int i = 0; i < 16; i++ ) {
        prefix_table[i] = (PREFIX_AC *) calloc (11, sizeof(PREFIX_AC));
        if (prefix_table[i] == NULL) {
            printf ("** Erro: Memoria Insuficiente **");
        }
    }

    define_PrefixTablesAC(prefix_table);

    int num_zeros, categoria, valor_ac, codigo;
    int EOB = 0;

    for(i = 0; i < num_blocos; i++){
        EOB = 0;

        for(j = 0; j < 63; j++){

            //Se ja atingiu o fim do bloco, o que tem a mais nao eh relevante
            if (EOB != 1){

                num_zeros = codigos[i][j][0];
                categoria = codigos[i][j][1];
                valor_ac = codigos[i][j][2];

                //binario = prefix_table[num_zeros][categoria];

                if (num_zeros == 0 && categoria == 0){
                    codigo = 0; //o que colocar aqui?... como eh fim de bloco, importa o q vem depois do prefixo(o valor do codigo, no caso)?
                    categoria = 0; //setando a categoria para 0, o prefixo comeca a ser concatenado ja na primeira posicao, ja que nao ha
                    //'codigo' relacionado com o EOB
                    codigos_bin[i][j][0] = concat_Code_AC(prefix_table[num_zeros][categoria], codigo, categoria);
                    codigos_bin[i][j][1] = prefix_table[num_zeros][categoria].tam_total;
                    EOB = 1;
                }
                else{
                    int modulo = valor_ac;
                    if(valor_ac < 0)
                        modulo *= -1;

                    unsigned int modulo2 = valor_ac;
                    unsigned int codigo = 0;
                    unsigned int mask = 1;

                    //Obter o código em complemento de 1
                    //Ver aula 18/05 para explicação
                    for(k = 0; k < categoria; k++){
                        codigo = modulo & mask;
                        mask = (mask << 1) + 1;
                    }
                    if(valor_ac < 0)
                        codigo = ~codigo;

                    codigos_bin[i][j][0] = concat_Code_AC(prefix_table[num_zeros][categoria], codigo, categoria);
                    codigos_bin[i][j][1] = prefix_table[num_zeros][categoria].tam_total;

                }

            }
        }
    }

    return codigos_bin;
}
