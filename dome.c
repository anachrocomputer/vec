/* dome --- plot net for geodesic dome                      1996-12-15 */
/* Copyright (c) 1996 John Honniball                                   */

#include <stdio.h>
#include "turtle.h"

void hexa(const double len);
void penta(const double len);

int main(int argc, char * const argv[])
{
   int i;
   const double size = 32.0;  /* 32mm sides */
   
   turtle(DEV_HPGL, SIZ_A3, ORI_LAND, FLG_BORD);
   
   pen(UP);
   turn(90.0);
   forward(size + 5.0);
   turn(-90.0);
   pen(DOWN);

   colour(BLACK);
   for (i = 0; i < 3; i++) {
      penta(size);

      turn(60.0);
      forward(size);
      turn(-60.0);

      hexa(size);

      turn(-60.0);

      penta(size);

      turn(-60.0);

      hexa(size);

      turn(-60.0);
      forward(size);
      turn(60.0);
   }
   
#ifdef DRAW_FOLD_LINES
   colour(RED);
   for (i = 0; i < 6; i++) {  /* Clockwise hexagon in centre */
      forward(size);
      turn(-60.0);
   }
   
   for (i = 0; i < 3; i++) {  /* Three half-hexagons for fold lines */
      pen(UP);
      forward(size);
      turn(60.0);
      forward(size);
      turn(-60.0);
      pen(DOWN);
      
      for (j = 0; j < 3; j++) {
         forward(size);
         turn(-60.0);
      }
      
      pen(UP);
      forward(size);
      turn(60.0);
      pen(DOWN);
   }
#endif

   show();
   
   return (0);
}
   

void hexa(const double len)
{
   int i;
   
   turn(120.0);

   for (i = 0; i < 5; i++) {
      forward(len);
      turn(-60.0);
   }

   turn(180.0);
}


void penta(const double len)
{
   int i;
   
   turn(108.0);

   for (i = 0; i < 4; i++) {
      forward(len);
      turn(-72.0);
   }

   turn(180.0);
}
