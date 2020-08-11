main: main.c img.o functions.o
	gcc main.c functions.o -o main -lm

img.o: img.c img.h stb_image.h stb_image_resize.h stb_image_write.h
	gcc -c -Wall img.c -lm
functions.o: functions.c structs.h img.h functions.h
	gcc -c -Wall functions.c