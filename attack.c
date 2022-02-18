#include "attack.h"
#include "aes.h"
#include "invsbox.h"
block* generateLambda(unsigned int val) {
    word key[4] = {0x00010203, 0x04050607, 0x08090a0b, 0x0c0d0e0f};
    AES* aes = create_aes_instance(key);

    block* ret = calloc(LAMBDA_SIZE, sizeof(block));
    for (int i = 0; i < LAMBDA_SIZE; i++) {
        ret[i][0] = (i << 24) | (val << 16) | (val << 8) | val;
        ret[i][1] = (val << 24) | (val << 16) | (val << 8) | val;
        ret[i][2] = (val << 24) | (val << 16) | (val << 8) | val;
        ret[i][3] = (val << 24) | (val << 16) | (val << 8) | val;
        encrypt(aes, ret[i]);
    }

    delete_aes_instance(aes);
    return ret;
}

unsigned int* reverseLastRound(
    block* lambdaSet,
    unsigned int keyByte,
    unsigned int position
) {
    unsigned int* ret = calloc(LAMBDA_SIZE, sizeof(unsigned int));
    for (int i = 0; i < LAMBDA_SIZE; i++) {
        // Revert AddRoundKey & SubBytes
        unsigned int byteToRevert = lambdaSet[i][position / 4];
        byteToRevert &= 0xff000000 >> ((position % 4) * 8);
        byteToRevert >>= (24 - ((position % 4)) * 8);
        ret[i] = SI[byteToRevert ^ keyByte];
    }
    return ret;
}

unsigned int isGuessValid(unsigned int* lambdaSet) {
    unsigned int checkSum = 0;
    for (int i = 0; i < LAMBDA_SIZE; i++){
        checkSum ^= lambdaSet[i];
    }

    return checkSum == 0;
}

bool allset(bool* B_a, unsigned int size) {
    bool b = true;
    for (unsigned int i = 0; i < size; i++){
        b &= B_a[i];
    }
    return b;
}

unsigned int* guess(){
    unsigned int* g = malloc(BLOCK_SIZE * sizeof(int)); // Guess array
    bool setpos[BLOCK_SIZE] = { false };
    unsigned int* ret;
    int o = 0; // Offset for generating new Lambda sets
    while (!allset(setpos, BLOCK_SIZE)){
        block* lambda = generateLambda(o++);
        for (unsigned int position = 0; position < BLOCK_SIZE; position++){
            unsigned int keepindex, ncandidates = 0;

            for (unsigned int keyByte = 0; keyByte < LAMBDA_SIZE; keyByte++) {
                ret = reverseLastRound(lambda,keyByte,position);
                if (isGuessValid(ret)) {
                    ncandidates++;
                    keepindex = keyByte;
                }
                free(ret);
            }

            // If ncandidates was incremented exactly once
            // keepindex was set exactly once
            if(ncandidates == 1) {
                g[position] = keepindex;
                setpos[position] = true;
            }
        }
        free(lambda);
    }
    g[0] = (g[0] << 24) | (g[1] << 16) | (g[2] << 8) | g[3];
    g[1] = (g[4] << 24) | (g[5] << 16) | (g[6] << 8) | g[7];
    g[2] = (g[8] << 24) | (g[9] << 16) | (g[10] << 8) | g[11];
    g[3] = (g[12] << 24) | (g[13] << 16) | (g[14] << 8) | g[15];

    return g;
}
