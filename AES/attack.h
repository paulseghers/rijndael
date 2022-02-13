#ifndef ATTACK_H
#define ATTACK_H

#include <stddef.h> // size_t
#include <stdint.h> // uint8_t, uint32_t
#include <stdio.h>

typedef uint32_t word;  // A word is an unsigned of 32-bit size.
typedef word block[4];  // A block is an array of 4 words.

block* generateDelta();
void reverseLastRound(block* toRevert, block lastKey, int bytePosition);

#endif
