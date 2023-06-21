#include "search.h"

void BFSOpenList::push(State *state)
{
  this->queue.push(state);
}

PopResult BFSOpenList::pop()
{
  State *state = this->queue.front();
  this->queue.pop();

  PopResult result{};

  result.state = state;
  result.cost = 0;

  return result;
}

bool BFSOpenList::isEmpty()
{
  return this->queue.empty();
}

void AStarOpenList::push(State *state)
{
  int cost = state->getDepth() + state->manhattanDistance();
  PriorityState priorityState(state, cost, this->insertionOrderCounter++);
  this->queue.push(priorityState);
}

PopResult AStarOpenList::pop()
{
  PriorityState priorityState = this->queue.top();
  this->queue.pop();

  PopResult result{};

  result.state = priorityState.state;
  result.cost = priorityState.cost;

  return result;
}

bool AStarOpenList::isEmpty()
{
  return this->queue.empty();
}

void GBFSOpenList::push(State *state)
{
  int cost = state->manhattanDistance();
  PriorityState priorityState(state, cost, this->insertionOrderCounter++);
  this->queue.push(priorityState);
}

PopResult GBFSOpenList::pop()
{
  PriorityState priorityState = this->queue.top();
  this->queue.pop();

  PopResult result{};

  result.state = priorityState.state;
  result.cost = priorityState.cost;

  return result;
}

bool GBFSOpenList::isEmpty()
{
  return this->queue.empty();
}

SearchResult depthLimitedSearch(State *state, unsigned long long maxDepth)
{
  SearchResult result{};

  if (state->isGoal())
  {
    result.state = state;

    return result;
  }

  if (maxDepth == 0)
  {
    return result;
  }

  auto children = state->getChildren();
  result.expandedNodes++;

  for (State *child : children)
  {
    auto innerResult = depthLimitedSearch(child, maxDepth - 1);

    result.expandedNodes += innerResult.expandedNodes;

    if (innerResult.state != nullptr)
    {
      result.state = innerResult.state;
      break;
    }
  }

  return result;
}

SearchResult iterativeDeepeningSearch(State *initialState, long long maxDepth)
{
  SearchResult result{};

  for (long long depth = 0; depth <= maxDepth; depth++)
  {
    auto innerResult = depthLimitedSearch(initialState, depth);

    result.expandedNodes += innerResult.expandedNodes;

    if (innerResult.state != nullptr)
    {
      result.state = innerResult.state;
      break;
    }
  }

  result.heuristicValues.push_back(0);

  return result;
}