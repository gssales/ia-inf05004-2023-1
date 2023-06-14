#ifndef _8PUZZLE_H
#define _8PUZZLE_H

#include <iostream>
#include <cstdlib>
#include <cstring>

struct Node8 {
  uint32_t data;
  uint16_t level;
  uint16_t value;
};

Node8 CreateNode8(char* initial_state);

void PrintState8(uint32_t data);

#endif // _8PUZZLE_H