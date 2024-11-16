#include "blokus.h"
#include "piece.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#define BOARD_SIZE 14
#define NB_PIECES 21

void end_sdl(char ok,            // normal end: ok = 0; abnormal: ok = 1
             char const *msg,    // message to print
             SDL_Window *window, // window & renderer to close
             SDL_Renderer *renderer)
{
    char msg_formated[255];
    int l;

    if (!ok)
    { // Display the error message
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

void RenderBackground(SDL_Texture *my_texture, SDL_Window *window,
                      SDL_Renderer *renderer)
{
    SDL_Rect
        source = {0},
        window_dimensions = {0},
        destination = {0};

    SDL_GetWindowSize(window, &window_dimensions.w, &window_dimensions.h);
    SDL_QueryTexture(my_texture, NULL, NULL, &source.w, &source.h);

    destination = window_dimensions;

    SDL_RenderCopy(renderer, my_texture, &source, &destination);
}

// this function generates a new main grid
void RenderBoard(SDL_Window *window, SDL_Renderer *renderer, int board[14][14], SDL_Texture **shapes)
{
    int i;
    int j;

    SDL_Rect rectangle;

    // int h_board;

    int w_screen;
    int h_screen;
    SDL_GetWindowSize(window, &w_screen, &h_screen);

    SDL_SetRenderDrawColor(renderer,
                           200, 200, 200, 255);

    rectangle.x = 50;
    rectangle.y = 50;
    rectangle.w = w_screen - 100;
    rectangle.h = h_screen - 100;

    int board_placement = rectangle.w / 4;

    SDL_RenderFillRect(renderer, &rectangle);

    // h_board = (h_screen - 100) / NB_PIECES;

    SDL_SetRenderDrawColor(renderer,
                           120, 120, 120, 255);

    for (i = 0; i < BOARD_SIZE; i++)
    {
        for (j = 0; j < BOARD_SIZE; j++)
        {
            RenderSquareOnGrid(window, renderer, j, i, board[i][j], board_placement, shapes);
        }
    }
}

// Add the shapes to the main grid
void RenderSquareOnGrid(SDL_Window *window, SDL_Renderer *renderer, int pos_x, int pos_y, int color, int board_placement, SDL_Texture **shapes)
{
    SDL_Rect rectangle;
    int h_board;
    int w_screen;
    int h_screen;

    SDL_GetWindowSize(window, &w_screen, &h_screen);

    h_board = (h_screen - 100) / NB_PIECES; // Number of lines that fit into the screen

    rectangle.x = 50 + board_placement + pos_x * h_board;
    rectangle.y = 60 + pos_y * h_board;
    rectangle.w = h_board;
    rectangle.h = h_board;

    // Copy the texture to the renderer at the specified position
    SDL_RenderCopy(renderer, shapes[color], NULL, &rectangle);
}

void RenderNewPiece(SDL_Window *window, SDL_Renderer *renderer, int pos_x, int pos_y, int color, int board_placement, SDL_Texture **shapes)
{
    SDL_Rect rectangle;
    int h_board;
    int w_screen;
    int h_screen;

    SDL_GetWindowSize(window, &w_screen, &h_screen);

    h_board = (h_screen - 100) / NB_PIECES; // Number of lines that fit into the screen

    rectangle.x = 50 + board_placement + pos_x * h_board;
    rectangle.y = 60 + pos_y * h_board;
    rectangle.w = h_board;
    rectangle.h = h_board;

    // Copy the texture to the renderer at the specified position
    SDL_SetTextureAlphaMod(shapes[color], 128);
    SDL_RenderCopy(renderer, shapes[color], NULL, &rectangle);
}

// Add the shapes to the side grid
void RenderSideGrid(SDL_Window *window, SDL_Renderer *renderer, Piece *pieceList[21], SDL_Texture **shapes, int player)
{
    SDL_Rect rectangle;
    int cell_size;
    int w_screen, h_screen;
    int i, j, k, l;
    int value = 0;

    SDL_GetWindowSize(window, &w_screen, &h_screen);

    cell_size = (h_screen - 100) / 35; // Size of each small cell (assuming a total height divided by 35)

    int x_offset = (player == 1) ? 60 : w_screen - 60 - 15 * cell_size; // Adjust x position based on player

    for (i = 0; i < 7; i++) // 7 rows
    {
        for (j = 0; j < 3; j++) // 3 columns
        {
            for (k = 0; k < 5; k++) // 5x5 subgrid rows
            {
                for (l = 0; l < 5; l++) // 5x5 subgrid columns
                {
                    rectangle.x = x_offset + (j * 5 + l) * cell_size; // Calculate x position based on column and subgrid column
                    rectangle.y = 60 + (i * 5 + k) * cell_size;       // Calculate y position based on row and subgrid row
                    rectangle.w = cell_size;
                    rectangle.h = cell_size;

                    // Access the correct element in the 3D table
                    if (pieceList[i * 3 + j] != NULL && pieceList[i * 3 + j]->shape[k][l] == 1)
                    {
                        value = player;
                    }
                    else
                    {
                        value = 0;
                    }

                    // Copy the texture to the renderer at the specified position
                    if (value == 0)
                    {
                        SDL_RenderCopy(renderer, shapes[0], NULL, &rectangle);
                    }
                    else if (value == 1)
                    {
                        SDL_RenderCopy(renderer, shapes[1], NULL, &rectangle);
                    }
                    else
                    {
                        SDL_RenderCopy(renderer, shapes[2], NULL, &rectangle);
                    }
                }
            }
        }
    }
}

void MoovePieceOnBoard(SDL_Window *window, SDL_Renderer *renderer, Piece *piece, int posX, int posY, int player, SDL_Texture **shapes)
{
    SDL_Rect rectangle;
    SDL_Rect rectangleP;
    int cell_size;
    int w_screen, h_screen;
    int raw, column;

    SDL_GetWindowSize(window, &w_screen, &h_screen);

    // position plateau
    rectangle.x = 50;
    rectangle.y = 50;
    rectangle.w = w_screen - 100;
    rectangle.h = h_screen - 100;
    int board_placement = rectangle.w / 4;

    // position piece
    int h_board;
    h_board = (h_screen - 100) / NB_PIECES; // Number of lines that fit into the screen

    rectangleP.x = 50 + board_placement + (posX - 2) * h_board;
    rectangleP.y = 60 + (posY - 2) * h_board;
    rectangleP.w = 5 * h_board;
    rectangleP.h = 5 * h_board;

    // Affichage de la piece
    for (int i = -2; i < 3; i++)
    {
        for (int j = -2; j < 3; j++)
        {
            if (piece->shape[i + 2][j + 2] == 1)
            {
                RenderSquareOnGrid(window, renderer, posX + j, posY + i, player, board_placement, shapes);
            }
        }
    }
    // Set the drawing color to white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Draw the border
    SDL_RenderDrawRect(renderer, &rectangleP);
}

void HighlightGrid(SDL_Window *window, SDL_Renderer *renderer, int piece, int player)
{
    SDL_Rect rectangle;
    int cell_size;
    int w_screen, h_screen;
    int raw, column;

    SDL_GetWindowSize(window, &w_screen, &h_screen);

    cell_size = (h_screen - 100) / 35; // Size of each small cell (assuming a total height divided by 35)

    int x_offset = (player == 1) ? 60 : w_screen - 60 - (15 * cell_size); // Adjust x position based on player

    raw = piece / 3;    // Calculating the row of the 5x5 grid
    column = piece % 3; // Calculating the column of the 5x5 grid

    // Define the rectangle for the border
    rectangle.x = x_offset + column * 5 * cell_size;
    rectangle.y = 60 + raw * 5 * cell_size;
    rectangle.w = 5 * cell_size;
    rectangle.h = 5 * cell_size;

    // Set the drawing color to white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Draw the border
    SDL_RenderDrawRect(renderer, &rectangle);
}

/*int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;



    return 0;
}*/
