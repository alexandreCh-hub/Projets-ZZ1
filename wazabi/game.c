#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "game.h"

// tirage aléatoire des dés
void rollDice(Game *game, Player *player)
{
    srand(time(NULL));
    int k = 0;
    for (int i = 0; i < NB_MAX_DICE; i++)
    {
        game->diceRolls[i] = nothing;
    }

    for (int j = 0; j < player->nbOfDice; j++)
    {
        k = (rand() % 6);
        game->diceRolls[j] = game->dice[k];
    }
}

// pioche une carte
void drawCard(Game *game, Player *currentPlayer)
{
    int k = 0, i = 0;
    while (game->drawer[k] == NULL && k < DRAWER_SIZE)
    {
        k++;
    }
    while (currentPlayer->deck[i] != NULL && i < DECK_SIZE)
    {
        i++;
    }
    if (k < DRAWER_SIZE)
    {
        currentPlayer->deck[i] = game->drawer[k];
        game->drawer[k] = NULL;
        currentPlayer->nbOfCards++;
    }
    else
    {
        printf("Pioche vide\n");
        resetDrawer(game);
        drawCard(game, currentPlayer);
    }
}

int endGame(Player *player1, Player *player2)
{
    return (player1->nbOfDice <= 0 || player2->nbOfDice <= 0);
}

void switchPlayer(Game *game)
{
    game->currentPlayer = (game->currentPlayer == firstPlayer) ? secondPlayer : firstPlayer;
}

// compte le nombre de wazabi et pioche les cartes
int playTurn(Game *game, Player *currentPlayer)
{
    int cptWazabi = 0;
    for (int i = 0; i < NB_MAX_DICE; i++)
    {
        if (game->diceRolls[i] == wazabi)
        {
            cptWazabi++;
        }
        else if (game->diceRolls[i] == draw)
        {
            drawCard(game, currentPlayer);
            printf("Carte piochée\n");
        }
    }
    return cptWazabi;
}

// donne les dés qui ont la face dé
void giveDices(Game *game, Player *currentPlayer, Player *otherPlayer)
{
    for (int i = 0; i < NB_MAX_DICE; i++)
    {
        if (game->diceRolls[i] == giveDice)
        {
            giveADice(game, currentPlayer, otherPlayer);
            printf("Dé donné\n");
        }
    }
}

void playCard(Game *game, Player *currentPlayer, Player *otherPlayer, int index)
{
    // exécute l'action de la carte jouée
    currentPlayer->deck[index]->action(game, currentPlayer, otherPlayer);
    int k = 0;
    while (game->discard[k] != NULL && k < DRAWER_SIZE)
        k++;
    // met la carte dans la défausse
    game->discard[k] = currentPlayer->deck[index];
    currentPlayer->deck[index] = NULL;
    currentPlayer->nbOfCards--;
}

// remet la discard dans la pioche quand celle-ci est vide
void resetDrawer(Game *game)
{
    for (int i = 0; i < DRAWER_SIZE; i++)
    {
        if (game->discard[i] != NULL)
        {
            game->drawer[i] = game->discard[i];
            game->discard[i] = NULL;
        }
    }
    shuffleDrawer(game->drawer);
}