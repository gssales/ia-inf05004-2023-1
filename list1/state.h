#ifndef STATE_H
#define STATE_H

/**
 * Sum of set bits in each state
 * Puzzle8 = 13
 * Puzzle15 = 32
*/
#define PUZZLE_8 13
#define PUZZLE_15 32

#include <string>
#include <list>


enum Action : unsigned char
{
  UP,
  DOWN,
  LEFT,
  RIGHT,
  NONE
};

std::string actionToString(Action action);

class State
{
private:
  unsigned long long state; // 8 B
  unsigned int depth; // 4 B
  unsigned char hValue; // 1 B
  Action action; // 1B

  bool canMoveUp();
  bool canMoveDown();
  bool canMoveLeft();
  bool canMoveRight();

  unsigned long long swap(unsigned char newEmpty);

  State *moveUp();
  State *moveDown();
  State *moveLeft();
  State *moveRight();

public:
  char getPuzzleType();
  static State *createInitialState(const std::string &description);
  void printState();
  unsigned char getPosition(int index) const;
  unsigned char getEmptyPosition();
  bool isGoal();
  std::list<State *> getChildren();
  unsigned long long getState();
  unsigned int getDepth();
  unsigned char getHeuristicValue();
  int manhattanDistance();
};

#endif