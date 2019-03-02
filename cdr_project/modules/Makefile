CC=gcc

INC_DIR=../include
CFLAGS=-g -fPIC -pthread -ansi -Wall -Werror -DDEBUG -I$(INC_DIR) 
LDFLAGS= -g 

.PHONY: all clean run

# target (the only value need to be changed between runs)
TARGET=manager
PRIMARY=manager

SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)


all: $(TARGET) run

$(TARGET) : $(OBJECTS) 
	$(CC) $^ $(LDFLAGS)  -o $(TARGET) -pthread
	
clean: 
	-rm ../object *.o 
	-rm $(TARGET)

run:
	mv *.o ../object

