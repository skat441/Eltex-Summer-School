all: client server

client: client.c
	gcc client.c -o client.exe

server: server.c
	gcc server.c -o server.exe

clean:
	rm -rf *.o server.exe client.exe