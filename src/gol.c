#include "gol.h"

#include <omp.h>
#include <stdlib.h>

static int livesp (char** state, int y, int x);

//cria a matrix para o próximo estado
char** gol_next_state (char** start, int w, int h)
{
  char** next;
  int i, j;

  next = calloc(h + 2, sizeof(char*));
 #pragma omp parallel for
  for (i = 0; i <= h+2; i++)
    next[i] = calloc(w + 2, sizeof(char));
# pragma omp parallel for collapse(2)
  for (i = 1; i <= h+1; i++)
    for (j = 1; j <= w+1; j++)
      next[i][j] = livesp(start, i, j);
  
  return next;
}

static int livesp (char** state, int y, int x)
{
  int acc = 0;
  int i, j;
//testa quantos vizinhos, incluindo ele próprio, estão vivos
# pragma omp parallel for collapse(2)
  for (i = -1; i <= 1; i++)
    for (j = -1; j <= 1; j++)
      if (state[y+i][x+j])
	acc++;
//aplica as regras do jogo
  if (state[y][x] &&
      acc > 2 &&
      acc < 5) {
    return 1;
  } else if (acc == 3) {
    return 1;
  } else {
    return 0;
  }
}
/*

for(int i = 0; i < 150; ++i){
  for(int j = 0; j < 200; ++j){
    regras(cur_state, i, j);
  }
}
 regras(cur_state, y, x){
  acumulador = 0;
  for(int i = -1; i <= 1; ++i){
   for(int j = -1; j <= 1; ++j){
     if(cur_state[y+i][x+j])
     ++acumulador; 
   }
 }
}
    Any live cell with fewer than two live neighbours dies, as if by underpopulation.
    Any live cell with two or three live neighbours lives on to the next generation.
    Any live cell with more than three live neighbours dies, as if by overpopulation.
    Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
 */
