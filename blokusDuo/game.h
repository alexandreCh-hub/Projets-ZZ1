#ifndef GAME_H
#define GAME_H
#include "board.h"
#include "piece.h"
#include "piece.h"

enum player
{
    empty,
    firstPlayer,
    secondPlayer
};

typedef struct game
{
    Piece *pieces[2][NB_PIECES]; // Pièces des joueurs 1 et 2
    int currentPlayer;           // joueur 1 ou 2
    Board *board;
    Piece *globalPieces[NB_PIECES];
    int end;
} Game;

void switchPlayer(Game *game);
int endGame(Game *game);
int isValidPosition(Game *game, Piece *piece, int row, int col);
int checkNeighbors(Game *game, int row, int col, int *rowOffsets, int *colOffsets);
void placePiece(Game *game, Piece *piece, int indexPiece, int row, int col);
Game *initGame();

#endif