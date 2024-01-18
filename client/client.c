#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MESSAGE_SIZE 200

int main() {
    int client_socket;
    struct sockaddr_in server;

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); // Use the correct IP address of the server
    server.sin_port = htons(8888);

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to the server on socket %d\n", client_socket);

    char message[MESSAGE_SIZE];

    while (1) {
        printf("Enter message: ");
        fgets(message, MESSAGE_SIZE, stdin);

        // Send the message to the server
        send(client_socket, message, strlen(message), 0);
    }

    close(client_socket);

    return 0;
}

