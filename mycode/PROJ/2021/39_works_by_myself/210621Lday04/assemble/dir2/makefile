SRCS:=$(wildcard *.c)
OBJS:=$(patsubst %.c,%.o,$(SRCS))
ELF:=client
CC:=gcc
CFLAGS:=-g -Wall
$(ELF):$(SRCS)#$(OBJS)
	$(CC) -o $@ $^
.PHONY: clean rebuild
clean:
	$(RM) $(OBJS) $(ELF)
rebuild: $(ELF) clean
