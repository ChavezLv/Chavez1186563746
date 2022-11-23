SRCS:=$(wildcard *.cpp)
OBJS:=$(patsubst %.cpp,%.o,$(SRCS))
ELF:=a.out
CC:=g++
CFLAGS:=-Wall -lpthread -llog4cpp
$(ELF):$(OBJS)
	  $(CC) -o $@ $^ $(CFLAGS)
.PHONY: clean rebuild
clean:
	  $(RM) $(OBJS) $(ELF)
rebuild: $(ELF) clean

