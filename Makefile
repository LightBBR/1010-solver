CC=g++
CFLAGS=-W -Wall -Werror -std=c++0x -g

all: main.o Board.o Piece.o
	$(CC) $(CFLAGS) main.o Board.o Piece.o -o 1010

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

Board.o: Board.cpp
	$(CC) $(CFLAGS) -c Board.cpp

Piece.o: Piece.cpp
	$(CC) $(CFLAGS) -c Piece.cpp

clean:
	rm *.o


