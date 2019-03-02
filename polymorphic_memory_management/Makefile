CC=g++

#vpath %.h ../include

#INC_DIR=../include
CFLAGS=-g -std=c++11 
#CFLAGS=-g -ggdb-pedantic -ansi -Wall -Werror -DDEBUG -I$(INC_DIR) 
LDFLAGS= -g 

# this command only required if library is being used
#export LD_LIBRARY_PATH := ../Lib:$(LD_LIBRARY_PATH)

.PHONY: all clean run

# target (the only value need to be changed between runs)
TARGET=test
PRIMARY=test

SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)


all:$(TARGET) run

$(TARGET) : $(OBJECTS) 
	$(CC) $^ $(LDFLAGS) -o $(TARGET)



clean: 
	-rm *.o
	-rm $(TARGET)

run:
	./$(TARGET)



