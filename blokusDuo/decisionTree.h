#ifndef ARBRE_H
#define ARBRE_H
#include <stdio.h>
#include <stdlib.h>
#define PLUS_INFINI 100000
#define MOINS_INFINI -100000
#define MAX_DEPTH 2
#define GRID_SIZE 14
#define NB_CHILDREN_MAX 10
#include "game.h"

// Définir la structure pour un nœud de l'arbre de décision
typedef struct Node
{
    struct Node *children[NB_CHILDREN_MAX]; // Tableau de pointeurs vers les enfants
    struct Node *father;                    // Pointeur vers le pere
    int idPiece;                            // id de la piece ajouter par le noeud
    int row;                                // numéro de ligne à laquelle est insérée la pieces
    int col;                                // numéro de colone à laquelle est insérée la pièce
    int value;                              // Valeur de l'évaluation de l'état
    int rotation;
} Node;

void printBoard(Board *board);

void removePieceFromBoard(Node *node, Game *game);

void addChild(Node *father, Node *child, int joueur);

// Fonction Minimax
Node *minimax(Node *node, int isMaximizingPlayer);

// Fonction pour créer un nouveau nœud
Node *createNode(int idPiece, int row, int col, int rotation);

// Fonction pour générer les enfants d'un nœud donné
void generateChildren(Node *root, Game *game, int depth);

// Fonction pour évaluer un état de jeu
int evaluateState(Board *board);

// Fonction pour compter les case possédés sur la grille par un joueur
int countCase(Board *board, int joueur);

// Fonction pour compter les case possédés sur la grille par un joueur
int countCorners(Board *board, int joueur);

// Fonction qui copie une piece
Piece *copyPiece(Piece *piece);

#endif