SRCS:=$(wildcard *.c)
BIN:=$(patsubst %.c, %, $(SRCS))
#OUT:=MAIN
main:$(BIN)
%:%.c
	gcc $^ -o $@
.PHONY: clean rebuild $(OUT)
clean:
	rm -vf $(BIN)
#	echo "hi\n"
rebuld: clean $(OUT)
