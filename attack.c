#include "attack.h"
#include "aes.h"
#include <stdlib.h>
#include <stdbool.h>
// AES Inverse S-Box
unsigned char SI[] = {
    0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
    0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
    0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
    0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
    0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
    0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
    0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
    0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
    0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
    0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
    0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
    0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
    0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
    0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
    0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D };

// Generates an array of 256 blocks, where the first byte equals 00-ff,
// others stay set to inactiveValue
block* generateDelta(unsigned int inactiveValue) {
    word key[4] = {0x00010203, 0x04050607, 0x08090a0b, 0x0c0d0e0f};
    AES* aes = create_aes_instance(key);

    block* ret = calloc(256, sizeof(block));
    for (int i = 0; i < 256; i++) {
        ret[i][0] = i << 24;
        ret[i][0] |= inactiveValue << 16;
        ret[i][0] |= inactiveValue << 8;
        ret[i][0] |= inactiveValue;

        ret[i][1] |= inactiveValue << 24;
        ret[i][1] |= inactiveValue << 16;
        ret[i][1] |= inactiveValue << 8;
        ret[i][1] |= inactiveValue;

        ret[i][2] |= inactiveValue << 24;
        ret[i][2] |= inactiveValue << 16;
        ret[i][2] |= inactiveValue << 8;
        ret[i][2] |= inactiveValue;

        ret[i][3] |= inactiveValue << 24;
        ret[i][3] |= inactiveValue << 16;
        ret[i][3] |= inactiveValue << 8;
        ret[i][3] |= inactiveValue;
        encrypt(aes, ret[i]);
    }

    delete_aes_instance(aes);
    return ret;
}

// Reverses AddRoundKey and SubBytes in a chosen byte position
unsigned int* reverseLastRound(block* deltaSet, unsigned int keyByte, unsigned int position) {
    unsigned int* ret = calloc(256, sizeof(unsigned int));
    for (int i = 0; i < 256; i++) {
        // Revert AddRoundKey & SubBytes
        unsigned int byteToRevert = deltaSet[i][position / 4];
        byteToRevert &= 0xff000000 >> ((position % 4) * 8);
        byteToRevert >>= (24 - ((position % 4)) * 8);
        ret[i] = SI[byteToRevert ^ keyByte];
    }
    return ret;
}

// Return true if the array of words keeps the active set property
unsigned int isGuessValid(unsigned int* deltaSet, unsigned int keyByte, unsigned int position) {
    unsigned int checkSum = 0;// keyByte;
    for (int i = 0; i < 256; i++){
        checkSum ^= deltaSet[i];
        //printf("%d\n", i);
    }

    return checkSum == 0;
}

void crackLastKey() {
    // Solution
    block* delta = generateDelta(1);
    unsigned int* ret;

    for (int j = 0; j < 16; j++) {
        printf("%d\n", j);
        for (int i = 0; i < 256; i++) {
            ret = reverseLastRound(delta, i, j);
            if (isGuessValid(ret, i, j))
                printf("\t%x\n", i);
            free(ret);
        }
    }
}

bool allset(bool* B_a, int size){
    bool b = true;
    for (size_t i =0; i<size; i++){
        b &= B_a[i];
    }
    return b;
}


void checkGuess(){
    int* guessarray = malloc(16*sizeof(int)); //block* indexguess = malloc(256*sizeof(block));
    bool setpos[16] = { false };
    unsigned int* ret;
    int o = 1; //offset for generating new Lambda sets
    while (!allset(setpos, 16)){
        block* delta = generateDelta(o);
        for (int j = 0; j < 16; j++){
            int keepindex = 0;
            int ncandidates = 0;
            printf("%d\n", j);

            for (int i = 0; i < 256; i++) {
                ret = reverseLastRound(delta, i, j);
                if (isGuessValid(ret, i, j)){
                    ncandidates++;
                    keepindex = i;
                    printf("\t%x\n", i);
                    //printf("reeee");
                }
                free(ret);
            }
            //printf("n candidates: \t %d", ncandidates);
            if(ncandidates == 1){
                guessarray[j] = keepindex; //if ncandidates was incremented exactly once, keepindex was set exactly once
                setpos[j] = true;
            }
        o++;
     }
     free(delta);
    }
    printf("key array:\n[");
    for (size_t i=0; i<16; i++){
        printf("%x ", guessarray[i]);
    }
    printf("]\n");
    
    free(guessarray);
    //free(guessarray);
}
