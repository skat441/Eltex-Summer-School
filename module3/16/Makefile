all: echo_client echo_server

echo_client: echo_client.c
	gcc echo_client.c -o echo_client.exe

echo_server: echo_server.c
	gcc echo_server.c -o echo_server.exe

clean:
	rm -rf *.o echo_server.exe echo_client.exe