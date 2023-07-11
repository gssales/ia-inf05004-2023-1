#include "state.h"

#include <iostream>
#include <list>
#include <sstream>
#include <iomanip>
#include <bit>

const unsigned long long GOAL_8 = 0x0000000876543210;
const unsigned long long GOAL_15 = 0xFEDCBA9876543210;

const char PUZZLE_8 = 13;
const char PUZZLE_15 = 32;

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
  default:
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

  // Create state
  int index = 0;

  for (unsigned long long value : positions)
  {
    state->state |= value << (index * 4);
    if (value == 0)
      state->empty = index;

    index++;
  }

  state->depth = 0;
  state->hValue = state->manhattanDistance();
  state->cost = state->depth + state->hValue;
  state->puzzleType = std::popcount(state->state);
  state->action = NONE;

  return state;
}

void State::printState()
{
  std::stringstream ss;

  ss << "Puzzle type: " << (this->getPuzzleType() == PUZZLE_8 ? "8" : "15") << std::endl;
  ss << "Is goal: " << (this->isGoal() ? "true" : "false") << std::endl;
  ss << "Empty position: " << (int)this->empty << std::endl;
  ss << "Action: " << actionToString(this->action) << std::endl;
  ss << "Depth: " << this->getDepth() << std::endl;
  ss << "HValue: " << this->getHeuristicValue() << std::endl;
  ss << "HValue: " << this->manhattanDistance() << std::endl;

  ss << "Board: " << std::endl;

  int size = this->getPuzzleType() == PUZZLE_8 ? 9 : 16;
  int width = this->getPuzzleType() == PUZZLE_8 ? 3 : 4;
  int positionWidth = this->getPuzzleType() == PUZZLE_8 ? 1 : 2;

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
  int size = this->getPuzzleType() == PUZZLE_8 ? 9 : 16;

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

int State::getEmpty()
{
  return (int)this->empty;
}

std::list<State *> State::getChildren()
{
  std::list<State *> children;

  if (this->canMoveUp())
  {
    children.push_back(this->moveUp());
  }

  if (this->canMoveLeft())
  {
    children.push_back(this->moveLeft());
  }

  if (this->canMoveRight())
  {
    children.push_back(this->moveRight());
  }

  if (this->canMoveDown())
  {
    children.push_back(this->moveDown());
  }

  return children;
}

bool State::canMoveUp()
{
  int puzzleWidth = this->puzzleType == PUZZLE_8 ? PUZZLE_8_WIDTH : PUZZLE_15_WIDTH;

  if (this->empty < puzzleWidth || this->action == DOWN)
  {
    return false;
  }

  return true;
}

bool State::canMoveDown()
{
  int puzzleWidth = this->puzzleType == PUZZLE_8 ? PUZZLE_8_WIDTH : PUZZLE_15_WIDTH;

  if (this->empty >= puzzleWidth * (puzzleWidth - 1) || this->action == UP)
  {
    return false;
  }

  return true;
}

bool State::canMoveLeft()
{
  int puzzleWidth = this->puzzleType == PUZZLE_8 ? PUZZLE_8_WIDTH : PUZZLE_15_WIDTH;

  if (this->empty % puzzleWidth == 0 || this->action == RIGHT)
  {
    return false;
  }

  return true;
}

bool State::canMoveRight()
{
  int puzzleWidth = this->puzzleType == PUZZLE_8 ? PUZZLE_8_WIDTH : PUZZLE_15_WIDTH;

  if (this->empty % puzzleWidth == puzzleWidth - 1 || this->action == LEFT)
  {
    return false;
  }

  return true;
}

unsigned long long State::swap(int newEmpty)
{
  unsigned long long nextState = this->state;

  int value = this->getPosition(newEmpty);

  nextState &= ~(0xFULL << (this->empty * 4));
  nextState |= (static_cast<unsigned long long>(value) << (this->empty * 4));

  nextState &= ~(0xFULL << (newEmpty * 4));

  nextState &= (this->puzzleType == PUZZLE_8 ? 0x0000000FFFFFFFFFULL : 0xFFFFFFFFFFFFFFFFULL);

  return nextState;
}

State *State::moveUp()
{
  int puzzleWidth = this->puzzleType == PUZZLE_8 ? PUZZLE_8_WIDTH : PUZZLE_15_WIDTH;

  auto *nextState = new State();

  unsigned char newEmpty = this->empty - puzzleWidth;
  nextState->state = this->swap(newEmpty);

  nextState->action = UP;
  nextState->depth = this->depth +1;
  nextState->empty = newEmpty;
  // nextState->hValue = nextState->manhattanDistance();
  nextState->hValue = this->getHeuristicValue() - this->manhattanDistance(nextState->empty) + nextState->manhattanDistance(this->empty);
  nextState->cost = nextState->depth + nextState->hValue;
  nextState->puzzleType = this->puzzleType;

  return nextState;
}

State *State::moveDown()
{
  int puzzleWidth = this->puzzleType == PUZZLE_8 ? PUZZLE_8_WIDTH : PUZZLE_15_WIDTH;

  auto *nextState = new State();

  unsigned char newEmpty = this->empty + puzzleWidth;
  nextState->state = this->swap(newEmpty);

  nextState->action = DOWN;
  nextState->depth = this->depth +1;
  nextState->empty = newEmpty;
  // nextState->hValue = nextState->manhattanDistance();
  nextState->hValue = this->getHeuristicValue() - this->manhattanDistance(nextState->empty) + nextState->manhattanDistance(this->empty);
  nextState->cost = nextState->depth + nextState->hValue;
  nextState->puzzleType = this->puzzleType;

  return nextState;
}

State *State::moveLeft()
{
  auto *nextState = new State();

  unsigned char newEmpty = this->empty - 1;
  nextState->state = this->swap(newEmpty);

  nextState->action = LEFT;
  nextState->depth = this->depth +1;
  nextState->empty = newEmpty;
  // nextState->hValue = nextState->manhattanDistance();
  nextState->hValue = this->getHeuristicValue() - this->manhattanDistance(nextState->empty) + nextState->manhattanDistance(this->empty);
  nextState->cost = nextState->depth + nextState->hValue;
  nextState->puzzleType = this->puzzleType;

  return nextState;
}

State *State::moveRight()
{
  auto *nextState = new State();

  unsigned char newEmpty = this->empty + 1;
  nextState->state = this->swap(newEmpty);

  nextState->action = RIGHT;
  nextState->depth = this->depth +1;
  nextState->empty = newEmpty;
  // nextState->hValue = nextState->manhattanDistance();
  nextState->hValue = this->getHeuristicValue() - this->manhattanDistance(nextState->empty) + nextState->manhattanDistance(this->empty);
  nextState->cost = nextState->depth + nextState->hValue;
  nextState->puzzleType = this->puzzleType;

  return nextState;
}

unsigned short State::getDepth()
{
  return this->depth;
}

unsigned short State::getCost()
{
  return this->cost;
}

int State::getHeuristicValue()
{
  return (int) this->hValue;
}

char State::getPuzzleType()
{
  return this->puzzleType;
}

int State::manhattanDistance()
{
  int distance = 0;
  int width = this->puzzleType == PUZZLE_8 ? PUZZLE_8_WIDTH : PUZZLE_15_WIDTH;

  for (int i = 0; i < width * width; i++)
  {
    int value = this->getPosition(i);

    if (value == 0)
    {
      continue;
    }

    int goalPosition = value;


    int x = i % width;
    int y = i / width;

    int goalX = goalPosition % width;
    int goalY = goalPosition / width;

    distance += abs(x - goalX) + abs(y - goalY);
  }

  return distance;
}

int State::manhattanDistance(int position)
{
  int width = this->puzzleType == PUZZLE_8 ? PUZZLE_8_WIDTH : PUZZLE_15_WIDTH;

  int value = this->getPosition(position);

  if (value == 0)
    return 0;

  int x = position % width;
  int y = position / width;

  int goalX = value % width;
  int goalY = value / width;

  return abs(x - goalX) + abs(y - goalY);
}