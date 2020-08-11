/* pappus --- draw the Pappus Chain on the plotter          2011-06-01 */
/* Copyright (c) 2011 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "hpgllib.h"

double pappus_circle(const double x0, const double y0, double x, double y, const double rad);

double Ycentre;

int main(int argc, char * const argv[])
{
   int opt;
   double xc, yc;
   double maxx, maxy;
   double x, y;
   double r;
   double r2;
   double rad;
   double csize_mm;
   int n;
   int n2;
   
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
   
   Ycentre = yc;
   
   /* Just draw with pen 1 for now. Maybe change pens half-way */
   pencolr(0);

   /* Draw outermost circle as large as will fit on a page */
   circle(xc, yc, yc);
   
   /* For an explanation of the mathematics of the Pappus
    * Chain, see Wikipedia: http://en.wikipedia.org/wiki/Pappus_chain
    */

   r = 1.0 / 2.0;
   n = 0;
   
   do {
      n2 = n * n;
      r2 = (1.0 - r) * (1.0 - r);

      x = (r * (1.0 + r)) / (2.0 * ((n2 * r2) + r));
      y = (n * r * (1.0 - r)) / ((n2 * r2) + r);
      rad = ((1.0 - r) * r) / (2.0 * ((n2 * r2) + r));
      
//    fprintf(stderr, "n = %d, x = %f, y = %f, rad = %f\n", n, x, y, rad);
      
      csize_mm = pappus_circle(xc, yc, x * maxy, y * maxy, rad * maxy);
      
      n++;
   } while (csize_mm > 1.5);
   
   /* Again, with the circles reflected. Repeating the entire sequence
    * results in less wasted pen movement and hence faster plotting
    */
   r = 1.0 / 2.0;
   n = 0;
   
   do {
      n2 = n * n;
      r2 = (1.0 - r) * (1.0 - r);

      x = (r * (1.0 + r)) / (2.0 * ((n2 * r2) + r));
      y = (n * r * (1.0 - r)) / ((n2 * r2) + r);
      rad = ((1.0 - r) * r) / (2.0 * ((n2 * r2) + r));
      
//    fprintf(stderr, "n = %d, x = %f, y = %f, rad = %f\n", n, x, y, rad);
      
      csize_mm = pappus_circle(xc, yc, (1.0 - x) * maxy, -y * maxy, rad * maxy);
      
      n++;
   } while (csize_mm > 1.5);
   
   plotend();

   exit (0);
}


double pappus_circle(const double x0, const double y0, double x, double y, const double rad)
{
   x += x0 - Ycentre;
   y += y0;
   
   circle(x, y, rad);
   
   return ((rad * 2.0) / 40.0);
}
