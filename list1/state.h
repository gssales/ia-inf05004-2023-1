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
  unsigned short depth; // 2 B
  unsigned short cost; // 2 B
  unsigned char hValue; // 1 B
  unsigned char puzzleType; // 1 B
  unsigned char empty; // 1 B
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
  int manhattanDistance(int position);

public:
  static State *createInitialState(const std::string &description);
  void printState();
  int getPosition(int index) const;
  int getEmptyPosition();
  int getEmpty();
  bool isGoal();
  std::list<State *> getChildren();
  unsigned long long getState() const;
  char getPuzzleType();
  unsigned short getDepth();
  unsigned short getCost();
  int getHeuristicValue();
};

#endif