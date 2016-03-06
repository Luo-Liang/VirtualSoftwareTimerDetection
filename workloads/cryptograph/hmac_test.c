/*
 * The HMAC example using APIs from openssl.
 */
#include <stdio.h>
#include <string.h>
#include <openssl/hmac.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#define HMAC_ITER 4096 * 4096
 
int main()
{
    // The key to hash
    char key[] = "012345678";
    // The data that we're going to hash using HMAC
    char data[] = "This is the project for CSE564";
    unsigned char* digest;
    int i = 0;
   
    pid_t pid = fork();
    if (pid == 0) {
        static char *argv[] = {"", NULL};
        execv("./software_vt", argv);
        exit(127);
    } else {

        while (i < HMAC_ITER) { 
            // Using sha1 hash engine here.
            // You may use other hash engines. e.g EVP_md5(), EVP_sha224, EVP_sha512, etc
            digest = HMAC(EVP_sha1(), key, strlen(key), (unsigned char*)data, strlen(data),
                      NULL, NULL);
            i++;
        }
        waitpid(pid, 0, 0);
    }
 
    // Be careful of the length of string with the choosen hash engine. SHA1 
    // produces a 20-byte hash value which rendered as 40 characters. Change
    // the length accordingly with your choosen hash engine
    char mdString[20];
    for(i = 0; i < 20; i++)
         sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
 
    printf("HMAC digest: %s\n", mdString);
 
    return 0;
}
