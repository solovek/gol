#include <SDL.h>
#include <stdio.h>

SDL_Window*   gwindow;
SDL_Renderer* grenderer;

int main (int argc, char** argv)
{
  Uint32 flags;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("%s\n", SDL_GetError());
    goto init_err;
  }
  
  flags = SDL_WINDOW_SHOWN;
  if (!(gwindow = SDL_CreateWindow("Game of Life",
				   SDL_WINDOWPOS_UNDEFINED,
				   SDL_WINDOWPOS_UNDEFINED,
				   800,
				   600,
				   flags))) {
    printf("%s\n", SDL_GetError());
    goto window_err;
  }

  flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
  if (!(grenderer = SDL_CreateRenderer(gwindow, -1, flags))) {
    printf("%s\n", SDL_GetError());
    goto renderer_err;
  }

  /* start thread to compute gol states here */
  /* call function to print the states onto the window here */

 renderer_err:
  SDL_DestroyRenderer(grenderer);
 window_err:
  SDL_DestroyWindow(gwindow);
 init_err:
  return 0;
}
