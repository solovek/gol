#include "gol.h"

#include <omp.h>

void gol (void)
{
  
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
