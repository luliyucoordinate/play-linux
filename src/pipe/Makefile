CPPFLAGS= -Iinclude
CFLAGS= -g -Wall
LDFLAGS= -lcurses
CC=gcc

src = pipedemo2.c 
obj = $(patsubst %.c,%.o,$(src))
target = app

$(target):$(obj)
	$(CC) $^ $(LDFLAGS) -o $@

%.o:%.c
	$(CC) -c $< $(CFLAGS) $(CPPFLAGS) -o $@

.PHONY:clean
clean:
	-rm -f *.o
	-rm -f app
