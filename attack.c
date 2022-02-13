#include "attack.h"
#include "aes.h"
#include <stdlib.h>

// Generates an array of 256 blocks, where the first byte equals 00-ff,
// others stay the same
block* generateDelta() {
    block* ret = calloc(256, sizeof(block));
    for (int i = 0; i < 256; i++)
        ret[i][0] = (0x00 + i) << 24;
    return ret;
}

// Reverses AddRoundKey and SubBytes in a chosen byte position
void reverseLastRound(block* toRevert, block lastKey, int bytePosition) {
    // Revert AddRoundKey
    for (int i = 0; i < 4; i++) {
        (*toRevert)[i] ^= lastKey[i];
    }

    // RevertSubBytes
    InvSubBytes(*toRevert);

    // Cut out the specified position
    word ret = (*toRevert)[bytePosition / 4] & (0xff << (24 - (bytePosition % 4) * 8));
    (*toRevert)[0] = 0x0;
    (*toRevert)[1] = 0x0;
    (*toRevert)[2] = 0x0;
    (*toRevert)[3] = 0x0;
    (*toRevert)[bytePosition / 4] = ret;
}
