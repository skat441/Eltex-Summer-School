all: main.exe

main.exe: main.o
	gcc main.o -o main.exe -lcunit

main.o: main.c header.h
	gcc -c main.c -Wall

clean:
	rm -rf *.o main.exe