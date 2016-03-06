/*
 * Mix execution of four applications:
 * -AES
 * -DES
 * -HMAC
 * -RSA
 */

#include <stdlib.h>

int main(int argc, char **argv)
{
    //system("./aes_test");
    //system("./des_test");
    system("./hmac_test");
    system("./rsa_test");

    return 0;
}
