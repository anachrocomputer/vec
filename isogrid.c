/* isogrid --- plot a grid of equilateral triangles         2018-06-18 */
/* Copyright (c) 2018 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include "hpgllib.h"

#define RADIANS  (M_PI / 180.0)


void isogrid(const double xc, const double yc, const int npts, const double pitch);
void drawlines(const double xc, const double yc, const int npts, const double pitch, const double theta, const int nodeflag);
void drawoneline(const double xc, const double yc, const double len, const double y, const double theta);
void drawlinesegs(const double xc, const double yc, const double pitch, const int nsegs, const double gap, const double y, const double theta, const int nodeflag);


int main(int argc, char * const argv[])
{
   int opt;
   double xc, yc;
   double maxx, maxy;
   
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
   
   isogrid(xc, yc, 9, 24.0 * 40.0);

   plotend();
   
   return (0);
}

   
void isogrid(const double xc, const double yc, const int npts, const double pitch)
{
   int i;
   double theta;
   
   for (i = 0; i < 3; i++) {
      theta = (i * 60) * RADIANS;
      drawlines(xc, yc, npts, pitch, theta, i == 0);
   }
}


void drawlines(const double xc, const double yc, const int npts, const double pitch, const double theta, const int nodeflag)
{
   const double s60 = sin(60.0 * RADIANS);
   const double ystep = s60 * pitch;
   int i;
   
   for (i = -(npts / 2); i <= (npts / 2); i++) {
      double nsegs;
      
      if (i < 0)
         nsegs = npts - (-i + 1);
      else
         nsegs = npts - (i + 1);

//    drawoneline(xc, yc, nsegs * pitch, i * ystep, theta);
      drawlinesegs(xc, yc, pitch, nsegs, 8.0 * 40.0, i * ystep, theta, nodeflag);
   }
}

void drawoneline(const double xc, const double yc, const double len, const double y, const double theta)
{
   const double s = sin(theta);
   const double c = cos(theta);
   const double halflen = len / 2.0;
   double x1, y1; 
   double x2, y2;
   double rx1, ry1; 
   double rx2, ry2;

   x1 = -halflen;
   y1 = y;
   x2 = halflen;
   y2 = y;
   
   rx1 = (x1 * c) - (y1 * s);
   ry1 = (x1 * s) + (y1 * c);
   
   rx2 = (x2 * c) - (y2 * s);
   ry2 = (x2 * s) + (y2 * c);
   
   moveto(xc + rx1, yc + ry1);
   lineto(xc + rx2, yc + ry2);
}

void drawlinesegs(const double xc, const double yc, const double pitch, const int nsegs, const double gap, const double y, const double theta, const int nodeflag)
{
   const double s = sin(theta);
   const double c = cos(theta);
   const double len = pitch - gap;
   const double radius = gap / 2.0;
   double x1, y1; 
   double x2, y2;
   double x3, y3;
   double rx1, ry1; 
   double rx2, ry2;
   double rx3, ry3;
   int i;
   
   for (i = 0; i < nsegs; i++) {
      const double xoff = (nsegs / 2.0) * pitch;
      
      x1 = (i * pitch) + radius - xoff;
      y1 = y;
      x2 = x1 + len;
      y2 = y;
      x3 = x1 - radius;
      y3 = y;
      
      rx1 = (x1 * c) - (y1 * s);
      ry1 = (x1 * s) + (y1 * c);
      
      rx2 = (x2 * c) - (y2 * s);
      ry2 = (x2 * s) + (y2 * c);
      
      if (nodeflag) {
         rx3 = (x3 * c) - (y3 * s);
         ry3 = (x3 * s) + (y3 * c);
         
         circle(xc + rx3, yc + ry3, radius);
      }

      moveto(xc + rx1, yc + ry1);
      lineto(xc + rx2, yc + ry2);
   }

   if (nodeflag) {
      x3 = x1 + len + radius;
      y3 = y;

      rx3 = (x3 * c) - (y3 * s);
      ry3 = (x3 * s) + (y3 * c);

      circle(xc + rx3, yc + ry3, radius);
   }
}
