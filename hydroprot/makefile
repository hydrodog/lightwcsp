CPP = g++ -std=c++11
DEBUG = -g
OPT = #-O2
all:	server client

server: server.cc
	$(CPP) $(DEBUG) $(OPT) server.cc -o server

client: client.cc
	$(CPP) $(DEBUG) $(OPT) client.cc -o client

run:
	./server
	./client ${ip}

clean:
	rm server
	rm client
