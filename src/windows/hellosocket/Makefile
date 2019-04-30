CPPFLAGS= -Iinclude
CFLAGS= -Wall -std=c++11 -pthread
LDFLAGS=
CC=g++

all:server client

server:./EasyTcpServer/server.cpp
	$(CC) $^ $(LDFLAGS) $(CFLAGS) -o $@
client:./EasyTcpClient/client.cpp
	$(CC) $^ $(LDFLAGS) $(CFLAGS) -o $@


.PHONY:clean
clean:
	-rm -f ./EasyTcpClient/*.o
	-rm -f ./EasyTcpServer/*.o
	-rm -f server client
	-rm -f *.txt
