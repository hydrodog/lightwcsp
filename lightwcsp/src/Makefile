CPP = g++ -std=c++11
OBJ = Config.o Logger.o MyStringBuffer.o RequestHandler.o FileSys.o server.o
OPT = #-O3
DEBUG = -g
INCLUDE = -I include/

server: Config.o Logger.o MystringBuffer.o RequestHandler.o FileSys.o server.o
	$(CPP) $(OPT) $(DEBUG) $(OBJ) -o server

Config.o: Config.cc
	$(CPP) $(INCLUDE) $(DEBUG) $(OPT) -c Config.cc
	
FileSys.o: FileSys.cc
	$(CPP) $(INCLUDE) $(DEBUG) $(OPT) -c FileSys.cc

Logger.o: Logger.cc
	$(CPP) $(INCLUDE) $(DEBUG) $(OPT) -c Logger.cc

MystringBuffer.o: MystringBuffer.cc
	$(CPP) $(INCLUDE) $(DEBUG) $(OPT) -c MystringBuffer.cc

RequestHandler.o: RequestHandler.cc
	$(CPP) $(INCLUDE) $(DEBUG) $(OPT) -c RequestHandler.cc

server.o: server.cc
	$(CPP) $(INCLUDE) $(DEBUG) $(OPT) -c server.cc

clean:
	rm -rf *o server
