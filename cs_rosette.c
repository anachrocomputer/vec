/* cs_rosette --- draw curve-stitching rosette              2013-06-22 */
/* Copyright (c) 2013 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "hpgllib.h"


void draw_curve(const double x1, const double y1, const double x2, const double y2,
                const double x3, const double y3, const int nlines);
void drawline(const double wx1, const double wy1, const double wx2, const double wy2);


#define MAXLINES (20)

struct vector {
   double x;
   double y;
};

int main(int argc, char * const argv[])
{
   int opt;
   int i, j, k;
   int j1, k1;
   int nrings, nsectors, nlines;
   struct vector v1, v2;
   double side;
   double delta, theta;
   double xc, yc;
   double x1, y1;
   double x2, y2;
   double x3, y3;
   double maxx, maxy;
   
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

   /* Basic drawing parameters */
   nsectors = 6;
   nrings = 4;
   nlines = 16;
   
   plotbegin(0);

   getplotsize(&maxx, &maxy);
   
   xc = maxx / 2.0;
   yc = maxy / 2.0;
   side = maxy / 2.0;
   
   /* Draw square border */
   rectangle(xc - (maxy / 2.0), 0.0, xc + (maxy / 2.0), maxy);
   
   /* Draw radials */
   delta = (2.0 * M_PI) / (double)nsectors;
   
   for (i = 0; i < nsectors; i++) {
      theta = (double)i * delta;
      
      x1 = xc + (side * cos(theta));
      y1 = yc + (side * sin(theta));
      
      drawline(xc, yc, x1, y1);
   }
   
   for (i = 0; i < nsectors; i++) {
      theta = (double)i * delta;

      /* Compute vectors, length side/nrings */
      v1.x = (side / (double)nrings) * cos(theta);
      v1.y = (side / (double)nrings) * sin(theta);
      v2.x = (side / (double)nrings) * cos(theta + delta);
      v2.y = (side / (double)nrings) * sin(theta + delta);
      
      /* Draw lines parallel to v2 */
      for (j = 1; j < nrings; j++) {
         k = nrings - j;

         x1 = xc + (double)j * v1.x;
         y1 = yc + (double)j * v1.y;
         x2 = x1 + ((double)k * v2.x);
         y2 = y1 + ((double)k * v2.y);
         
         drawline(x1, y1, x2, y2);
      }

      /* Draw lines parallel to v1 */
      for (j = 1; j < nrings; j++) {
         k = nrings - j;

         x1 = xc + (double)j * v2.x;
         y1 = yc + (double)j * v2.y;
         x2 = x1 + ((double)k * v1.x);
         y2 = y1 + ((double)k * v1.y);
         
         drawline(x1, y1, x2, y2);
      }
   }
   
   /* Loop around the n-agon */
   for (i = 0; i < nsectors; i++) {
      theta = (double)i * delta;
      
      /* Vectors of length side/nrings again */
      v1.x = (side / (double)nrings) * cos(theta);
      v1.y = (side / (double)nrings) * sin(theta);
      v2.x = (side / (double)nrings) * cos(theta + delta);
      v2.y = (side / (double)nrings) * sin(theta + delta);
      
      for (j = 0; j < nrings; j++) {
         for (k = 0; k < (nrings - j); k++) {                           
            /* Draw curves with vertex nearest centre */
            x2 = xc + ((double)j * v1.x) + ((double)k * v2.x);
            y2 = yc + ((double)j * v1.y) + ((double)k * v2.y);
            x1 = x2 + v1.x;
            y1 = y2 + v1.y;
            x3 = x2 + v2.x;
            y3 = y2 + v2.y;
           
            draw_curve(x1, y1, x2, y2, x3, y3, nlines);
           
            j1 = j + 1;
            k1 = k + 1;
           
            /* If there's room, draw curve with vertex away from centre */
            if ((j1 < nrings) && (k1 < (nrings - j))) {
               x2 = xc + ((double)j1 * v1.x) + ((double)k1 * v2.x);
               y2 = yc + ((double)j1 * v1.y) + ((double)k1 * v2.y);
               x1 = x2 - v1.x;
               y1 = y2 - v1.y;
               x3 = x2 - v2.x;
               y3 = y2 - v2.y;
              
               draw_curve(x1, y1, x2, y2, x3, y3, nlines);
            }
         }
      }
   }

   plotend();
   
   return (0);
}


/* draw_curve --- draw a single curve-stitch */

void draw_curve(const double x1, const double y1, const double x2, const double y2,
                const double x3, const double y3, const int nlines)
{
   int i;
   double alpha;
   double dx, dy;
   double xpt1[MAXLINES];
   double ypt1[MAXLINES];
   double xpt2[MAXLINES];
   double ypt2[MAXLINES];
   
   dx = x2 - x1;
   dy = y2 - y1;
   
   for (i = 1; i < nlines; i++) {
      alpha = (1.0 / (double)nlines) * (double)i;
      
      xpt1[i] = x1 + (dx * alpha);
      ypt1[i] = y1 + (dy * alpha);
   }

   dx = x3 - x2;
   dy = y3 - y2;
   
   for (i = 1; i < nlines; i++) {
      alpha = (1.0 / (double)nlines) * (double)i;
      
      xpt2[i] = x2 + (dx * alpha);
      ypt2[i] = y2 + (dy * alpha);
   }
   
   for (i = 1; i < nlines; i++)
      drawline(xpt1[i], ypt1[i], xpt2[i], ypt2[i]);
}


void drawline(const double wx1, const double wy1, const double wx2, const double wy2)
{
   static int cx = -32767, cy = -32767;
   static double wcx = -1.0, wcy = -1.0;
   int x1, y1, x2, y2;
   const double dx1 = wx1 - wcx;
   const double dy1 = wy1 - wcy;
   const double dx2 = wx2 - wcx;
   const double dy2 = wy2 - wcy;
   const double d1 = sqrt((dx1 * dx1) + (dy1 * dy1));
   const double d2 = sqrt((dx2 * dx2) + (dy2 * dy2));
   char hpgl[32];
   
   if (d1 < d2) {
      x1 = getdevx(wx1);
      y1 = getdevy(wy1);
      x2 = getdevx(wx2);
      y2 = getdevy(wy2);
      wcx = wx2;
      wcy = wy2;
   }
   else {
      x1 = getdevx(wx2);
      y1 = getdevy(wy2);
      x2 = getdevx(wx1);
      y2 = getdevy(wy1);
      wcx = wx1;
      wcy = wy1;
   }
   
   if ((cx != x1) || (cy != y1)) {
      snprintf(hpgl, sizeof (hpgl), "\nPU;PA%d,%d;", x1, y1);
      hpglout(hpgl);
   }
      
   snprintf(hpgl, sizeof (hpgl), "PD;PA%d,%d;", x2, y2);
   hpglout(hpgl);
   
   cx = x2;
   cy = y2;
}
