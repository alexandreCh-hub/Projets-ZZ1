#include "decisionTree.h"
#include "game.h"
#include "piece.h"

// Fonction Minimax
Node *minimax(Node *node, int isMaximizingPlayer)
{
    if (node->children[0] == NULL)
    {
        return node;
    }
    Node *eval = NULL;
    if (isMaximizingPlayer)
    {
        int maxEval = MOINS_INFINI;
        for (int i = 0; i < NB_CHILDREN_MAX; i++)
        {
            if (node->children[i] != NULL)
            {
                eval = minimax(node->children[i], 0);
                maxEval = (eval->value > maxEval) ? eval->value : maxEval;
            }
        }
        node->value = maxEval;
    }
    else
    {
        int minEval = PLUS_INFINI;
        for (int i = 0; i < NB_CHILDREN_MAX; i++)
        {
            if (node->children[i] != NULL)
            {
                eval = minimax(node->children[i], 1);
                minEval = (eval->value < minEval) ? eval->value : minEval;
            }
        }
        node->value = minEval;
    }
    if (node->father == NULL)
    {
        return eval;
    }
    else
    {
        return node;
    }
}

// Fonction pour créer un nouveau nœud
Node *createNode(int idPiece, int row, int col, int rotation)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->father = NULL;
    ;
    for (int i = 0; i < NB_CHILDREN_MAX; i++)
    {
        node->children[i] = NULL;
    }
    node->value = 0; // Initialiser à 0, sera mis à jour plus tard
    node->idPiece = idPiece;
    node->row = row;
    node->col = col;
    node->rotation = rotation;
    return node;
}

// Fonction qui ajoute un fils à un noeud
void addChild(Node *father, Node *child, int joueur)
{
    int indexChild = 0;
    int indexMin = 0;
    int indexMax = 0;
    while (indexChild < NB_CHILDREN_MAX - 1 && father->children[indexChild] != NULL)
    {
        // printf("a\n");
        indexChild++;
    }
    if (father->children[indexChild] == NULL)
    {
        // printf("b\n");
        father->children[indexChild] = child;
        child->father = father;
    }
    else
    {
        if (joueur == firstPlayer) // le pere est un noeud max
        {
            indexMin = 0;
            for (indexChild = 0; indexChild < NB_CHILDREN_MAX; indexChild++)
            {
                if (father->children[indexChild]->value < father->children[indexMin]->value)
                {
                    indexMin = indexChild;
                }
            }
            if (father->children[indexMin]->value < child->value)
            {
                father->children[indexMin] = child;
                child->father = father;
            }
        }
        else // le pere est un noeud min
        {
            indexMax = 0;
            for (indexChild = 0; indexChild < NB_CHILDREN_MAX; indexChild++)
            {
                if (father->children[indexChild]->value > father->children[indexMax]->value)
                {
                    indexMax = indexChild;
                }
            }
            if (father->children[indexMax]->value > child->value)
            {
                father->children[indexMax] = child;
                child->father = father;
            }
        }
    }
}

Piece *copyPiece(Piece *piece)
{
    Piece *pieceCpy = malloc(sizeof(Piece));
    pieceCpy->size = piece->size;
    pieceCpy->id = piece->id;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            pieceCpy->shape[i][j] = piece->shape[i][j];
        }
    }
    return pieceCpy;
}

// // Fonction qui enleve une piece joué, la rend au joueur concerné
void removePieceFromBoard(Node *node, Game *game)
{
    int id = node->idPiece;
    int row = node->row;
    int col = node->col;
    // copie de la pièce :
    Piece *piece = copyPiece(game->globalPieces[id]);
    for (int r = 0; r < node->rotation; r++)
    {
        piece = rotatePiece(piece);
    }
    for (int i = -2; i < 3; i++)
    {
        for (int j = -2; j < 3; j++)
        {
            if (piece->shape[i + 2][j + 2] == 1)
            {
                game->board->grid[row + i][col + j] = 0;
            }
        }
    }
    // remet la pièce dans la table du joueur
    game->pieces[game->currentPlayer - 1][id] = copyPiece(game->globalPieces[id]);
}

void printBoard(Board *board)
{
    for (int k = -1; k < BOARD_SIZE; k++)
    {
        if (k >= 0 && k < 10)
            printf("%d  ", k);
        else if (k >= 10)
            printf("%d ", k);
        else
            printf("    ");
    }
    printf("\n");
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (i < 10)
            printf("%d - ", i);
        else
            printf("%d- ", i);
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            printf("%d  ", board->grid[i][j]);
        }
        printf("\n");
    }
}

