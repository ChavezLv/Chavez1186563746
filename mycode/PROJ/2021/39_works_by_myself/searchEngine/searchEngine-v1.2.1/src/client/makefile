SRCS:=$(wildcard *.cpp)
OBJS:=$(patsubst %.cpp,%.o,$(SRCS))
ELF:=client
CC:=g++ 
CFLAGS:=-g -Wall -lpthread -llog4cpp -ljson
$(ELF):$(OBJS)
	  $(CC) -o $@ $^ $(CFLAGS)
.PHONY: clean rebuild
clean:
	  $(RM) $(OBJS) $(ELF)
rebuild: $(ELF) clean

