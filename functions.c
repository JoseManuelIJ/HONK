#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "functions.h"



//Retuns the r, g or b value of a pixel in the image coordinate [row, column] given a img struct
//Input: A pointer to the img struct, the row and column of the pixel to get it's value and a char ('r', 'g' or 'b')
//Output: the r, g or b value of the pixel
int getPixelRGBValues(Img * img, int row, int column, char rgb){
    int pixelValue = (rgb == 'r' ? img->pixelMatrix[row][column * 3] : (rgb == 'g' ? img->pixelMatrix[row][(column * 3) + 1] : (rgb == 'b' ? img->pixelMatrix[row][(column * 3) +2] : 0)) );
    return pixelValue;
}

//applies the gray scale to the image in the input
//Input: A pointer to the img struct
void grayScale(Img* actualImage){
    int i,j,Y,r,g,b;
    int width = actualImage->width, height = actualImage->height;
    printf("hola");
    for(i=0; i< height; i++)
    {

        for(j=0;j< width; j+=3)
        {
            r=getPixelRGBValues(actualImage,i,j,'r');
            g=getPixelRGBValues(actualImage,i,j,'g');
            b=getPixelRGBValues(actualImage,i,j,'b');
            Y=r*0.3 + g*0.59 + b*0.11;
            actualImage->grayMatrix[i][j] =Y;
        }
    }
}
int valor(Img* actualImage,int i,int j) 
{
    int sum;
    int MC[3][3]= {{0,1,0},
        {1,-4,1},
        {0,1,0}    
        };
    sum=0;
    for(int f=0;f<3;f++)
    {
        for(int c=0;c<3;c++)
        {
            /*sum=sum+actualImage->[i+f-1][j+c-1] * MC[f][c]*/;
        }
    }
    return sum;
}
void laplace(Img* actualImage, int** kernel)
{
    
}