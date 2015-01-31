# ZZZ: how add other kay for g++
#
# c cl.exe пока стоп

SRCS = *.cc gtest/*.cc utils/*.cc
#OBJS = objs/*.o
OBJS = *.o
HEAD = 
CC = g++
# obj. files

all: run

main: $(OBJS) 
	$(CC) -Wall -o main  $(OBJS)
$(OBJS): $(SRCS)
	$(CC) -Wall -O0 -I. -I./include -c $(SRCS)
#	$(CC) -Wall -lrt -I. -I./include -c $(SRCS)

# add
clean:
	rm -f $(OBJS) main.*
run: main
	./main
