#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "img.h"

//stb_image libraries
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void getImageData(Img *actualImage, unsigned char* img){
    unsigned char *p;
    int i, j;
    int width = actualImage->width, height = actualImage->height, channels = actualImage->channels;

    p = img;
    for(i = 0; i < height; i++){
        //DEBUG printf("|");
        for(j = 0; j < width * channels; j += 3){
            actualImage->pixelMatrix[i][j] = (uint8_t)*p; //Red value of current pixel
            actualImage->pixelMatrix[i][j + 1] = (uint8_t)(*p + 1);//Blue value of current pixel
            actualImage->pixelMatrix[i][j + 2] = (uint8_t)(*p + 2);//Green value of current pixel
            p += 3;
            //DEBUG printf("[%d, %d, %d]", actualImage->pixelMatrix[i][j], actualImage->pixelMatrix[i][j + 1], actualImage->pixelMatrix[i][j + 2]);
        }
        //DEBUG printf("|\n");
    }
}

int getPixelRGBValues(Img * img, int row, int column, char rgb){
    int pixelValue = (rgb == 'r' ? img->pixelMatrix[i][j] : (rgb == 'g' ? img->pixelMatrix[i][j + 1] : img->pixelMatrix[i][j+2]));
}

void imageProcessingMain(char* fileName){
    Img actualImage;
    //Read
    readImage(&actualImage, fileName);

    //Apply bunch of stuff
    //???
    //Profit!
}

void readImage(Img* actualImage, char* fileName){
    int width, height, channels;
    unsigned char *img = stbi_load(fileName, &width, &height, &channels, 0);

    if(img == NULL){
        printf("Error loading %s\n", fileName);
        exit(1);
    }

    //DEBUG printf("%s width is %d, height is %d and it's number of channels is %d\n", fileName, width, height, channels);
    actualImage->width = width;
    actualImage->height = height;
    actualImage->channels = channels;
    actualImage->dataSize = width * height * channels;
    actualImage->pixelMatrix = (unsigned char**) malloc(sizeof(unsigned char*) * height);
    for(int i = 0; i < height; i++){
        actualImage->pixelMatrix[i] = (unsigned char*) malloc(sizeof(unsigned char) * width * channels);
    }

    getImageData(actualImage, img);
    /*DEBUG
    int k = 0;
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            printf("%c ", img[k]);
            k++;
        }
        printf("\n");
    }
    */
    free(img);
}
/*
int main(){
    imageProcessingMain("imagen_1.jpg");
    return 0;
}*/