vpath %.c src
vpath %.h inc
SRCS:=$(wildcard *.c)
OBJS:=$(patsubst %.c,%.o,$(SRCS))
ELF:=client
ELFS:=main
CC:=gcc
CFLAGS:=-g -Wall -I inc
#$(ELF):$(OBJS)
#	$(CC) -o $@ $^ CFLAGS
$(OBJS):=$(SRCS)
	$(CC) -o $@ -c $(SRCS) CFLAGS
.PHONY: clean rebuild
clean:
	$(RM) $(OBJS) $(ELF) $(ELFS)
rebuild: $(ELF) clean
