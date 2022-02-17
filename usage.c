/*
KEY TO CRACK = 000102030405060708090a0b0c0d0e0f (128 bit)

LAST ROUND KEY = 47f7f7bc95353e03f96c32bcfd058dfd
*/

#include "aes.h"
#include "attack.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    crackLastKey();
    return 0;
}
