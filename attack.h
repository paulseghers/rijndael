#ifndef ATTACK_H
#define ATTACK_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef unsigned int word; // A word is an unsigned of 32-bit size.
typedef word block[4]; // A block is an array of 4 words.

#define LAMBDA_SIZE 256
#define BLOCK_SIZE 16

/**
 * Generates an array of 256 blocks, where the first byte equals 00-ff
 *
 * @param val inactive value
 */
block* generateLambda(unsigned int val);

/**
 * Reverses AddRoundKey and SubBytes in a chosen byte position
 *
 * @param lambdaSet An array of aes blocks representing the lambda lambda set
 * @param keyByte Guess of a key byte
 * @param position Position of the guess
 */
unsigned int* reverseLastRound(
    block* lambdaSet,
    unsigned int keyByte,
    unsigned int position
);

/**
 * Return true if the lambda set keeps the active set property
 *
 * @param lambdaSet An array  of aes blocks
 */
unsigned int isGuessValid(unsigned int* lambdaSet);

/**
 * Returns true if all items in B_a are true
 *
 * @param B_a Array of booleans
 * @param size Size of the boolean array
 */
bool allset(bool* B_a, unsigned int size);

/**
 * Calculate and return the last key
 */
unsigned int* guess();

#endif
