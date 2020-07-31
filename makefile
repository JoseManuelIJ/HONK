main: main.c img.o
	gcc main.c -o main -lm

img.o: img.c img.h stb_image.h stb_image_resize.h stb_image_write.h
	gcc -c -Wall img.c -lm