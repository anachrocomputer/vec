/* frustum --- plot the net of a frustum as a container     2020-09-06 */
/* Copyright (c) 2020 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include "hpgllib.h"

struct Trapezoid {
   double angle;
   double x1, y1;
   double x2, y2;
   double x3, y3;
   double x4, y4;
   double xc, yc;
   double xt, yt, r;
};

void rotate(double *x, double *y, const double st, const double ct);

int main(int argc, char * const argv[])
{
   int opt;
   int i;
   double xc, yc;
   double maxx, maxy;
   int nsides = 4;
   double delta;
   struct Trapezoid t[32];
   double base;
   double height;
   double top;
   
   while ((opt = getopt(argc, argv, "no:p:s:t:v:")) != -1) {
      switch (opt) {
      case 's':
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

   if (plotbegin(1) < 0) {
      fputs("Failed to initialise HPGL library\n", stderr);
      exit(EXIT_FAILURE);
   }

   getplotsize(&maxx, &maxy);
   
   xc = maxx / 2.0;
   yc = maxy / 2.0;
   
   base = maxy / 6.0;
   height = maxy / 4.0;
   top = base * 1.5;
   
   delta = (2.0 * M_PI) / (double)nsides;
   
   for (i = 0; i < nsides; i++) {
      const double theta = delta * (double)i;
      const double st = sin(theta);
      const double ct = cos(theta);
      
      t[i].angle = theta;
      
      t[i].x1 = ((base / 2.0) / tan(delta / 2.0));
      t[i].y1 = -(base / 2.0);
      
      t[i].x2 = t[i].x1 + height;
      t[i].y2 = -(top / 2.0);
      
      t[i].x3 = t[i].x2;
      t[i].y3 = top / 2.0;
      
      t[i].x4 = t[i].x1;
      t[i].y4 = base / 2.0;
      
      t[i].xt = t[i].x2;
      t[i].yt = 0.0;
      t[i].r = top / 2.0;
      
      t[i].xc = t[i].x1 + (height / 2.0);
      t[i].yc = 0.0;
      
      rotate(&t[i].x1, &t[i].y1, st, ct);
      rotate(&t[i].x2, &t[i].y2, st, ct);
      rotate(&t[i].x3, &t[i].y3, st, ct);
      rotate(&t[i].x4, &t[i].y4, st, ct);
      rotate(&t[i].xc, &t[i].yc, st, ct);
      rotate(&t[i].xt, &t[i].yt, st, ct);
      
      t[i].x1 += xc;
      t[i].y1 += yc;
      
      t[i].x2 += xc;
      t[i].y2 += yc;
      
      t[i].x3 += xc;
      t[i].y3 += yc;
      
      t[i].x4 += xc;
      t[i].y4 += yc;
      
      t[i].xt += xc;
      t[i].yt += yc;
      
      t[i].xc += xc;
      t[i].yc += yc;
   }
   
   // Draw trapezoids
   for (i = 0; i < nsides; i++) {
      moveto(t[i].x1, t[i].y1);
      lineto(t[i].x2, t[i].y2);
      lineto(t[i].x3, t[i].y3);
      lineto(t[i].x4, t[i].y4);
      lineto(t[i].x1, t[i].y1);
      
      moveto(t[i].x2, t[i].y2);
      arc(t[i].xt, t[i].yt, 180.0);
      
      ellipse(t[i].xc, t[i].yc, base / 1.6, base / 2.0, t[i].angle);
   }
   
   // Draw fold lines
   for (i = 0; i < nsides; i++) {
      const int j = (i + 1) % nsides;
      const double midx = (t[i].x3 + t[j].x2) / 2.0;
      const double midy = (t[i].y3 + t[j].y2) / 2.0;
      
      moveto(t[i].x3, t[i].y3);
      lineto(t[j].x2, t[j].y2);
      
      moveto(t[i].x4, t[i].y4);
      lineto(midx, midy);
   }

   plotend();
   
   return (0);
}

void rotate(double *x, double *y, const double st, const double ct)
{
   const double newx = (*x * ct) - (*y * st);
   const double newy = (*x * st) + (*y * ct);
   
   *x = newx;
   *y = newy;
}
