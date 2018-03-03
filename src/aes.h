#ifndef _ASE_H_
    #define _ASE_H_
        #define FREE_STR(ptr) if (ptr != NULL ) { free(ptr); }
        int aes_encrypt(char *input_string, char **encrypt_string);
        int aes_decrypt(char *encrypt_string, char **decrypt_string, int decrypted_string_length);
#endif
