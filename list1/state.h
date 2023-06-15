#ifndef STATE_H
#define STATE_H

#include <string>

enum PuzzleType
{
  PUZZLE_8,
  PUZZLE_15
};

class State
{
private:
  PuzzleType puzzleType;
  unsigned long long state;

  std::string createBoard();

public:
  static State *createInitialState(std::string description);
  void printState();
  int getPosition(int index);
};

#endif