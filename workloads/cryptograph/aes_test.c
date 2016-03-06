/*
 * The AES example using APIs from openssl.
 */
#include <openssl/aes.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#define AES_ITER    4096 * 4096 

static const unsigned char key[] = {
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
    0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
};

int main()
{
    unsigned char text[] = "This is the project for CSE564";
    unsigned char enc_out[80];
    unsigned char dec_out[80];
    AES_KEY enc_key, dec_key;
    int i = 0;

    pid_t pid = fork();
    if (pid == 0) {
        static char *argv[] = {"", NULL};
        execv("./software_vt", argv);
        exit(127);
    } else {

        while (i < AES_ITER) {
            AES_set_encrypt_key(key, 128, &enc_key);
            AES_encrypt(text, enc_out, &enc_key);      

            AES_set_decrypt_key(key,128,&dec_key);
            AES_decrypt(enc_out, dec_out, &dec_key);

            i++;
        }

        waitpid(pid, 0, 0);
    }

    printf("original:\t");
    for(i=0;*(text+i)!=0x00;i++)
        printf("%X ",*(text+i));
    printf("\nencrypted:\t");
    for(i=0;*(enc_out+i)!=0x00;i++)
        printf("%X ",*(enc_out+i));
    printf("\ndecrypted:\t");
    for(i=0;*(dec_out+i)!=0x00;i++)
        printf("%X ",*(dec_out+i));
    printf("\n");

    return 0;
}
