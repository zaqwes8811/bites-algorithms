# http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

CC=g++
APP=w6
BOOST=/home/zaqwes/work/third_party/boost_1_55_0
DETAILS=../../

# -pg - for valgrind
CCFLAGS=-I$(BOOST) -I$(DETAILS) -O2  -std=c++11 -Wall -Wno-deprecated -pg

# http://stackoverflow.com/questions/10363646/compiling-c11-with-g
all:
	# 
	$(CC) $(CCFLAGS) -c  $(APP).cc 
	$(CC) $(CCFLAGS) -c  $(DETAILS)details/io_details.cc 
	$(CC) -o $(APP).app *.o -pg -ltbb
	
run:
	./$(APP).app
	
clean:
	rm *.o *.app
	
benchmark:
	#valgrind --tool=callgrind ./$(APP).app
	time ./$(APP).app