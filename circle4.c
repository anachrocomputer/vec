/* circle4 --- plot four drawings based on circles          2020-08-24 */
/* Copyright (c) 2020 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "hpgllib.h"


void circle4(const double x0, const double y0, const double ht);
void circlearcs(const double x0, const double y0, const double ht);
void yinyang(const double x0, const double y0, const double ht, const int n);


int main(int argc, char * const argv[])
{
   int opt;
   double xc, yc;
   double h4, w4;
   double maxx, maxy;
   double side;
   
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
   
   h4 = maxy / 4.0;
   w4 = maxx / 4.0;
   
   side = maxy / 3.0;
   
   /* Split page into quarters */
   moveto(0.0, yc);
   lineto(maxx, yc);
   moveto(xc, 0.0);
   lineto(xc, maxy);
   
   /* Draw four simple Lissajous curves */
   circle4(w4, h4, maxy / 2.0);
   circlearcs(xc + w4, h4, maxy / 2.0);
   yinyang(w4, yc + h4, maxy / 2.0, 3);
   //lissajous(xc + w4, yc + h4, side, 7.0, 9.0, 0.0, 144);
   
   plotend();
   
   return (0);
}


void circle4(const double x0, const double y0, const double ht)
{
   const double radius = ht / 5.0;
   
   circle(x0, y0, radius * sqrt(2.0));
   circle(x0, y0, radius * 2.0);
   circle(x0 - radius, y0, radius);
   circle(x0, y0 + radius, radius); 
   circle(x0, y0 - radius, radius);
   circle(x0 + radius, y0, radius);
}


void circlearcs(const double x0, const double y0, const double ht)
{
   /* Intersection of circles or arcs:
    * https://mathworld.wolfram.com/Circle-CircleIntersection.html
    */
   int i;
   const double radius = ht / 2.5;
   
   circle(x0, y0, radius);
   moveto(x0, y0 - radius);
   lineto(x0, y0 + radius);
   
   for (i = 0; i < 22; i++) {
      const double x1 = x0 - radius - (8.0 * 40.0 * (double)i);
      const double y1 = y0;
      const r = 10.0 * 40.0 * (double)(i + 1);
      
      //circle(x1, y1, r);
      
      const double d = x0 - x1;
      const double x = ((d * d) - (radius * radius) + (r * r)) / (2.0 * d);
      const double t1 = -d + radius - r;
      const double t2 = -d - radius + r;
      const double t3 = -d + radius + r;
      const double t4 =  d + radius + r;
      const double a = (1.0 / d) * sqrt(t1 * t2 * t3 * t4);
      const double arcx = x1 + x;
      const double arcy = y0 - (a / 2.0);
      const double theta = atan2(a / 2.0, x) * 2.0;
      
      //moveto(x1 + x, y0);
      //lineto(x1 + x, y0 + radius);
      
      //moveto(x0 - radius, y0 + (a / 2.0));
      //lineto(x0,          y0 + (a / 2.0));
      
      moveto(arcx, arcy);
      arc(x1, y1, theta * (180.0 / M_PI));
   }
}


void yinyang(const double x0, const double y0, const double ht, const int n)
{
   const double interior = ((double)(n - 2) * 180.0) / (double)n;
   const double delta = (2.0 * M_PI) / (double)n;
   const double r = ht / 4.0;
   const double r2 = r * sin(delta / 2.0);
   int i;
   
   // Outer circle
   circle(x0, y0, r + r2);
   
   for (i = 0; i < n; i++) {
      const double theta = delta * (double)i;
      const double x = r * cos(theta);
      const double y = r * sin(theta);
      const double arcx = x + (r2 * cos(theta));
      const double arcy = y + (r2 * sin(theta));
      
      moveto(x0 + arcx, y0 + arcy);
      arc(x0 + x, y0 + y, 180.0 + (interior / 2.0));
      circle(x0 + x, y0 + y, r2 / 3.0);
   }
   
}
