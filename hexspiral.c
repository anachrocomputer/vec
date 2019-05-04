/* hexspiral --- plot a hexagonal spiral                    2019-05-04 */
/* Copyright (c) 2019 John Honniball, Froods Software Development      */

#include <stdio.h>
#include "turtle.h"

int main(int argc, const char *argv[])
{
   int i;
   
   turtle(DEV_HPGL, SIZ_A4, ORI_LAND, FLG_BORD);
   
   colour(BLACK);
   
   pen(DOWN);
   
   for (i = 1; i < 360; i++) {
      forward((double)i / 4.0);
      turn(59.0);
   }

   show();
   
   return (0);
}
