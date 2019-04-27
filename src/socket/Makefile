CPPFLAGS= -Iinclude
CFLAGS= -g -Wall
LDFLAGS=
CC=gcc

src = rls.c
obj = $(patsubst %.c,%.o,$(src))
target = client

$(target):$(obj)
	$(CC) $^ $(LDFLAGS) -o $@

%.o:%.c
	$(CC) -c $< $(CFLAGS) $(CPPFLAGS) -o $@

.PHONY:clean
clean:
	-rm -f *.o
	-rm -f app
