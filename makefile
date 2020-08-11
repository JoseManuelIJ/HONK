main: main.c img.o functions img
	gcc -o main main.c img.o functions.o -Wall -lm

img: img.c img.h functions stb_image.h stb_image_resize.h stb_image_write.h
	gcc -c img.c -Wall -lm

functions: functions.c functions.h
	gcc -c functions.c -Wall