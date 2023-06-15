#include "state.h"

#include <iostream>
#include <list>
#include <sstream>
#include <iomanip>

const unsigned long long GOAL_8 = 0x0000000876543210;
const unsigned long long GOAL_15 = 0xFEDCBA9876543210;

const short PUZZLE_8_WIDTH = 3;
const short PUZZLE_15_WIDTH = 4;

std::string actionToString(Action action)
{
  switch (action)
  {
  case UP:
    return "UP";
  case DOWN:
    return "DOWN";
  case LEFT:
    return "LEFT";
  case RIGHT:
    return "RIGHT";
  case NONE:
    return "NONE";
  }
}

std::list<int> extractPositions(const std::string &description)
{
  std::list<int> positions;

  std::stringstream ss(description);

  std::string token;

  while (std::getline(ss, token, ' '))
  {
    positions.push_back(std::stoi(token));
  }

  return positions;
}

State *State::createInitialState(const std::string &description)
{
  auto *state = new State();

  // Extract puzzle size
  std::list<int> positions = extractPositions(description);

  if (positions.size() == 9)
  {
    state->puzzleType = PUZZLE_8;
  }
  else
  {
    state->puzzleType = PUZZLE_15;
  }

  // Create state
  int index = 0;

  for (unsigned long long value : positions)
  {
    state->state |= value << (index * 4);

    index++;
  }

  // Set parent and action
  state->parent = nullptr;
  state->action = NONE;

  return state;
}

void State::printState()
{
  std::stringstream ss;

  ss << "Puzzle type: " << (this->puzzleType == PUZZLE_8 ? "8" : "15") << std::endl;
  ss << "Is goal: " << (this->isGoal() ? "true" : "false") << std::endl;
  ss << "Empty position: " << this->getEmptyPosition() << std::endl;
  ss << "Action: " << actionToString(this->action) << std::endl;
  ss << "Depth: " << this->getDepth() << std::endl;

  ss << "Board: " << std::endl;

  int size = this->puzzleType == PUZZLE_8 ? 9 : 16;
  int width = this->puzzleType == PUZZLE_8 ? 3 : 4;
  int positionWidth = this->puzzleType == PUZZLE_8 ? 1 : 2;

  for (int i = 0; i < size; i++)
  {
    int value = this->getPosition(i);

    if (value == 0)
    {
      ss << std::setw(positionWidth) << "";
    }
    else
    {
      ss << std::setw(positionWidth) << value;
    }

    if ((i + 1) % width == 0)
    {
      ss << std::endl;
    }
    else
    {
      ss << " ";
    }
  }

  std::cout << ss.str();
}

int State::getPosition(int index) const
{
  return (this->state >> (index * 4)) & 0xF;
}

int State::getEmptyPosition()
{
  int size = this->puzzleType == PUZZLE_8 ? 9 : 16;

  for (int i = 0; i < size; i++)
  {
    if (this->getPosition(i) == 0)
    {
      return i;
    }
  }

  return -1;
}

bool State::isGoal()
{
  return this->state == (this->puzzleType == PUZZLE_8 ? GOAL_8 : GOAL_15);
}

unsigned long long State::getState() const
{
  return this->state;
}

std::set<State *> State::getChildren()
{
  std::set<State *> children;

  if (this->canMoveLeft())
  {
    children.insert(this->moveLeft());
  }

  if (this->canMoveRight())
  {
    children.insert(this->moveRight());
  }

  if (this->canMoveUp())
  {
    children.insert(this->moveUp());
  }

  if (this->canMoveDown())
  {
    children.insert(this->moveDown());
  }

  return children;
}

bool State::canMoveUp()
{
  int emptyPostion = this->getEmptyPosition();
  int puzzleWidth = this->puzzleType == PUZZLE_8 ? PUZZLE_8_WIDTH : PUZZLE_15_WIDTH;

  if (emptyPostion < puzzleWidth || this->action == DOWN)
  {
    return false;
  }

  return true;
}

bool State::canMoveDown()
{
  int emptyPostion = this->getEmptyPosition();
  int puzzleWidth = this->puzzleType == PUZZLE_8 ? PUZZLE_8_WIDTH : PUZZLE_15_WIDTH;

  if (emptyPostion >= puzzleWidth * (puzzleWidth - 1) || this->action == UP)
  {
    return false;
  }

  return true;
}

bool State::canMoveLeft()
{
  int emptyPostion = this->getEmptyPosition();
  int puzzleWidth = this->puzzleType == PUZZLE_8 ? PUZZLE_8_WIDTH : PUZZLE_15_WIDTH;

  if (emptyPostion % puzzleWidth == 0 || this->action == RIGHT)
  {
    return false;
  }

  return true;
}

bool State::canMoveRight()
{
  int emptyPostion = this->getEmptyPosition();
  int puzzleWidth = this->puzzleType == PUZZLE_8 ? PUZZLE_8_WIDTH : PUZZLE_15_WIDTH;

  if (emptyPostion % puzzleWidth == puzzleWidth - 1 || this->action == LEFT)
  {
    return false;
  }

  return true;
}

unsigned long long State::swap(int newEmpty)
{
  int emptyPostion = this->getEmptyPosition();

  unsigned long long nextState = this->state;

  int value = this->getPosition(newEmpty);

  nextState &= ~(0xFULL << (emptyPostion * 4));
  nextState |= (static_cast<unsigned long long>(value) << (emptyPostion * 4));

  nextState &= ~(0xFULL << (newEmpty * 4));

  nextState &= (this->puzzleType == PUZZLE_8 ? 0x0000000FFFFFFFFFULL : 0xFFFFFFFFFFFFFFFFULL);

  return nextState;
}

State *State::moveUp()
{
  int puzzleWidth = this->puzzleType == PUZZLE_8 ? PUZZLE_8_WIDTH : PUZZLE_15_WIDTH;

  auto *nextState = new State();

  nextState->state = this->swap(this->getEmptyPosition() - puzzleWidth);

  nextState->action = UP;
  nextState->parent = this;

  return nextState;
}

State *State::moveDown()
{
  int puzzleWidth = this->puzzleType == PUZZLE_8 ? PUZZLE_8_WIDTH : PUZZLE_15_WIDTH;

  auto *nextState = new State();

  nextState->state = this->swap(this->getEmptyPosition() + puzzleWidth);

  nextState->action = DOWN;
  nextState->parent = this;

  return nextState;
}

State *State::moveLeft()
{
  auto *nextState = new State();

  nextState->state = this->swap(this->getEmptyPosition() - 1);

  nextState->action = LEFT;
  nextState->parent = this;

  return nextState;
}

State *State::moveRight()
{
  auto *nextState = new State();

  nextState->state = this->swap(this->getEmptyPosition() + 1);

  nextState->action = RIGHT;
  nextState->parent = this;

  return nextState;
}

int State::getDepth()
{
  int depth = 0;

  State *p = this->parent;

  while (p != nullptr)
  {
    depth++;
    p = p->parent;
  }

  return depth;
}