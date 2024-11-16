#ifndef PIECE_H
#define PIECE_H
#define PIECE_SIZE 5
#define NB_PIECES 21

typedef struct piece
{
    int shape[PIECE_SIZE][PIECE_SIZE]; // tableau avec des 1 sur les carrés et des 0 ailleurs
    int size;                          // nombre de carrés sur la pièce (utile pour l'algo min-max)
    int id;
} Piece;

void initPieces(Piece *piecesP1[NB_PIECES], Piece *piecesP2[NB_PIECES], Piece *globalPieces[NB_PIECES]);
Piece *rotatePiece(Piece *piece);
void affichePiece(Piece *p);

#endif