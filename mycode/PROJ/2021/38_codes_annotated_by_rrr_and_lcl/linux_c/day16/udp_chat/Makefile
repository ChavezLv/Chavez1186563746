SRCS:=$(wildcard *.c)
ELFS:=$(SRCS:%.c=%)
CC:=gcc
all:$(ELFS)

%:%.c
	$(CC) $< -o $@ -pthread
clean:
	rm -rf $(ELFS)

