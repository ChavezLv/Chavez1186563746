FROM:=$(wildcard *.c)
TO:=$(FROM:%.c=%.o)
CC:=gcc
server:$(FROM)
	$(CC) $^ -o server -pthread -lmysqlclient -lcrypt

