CC=gcc

#vpath %.h ../include

LDFLAGS=-g -shared 

#ARFLAGS=

LD_LIBRARY_PATH := .:$(LD_LIBRARY_PATH)

OBJECTS=$(wildcard *.o)

all: libShared.so libShared.a

libShared.so: $(OBJECTS)	;	@echo "$(OBJECTS)"
	$(CC) $(LDFLAGS) -o $@ $^
	
libShared.a: $(OBJECTS)
	$(AR) cvr $@ $?

clean: 
	-rm libShared.a
	-rm libShared.so
	-rm *.o


#this is the commands to run a executable file with library:
#gcc StackTestMU.o -L -lshared ../Lib/libShared.so
#gcc QueueTestMU.o -L -lstatic ../Lib/libShared.so
