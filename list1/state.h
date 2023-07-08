#ifndef STATE_H
#define STATE_H

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

  unsigned long long swap(int newEmpty);

  State *moveUp();
  State *moveDown();
  State *moveLeft();
  State *moveRight();
  int manhattanDistance();

public:
  static State *createInitialState(const std::string &description);
  void printState();
  int getPosition(int index) const;
  int getEmptyPosition();
  bool isGoal();
  std::list<State *> getChildren();
  unsigned long long getState() const;
  char getPuzzleType();
  unsigned int getDepth();
  int getHeuristicValue();
};

#endif