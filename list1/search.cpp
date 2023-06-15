#include "search.h"

void BFSOpenList::push(State *state)
{
  this->queue.push(state);
}

State *BFSOpenList::pop()
{
  State *state = this->queue.front();
  this->queue.pop();

  return state;
}

bool BFSOpenList::isEmpty()
{
  return this->queue.empty();
}