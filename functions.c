#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "functions.h"

void greyScale(Img* actualImage){
    int i,j;
    int width = actualImage->width, height = actualImage->height;
    //channels = actualImage->channels;
    
    for(i=0; i< height; i++)
    {
        for(j=0;j< width; j+=3)
        {
            ;
        }
    }
}

//Retuns the r, g or b value of a pixel in the image coordinate [row, column] given a img struct
//Input: A pointer to the img struct, the row and column of the pixel to get it's value and a char ('r', 'g' or 'b')
//Output: the r, g or b value of the pixel
int getPixelRGBValues(Img * img, int row, int column, char rgb){
    int pixelValue = (rgb == 'r' ? img->pixelMatrix[row][column * 3] : (rgb == 'g' ? img->pixelMatrix[row][(column * 3) + 1] : (rgb == 'b' ? img->pixelMatrix[row][(column * 3) +2] : 0)) );
    return pixelValue;
}