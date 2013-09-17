/* twist --- draw multiple spirals on a common centre       2013-01-26 */
/* Copyright (c) 2013 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include "hpgllib.h"


void spiral (double cx, double cy, double radius, double ang, int n);


int main (int argc, char * const argv[])
{
/* High-Resolution Computer Graphics Using C, by Ian O. Angell, 1989.
   Exercise 1.8, page 19. */
   int opt;
   double xc, yc;
   double maxx, maxy;
   double height;
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
   
   height = maxy;

   /* Draw square border */
// rectangle (xc - (height / 2.0), 0.0, xc + (height / 2.0), maxy);
   
   radius = maxy / 2.0;
   
   circle (xc, yc, radius);

   spiral (xc, yc, radius, 0.0, 4);
   spiral (xc, yc, radius, M_PI / 2.0, 4);
   spiral (xc, yc, radius, M_PI, 4);
   spiral (xc, yc, radius, 3.0 * M_PI / 2.0, 4);
   
   plotend ();
   
   return (0);
}


void spiral (double cx, double cy, double radius, double ang, int n)
{
   double theta = ang;
   double thinc = 2.0 * M_PI / 72.0;
   double r;
   int i;
   int ptnumber = 72 * n;
   double x, y;
   
   moveto (cx, cy);
   
   for (i = 0; i < ptnumber; i++) {
      theta += thinc;
      r = (radius * i) / (double)ptnumber;
      x = (r * cos (theta)) + cx;
      y = (r * sin (theta)) + cy;
      
      lineto (x, y);
   }
}
