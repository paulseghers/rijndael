#ifndef AES_H_
#define AES_H_

#include <stddef.h> // size_t
#include <stdint.h> // uint8_t, uint32_t

typedef uint32_t word;  // A word is an unsigned of 32-bit size.
typedef word block[4];  // A block is an array of 4 words.

typedef struct AES {
  word* key;
  size_t rounds;
  block* round_keys;
  block* inv_round_keys;
} AES;

void dumpMessage(char prefix[], block message);
AES* create_aes_instance(word* key);
void delete_aes_instance(AES* aes);
void encrypt(AES* aes, word* m);
void keyschedule128(AES* aes);
void SubBytes(word* m);
void ShiftRows(word* m);
void MixColumns(word* m);
unsigned char** genLambdaset();

#endif
