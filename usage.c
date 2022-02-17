/*
KEY TO CRACK = 000102030405060708090a0b0c0d0e0f (128 bit)

1:d6aa74fdd2af72fadaa678f1d6ab76fe
2:b692cf0b643dbdf1be9bc5006830b3fe
3:b6ff744ed2c2c9bf6c590cbf0469bf41

LAST ROUND KEY = 47f7f7bc95353e03f96c32bcfd058dfd
*/

#include "aes.h"
#include "attack.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    unsigned int* ret = checkGuess();
    dumpMessage("LAST: ", ret);
    ret = invkeyschadule128(ret);
    dumpMessage("ORIG: ", ret);
    free(ret);
    return 0;
}
