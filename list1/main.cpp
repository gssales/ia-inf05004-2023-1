#include <iostream>
#include <cstring>
#include <cstdlib>

#include "8puzzle.h"
#include "15puzzle.h"

/* 8-puzzle
 * Needs 9 values 0 to 8. Conventionally needs 4 bits to represent (at least 5 bytes),
 * but we can represent with 3 bit + 4 bit to address the zero (4 bytes).
 * 0 1 2 3 4 5 6 7 8
 * 000 000 001 010 011 100 101 110 111 00000
 * 0000_0000 1010_0111 0010_1110 1110_0000
 *
 * 15-puzzle
 * Needs 16 values 0 to 15. Must be represented with 4 bits (8 bytes).
 * 0000_0001 0010_0011 0100_0101 0110_0111 
 * 1000_1001 1010_1011 1100_1101 1110_1111
 *
 */

int main(int argc, char* argv[]) {
  std::cout << "Hello world" << std::endl;
  
  char str[] = "4 1 2 3 0 5 6 7 8\0";
  Node8 n = CreateNode8(str);
  PrintState8(n.data);

  char str16[] = "0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15\0";
  Node15 n2 = CreateNode15(str16);
  PrintState15(n2.data);

  return 0;
}
