//Code from: http://www.vbforums.com/showthread.php?261522.html

#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    unsigned short bfType;  //specifies the file type
    unsigned int bfSize;  //specifies the size in bytes of the bitmap file
    unsigned short bfReserved1;  //reserved; must be 0
    unsigned short bfReserved2;  //reserved; must be 0
    unsigned int bfOffBits;  //species the offset in bytes from the bitmapfileheader to the bitmap bits
}BITMAPFILEHEADER;

typedef struct
{
    unsigned int biSize;  //specifies the number of bytes required by the struct
    int biWidth;  //specifies width in pixels
    int biHeight;  //species height in pixels
    unsigned short biPlanes; //specifies the number of color planes, must be 1
    unsigned short biBitCount; //specifies the number of bit per pixel
    unsigned int biCompression;//spcifies the type of compression
    unsigned int biSizeImage;  //size of image in bytes
    int biXPelsPerMeter;  //number of pixels per meter in x axis
    int biYPelsPerMeter;  //number of pixels per meter in y axis
    unsigned int biClrUsed;  //number of colors used by th ebitmap
    unsigned int biClrImportant;  //number of colors that are important
}BITMAPINFOHEADER;

unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader){
    FILE *filePtr; //our file pointer
    BITMAPFILEHEADER bitmapFileHeader; //our bitmap file header
    unsigned char *bitmapImage;  //store image data
    int imageIdx=0;  //image index counter
    unsigned char tempRGB;  //our swap variable

    //open filename in read binary mode
    filePtr = fopen(filename,"rb");
    if (filePtr == NULL)
    return NULL;

    //read the bitmap file header
    fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER),1,filePtr);

    //verify that this is a bmp file by check bitmap id
    if (bitmapFileHeader.bfType !=0x4D42)
    {
        printf("Não é um arquivo bmp meu chapa...");
        fclose(filePtr);
        return NULL;
    }
    else{
        printf("Eh um arquivo bitmap, realmente... %s", &bitmapFileHeader.bfType);
    }

    //read the bitmap info header
    fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER),1,filePtr);

    //move file point to the begging of bitmap data
    fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

    //allocate enough memory for the bitmap image data
    bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);

    //verify memory allocation
    if (!bitmapImage)
    {
        free(bitmapImage);
        fclose(filePtr);
        return NULL;
    }

    //read in the bitmap image data
    fread(bitmapImage,bitmapInfoHeader->biSizeImage,1,filePtr);

    //make sure bitmap image data was read
    if (bitmapImage == NULL)
    {
        fclose(filePtr);
        return NULL;
    }

    //swap the r and b values to get RGB (bitmap is BGR)
    for (imageIdx = 0;imageIdx < bitmapInfoHeader->biSizeImage;imageIdx+=3)
    {
        tempRGB = bitmapImage[imageIdx];
        bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
        bitmapImage[imageIdx + 2] = tempRGB;
    }

    //close file and return bitmap iamge data
    fclose(filePtr);
    return bitmapImage;
}

int main_old(){
    BITMAPINFOHEADER bitmapInfoHeader;
    unsigned char *bitmapData;
    bitmapData = LoadBitmapFile("bmp_sample.bmp",&bitmapInfoHeader);
    //now do what you want with it, later on i will show you how to display it in a normal window
}

