#include "decisionMaking.h"
#include "game.h"
#include "wazabi.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void testHash()
{
    int set1[] = {1, 2, 3};
    int set2[] = {2, 1, 3};

    size_t size1 = sizeof(set1) / sizeof(set1[0]);
    size_t size2 = sizeof(set2) / sizeof(set2[0]);

    uint32_t result1 = hash_set_and_int(set1, size1, 3, 3, 3, 3);
    uint32_t result2 = hash_set_and_int(set2, size2, 3, 3, 3, 3);
    printf("Hash: %u\n", result1);
    printf("Hash: %u\n", result2);
}

void printDeck(Player *player)
{
    for (int i = 0; i < DECK_SIZE; i++)
    {
        if (player->deck[i] != NULL)
        {
            printf("\n %s\n", player->deck[i]->name);
            printf("indice : %d\n", i);
            printf("Price : %d\n", player->deck[i]->price);
        }
    }
}

void printproperties(State *state)
{
    for (int i = 0; i < 10; i++)
    {
        printf("passages : %d valeur : %d\n", state->properties[i][0], state->properties[i][0]);
        printf("\n");
    }
}

void testDecisionMaking()
{
    Player *p = NULL, *m = NULL;
    Game *game = NULL;
    initGame(&game, &p, &m);
    switchPlayer(game); // on fait commencer l'IA pour les tests

    rbNode *root = NULL;

    // lancer de dés
    rollDice(game, m);
    int nbwazabi = playTurn(game, m);
    giveDices(game, m, p);
    if (isPlayable(m, nbwazabi))
    {
        clock_t start_time = clock();

        while ((clock() - start_time) < CLOCKS_PER_SEC * 5) // Run the loop for 1 second
        {
            // copy des decks des joueurs, structure game, experience aleatoire sur la mains du joueur humain et la pioche
            // Copie des joueurs
            Player *p_copy = malloc(sizeof(Player));
            Player *m_copy = malloc(sizeof(Player));
            m_copy->idPlayer = 1;
            m_copy->nbOfDice = m->nbOfDice;
            m_copy->nbOfCards = m->nbOfCards;
            p_copy->idPlayer = 0;
            p_copy->nbOfDice = p->nbOfDice;
            p_copy->nbOfCards = p->nbOfCards;
            copyCardsTable(p->deck, p_copy->deck);
            copyCardsTable(m->deck, m_copy->deck);
            // copie de la structure game
            Game *copy_game = malloc(sizeof(Game));
            copy_game->currentPlayer = 1;
            copyCardsTable(game->drawer, copy_game->drawer);
            copyCardsTable(game->discard, copy_game->discard);
            initDice(copy_game->dice);
            initDiceRoll(copy_game->diceRolls);
            // création aléatoire de la main de l'adversaire
            addPCardsToDrawer(copy_game, p_copy);
            shuffleDrawer(copy_game->drawer);
            int sauv_nbcards = p_copy->nbOfCards;
            for (int i = 0; i < sauv_nbcards; i++)
            {
                drawCard(copy_game, p_copy);
            }
            p_copy->nbOfCards -= sauv_nbcards;
            // MCTS
            MCTS(copy_game, p_copy, m_copy, nbwazabi, &root);
        }
    }
    int moveToPlay = choseMove(root);
    printf("Le coup à jouer est : %d\n", moveToPlay);
    printf("nb de wasabi : %d\n", nbwazabi);
    printDeck(m);
    printproperties(root->state);
}

int isTurnPassed = 0;

void freeMemory(Game *game, Player *player1, Player *player2)
{
    // libération de la mémoire (à mettre dans une fonction)
    for (int i = 0; i < DRAWER_SIZE; ++i)
    {
        if (game->drawer[i] != NULL)
        {
            free(game->drawer[i]);
        }
    }
    for (int i = 0; i < DRAWER_SIZE; ++i)
    {
        if (game->discard[i] != NULL)
        {
            free(game->discard[i]);
        }
    }
    for (int i = 0; i < DECK_SIZE; ++i)
    {
        if (player1->deck[i] != NULL)
        {
            free(player1->deck[i]);
        }
    }
    for (int i = 0; i < DECK_SIZE; ++i)
    {
        if (player2->deck[i] != NULL)
        {
            free(player2->deck[i]);
        }
    }
    free(player1);
    free(player2);
    free(game);
}

