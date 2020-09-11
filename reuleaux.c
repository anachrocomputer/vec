/* reuleaux --- draw some Reuleaux polygons                 2020-09-05 */
/* Copyright (c) 2020 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>
#include "hpgllib.h"

void plot_ul(const double x0, const double y0, const double width, const double height, const double r1, const double r2);
void plot_ur(const double x0, const double y0, const double width, const double height, const double r1, const double r2);
void plot_ll(const double x0, const double y0, const double width, const double height, const double r1, const double r2);
void plot_lr(const double x0, const double y0, const double width, const double height, const double r1, const double r2);
void reuleaux(const double xc, const double yc, const double r, const int n, const bool drawSides);

int main(int argc, char * const argv[])
{
   int opt;
   double xc, yc;
   double maxx, maxy;
   double r1;  /* Radius of smaller arcs */
   double r2;  /* Radius of larger arc */
   
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

   /* Select first pen and draw border */
   if (plotbegin(1) < 0) {
      fputs("Failed to initialise HPGL library\n", stderr);
      exit(EXIT_FAILURE);
   }

   getplotsize(&maxx, &maxy);
   
   xc = maxx / 2.0;
   yc = maxy / 2.0;
   
   r1 = maxx / 5.0;
   r2 = maxy / 5.0;
   
   /* Split page into quarters */
   moveto(0.0, yc);
   lineto(maxx, yc);
   moveto(xc, 0.0);
   lineto(xc, maxy);
   
   /* Draw four Reuleaux polygons */
   plot_ll(0.0, 0.0, xc, yc, r1, r2);
   plot_lr(xc,  0.0, xc, yc, r1, r2);
   plot_ul(0.0, yc,  xc, yc, r1, r2);
   plot_ur(xc,  yc,  xc, yc, r1, r2);

   plotend();
   
   return (0);
}


void plot_ul(const double x0, const double y0, const double width, const double height, const double r1, const double r2)
{
   const double xc = x0 + (width / 2.0);
   const double yc = y0 + (height / 2.0);
   
   reuleaux(xc, yc, r2, 3, true);
   reuleaux(xc, yc, r2 * 1.15, 3, false);
}


void plot_ur(const double x0, const double y0, const double width, const double height, const double r1, const double r2)
{
   const double xc = x0 + (width / 2.0);
   const double yc = y0 + (height / 2.0);
   
   reuleaux(xc, yc, r2, 5, true);
   reuleaux(xc, yc, r2 * 1.15, 5, false);
}


void plot_ll(const double x0, const double y0, const double width, const double height, const double r1, const double r2)
{
   const double xc = x0 + (width / 2.0);
   const double yc = y0 + (height / 2.0);
   
   reuleaux(xc, yc, r2, 7, true);
   reuleaux(xc, yc, r2 * 1.15, 7, false);
}


void plot_lr(const double x0, const double y0, const double width, const double height, const double r1, const double r2)
{
   const double xc = x0 + (width / 2.0);
   const double yc = y0 + (height / 2.0);
   
   reuleaux(xc, yc, r2, 9, true);
   reuleaux(xc, yc, r2 * 1.15, 9, false);
}


void reuleaux(const double xc, const double yc, const double r, const int n, const bool drawSides)
{
   /* https://en.wikipedia.org/wiki/Reuleaux_triangle */
   int i;
   const double delta = (2.0 * M_PI) / (double)n;
   const double degrees = 180.0 / (double)n;
   double x[32];
   double y[32];
   
   for (i = 0; i <= n; i++) {
      const double theta = (delta * (double)i) + (M_PI / 2.0);
      x[i] = xc + (r * cos(theta));
      y[i] = yc + (r * sin(theta));
   }
      
   for (i = 0; i < n; i++) {
      const int j = (i + (n / 2)) % n;

      moveto(x[i], y[i]);
      arc(x[j], y[j], -degrees);
   }

   if (drawSides) {
      for (i = 0; i <= n; i++) {
         if (i == 0)
            moveto(x[i], y[i]);
         else
            lineto(x[i], y[i]);
      }
   }
}
