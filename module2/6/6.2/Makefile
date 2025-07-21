all: main.exe

main.exe: main.o liblist.so
	gcc main.o -L. -llist -o main.exe -Wl,-rpath,.

main.o: main.c
	gcc -c main.c

liblist.so: header.o
	gcc -shared header.o -o liblist.so

header.o: header.c
	gcc -fPIC header.c -c
	
clean:
	rm -rf *.o main.exe *.a *.so