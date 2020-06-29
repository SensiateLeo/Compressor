void converte_YCbCr(unsigned char **Y, unsigned char **Cb, unsigned char **Cr, unsigned char **B, unsigned char **G, unsigned char **R, int w, int h){
    int i , j;
    unsigned char aux;
    for (i=0; i < h; i++){
        for(j=0; j < w; j++){
            aux = B[i][j];
            Y[i][j] = 0.299*R[i][j] + 0.587*G[i][j] + 0.114*aux;
            Cb[i][j] = 0.564*(aux - Y[i][j]);
            Cr[i][j] = 0.713*(R[i][j] - Y[i][j]);
        }
    }
}

void amostra_YCbCr_4_2_0(unsigned char **Cb_amostr, unsigned char **Cr_amostr, unsigned char **Cb, unsigned char **Cr, int w, int h){
    int i_amostr, j_amostr;

    i_amostr = 0;
    for (int i = 0; i < h; i+=2){
        j_amostr=0;
        for (int j = 0; j < w; j+=2){
            Cb_amostr[i_amostr][j_amostr] = Cb[i][j];
            Cr_amostr[i_amostr][j_amostr] = Cr[i][j];
            j_amostr++;
        }
        i_amostr++;
    }
}
