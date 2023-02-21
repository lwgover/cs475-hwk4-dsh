all: dsh.o 
	gcc -Wall -g -o dsh main.c draw.o dsh.o

draw.o: draw.h draw.c
	gcc -Wall -g -c draw.c

dsh.o: dsh.h dsh.c
	gcc -Wall -g -c dsh.c

clean:
	rm -f dsh *.o
