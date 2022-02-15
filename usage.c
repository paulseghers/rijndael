#include "aes.h"
#include "attack.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    word key[4] = {0x00010203, 0x04050607, 0x08090a0b, 0x0c0d0e0f};
    AES* aes = create_aes_instance(key);

    // Generate delta set
    block* delta = generateDelta(0);

    // Encrypt delta set
    for (int i = 0; i < 256; i++) {
        encrypt(aes, delta[i]);
    }

    delete_aes_instance(aes);
    free(delta);
    return 0;
}
