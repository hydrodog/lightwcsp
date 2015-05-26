all:
	g++ server.cc -o server
	g++ client.cc -o client

server:
	g++ server.cc -o server

client:
	g++ client.cc -o client

run:
	./server
	./client ${ip}

clean:
	rm server
	rm client