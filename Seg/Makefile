# ZZZ: how add other kay for g++
#

SRCS = *.cc
OBJS = *.o
HEAD = 
CC = g++
# obj. files

all: main

main: $(OBJS) 
	$(CC) -o main  $(OBJS)
$(OBJS): $(SRCS)
	$(CC) -lrt -c $(SRCS)

# add
clean:
	rm -f $(OBJS) main.*
run: 
	./main
