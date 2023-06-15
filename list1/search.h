#ifndef SEARCH_H
#define SEARCH_H

#include <set>
#include <queue>
#include <iostream>

#include "state.h"

struct SearchResult
{
  State *state;
  int expandedNodes;
};

class OpenList
{
  virtual void push(State *state) = 0;
  virtual State *pop() = 0;
  virtual bool isEmpty() = 0;
};

class BFSOpenList : public OpenList
{
private:
  std::queue<State *> queue;

public:
  void push(State *state) override;
  State *pop() override;
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
    State *state = openList.pop();

    if (!closedList.contains(state->getState()))
    {
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