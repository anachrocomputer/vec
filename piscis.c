/* piscis --- plot the "fish bladder", vesica piscis        2011-10-02 */
/* Copyright (c) 2011 John Honniball, Froods Software Development      */

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
   
   /* Draw four circle plots */
   plot_ll(0.0, 0.0, xc, yc, r1, r2);
   plot_lr(xc,  0.0, xc, yc, r1, r2);
   plot_ul(0.0, yc,  xc, yc, r1, r2);
   plot_ur(xc,  yc,  xc, yc, r1, r2);

   plotend();
   
   return (0);
}


void plot_ul(const double x0, const double y0, const double width, const double height, const double r1, const double r2)
{
   /* https://en.wikipedia.org/wiki/Vesica_piscis */
   const double xc = x0 + (width / 2.0);
   const double yc = y0 + (height / 2.0);
   const double r = r2 * 1.1;
   const double d = r2 / 2.0;
   
   /* Two overlapping circles */
   circle(xc - d, yc, r2);
   circle(xc + d, yc, r2);
   
   /* Round-ended oval around vesica piscis */
   moveto(xc - d, yc - r);
   lineto(xc + d, yc - r);
   arc(xc + d, yc, 180.0);
   lineto(xc - d, yc + r);
   arc(xc - d, yc, 180.0);
}


void plot_ur(const double x0, const double y0, const double width, const double height, const double r1, const double r2)
{
   const double xc = x0 + (width / 2.0);
   const double yc = y0 + (height / 2.0);
   
   reuleaux(xc, yc, r2, 3, true);
   reuleaux(xc, yc, r2 * 1.1, 3, false);
}


void plot_ll(const double x0, const double y0, const double width, const double height, const double r1, const double r2)
{
   /* https://en.wikipedia.org/wiki/Six-petal_rosette */
   int i;
   const double delta = (2.0 * M_PI) / 6.0;
   const double xc = x0 + (width / 2.0);
   const double yc = y0 + (height / 2.0);
   double x[7];
   double y[7];
   
   circle(xc, yc, r2);
   
   for (i = 0; i <= 6; i++) {
      const double theta = delta * (double)i;

      x[i] = xc + (r2 * cos(theta));
      y[i] = yc + (r2 * sin(theta));
   }
      
   for (i = 0; i < 6; i++) {
      moveto(x[i], y[i]);
      arc(x[i + 1], y[i + 1], -120.0);
   }

   circle(xc, yc, r2 * 1.1);
}


void plot_lr(const double x0, const double y0, const double width, const double height, const double r1, const double r2)
{
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
