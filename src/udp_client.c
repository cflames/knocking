#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 

#include "aes.h"
#include "dns.h"

int setup_tcp_connection()
{
    int client_socket;

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family      = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port        = htons(443);

    if( (client_socket = socket(PF_INET,SOCK_STREAM,0)) < 0 )
    {
        printf("Failed to create tcp client socket\n");
        return -1;
    }

    if( connect(client_socket, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0 )
    {
        printf("Failed to connect tcp server\n");
        return -1;   
    }

    printf("Setup tcp connection success\n");
    close(client_socket);
    return 0;
}
/*
* Main function for udp socket client
* Just send a message to server
* server address and port are hardcoded
*/
int main(int argc, char *argv[])
{
    int client_socket = -1;
    struct sockaddr_in server_addr;
    socklen_t addr_size;

    char *encrypt_string = NULL;
    char *buffer         = NULL;

    // dns message
    unsigned char dns_buf[256], *qname;
    memset(dns_buf, 0, 256);

    struct DNS_HEADER *dns = NULL;
    struct QUESTION *qinfo = NULL;

    // address settting
    server_addr.sin_family      = AF_INET;
    server_addr.sin_port        = htons(53);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(server_addr.sin_zero, '\0', sizeof server_addr.sin_zero);

    addr_size = sizeof server_addr;

    if ( argc < 2 )
    {
        printf("password must be provided\n");
        return -1;
    }

    buffer = argv[1];

    client_socket = socket(PF_INET, SOCK_DGRAM, 0);
    if ( client_socket < 0 )
    {
        printf("Failed to create udp socket\n");
        return -1;
    }

    // encrypt input string
    int length = aes_encrypt(buffer, &encrypt_string);
    if ( length < 0 )
    {
        printf("Encrypted string is failed\n");
        FREE_STR(encrypt_string)
        return -1;
    }

    //Set the DNS structure to standard queries
    dns = (struct DNS_HEADER *)&dns_buf;
    dns->id         = (unsigned short) htons(10);
    dns->rd         = 1;
    dns->q_count    = htons(1);
 
    //point to the query portion
    qname           = (unsigned char*)&dns_buf[sizeof(struct DNS_HEADER)];
    qname[0]        = length; // first byte is the length
    qname++; // move the pointer to next byte
    memcpy(qname, encrypt_string, length);
    qname[length]   = 0x0; // end of qname

    int dns_message_length = sizeof(struct DNS_HEADER) + (strlen((const char*)qname) + 2);
    qinfo = (struct QUESTION*)&dns_buf[dns_message_length];
    qinfo->qtype = htons(1); //type of the query
    qinfo->qclass = htons(1);

     /*Send message to server*/
    dns_message_length = dns_message_length + sizeof(struct QUESTION);
    if ( sendto( client_socket, dns_buf, dns_message_length, 0, (struct sockaddr *)&server_addr, addr_size) < 0 )
    {
        printf("Failed to send message to server\n");
        FREE_STR(encrypt_string)
        return -1;
    }

    FREE_STR(encrypt_string)

    // better to get message from server to indicate 443 is listening, but for this poc, just sleep 1 second
    sleep(1);
    setup_tcp_connection();

    close(client_socket);

    printf("Client is to close now\n");

    return 0;
}

