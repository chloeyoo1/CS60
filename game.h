#ifndef GAME_H
#define GAME_H

#define GRID_SIZE 10

typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    int state; // 0 for ongoing, 1 for win, -1 for loss by trap, -2 for loss by Wumpus
    Position player;
    Position wumpus;
    int treasures[GRID_SIZE][GRID_SIZE];
    int traps[GRID_SIZE][GRID_SIZE];
} GameState;

void initialize_game(GameState *game);
void move_player(GameState *game, char direction);
char* get_game_state(GameState *game);
void move_wumpus(GameState *game);

#endif // GAME_H
