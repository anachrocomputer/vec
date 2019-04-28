/* flake --- plot a snowflake                               2009-11-12 */
/* Copyright (c) 2009 John Honniball, Froods Software Development      */

#include <stdio.h>
#include "turtle.h"

struct Finger {
   double hi;
   double w1;
   double lo;
   double w2;
};

int main(int argc, char * const argv[])
{
   int i;
   int j;
   const double size = 140.0;  /* 140mm sides */
   double radius;
   double height;
   struct Finger finger[10] = {
      {50.0, 10.0, 40.0, 10.0},
      {20.0, 10.0, 20.0, 10.0},
      {30.0, 10.0, 30.0, 10.0} 
   };
   
   turtle(DEV_HPGL, SIZ_A3, ORI_LAND, FLG_NONE);
   
   title("TOP/LEFT", 3.75, TOP|LEFT, FLG_NONE);
   title("TOP/CENTRE", 3.75, TOP|CENTRE, FLG_NONE);
   title("TOP/RIGHT", 3.75, TOP|RIGHT, FLG_NONE);
   title("MID/LEFT", 3.75, MID|LEFT, FLG_NONE);
   title("MID/CENTRE",  3.75, MID|CENTRE, FLG_NONE);
   title("MID/RIGHT", 3.75, MID|RIGHT, FLG_NONE);
// title("http://www.dorkbot.org/dorkbotbristol/", 3.75, BOT|LEFT, FLG_NONE);
// title("Bristol Hackspace", 3.75, BOT|LEFT, FLG_NONE);
   title("BOT/LEFT", 3.75, BOT|LEFT, FLG_NONE);
   title("gypq BOT/CENTRE j690,;", 3.75, BOT|CENTRE, FLG_NONE);
   title("BOT/RIGHT", 3.75, BOT|RIGHT, FLG_NONE);
   
   colour(BLACK);
   
   pen(UP);
   forward(size);
   turn(120.0);
   pen(DOWN);
   
   for (i = 0; i < 6; i++) {
      radius = size;
      height = 0.0;
      for (j = 0; j < 3; j++) {
         forward(finger[j].hi);
         height += finger[j].hi;
         turn(60.0);
         forward(finger[j].w1);
         radius -= finger[j].w1;
         turn(120.0);
         forward(finger[j].lo);
         height -= finger[j].lo;
         turn(-120.0);
         forward(finger[j].w2);
         radius -= finger[j].w2;
         turn(-60.0);
      }
      
      /* At line of symmetry */
      forward(radius - (2.0 * height));

      for (j = 2; j >= 0; j--) {
         turn(-60.0);
         forward(finger[j].w2);
         turn(-120.0);
         forward(finger[j].lo);
         turn(120.0);
         forward(finger[j].w1);
         turn(60.0);
         forward(finger[j].hi);
      }
      
      /* Turn for next sixth */
      turn(60.0);
   }

   show();
   
   return (0);
}
