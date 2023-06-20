#include <string>
#include <sstream>
#include <iostream>
#include <list>
#include <chrono>

#include "state.h"
#include "search.h"

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

  for (auto &initialState : initialStates)
  {
    auto start = std::chrono::high_resolution_clock::now();

    SearchResult result = search<BFSOpenList>(initialState);

    auto end = std::chrono::high_resolution_clock::now();

    if (result.state != nullptr)
    {
      std::chrono::duration<double, std::milli> fp_ms = end - start;
      auto timeInSeconds = fp_ms.count() / 1000;

      std::cout << result.expandedNodes << ",";
      std::cout << result.state->getDepth() << ",";
      std::cout << timeInSeconds << ",";
      // TODO: mean heuristic value for all states
      std::cout << initialState->manhattanDistance() << std::endl;
    }
  }

  return 0;
}
