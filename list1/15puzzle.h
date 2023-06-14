#ifndef _15PUZZLE_H
#define _15PUZZLE_H

#include <iostream>
#include <cstdlib>
#include <cstring>

struct Node15 {
  uint64_t data;
  uint16_t level;
  uint16_t value;
};

Node15 CreateNode15(char* initial_state);

void PrintState15(uint64_t data);

#endif // _15PUZZLE_H