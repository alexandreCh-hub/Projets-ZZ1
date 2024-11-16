#include "board.h"
#include "decisionTree.h"
#include "game.h"
#include "piece.h"
#include <stdio.h>

#include "blokus.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>

int main()
{
    Game *game = initGame();

    // int posX, posY, numPiece, rotation;
    Piece *piece = NULL;

    // ajouter un cas particulier pour le premier coup : 2 ronds sur le plateau + pas besoin de toucher un angle
    game->board->grid[4][4] = 1;
    game->board->grid[9][9] = 2;

    int idPosition = 0;
    int xPosition = 0;
    int yPosition = 0;

    // int i, j, k;
    srand(time(NULL));

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_DisplayMode screen;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        end_sdl(0, "ERROR SDL INIT", window, renderer);

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
        end_sdl(0, "ERROR SDL IMG INIT", window, renderer);

    SDL_GetCurrentDisplayMode(0, &screen);

    // Create the window
    window = SDL_CreateWindow("Blokus",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              screen.w * 0.66,
                              screen.h * 0.66,
                              SDL_WINDOW_OPENGL);
    if (window == NULL)
        end_sdl(0, "ERROR WINDOW CREATION", window, renderer);

    // Create the renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
        end_sdl(0, "ERROR RENDERER CREATION", window, renderer);

    // Load textures
    SDL_Texture *bg = IMG_LoadTexture(renderer, "./pictures/bg.png");
    if (!bg)
        end_sdl(0, "ERROR LOADING TEXTURE", window, renderer);

    SDL_Texture *red_shape = IMG_LoadTexture(renderer, "./pictures/red.png");
    if (!red_shape)
        end_sdl(0, "ERROR LOADING TEXTURE (red)", window, renderer);

    SDL_Texture *blue_shape = IMG_LoadTexture(renderer, "./pictures/blue.png");
    if (!blue_shape)
        end_sdl(0, "ERROR LOADING TEXTURE (blue)", window, renderer);

    SDL_Texture *grey_shape = IMG_LoadTexture(renderer, "./pictures/grey.png");
    if (!grey_shape)
        end_sdl(0, "ERROR LOADING TEXTURE (grey)", window, renderer);

    SDL_Texture *shapes[] = {grey_shape, red_shape, blue_shape};

    SDL_bool program_on = SDL_TRUE;
    SDL_Event event;

    // Variable to track the state of the 'S' key
    SDL_bool is_s_key_pressed = SDL_FALSE;

    Node *moveToPlay = NULL;

    // Main loop
    while (program_on)
    {

        if (game->currentPlayer == firstPlayer)
        {
            Node *root = createNode(-1, -1, -1, 0);
            generateChildren(root, game, 0);
            moveToPlay = minimax(root, 1);
            piece = copyPiece(game->pieces[firstPlayer - 1][moveToPlay->idPiece]);
            for (int rota = 0; rota < moveToPlay->rotation; rota++)
            {
                piece = rotatePiece(piece);
            }
            placePiece(game, piece, piece->id, moveToPlay->row, moveToPlay->col);
            switchPlayer(game);
        }

        // Handle SDL events
        else
        {
            while (SDL_PollEvent(&event))
            {
                switch (event.type)
                {
                case SDL_QUIT:
                    program_on = SDL_FALSE;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_s:
                        // // Handle 'S' key down event here
                        // printf("s key pressed down\n");
                        // // Set the state of 'S' key to pressed
                        if (game->pieces[secondPlayer - 1][idPosition] != NULL)
                        {
                            is_s_key_pressed = !is_s_key_pressed;
                            piece = copyPiece(game->pieces[secondPlayer - 1][idPosition]);
                        }
                        break;
                    case SDLK_p:
                        if (isValidPosition(game, piece, yPosition, xPosition))
                        {
                            placePiece(game, piece, piece->id, yPosition, xPosition);
                            is_s_key_pressed = !is_s_key_pressed;
                            switchPlayer(game);
                            idPosition = 0;
                        }
                        break;
                    case SDLK_r:
                        if (is_s_key_pressed)
                            piece = rotatePiece(piece);
                        break;
                    case SDLK_LEFT:
                        if (is_s_key_pressed)
                        {
                            if (xPosition > 0)
                                xPosition--;
                        }
                        else
                        {
                            if (idPosition > 0)
                                idPosition--;
                        }
                        break;
                    case SDLK_RIGHT:
                        if (is_s_key_pressed)
                        {
                            if (xPosition < 14)
                                xPosition++;
                        }
                        else
                        {
                            if (idPosition < NB_PIECES - 1)
                                idPosition++;
                        }
                        break;
                    case SDLK_UP:
                        if (is_s_key_pressed)
                        {
                            if (yPosition > 0)
                                yPosition--;
                        }
                        else
                        {
                            if (idPosition > 2)
                            {
                                idPosition -= 3;
                            }
                        }
                        break;
                    case SDLK_DOWN:
                        if (is_s_key_pressed)
                        {
                            if (yPosition < 14)
                                yPosition++;
                        }
                        else
                        {
                            if (idPosition < NB_PIECES - 3)
                            {
                                idPosition += 3;
                            }
                        }
                        break;
                    default:
                        break;
                    }
                // Add cases for other keys if needed
                default:
                    break;
                }
            }
        }

        // Clear the renderer and render the scene
        SDL_RenderClear(renderer);
        RenderBackground(bg, window, renderer);
        RenderBoard(window, renderer, game->board->grid, shapes);
        RenderSideGrid(window, renderer, game->pieces[firstPlayer - 1], shapes, 1);
        RenderSideGrid(window, renderer, game->pieces[secondPlayer - 1], shapes, 2);
        HighlightGrid(window, renderer, idPosition, game->currentPlayer); // Example: Highlight grid cell at position 5 for player 1

        // Additional logic based on the state of 'S' key
        if (is_s_key_pressed)
        {
            // Additional logic when 'S' key is pressed
            // For example, you can continuously update something here
            MoovePieceOnBoard(window, renderer, piece, xPosition, yPosition, game->currentPlayer, shapes);
        }

        // Present the rendered scene to the screen
        SDL_RenderPresent(renderer);
    }

    // Clean up resources
    SDL_DestroyTexture(bg);
    SDL_DestroyTexture(red_shape);
    SDL_DestroyTexture(blue_shape);
    SDL_DestroyTexture(grey_shape);
    end_sdl(1, "Normal ending", window, renderer);

    // Donner gagnant
    return 0;

    // Donner gagnant
}