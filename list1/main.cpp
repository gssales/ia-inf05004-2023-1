#include <string>
#include <sstream>
#include <iostream>

#include "state.h"

// ./main -bfs 0 1 2 3 4 5 6 7 8

int main(int argc, char *argv[])
{
  std::string algorithm = argv[1];

  std::stringstream ss;

  for (int i = 2; i < argc; i++)
  {
    ss << argv[i] << " ";
  }

  std::string description = ss.str();

  std::cout << "Algorithm: " << algorithm << std::endl;

  State *initialState8 = State::createInitialState(description);
  initialState8->printState();

  return 0;
}
