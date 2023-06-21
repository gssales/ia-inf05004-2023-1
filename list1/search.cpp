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