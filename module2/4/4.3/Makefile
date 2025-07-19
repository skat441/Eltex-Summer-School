all: main.exe

main.exe: main.o
	gcc main.o -o main.exe

main.o: main.c
	gcc -c main.c -Wall

clean:
	rm -rf *.o main.exe