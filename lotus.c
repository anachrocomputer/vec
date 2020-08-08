/* lotus --- draw lotus flower pattern                      2013-07-25 */
/* Copyright (c) 2013 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "hpgllib.h"


double zigzagring(const double x0, const double y0, const double r1, const double r2, const int npts, const int incr, const int flag);


int main(int argc, char * const argv[])
{
/* Mosaic and Tessellated Patterns, by John Willson, 1983.
   ISBN: 0-486-24379-6. Plate 23. */
   int i;
   int opt;
   double xc, yc;
   double scale = 40.0;
   double maxx, maxy;
   double radius;
   double incrad;
   
   while ((opt = getopt(argc, argv, "no:p:s:t:v:")) != -1) {
      switch (opt) {
      case 's':
         if (strchr(optarg, '1'))
            scale = 80.0;
         else if (strchr(optarg, '2'))
            scale = 56.57;
         else if (strchr(optarg, '4'))
            scale = 28.28;
         else if (strchr(optarg, '5'))
            scale = 20.0;
            
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
         exit (EXIT_FAILURE);
      }
   }

   plotbegin(0);

   getplotsize(&maxx, &maxy);
   
   xc = maxx / 2.0;
   yc = maxy / 2.0;
   
   /* Draw circular border */
   circle(xc, yc, yc);
   
   radius = 15.0 * scale;  /* Initial radius 15mm */
   incrad = 15.0 * scale;  /* Each zig-zag 15mm bigger than the previous one */

   for (i = 0; i < 8; i++)
      radius = zigzagring(xc, yc, radius, radius + incrad, 18, 1, i & 1);
   
   plotend();
   
   return (0);
}


double zigzagring(const double x0, const double y0, const double r1, const double r2, const int npts, const int incr, const int flag)
{
   int i;
   double x1[128], y1[128];
   double x2[128], y2[128];
   double theta1, theta2;
   int n1, n2;
   const double delta = (2.0 * M_PI) / (double)npts;
   
   for (i = 0; i < npts; i++) {
      if (flag) {
         theta2 = delta * (double)i;
         theta1 = (delta * (double)i) - (delta / 2.0);
      }
      else {
         theta1 = delta * (double)i;
         theta2 = (delta * (double)i) + (delta / 2.0);
      }
      
      x1[i] = (cos(theta1) * r1) + x0;
      y1[i] = (sin(theta1) * r1) + y0;
      
      x2[i] = (cos(theta2) * r2) + x0;    
      y2[i] = (sin(theta2) * r2) + y0;
   }
   
   moveto(x1[0], y1[0]);
   n1 = 0;
   n2 = incr / 2;
   
   for (i = 0; i < npts; i++) {
      lineto(x1[n1], y1[n1]);
      lineto(x2[n2], y2[n2]);
      
      n1 = (n1 + incr) % npts;
      n2 = (n2 + incr) % npts;
   }
   
   lineto(x1[0], y1[0]);
           
   return (r2);
}
