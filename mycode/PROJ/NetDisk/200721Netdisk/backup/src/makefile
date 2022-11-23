FROM:=$(wildcard *.c)
TO:=$(FROM:%.c=%.o)
CC:=gcc
main:$(FROM)
	$(CC) $^ -o main -pthread 
	
