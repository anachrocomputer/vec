/* circle_lines --- plot lines but form a circle            2019-04-14 */
/* Copyright (c) 2019 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include "hpgllib.h"

void draw_side(const double xc, const double yc, const double radius, const double maxx, const int n);
void draw3lines(const double xc, const double yc, const double x, const double y, const int up, const double maxx);

int main (int argc, char * const argv[])
{
   /* Inspired by Sol LeWitt's "Wall Drawing #370",
      via a tweet by Paul Ricards. */
   int opt;
   int side;
   int lines;
   double height;
   double xc, yc;
   double maxx, maxy;
   double left, right;
   double radius;
   
   lines = 64; /* Default for A3 */
   
   while ((opt = getopt (argc, argv, "no:p:s:t:v:")) != -1) {
      switch (opt) {
      case 's':
         if (strchr (optarg, '1'))
            lines = 128;
         else if (strchr (optarg, '2'))
            lines = 96;
         else if (strchr (optarg, '4'))
            lines = 48;
         else if (strchr (optarg, '5'))
            lines = 32;
            
      case 'n':
      case 'o':
      case 'p':
      case 't':
      case 'v':
         plotopt (opt, optarg);
         break;
      default: /* '?' */
         fprintf (stderr, "Usage: %s [-p pen] [-s <size>] [-t title]\n",
                  argv[0]);
         fprintf (stderr, "       <size> ::= A1 | A2 | A3 | A4 | A5\n");
//       exit (EXIT_FAILURE);
      }
   }

   plotbegin(0);

   getplotsize(&maxx, &maxy);
   
   xc = maxx / 2.0;
   yc = maxy / 2.0;
   height = maxy;
   radius = maxy / 2.0;
   left = xc - (height / 2.0);
   right = xc + (height / 2.0);
   
   /* Draw square border */
   rectangle(left, 0.0, right, maxy);

   /* Draw central dividing line */
   moveto(xc, 0.0);
   lineto(xc, maxy);
   
   /* Draw left & right halves of circle */
   for (side = 0; side < 2; side++)
      draw_side(xc, yc, radius, side ? left : right, lines);
   
   plotend();
   
   return (0);
}


void draw_side(const double xc, const double yc, const double radius, const double maxx, const int n)
{
   int up;
   int i;
   const double delta = (M_PI / 2.0) / (double)n;
   double x, y;
   
   up = 0;
   
   for (i = 1; i < n; i++) {
      const double theta = i * delta;
      
      x = cos(theta) * radius;
      y = sin(theta) * radius;
      
      draw3lines(xc, yc, x, y, up, maxx);
      
      up = !up;   /* Draw alternately up & down to save time */
   }
}


void draw3lines(const double xc, const double yc, const double x, const double y, const int up, const double maxx)
{
   double top, bottom;
   
   if (up) {
      top = yc + y;
      bottom = yc - y;
   }
   else {
      top = yc - y;   
      bottom = yc + y;
   }              
   
   moveto(maxx, top);
   if (maxx > xc) {
      lineto(xc + x, top);
      lineto(xc + x, bottom);
   }
   else {
      lineto(xc - x, top);
      lineto(xc - x, bottom);
   }
   lineto(maxx, bottom);
}
