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

	// Image rectification statements
	int i, j;
	char str[128];
	int imgNumber, threshold, skipAnalysis;
	int tempN = 0;
	Img imageFile;

	// Read global args.
	read(READ, str, 128);
	sscanf(str, "%d %d %d", &imgNumber, &threshold, &skipAnalysis);

	imageFile.data = (char*) malloc(sizeof(char) * imageFile.dataSize);

	// Transmission start.
	pipe(piped);
	pid = fork();

	// Child section
	if (pid == 0)
	{
		close(piped[WRITE]);
		dup2(piped[READ], STDIN_FILENO);

		char *args[] = {(char *)"imagePooling", NULL};
		execvp("./imagePooling", args);
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
		//printf("%u %u %u\n", imageFile.width, imageFile.height, imageFile.dataSize);


		imageFile.image2 = (float**)malloc(sizeof(float*) * imageFile.height);
		for(i = 0; i < imageFile.height; i++)
		{
			imageFile.image2[i] = (float*) malloc(sizeof(float) * imageFile.width);
		}


		for(i = 0; i < imageFile.height; i++)
		{
			for(j = 0; j < imageFile.width; j++)
			{
				read(READ, str, 128);
				sscanf(str, "%f", &imageFile.image2[i][j]);
			}
		}

		rectification(&imageFile);

		// Send image data.

		// Send image params.
		sprintf(str, "%u %u %u", imageFile.width, imageFile.height, imageFile.dataSize);
		write(piped[WRITE], str, 128);

		for(i = 0; i < imageFile.height; i++)
		{
			for(j = 0; j < imageFile.width; j++)
			{
				sprintf(str, "%f", imageFile.image2[i][j]);
				write(piped[WRITE], str, 128);
			}

			free(imageFile.image2[i]);
		}
		free(imageFile.image2);
		tempN++;
	}

	close(piped[WRITE]); 

 	pid_t wpid = waitpid(pid, NULL, 0);

	return 0;
}