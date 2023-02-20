all: dsh.o 
	gcc -Wall -g -o dsh main.c dsh.o draw.o

dsh.o: dsh.h dsh.c
	gcc -Wall -g -c dsh.c

draw.o: draw.h draw.c
	gcc -Wall -g -c draw.c

clean:
	rm -f dsh *.o
