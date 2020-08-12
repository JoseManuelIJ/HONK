//functions.h
#ifndef _functions_h
#define _functions_h

void grayScale(Img* actualImage);
int getPixelRGBValues(Img * img, int row, int column, char rgb);
void laplace(Img* actualImage, int** kernel);
void binarization(Img* actualImage, int threshold);
void classification(Img* actualImage, int threshold, int showResults, char* fileName);
#endif