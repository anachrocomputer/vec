/* plottext --- plot a text string as wide as it will go    2012-09-09 */
/* Copyright (c) 2012 John Honniball                                   */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "turtle.h"

#define MAXX   (15970.0)

int main (int argc, const char *argv[])
{
   int len;
   double width = MAXX / 40.0;  /* Width of plotter, in mm */
   double effchars;  /* Effective chars, allowing for spacing */
   double size;      /* Text size (height) required, in mm */
   
   if (argc < 2) {
      fputs ("Usage: plottext <string>\n", stderr);
      exit (1);
   }
   
   len = strlen (argv[1]);
   effchars = ((double)len) + 0.25;
   effchars *= 1.5;  /* Char cell 1.5 times actual char width */
   size = (width / effchars) * 1.31579;
   
   turtle (DEV_HPGL, SIZ_A3, ORI_LAND, FLG_NONE);
   
   title (argv[1], size, MID|CENTRE, FLG_NONE);
   
   show ();
   
   return (0);
}
