#ifndef SEARCH_H
#define SEARCH_H

#include <list>
#include <queue>
#include <iostream>
#include <vector>
#include <set>

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
  unsigned long long insertionOrder;

  PriorityState(State *state, int cost, unsigned long long insertionOrder) : state(state), cost(cost), insertionOrder(insertionOrder) {}

  bool operator<(const PriorityState &rhs) const
  {
    return this->insertionOrder < rhs.insertionOrder;
  }
};

struct AStarPriorityComparator
{
  bool operator()(const PriorityState &lhs, const PriorityState &rhs) const
  {
    // Compare the f values of two states
    if (lhs.cost != rhs.cost)
    {
      return lhs.cost > rhs.cost;
    }

    // If the f values are equal, compare the h values
    auto thisMD = lhs.state->manhattanDistance();
    auto otherMD = rhs.state->manhattanDistance();

    if (thisMD != otherMD)
    {
      return thisMD > otherMD;
    }

    // If the h values are equal, LIFO
    return lhs.insertionOrder < rhs.insertionOrder;
  }
};

class AStarOpenList : public OpenList
{
private:
  std::priority_queue<PriorityState, std::vector<PriorityState>, AStarPriorityComparator> queue;
  unsigned long long insertionOrderCounter = 0;

public:
  void push(State *state) override;
  PopResult pop() override;
  bool isEmpty() override;
};

struct GBFSPriorityComparator
{
  bool operator()(const PriorityState &lhs, const PriorityState &rhs) const
  {
    // Compare the h values of two states
    if (lhs.cost != rhs.cost)
    {
      return lhs.cost > rhs.cost;
    }

    // If the h values are equal, compare the g values
    int lhsDepth = lhs.state->getDepth();
    int rhsDepth = rhs.state->getDepth();

    if (lhsDepth != rhsDepth)
    {
      return lhsDepth < rhsDepth;
    }

    // If the g values are equal, LIFO
    return lhs.insertionOrder < rhs.insertionOrder;
  }
};

class GBFSOpenList : public OpenList
{
private:
  std::priority_queue<PriorityState, std::vector<PriorityState>, GBFSPriorityComparator> queue;
  unsigned long long insertionOrderCounter = 0;

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

      std::list<State *> children = state->getChildren();

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