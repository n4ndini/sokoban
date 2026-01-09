// Sokoban
// Written by Nandini Patel,
// on 26/12/25

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define ROWS 10
#define COLS 10
#define MAX_BOUND 10

// TODO: You #defines go there

// Every tile on the map has to be one of the following values.
enum base { 
    NONE,
    WALL,
    STORAGE
};

// A single tile of our board.
// box should only contain the value:
// - true (1): there exists a box here
// - false (0): there doesn't exist a box here
struct tile {
    enum base base;
    int box;
};

struct player {
    int row;
    int col;
    int moves;
};


// Function Prototypes
void init_board(struct tile board[ROWS][COLS]);
void print_line(void);
void print_title(void);
void print_board(struct tile board[ROWS][COLS], int player_row, int player_col);

// Stage 1 Function Prototypes
void level_setup(struct tile board[ROWS][COLS]);
void place_wall(struct tile board[ROWS][COLS], int row, int col);
void place_storage(struct tile board[ROWS][COLS], int row, int col);
void place_box(struct tile board[ROWS][COLS], int row, int col);
void place_wall_line(struct tile board[ROWS][COLS], int startRow, int startCol, 
                int endRow, int endCol);
bool inbounds(int coord);

// Stage 2 Function Prototypes
void player_setup(struct tile board[ROWS][COLS], struct player *player);
void update_player_location(struct player *player, int row, int col);
void game_loop(struct tile board[ROWS][COLS], struct player *player);
bool check_wall(struct tile board[ROWS][COLS], int row, int col);
void move_up(struct tile board[ROWS][COLS], struct player *player);
void move_down(struct tile board[ROWS][COLS], struct player *player);
void move_left(struct tile board[ROWS][COLS], struct player *player);
void move_right(struct tile board[ROWS][COLS], struct player *player);
int wrap_coord(int coord);

// Stage 3 Prototypes
bool check_box(struct tile board[ROWS][COLS], int row, int col);
void move_box_up(struct tile board[ROWS][COLS], struct player *player, int row, int col);
void move_box_down(struct tile board[ROWS][COLS], struct player *player, int row, int col);
void move_box_left(struct tile board[ROWS][COLS], struct player *player, int row, int col);
void move_box_right(struct tile board[ROWS][COLS], struct player *player, int row, int col);


