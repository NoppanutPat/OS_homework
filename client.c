// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <sys/types.h>  

#define PORT 5000

int main(int argc, char const *argv[])
{
    for(int i=0;i<10;i++){
        fork();
    }

    int sock = 0, valread;
    struct sockaddr_in serv_addr;

    char *buffer_inp;

    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    // printf("%d\n",i);

    // printf("input : ");
    // scanf("%d",&i);

    // printf("Input : ");
    // scanf("%s", buffer_inp);

    buffer_inp = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";

    char *message = buffer_inp;

    // printf("Sending data.\n");

    send(sock, message, strlen(message), 0);
    // printf("----------------\n");
    // printf("Message sent\n");

    // printf("Waiting for server.\n");
    valread = read(sock, buffer, 1024);
    printf("server : %s\n", buffer);
}
