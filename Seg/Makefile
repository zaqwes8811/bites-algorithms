# ZZZ: how add other kay for g++
#

SRCS = *.cc
OBJS = *.o
HEAD = 
CC = g++
# obj. files

all: run

main: $(OBJS) 
	$(CC) -o main  $(OBJS)
$(OBJS): $(SRCS)
	$(CC) -lrt -c $(SRCS)

# add
clean:
	rm -f $(OBJS) main.*
run: main
	./main
