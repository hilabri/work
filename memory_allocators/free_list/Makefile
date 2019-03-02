CC=gcc

#vpath %.h ../include

INC_DIR=../include
CFLAGS=-g -fPIC -pedantic -ansi -Wall -Werror -DDEBUG -I$(INC_DIR) 
LDFLAGS= -g -L../lib

# this command only required if library is being used
#export LD_LIBRARY_PATH := ../lib:$(LD_LIBRARY_PATH)

.PHONY: all clean run

# target (the only value need to be changed between runs)
TARGET=allocator_test
PRIMARY=allocator

SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)


all: $(TARGET) run

$(TARGET) : $(OBJECTS) 
	$(CC) $^ $(LDFLAGS)  -o $(TARGET)

commit:
	cp $(PRIMARY).o ../lib
	$(MAKE) -C ../lib 
	
clean: 
	-rm *.o
	-rm $(TARGET)

run:
	./$(TARGET)



