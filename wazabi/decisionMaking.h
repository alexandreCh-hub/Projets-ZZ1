#include "game.h"
#include "rbTree.h"

uint32_t hash_int(int value);
uint32_t hash_set_and_int(int *mCardsId, int nbDiceM, int nbDiceP, int nbCardsP, int nbCardsM, int nbWazabi); // Fonction de hash pour un ensemble et un entier
State *createState(Cards **deck, int nbDiceCurrent, int nbDiceOther, int nbCardsCurrent, int nbCardsOther, int nbWazabi);
void copyCardsTable(Cards *source[DECK_SIZE], Cards *destination[DECK_SIZE]);
void addPCardsToDrawer(Game *game, Player *player);
int MCTS(Game *game, Player *p, Player *m, int nbWazabi, rbNode **root);
int isPlayable(Player *player, int nbWazabi);
int choseMove(rbNode *root);