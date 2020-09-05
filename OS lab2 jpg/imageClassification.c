#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "structs.h"
#include "img.h"

#define READ 0
#define WRITE 1

int main(int argc, char *argv[])
{
	// Transmission statements
	int piped[2];
	pid_t pid;

	// Image pooling statements
	int i, j;
	char str[128];
	int tempN = 0;
	int imgNumber, threshold, skipAnalysis;
	Img imageFile;


	// Read global args.
	read(READ, str, 128);
	sscanf(str, "%d %d %d", &imgNumber, &threshold, &skipAnalysis);


	int results[imgNumber];

	while(tempN < imgNumber)
	{
		// Read image params.
		read(READ, str, 128);
		sscanf(str, "%u %u", &imageFile.poolHeight, &imageFile.poolWidth);

		imageFile.poolImg = (float**)malloc(sizeof(float*) * imageFile.poolHeight);
		for(i = 0; i < imageFile.poolHeight; i++)
		{
			imageFile.poolImg[i] = (float*) malloc(sizeof(float) * imageFile.poolWidth);
		}

		// Read image.
		for(i = 0; i < imageFile.poolHeight; i++)
		{
			for(j = 0; j < imageFile.poolWidth; j++)
			{
				read(READ, str, 128);
				sscanf(str, "%f", &imageFile.poolImg[i][j]);
			}
		}

		results[tempN] = nearlyBlack(imageFile.poolImg, &imageFile, (float) threshold);

		tempN++;
	}

	if(skipAnalysis == 1)
	{
		printf("Image      |   nearly black\n");

		for(int i = 0; i < imgNumber; i++)
		{
			if(results[i])
				printf("imagen_%d   |   yes\n", i+1);

			else
				printf("imagen_%d   |   no\n", i+1);
		}
	}

	return 0;
}