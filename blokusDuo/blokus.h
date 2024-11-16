#ifndef BLOKUS_H
#define BLOKUS_H
#include "piece.h"

#include <SDL2/SDL.h>

// Function prototypes
void end_sdl(char ok, const char *msg, SDL_Window *window, SDL_Renderer *renderer);
void RenderBackground(SDL_Texture *my_texture, SDL_Window *window, SDL_Renderer *renderer);
void RenderSquareOnGrid(SDL_Window *window, SDL_Renderer *renderer, int pos_x, int pos_y, int color, int board_placement, SDL_Texture **shapes);
void RenderBoard(SDL_Window *window, SDL_Renderer *renderer, int board[14][14], SDL_Texture **shapes);
// void RenderLeftGrid(SDL_Window *window, SDL_Renderer *renderer, int table[21][5][5], SDL_Texture **shapes, int player);
void RenderSideGrid(SDL_Window *window, SDL_Renderer *renderer, Piece *pieceList[21], SDL_Texture **shapes, int player);
void HighlightGrid(SDL_Window *window, SDL_Renderer *renderer, int piece, int player);

void MoovePieceOnBoard(SDL_Window *window, SDL_Renderer *renderer, Piece *piece, int posX, int posY, int player, SDL_Texture **shapes);

#endif // BLOKUS_H
