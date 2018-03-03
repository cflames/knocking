#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

#include "aes.h"

#define BUFFER_LENGTH 1024

/*
* Main function for udp socket server, ip and port are hardcoded
* Receive a message and exit
*/
int main()
{
    int udp_socket = -1;
    char buffer[BUFFER_LENGTH];
    struct sockaddr_storage serverStorage;
    struct sockaddr_in server_addr;
    int sin_size;
    
    char *decrypt_string  = NULL;
    char *password        = "1234567";
    int recv_length       = 0; // length for received message

    /*Create UDP socket*/
    udp_socket = socket(PF_INET, SOCK_DGRAM, 0);
    if ( udp_socket < 0 )
    {
        printf("Failed to create udp socket\n");
        return -1;
    }

    /*Configure address struct*/
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family      = AF_INET;
    server_addr.sin_port        = htons(53);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if ( bind(udp_socket, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0 )
    {
        printf("bind is failed for udp socket\n");
        return -1;
    }

    for (;;)
    {
        recv_length = recv(udp_socket, buffer, BUFFER_LENGTH, 0);
        if ( recv_length == -1 )
        {
            printf("Failed to receive message from client\n");
            return -1;
        }
        if ( recv_length > 0 )
        {
            printf("received: %d bytes\n", recv_length);
            break;
        }
    }

    int res = aes_decrypt(buffer, &decrypt_string, recv_length);
    if ( res != 0 )
    {
        printf("Failed to decrypt string\n");
        FREE_STR(decrypt_string)
        return -1;
    }
    if ( strcmp(decrypt_string, password) != 0 )
    {
        printf("Password incorrect\n");
        //send(udp_socket, "")
        FREE_STR(decrypt_string)
        return -1;
    }

    printf("Received string: %s\n", decrypt_string);

    FREE_STR(decrypt_string)
    return 0;

}