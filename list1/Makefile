CXX=g++
CXXFLAGS=-std=c++20 -Wall -Wextra -Werror
BIN=main

SRC=$(wildcard *.cpp)
OBJ=$(SRC:%.cpp=%.o)

8_TEST = 0 6 1 7 4 2 3 8 5, 5 0 2 6 4 8 1 7 3, 2 4 7 0 3 6 8 1 5

15_TEST = 7 11 8 3 14 0 6 15 1 4 13 9 5 12 2 10, 12 9 0 6 8 3 5 14 2 4 11 7 10 1 15 13, 13 0 9 12 11 6 3 5 15 8 1 10 4 14 2 7

ALGORITHM=idfs

all: $(OBJ)
	$(CXX) -o $(BIN) $^

%.o: %.c
	$(CXX) $@ -c $<

clean:
	rm -f *.o $(BIN)

run8:
	./$(BIN) -$(ALGORITHM) $(8_TEST)

run15:
	./$(BIN) -$(ALGORITHM) $(15_TEST)