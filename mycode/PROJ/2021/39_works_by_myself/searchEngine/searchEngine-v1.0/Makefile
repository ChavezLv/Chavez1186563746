INC_DIR := ./include/ 
CC := g++
CPPFLAGS := -g
LIBS := -lpthread -ljson
INC_FILE := $(addprefix -I, $(INC_DIR))
CPP_FILE := $(wildcard ./src/*.cpp) $(wildcard ./test/*.cpp)
OBJS := $(patsubst %.cpp, %.o, $(CPP_FILE))
TARGET := server.exe

$(TARGET):$(OBJS)
	$(CC) $(CPPFLAGS) -o $@ $^ $(INC_FILE) $(LIBS)
%.o:%.cpp
	$(CC) $(CPPFLAGS) -o $@ -c $< $(INC_FILE) $(LIBS)
object:
	@echo $(INC_FILE)
	@echo $(CPP_FILE)
	@echo $(OBJS)
clean:
	rm -rf $(TARGET) $(OBJS)
rebuild: clean $(TARGET)

