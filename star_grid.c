/* star_grid --- plot a grid of stars with offset centres   2019-04-16 */
/* Copyright (c) 2019 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include "hpgllib.h"

void draw_star(const double x0, const double y0, const double gridsz, const double xc, const double yc, const int n);

int main(int argc, char * const argv[])
{
   /* Inspired by Paul Rickards' tweet:
      https://twitter.com/paulrickards/status/1096544602050514944
    */
   int opt;
   int i, j;
   int ncells;
   double height;
   double xc;
   double maxx, maxy;
   double left, right;
   double gridsz;
   
   ncells = 7; /* Default for A3 */
   
   while ((opt = getopt(argc, argv, "no:p:s:t:v:")) != -1) {
      switch (opt) {
      case 's':
         if (strchr(optarg, '1'))
            ncells = 15;
         else if (strchr(optarg, '2'))
            ncells = 9;
         else if (strchr(optarg, '4'))
            ncells = 7;
         else if (strchr(optarg, '5'))
            ncells = 7;
            
      case 'n':
      case 'o':
      case 'p':
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

   plotbegin(0);

   getplotsize(&maxx, &maxy);
   
   xc = maxx / 2.0;
   height = maxy;
   gridsz = maxy / (double)ncells;
   left = xc - (height / 2.0);
   right = xc + (height / 2.0);
   
   /* Draw square border */
   rectangle(left, 0.0, right, maxy);

   /* Draw grid */
   for (i = 1; i < ncells; i++) {
      const double x = left + (i * gridsz);
      
      moveto(x, 0.0);
      lineto(x, maxy);
   }
   
   for (i = 1; i < ncells; i++) {
      const double y = i * gridsz;
      
      moveto(left, y);
      lineto(right, y);
   }
   
   /* Draw stars */
   for (i = 0; i < ncells; i++) {
      for (j = 0; j < ncells; j++) {
         const double x0 = left + (i * gridsz);
         const double y0 = j * gridsz;
         const double xc = x0 + ((gridsz * (i + 1)) / (double)(ncells + 1));
         const double yc = y0 + ((gridsz * (j + 1)) / (double)(ncells + 1));
         
         draw_star(x0, y0, gridsz, xc, yc, 9);
      }
   }
   
   plotend();
   
   return (0);
}


void draw_star(const double x0, const double y0, const double gridsz, const double xc, const double yc, const int n)
{
   int i;
   const double d = gridsz / (double)(n - 1);
   
   for (i = 0; i < (n / 2); i++) {
      const double x1 = x0 + (i * d * 2.0);
      const double x2 = x1 + d;

      moveto(x1, y0);
      lineto(xc, yc);
      lineto(x2, y0);
   }
   
   for (i = 0; i < (n / 2); i++) {
      const double y1 = y0 + (i * d * 2.0);
      const double y2 = y1 + d;

      moveto(x0 + gridsz, y1);
      lineto(xc, yc);
      lineto(x0 + gridsz, y2);
   }
   
   for (i = 0; i < (n / 2); i++) {
      const double x1 = x0 + gridsz - (i * d * 2.0);
      const double x2 = x1 - d;

      moveto(x1, y0 + gridsz);
      lineto(xc, yc);
      lineto(x2, y0 + gridsz);
   }
   
   for (i = 0; i < (n / 2); i++) {
      const double y1 = y0 + gridsz - (i * d * 2.0);
      const double y2 = y1 - d;

      moveto(x0, y1);
      lineto(xc, yc);
      lineto(x0, y2);
   }
}
