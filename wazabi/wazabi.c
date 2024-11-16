#include <stdio.h>
#include <string.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "wazabi.h"
#include "game.h"

void end_sdl(char ok, const char *msg, SDL_Window *window, SDL_Renderer *renderer)
{
    char msg_formated[255];
    int l;

    if (!ok)
    {
        strncpy(msg_formated, msg, 250);
        l = strlen(msg_formated);
        strcpy(msg_formated + l, " : %s\n");
        SDL_Log(msg_formated, SDL_GetError());
    }

    if (renderer != NULL)
    {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }
    if (window != NULL)
    {
        SDL_DestroyWindow(window);
        window = NULL;
    }

    SDL_Quit();

    if (!ok)
    {
        exit(EXIT_FAILURE);
    }
}

SDL_Texture *loadTexture(const char *file, SDL_Renderer *renderer)
{
    SDL_Texture *texture = NULL;
    SDL_Surface *loadedImage = IMG_Load(file);

    if (loadedImage != NULL)
    {
        texture = SDL_CreateTextureFromSurface(renderer, loadedImage);
        SDL_FreeSurface(loadedImage);
    }
    else
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", file, IMG_GetError());
    }

    return texture;
}

void RenderInit(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *my_texture)
{
    SDL_Rect source = {0}, window_dimensions = {0}, destination = {0};

    SDL_GetWindowSize(window, &window_dimensions.w, &window_dimensions.h);
    SDL_QueryTexture(my_texture, NULL, NULL, &source.w, &source.h);

    destination = window_dimensions;

    SDL_RenderCopy(renderer, my_texture, &source, &destination); // add the background to the window
}

void RenderDice(SDL_Renderer *renderer, SDL_Texture **dice, Player *player, Game *game)
{
    int i;
    int renderer_w, renderer_h; // Variables to store renderer width and height
    int position = 0;           // Initial position for placing dice
    int nb_dices;               // Number of dice to render
    float total_width;          // Total width needed to render all dice
    float start_x;              // Starting x position for rendering dice

    nb_dices = player->nbOfDice;

    // Get the output size of the renderer
    SDL_GetRendererOutputSize(renderer, &renderer_w, &renderer_h);

    SDL_Rect source = {0};      // Source rectangle for the texture
    SDL_Rect destination = {0}; // Destination rectangle for rendering the texture

    // Loop through each dice
    for (i = 0; i < nb_dices; i++)
    {

        // Query the texture to get its width and height
        SDL_QueryTexture(dice[0], NULL, NULL, &source.w, &source.h);

        // Calculate the total width required to render all dice with spacing
        total_width = (nb_dices - 1) * DICE_SIZE * SPACING_FACTOR_DICES + DICE_SIZE;

        // Calculate the starting x position to center the dice
        start_x = (renderer_w - total_width) / 2;

        // Set the x position for the current dice
        destination.x = start_x + position * DICE_SIZE * SPACING_FACTOR_DICES;

        // Set the y position for the current dice based on the player
        if (player->idPlayer == game->currentPlayer)
        {
            destination.y = (renderer_h - DICE_SIZE) * 8 / 9 + 0.05 * renderer_h;
        }
        else
        {
            destination.y = (renderer_h - DICE_SIZE) / 9 - 0.05 * renderer_h;
        }

        // Set the width and height for the current dice
        destination.w = DICE_SIZE;
        destination.h = DICE_SIZE;

        // Render the current dice texture to the renderer
        if (player->idPlayer == game->currentPlayer)
        {
            SDL_RenderCopy(renderer, dice[game->diceRolls[i]], &source, &destination);
        }
        else
        {
            SDL_RenderCopy(renderer, dice[0], &source, &destination);
        }

        // Increment the position for the next dice
        position++;
    }
}

void RenderBackCard(SDL_Renderer *renderer, SDL_Texture *back, int rendererWidth, int rendererHeight)
{
    SDL_Rect source = {0}, destination = {0};

    SDL_QueryTexture(back, NULL, NULL, &source.w, &source.h);

    // Position of the card
    destination.x = 0.1 * rendererWidth; //(rendererWidth) / 7;
    destination.y = (rendererHeight - CUP_SIZE) / 3 + 50;
    destination.w = CARD_WIDTH;
    destination.h = CARD_HEIGHT;

    SDL_RenderCopy(renderer, back, &source, &destination);
}

void RenderDiscard(SDL_Renderer *renderer, SDL_Texture *card, int rendererWidth, int rendererHeight)
{
    SDL_Rect source = {0}, destination = {0};

    SDL_QueryTexture(card, NULL, NULL, &source.w, &source.h);

    // Position of the card
    destination.x = rendererWidth - 0.3 * rendererWidth; //(rendererWidth) / 1.5;
    destination.y = (rendererHeight - CUP_SIZE) / 3 + 50;
    destination.w = CARD_WIDTH;
    destination.h = CARD_HEIGHT;

    SDL_RenderCopy(renderer, card, &source, &destination);
}

void RenderPlayedCard(SDL_Renderer *renderer, SDL_Texture *card, int rendererWidth, int rendererHeight)
{
    SDL_Rect source = {0}, destination = {0};

    SDL_QueryTexture(card, NULL, NULL, &source.w, &source.h);

    // Position of the card
    destination.x = (rendererWidth) / 3;
    destination.y = (rendererHeight - CUP_SIZE) / 3 + 50;
    destination.w = CARD_WIDTH * 1.7;
    destination.h = CARD_HEIGHT * 1.7;

    SDL_RenderCopy(renderer, card, &source, &destination);
}

