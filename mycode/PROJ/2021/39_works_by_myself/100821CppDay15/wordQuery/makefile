SRCS:=$(wildcard src/*.cc)
OBJS:=$(patsubst %.cc,%.o,$(SRCS))
ELF:=client
CC:=g++
CFLAGS:= -Wall -lpthread -llog4cpp
$(ELF):$(OBJS)
	  $(CC) -o $@ $^ $(CFLAGS)
.PHONY: clean rebuild
clean:
	  $(RM) $(OBJS) $(ELF)
rebuild: $(ELF) clean

