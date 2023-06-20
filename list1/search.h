#ifndef SEARCH_H
#define SEARCH_H

#include <set>
#include <queue>
#include <iostream>
#include <vector>

#include "state.h"

struct SearchResult
{
  State *state;
  int expandedNodes;
  std::vector<int> heuristicValues;
};

struct PopResult
{
  State *state;
  int cost;
};

class OpenList
{
  virtual void push(State *state) = 0;
  virtual PopResult pop() = 0;
  virtual bool isEmpty() = 0;
};

class BFSOpenList : public OpenList
{
private:
  std::queue<State *> queue;

public:
  void push(State *state) override;
  PopResult pop() override;
  bool isEmpty() override;
};

struct PriorityState
{
  State *state;
  int cost;

  PriorityState(State *state, int cost) : state(state), cost(cost) {}

  bool operator<(const PriorityState &other) const
  {
    return this->cost > other.cost;
  }
};

class AStarOpenList : public OpenList
{
private:
  std::priority_queue<PriorityState> queue;

public:
  void push(State *state) override;
  PopResult pop() override;
  bool isEmpty() override;
};

template <class T>
SearchResult search(State *initialState)
{
  SearchResult result{};
  result.state = nullptr;
  result.expandedNodes = 0;

  T openList;
  std::set<unsigned long long> closedList;

  openList.push(initialState);

  while (!openList.isEmpty())
  {
    PopResult res = openList.pop();
    State *state = res.state;

    if (!closedList.contains(state->getState()))
    {
      result.heuristicValues.push_back(res.cost);

      closedList.insert(state->getState());

      if (state->isGoal())
      {
        result.state = state;
        break;
      }

      std::set<State *> children = state->getChildren();

      for (auto it : children)
      {
        openList.push(it);
      }

      result.expandedNodes++;
    }
  }

  return result;
}

#endif