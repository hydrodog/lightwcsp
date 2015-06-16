CPP= g++ -std=c++11
DEBUG = -g -pg
LIB = -lssl -lcrypto -ldl

testEncrypt: Bench.o
	$(CPP) $(DEBUG) Bench.o testEncrypt.cc -o testEncrypt $(LIB)

Bench.o: Bench.cc
	$(CPP) -c Bench.cc

sample: SSLsample.c
	gcc -g -pg SSLsample.c -lssl -lcrypto -ldl

clean:
	rm -rf *o testEncrypt