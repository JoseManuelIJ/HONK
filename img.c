#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

//Function that frees the dynamic memory allocated in the image struct
//Input: Pointer to the struct
//Output: The same structure
void freeImageStruct(Img* img){
    int i;
    for(i = 0; i < img->height; i++){
        free(img->pixelMatrix[i]);
        free(img->grayMatrix[i]);
        free(img->laplacedMatrix[i]);
        free(img->binaryMatrix[i]);
    }
    free(img->pixelMatrix);
    free(img->grayMatrix);
    free(img->laplacedMatrix);
    free(img->binaryMatrix);
}

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
void imageProcessingMain(char* fileName, int** kernel, int u, int n, int showResults){
    Img actualImage;
    char *outFileName = outputFileName(fileName);
    //Read the image
    readImage(&actualImage, fileName);
    //Calculate gray pixel taking into account it's RGB values
    grayScale(&actualImage);
    //Apply the mask using laplace
    laplace(&actualImage,kernel);
    //Binarization for each pixel
    binarization(&actualImage, u);
    //Give the proper classification for the pixel if showResults says so
    classification(&actualImage, n, showResults, fileName);
    //Write the jpg file to disk
    writeImage(&actualImage, outFileName);
    //Free the memory of the struct that allocates all the information for the images and it's stages
    freeImageStruct(&actualImage);
    //Free the memory of the fileName
    free(outFileName);
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
    //printf("channels = %d\n", channels);
    actualImage->dataSize = width * height * channels;
    actualImage->pixelMatrix = (unsigned char**) malloc(sizeof(unsigned char*) * height);
    for(int i = 0; i < height; i++){
        actualImage->pixelMatrix[i] = (unsigned char*) malloc(sizeof(unsigned char) * width * channels);
    }
    actualImage->grayMatrix = (float **) malloc(sizeof(float*) * height);
    actualImage->laplacedMatrix = (float**) malloc(sizeof(float*) * height);
    actualImage->binaryMatrix = (float**) malloc(sizeof(float*) * height);
    for(int i = 0; i < height; i++){
        actualImage->grayMatrix[i] = (float*) malloc(sizeof(float) * width);
        actualImage->laplacedMatrix[i] = (float*) malloc(sizeof(float) * width);
        actualImage->binaryMatrix[i] = (float*) malloc(sizeof(float) * width);
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

//Function that writes the image to disk
//Input: the struct that stores the initial jpg data, the name of the current file
//Output: The image after all stages in disk
void writeImage(Img *img, char* outFileName){
    int grayChannels = img->channels == 4 ? 2 : 1;
    //printf("%d, width = %d, height = %d\n", grayChannels, img->width, img->height);
    size_t grayImgSize = img->width * img->height * grayChannels;
    unsigned char *grayImg = malloc(grayImgSize);
    unsigned char *grayImgP = grayImg;
    for(int i = 0; i < img->height; i++){
        for(int j = 0; j < img->width; j++){
            *grayImgP = (uint8_t) img->binaryMatrix[i][j];
            grayImgP += grayChannels;
            //printf("i = %d, j = %d\n", i, j);
        }
    }
    stbi_write_jpg(outFileName, img->width, img->height, grayChannels, grayImg, /*100*/ img->width * grayChannels);
}

//Function that generates the output file name
//Input: The name of the input file
//Output: The generated output file name
char *outputFileName(char* fileName){
    char *outFileName = (char*) malloc(sizeof(char) * 50);
    char *imgNumberStr = (char*) malloc(sizeof(char) * 4);
    int imageNumber;
    sscanf(fileName, "imagen_%d.jpg", &imageNumber); //Get the number of image from fileName generated at the main.c file
    sprintf(imgNumberStr, "%d", imageNumber); //Put that number into a string
    strcpy(outFileName, "imagen_"); //Generate the beginning of the output file 
    strcat(outFileName, imgNumberStr); //Append the number in string format
    strcat(outFileName, "_out.jpg"); //Append the last part of the file name
    return outFileName;
}