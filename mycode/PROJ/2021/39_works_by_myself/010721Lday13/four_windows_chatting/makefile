CC:=gcc
SRCS:=$(wildcard *.c)
EXE:=$(patsubst %.c,%,$(SRCS))
CFLAGS:=-g -Wall -lpthread
all:$(EXE)
%:%.c
	$(CC) -o $@ $^ $(CFLAGS)
.PHONY: clean rebuild
clean:
	$(RM) $(EXE)
rebuild: all clean
