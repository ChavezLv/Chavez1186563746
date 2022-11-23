SRCS:=$(wildcard *.c)
ELFS:=$(SRCS:%.c=%)
CC:=gcc
all:$(ELFS)

%:%.c
	$(CC) $< -o $@ -lpthread -lcrypt
clean:
	rm -rf $(ELFS)

