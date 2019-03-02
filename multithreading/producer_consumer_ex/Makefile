CC=gcc

#vpath %.h ../include

INC_DIR=../AdvDS/include
CFLAGS=-g -fPIC -pedantic -ansi -Wall -Werror -DDEBUG -I$(INC_DIR) 
LDFLAGS= -g -L../AdvDS/Lib -lShared

# this command only required if library is being used
export LD_LIBRARY_PATH := ./Lib:$(LD_LIBRARY_PATH)

.PHONY: all clean run

# target (the only value need to be changed between runs)
TARGET=main
PRIMARY=main

SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)


all: $(TARGET) run

$(TARGET) : $(OBJECTS) 
	$(CC) $^ $(LDFLAGS)  -o $(TARGET) -pthread

commit:
	cp $(PRIMARY).o ./Lib
	$(MAKE) -C ./Lib 
	
clean: 
	-rm *.o
	-rm $(TARGET)

run:
	./$(TARGET)



