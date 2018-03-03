#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aes.h"

/*
* Test for decryption
*/
int test_aes_decryption()
{
    int return_code = 0;
    char *result_string = "1234567";    
    char *decryption_string = NULL;
    char buffer[16] = { 0x76, 0x62, 0x59, 0x2E, 0x14, 0x17, 0x36, 0x0C,\
                        0x26, 0x90, 0x60, 0x30, 0xF9, 0x4F, 0xC2, 0x13 };

    aes_decrypt(buffer, &decryption_string, 16);   

    if ( strcmp(result_string, decryption_string) != 0 )
    {
        return_code = -1;

    }
    FREE_STR(decryption_string)

    return return_code;
}


/*
* Test for both encryption and decryption function
*/
int test_aes_encrypt_and_decrypt()
{
    int return_code = 0;
    char *test_string = "1234567";    
    char *decrypt_string = NULL;
    char *encrypt_string = NULL;

    int length = aes_encrypt(test_string, &encrypt_string);
    if ( length != 16 ) // the encrypted string length must be 16
    {
        printf("Encrypted string length is not correct\n");
        FREE_STR(encrypt_string)
        return -1;
    }

    // print result of encrypted
    for( int i= 0; i<length; i+=1 ){
        printf("%02x",(unsigned char)encrypt_string[i]);
    }
    printf("\n");

    return_code = aes_decrypt(encrypt_string, &decrypt_string, length);
    if ( return_code != 0 )
    {
        FREE_STR(encrypt_string)
        FREE_STR(decrypt_string)
        return -1;
    }

    printf("decrypted: %s\n", decrypt_string);

    if ( strcmp(decrypt_string, test_string) != 0 )
    {
        printf("decrypt result is not matched with input string\n");
        return_code = -1;
    }

    FREE_STR(encrypt_string)
    FREE_STR(decrypt_string)

    return return_code;

}

int main()
{


    if ( test_aes_encrypt_and_decrypt() != 0 )
    {
        printf("Test case test_aes_encrypt_and_decrypt is failed\n");
        return -1;
    }
    printf("Test case test_aes_encrypt_and_decrypt is passed\n");

    if ( test_aes_decryption() != 0 )
    {
        printf("Test case test_aes_decryption is failed\n");
        return -1;
    }
    printf("Test case test_aes_decryption is passed\n");

    return 0;
}
