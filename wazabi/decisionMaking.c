#include "decisionMaking.h"
#include <math.h>
#include <time.h>
#define C sqrt(2)

int CostForType[10] = {1, 2, 3, 4, 1, 1, 1, 2, 0, 0};

// Fonction de hash de base pour un entier
uint32_t hash_int(int value)
{
    // Utilisation d'une fonction de hachage simple, par exemple la fonction Jenkins One-at-a-time hash
    uint32_t hash = value;
    hash += (hash << 10);
    hash ^= (hash >> 6);
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

// Fonction de hash pour un ensemble et cinq entiers en préservant leur ordre
uint32_t hash_set_and_int(int *mCardsId, int nbCardsM, int nbDiceM, int nbDiceP, int nbCardsP, int nbWazabi)
{
    uint32_t hash = 0;
    for (int i = 0; i < nbCardsM; ++i)
    {
        hash ^= hash_int(mCardsId[i]);
    }
    // Mélanger les entiers en préservant l'ordre
    hash ^= hash_int(nbDiceM);
    hash = (hash << 1) | (hash >> 31);
    hash ^= hash_int(nbDiceP);
    hash = (hash << 1) | (hash >> 31);
    hash ^= hash_int(nbCardsP);
    hash = (hash << 1) | (hash >> 31);
    hash ^= hash_int(nbCardsM);
    hash = (hash << 1) | (hash >> 31);
    hash ^= hash_int(nbWazabi);
    return hash;
}

State *createState(Cards **deck, int nbDiceCurrent, int nbDiceOther, int nbCardsCurrent, int nbCardsOther, int nbWazabi)
{
    int taille = 0;
    int i = 0, k = 0;
    State *state = malloc(sizeof(State));
    // récupération de l'info sur les cartes du joueurs M
    for (i = 0; i < 20; i++)
    {
        if (deck[i] != NULL)
        {
            taille += 1;
        }
    }
    state->CardsTypes = malloc(sizeof(int) * taille);
    k = 0;
    for (i = 0; i < 20; i++)
    {
        if (deck[i] != NULL)
        {
            state->CardsTypes[k] = deck[i]->cardType;
            k++;
        }
    }
    state->nbDiceCurrent = nbDiceCurrent;
    state->nbDiceOther = nbDiceOther;
    state->nbCardsOther = nbCardsOther;
    state->nbCardsCurrent = nbCardsCurrent;
    state->nbWazabi = nbWazabi;
    for (i = 0; i < 10; i++)
    {
        state->properties[i][0] = 0;
    }
    state->hash = hash_set_and_int(state->CardsTypes, state->nbDiceCurrent, state->nbDiceOther, state->nbCardsOther, state->nbCardsCurrent, state->nbWazabi);
    return state;
}

void copyCardsTable(Cards *source[DECK_SIZE], Cards *destination[DECK_SIZE])
{
    for (int i = 0; i < DECK_SIZE; i++)
    {
        destination[i] = source[i];
    }
}

void addPCardsToDrawer(Game *game, Player *player)
{
    int indexDrawer = 0;
    for (int indexDeck = 0; indexDeck < DECK_SIZE; indexDeck++)
    {
        if (player->deck[indexDeck] != NULL)
        {
            while (game->drawer[indexDrawer] != NULL)
            {
                indexDrawer++;
            }
            game->drawer[indexDrawer] = player->deck[indexDeck];
            player->deck[indexDeck] = NULL;
        }
    }
}

int UCB(State *state, int currentPlayer)
{
    int n = 0; // nombre de passages par les fils
    int i = 0;
    int isFirstVisited = 1; // = 1 si on a pas encore visité un coup faisable dans le tableau associé
    int maxType = -1;
    int maxInterest = 0;
    int interest = 0;
    for (i = 0; i < 10; i++)
    {
        n += state->properties[i][0];
    }
    for (i = 0; i < state->nbCardsCurrent; i++)
    {
        if (state->properties[state->CardsTypes[i]][0] > 0)
        {
            interest = state->properties[state->CardsTypes[i]][1] - C * (sqrt(log(n) / (state->properties[state->CardsTypes[i]][0])));
            if ((currentPlayer == firstPlayer) && (interest < maxInterest || isFirstVisited))
            {
                maxType = state->CardsTypes[i];
                maxInterest = interest;
                isFirstVisited = 0;
            }
            if ((currentPlayer == secondPlayer) && (interest > maxInterest || isFirstVisited))
            {
                maxType = i;
                maxInterest = interest;
                isFirstVisited = 0;
            }
        }
    }
    return maxType;
}

int Simulation(Game *game, Player *p, Player *m, int nbWazabi) // simulation de partie avec coups aléatoire
{
    Player *current = game->currentPlayer == 1 ? m : p;
    Player *other = game->currentPlayer == 0 ? m : p;
    if (current->nbOfDice == 0 || other->nbOfDice == 0) // Condition d'arret dans le cas ou le joueur actuel n'a plus de dés
    {
        return p->nbOfDice - m->nbOfDice;
    }
    // génération d'un coup aléatoire (on prend la premiere carte jouable)
    if (isPlayable(current, nbWazabi))
    {
        int i = 0;
        shuffleDrawer(current->deck);
        while (current->deck[i] == NULL || current->deck[i]->price > nbWazabi)
        {
            i++;
        }
        playCard(game, current, other, i);
        if (current->nbOfDice == 0) // Condition d'arret dans le cas ou le joueur actuel n'a plus de dés
        {
            return p->nbOfDice - m->nbOfDice;
        }
    }
    // Le joueur suivant lance les dés afin de savoir cb de dés donner à son adversaire, cb de cartes à piocher
    // et connaitre son nombre de wasabi. Puis rend les dés à l'adversaire
    switchPlayer(game);
    current = game->currentPlayer == 1 ? m : p;
    other = game->currentPlayer == 0 ? m : p;
    rollDice(game, current);
    nbWazabi = playTurn(game, p);
    giveDices(game, current, other);
    return Simulation(game, p, m, nbWazabi);
}

int MCTS(Game *game, Player *p, Player *m, int nbWazabi, rbNode **root)
{
    Player *current = game->currentPlayer == 1 ? m : p;
    Player *other = game->currentPlayer == 0 ? m : p;
    if (current->nbOfDice == 0) // Condition d'arret dans le cas ou le joueur actuel n'a plus de dés
    {
        return p->nbOfDice - m->nbOfDice;
    }
    // Récupération / Création de l'état concerné
    State *new_state = createState(current->deck, current->nbOfDice, other->nbOfDice, other->nbOfCards, current->nbOfCards, nbWazabi);
    State *state = rbtSearch(*root, new_state->hash);
    int finalValue = 0;
    int moveToPlay = -1;
    int is_playable = isPlayable(current, nbWazabi);
    if (state == NULL)
    {
        *root = rbtInsert(*root, new_state);
        int i = 0;
        while (current->deck[i] == NULL && current->deck[i]->price > nbWazabi && i < DECK_SIZE) // recherche le coup pour lequel on va incrementer le nombre de passage et la valeur moyenne des gains dans le tableau properties du nouveau noeud
        {
            i++;
        }
        moveToPlay = current->deck[i]->cardType;
        finalValue = Simulation(game, p, m, nbWazabi);
        if (is_playable)
        {
            (*root)->state->properties[moveToPlay][0] += 1;
            (*root)->state->properties[moveToPlay][1] += finalValue;
        }
        return finalValue;
    }
    else
    {
        free(new_state);
    }
    new_state = NULL;
    // Verification qu'un coup peut etre jouer, sinon le joueur ne joue pas
    moveToPlay = -1; // 10 si aucun coup n'est jouable
    if (is_playable)
    {
        // Vérification que tout les coups possible ont déjà été joués
        int i = 0;
        while (moveToPlay == -1 && i < current->nbOfCards && CostForType[state->CardsTypes[i]] <= nbWazabi)
        {
            if (state->properties[state->CardsTypes[i]][0] == 0)
            {
                moveToPlay = state->CardsTypes[i];
            }
            i++;
        }
        if (moveToPlay == -1)
        {
            moveToPlay = UCB(state, game->currentPlayer);
        }
        // on copie l'etat et on joue une carte dessus
        i = 0;
        while (current->deck[i] == NULL || current->deck[i]->cardType != moveToPlay)
        {
            i++;
        }
        playCard(game, current, other, i);
        if (current->nbOfDice == 0) // On verifie que c'est la fin de la partie
        {
            return p->nbOfDice - m->nbOfDice;
        }
    }
    // Le joueur suivant lance les dés afin de savoir cb de dés donner à son adversaire, cb de cartes à piocher
    // et connaitre son nombre de wasabi. Puis rend les dés à l'adversaire
    switchPlayer(game);
    rollDice(game, other);
    nbWazabi = playTurn(game, p);
    giveDices(game, other, current);
    // modification dans l'état et récursion sur les états suivants d'apres le coup donné par moveToPlay
    finalValue = MCTS(game, p, m, nbWazabi, root);
    if (is_playable)
    {
        state->properties[moveToPlay][0] += 1;
        state->properties[moveToPlay][1] += finalValue;
    }
    return finalValue;
}

int choseMove(rbNode *root)
{
    int moveToPlay = -1;
    float maxValue = -100000;
    float currentValue = 0;
    if (root != NULL)
    {
        for (int i = 0; i < root->state->nbCardsCurrent; i++)
        {
            if (root->state->properties[root->state->CardsTypes[i]][0] != 0)
            {
                currentValue = (float)root->state->properties[root->state->CardsTypes[i]][1] / (float)root->state->properties[root->state->CardsTypes[i]][0];
                if (currentValue > maxValue)
                {
                    moveToPlay = root->state->CardsTypes[i];
                    maxValue = currentValue;
                }
            }
        }
    }
    return moveToPlay;
}

int isPlayable(Player *player, int nbWazabi)
{
    int isPlayable = 0;
    int i = 0;
    while (!isPlayable && i < DECK_SIZE)
    {
        if (player->deck[i] != NULL && player->deck[i]->price <= nbWazabi)
        {
            isPlayable = 1;
        }
        i++;
    }
    return isPlayable;
}