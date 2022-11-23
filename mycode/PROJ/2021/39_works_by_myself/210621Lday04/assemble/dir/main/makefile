vpath %.h 

SRCS:=$(wildcard *.c)
OBJS:=$(patsubst %.c,%.o,$(SRCS))
ELF:=client
ELFS:=main
CC:=gcc
CFLAGS:=-g -Wall
$(ELF):$(OBJS)
	$(CC) -o $@ $^ -I ../head/main.h ../fun/fun.h
.PHONY: clean rebuild
clean:
	$(RM) $(OBJS) $(ELF) $(ELFS)
rebuild: $(ELF) clean