void saveDeckIndex(Player *player, int idCardDeck[DECK_SIZE])
{
    int i = 0, j = 0;
    while (i < player->nbOfCards && j < DECK_SIZE)
    {
        if (player->deck[j] != NULL)
        {
            idCardDeck[i] = j;
            i++;
        }
        j++;
    }
    while (i < DECK_SIZE)
    {
        idCardDeck[i] = -1;
        i++;
    }
}

void RenderCards(SDL_Renderer *renderer, SDL_Texture **cards, Game *game, Player *player, int selected_card1, int *idCardDeck)
{
    int i;
    int renderer_w, renderer_h; // Renderer width and height
    int position = 0;           // Position index for cards
    int nb_cards;               // Number of cards for the player
    float total_width;          // Total width occupied by all cards
    float start_x;              // Starting x position for the cards

    // Determine the number of cards based on the player
    nb_cards = player->nbOfCards;

    // Get the output size of the renderer
    SDL_GetRendererOutputSize(renderer, &renderer_w, &renderer_h);

    SDL_Rect source = {0};
    SDL_Rect destination = {0};
    int save_x = 0; // Saved x position for the selected card
    int save_y = 0; // Saved y position for the selected card
    int index = 0;

    for (i = 0; i < nb_cards; i++)
    {
        index = idCardDeck[i];
        // Query the texture to get its width and height
        if (player->idPlayer == game->currentPlayer)
        {
            SDL_QueryTexture(cards[player->deck[index]->cardType + 1], NULL, NULL, &source.w, &source.h);
        }
        else
        {
            SDL_QueryTexture(cards[0], NULL, NULL, &source.w, &source.h);
        }

        // Calculate the total width of all cards including spacing
        total_width = (nb_cards - 1) * CARD_WIDTH * SPACING_FACTOR_CARDS + CARD_WIDTH;

        // Calculate the starting x position to center the cards
        start_x = (renderer_w - total_width) / 2;

        // Set the x position for the current card
        destination.x = start_x + position * CARD_WIDTH * SPACING_FACTOR_CARDS;

        if (player->idPlayer == game->currentPlayer)
        {
            destination.y = (renderer_h - CARD_HEIGHT) * 4 / 5;
        }
        else
        {
            destination.y = (renderer_h - CARD_HEIGHT) / 5;
        }
        destination.w = CARD_WIDTH;
        destination.h = CARD_HEIGHT;

        if (selected_card1 == i)
        {
            save_x = destination.x;
            save_y = destination.y;
        }
        else
        {
            if (player->idPlayer == game->currentPlayer)
            {
                SDL_RenderCopy(renderer, cards[player->deck[index]->cardType + 1], &source, &destination);
            }
            else
            {
                SDL_RenderCopy(renderer, cards[0], &source, &destination);
            }
        }

        position++; // Move to the next position
    }

    // Draw a white rectangle around the selected card and render it with zoom
    if (selected_card1 >= 0)
    {
        index = idCardDeck[selected_card1];
        destination.x = save_x - 8;
        destination.y = save_y - 8;
        destination.w = CARD_WIDTH * CARD_ZOOM + 16;
        destination.h = CARD_HEIGHT * CARD_ZOOM + 16;
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Set color to white
        SDL_RenderFillRect(renderer, &destination);           // Fill rectangle
        SDL_RenderDrawRect(renderer, &destination);           // Draw rectangle border
        destination.x = save_x;
        destination.y = save_y;
        destination.w = CARD_WIDTH * CARD_ZOOM;
        destination.h = CARD_HEIGHT * CARD_ZOOM;
        SDL_RenderCopy(renderer, cards[player->deck[index]->cardType + 1], &source, &destination); // Render the selected card with zoom
    }
}

void RenderMoveCup(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *bg, SDL_Texture *cup_moving)
{
    int renderer_w, renderer_h;
    // Get the output size of the renderer
    SDL_GetRendererOutputSize(renderer, &renderer_w, &renderer_h);

    SDL_Rect source1 = {0};
    SDL_Rect source2 = {0};
    SDL_Rect destination1 = {0};
    SDL_Rect destination2 = {0};

    // Query the texture to get its width and height for both source rectangles
    SDL_QueryTexture(cup_moving, NULL, NULL, &source1.w, &source1.h);
    SDL_QueryTexture(cup_moving, NULL, NULL, &source2.w, &source2.h);

    // Set the x position for both destination rectangles to center the cup
    destination1.x = (renderer_w - CUP_SIZE) / 2;
    destination2.x = (renderer_w - CUP_SIZE) / 2;

    // Set the initial y position for both destination rectangles
    destination1.y = (renderer_h - CUP_SIZE) * 5 / 6;
    destination2.y = (renderer_h - CUP_SIZE) / 6;

    // Set the width and height for both destination rectangles
    destination1.w = CUP_SIZE;
    destination2.w = CUP_SIZE;
    destination1.h = CUP_SIZE;
    destination2.h = CUP_SIZE;

    srand(time(NULL));

    int dy = 4; // Initial movement delta for the y direction

    clock_t start_time = clock();
    while ((clock() - start_time) < CLOCKS_PER_SEC / 2) // Loop for half a second
    {
        SDL_PumpEvents();

        // Check if the cup reached the upper or lower bounds, reverse direction if so
        if (destination1.y + dy < renderer_h / 2 || destination1.y + dy + CUP_SIZE > renderer_h)
        {
            dy = -dy; // Reverse the direction of movement
        }

        // Update the y position for both destination rectangles
        destination1.y += dy;
        destination2.y += dy;

        SDL_RenderClear(renderer); // Clear the renderer

        RenderInit(window, renderer, bg); // Render the initial state

        // Render the moving cup textures
        SDL_RenderCopy(renderer, cup_moving, &source1, &destination1);
        SDL_RenderCopy(renderer, cup_moving, &source2, &destination2);

        SDL_RenderPresent(renderer); // Present the updated renderer
    }
}