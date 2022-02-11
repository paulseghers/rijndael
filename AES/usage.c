#include "aes.h"
#include "attack.h"
#include <stdio.h>

int main() {
  word key[4] = {0x00010203, 0x04050607, 0x08090a0b, 0x0c0d0e0f};
  AES* aes = create_aes_instance(key);
  block message = {0x00112233, 0x44556677, 0x8899aabb, 0xccddeeff};
  dumpMessage("Original: \n\t", message);
  encrypt(aes, message);
  dumpMessage("Encrypted: \n\t", message);
  delete_aes_instance(aes);
  hello();
  return 0;
}
