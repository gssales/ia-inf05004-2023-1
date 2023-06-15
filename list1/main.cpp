#include <string>
#include <sstream>
#include <iostream>
#include <list>

#include "state.h"

std::list<State *> extractInitialStates(int argc, char *argv[])
{
  std::list<State *> initialStates;
  std::stringstream ss;

  for (int i = 2; i < argc; i++)
  {
    std::string arg = argv[i];

    if (arg.find(',') != std::string::npos)
    {
      arg.erase(std::remove(arg.begin(), arg.end(), ','), arg.end());
      ss << arg;

      State *initialState = State::createInitialState(ss.str());
      initialStates.push_back(initialState);
      ss = std::stringstream();
    }
    else
    {
      ss << argv[i] << " ";
    }
  }

  State *initialState = State::createInitialState(ss.str());
  initialStates.push_back(initialState);

  return initialStates;
}

int main(int argc, char *argv[])
{
  std::string algorithm = argv[1];

  std::list<State *> initialStates = extractInitialStates(argc, argv);

  std::cout << "Algorithm: " << algorithm << std::endl
            << std::endl;

  for (std::list<State *>::iterator it = initialStates.begin(); it != initialStates.end(); ++it)
  {
    State *initialState = *it;

    initialState->printState();

    std::cout << std::endl;
  }

  return 0;
}
