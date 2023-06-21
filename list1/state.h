#ifndef STATE_H
#define STATE_H

#include <string>
#include <list>

enum PuzzleType
{
  PUZZLE_8,
  PUZZLE_15
};

enum Action
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
  PuzzleType puzzleType;
  unsigned long long state;
  State *parent;
  Action action;

  std::string createBoard();

  bool canMoveUp();
  bool canMoveDown();
  bool canMoveLeft();
  bool canMoveRight();

  unsigned long long swap(int newEmpty);

  State *moveUp();
  State *moveDown();
  State *moveLeft();
  State *moveRight();

public:
  static State *createInitialState(const std::string &description);
  void printState();
  int getPosition(int index) const;
  int getEmptyPosition();
  bool isGoal();
  std::list<State *> getChildren();
  unsigned long long getState() const;
  int getDepth();
  int manhattanDistance();
};

#endif