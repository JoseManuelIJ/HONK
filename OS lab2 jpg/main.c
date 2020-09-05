#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include "utils.h"

#define READ 0
#define WRITE 1

int main(int argc, char *argv[])
{
	int opt;
	int flags = 0;

	int piped[2];
	pid_t pid;

	int imgNumber; // Number of images received.
	double kernel[3][3];
	int clssThreshold;	// Classification threshold.
	int skipAnalysis = 0; // Boolean. 1 for showing 'nearly black' analysis, 0 for skipping it.

	char str[128];

	while ((opt = getopt(argc, argv, ":c:m:n:b")) != -1)
	{
		switch (opt)
		{
		// Number of images flag.
		case 'c':
			if ((imgNumber = validateCFlag(optarg)) == -1)
			{
				printf("Invalid input in: Number of images (-c)\n");
				exit(1);
			}

			flags++;
			break;

		// Kernel.
		case 'm':

			readKernelFile(optarg, kernel);

			flags++;
			break;

		// Classification threshold flag.
		case 'n':
			if ((clssThreshold = validateNFlag(optarg)) == -1)
			{
				printf("Invalid input in: Classification threshold (-n)\n");
				exit(1);
			}

			flags++;
			break;

		// Skipping analysis flag.
		case 'b':
			skipAnalysis = 1;
			break;

		// Missing argument.
		case ':':
			printf("Option needs an argument\n");
			exit(1);

		// Unknown flag.
		case '?':
			printf("Unknown option: %c\n", optopt);
			exit(1);
		}
	}

	if (flags != 3)
	{
		printf("Incorrect number of arguments. Terminating...\n");
		exit(1);
	}

	pipe(piped);
	pid = fork();

	// Child section
	if (pid == 0)
	{
		close(piped[WRITE]);
		dup2(piped[READ], STDIN_FILENO);

		char *args[] = {(char *)"imageReading", NULL};
		execvp("./imageReading", args);
		perror("exec failed");
		return 1;
	}

	else
	{
		close(piped[READ]);

		sprintf(str, "%d %d %d", imgNumber, clssThreshold, skipAnalysis);
		write(piped[WRITE], str, 128);

		sprintf(str, "%lf %lf %lf %lf %lf %lf %lf %lf %lf", kernel[0][0], kernel[0][1], kernel[0][2],
												   kernel[1][0], kernel[1][1], kernel[1][2],
												   kernel[2][0], kernel[2][1], kernel[2][2]);
		write(piped[WRITE], str, 128);
	}

	close(piped[WRITE]); 

 	pid_t wpid = waitpid(pid, NULL, 0);

	return 0;
}