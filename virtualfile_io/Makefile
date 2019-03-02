CC=g++

CFLAGS=-g 
LDFLAGS=-g 


.PHONY: all clean run

TARGET=VirtIO
PRIMARY=VirtIO

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



