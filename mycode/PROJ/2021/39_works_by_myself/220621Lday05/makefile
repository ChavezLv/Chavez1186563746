CC:=gcc
SRCS:=$(wildcard *.c)
EXE:=$(patsubst %.c,%,$(SRCS))
CFLAGS:=-g -Wall
all:$(EXE)
%:%.c
	  $(CC) -o $@ $^ $(CFLAGS)
.PHONY: clean rebuild
clean:
	  $(RM) $(EXE)
rebuild: all clean
	
