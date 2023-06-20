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