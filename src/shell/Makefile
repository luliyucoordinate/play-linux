CPPFLAGS= -Iinclude
CFLAGS= -g -Wall
LDFLAGS=
CC=gcc

src = smsh1.c splitline.c execute.c
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
