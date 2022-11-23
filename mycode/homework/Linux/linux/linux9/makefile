SRCS:=$(wildcard *.c)
BIN:=$(patsubst %.c, %, $(SRCS))
OUT:=MAIN
$(OUT):$(BIN)
%:%.c
	gcc $^ -o $@
.PHONY: clean rebuild $(OUT)
clean:
	rm -f $(BIN)
rebuld: clean $(OUT)
