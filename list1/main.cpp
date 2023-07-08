#include <string>
#include <sstream>
#include <iostream>
#include <list>
#include <chrono>
#include <numeric>

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
  algorithm.erase(std::remove(algorithm.begin(), algorithm.end(), '-'), algorithm.end());

  std::list<State *> initialStates = extractInitialStates(argc, argv);

  for (auto &initialState : initialStates)
  {
    SearchResult result;
    auto start = std::chrono::high_resolution_clock::now();

    if (algorithm == "bfs")
    {
      result = search<BFSOpenList>(initialState);
      result.heuristicValues = std::vector<unsigned char>(1, 0);
    }
    else if (algorithm == "astar")
    {
      result = search<AStarOpenList>(initialState);
    }
    else if (algorithm == "gbfs")
    {
      result = search<GBFSOpenList>(initialState);
    }
    else if (algorithm == "idfs")
    {
      result = iterativeDeepeningSearch(initialState);
    }
    else if (algorithm == "idastar")
    {
      result = idaStarSearch(initialState);
    }

    auto end = std::chrono::high_resolution_clock::now();

    if (result.state != nullptr)
    {
      std::chrono::duration<double, std::milli> fp_ms = end - start;
      auto timeInSeconds = fp_ms.count() / 1000;

      std::cout << result.expandedNodes << ",";
      std::cout << result.state->getDepth() << ",";
      std::cout << timeInSeconds << ",";
      std::cout << std::accumulate(result.heuristicValues.begin(), result.heuristicValues.end(), 0.0) / result.heuristicValues.size() << ",";
      std::cout << (int)initialState->getHeuristicValue() << std::endl;
    }
    else {
      std::cout << "-,-,-,-,-" << std::endl;
    }
  }

  return 0;
}
