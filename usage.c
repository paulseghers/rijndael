/**
 * Assignment for 02255 Modern Cryptology Spring 2022 at DTU
 * Square attack on AES with 4 rounds and 128 bit key
 *
 * @date 18.02.2022
 * @author Group 5
 */
#include "aes.h"
#include "attack.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * KEY TO CRACK = 000102030405060708090a0b0c0d0e0f
 *
 * 1:d6aa74fdd2af72fadaa678f1d6ab76fe
 * 2:b692cf0b643dbdf1be9bc5006830b3fe
 * 3:b6ff744ed2c2c9bf6c590cbf0469bf41
 *
 * LAST ROUND KEY = 47f7f7bc95353e03f96c32bcfd058dfd
 */
int main() {
    // Guess the last key
    unsigned int* ret = guess();
    dumpMessage("LAST KEY: ", ret);

    // Calculate the original key from the last one
    ret = invkeyschedule128(ret);
    dumpMessage("ORIG KEY: ", ret);
    free(ret);

    return 0;
}
