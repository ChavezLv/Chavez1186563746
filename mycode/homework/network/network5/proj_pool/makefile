#编译.c文件
#SRCS:=$(wildcard *.c)
#BIN:=$(patsubst %.c, %, $(SRCS))
#OUT:=MAIN
#$(OUT):$(BIN)
#%:%.c
#	gcc $^ -o $@
#	gcc $^ -o $@ -pthread
MAIN:*.o
	gcc *.o -o pool_outs
tcp.o:tcp.c
	gcc -c tcp.c
transerFd.o:transferFd.c
	gcc -c transerFd.c
proj_pool.o:proj_pool.c
	gcc -c proj_pool.c
makechild.o:makechild.c
	gcc -c makechild.c
transferFile.o:transferFile.c
	gcc -c transferFile.c
	
	
#伪目标
.PHONY: clean
clean:
	rm -f *.o
rebuld: clean $(OUT)
