#include <stdio.h>
#include <openssl/aes.h>
#include <stdlib.h>
#include <string.h>

// shared key for client and server
static const char shared_key[AES_BLOCK_SIZE] = { 0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,\
                                                 0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF };


/*
* Function: aes_encrypt
* ---------------------
* encrypt a string with aes cbc, it encrypts a string and 
* allocate a memory for the output encrypted string
*
*  input_string: the pointer for the string to be encrypted
*  encrypt_string: the pointer to the output string's pointer
*                  it will be updated to the new memory address
*
*  return:   the length of encrypted string
*            -1, if there is any error 
*/
int aes_encrypt(char *input_string, char **encrypt_string)
{
    AES_KEY aes_key;
    int encrypted_string_length = 0;
    unsigned char iv[AES_BLOCK_SIZE];        // init vector
    memset(iv, 0, AES_BLOCK_SIZE);

    // set the encryption length, length = 16 * n
    if ( ( strlen(input_string) + 1 ) % AES_BLOCK_SIZE == 0 ) 
    {
        encrypted_string_length = strlen(input_string) + 1;
    } 
    else 
    {
        encrypted_string_length = ( (strlen(input_string) + 1) / AES_BLOCK_SIZE + 1 ) * AES_BLOCK_SIZE;
    }

    if ( AES_set_encrypt_key(shared_key, 128, &aes_key) < 0 ) {
        printf("Failed to set AES encryption key\n");
        return -1;
    }

    // allocate new memory for output string
    *encrypt_string = (unsigned char*)calloc(encrypted_string_length, sizeof(unsigned char));    
    if ( *encrypt_string == NULL ) {
        printf("Failed to allocate memory for encrypt_string, length: %d\n", encrypted_string_length);
        return -1;
    }

    // encrypt (iv will change)
    AES_cbc_encrypt(input_string, *encrypt_string, encrypted_string_length, &aes_key, iv, AES_ENCRYPT);

    return encrypted_string_length;

}

/*
* Function: aes_decrypt
* ---------------------
* decrypt a input string, save the result to a new memory 
*
*  encrypt_string: the pointer to the encrypted string
*  decrypt_string: the pointer to the decrypted output string's pointer
*  encrypted_string_length: the length of encrypted string
*
*  return: 0, success
*          -1, failed
*/
int aes_decrypt(char *encrypt_string, char **decrypt_string, int encrypted_string_length)
{
    AES_KEY aes_key;
    unsigned char key[AES_BLOCK_SIZE];

    unsigned char iv[AES_BLOCK_SIZE]; 
    memset(iv, 0, AES_BLOCK_SIZE);

    // alloc decrypt_string
    *decrypt_string = (unsigned char*)calloc(encrypted_string_length, sizeof(unsigned char));
    if ( *decrypt_string == NULL ) {
        printf("Failed to allocate memory for decrypt_string, length: %d\n", encrypted_string_length);
        return -1;
    }

    if ( AES_set_decrypt_key(shared_key, 128, &aes_key) < 0 ) {
        printf("Failed to set AES decryption key\n");
        return -1;
    }

    // decrypt
    AES_cbc_encrypt(encrypt_string, *decrypt_string, encrypted_string_length, &aes_key, iv, AES_DECRYPT);

    return 0;
}