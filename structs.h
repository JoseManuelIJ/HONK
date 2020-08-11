//Img struct
#ifndef _structs_h
#define _structs_h

typedef struct
{
    unsigned int width;
    unsigned int height;
    unsigned int channels;
    size_t dataSize;
    unsigned char *data;
    unsigned char **pixelMatrix;
    float **grayMatrix;
    float **laplacedMatrix;
    float **binaryMatrix;
}Img;
#endif