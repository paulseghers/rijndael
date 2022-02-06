#include <stdio.h>
#include <stdlib.h>
#include "aessbox.h"
#define BYTE_COUNT 16

void dumpState(unsigned char state[]) {
    printf("state:= \n");
    for (int i = 0; i < BYTE_COUNT; i++) {
        printf("%x ", state[i]);
        if (4-(i%4) == 1){
        	printf("\n");
        }
    }
    printf("\n");
}

void subBytes(unsigned char* s){
    for (int i = 0; i < BYTE_COUNT; i++) {
        s[i] = S[(int)s[i]];
    }
}

void shiftRows(unsigned char* s) {
    unsigned char* savestate = (unsigned char*)malloc(BYTE_COUNT*sizeof(unsigned char));
    for (int t = 0; t < BYTE_COUNT; t++){savestate[t] = s[t];}
    
    for (int d = 0; d < 4; d++){
	    for (int i = 0; i < 4; i++){
	    	s[4*d+i] = savestate[4*d + (i+d)%4];
	    }
    }
    free(savestate);
}

void galois_Colmul(unsigned char* s){
	unsigned char* a = (unsigned char*)malloc(4*sizeof(unsigned char)); //bad memory use but ok bc fixed size
	unsigned char* b = (unsigned char*)malloc(4*sizeof(unsigned char));
	unsigned char c;
    unsigned char h;
	for (c = 0; c < 4; c++) {
        a[c] = s[c];
        /* h should be 0xff if the high bit of r[c] is set, 0 otherwise */
        h = (s[c] >> 7) & 1; /* arithmetic right shift, thus shifting in either zeros or ones */
        b[c] = s[c] << 1; /* implicitly removes high bit because b[c] is an 8-bit char, so we xor by 0x1b and not 0x11b in the next line */
        b[c] ^= h * 0x1B; //idk
    }
	s[0] = b[0] ^ a[3] ^ a[2] ^ b[1] ^ a[1]; /* 2 * a0 + a3 + a2 + 3 * a1 */
    s[1] = b[1] ^ a[0] ^ a[3] ^ b[2] ^ a[2]; /* 2 * a1 + a0 + a3 + 3 * a2 */
    s[2] = b[2] ^ a[1] ^ a[0] ^ b[3] ^ a[3]; /* 2 * a2 + a1 + a0 + 3 * a3 */
    s[3] = b[3] ^ a[2] ^ a[1] ^ b[0] ^ a[0]; /* 2 * a3 + a2 + a1 + 3 * a0 */
}

/*
0 1 2 3 
4 5 6 7
8 9 10 11
12 12 14 15
*/


void mixColumns(unsigned char* s){
    for (int j = 0; j<4; j++){ //col index (+)
        unsigned char* a = (unsigned char*)malloc(4*sizeof(unsigned char));
        for (int i = 0; i<4; i++){ //row index (*)
            a[i] = s[4*i + j];
        }
        galois_Colmul(a);
        for (int i = 0; i<4; i++){ //row index (*)
             s[4*i + j] = a[i];
        }
    }
}


int main(int argc, char** argv) {
    unsigned char aes_state[BYTE_COUNT] = "12345678asdfghjk";
    /*unsigned char aes_state[BYTE_COUNT] = 
    {0xdb, 0xf2, 0x01, 0x2d,
    0x13, 0x0a, 0x01, 0x26,
    0x53, 0x22, 0x01, 0x31,
    0x45, 0x5c, 0x01, 0x4c};*/

    // SubBytes
    subBytes(aes_state);
    printf("sub_bytes:\n");
   	dumpState(aes_state);

    // ShiftRows
    /*16 8-bit integers
    -> cast to 4 32bit ints 
    wrapping left shift on ints
    new bytes are right*/
    printf("shift_rows:\n");
   	shiftRows(aes_state);
    dumpState(aes_state);

    // MixCols
    printf("mix_cols:\n");
    mixColumns(aes_state);
    dumpState(aes_state);
}
