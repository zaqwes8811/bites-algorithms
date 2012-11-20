# ZZZ: how add other kay for g++
#
# c cl.exe пока стоп

SRCS = *.cc gtest/*.cc utils/*.cc
OBJS = *.o
HEAD = 
CC = g++
# obj. files

all: run

main: $(OBJS) 
	$(CC) -o main  $(OBJS)
$(OBJS): $(SRCS)
	$(CC) -lrt -I. -I./include -c $(SRCS)

# add
clean:
	rm -f $(OBJS) main.*
run: main
	./main
