/* curve_stitching --- draw curve-stitching isometric cube  2013-06-16 */
/* Copyright (c) 2013 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "hpgllib.h"


#define RADIANS  (M_PI / 180.0)

#define NPTS (20)


void draw_quad(const double x1, const double y1, const double x2, const double y2,
               const double x3, const double y3, const double x4, const double y4);
void draw_curve(const double x1, const double y1, const double x2, const double y2, const double x3, const double y3);
void drawline(const double wx1, const double wy1, const double wx2, const double wy2);


int main(int argc, char * const argv[])
{
   int opt;
   double side;
   const double c30 = cos(30.0 * RADIANS);    /* Precompute sin and cos of 30 degrees */
   const double s30 = sin(30.0 * RADIANS);
   double xc, yc;
   double x1, y1;
   double x2, y2;
   double x3, y3;
   double x4, y4;
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

   plotbegin(0);

   getplotsize(&maxx, &maxy);
   
   xc = maxx / 2.0;
   yc = maxy / 2.0;
   side = maxy / 2.0;
   
   /* Draw circular border */
   circle(xc, yc, yc);
   
   x1 = xc;
   y1 = yc;
   x2 = xc;
   y2 = yc - side;
   x3 = xc - (side * c30);
   y3 = (yc - side) + (side * s30);
   x4 = x3;
   y4 = yc + (side * s30);
   
   draw_quad(x1, y1, x2, y2, x3, y3, x4, y4);
   
   x3 = xc + (side * c30);
   x4 = x3;

   draw_quad(x1, y1, x2, y2, x3, y3, x4, y4);
   
   x1 = xc;
   y1 = yc;
   x2 = xc - (side * c30);
   y2 = yc + (side * s30);
   x3 = xc;
   y3 = yc + (side * s30 * 2.0);
   x4 = xc + (side * c30);
   y4 = y2;

   draw_quad(x1, y1, x2, y2, x3, y3, x4, y4);

   plotend();
   
   return (0);
}


/* draw_quad --- draw a four-sided curve-stitch */

void draw_quad(const double x1, const double y1, const double x2, const double y2,
               const double x3, const double y3, const double x4, const double y4)
{
   draw_curve(x1, y1, x2, y2, x3, y3);
   draw_curve(x2, y2, x3, y3, x4, y4);
   draw_curve(x3, y3, x4, y4, x1, y1);
   draw_curve(x4, y4, x1, y1, x2, y2);
}


/* draw_curve --- draw a single curve-stitch */

void draw_curve(const double x1, const double y1, const double x2, const double y2, const double x3, const double y3)
{
   int i;
   double alpha;
   double dx, dy;
   double xpt1[NPTS];
   double ypt1[NPTS];
   double xpt2[NPTS];
   double ypt2[NPTS];
   
   /* Draw axes */
   drawline(x1, y1, x2, y2);
   drawline(x2, y2, x3, y3);
   
   dx = x2 - x1;
   dy = y2 - y1;

   for (i = 1; i < NPTS; i++) {
      alpha = (1.0 / (double)NPTS) * (double)i;
      
      xpt1[i] = x1 + (dx * alpha);
      ypt1[i] = y1 + (dy * alpha);
   }

   dx = x3 - x2;
   dy = y3 - y2;
   
   for (i = 1; i < NPTS; i++) {
      alpha = (1.0 / (double)NPTS) * (double)i;
      
      xpt2[i] = x2 + (dx * alpha);
      ypt2[i] = y2 + (dy * alpha);
   }
   
   for (i = 1; i < NPTS; i++)
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
