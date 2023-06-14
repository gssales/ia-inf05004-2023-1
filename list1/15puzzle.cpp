#include "15puzzle.h"

Node15 CreateNode15(char* initial_state) {
  Node15 node = Node15();

  char* ptoken;
  uint8_t shift = 60;
  ptoken = strtok(initial_state, " ");
  while (ptoken != NULL) {
    uint64_t val = atoi(ptoken);

    val = val << shift;
    node.data = node.data ^ val;

    ptoken = strtok(NULL, " ");
    shift -= 4;
  }

  return node;
}

void PrintState15(uint64_t data) {
  uint8_t shift = 60;
  uint64_t mask = 15 << shift;
  uint8_t pos = 0;
  while(pos < 16) {
    uint64_t val = (data & mask) >> shift;
    std::cout << val << std::endl;

    mask = mask >> 4;
    shift -= 4;
  pos++;
  }
}