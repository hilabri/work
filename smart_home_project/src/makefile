CC=g++ 


INC_DIR=../include

CPPFLAGS=-g -pedantic -Wall -Wconversion -std=c++03 -I$(INC_DIR) 
LDFLAGS=-g -pthread 


.PHONY: all clean run

TARGET=TestSH
PRIMARY=TestSH

SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)


all:$(TARGET) run

$(TARGET) : $(OBJECTS) 
	$(CC) $^ $(LDFLAGS) $(CPPFLAGS) -o $(TARGET)



clean: 
	-rm lib/*.o lib/*.so
	-rm $(TARGET)

run:
	mv $(OBJECTS) lib/
	cd lib && $(MAKE)
	./$(TARGET)



