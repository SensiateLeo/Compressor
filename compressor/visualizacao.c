#include "leitura_escrita.h"

//Exibindo informações do cabeçalho da imagem
void exibeInformacoesCabecalhos(BMPFILEHEADER FH, BMPINFOHEADER IH){
    printf("\n\n===========================\n");
    printf("BMP File Header");
    printf("\n===========================");
    printf("\nbfType: %s", (char *)&FH.bfType);
    printf("\nbfSize: %d", FH.bfSize);
    printf("\nbfReserved1: %d", FH.bfReserved1);
    printf("\nbfReserved2: %d", FH.bfReserved2);
    printf("\nbfOffBits: %d", FH.bfOffBits);

    printf("\n===========================\n");
    printf("BMP Info Header");
    printf("\n===========================");
    printf("\nbiSize: %d", IH.biSize);
    printf("\nbiWidth: %d", IH.biWidth);
    printf("\nbiHeight: %d", IH.biHeight);
    printf("\nbiPlanes: %d", IH.biPlanes);
    printf("\nbiBitCount: %d", IH.biBitCount);
    printf("\nbiCompression: %d", IH.biCompression);
    printf("\nbiSizeImage: %d", IH.biSizeImage);
    printf("\nbiXPelsPerMeter: %d", IH.biXPelsPerMeter);
    printf("\nbiYPelsPerMeter: %d", IH.biYPelsPerMeter);
    printf("\nbiClrUsed: %d", IH.biClrUsed);
    printf("\nbiClrImportant: %d", IH.biClrImportant);
}
