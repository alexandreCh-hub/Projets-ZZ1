#include "game.h"
#include <stdio.h>

void switchPlayer(Game *game)
{
    game->currentPlayer = (game->currentPlayer == firstPlayer) ? secondPlayer : firstPlayer;
}

int endGame(Game *game) // bool
{
    return game->end != 0;

    // Détecter à partir de la création d'arbres :
    // Joueur ne peux plus placer de pièce (nécessite la fonction qui génère tous les coups)
    // Joueur à placé toutes ses pièces (ajouter nb de pièces dans la structure)
}

// fonction qui vérifie si les voisins sont touchés
int checkNeighbors(Game *game, int row, int col, int *rowOffsets, int *colOffsets)
{
    int neighbourRow, neighbourCol;
    int neighbourTouched = 0;
    int i = 0;

    while (i < 4 && neighbourTouched == 0)
    {
        neighbourRow = row + rowOffsets[i];
        neighbourCol = col + colOffsets[i];

        // Vérification si les voisins sont dans les limites du tableau
        if (neighbourRow >= 0 && neighbourRow < BOARD_SIZE && neighbourCol >= 0 && neighbourCol < BOARD_SIZE)
        {
            // si un voisin est de la même couleur
            if (game->board->grid[neighbourRow][neighbourCol] == game->currentPlayer)
            {
                neighbourTouched = 1;
                // printf("Voisin de même couleur\n");
            }
        }
        i++;
    }

    return neighbourTouched;
}

// vérifie si le placement est valide
int isValidPosition(Game *game, Piece *piece, int row, int col)
{
    if (piece->id == 3 && row == 5 && col == 3)
    {
        // affichePiece(piece);
        // printf("id piece : %d ,  row : %d , col : %d      ", piece->id, row, col);
    }
    // Offset pour check les 4 voisins autour
    int rowOffsetsAround[] = {-1, 1, 0, 0};
    int colOffsetsAround[] = {0, 0, -1, 1};
    // Offset pour check les 4 voisins diagonaux
    int rowOffsetsDiag[] = {-1, -1, 1, 1};
    int colOffsetsDiag[] = {-1, 1, -1, 1};

    int validPosition = 1; // bool
    int cornerTouched = 0; // bool
    int i = -2, j;

    // parcours toutes les cases d'une pièce tant que la position est supposée valide
    while (i < 3 && validPosition == 1)
    {
        j = -2;
        while (j < 3 && validPosition == 1)
        {
            if (piece->shape[i + 2][j + 2] == 1)
            {
                // la pièce n'est ni en dehors du plateau ni sur une autre pièce
                if ((row + i) >= 0 && (row + i) < BOARD_SIZE && (col + j) >= 0 && (col + j) < BOARD_SIZE && game->board->grid[row + i][col + j] == 0) // case valide
                {
                    // vérifie qu'aucune pièce de même couleur est adjacente
                    validPosition = !checkNeighbors(game, row + i, col + j, rowOffsetsAround, colOffsetsAround);
                    if (cornerTouched == 0)
                    {
                        // vérifie si un angle de la même couleur est touché
                        cornerTouched = checkNeighbors(game, row + i, col + j, rowOffsetsDiag, colOffsetsDiag);
                    }
                }
                else
                {
                    // printf("Pièce hors du plateau ou sur une autre pièce\n");
                    validPosition = 0;
                }
            }
            j++;
        }
        i++;
    }

    // Si aucun coin n'est touché, la position n'est pas valide
    if (validPosition == 1 && cornerTouched == 0)
    {
        validPosition = 0;
        // printf("Aucun coin touché \n");
    }

    return validPosition;
}

// met des 1 dans la grille là où la pièce est posée
void placePiece(Game *game, Piece *piece, int indexPiece, int row, int col)
{
    // placement à partir du milieu de la pièce
    for (int i = -2; i < 3; i++)
    {
        for (int j = -2; j < 3; j++)
        {
            if (piece->shape[i + 2][j + 2] == 1)
            {
                game->board->grid[row + i][col + j] = game->currentPlayer; // currentPlayer = 1 ou 2
            }
        }
    }

    // supprimer la pièce de la liste du joueur

    game->pieces[game->currentPlayer - 1][indexPiece] = NULL;
}