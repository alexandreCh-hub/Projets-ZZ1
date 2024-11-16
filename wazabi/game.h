#ifndef GAME_H
#define GAME_H
#define DRAWER_SIZE 36
#define MAX_DEPTH 5
#define DECK_SIZE 36
#define NB_MAX_DICE 8
#define NB_FACES 6
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct State
{
    int hash;
    int *CardsTypes;
    int nbDiceCurrent;
    int nbDiceOther;
    int nbCardsCurrent;
    int nbCardsOther;
    int nbWazabi;
    int properties[10][2]; // stock le nombre de passage par le noeud et score cumulé
} State;

enum idPlayer
{
    firstPlayer,
    secondPlayer
};

enum faces
{
    wazabi,
    giveDice,
    draw,
    nothing = -1
};

typedef void (*CardFunction)(int); // à vérifier (changer les paramètres)
typedef struct game Game;
typedef struct player Player;

typedef struct cards
{
    char *name;
    int idCard;                                                             // un id par carte
    int cardType;                                                           // un id par type de carte
    void (*action)(Game *game, Player *currentPlayer, Player *otherPlayer); // Pointeur de fonction
    int price;
} Cards;

typedef struct game
{
    Cards *drawer[DRAWER_SIZE];
    Cards *discard[DRAWER_SIZE];
    int currentPlayer;
    enum faces dice[NB_FACES];
    enum faces diceRolls[NB_MAX_DICE];
} Game;

typedef struct player
{
    int idPlayer; // 0 ou 1
    int nbOfDice;
    Cards *deck[DECK_SIZE];
    int nbOfCards;
} Player;

extern int isTurnPassed;

void initGame(Game **game, Player **player1, Player **player2);
void initDice(enum faces *dice);
void initDiceRoll(enum faces *dice);
void rollDice(Game *game, Player *player);
void drawCard(Game *game, Player *currentPlayer);
int endGame(Player *player1, Player *player2);
void switchPlayer(Game *game);
void giveADice(Game *game, Player *currentPlayer, Player *otherPlayer);
int playTurn(Game *game, Player *currentPlayer);
void giveDices(Game *game, Player *currentPlayer, Player *otherPlayer);
void shuffleDrawer(Cards **drawer);
void resetDrawer(Game *game);
void playCard(Game *game, Player *currentPlayer, Player *otherPlayer, int index);

#endif