int main()
{
    testDecisionMaking();
    // Player *p1 = NULL, *p2 = NULL;
    // Game *game = NULL;
    // int nbWazabi = 0;
    // initGame(&game, &p1, &p2);

    // /* ************************************** Initialization ************************************** */
    // srand(time(NULL));

    // // 1.1 Initialize SDL video subsystem
    // if (SDL_Init(SDL_INIT_VIDEO) != 0)
    // {
    //     fprintf(stderr, "Erreur d'initialisation de la SDL: %s\n", SDL_GetError());
    //     return EXIT_FAILURE;
    // }

    // // 1.2 Initialize SDL_ttf for font rendering
    // if (TTF_Init() == -1)
    // {
    //     fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
    //     return EXIT_FAILURE;
    // }

    // SDL_DisplayMode displayMode;
    // // 2.1 Get the current display mode
    // if (SDL_GetCurrentDisplayMode(0, &displayMode) != 0)
    // {
    //     fprintf(stderr, "Erreur lors de l'obtention du mode d'affichage: %s\n", SDL_GetError());
    //     SDL_Quit();
    //     return EXIT_FAILURE;
    // }

    // int screenWidth = displayMode.w;
    // int screenHeight = displayMode.h;

    // // 2.2 Create the window
    // SDL_Window *window = SDL_CreateWindow("Wazabi",
    //                                       SDL_WINDOWPOS_CENTERED,
    //                                       SDL_WINDOWPOS_CENTERED,
    //                                       screenWidth * 0.85, screenHeight * 0.85, SDL_WINDOW_OPENGL);
    // if (window == NULL)
    // {
    //     fprintf(stderr, "Erreur de création de la fenêtre: %s\n", SDL_GetError());
    //     SDL_Quit();
    //     return EXIT_FAILURE;
    // }

    // // 2.3 Create the renderer
    // SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    // if (renderer == NULL)
    // {
    //     fprintf(stderr, "Erreur de création du renderer: %s\n", SDL_GetError());
    //     SDL_DestroyWindow(window);
    //     SDL_Quit();
    //     return EXIT_FAILURE;
    // }

    // // 3.1 Load textures
    // SDL_Texture *bg = loadTexture("img/bg.png", renderer);
    // SDL_Texture *cup_moving = loadTexture("img/cup_moving.png", renderer);
    // SDL_Texture *cup = loadTexture("img/cup.png", renderer);

    // if (!bg || !cup || !cup_moving)
    // {
    //     end_sdl(0, "Echec du chargement des textures", window, renderer);
    // }

    // int i;

    // // 3.2 Load dice textures
    // SDL_Texture *dice_textures[NB_OF_DICES];
    // SDL_Surface *dice_surfaces[NB_OF_DICES];

    // char *result = malloc(100 * sizeof(char));

    // for (i = 0; i < NB_OF_DICES; i++)
    // {
    //     sprintf(result, "img/%d.png", i + 1);
    //     dice_surfaces[i] = IMG_Load(result);
    //     if (!dice_surfaces[i])
    //     {
    //         end_sdl(0, "Echec du chargement de l'image dice", window, renderer);
    //     }
    //     dice_textures[i] = SDL_CreateTextureFromSurface(renderer, dice_surfaces[i]);
    //     if (!dice_textures[i])
    //     {
    //         end_sdl(0, "Echec de la création des textures", window, renderer);
    //     }
    //     SDL_FreeSurface(dice_surfaces[i]);
    // }

    // // 3.3 Load card textures
    // SDL_Texture *card_textures[NB_OF_CARDS];
    // SDL_Surface *card_surfaces[NB_OF_CARDS];
    // for (i = 0; i < NB_OF_CARDS; i++)
    // {
    //     sprintf(result, "img/card%d.png", i);
    //     card_surfaces[i] = IMG_Load(result);
    //     if (!card_surfaces[i])
    //     {
    //         end_sdl(0, "Echec du chargement d'une carte", window, renderer);
    //     }
    //     card_textures[i] = SDL_CreateTextureFromSurface(renderer, card_surfaces[i]);
    //     if (!card_textures[i])
    //     {
    //         end_sdl(0, "Echec de la création des textures", window, renderer);
    //     }
    //     SDL_FreeSurface(card_surfaces[i]);
    // }

    // int running = 1;
    // SDL_Event event;

    // // 4.1 Allocate memory for dice textures
    // SDL_Texture **dice = malloc(NB_OF_DICES * sizeof(SDL_Texture *));
    // if (dice == NULL)
    // {
    //     end_sdl(0, "Erreur malloc dé", window, renderer);
    // }

    // for (i = 0; i < NB_OF_DICES; i++)
    // {
    //     dice[i] = dice_textures[i];
    // }

    // // 4.3 Allocate memory for card textures
    // SDL_Texture **cards = malloc(NB_OF_CARDS * sizeof(SDL_Texture *));
    // if (cards == NULL)
    // {
    //     end_sdl(0, "Erreur malloc carte", window, renderer);
    // }

    // for (i = 0; i < NB_OF_CARDS; i++)
    // {
    //     cards[i] = card_textures[i];
    // }

    // /* ************************************** Interface ************************************** */

    // Player *currentPlayer = NULL, *otherPlayer = NULL;

    // Initialize variables for card selection
    // int selected_card1 = -1;
    // int selected_card2 = -1;
    // int id_card1 = 0; // Start with the first card
    // int id_card2 = 0; // Start with the first card
    // int index = 0;
    // int isPlaying = 0;
    // int idCardDeck1[DECK_SIZE] = {-1};
    // int idCardDeck2[DECK_SIZE] = {-1};
    // int type = -1;
    // Uint32 displayTime = 2000; // 2 secondes
    // Uint32 startTime = SDL_GetTicks();
    // Uint32 currentTime = SDL_GetTicks();

    // while (running && !endGame(p1, p2))
    // {
    //     if (isPlaying == 0)
    //     {
    //         id_card1 = 0;

    //         switchPlayer(game);
    //         if (game->currentPlayer == firstPlayer)
    //         {
    //             currentPlayer = p1;
    //             otherPlayer = p2;
    //         }
    //         else
    //         {
    //             currentPlayer = p2;
    //             otherPlayer = p1;
    //         }

    //         rollDice(game, currentPlayer);
    //         nbWazabi = playTurn(game, currentPlayer);
    //         printf("NbWazabi :%d\n", nbWazabi);
    //         isPlaying = 1;
    //         printDeck(currentPlayer);
    //     }

    //     if (!endGame(p1, p2))
    //     {
    //         while (SDL_PollEvent(&event))
    //         {
    //             switch (event.type)
    //             {
    //             case SDL_QUIT:
    //                 running = 0; // Exit the loop if the user closes the window
    //                 break;

    //             case SDL_KEYDOWN:
    //                 if (event.key.keysym.sym == SDLK_z && selected_card1 == -1)
    //                 {
    //                     selected_card1 = id_card1; // Select the card
    //                 }
    //                 else if (event.key.keysym.sym == SDLK_z && selected_card1 != -1)
    //                 {
    //                     selected_card1 = -1; // Deselect the card
    //                 }
    //                 else if (event.key.keysym.sym == SDLK_q)
    //                 {
    //                     if (id_card1 > 0)
    //                     {
    //                         id_card1--;
    //                         selected_card1 = id_card1; // Select the card
    //                     }
    //                 }
    //                 else if (event.key.keysym.sym == SDLK_d)
    //                 {
    //                     if (id_card1 < currentPlayer->nbOfCards - 1)
    //                     {
    //                         id_card1++;
    //                         selected_card1 = id_card1; // Select the card
    //                     }
    //                 }
    //                 else if (event.key.keysym.sym == SDLK_s)
    //                 {
    //                     index = idCardDeck1[id_card1];

    //                     if (currentPlayer->deck[index] != NULL && currentPlayer->deck[index]->price <= nbWazabi)
    //                     {
    //                         type = currentPlayer->deck[index]->cardType; // variable pour l'affichage
    //                         // donne les dés qui ont la face dé
    //                         giveDices(game, currentPlayer, otherPlayer);
    //                         // pose une carte
    //                         playCard(game, currentPlayer, otherPlayer, index);
    //                         isPlaying = 0;
    //                         selected_card1 = -1;
    //                         selected_card2 = -1;

    //                         startTime = SDL_GetTicks();
    //                     }
    //                 }
    //                 else if (event.key.keysym.sym == SDLK_p)
    //                 {
    //                     // passer son tour
    //                     giveDices(game, currentPlayer, otherPlayer);
    //                     isPlaying = 0;
    //                     selected_card1 = -1;
    //                     selected_card2 = -1;
    //                 }
    //                 if (event.key.keysym.sym == SDLK_i && selected_card2 == -1)
    //                 {
    //                     selected_card2 = id_card2; // Select the card
    //                 }
    //                 else if (event.key.keysym.sym == SDLK_i && selected_card2 != -1)
    //                 {
    //                     selected_card2 = -1; // Deselect the card
    //                 }
    //                 else if (event.key.keysym.sym == SDLK_j)
    //                 {
    //                     if (id_card2 > 0)
    //                     {
    //                         id_card2--;
    //                     }
    //                     selected_card2 = id_card2; // Select the card
    //                 }
    //                 else if (event.key.keysym.sym == SDLK_l)
    //                 {
    //                     if (id_card2 < otherPlayer->nbOfCards - 1)
    //                     {
    //                         id_card2++;
    //                     }
    //                     selected_card2 = id_card2; // Select the card
    //                 }
    //                 break;
    //             }
    //         }
    //     }

    //     if (!isTurnPassed)
    //     {
    //         saveDeckIndex(currentPlayer, idCardDeck1);
    //         saveDeckIndex(otherPlayer, idCardDeck2);

    //         // Render everything
    //         SDL_RenderClear(renderer);        // Clear the renderer
    //         RenderInit(window, renderer, bg); // Render the background
    //         currentTime = SDL_GetTicks();
    //         RenderDice(renderer, dice, currentPlayer, game);                       // Render player 0's dice
    //         RenderDice(renderer, dice, otherPlayer, game);                         // Render player 1's dice
    //         RenderBackCard(renderer, card_textures[0], screenWidth, screenHeight); // Render the back of the card
    //         if (currentTime - startTime >= displayTime || type == -1)
    //         {
    //             RenderDiscard(renderer, card_textures[type + 1], screenWidth, screenHeight);
    //         }
    //         RenderCards(renderer, cards, game, currentPlayer, selected_card1, idCardDeck1); // Render player 0's cards
    //         RenderCards(renderer, cards, game, otherPlayer, selected_card2, idCardDeck2);   // Render player 1's cards
    //         if (currentTime - startTime < displayTime && type != -1)
    //         {
    //             RenderPlayedCard(renderer, card_textures[type + 1], screenWidth, screenHeight);
    //         }
    //         SDL_RenderPresent(renderer);
    //     }
    //     else
    //     {
    //         isTurnPassed = 0;
    //     }
    // }

    // /* ************************************** Cleaning ************************************** */
    // // Cleanup textures
    // for (i = 0; i < NB_OF_DICES; i++)
    // {
    //     SDL_DestroyTexture(dice_textures[i]);
    // }
    // for (i = 0; i < NB_OF_CARDS; i++)
    // {
    //     SDL_DestroyTexture(card_textures[i]);
    // }
    // SDL_DestroyTexture(bg);
    // SDL_DestroyTexture(cup);
    // SDL_DestroyTexture(cup_moving);

    // // End SDL and free resources
    // end_sdl(1, "Fin normale", window, renderer);

    // freeMemory(game, p1, p2);
    return 0;
}