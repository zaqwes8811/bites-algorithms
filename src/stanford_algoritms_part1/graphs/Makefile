# http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

CC=g++
APP=shortest_path
BOOST=/home/zaqwes/work/third_party/boost_1_55_0
CCFLAGS=-I$(BOOST) -O2

all:
	$(CC) -Wall -c -pg $(APP).cc $(CCFLAGS)
	$(CC) -o $(APP).app $(APP).o -pg -ltbb
	
run:
	./$(APP).app
	
clean:
	rm *.o *.app
	
benchmark:
	#valgrind --tool=callgrind ./$(APP).app
	time ./$(APP).app