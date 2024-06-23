#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "game.h"

#define SERVER_IP "193.168.1.81"
#define PORT 8080
#define BUFFER_SIZE 1024

void handle_client(int client_socket);

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        perror("setsockopt failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 3) == -1) {
        perror("Listen failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    while (1) {
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_len);
        if (client_socket == -1) {
            perror("Accept failed");
            continue;
        }
        handle_client(client_socket);
        close(client_socket);
    }

    close(server_socket);
    return 0;
}

void handle_client(int client_socket) {
    GameState game;
    initialize_game(&game);
    
    char buffer[BUFFER_SIZE];

    while (1) {
        // Send game board to client
        char *board = get_game_state(&game);
        send(client_socket, board, strlen(board), 0); 

        // Check game state and break loop if game is over
        if (game.state != 0) break;

        // Receive user command from client
        int read_size = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (read_size <= 0) {
            perror("recv failed or client disconnected");
            break; // Exit loop if there's an error or the client disconnects
        }

        // Process received command (first character of the message)
        char command = toupper(buffer[0]);
        move_player(&game, command); 
    }
}