int main(void) {
    struct tile board[ROWS][COLS];
    init_board(board);

    printf("=== Level Setup ===\n");
    level_setup(board);

    struct player player;
    struct player *player_pointer = &player;
    player.moves = -1;
    player_setup(board, player_pointer);

    game_loop(board, player_pointer);

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////// YOUR FUNCTIONS //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Stage 1 Functions
void level_setup(struct tile board[ROWS][COLS]) {
    char command;
    int row = 0;
    int col = 0;
    while (scanf(" %c", &command) == 1) {
        if (command == 'q') {
            return;
        }

        scanf("%d %d", &row, &col);

        if (command == 'w') {
            place_wall(board, row, col);
        }
        if (command == 's') {
            place_storage(board, row, col);
        }
        if (command == 'b') {
            place_box(board, row, col);
        }
        if (command == 'W') {
            int endRow;
            int endCol;
            scanf("%d %d", &endRow, &endCol);
            place_wall_line(board, row, col, endRow, endCol);
        }

        print_board(board, -1, -1);
    }
}

void place_wall(struct tile board[ROWS][COLS], int row, int col) {
    if (inbounds(row) && inbounds(col)) {
        if (board[row][col].box == 1) {
            board[row][col].box = 0;
            board[row][col].base = WALL;
        } else {
            board[row][col].base = NONE;
            board[row][col].base = WALL;
        }
    } else {
        printf("Location out of bounds\n");
    }
}

void place_storage(struct tile board[ROWS][COLS], int row, int col) {
    if (inbounds(row) && inbounds(col)) {
        if (board[row][col].base == WALL) {
            board[row][col].base = NONE;
            board[row][col].box = 0;
            board[row][col].base = STORAGE;
        } else  {
            board[row][col].base = STORAGE;
        }
    } else {
        printf("Location out of bounds\n");
    }
}

void place_box(struct tile board[ROWS][COLS], int row, int col) {
    if (inbounds(row) && inbounds(col)) {
        if (board[row][col].base == WALL) {
            board[row][col].base = NONE;
            board[row][col].box = 1;
        } 
        if ((board[row][col].base == STORAGE) || (board[row][col].base == NONE)) {
            board[row][col].box = 1;
        }
    } else {
        printf("Location out of bounds\n");
    }
}

void place_wall_line(struct tile board[ROWS][COLS], int startRow, int startCol, int endRow, int endCol) {
    if ((!inbounds(startRow) || !inbounds(startCol)) && (!inbounds(endRow) && !inbounds(endCol))) {
        printf("Location out of bounds\n");
        return;
    }

    if (startRow < 0) { startRow = 0; }
    if (startCol < 0) { startCol = 0; } 
    if (endRow >= ROWS) { endRow = ROWS - 1; }
    if (endCol >= COLS) { endCol = COLS - 1; }

    int count = 0;

    if (startRow == endRow) {
        count = startCol;
        while (count <= endCol) {
            place_wall(board, startRow, count);
            count += 1;
        }
    }
    if (startCol == endCol) {
        count = startRow;
        while (count <= endRow) {
            place_wall(board, count, startCol);
            count += 1;
        }
    }
}

bool inbounds(int coord) {
    if ((coord < 0) || (coord >= MAX_BOUND)) {
        return false;
    }
    return true;
}


// Stage 2 Functions
void player_setup(struct tile board[ROWS][COLS], struct player *player) {
    int row, col;

    printf("Enter player starting position: ");
    while (scanf("%d %d", &row, &col) == 2) {
        if (!inbounds(row) || !inbounds(col) ||
            board[row][col].base == WALL ||
            board[row][col].box == 1) {

            printf("Position (%d, %d) is invalid\n", row, col);
            printf("Enter player starting position: ");
            continue;
        }

        printf("\n=== Starting Sokoban! ===\n");
        print_board(board, row, col);
        update_player_location(player, row, col);
        break;
    }
}

void update_player_location(struct player *player, int row, int col) {
    player->row = row;
    player->col = col;
    player->moves++;
}

void game_loop(struct tile board[ROWS][COLS], struct player *player) {
    char move;
    while (scanf(" %c", &move) == 1) {
        if (move == 'w') {
            move_up(board, player);
        }else if (move == 'a') {
            move_left(board, player);
        } else if (move == 's') {
            move_down(board, player);
        } else if (move == 'd') {
            move_right(board, player);
        } else if (move == 'c') {
            printf("Number of moves so far: %d\n", player->moves);
        }
        print_board(board, player->row, player->col);
    }

}

void move_up(struct tile board[ROWS][COLS], struct player *player) {
    int row = player->row - 1;
    int col = player->col;
    if (!inbounds(row)) {
        row = wrap_coord(row);
    }
    
    if (!inbounds(col)) {
        col = wrap_coord(col);
    }
    
    if (check_wall(board, row, col)) {
        return;
    }

    if (check_box(board, row, col)) {
        move_box_up(board, player, row, col);
        return;
    } else {
        update_player_location(player, row, col); 
    }    
}

void move_down(struct tile board[ROWS][COLS], struct player *player) {
    int row = player->row + 1;
    int col = player->col;
    if (!inbounds(row)) {
        row = wrap_coord(row);
    }
    
    if (!inbounds(col)) {
        col = wrap_coord(col);
    }
    
    if (check_wall(board, row, col)) {
        return;
    }

    if (check_box(board, row, col)) {
        move_box_down(board, player, row, col);
        return;
    } else {
        update_player_location(player, row, col); 
    }   
}

void move_left(struct tile board[ROWS][COLS], struct player *player) {
    int col = player->col - 1;
    int row = player->row;

    if (!inbounds(row)) {
        row = wrap_coord(row);
    }
    
    if (!inbounds(col)) {
        col = wrap_coord(col);
    }
    
    if (check_wall(board, row, col)) {
        return;
    }

    if (check_box(board, row, col)) {
        move_box_left(board, player, row, col);
        return;
    } else {
        update_player_location(player, row, col); 
    } 

}

void move_right(struct tile board[ROWS][COLS], struct player *player) {
    int col = player->col + 1;
    int row = player->row;

    if (!inbounds(row)) {
        row = wrap_coord(row);
    }
    
    if (!inbounds(col)) {
        col = wrap_coord(col);
    }
    
    if (check_wall(board, row, col)) {
        return;
    }

    if (check_box(board, row, col)) {
        move_box_right(board, player, row, col);
        return;
    } else {
        update_player_location(player, row, col); 
    } 
}

bool check_wall(struct tile board[ROWS][COLS], int row, int col) {
    if (board[row][col].base == WALL) {
        return true;
    }

    return false;
}

int wrap_coord(int coord) {
    if (coord < 0) {
        return MAX_BOUND - 1;
    } else {
        return 0;
    }
}


// Stage 3 Functions
bool check_box(struct tile board[ROWS][COLS], int row, int col) {
    if (board[row][col].box == 1) {
        return true;
    }

    return false;
}

void move_box_up(struct tile board[ROWS][COLS], struct player *player, int row, int col) {
    int boxRow = row;
    int boxCol = col;
    if (!inbounds(row - 1)) {
        boxRow = wrap_coord(row - 1);
    } else {
        boxRow = row - 1;
    }

    if (!inbounds(col)) {
        boxCol = wrap_coord(col);
    }

    if (check_wall(board, boxRow, boxCol)) {
        return;
    }

    board[row][col].box = 0;
    board[boxRow][boxCol].box = 1;

    update_player_location(player, row, col);
}

void move_box_down(struct tile board[ROWS][COLS], struct player *player, int row, int col) {
    int boxRow = row;
    int boxCol = col;
    if (!inbounds(row + 1)) {
        boxRow = wrap_coord(row + 1);
    } else {
        boxRow = row + 1;
    }

    if (!inbounds(col)) {
        boxCol = wrap_coord(col);
    }

    if (check_wall(board, boxRow, boxCol)) {
        return;
    }

    board[row][col].box = 0;
    board[boxRow][boxCol].box = 1;

    update_player_location(player, row, col);
}

void move_box_left(struct tile board[ROWS][COLS], struct player *player, int row, int col) {
    int boxRow = row;
    int boxCol = col - 1;
    if (!inbounds(row)) {
        boxRow = wrap_coord(row);
    } 

    if (!inbounds(boxCol)) {
        boxCol = wrap_coord(boxCol);
    }

    if (check_wall(board, boxRow, boxCol)) {
        return;
    }

    board[row][col].box = 0;
    board[boxRow][boxCol].box = 1;

    update_player_location(player, row, col);
}

void move_box_right(struct tile board[ROWS][COLS], struct player *player, int row, int col) {
    int boxRow = row;
    int boxCol = col + 1;
    if (!inbounds(row)) {
        boxRow = wrap_coord(row);
    } 

    if (!inbounds(boxCol)) {
        boxCol = wrap_coord(boxCol);
    }

    if (check_wall(board, boxRow, boxCol)) {
        return;
    }

    board[row][col].box = 0;
    board[boxRow][boxCol].box = 1;

    update_player_location(player, row, col);
}




////////////////////////////////////////////////////////////////////////////////
////////////////////////////// PROVIDED FUNCTIONS //////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// initialises the board to default values.
void init_board(struct tile board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            board[i][j].base = NONE;
            board[i][j].box = 0;
        }
    }
}

