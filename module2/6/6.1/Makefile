all: main.exe

main.exe: main.o liblist.a
	gcc main.o -L. -llist -o main.exe

main.o: main.c
	gcc -c main.c

liblist.a: header.o
	ar rc liblist.a header.o

header.o: header.c
	gcc -c header.c

clean:
	rm -rf *.o main.exe