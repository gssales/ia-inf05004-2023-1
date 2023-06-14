#include "8puzzle.h"

Node8 CreateNode8(char* initial_state) {
  Node8 node = Node8();
  char* ptoken;
  uint8_t pos = 0, zero = 0;
  ptoken = strtok(initial_state, " ");
  while (ptoken != NULL) {
    uint32_t val = atoi(ptoken);

    if (val > 0) val--;
    else zero = pos;

    val = val << (29 - pos*3);
    node.data = node.data ^ val;

    ptoken = strtok(NULL, " ");
    pos++;
  }
  uint32_t l = (zero / 3) << 3, c = (zero % 3) << 1;
  node.data = node.data ^ l ^ c;  

  return node;
};

void PrintState8(uint32_t data) {
  uint32_t zeroL = (data & (3 << 3)) >> 3, zeroC = (data & (3 << 1)) >> 1;
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