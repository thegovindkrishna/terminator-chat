#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#define USERNAME_SIZE 20
#define TIMESTAMP_SIZE 10
#define MESSAGE_SIZE 200
#define FORMATTED_MESSAGE_SIZE (USERNAME_SIZE + TIMESTAMP_SIZE + MESSAGE_SIZE + 3)

void formatMessage(char* message, char* username, char* timestamp, char* formatted_message){
    strcpy(formatted_message, "[");
    strcat(formatted_message, timestamp);
    strcat(formatted_message, "] ");
    strcat(formatted_message, username);
    strcat(formatted_message, ": ");
    strcat(formatted_message, message);
}


void getCurrentTimestamp(char *timestamp) {
    time_t rawTime;
    struct tm *localTime;

    // Get the current time
    time(&rawTime);
    localTime = localtime(&rawTime);
    // Format the time into the provided buffer
    strftime(timestamp, TIMESTAMP_SIZE, "%H:%M:%S", localTime);
}

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

    printf("Enter your username: ");
    char username[USERNAME_SIZE];
    fgets(username, USERNAME_SIZE, stdin);
    username[strcspn(username, "\n")] = '\0';

    char message[MESSAGE_SIZE];
    char exit_message[] = " has disconnected from the server.\n";

    while (1) {
        printf("Enter message: ");
        fgets(message, MESSAGE_SIZE, stdin);

        char timestamp[TIMESTAMP_SIZE];
        getCurrentTimestamp(timestamp);

        char formatted_message[FORMATTED_MESSAGE_SIZE];        

        if (strcmp(message, "/exit\n") == 0) {
            // Send the exit message to the server
            formatMessage(exit_message, username, timestamp, formatted_message);
            //formatted_message[strcspn(formatted_message, "\n")] = '\0';
            send(client_socket, formatted_message, strlen(formatted_message), 0);
            printf("Disconnected from the server.\n");
            break;
        }

        // Send the message to the server
        formatMessage(message, username, timestamp, formatted_message);
        //formatted_message[strcspn(formatted_message, "\n")] = '\0';
        send(client_socket, formatted_message, strlen(formatted_message), 0);
    }

    close(client_socket);

    return 0;
}

