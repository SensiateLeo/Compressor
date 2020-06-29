#ifndef QUANTIZACAO_H
#define QUANTIZACAO_H

static const unsigned char mat_quantizacao_luminancia[8][8] = {{16, 11, 10, 16,  24,  40,  51,  61},
                                                            {12, 12, 14, 19,  26,  58,  60,  55},
                                                            {14, 13, 16, 24,  40,  57,  69,  56},
                                                            {14, 17, 22, 29,  51,  87,  80,  62},
                                                            {18, 22, 39, 56,  68, 109, 103,  77},
                                                            {24, 35, 55, 64,  81, 104, 113,  92},
                                                            {79, 64, 78, 87, 103, 121, 120, 101},
                                                            {72, 92, 95, 98, 112, 100, 103,  99}};


static const unsigned char mat_quantizacao_cromancia[8][8] = {{17, 18, 24, 47, 99, 99, 99, 99},
                                                           {18, 21, 26, 66, 99, 99, 99, 99},
                                                           {24, 26, 56, 99, 99, 99, 99, 99},
                                                           {47, 66, 99, 99, 99, 99, 99, 99},
                                                           {99, 99, 99, 99, 99, 99, 99, 99},
                                                           {99, 99, 99, 99, 99, 99, 99, 99},
                                                           {99, 99, 99, 99, 99, 99, 99, 99},
                                                           {99, 99, 99, 99, 99, 99, 99, 99}};


void aplica_quantizacao(const unsigned char mat_quantizacao[8][8], int ***blocosDCT, int num_lin, int num_col, int *** Blocos_Quantizados);
#endif // QUANTIZACAO
