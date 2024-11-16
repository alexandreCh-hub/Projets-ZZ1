#include <stdio.h>
#include <stdlib.h>
#include "piece.h"

// fonction uniquement pour les tests
void affichePiece(Piece *p)
{
    printf("Pièce choisit :\n");
    for (int j = 0; j < PIECE_SIZE; j++)
    {
        for (int k = 0; k < PIECE_SIZE; k++)
        {
            printf("%d ", p->shape[j][k]);
        }
        printf("\n");
    }
}

Piece *rotatePiece(Piece *piece)
{
    Piece *rotated = (Piece *)malloc(sizeof(Piece));
    if (rotated == NULL)
    {
        fprintf(stderr, "Error malloc\n");
        exit(1);
    }
    rotated->size = piece->size;
    rotated->id = piece->id;

    // initialiser la pièce avec des 0 ??

    for (int i = 0; i < PIECE_SIZE; i++)
    {
        for (int j = 0; j < PIECE_SIZE; j++)
        {
            rotated->shape[j][PIECE_SIZE - 1 - i] = piece->shape[i][j];
        }
    }

    return rotated;
}