// Helper function for print_board().
// Prints a line the width of the sokoban board.
void print_line(void) {
    for (int i = 0; i < COLS * 4 + 1; i++) {
        printf("-");
    }
    printf("\n");
}

// Helper function for print_board().
// Prints out the title for above the sokoban board.
void print_title(void) {
    print_line();
    char *title = "S O K O B A N";
    int len = COLS * 4 + 1;
    int n_white = len - strlen(title) - 2;
    printf("|");
    for (int i = 0; i < n_white / 2; i++) {
        printf(" ");
    }
    printf("%s", title);
    for (int i = 0; i < (n_white + 1) / 2; i++) {
        printf(" ");
    }
    printf("|\n");
}

// Prints out the current state of the sokoban board.
// It will place the player on the board at position player_row, player_col.
// If player position is out of bounds, it won't place a player anywhere.
void print_board(
    struct tile board[ROWS][COLS], int player_row, int player_col) {
    print_title();
    for (int i = 0; i < ROWS; i++) {
        print_line();
        for (int j = 0; j < COLS; j++) {
            printf("|");

            struct tile curr = board[i][j];
            if (i == player_row && j == player_col) {
                printf("^_^");
            } else if (curr.base == WALL) {
                printf("===");
            } else if (curr.box && curr.base == STORAGE) {
                printf("[o]");
            } else if (curr.box) {
                printf("[ ]");
            } else if (curr.base == STORAGE) {
                printf(" o ");
            } else {
                printf("   ");
            }
        }
        printf("|\n");
    }
    print_line();
    printf("\n");
}
