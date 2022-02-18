#include "aes.h"
#include "sbox.h"
#define KEYSIZE 4

void dumpMessage(char prefix[], block message) {
    printf("%s", prefix);
    printf("%08x", message[0]);
    printf("%08x", message[1]);
    printf("%08x", message[2]);
    printf("%08x", message[3]);
    printf("\n");
}

AES* create_aes_instance(word* key) {
    AES* aes = malloc(sizeof(AES));
    aes->key = malloc(KEYSIZE * sizeof(word));
    memcpy(aes->key, key, KEYSIZE * sizeof(word));
    aes->rounds = 4;
    keyschedule128(aes);
    return aes;
}

void delete_aes_instance(AES* aes) {
    free(aes->key);
    free(aes->round_keys);
}

void encrypt(AES* aes, word* m) {
    // Pre-whitening
    for (size_t w = 0; w < 4; ++w)
        m[w] ^= aes->round_keys[0][w];

    // Rounds
    for (unsigned int i = 1; i <= aes->rounds; i++) {
        SubBytes(m);
        ShiftRows(m);
        // Skip MixColumns on the last round
        if (i != aes->rounds)
            MixColumns(m);
        // AddRoundKey
        for (size_t w = 0; w < 4; ++w)
            m[w] ^= aes->round_keys[i][w];
    }
}

void keyschedule128(AES* aes) {
    block round_key;
    memcpy(round_key, aes->key, sizeof(block));
    word tword;
    aes->round_keys = malloc((aes->rounds + 1) * sizeof(block));

    memcpy(aes->round_keys[0], round_key, sizeof(block));
    for (size_t round = 1; round <= aes->rounds; ++round) {
        tword = S[(round_key[3] >> 16) & 0xFF];
        tword <<= 8;
        tword ^= S[(round_key[3] >> 8) & 0xFF];
        tword <<= 8;
        tword ^= S[round_key[3] & 0xFF];
        tword <<= 8;
        tword ^= S[(round_key[3] >> 24) & 0xFF];
        tword ^= R[round] << 24;

        round_key[0] ^= tword;
        round_key[1] ^= round_key[0];
        round_key[2] ^= round_key[1];
        round_key[3] ^= round_key[2];

        memcpy(aes->round_keys[round], round_key, sizeof(block));
    }
    return;
}

void SubBytes(word* m) {
    for (unsigned w = 0; w < 4; ++w) {
        m[w] = (S[m[w] >> 24] << 24) ^ (S[(m[w] >> 16) & 0xff] << 16) ^
           (S[(m[w] >> 8) & 0xff] << 8) ^ S[m[w] & 0xff];
     }
}

void ShiftRows(word* m) {
    word w0, w1, w2, w3;
    // Shift the second row
    w0 = m[0] & 0x00ff0000;
    w1 = m[1] & 0x00ff0000;
    w2 = m[2] & 0x00ff0000;
    w3 = m[3] & 0x00ff0000;
    m[0] ^= w0 ^ w1;
    m[1] ^= w1 ^ w2;
    m[2] ^= w2 ^ w3;
    m[3] ^= w3 ^ w0;

    // Shift the third row
    w0 = m[0] & 0x0000ff00;
    w1 = m[1] & 0x0000ff00;
    w2 = m[2] & 0x0000ff00;
    w3 = m[3] & 0x0000ff00;
    m[0] ^= w0 ^ w2;
    m[1] ^= w1 ^ w3;
    m[2] ^= w2 ^ w0;
    m[3] ^= w3 ^ w1;

    // Shift the fourth row
    w0 = m[0] & 0x000000ff;
    w1 = m[1] & 0x000000ff;
    w2 = m[2] & 0x000000ff;
    w3 = m[3] & 0x000000ff;
    m[0] ^= w0 ^ w3;
    m[1] ^= w1 ^ w0;
    m[2] ^= w2 ^ w1;
    m[3] ^= w3 ^ w2;
}

void MixColumns(word* m) {
    block t = {0, 0, 0, 0};
    for (unsigned i = 0; i < 4; ++i) {
        for (unsigned j = 0; j < 4; ++j) {
            // Calculate b1 = S(a)*1, b2 = S(a)*2 and b3 = S(a)*3
            word b1 = (m[i] >> 8 * (3 - j)) & 0xFF;
            word b2;
            if ((b1 & 0x80) == 0x80) {
                b2 = ((b1 << 1) & 0xFF) ^ 0x1b;
            } else {
                b2 = (b1 << 1) & 0xFF;
            }
            word b3 = b1 ^ b2;

            if (j == 0) {
                t[i] ^= (b2 << 24) ^ (b1 << 16) ^ (b1 << 8) ^ b3;
            } else if (j == 1) {
                t[i] ^= (b3 << 24) ^ (b2 << 16) ^ (b1 << 8) ^ b1;
            } else if (j == 2) {
                t[i] ^= (b1 << 24) ^ (b3 << 16) ^ (b2 << 8) ^ b1;
            } else if (j == 3) {
                t[i] ^= (b1 << 24) ^ (b1 << 16) ^ (b3 << 8) ^ b2;
            }
        }
    }
    memcpy(m, t, sizeof(block));
}

unsigned int* invkeyschedule128(unsigned int* lastKey) {
    unsigned int* currentKey = malloc(4*sizeof(int));
    memcpy(currentKey, lastKey, sizeof(int) * 4);
    int tword;
    for (int round = 3; round >= 0; round--) {
        currentKey[3] ^= currentKey[2];

        tword = S[(currentKey[3] >> 16) & 0xFF];
        tword <<= 8;
        tword ^= S[(currentKey[3] >> 8) & 0xFF];
        tword <<= 8;
        tword ^= S[currentKey[3] & 0xFF];
        tword <<= 8;
        tword ^= S[(currentKey[3] >> 24) & 0xFF];
        tword ^= R[round + 1] << 24;

        currentKey[2] ^= currentKey[1];
        currentKey[1] ^= currentKey[0];
        currentKey[0] ^= tword;
    }
    return currentKey;
}
