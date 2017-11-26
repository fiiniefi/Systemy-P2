CC=g++ -std=c++11
CFLAGS=-Wall -Werror
LIBS=-pthread -time

settlers: settlers.o
	$(CC) settlers.o -o settlers $(LIBS)

main.o: settlers.cpp
	$(CC) $(CFLAGS) settlers.cpp -c -o settlers.o

run: settlers
	./settlers 10 11 3 77
