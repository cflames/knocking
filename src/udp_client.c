#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

/*
* Main function for udp socket client
* Just send a message to server
* server address and port are hardcoded
*/
int main()
{
    int client_socket = -1;
    struct sockaddr_in server_addr;
    socklen_t addr_size;

    char *buffer = "1234567";

    /*Configure  address struct*/
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(53);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(server_addr.sin_zero, '\0', sizeof server_addr.sin_zero);

    addr_size = sizeof server_addr;

    client_socket = socket(PF_INET, SOCK_DGRAM, 0);
    if ( client_socket < 0 )
    {
        printf("Failed to create udp socket\n");
        return -1;
    }

     /*Send message to server*/
    if ( sendto( client_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, addr_size) < 0 )
    {
        printf("Failed to send message to server\n");
    }

    return 0;
}