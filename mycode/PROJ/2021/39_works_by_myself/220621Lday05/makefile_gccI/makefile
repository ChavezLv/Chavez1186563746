#vpath *.c src
#vpath *.h header#vpath 只是负责找到路径
SRCS:=$(wildcard src/*.c)
#OBJS:=$(patsubst %.c,%.o,$(SRCS))
ELF:=main
CC:=gcc
CFLAGS:=-g -Wall -I inc
$(ELF):$(SRCS)
	$(CC) -o $@ $^ $(CFLAGS)
.PHONY: clean rebuild
clean:
	$(RM) $(ELF)
rebuild: $(ELF) clean
