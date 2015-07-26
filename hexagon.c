/* hexagon --- plot a hexagon as a basis for a snowflake    2009-11-12 */
/* Copyright (c) 2009 John Honniball, Froods Software Development      */

#include <stdio.h>
#include "turtle.h"

int main (int argc, const char *argv[])
{
   int i;
   double size = 80.0;  /* 80mm sides */
   
   turtle (DEV_HPGL, SIZ_A4, ORI_LAND, FLG_NONE);
   
   colour (BLACK);
   
   pen (UP);
   forward (size);
   turn (120.0);
   pen (DOWN);
   
   for (i = 0; i < 6; i++) {
      forward (size);
      turn (60.0);
   }

   show ();
   
   return (0);
}
