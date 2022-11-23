#vpath %.c src
#vpath %.h header
SRCS:=$(wildcard *.c)
#OBJS:=$(patsubst %.c,%.o,$(SRCS))
ELF:=main
CC:=gcc
CFLAGS:=-g -Wall
$(ELF):$(SRCS)
	$(CC) -o $@ $^ -I ../header/ 
.PHONY: clean rebuild
clean:
	$(RM) $(OBJS) $(ELF)
rebuild: $(ELF) clean
