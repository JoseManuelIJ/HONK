main: main.c utils.o imageReading.c imageReading
	gcc -o main main.c utils.o

imageReading: imageReading.c img.o imageConvolution
	gcc -o imageReading imageReading.c img.o

imageConvolution: imageConvolution.c img.o imageRectification
	gcc -o imageConvolution imageConvolution.c img.o

imageRectification: imageRectification.c img.o imagePooling
	gcc -o imageRectification imageRectification.c img.o

imagePooling: imagePooling.c img.o imageClassification
	gcc -o imagePooling imagePooling.c img.o

imageClassification: imageClassification.c img.o
	gcc -o imageClassification imageClassification.c img.o

utils.o: utils.c utils.h
	gcc -c utils.c

img.o: img.c img.h
	gcc -c img.c