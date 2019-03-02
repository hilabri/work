CC=g++ 

CFLAGS=-g -pthread -pedantic -Werror -Wall -Wconversion -std=c++03 
LDFLAGS=-g 


.PHONY: all clean run

TARGET=TestQ
PRIMARY=TestQ

SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)


all:$(TARGET) run

$(TARGET) : $(OBJECTS) 
	$(CC) $^ $(LDFLAGS) $(CFLAGS) -o $(TARGET)



clean: 
	-rm *.o
	-rm $(TARGET)

run:
	./$(TARGET)



