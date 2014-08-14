# http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

CC=g++
APP=shortest_path
BOOST=/home/zaqwes/work/third_party/boost_1_55_0
DETAILS=../../
CCFLAGS=-I$(BOOST) -I$(DETAILS) -O2

# http://stackoverflow.com/questions/10363646/compiling-c11-with-g
all:
	$(CC) $(CCFLAGS) -std=c++11 -Wall -c -pg $(APP).cc
	$(CC) $(CCFLAGS) -std=c++11 -Wall -c -pg $(DETAILS)details/graph_details.cc 
	$(CC) $(CCFLAGS) -std=c++11 -Wall -c -pg $(DETAILS)details/io_details.cc 
	$(CC) -o $(APP).app *.o -pg -ltbb
	
run:
	./$(APP).app
	
clean:
	rm *.o *.app
	
benchmark:
	#valgrind --tool=callgrind ./$(APP).app
	time ./$(APP).app