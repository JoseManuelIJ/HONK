//Img struct
#ifndef _structs_h
#define _structs_h
typedef struct
{
    unsigned int width;
    unsigned int idatChunks;
    unsigned int height;
    unsigned int dataSize;
    unsigned char *data;
    unsigned int bitDepth;
    unsigned int colorType;
    unsigned int cMethod;
    float ** image2;
    float **poolImg;
    unsigned int poolWidth;
    unsigned int poolHeight;
}Img;
#endif