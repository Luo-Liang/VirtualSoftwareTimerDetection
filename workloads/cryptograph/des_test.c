/*
 * The des example using APIs from openssl.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/des.h>
#include <openssl/rand.h>
 
#define BUFSIZE  64
#define DES_ITER 4096 * 4096
 
int main(void)
{
    char in[BUFSIZE], out[BUFSIZE], back[BUFSIZE];
    char *e = out;
    int i;
 
    DES_cblock key;
    DES_cblock seed = {0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10};
    DES_key_schedule keysched;
 
    memset(in, 0, sizeof(in));
    memset(out, 0, sizeof(out));
    memset(back, 0, sizeof(back));
 
    RAND_seed(seed, sizeof(DES_cblock));
    DES_random_key(&key);
    DES_set_key((C_Block *)key, &keysched);
 
    /* 8 bytes of plaintext */
    strcpy(in, "CSE564ML");
    printf("Plaintext: [%s]\n", in);

    for (i = 0; i < DES_ITER; i++) 
        DES_ecb_encrypt((C_Block *)in,(C_Block *)out, &keysched, DES_ENCRYPT);
 
    printf("Ciphertext:");
    while (*e) printf(" [%02x]", *e++);
    printf("\n");
 
    for (i = 0; i < DES_ITER; i++) 
        DES_ecb_encrypt((C_Block *)out,(C_Block *)back, &keysched, DES_DECRYPT);
 
    printf("Decrypted Text: [%s]\n", back);
 
    return(0);
}

