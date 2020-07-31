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
    //float ** image2;
    //float **poolImg;
    //unsigned int poolWidth;
   // unsigned int poolHeight;
}Img;
#endif