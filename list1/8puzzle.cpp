#include "8puzzle.h"

#include<list>

uint32_t Get8PuzzleData(char* initial_state) {
  uint32_t data = 0;
  uint8_t pos = 0, zero = 0;
  char* ptoken;
  ptoken = strtok(initial_state, " ");
  while (ptoken != NULL) {
    uint32_t val = atoi(ptoken);

    if (val > 0) val--;
    else zero = pos;

    val = val << (29 - pos*3);
    data = data ^ val;

    ptoken = strtok(NULL, " ");
    pos++;
  }

  uint32_t l = (zero / 3) << 2, c = (zero % 3);
  data = data ^ l ^ c;  

  return data;
}

std::list<Action> GetStateActions(uint32_t data) {
  std::list<uint32_t> actions;

  uint32_t zeroL = (data & (3 << 2)) >> 2, zeroC = data & 3;
  if (zeroL > 0) actions.push_back(Action.UP);
  if (zeroL < 2) actions.push_back(Action.DOWN);
  if (zeroC > 0) actions.push_back(Action.LEFT);
  if (zeroC < 2) actions.push_back(Action.RIGHT);

  return actions;
}

Node8 CreateNode8(char* initial_state) {
  Node8 node = Node8();
  node.data = Get8PuzzleData(initial_state);
  return node;
};

void PrintState8(uint32_t data) {
  uint32_t zeroL = (data & (3 << 2)) >> 2, zeroC = data & 3;
  uint8_t zero = zeroL*3 + zeroC;

  uint8_t shift = 29;
  uint32_t mask = 7 << shift;
  int pos = 0;
  while(pos < 9) {
    uint32_t val = (data & mask) >> shift;
    if (pos != zero) val++;
    std::cout << val << std::endl;

    mask = mask >> 3;
    shift -= 3;
    pos++;
  }
}

void BFS8Puzzle(char* initial_state, char* goal_state) {
  std::list<Node8> open;
  std::list<uint32_t> explored;
  Node8 node = CreateNode8(initial_state);
  uint32_t goal = Get8PuzzleData(goal_state);
  if (node.data == goal)
    return node;
  open.push_front(node);
  while(true) {
    if (open.empty()) /* return failure */;
    node = open.front();
    explored.push_front(node.data);

    open.pop_front();
  }
}