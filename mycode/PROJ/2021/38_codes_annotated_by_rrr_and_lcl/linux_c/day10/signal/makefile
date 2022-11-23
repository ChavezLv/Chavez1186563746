CC:=gcc
srcs:=$(wildcard *.c)#1.c 2.c 3.c
exe:=$(patsubst %.c,%,$(srcs))#1 2 3
#gcc 1.c -o 1
#gcc 2.c -o 2
#gcc 3.c -o 3
#all:$(exe)
#	for i in $(exe);do $(CC) $$i.c -o $$i;done
all:$(exe)
%:%.c
	$(CC) $^ -o $@
.PHONY:clean rebuild
clean:
	$(RM) $(exe)
rebuild:clean all
