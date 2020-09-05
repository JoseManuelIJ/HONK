//Image test
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "img.h"

//Input: ints a and b
//Function: a ^ b
//Returns: int a ^ b
int powOf(int a, int b)
{
    int i, result = 1;
    if(b == 0)
    {
        return 1;
    }
    else if(b > 0)
    {
        for(i = 0; i < b; i++)
        {
            result *= a;
        }
    }
    else
    {
        for(i = b; i < 0; i++)
        {
            result /= a;
        }
    }
    
    return result;
}

//Input: Hex number (without 0x) and the lenght of the string
//Function: Calculates the value of the number in it's decimal equivalent
//Output: Unsigned int with the decimal value
unsigned int hexToDec(unsigned char hex[], int length)
{
    int  i, result = 0, exp = 0;
    unsigned int tempHex;
    result = 0;
    exp = length - 1;
    for(i = 0; i < length; i++)
    {
        tempHex = (int)hex[i];
        result += tempHex * powOf(256, exp);
        exp -= 1;
    }
    return result;
}

//Input: Bidimensional array for image (pointer) and Img pointer to extract the image data
//Function: Sets an array with the pixel values of the image
//Output: None (void)
void setImage(float **imgMatrix, Img *imgFile)
{
    int i, j, pos = 0;
    char pixel[3];
    for(i = 0; i < imgFile->height; i++)
    {
        for(j = 0; j < imgFile->width; j++)
        {
            pixel[0] = imgFile->data[pos];
            pixel[1] = imgFile->data[pos + 1];
            pixel[2] = imgFile->data[pos + 2];
            imgMatrix[i][j] = (float) hexToDec(pixel, 3);
            pos += 3;
        }
    }
}

//Input: Bidimensional array for image (pointer) and Img pointer to extract the image data
//Function: Sets an array with the data from the Img struct
//Output: None (void)
void setImgMatrix(float **imgMatrix, Img *imgFile)
{
    int i, j;
    for(i = 0; i < imgFile->height; i++)
    {
        for(j = 0; j < imgFile->width; j++)
        {
            //imgMatrix[i][j] = (float) hexToDec(pixel, 3);
            //imgMatrix[i][j] = 
        }
    }
}

//Input: The bidimensional array as float array, a double array for the final convolution, kernel mask and Img pointer
//Function: Perfoms the convolution according to the first image matrix and kernel and stores it in the second image matrix
//Output: Void (none)
void convolution(float **imgMatrix, float **imgMatrix2, double kernel[3][3], Img *imgFile)
{
    int i, j;
    for(i = 0; i < imgFile->height; i++)
    {
        for(j = 0; j < imgFile->width; j++)
        {
            if(i == 0 || j == 0)
            {
                imgMatrix2[i][j] = 0;
            }
            else if(i == imgFile->height - 1 || j == imgFile->width - 1)
            {
                imgMatrix2[i][j] = 0;
            }
            else
            {
                //printf("pro calcular i = %d j = %d\n", i, j);
                imgMatrix2[i][j] = (imgMatrix[i-1][j-1] * kernel[0][0] + imgMatrix[i-1][j] * kernel[0][1] + imgMatrix[i-1][j+1] * kernel[0][2]
                                  + imgMatrix[i][j-1] * kernel[1][0] + imgMatrix[i][j] * kernel[1][1] + imgMatrix[i+1][j] * kernel[1][2]
                                  + imgMatrix[i+1][j-1] * kernel[2][0] + imgMatrix[i+1][j] * kernel[2][1] + imgMatrix[i+1][j+1] * kernel[2][2]
                                   ) / 9;
            }
        }
    }
}

//Input: The bidimensional array for the image, Img struct pointer
//Function: Verifies if all pixel values are non negative, if they are, changes them to 0
//Output: Rectified image
void rectification(Img *imgFile)
{
    int i,j;
    for(i = 0; i < imgFile->height; i++)
    {
        for(j = 0; j < imgFile->width; j++)
        {
            if(imgFile->image2[i][j] < 0)
            {
                imgFile->image2[i][j] = 0;
            }
        }
    }
}

//Input: Original image, struct pointer
//Function: Applies max pooling to the image and stores it in the new one
//Output: None (Void)
void pooling(float **imgMatrix, Img *imgFile)
{
    int i, j, x, y, width = imgFile->width, height = imgFile->height, fWidth = 2, fHeight = 2;
    int poolImgW, poolImgH, pW = 0, pH = 0;
    float maxTemp;
    if(width % fWidth == 0)
    {
        poolImgW = width / fWidth;
    }
    else
    {
        poolImgW = (width / fWidth) + 1; 
    }
    if(height % fHeight == 0)
    {
        poolImgH = height / fHeight;
    }
    else
    {
        poolImgH = (height / fHeight) + 1;
    }
    imgFile->poolImg = (float**) malloc(sizeof(float*) * poolImgH);
    for(i = 0; i < poolImgH; i++)
    {
        imgFile->poolImg[i] = (float*) malloc(sizeof(float) * poolImgW);
    }
    imgFile->poolWidth = poolImgW;
    imgFile->poolHeight = poolImgH;
    for(i = 0; i < height; i += fHeight)
    {
        for(j = 0; j < width; j += fWidth)
        {
            maxTemp = imgMatrix[i][j];
            for(y = i; y < height || y < i + fHeight; y++)
            {
                for(x = j; x < width || x < j + fWidth; x++)
                {
                    //printf("%f\n", imgMatrix[y][x]);
                    if(imgMatrix[y][x] > maxTemp)
                    {
                        maxTemp = imgMatrix[y][x];
                    }
                }
            }
            imgFile->poolImg[pH][pW];
            pW++;
        }
        pW = 0;
        pH++;
    }
}

