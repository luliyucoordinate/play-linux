CPPFLAGS= -Iinclude
CFLAGS= -Wall
LDFLAGS=
CC=gcc

src = tinybc.c 
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
	-rm -f *.txt
