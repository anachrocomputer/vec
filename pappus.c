/* pappus --- draw the Pappus Chain on the plotter          2011-06-01 */
/* Copyright (c) 2011 John Honniball, Dorkbot Bristol                  */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "hpgllib.h"

double pappus_circle (double x0, double y0, double x, double y, double rad);

double Rad;

int main (int argc, char * const argv[])
{
   int opt;
   double xc, yc;
   double maxx, maxy;
   double x, y;
   double r;
   double r2;
   double rad;
   double csize;
   int n;
   int n2;
   
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
//       exit (EXIT_FAILURE);
      }
   }

   plotbegin (0);

   getplotsize (&maxx, &maxy);
   
   xc = maxx / 2.0;
   yc = maxy / 2.0;
   
   Rad = yc;
   
   /* Just draw with pen 1 for now. Maybe change pens half-way */
   pencolr (0);

   /* Draw outermost circle as large as will fit on a page */
   circle (xc, yc, Rad);
   
   /* For an explanation of the mathematics of the Pappus
    * Chain, see Wikipedia: http://en.wikipedia.org/wiki/Pappus_chain
    */

// r = 2.0 / 3.0;
   r = 1.0 / 2.0;
   n = 0;
   
   /* pappus_circle ((1.0 - r) / 2.0, 0.0, (1.0 - r) / 2.0); */
   
   do {
      n2 = n * n;
      r2 = (1.0 - r) * (1.0 - r);

      x = (r * (1.0 + r)) / (2.0 * ((n2 * r2) + r));
      y = (n * r * (1.0 - r)) / ((n2 * r2) + r);
      rad = ((1.0 - r) * r) / (2.0 * ((n2 * r2) + r));
      
//    fprintf (stderr, "n = %d, x = %f, y = %f, rad = %f\n", n, x, y, rad);
      
      csize = pappus_circle (xc, yc, x * maxy, y * maxy, rad * maxy);
      
      n++;
   } while (csize > 1.5);
   
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
      
//    fprintf (stderr, "n = %d, x = %f, y = %f, rad = %f\n", n, x, y, rad);
      
      csize = pappus_circle (xc, yc, (1.0 - x) * maxy, -y * maxy, rad * maxy);
      
      n++;
   } while (csize > 1.5);
   
   plotend ();

   exit (0);
}


double pappus_circle (double x0, double y0, double x, double y, double rad)
{
   x += x0 - Rad;
   y += y0;
   
   circle (x, y, rad);
   
   return ((rad * 2.0) / 40.0);
}
