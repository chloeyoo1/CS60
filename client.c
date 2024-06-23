#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "193.168.1.81"
#define PORT 8080
#define BUFFER_SIZE 1024


int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(PORT);

    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection to server failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server\n");
    printf("Welcome to the Evade the Wumpus!\nRules:\n1. Move using U (up), D (down), L (left), R (right)\n2. Avoid traps (-) and the moving Wumpus (W)\n3. Collect treasure (+). The game is won when all the treasure has been collected\n");

    while (1) {
        // Receive game state message from server
        memset(buffer, 0, BUFFER_SIZE);
        int read_size = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (read_size <= 0) {
            perror("recv failed or server disconnected");
            break;
        }
        printf("%s\n", buffer);

        // Check game state message for end game condition
        if (strstr(buffer, "Congratulations!") || strstr(buffer, "Game Over!")) break;

        // Prompt and get next command
        printf("Enter command (U/D/L/R): ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        printf("Sending command to server: %s\n", buffer);

        // Send user command to the server
        send(client_socket, buffer, strlen(buffer), 0);
    }

    close(client_socket);
    return 0;
}
