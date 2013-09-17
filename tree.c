/* tree --- plot recursive tree                             1996-12-16 */
/* Copyright (c) 1996 BJ, Froods Software Development                  */

#include <stdio.h>
#include "turtle.h"

/* Geometric parameters */
#define LEFT_ANGLE   (30.0)
#define LEFT_SIZE    (0.8)
#define RIGHT_ANGLE  (25.0)
#define RIGHT_SIZE   (0.75)

void tree (int level, double len);

int main (int argc, const char *argv[])
{
   int level = 8;
// double size = 30.0;  /* 30mm trunk for A4 */
   double size = 65.0;  /* 65mm trunk for A3 */
   
   turtle (DEV_HPGL, SIZ_A1, ORI_LAND, FLG_NONE|FLG_RELS|FLG_BORD);
   
// title ("http://www.dorkbot.org/dorkbotbristol/", 3.75, BOT|CENTRE, FLG_NONE);
   title ("Bristol Hackspace", 3.75, BOT|CENTRE, FLG_NONE);
   
   pen (UP);
   turn (-90.0);
   forward (1.9 * size);
   turn (180.0);
   pen (DOWN);

   colour (BLACK);
   
   tree (level, size);

   show ();
   
   return (0);
}
   

/* tree --- recursive function to plot a tree */

void tree (int level, double len)
{
   forward (len);

   if (level == 0) {
      turn (180.0);
   }
   else {
      turn (LEFT_ANGLE);
      tree (level - 1, len * LEFT_SIZE);
      turn ((180.0 - LEFT_ANGLE) - RIGHT_ANGLE);
      tree (level - 1, len * RIGHT_SIZE);
      turn (RIGHT_ANGLE);
   }

   forward (len);
}
