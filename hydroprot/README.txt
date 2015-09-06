Compiling
	clang++ / g++ server.cc -o server
	clang++ / g++ convert.cc client.cc -o client

Using
	./server fixed.dat
	./client 192.168.1.129 1 fixed.dat
	Server has embedded example code, which means that it'll have to be changed in some moment
	Server needs a file that will be streamed to Client in case Client's option is 0 (first request)
	Client needs Server's IP address, option (0 for first request) and a data file (in case option is not 0)