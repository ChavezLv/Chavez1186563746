INC_DIR :=  ./include/
SRC_DIR = ./src/online/
EXE_DIR := ./bin
CC := g++
CPPFLAGS :=  -g 
LIBS := -lpthread -ljson
INC_FILE := $(addprefix -I, $(INC_DIR))	#addprefix加前缀，代码中不写头文件位置，可以用g++ -I+头文件路径
CPP_FILE :=  $(wildcard ./src/online/*.cc)
OBJS := $(patsubst %.cc, %.o, $(CPP_FILE))	#patsubst替换，把.cc替换为.o
objs:=$(addsuffix /*.o, $(SRC_DIR))	#addsuffix加后缀
TARGET := ./bin/online.exe

index:$(TARGET)
$(TARGET):$(OBJS)
	$(CC) $(CPPFLAGS) -o $@ $^ $(INC_FILE) $(LIBS)
%.o:%.cc
	$(CC) $(CPPFLAGS) -o $@ -c $< $(INC_FILE) $(LIBS)
object:
	@echo $(OBJS)
clean:
	rm -rf $(TARGET) $(OBJS)