//Input: matrix with pixel values, Img struct pointer and treshold percentage
//Function: Checks each pixel value and counts how many of them are black
//Output: 1 if it's classified as nearly black, 0 in the opposite case
int nearlyBlack(float** imgMatrix, Img *imgFile, float percentage)
{
    int i, j;
    float max = (float)imgFile->width * (float)imgFile->height, blackP = 0.0, finalP;
    for(i = 0; i < imgFile->height; i++)
    {
        for(j = 0; j < imgFile->width; j++)
        {
            if(imgMatrix[i][j] == 0.0)
            {
                blackP += 1;
            }
        }
    }
    finalP = (blackP * 100) / max;
    if(finalP > percentage)
    {
        return 1; //Nearly black
    }
    return 0; //Opposite case
}

//Input:Bidimensional array for the image, Img pointer
//Function: Prints the matrix
//Output: None (Void)
void printMat(float** imgMatr, Img *imgFile)
{
    int i,j;
    for(i = 0; i < imgFile->height; i++)
    {
        for(j = 0; j < imgFile->width; j++)
        {
            printf("%f ", imgMatr[i][j]);
        }
        printf("\n");
    }
}

//Input: String with binary/Hex data, array size
//Function: Prints the data in hex
//Output: None (Void)
void stringToHex(unsigned char* str, unsigned int realSize)
{
    int i;
    printf("| ");
    for(i = 0; i < realSize; i++)
    {        
        printf("%x ", (unsigned int)str[i]);
        if(i % 16 == 0 && i != 0) printf("\n  ");
    }
    printf("|\n");
}

//Input: Data from IHDR chunk, Img struct pointer
//Function: Stores all important data in a Img struct
//Output: None (Void)
void getDimensions(char *buffer, Img *imgFile)
{
    unsigned char strWidth[4], strHeight[4], bitDepth[1], cType[1], cMethod[1];
    int i;
    for(i = 0; i < 4; i++)
    {
        strWidth[i] = buffer[i];
        strHeight[i] = buffer[i + 4];           
    }
    bitDepth[0] = buffer[8];
    cType[0] = buffer[9];
    cMethod[0] = buffer[10];
    imgFile->width = hexToDec(strWidth, 4);
    imgFile->height = hexToDec(strHeight, 4);
    imgFile->bitDepth = hexToDec(bitDepth, 1);
    imgFile->colorType = hexToDec(cType, 1);
    imgFile->cMethod = hexToDec(cMethod, 1);
}

//Input: Data from IDAT chunk, Img struct pointer, lenght of the data
//Function: Stores the IDAT data to the struct, appends data from IDAT chunk if one or more have already been stored
//Output: None (Void)
void getData(unsigned char* buffer, Img *imgFile, int lenght)
{
    int i;
    if(imgFile->idatChunks == 0)
    {
        imgFile->data = (char*) malloc(sizeof(char) * lenght);
        for(i = 0; i < lenght; i++)
        {
            imgFile->data[i] = buffer[i];
        }
    }
    else
    {
        //strcat(imgFile, buffer);
    }    
    imgFile->dataSize += lenght;
}

//Input: File descriptor to read binary file from and Img pointer
//Function: Reads chunks and stores information as it goes into the struct
//Output: None (Void)
char* readChunk(int fd, Img *imgFile)
{
    int size, lenght;
    unsigned char lenStr[10], *chunkName, *buffer,  crc[4];

    chunkName = (char*) malloc(sizeof(char) * 4);
    size = read(fd, lenStr, 4);//First, get the chunk data's lenght
    lenght = hexToDec(lenStr, 4);
    
    if((unsigned int)lenStr[0] == 0 && (unsigned int)lenStr[1] == 0 && (unsigned int)lenStr[2] == 0 && (unsigned int)lenStr[3] == 0)
    {
        strcpy(chunkName, "IEND");
        return chunkName;
    }

    size = read(fd, chunkName, 4);//Get the chunk name

    if(lenght > 0)//IEND has data lenght 0
    {
        buffer = (char*) malloc(sizeof(char) * (lenght));
        size = read(fd, buffer, lenght);//Store chunk data

        size = read(fd, crc, 4);//Get crc
        if(strcmp(chunkName, "IHDR") == 0)
        {
            getDimensions(buffer, imgFile);
        }
        if(strcmp(chunkName, "IDAT") == 0)
        {
            getData(buffer, imgFile, lenght);
        }
        free(buffer);
    }
    
    return chunkName;
}

//Input: File descriptor to read from
//Function: Start reading PNG data and stops at the last chunk (IEND)
//Output: Img structre with info stored
Img readPNG(int fd)
{
    unsigned char *chunkName = (unsigned char*) malloc(sizeof(char) * 4);
    unsigned int width, height;
    Img imgFile;
    
    imgFile.dataSize = 0;
    imgFile.idatChunks = 0;
    
    while(strcmp(chunkName, "IEND") != 0)
    {
        chunkName = readChunk(fd, &imgFile);
        if(strcmp(chunkName, "IEND") == 0)
        {
            break;
        }
    }
    free(chunkName);
    return imgFile;
}

//Input: Name of the image file to read
//Function: Starts reading the png file information
//Output: Img struct with all data stored
Img startLecture(char *filename)
{
    int fd, i = 0;
    char buffer[256];
    int size;
    Img imageFile;

    fd = open(filename, O_RDONLY);

    //First 8 bytes of header
    for(i; i < 8; i++)
    {
        size = read(fd, buffer, 1);
        buffer[0] = 0;
    }

    imageFile = readPNG(fd);
    close(fd);
    
    return imageFile;
}

