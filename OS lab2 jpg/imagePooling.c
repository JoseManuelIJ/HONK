#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
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


	// Transmission start.
	pipe(piped);
	pid = fork();

	// Child section
	if (pid == 0)
	{
		close(piped[WRITE]);
		dup2(piped[READ], STDIN_FILENO);

		char *args[] = {(char *)"imageClassification", NULL};
		execvp("./imageClassification", args);
		perror("exec failed");
		return 1;
	}

	else
	{
		close(piped[READ]);

		// Send global args.
		sprintf(str, "%d %d %d", imgNumber, threshold, skipAnalysis);
		write(piped[WRITE], str, 128);
	}

	while(tempN < imgNumber)
	{

		// Read image params.
		read(READ, str, 128);
		sscanf(str, "%u %u %u", &imageFile.width, &imageFile.height, &imageFile.dataSize);

		imageFile.image2 = (float**)malloc(sizeof(float*) * imageFile.height);
		for(i = 0; i < imageFile.height; i++)
		{
			imageFile.image2[i] = (float*) malloc(sizeof(float) * imageFile.width);
		}

		// Read image.
		for(i = 0; i < imageFile.height; i++)
		{
			for(j = 0; j < imageFile.width; j++)
			{
				read(READ, str, 128);
				sscanf(str, "%f", &imageFile.image2[i][j]);
			}
		}



		pooling(imageFile.image2, &imageFile);
		tempN++;


		// Send pooled image and new params.
		sprintf(str, "%u %u", imageFile.poolHeight, imageFile.poolWidth);
		write(piped[WRITE], str, 128);

		for(i = 0; i < imageFile.poolHeight; i++)
		{
			for(j = 0; j < imageFile.poolWidth; j++)
			{
				sprintf(str, "%f", imageFile.poolImg[i][j]);
				write(piped[WRITE], str, 128);
			}

			free(imageFile.poolImg[i]);
		}

		free(imageFile.poolImg);

	}

	close(piped[WRITE]); 

 	pid_t wpid = waitpid(pid, NULL, 0);

	return 0;
}