#include "cards.c"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define DICE_NB 4

void initDrawer(Game *game)
{
    FILE *listCards;
    char buffer[30];
    int id = 0;
    int state = 0;

    listCards = fopen("cards.txt", "rt");
    if (listCards == NULL)
    {
        perror("Error open file");
        return exit(1);
    }

    while (fgets(buffer, sizeof(buffer), listCards) != NULL)
    {
        if (strcmp(buffer, "\n") != 0)
        {
            switch (state)
            {
            case 0:
                // Command line
                game->drawer[id] = malloc(sizeof(Cards));
                game->drawer[id]->idCard = id;
                game->drawer[id]->name = strdup(buffer);
                state = 1;
                break;
            case 1:
                // First value line
                game->drawer[id]->price = atoi(buffer);
                state = 2;
                break;
            case 2:
                // Second value line
                game->drawer[id]->cardType = atoi(buffer);
                game->drawer[id]->action = actionTable[atoi(buffer)];
                id++;
                state = 0;
                break;
            }
        }
    }

    fclose(listCards);
}

void initDiscard(Game *game)
{
    for (int i = 0; i < DRAWER_SIZE; i++)
    {
        game->discard[i] = NULL;
    }
}

void shuffleDrawer(Cards **drawer)
{
    // mélange de la pioche
    srand(time(NULL));
    for (int i = DRAWER_SIZE - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        Cards *temp = drawer[i];
        drawer[i] = drawer[j];
        drawer[j] = temp;
    }
}

void initDeck(Game *game, Player *player1, Player *player2)
{
    for (int i = 0; i < 3; i++) // 3 cartes au départ
    {
        // cartes déjà alloués dans drawer
        player1->deck[i] = game->drawer[i];
        game->drawer[i] = NULL;
    }
    for (int j = 0; j < 3; j++)
    {
        player2->deck[j] = game->drawer[j + 3];
        game->drawer[j + 3] = NULL;
    }
    for (int k = 3; k < DECK_SIZE; k++)
    {
        player1->deck[k] = NULL;
        player2->deck[k] = NULL;
    }
    player1->nbOfCards = 3;
    player2->nbOfCards = 3;
}

Player *initPlayer(int player)
{
    Player *p = malloc(sizeof(Player));
    p->idPlayer = player;
    p->nbOfDice = DICE_NB;
    // p->deck = malloc(sizeof(Cards *) * DECK_SIZE);
    return p;
}

void initDice(enum faces *dice)
{
    dice[0] = wazabi;
    dice[1] = wazabi;
    dice[2] = wazabi;
    dice[3] = draw;
    dice[4] = draw;
    dice[5] = giveDice;
}

void initDiceRoll(enum faces *dice)
{
    for (int i = 0; i < NB_MAX_DICE; i++)
    {
        dice[i] = nothing;
    }
}

void initGame(Game **game, Player **player1, Player **player2)
{
    *player1 = initPlayer(firstPlayer);
    *player2 = initPlayer(secondPlayer);

    *game = malloc(sizeof(Game));
    (*game)->currentPlayer = firstPlayer;
    initDiceRoll((*game)->diceRolls);
    initDice((*game)->dice);
    initDrawer(*game);
    initDiscard(*game);

    shuffleDrawer((*game)->drawer);
    initDeck(*game, *player1, *player2);
}