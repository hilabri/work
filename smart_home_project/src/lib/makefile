CC=g++ 

FLAGS=-g -fPIC -pedantic 

.PHONY: all

OBJECTS=sensor.o fire_sensor.o event.o semaphore.o mutex.o id.o receiver.o 

TARGET=dlFireSensor.so 

all: $(TARGET) 

dlFireSensor.so: $(OBJECTS)
	$(CC) -fpic -shared $^ -o $@

clean: 
	-rm $(TARGET)
