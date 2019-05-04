/* hilb --- plot Hilbert curve                              1996-12-16 */
/* Copyright (c) 1996 John Honniball, Froods Software Development      */

#include <stdio.h>
#include "turtle.h"

#define A3

void hilb(const int level, const double len);
void bert(const int level, const double len);

int main(int argc, const char *argv[])
{
   int level;
#ifdef A1
   const double overall = 540.0; /* Overall size 540mm */
   const int maxlevel = 6;
#else
   const double overall = 270.0; /* Overall size 270mm */
   const int maxlevel = 5;
#endif
   double size;
   double height;
   double width;

#ifdef A1
   turtle(DEV_HPGL, SIZ_A1, ORI_LAND, FLG_BORD);
#else
   turtle(DEV_HPGL, SIZ_A3, ORI_LAND, FLG_BORD|FLG_RELS);
#endif
   
   title("HILBERT", 3.75, BOT|LEFT, ITALIC);
// title("2013-03-23", 3.75, BOT|RIGHT, ITALIC);
   
   height = page_height();
   width = page_width();
   size = overall / 2.0;

   for (level = 1; level <= maxlevel; level++) {
      bottom_left();
      set_heading(0.0);
   
      pen(UP);
      forward((size / 2.0) + ((width - overall) / 2.0));
      turn(90.0);
      forward((size / 2.0) + ((height - overall) / 2.0));
      turn(-90.0);
      pen(DOWN);

      switch (level % 4) {   
      case 0:
         colour(BLACK);
         break;
      case 1:
         colour(RED);
         break;
      case 2:
         colour(GREEN);
         break;
      case 3:
         colour(BLUE);
         break;
      }
   
      hilb(level, size);

      size /= 2.0;
   }

   show();
   
   return (0);
}
   

void hilb(const int level, const double len)
{
   if (level == 0)
      return;
      
   turn(90.0);
   bert(level - 1, len);
   forward(len);
   turn(-90.0);
   hilb(level - 1, len);
   forward(len);
   hilb(level - 1, len);
   turn(-90.0);
   forward(len);
   bert(level - 1, len);
   turn(90.0);
}


void bert(const int level, const double len)
{
   if (level == 0)
      return;
      
   turn(-90.0);
   hilb(level - 1, len);
   forward(len);
   turn(90.0);
   bert(level - 1, len);
   forward(len);
   bert(level - 1, len);
   turn(90.0);
   forward(len);
   hilb(level - 1, len);
   turn(-90.0);
}
