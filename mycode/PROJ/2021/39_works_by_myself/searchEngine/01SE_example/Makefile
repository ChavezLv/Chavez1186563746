SUFFIX:=.cc#文件后缀

#离线部分的源文件和编译文件
local_lib_src:=$(wildcard src/locallib/*$(SUFFIX)) libs/Mylog.cc
local_lib_obj:=$(local_lib_src:$(SUFFIX)=.o)

#服务器部分的源文件和编译文件
server_src:=$(wildcard src/server/*$(SUFFIX)) $(wildcard src/tasklib/*$(SUFFIX)) $(wildcard src/networklib/*$(SUFFIX)) $(wildcard src/cache/*$(SUFFIX)) src/locallib/Configuration.cc libs/Mylog.cc
server_obj:=$(server_src:$(SUFFIX)=.o)

#客户端部分的源文件和编译文件
client_src:=$(wildcard src/client/*$(SUFFIX))
client_obj:=$(client_src:$(SUFFIX)=.o)

ELF:=bin/server bin/client bin/Exe_DictProducer
CFLAGS:=-g -Wall
CC:=g++# 编译器 
all:$(local_lib_obj) $(server_obj) $(client_obj) $(ELF)

bin/Exe_DictProducer:$(local_lib_obj)
	$(CC) $(CFLAGS) -o $@ $^ -llog4cpp -pthread

bin/server:$(server_obj)
	$(CC) $(CFLAGS) -o $@ $^ -llog4cpp -ljsoncpp -pthread

bin/client:$(client_obj)
	$(CC) $(CFLAGS) -o $@ $^ -ljsoncpp -pthread

%.o:%$(SUFFIX)
	$(CC) -c $(CFLAGS) $^ -o $@

.PHONY:clean rebuild

clean:
	@rm -rf $(ELF) $(local_lib_obj) $(server_obj) $(client_obj) || [ 1 ]

rebuild:clean all

