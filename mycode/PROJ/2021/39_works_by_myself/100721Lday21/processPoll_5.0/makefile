SRCS:=$(wildcard ./src/*.c)
OBJS:=$(patsubst %.c,%.o,$(SRCS))
ELF:=server
CC:=gcc
CFLAGS:=-g -Wall
$(ELF):$(OBJS)
	  $(CC) -o $@ $^
.PHONY: clean rebuild
clean:
	  $(RM) $(OBJS) $(ELF)
rebuild: $(ELF) clean

