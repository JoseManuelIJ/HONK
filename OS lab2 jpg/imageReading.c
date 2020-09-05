#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h> //  Both of these are potentially useless
#include <sys/stat.h>  //
#include "structs.h"
#include "img.h"

#define READ 0
#define WRITE 1

int main(int argc, char *argv[])
{
	// Transmission statements
	int piped[2];
	pid_t pid;

	// Image reading statements
	int fileDescr, dataStream, i = 0, j = 0, tempN = 1;
	ssize_t pngFile;
	char str[128], fileName[255], data[4097];
	int imgNumber, threshold, skipAnalysis;
	Img imageFile;

	

	// Read args
	read(READ, str, 128);
	sscanf(str, "%d %d %d", &imgNumber, &threshold, &skipAnalysis);

	// Read kernel
	read(READ, str, 128);

	// Transmission start.
	pipe(piped);
	pid = fork();

	// Child section
	if (pid == 0)
	{
		close(piped[WRITE]);
		dup2(piped[READ], STDIN_FILENO);

		char *args[] = {(char *)"imageConvolution", NULL};
		execvp("./imageConvolution", args);
		perror("exec failed");
		return 1;
	}

	else
	{
		close(piped[READ]);

		// Send kernel.
		write(piped[WRITE], str, 128);

		// Send global args.
		sprintf(str, "%d %d %d", imgNumber, threshold, skipAnalysis);
		write(piped[WRITE], str, 128);
		
	}

	while(tempN <= imgNumber)
	{
		sprintf(fileName,"imagen_%d.png", tempN);//Generate the name of the file to be opened
		printf("Searching file '%s'....\n", fileName);
		fileDescr = open(fileName, O_RDONLY);
		if(fileDescr < 0)
		{
			printf("File '%s' not found\nClosing program...\n", fileName);
			exit(0);
		}


		else
		{
			imageFile = startLecture(fileName);
		}

		close(fileDescr);
		//Function number 1 goes here

		// Send image.

		sprintf(str, "%u %u %u %u", imageFile.width, imageFile.height, imageFile.dataSize, imageFile.bitDepth);
		write(piped[WRITE], str, 128);

		
		while (i < imageFile.dataSize)
		{	
			sprintf(str,"%c", imageFile.data[i]);
			write(piped[WRITE], str, 128);
			i = i + 1;
		}
		
		
		tempN++;
	}

	close(piped[WRITE]); 

 	pid_t wpid = waitpid(pid, NULL, 0);

	return 0;
}