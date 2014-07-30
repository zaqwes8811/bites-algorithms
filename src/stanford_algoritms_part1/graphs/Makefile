# http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

CC=g++
APP=shortest_path
BOOST=/home/zaqwes/work/third_party/boost_1_55_0
CCFLAGS=-I$(BOOST)

all:
	$(CC) -c $(APP).cc $(CCFLAGS)
	$(CC) -o $(APP).app $(APP).o 
	
run:
	./$(APP).app
	
clean:
	rm *.o *.app