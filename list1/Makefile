CXX=g++
CXXFLAGS=-std=c++20 -Wall -Wextra -O3
BIN=main

SRC=$(wildcard *.cpp)
OBJ=$(SRC:%.cpp=%.o)

8_TEST = 0 6 1 7 4 2 3 8 5, 5 0 2 6 4 8 1 7 3, 2 4 7 0 3 6 8 1 5

15_TEST = 1 0 2 3 4 5 6 7 8 9 10 11 12 13 14 15

ALGORITHM=astar

all: $(OBJ)
	$(CXX) -o $(BIN) $^

%.o: %.c
	$(CXX) $@ -c $<

clean:
	rm -f *.o $(BIN)

run8:
	./$(BIN) -$(ALGORITHM) $(8_TEST)

run15: all
	./$(BIN) -$(ALGORITHM) $(15_TEST)