//Standar libraries
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include "structs.h"
#include "img.h"

//Function to determine if a string is a number
//Input: the string to analyze
//Output: -1 if it isn't a number, 1 in the contrary case
int isNumeric(char* str)
{
	int len = strlen(str);

	for(int i = 0; i < len; i++)
	{
		if(isdigit(str[i]) == 0)
			return -1;
	}

	return 1;
}

//Function that reads and stores the filter mask for the image
//Input: The name of the file with the mask, the matrix to store the kernel
//Output: -1 if it failed reading the file, 1 if it's successful
int readMaskFile(char *fileName, int **kernel){

    int i;
    FILE *fp;
    char buffer[255];

    fp = fopen(fileName, "r");

    if(!fp){
        printf("File not found! (Actual file name input: %s)", fileName);
        return -1;
    }

    for(i = 0; i < 3; i++){
        if(fgets(buffer, 255, fp) != NULL){
            sscanf(buffer, "%d %d %d", &kernel[i][0], &kernel[i][1], &kernel[i][2]);    
        }
    }

    return 1;
}

int main(int argc, char *argv[]){

    int opt, c, cIsValid = 0, u, uIsValid = 0, n, nIsValid = 0, kernelIsValid = 0, showResults = 0;
    int **kernel = (int**)malloc(sizeof(int*) * 3);
    for(int i = 0; i < 3; i++){
        kernel[i] = (int*)malloc(sizeof(int) * 3);
    }
    char *actualImageNumber = (char*) malloc(sizeof(char) * 4);
    char *imgString = (char*) malloc(sizeof(char) * 8);
    strcpy(imgString, "imagen_");
    char *jpgExtension = (char*) malloc(sizeof(char) * 5);
    strcpy(jpgExtension, ".jpg");

    while((opt = getopt(argc, argv, ":c:u:n:m:b")) != -1){
        switch(opt){            

            //Number of images to read//
            case 'c':
                if(isNumeric(optarg) == -1){
                    printf("The -c flag (number of images) is not numeric!\n");
                    exit(1);
                }

                c = atoi(optarg);

                if(c <= 0){
                    printf("The -c flag (number of images) must be greater than 0 (actual value: %d)\n", c);
                    exit(1);
                }
                cIsValid = 1;
                break;

            //Pixel value tresshold for binarization
            case 'u':
                if(isNumeric(optarg) == -1){
                    printf("The -u flag (pixel value tresshold) is not numeric!\n");
                    exit(1);
                }

                u = atoi(optarg);

                if(u < 0 || u > 255){
                    printf("The -u flag (pixel value tresshold) must be between 0 and 255 (actual value: %d\n)", u);
                    exit(1);
                }
                uIsValid = 1;
                break;
            
            //Tresshold for classification
            case 'n':
                if(isNumeric(optarg) == -1){
                    printf("The -n flag (percentage of black pixels tresshold) is not numeric!\n");
                    exit(1);
                }

                n = atoi(optarg);

                if(u < 0 || u > 100){
                    printf("The -n flag (percentage of black pixels tresshold) must be between 0 and 100 (actual value: %d)\n", n);
                    exit(1);
                }
                nIsValid = 1;
                break;

            //File name where the mask is located at
            case 'm':
                if(readMaskFile(optarg, kernel) == -1){
                    exit(1);
                }
                kernelIsValid = 1;
                break;
            
            //Optional flag to show results
            case 'b':
                showResults = 1;
                break;
            
            //Optarg argument missing
            case ':':
                printf("Please, check if there's a missing argument\n");
                exit(1);
                break;

            //Optarg is an unknown option
            case '?':
                printf("There is an unknown option... -%c\n", optopt);
                exit(1);
                break;
        }
    }

    if(cIsValid == 1 && nIsValid == 1 && uIsValid == 1 && kernelIsValid == 1){
        int i;
        for(i =1 ; i <= c; i++){
            sprintf(actualImageNumber, "%d", i); //Convert image number to string
            strcat(imgString, actualImageNumber); //Append it to the expected filename format "imagen_"
            strcat(imgString, jpgExtension); //"Append the .jpg extension"
            printf("Processing %s\n", imgString);
            //Process the image
            imageProcessingMain(imgString);
            if(showResults == 1){
                printf("Resultado de %s es:  \n", imgString);
            }
            strcpy(imgString, "imagen_");//Set imgString back to the beggining again
        }
    }

    for(int i = 0; i < 3; i++){
        free(kernel[i]);
    }
    free(kernel);
    free(actualImageNumber);
    free(imgString);
    free(jpgExtension);
    return 0;
}