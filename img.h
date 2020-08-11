//#include "structs.h"
#ifndef _img_h
#define _img_h

void readImage(Img* actualImage, char* fileName);
void imageProcessingMain(char* fileName, int** kernel);
void getImageData(Img *actualImage, unsigned char* img);
#endif