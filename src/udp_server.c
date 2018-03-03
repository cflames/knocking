#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 

#include "aes.h"
#include "dns.h"

#define BUFFER_LENGTH 1024

int setup_socket(int type, char* addr, int port)
{
    int server_socket;
    struct sockaddr_in server_addr;

    // init server address struct
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(addr);
    server_addr.sin_port = htons(port);

    // create socket for TCP
    if( (server_socket = socket(PF_INET, type, 0)) < 0 )
    {  
        printf("Failed to create tcp socket\n");
        return -1;
    }
   
    // bind socket to address
    if ( bind(server_socket, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0 )
    {
        printf("Failed to bind tcp socket to address\n");
        return -1;
    }

    return server_socket;
    
}
/*
* Function: run_tcp_server
* ---------------------
* run a tcp server at 443 port
*
*  return:   0, success
*           -1, if there is any error 
*/
int run_tcp_server()
{
    int server_socket, client_socket;
    struct sockaddr_in client_addr;
    int sin_size;

    server_socket = setup_socket(SOCK_STREAM, "127.0.0.1", 443);
    if ( server_socket == -1 )
    {
        printf("Failed to setup tcp socket\n");
        return -1;
    }

    // run tcp listener, the length for listen queue is 5
    listen(server_socket, 5);

    sin_size = sizeof(struct sockaddr_in);

    if( (client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &sin_size)) < 0 )
    {
        printf("Failed to accept a client connection request\n");
        return -1;
    }

    printf("Accept client connection: %s\n",inet_ntoa(client_addr.sin_addr));

    char* message = "welcome";
    send(client_socket, message, strlen(message), 0);

    close(client_socket);
    close(server_socket);

    return 0;
}

/*
* Main function for udp socket server, ip and port are hardcoded
* Receive a message and exit
*/
int main()
{
    int udp_socket = -1;
    char buffer[BUFFER_LENGTH];
    
    char *decrypt_string  = NULL;
    char *password        = "1234567";
    int recv_length       = 0; // length for received message

    // Create UDP socket
    udp_socket = setup_socket(SOCK_DGRAM, "127.0.0.1", 53);
    if ( udp_socket == -1 )
    {
        printf("Failed to setup udp socket\n");
        return -1;
    }

    for (;;)
    {
        recv_length = recv(udp_socket, buffer, BUFFER_LENGTH, 0);
        if ( recv_length == -1 )
        {
            printf("Failed to receive message from client\n");
            close(udp_socket);
            return -1;
        }
        if ( recv_length > 0 )
        {
            printf("received: %d bytes\n", recv_length);
            break;
        }
    }
    
    close(udp_socket);

    // get the encrypted content from dns message
    char* qname =(unsigned char*)&buffer[sizeof(struct DNS_HEADER)];
    qname++;
    printf("Received DNS packet\n");

    int res = aes_decrypt(qname, &decrypt_string, recv_length);
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

    run_tcp_server();

    FREE_STR(decrypt_string)
    return 0;

}