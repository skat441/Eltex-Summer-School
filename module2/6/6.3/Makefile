all: main.exe

main.exe: main.c libsum.so libneg.so libmult.so libdiv.so libmod.so libselect.so
	gcc -rdynamic -o main.exe main.c -ldl

libsum.so: sum.o
	gcc -shared sum.o -o libsum.so

sum.o: sum.c
	gcc -fPIC sum.c -c

libneg.so: neg.o
	gcc -shared neg.o -o libneg.so

neg.o: neg.c
	gcc -fPIC neg.c -c

libmult.so: mult.o
	gcc -shared mult.o -o libmult.so

mult.o: mult.c
	gcc -fPIC mult.c -c

libdiv.so: div.o
	gcc -shared div.o -o libdiv.so

div.o: div.c
	gcc -fPIC div.c -c

libmod.so: mod.o
	gcc -shared mod.o -o libmod.so

mod.o: mod.c
	gcc -fPIC mod.c -c

libselect.so: select.o
	gcc -shared select.o -o libselect.so

select.o: select.c
	gcc -fPIC select.c -c
	
clean:
	rm -rf *.o main.exe *.a *.so