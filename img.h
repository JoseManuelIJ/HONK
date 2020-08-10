//img.h
#ifndef _img_h
#define _img_h

void readImage(Img* actualImage, char* fileName);
void imageProcessingMain(char* fileName);
void getImageData(Img *actualImage, unsigned char* img);
int getPixelRGBValues(Img * img, int row, int column, char rgb);
#endif