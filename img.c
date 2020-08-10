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

//Retuns the r, g or b value of a pixel in the image coordinate [row, column] given a img struct
//Input: A pointer to the img struct, the row and column of the pixel to get it's value and a char ('r', 'g' or 'b')
//Output: the r, g or b value of the pixel
int getPixelRGBValues(Img * img, int row, int column, char rgb){
    int pixelValue = (rgb == 'r' ? img->pixelMatrix[row][j * 3] : (rgb == 'g' ? img->pixelMatrix[row][(j * 3) + 1] : (rgb == 'b' ? img->pixelMatrix[row][(j * 3) +2] : 0)) );
    return pixelValue;
}

//Function where the image processing begins, including pooling, mask filtering, image binarization and classificcation
//Input: the filename of the image to start processing it
//Output: None, even though the classification process should print the image results
void imageProcessingMain(char* fileName){
    Img actualImage;
    //Read
    readImage(&actualImage, fileName);

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