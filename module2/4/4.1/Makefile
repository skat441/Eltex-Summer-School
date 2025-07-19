all: main.exe

main.exe: main.o
	gcc main.o -o main.exe -lcunit

main.o: main.c
	gcc -c main.c -lcunit

clean:
	rm -rf *.o main.exe