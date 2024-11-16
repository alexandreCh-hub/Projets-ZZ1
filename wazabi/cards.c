#include "game.h"
#include <stdio.h>

// actions des différentes cartes
// supprimer un dé
void deleteDice(Game *game, Player *currentPlayer, Player *otherPlayer)
{
    if (currentPlayer->nbOfDice > 0)
    {
        (currentPlayer->nbOfDice)--;
        (void)game;
        (void)otherPlayer;
    }
}

// échange les dés
void changeDice(Game *game, Player *currentPlayer, Player *otherPlayer)
{
    int temp;
    temp = currentPlayer->nbOfDice;
    currentPlayer->nbOfDice = otherPlayer->nbOfDice;
    otherPlayer->nbOfDice = temp;
    (void)game;
}

// supprime 2 dés
void deleteTwoDice(Game *game, Player *currentPlayer, Player *otherPlayer)
{
    if (currentPlayer->nbOfDice > 1)
    {

        currentPlayer->nbOfDice -= 2;
        (void)game;
        (void)otherPlayer;
    }
}

// donne un dé
void giveADice(Game *game, Player *currentPlayer, Player *otherPlayer)
{
    if (currentPlayer->nbOfDice > 0)
    {
        currentPlayer->nbOfDice--;
        otherPlayer->nbOfDice++;
        (void)game;
    }
}

// prendre une carte à l'autre joueur
void takeCard(Game *game, Player *currentPlayer, Player *otherPlayer)
{
    int i = 0, j = 0;
    while (otherPlayer->deck[i] == NULL && i < DECK_SIZE)
        i++;
    while (currentPlayer->deck[j] != NULL && j < DECK_SIZE)
        j++;
    if (i < DECK_SIZE)
    {
        currentPlayer->deck[j] = otherPlayer->deck[i];
        otherPlayer->deck[i] = NULL;
        currentPlayer->nbOfCards++;
        otherPlayer->nbOfCards--;
    }
    else
    {
        drawCard(game, currentPlayer);
    }
}

// une carte restante pour l'autre joueur
void oneCardLeft(Game *game, Player *currentPlayer, Player *otherPlayer)
{
    int cptCards = 0;
    int k = 0;
    for (int i = 0; i < DECK_SIZE; i++)
    {
        if (otherPlayer->deck[i] != NULL)
        {
            if (cptCards >= 1)
            {
                while (game->discard[k] != NULL && k < DRAWER_SIZE)
                    k++;
                game->discard[k] = otherPlayer->deck[i];
                otherPlayer->deck[i] = NULL;
            }
            else
            {
                cptCards++;
            }
        }
    }
    if (cptCards == 0)
    {
        otherPlayer->nbOfCards = 0;
    }
    else
    {
        otherPlayer->nbOfCards = 1;
    }
    (void)currentPlayer;
}

// piocher 3 cartes
void drawThreeCards(Game *game, Player *currentPlayer, Player *otherPlayer)
{
    int i = 0;
    while (i < 3)
    {
        drawCard(game, currentPlayer);
        i++;
    }
    currentPlayer->nbOfCards += 3;
    (void)otherPlayer;
}

// 2 cartes restantes pour l'autre joueur
void twoCardsLeft(Game *game, Player *currentPlayer, Player *otherPlayer)
{
    int cptCards = 0;
    int k = 0;
    for (int i = 0; i < DECK_SIZE; i++)
    {
        if (otherPlayer->deck[i] != NULL)
        {
            if (cptCards >= 2)
            {
                while (game->discard[k] != NULL && k < DRAWER_SIZE)
                    k++;
                game->discard[k] = otherPlayer->deck[i];
                otherPlayer->deck[i] = NULL;
            }
            else
            {
                cptCards++;
            }
        }
    }
    if (cptCards <= 1)
    {
        otherPlayer->nbOfCards = cptCards;
    }
    else
    {
        otherPlayer->nbOfCards = 2;
    }
    (void)currentPlayer;
}

// l'autre joueur passe son tour
void skipTurn(Game *game, Player *currentPlayer, Player *otherPlayer)
{
    (void)currentPlayer;
    (void)otherPlayer;
    switchPlayer(game);
    isTurnPassed = 1;
}

// rejouer
void playAgain(Game *game, Player *currentPlayer, Player *otherPlayer)
{
    (void)currentPlayer;
    (void)otherPlayer;
    switchPlayer(game);
    isTurnPassed = 1;
}

void (*actionTable[10])(Game *, Player *, Player *) = { // tableau contenant les actions
    deleteDice,
    changeDice,
    deleteTwoDice,
    giveADice,
    takeCard,
    oneCardLeft,
    drawThreeCards,
    twoCardsLeft,
    skipTurn,
    playAgain};