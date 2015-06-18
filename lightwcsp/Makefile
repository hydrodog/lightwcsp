CPP = g++ -std=c++11
OBJ = Config.o Logger.o HttpRequest.o FileSys.o Buffer.o server.o CspServlet.o InMemoryFileServlet.o FileServlet.o Bench.o Signal.o
OPT = #-O3
DEBUG = -g
INCLUDE = -I include/
#COMP=$(CPP) $(OPT) $(DEBUG) $(INCLUDE) -Wl,--no-as-needed
COMP=$(CPP) $(OPT) $(DEBUG) $(INCLUDE) -pthread

server: $(OBJ)
	$(CPP) $(OPT) $(DEBUG) $(OBJ) -pthread -o server -lssl -lcrypto -ldl

Config.o: Config.cc
	$(COMP) -c Config.cc

FileSys.o: FileSys.cc include/FileSys.hh
	$(COMP) -c FileSys.cc

Logger.o: Logger.cc
	$(COMP) -c Logger.cc

HttpRequest.o: HttpRequest.cc include/HttpRequest.hh
	$(COMP) -c HttpRequest.cc

CspServlet.o: CspServlet.cc include/HttpRequest.hh
	$(COMP) -c CspServlet.cc

InMemoryFileServlet.o: InMemoryFileServlet.cc include/HttpRequest.hh
	$(COMP) -c InMemoryFileServlet.cc

FileServlet.o: FileServlet.cc include/HttpRequest.hh
	$(COMP) -c FileServlet.cc

server.o: server.cc include/HttpRequest.hh
	$(COMP) -c server.cc

Buffer.o: Buffer.cc include/Buffer.hh
	$(COMP) -c Buffer.cc

Bench.o: Bench.cc
	$(COMP) -c Bench.cc

Signal.o:Signal.cc
	$(COMP) -c Signal.cc

clean:
	rm -rf *o server
