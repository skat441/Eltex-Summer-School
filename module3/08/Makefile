all: producer.exe consumer.exe

producer.exe: producer.c
	gcc producer.c -o producer.exe

consumer.exe: consumer.c
	gcc consumer.c -o consumer.exe

clean:
	rm -rf *.o producer.exe consumer.exe *.txt