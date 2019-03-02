CC=g++

CFLAGS=-g -pedantic -Werror -Wall -wconvertion -std=c++03
LDFLAGS=-g 


.PHONY: all clean run

TARGET=TestToyV
PRIMARY=TestToyV

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



