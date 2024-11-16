#ifndef WAZABI_H
#define WAZABI_H

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "game.h"

#define CUP_SIZE 200
#define CARD_HEIGHT 250
#define CARD_WIDTH 180
#define CARD_ZOOM 1.7
#define DICE_SIZE 50
#define SPACING_FACTOR_DICES 1.5  // spacing between dices
#define SPACING_FACTOR_CARDS 1.05 // spacing between cards
#define NB_OF_DICES 3
#define NB_OF_CARDS 11

// Function prototypes

// End the videogame properly and return error messages if needed
void end_sdl(char ok, const char *msg, SDL_Window *window, SDL_Renderer *renderer);

// Load the textures
SDL_Texture *loadTexture(const char *file, SDL_Renderer *renderer);

// Initialize the board
void RenderInit(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *my_texture);

// Display dices
void RenderDice(SDL_Renderer *renderer, SDL_Texture **dice, Player *player, Game *game);

// Moving cup animation
void RenderMoveCup(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *bg, SDL_Texture *cup_moving);

// Display the stack of cards
void RenderBackCard(SDL_Renderer *renderer, SDL_Texture *back, int rendererWidth, int rendererHeight);

// Display the cards from player
void RenderCards(SDL_Renderer *renderer, SDL_Texture **cards, Game *game, Player *player, int selected_card1, int *idCardDeck);

void saveDeckIndex(Player *player, int idCardDeck[DECK_SIZE]);

void RenderDiscard(SDL_Renderer *renderer, SDL_Texture *card, int rendererWidth, int rendererHeight);

void RenderPlayedCard(SDL_Renderer *renderer, SDL_Texture *card, int rendererWidth, int rendererHeight);

#endif
