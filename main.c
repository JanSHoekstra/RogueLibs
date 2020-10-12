#include <SDL2/SDL.h>
#include "lib/proceduralDunGen.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900
#define TILESIZE 16
SDL_Window *window = 0; // Global window
SDL_Renderer *renderer = 0; // Global renderer
SDL_Texture *mapTexture = 0; // Global background

const int rows = 50, cols = 100; // Size of map
int **map; // Global map

SDL_Texture *textures[10];
enum texture{WALLTEX = 0, PLAYERTEX};

void initSDL()
{
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        exit(1);
    }
    //Create window
    window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN );
    if( window == 0 )
    {
        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        SDL_Quit();
        exit(2);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if( renderer == 0 )
    {
        printf( "Renderer could not be created! SDL_Error: %s\n", SDL_GetError() );
        SDL_Quit();
        exit(2);
    }
    //SDL_RenderSetIntegerScale(renderer, 1);
    SDL_RenderSetLogicalSize(renderer, cols*TILESIZE,rows*TILESIZE);

    mapTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, cols*TILESIZE, rows*TILESIZE);
    if( mapTexture == 0 )
    {
        printf( "map texture could not be created! SDL_Error: %s\n", SDL_GetError() );
        SDL_Quit();
        exit(2);
    }
}

void createDungeon()
{
    // Create a 2D array of fully dynamic size
    //free(map);
    map = malloc(rows * sizeof *map);
    for (int i=0; i < rows; i++)
        map[i] = malloc(cols * sizeof *map[i]);

    // Create dungeon map.
    generate(map, rows, cols);

    const SDL_Rect bmpRect = {0,0,TILESIZE,TILESIZE};
    SDL_Rect destRect = {0,0,TILESIZE,TILESIZE};

    //Temporarily render to texture
    SDL_SetRenderTarget(renderer, mapTexture);

    // Print dungeon map.
    for(int i = 0; i < rows; i++)
        for(int j = 0; j < cols; j++)
            if(map[i][j] == WALL){
                destRect.x = TILESIZE*j;
                destRect.y = TILESIZE*i;
                SDL_RenderCopy(renderer, textures[WALLTEX], &bmpRect, &destRect);
            }
    // Future render calls render to renderer itself again.
    SDL_SetRenderTarget(renderer, 0);
}

void loadTextures()
{
    // Make wall texture
    SDL_Surface *bmps = SDL_LoadBMP("img/wall.bmp");
    SDL_Texture *bmpt = SDL_CreateTextureFromSurface(renderer, bmps);
    SDL_FreeSurface(bmps);
    textures[0] = bmpt;

    bmps = SDL_LoadBMP("img/player.bmp");
    bmpt = SDL_CreateTextureFromSurface(renderer, bmps);
    SDL_FreeSurface(bmps);
    textures[1] = bmpt;

}

int handleEvents()
{
    // Get the next event
    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            // Break out of the gameloop on quit
            return 0;
    }

    return 1;
}

void render()
{
        SDL_RenderClear(renderer);
        const SDL_Rect mapRect = {0,0,cols*TILESIZE,rows*TILESIZE};
        SDL_RenderCopy(renderer, mapTexture,&mapRect,&mapRect);
        SDL_RenderPresent(renderer);
}

int main(int argc, char ** argv)
{
    srand(time(0));
    initSDL();
    loadTextures();
    createDungeon();
    while (1)
    {
        if (handleEvents() == 0) break;

        render();
    }

    SDL_DestroyTexture(mapTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow( window );
    SDL_Quit();

    return 0;
}