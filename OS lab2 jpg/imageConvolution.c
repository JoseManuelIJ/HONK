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

	// image convolving statements
	int i, j;
	char str[128];
	double kernel[3][3];
	float **imgMatrix, **imgMatrix2;
	int imgNumber, threshold, skipAnalysis, tempN = 0;
	Img imageFile;

	// Read kernel;
	read(READ, str, 128);
	sscanf(str, "%lf %lf %lf %lf %lf %lf %lf %lf %lf", &kernel[0][0], &kernel[0][1], &kernel[0][2],
												&kernel[1][0], &kernel[1][1], &kernel[1][2],
												&kernel[2][0], &kernel[2][1], &kernel[2][2]);

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

		char *args[] = {(char *)"imageRectification", NULL};
		execvp("./imageRectification", args);
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

		imageFile.data = (char*) malloc(sizeof(char) * (imageFile.dataSize));

		i = 0;
		while (i < (imageFile.dataSize))
		{	
			read(READ, str, 128);
			sscanf(str,"%c", &imageFile.data[i]);
			i = i + 1;
		}

		imgMatrix = (float**) malloc(sizeof(float*) * imageFile.height);
		for(i = 0; i < imageFile.height; i++)
		{
			imgMatrix[i] = (float*) malloc(sizeof(float) * imageFile.width);
		}
		setImage(imgMatrix, &imageFile);

		float **imgMatrix2 = (float**) malloc(sizeof(float*) * imageFile.height);
		for(i = 0; i < imageFile.height; i++)
		{
			imgMatrix2[i] = (float*) malloc(sizeof(float) * imageFile.width);
		}

		convolution(imgMatrix, imgMatrix2, kernel, &imageFile);

		tempN++;

		// Send image params.
		sprintf(str, "%u %u %u", imageFile.width, imageFile.height, imageFile.dataSize);
		write(piped[WRITE], str, 128);

		for(i = 0; i < imageFile.height; i++)
		{

			for(j = 0; j < imageFile.width; j++)
			{
				sprintf(str,"%f", imgMatrix2[i][j]);
				write(piped[WRITE], str, 128);
			}
			
			free(imgMatrix[i]);
			free(imgMatrix2[i]);
			
		}

		free(imgMatrix);
		free(imgMatrix2);
	}

	close(piped[WRITE]); 

 	pid_t wpid = waitpid(pid, NULL, 0);

	return 0;
}