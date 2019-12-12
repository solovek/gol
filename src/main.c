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
    foo();
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

static void mtxfree (char**, int);

void foo ()
{
  char** cur_state;
  char** prv_state;
  SDL_Event e;
  char mouse_pressed = 0;
  char paused        = 1;
  int x, y;

  cur_state = calloc(152, sizeof(char*));

#pragma omp parallel for
  for (int i = 0; i <= 152; i++)
    cur_state[i] = calloc(202, sizeof(char));
  
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
      case SDL_KEYDOWN:
	switch (e.key.keysym.sym) {
	case SDLK_SPACE:
	  paused ^= 1;
	  if (paused)
	    printf("paused\n");
	  else
	    printf("unpaused\n");
	  break;
	}
	break;
      }
    }
      
    SDL_SetRenderDrawColor(grenderer, 255, 255, 255, 255);
    SDL_RenderClear(grenderer);

    if (!paused) {
      prv_state = cur_state;
      cur_state = gol_next_state(cur_state, 200, 150);
      mtxfree(prv_state, 152);
    }
    
    if (mouse_pressed) {
      SDL_GetMouseState(&x, &y);
      printf("mouse pressed, x: %d, y: %d\n", x, y);
      //gol_print(y / 4, x / 4);
      cur_state[y/4][x/4] = 1;
    }
#pragma omp taskloop collapse(2)
    for (int i = 1; i <= 150; i++) {
      for (int j = 1; j <= 200; j++) {
	if (cur_state[i][j])
	  gol_print(i-1, j-1);
      }
    }
    
    SDL_RenderPresent(grenderer);
    
  }
}

static void mtxfree (char** m, int h)
{
  int i;
#pragma omp parallel for
  for (i = 0; i < h; i++)
    free(m[i]);
  free(m);
}
