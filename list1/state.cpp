#include "state.h"

#include <iostream>
#include <list>
#include <sstream>
#include <iomanip>

const unsigned long long GOAL_8 = 0x0000000876543210;
const unsigned long long GOAL_15 = 0xFEDCBA9876543210;

std::list<int> extractPositions(std::string description)
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

State *State::createInitialState(std::string description)
{
  State *state = new State();

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

  for (std::list<int>::iterator it = positions.begin(); it != positions.end(); ++it)
  {
    unsigned long long value = *it;

    state->state |= value << (index * 4);

    index++;
  }

  return state;
}

void State::printState()
{
  std::stringstream ss;

  ss << "Puzzle type: " << (this->puzzleType == PUZZLE_8 ? "8" : "15") << std::endl;
  ss << "Is goal: " << (this->isGoal() ? "true" : "false") << std::endl;
  ss << "Empty position: " << this->getEmptyPosition() << std::endl;

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

int State::getPosition(int index)
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