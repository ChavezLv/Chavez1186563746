#编译.c文件
SRCS:=$(wildcard *.c)
BIN:=$(patsubst %.c, %, $(SRCS))
OUT:=MAIN
$(OUT):$(BIN)
%:%.c
	#gcc $^ -o $@
	#gcc $^ -o $@ -pthread
	gcc $^ -o $@ -lmysqlclient

#伪目标
.PHONY: clean rebuild $(OUT)
clean:
	rm -f $(BIN)
rebuld: clean $(OUT)
