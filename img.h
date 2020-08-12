//#include "structs.h"
#ifndef _img_h
#define _img_h

void readImage(Img* actualImage, char* fileName);
void imageProcessingMain(char* fileName, int** kernel, int u, int n, int showResults);
void getImageData(Img *actualImage, unsigned char* img);
void freeImageStruct(Img* img);
void writeImage(Img *img, char* outFileName);
char *outputFileName(char* fileName);
#endif