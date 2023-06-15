all: main
	g++ state.o main.o -o main

main: state main.cpp
	g++ main.cpp -c

state: state.cpp
	g++ state.cpp -c

run8-bfs:
	./main -bfs 0 1 2 3 4 5 6 7 8

run15-bfs:
	./main -bfs 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15

clean:
	rm -rf *.o main