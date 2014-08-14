# http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

CC=g++
APP=shortest_path
BOOST=/home/zaqwes/work/third_party/boost_1_55_0
DETAILS=../../

# -pg - for valgrind
CCFLAGS=-I$(BOOST) -I$(DETAILS) -O2  -std=c++11 

# http://stackoverflow.com/questions/10363646/compiling-c11-with-g
all:
	$(CC) $(CCFLAGS) -Wall -c  $(APP).cc
	$(CC) $(CCFLAGS) -Wall -c  $(DETAILS)details/graph_details.cc 
	$(CC) $(CCFLAGS) -Wall -c  $(DETAILS)details/io_details.cc 
	$(CC) -o $(APP).app *.o -pg -ltbb
	
run:
	./$(APP).app
	
clean:
	rm *.o *.app *.pyc
	
benchmark:
	#valgrind --tool=callgrind ./$(APP).app
	time ./$(APP).app