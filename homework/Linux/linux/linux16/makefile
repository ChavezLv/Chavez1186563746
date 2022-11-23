SRCS:=$(wildcard *.c)
BIN:=$(patsubst %.c, %, $(SRCS))
OUT:=MAIN
$(OUT):$(BIN)
%:%.c
	gcc $^ -o $@ -pthread
.PHONY: clean rebuild $(OUT)
clean:
	rm -f $(BIN)
rebuld: clean $(OUT)
