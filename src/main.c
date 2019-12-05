#include <omp.h>
#include <SDL.h>
#include <stdio.h>
#include "gol.h"

SDL_Window*   gwindow;
SDL_Renderer* grenderer;

int** current_state;

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

# pragma omp parallel single
  {
    /* start thread to print the states onto the window here */
#   pragma omp task
    foo();
    /* call function to compute gol states here */
    gol();
  }
  
 renderer_err:
  SDL_DestroyRenderer(grenderer);
 window_err:
  SDL_DestroyWindow(gwindow);
  SDL_Quit();
 init_err:
  return 0;
}

void gol_print (int y, int x)
{
  SDL_Rect r = {x * 4, y * 4, 4, 4};
  
  SDL_SetRenderDrawColor(grenderer, 0, 0, 0, 255);
  SDL_RenderFillRect(grenderer, &r);
}

char** cur_state;

void foo ()
{
  SDL_Event e;
  char mouse_pressed = 0;
  int x, y;

  cur_state = calloc(150, sizeof(char*));
  for (int i = 0; i < 150; i++)
    cur_state[i] = calloc(200, sizeof(char));
  
  for (;;) {
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
      case SDL_QUIT:
	/* TODO tell the other thread it has stopped later */
	return;
      case SDL_MOUSEBUTTONDOWN:
	mouse_pressed = 1;
	break;
      case SDL_MOUSEBUTTONUP:
	mouse_pressed = 0;
	break;
      }
    }
      
    SDL_SetRenderDrawColor(grenderer, 255, 255, 255, 255);
    SDL_RenderClear(grenderer);
    /* read game states and print to screen */

    if (mouse_pressed) {
      SDL_GetMouseState(&x, &y);
      printf("mouse pressed, x: %d, y: %d\n", x, y);
      //gol_print(y / 4, x / 4);
      cur_state[y/4][x/4] = 1;
    }

    for (int i = 0; i < 150; i++) {
      for (int j = 0; j < 200; j++) {
	if (cur_state[i][j])
	  gol_print(i, j);
      }
    }
    
    SDL_RenderPresent(grenderer);
    
  }
}
