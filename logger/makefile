CC=gcc

# C compiler flags
CFLAGS= -g -ansi -Wall -Werror -DDEBUG

# Linker (ld) flags
LDFLAGS= -g -L . -l liblog.a

.PHONY: all clean run

TARGET=TestZLog.out

LIB_SOURCES=zlog4.c
TEST_SOURCES=main.c

LIB_OBJECTS=$(LIB_SOURCES:.c=.o)
TEST_OBJECTS=$(TEST_SOURCES:.c=.o)

all: $(TARGET) run

$(TARGET) : $(TEST_OBJECTS) 
	$(CC) $(LDFLAGS) $^ -o $(TARGET)

commit:
	ar cvr liblog.a $(LIB_OBJECTS)
	
clean: 
	-rm *.o
	-rm $(TARGET)

run:
	./$(TARGET) log_config

