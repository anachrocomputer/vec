/* fraserspiral --- plot the Fraser Spiral optical illusion 2013-05-16 */
/* Copyright (c) 2013 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "hpgllib.h"

#define DEGREES   (180.0 / M_PI)

void ringoshapes(const double x0, const double y0, const double radius, const int nshapes);


int main(int argc, char * const argv[])
{
   int opt;
   int i;
   double xc, yc;
   double maxx, maxy;
   double height;
   double radius;
   double r;
   
   while ((opt = getopt(argc, argv, "no:p:s:t:v:")) != -1) {
      switch (opt) {
      case 'n':
      case 'o':
      case 'p':
      case 's':
      case 't':
      case 'v':
         plotopt(opt, optarg);
         break;
      default: /* '?' */
         fprintf(stderr, "Usage: %s [-p pen] [-s <size>] [-t title]\n", argv[0]);
         fprintf(stderr, "       <size> ::= A1 | A2 | A3 | A4 | A5\n");
         exit(EXIT_FAILURE);
      }
   }

   if (plotbegin(0) < 0) {
      fputs("Failed to initialise HPGL library\n", stderr);
      exit(EXIT_FAILURE);
   }

   getplotsize(&maxx, &maxy);
   
   xc = maxx / 2.0;
   yc = maxy / 2.0;
   
   height = maxy;

   /* Draw circular border */
   circle(xc, yc, yc);

   radius = height / 24.0;
   
   /* Draw eleven concentric circles of shapes */
   for (i = 0; i < 11; i++) {
      r = radius * (i + 1);
      ringoshapes(xc, yc, r, 16 + i);
   }
   
   plotend();
   
   return (0);
}


void ringoshapes(const double x0, const double y0, const double radius, const int nshapes)
{
   int i;
   double x, y;
   double xc, yc;
   const double d = radius / 4.0;
   const double delta = 2.0 * M_PI / (double)nshapes;
   
   for (i = 0; i < nshapes; i++) {
      const double theta = i * delta;
      
      x = (radius * cos(theta)) + x0;
      y = (radius * sin(theta)) + y0;
   
      moveto(x, y);
      
      xc = (d * cos(theta + (M_PI / 2.0))) + x0;
      yc = (d * sin(theta + (M_PI / 2.0))) + y0;

      arc(xc, yc, (delta * 1.2) * DEGREES);
   }
}
