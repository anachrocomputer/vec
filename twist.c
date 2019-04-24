/* twist --- draw multiple spirals on a common centre       2013-01-26 */
/* Copyright (c) 2013 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include "hpgllib.h"


void spiral (const double xc, const double yc, const double r1, const double r2, const double ang, const int n);


int main (int argc, char * const argv[])
{
/* High-Resolution Computer Graphics Using C, by Ian O. Angell, 1989.
   Exercise 1.8, page 19. */
   int opt;
   double xc, yc;
   double maxx, maxy;
   double radius;
   
   while ((opt = getopt (argc, argv, "no:p:s:t:v:")) != -1) {
      switch (opt) {
      case 'n':
      case 'o':
      case 'p':
      case 's':
      case 't':
      case 'v':
         plotopt (opt, optarg);
         break;
      default: /* '?' */
         fprintf (stderr, "Usage: %s [-p pen] [-s <size>] [-t title]\n",
                  argv[0]);
         fprintf (stderr, "       <size> ::= A1 | A2 | A3 | A4 | A5\n");
         exit (EXIT_FAILURE);
      }
   }

   plotbegin (0);

   getplotsize (&maxx, &maxy);
   
   xc = maxx / 2.0;
   yc = maxy / 2.0;
   
   radius = maxy / 2.0;
   
   circle (xc, yc, radius);

   spiral (xc, yc, 0.0, radius, 0.0, 4);
   spiral (xc, yc, 0.0, radius, M_PI / 2.0, 4);
   spiral (xc, yc, 0.0, radius, M_PI, 4);
   spiral (xc, yc, 0.0, radius, 3.0 * M_PI / 2.0, 4);
   
   plotend ();
   
   return (0);
}


/* spiral --- draw a spiral given angle and number of turns */

void spiral (const double xc, const double yc, const double r1, const double r2, const double ang, const int n)
{
   const double delta = (2.0 * M_PI) / 72.0;
   int i;
   const double dr = (r2 - r1) / (72.0 * n);
   
   openlinesequence ((r1 * cos (ang)) + xc, (r1 * sin (ang)) + yc);
   
   for (i = 0; i < (72 * n); i++) {
      const double theta = ang + (delta * i);
      const double r = r1 + (dr * i);
      const double x = (r * cos (theta)) + xc;
      const double y = (r * sin (theta)) + yc;
      
      linesegmentto (x, y);
   }
   
   closelinesequence (0);
}
