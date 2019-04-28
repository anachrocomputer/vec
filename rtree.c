/* rtree --- plot randomised recursive tree                 1996-12-16 */
/* Copyright (c) 1996 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "turtle.h"

/* Geometric parameters */
#define LEFT_ANGLE   (27.0)
#define LEFT_SIZE    (0.75)
#define RIGHT_ANGLE  (22.0)
#define RIGHT_SIZE   (0.70)

void tree(const int level, const double len);

double Left_angle;
double Right_angle;
double Left_size;
double Right_size;

int main(int argc, const char *argv[])
{
   const int level = 8;
   const double size = 65.0;  /* 65mm trunk for A3 */
   
   turtle(DEV_HPGL, SIZ_A3, ORI_LAND, FLG_BORD|FLG_RELS);
   
// title("http://www.dorkbot.org/dorkbotbristol/", 3.75, BOT|LEFT, FLG_NONE);
   title("Bristol Hackspace", 3.75, BOT|LEFT, FLG_NONE);
   title("2018-04-22", 3.75, BOT|RIGHT, FLG_NONE);
   
   srand((unsigned int)time(NULL));

   Left_angle = LEFT_ANGLE + (((double)rand() / (double)RAND_MAX) * 6.0);
   Right_angle = RIGHT_ANGLE + (((double)rand() / (double)RAND_MAX) * 6.0);
   
   Left_size = LEFT_SIZE + (((double)rand() / (double)RAND_MAX) * 0.05);
   Right_size = RIGHT_SIZE + (((double)rand() / (double)RAND_MAX) * 0.05);
   
   pen(UP);
   turn(-90.0);
   forward(1.9 * size);
   turn(180.0);
   pen(DOWN);

   colour(BLACK);
   
   tree(level, size);

   show();
   
   return (0);
}
   

/* tree --- recursive function to plot a tree */

void tree(const int level, const double len)
{
   forward(len);

   if (level == 0) {
      turn(180.0);
   }
   else {
      turn(Left_angle);
      tree(level - 1, len * Left_size);
      turn((180.0 - Left_angle) - Right_angle);
      tree(level - 1, len * Right_size);
      turn(Right_angle);
   }

   forward(len);
}
