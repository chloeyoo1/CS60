#include "game.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void initialize_game(GameState *game) {
    game->state = 0;
    game->player.x = 0;
    game->player.y = 0;
    game->wumpus.x = rand() % GRID_SIZE;
    game->wumpus.y = rand() % GRID_SIZE;

    memset(game->treasures, 0, sizeof(game->treasures));
    memset(game->traps, 0, sizeof(game->traps));
    
    // Initialize random number of treasures and traps in random spots
    for (int i = 0; i < ((rand() % GRID_SIZE) * (2 * GRID_SIZE)); i++) {
        game->treasures[rand() % GRID_SIZE][rand() % GRID_SIZE] = 1;
        game->traps[rand() % GRID_SIZE][rand() % GRID_SIZE] = 1;
    }
}

void move_player(GameState *game, char direction) {
    switch (direction) {
        case 'U': if (game->player.y > 0) game->player.y--; break;
        case 'D': if (game->player.y < GRID_SIZE - 1) game->player.y++; break;
        case 'L': if (game->player.x > 0) game->player.x--; break;
        case 'R': if (game->player.x < GRID_SIZE - 1) game->player.x++; break;
    }
    
    // Check if player landed on a treasure or trap
    if (game->treasures[game->player.x][game->player.y]) {
        game->treasures[game->player.x][game->player.y] = 0;
        printf("Treasure collected!\n");
    }
    if (game->traps[game->player.x][game->player.y]) {
        game->state = -1; 
        printf("Trap triggered!\n");
    }

    // Move the Wumpus
    move_wumpus(game);

    // Check if player is caught by Wumpus
    if (game->wumpus.x == game->player.x && game->wumpus.y == game->player.y) {
        game->state = -2; 
        printf("Caught by the Wumpus!\n");
    }
    
    // Check if all treasures are collected
    int treasures_left = 0;
    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            if (game->treasures[x][y]) {
                treasures_left++;
            }
        }
    }
    if (treasures_left == 0) {
        game->state = 1; // Player wins
    }
}

void move_wumpus(GameState *game) {
    int direction = rand() % 4;
    switch (direction) {
        case 0: if (game->wumpus.y > 0) game->wumpus.y--; break; // up
        case 1: if (game->wumpus.y < GRID_SIZE - 1) game->wumpus.y++; break; // down
        case 2: if (game->wumpus.x > 0) game->wumpus.x--; break; // left
        case 3: if (game->wumpus.x < GRID_SIZE - 1) game->wumpus.x++; break; // right
    }
}

char* get_game_state(GameState *game) {
    static char board[256];
    char *p = board;
    // Format game board
    p += sprintf(p, "Game Board:\n");
    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            if (game->player.x == x && game->player.y == y) {
                p += sprintf(p, "P ");
            } else if (game->wumpus.x == x && game->wumpus.y == y) {
                p += sprintf(p, "W ");
            } else if (game->treasures[x][y]) {
                p += sprintf(p, "+ ");
            } else if (game->traps[x][y]) {
                p += sprintf(p, "- ");
            } else {
                p += sprintf(p, ". ");
            }
        }
        p += sprintf(p, "\n");
    }
    p += sprintf(p, "Player is at (%d, %d)\n", game->player.x, game->player.y);
    if (game->state == 1) {
        p += sprintf(p, "Congratulations! You collected all the treasures!\n");
    } else if (game->state == -1) {
        p += sprintf(p, "Game Over! You triggered a trap!\n");
    } else if (game->state == -2) {
        p += sprintf(p, "Game Over! You were caught by the Wumpus!\n");
    }
    return board;
}