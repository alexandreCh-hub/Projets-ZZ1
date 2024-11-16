#ifndef BOARD_H
#define BOARD_H
#define BOARD_SIZE 14

typedef struct board
{
    int grid[BOARD_SIZE][BOARD_SIZE]; // 0 si case vide, 1 pour joueur 1 et 2 pour joueur 2
} Board;

// int isValidPosition(Board *board, Piece *piece, int x, int y, int player);
// void placePiece(Board *board, Piece *piece, int x, int y, int player);
// void rotatePiece(Piece *piece, int rotation);
Board *initBoard();

#endif