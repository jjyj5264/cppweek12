CC = g++
CFLAGS = -g -Wall -Werror -std=c++11

all: first second

node.o: node.cpp
	$(CC) $(CFLAGS) -c node.cpp

service.o: service.cpp
	$(CC) $(CFLAGS) -c service.cpp

first.o: scenarios/first.cpp
	$(CC) $(CFLAGS) -c scenarios/first.cpp

second.o: scenarios/second.cpp
	$(CC) $(CFLAGS) -c scenarios/second.cpp

first: first.o node.o service.o
	$(CC) $(CFLAGS) -o first first.o node.o service.o

second: second.o node.o service.o
	$(CC) $(CFLAGS) -o second second.o node.o service.o

clean:
	rm -f *.o first second