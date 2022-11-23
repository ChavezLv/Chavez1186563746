SRCS:=$(wildcard *.c)
OBJS:=$(patsubst %.c,%.o,$(SRCS))
ELF:=main
CC:=gcc
CFLAGS:=-g -Wall -lpthread -lmysqlclient
$(ELF):$(OBJS)
	  $(CC) -o $@ $^ $(CFLAGS)
.PHONY: clean rebuild
clean:
	  $(RM) $(OBJS) $(ELF)
rebuild: $(ELF) clean

