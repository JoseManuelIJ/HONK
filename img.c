#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "img.h"
#include "functions.h"

//stb_image libraries
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

//Stores the RGB values of a pixel in a matrix in a struct
//Input: Pointer to the img struct, the raw img data
//Output: None
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

//Function where the image processing begins, including pooling, mask filtering, image binarization and classificcation
//Input: the filename of the image to start processing it
//Output: None, even though the classification process should print the image results
void imageProcessingMain(char* fileName, int** kernel){
    Img actualImage;
    //Read
    readImage(&actualImage, fileName);
    grayScale(&actualImage);
    laplace(&actualImage,kernel);
    binarization(&actualImage, 150);
    //Apply bunch of stuff
    //???
    //Profit!
}

//Function that reads and oipens the image file to extract the data and assign it to an image struct
//Input: Pointer to the image struct and the filename
//Output: The image data (size, color channels, pixel values) stored in the struct
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
    actualImage->grayMatrix = (float **) malloc(sizeof(float*) * height);
    actualImage->laplacedMatrix = (float**) malloc(sizeof(float*) * height);
    actualImage->binaryMatrix = (float**) malloc(sizeof(float*) * height);
    for(int i = 0; i < width; i++){
        actualImage->grayMatrix[i] = (float*) malloc(sizeof(float*) * width);
        actualImage->laplacedMatrix[i] = (float*) malloc(sizeof(float*) * width);
        actualImage->binaryMatrix[i] = (float*) malloc(sizeof(float*) * width);
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