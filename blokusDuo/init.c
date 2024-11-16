#include "board.h"
#include "game.h"
#include "piece.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Board *initBoard()
{
    Board *board = malloc(sizeof(Board));
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            board->grid[i][j] = 0;
        }
    }

    return board;
}

/*void afficheListe(Piece pieces[NB_PIECES])
{
    for (int i = 0; i < NB_PIECES; i++)
    {
        printf("taille : %d \n", pieces[i].size);
        for (int j = 0; j < PIECE_SIZE; j++)
        {
            for (int k = 0; k < PIECE_SIZE; k++)
            {
                printf("%d ", pieces[i].shape[j][k]);
            }
            printf("\n");
        }
    }
}*/

void initPieces(Piece *piecesP1[NB_PIECES], Piece *piecesP2[NB_PIECES], Piece *globalPieces[NB_PIECES])
{
    FILE *listPieces;
    char buffer[50];
    int isSize = 1;
    int i = 0, j = 0;
    int k = 0;

    listPieces = fopen("pieces.txt", "rt");
    if (listPieces == NULL)
    {
        perror("Error open file");
        return exit(1);
    }

    while (fgets(buffer, sizeof(buffer), listPieces) != NULL)
    {
        if (strcmp(buffer, "\n") != 0)
        {
            if (isSize == 1) // caractère qui donne la taille de la pièce
            {
                piecesP1[k] = malloc(sizeof(Piece));
                piecesP2[k] = malloc(sizeof(Piece));
                globalPieces[k] = malloc(sizeof(Piece));
                piecesP1[k]->size = atoi(buffer);
                piecesP2[k]->size = atoi(buffer);
                globalPieces[k]->size = atoi(buffer);
                isSize = 0;
            }
            else
            {
                for (j = 0; j < PIECE_SIZE; j++) // copie de la pièce dans la liste
                {
                    piecesP1[k]->shape[i][j] = (buffer[j]) - '0';
                    piecesP2[k]->shape[i][j] = (buffer[j]) - '0';
                    globalPieces[k]->shape[i][j] = (buffer[j]) - '0';
                }
                i++;
            }
        }
        else
        {
            // passage à la pièce suivante
            piecesP1[k]->id = k;
            piecesP2[k]->id = k;
            globalPieces[k]->id = k;
            k++;
            isSize = 1;
            i = 0;
        }
    }

    fclose(listPieces);
}

Game *initGame()
{
    Game *game = malloc(sizeof(Game));
    game->end = 0;
    game->board = initBoard();
    initPieces(game->pieces[0], game->pieces[1], game->globalPieces);
    game->currentPlayer = firstPlayer;

    return game;
    // game->nbPieces1 = 21;
    // game->nbPieces2 = 21;
    // afficheListe(game->pieces[0]);
    // afficheListe(game->pieces[1]);
}