// Fonction pour générer les enfants d'un nœud donné
void generateChildren(Node *root, Game *game, int depth)
{
    if (depth < MAX_DEPTH) // on est pas sur la fin
    {
        Piece *pieceCpy = NULL;
        Node *child = NULL;
        for (int indexPiece = 0; indexPiece < 21; indexPiece++) // pour toutes les pieces du joueur concerné
        {
            for (int row = 0; row < GRID_SIZE; row++)
            { // pour toutes les cases du board
                for (int col = 0; col < GRID_SIZE; col++)
                {
                    if (game->pieces[game->currentPlayer - 1][indexPiece] != NULL) // si la piece est possédée par le joueur
                    {
                        for (int indexRotation = 0; indexRotation < 4; indexRotation++) // pour toutes les rotations
                        {
                            pieceCpy = copyPiece(game->pieces[game->currentPlayer - 1][indexPiece]);
                            for (int rota = 0; rota < indexRotation; rota++)
                            {
                                pieceCpy = rotatePiece(pieceCpy);
                            }
                            if (isValidPosition(game, pieceCpy, row, col))
                            { // Si la piece est posable
                                child = createNode(pieceCpy->id, row, col, indexRotation);
                                placePiece(game, pieceCpy, child->idPiece, child->row, child->col);
                                child->value = evaluateState(game->board);
                                addChild(root, child, game->currentPlayer);
                                removePieceFromBoard(child, game);
                            }
                        }
                    }
                }
            }
        }
        for (int indexChild = 0; indexChild < NB_CHILDREN_MAX; indexChild++)
        {
            if (root->children[indexChild] != NULL)
            {
                pieceCpy = copyPiece(game->pieces[game->currentPlayer - 1][root->children[indexChild]->idPiece]);
                for (int rota = 0; rota < root->children[indexChild]->rotation; rota++)
                {
                    pieceCpy = rotatePiece(pieceCpy);
                }
                placePiece(game, pieceCpy, pieceCpy->id, root->children[indexChild]->row, root->children[indexChild]->col);
                switchPlayer(game); // changement de joueur courrant
                generateChildren(root->children[indexChild], game, depth + 1);
                switchPlayer(game); // changement de joueur courrant
                removePieceFromBoard(root->children[indexChild], game);
            }
        }
    }
}

// Fonction pour compter les case possédés sur la grille par un joueur
int countCase(Board *board, int joueur)
{
    int counter = 0;
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            if (board->grid[i][j] == joueur)
            {
                counter++;
            }
        }
    }
    return counter;
}

// fonction qui compte le nombre de pièces d'un joueur et son nombre d'angles disponible
int countCorners(Board *board, int joueur)
{
    int rowOffsetsDiag[] = {-1, -1, 1, 1};
    int colOffsetsDiag[] = {-1, 1, -1, 1};
    int rowOffsetsAround[] = {-1, 1, 0, 0};
    int colOffsetsAround[] = {0, 0, -1, 1};
    int counter = 0;
    int k = 0;
    int neighbourRow = 0, neighbourCol = 0;
    int neighbourRowDiag = 0, neighbourColDiag = 0;

    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            if (board->grid[i][j] == joueur)
            {
                // counter -= abs((i - center) + (j - center));

                // pièce proche du centre
                if (i >= 6 && j >= 6)
                {
                    counter += 5;
                }
                while (k < 4)
                {
                    neighbourRowDiag = i + rowOffsetsDiag[i];
                    neighbourColDiag = j + colOffsetsDiag[i];
                    neighbourRow = i + rowOffsetsAround[i];
                    neighbourCol = j + colOffsetsAround[i];

                    // Vérification si les voisins sont dans les limites du tableau
                    if (neighbourRowDiag >= 0 && neighbourRowDiag < BOARD_SIZE && neighbourColDiag >= 0 && neighbourColDiag < BOARD_SIZE)
                    {
                        if (neighbourRow >= 0 && neighbourRow < BOARD_SIZE && neighbourCol >= 0 && neighbourCol < BOARD_SIZE)
                        {
                            if (board->grid[neighbourRowDiag][neighbourColDiag] == 0 && board->grid[neighbourRow][neighbourCol] == 0)
                            {
                                // compteur augmente dès qu'un angle de la pièce est libre
                                counter++;
                            }
                        }
                    }
                    k++;
                }
                k = 0;
                // compteur augmente plus pour donner plus de valeur aux grosses pièces
                counter += 2;
            }
        }
    }

    return counter;
}

// Fonction pour évaluer un état de jeu
int evaluateState(Board *board)
{ /*
     int nbCase1 = 0;
     int nbCase2 = 0;
     nbCase1 = countCase(board, 1);
     nbCase2 = countCase(board, 2);
     return nbCase1 - nbCase2;
     */
    int nbCase1 = 0;
    int nbCase2 = 0;
    nbCase1 = countCorners(board, 1);
    nbCase2 = countCorners(board, 2);
    return nbCase1 - nbCase2;
}

/*void testChildGeneration()
{
    Game *game = malloc(sizeof(Game));
    initGame(game);
    Node *root = createNode(NULL, -1, -1, -1);
    generateChildren(root);
    printf("root = ");
    printState(root);
    for (int i = 0; i < 10; i++)
    {
        printf("enfant %d = \n", i);
        printState(root);
    }
}*/