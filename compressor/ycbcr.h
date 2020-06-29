#ifndef YCBCR_H
#define YCBCR_H

void converte_YCbCr(unsigned char **Y, unsigned char **Cb, unsigned char **Cr, unsigned char **B, unsigned char **G, unsigned char **R, int w, int h);
void amostra_YCbCr_4_2_0(unsigned char **Cb_amostr, unsigned char **Cr_amostr, unsigned char **Cb, unsigned char **Cr, int w, int h);

#endif // YCBCR
