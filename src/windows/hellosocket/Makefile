CPPFLAGS= -Iinclude
CFLAGS= -Wall -std=c++11 -pthread
LDFLAGS=
CC=g++

all:server client

server:./MyTcpServer/server.cpp
	$(CC) $^ $(LDFLAGS) $(CFLAGS) -o $@
client:./MyTcpClient/client.cpp
	$(CC) $^ $(LDFLAGS) $(CFLAGS) -o $@


.PHONY:clean
clean:
	-rm -f ./MyTcpClient/*.o
	-rm -f ./MyTcpServer/*.o
	-rm -f server client
	-rm -f *.txt
