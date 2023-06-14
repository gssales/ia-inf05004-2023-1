#include <iostream>
#include <cstring>
#include <cstdlib>

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

struct Node8 {
	uint32_t data;
	uint16_t level;
	uint16_t value;
};

struct Node16 {
	uint64_t data;
	uint16_t level;
	uint16_t value;
};

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

Node16 CreateNode16(char* initial_state) {
	Node16 node = Node16();

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

void PrintState16(uint64_t data) {
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

int main(int argc, char* argv[]) {
	char str[] = "4 1 2 3 0 5 6 7 8\0";
	Node8 n = CreateNode8(str);
	PrintState8(n.data);

	char str16[] = "0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15\0";
	Node16 n2 = CreateNode16(str16);
	PrintState16(n2.data);

	std::cout << "Hello world" << std::endl;
	return 0;
}
