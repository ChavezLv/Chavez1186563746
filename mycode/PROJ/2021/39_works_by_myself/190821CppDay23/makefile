CC:=g++
SRCS:=$(wildcard *.cc)
EXE:=$(patsubst %.cc, %, $(SRCS))
CFLAGS:=-Wall  -llog4cpp -lpthread -ljsoncpp
all:$(EXE)
%:%.cc
	$(CC) -o $@ $^ $(CFLAGS)
.PHONY: clean rebuild
clean:
	$(RM) $(EXE)
rebuild:
	all clean
