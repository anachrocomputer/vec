/* a6cardlabels --- plot labelling for A6 postcards         2020-08-23 */
/* Copyright (c) 2020 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include "hpgllib.h"

void postcard_port(const double x, const double y, const double wd, const double ht, const double maxx, const double maxy);
void postcard_land(const double x, const double y, const double wd, const double ht, const double maxx, const double maxy);

int main(int argc, char * const argv[])
{
   int opt;
   int i, j;
   bool portrait;
   double x, y;
   double dx, dy;
   int nx, ny;
   double maxx, maxy;
   double trueWidthmm;
   double trueHeightmm;
   double borderx;
   double bordery;
   PlotInfo_t info;
   
   while ((opt = getopt(argc, argv, "no:p:q:s:t:v:")) != -1) {
      switch (opt) {
      case 's':
      case 'n':
      case 'o':
      case 'p':
      case 'q':
      case 'v':
         plotopt(opt, optarg);
         break;
      case 't':   // Suppress title, which would spoil the lower-left postcard
         break;
      default: /* '?' */
         fprintf(stderr, "Usage: %s [-p pen] [-q plotter] [-s <size>] [-t title]\n", argv[0]);
         fprintf(stderr, "       <size> ::= A1 | A2 | A3 | A4 | A5 | A6\n");
         exit(EXIT_FAILURE);
      }
   }

   if (plotbegin(0) < 0) {
      fputs("Failed to initialise HPGL library\n", stderr);
      exit(EXIT_FAILURE);
   }

   if (getplotinfo(&info, sizeof (info)) < 0) {
      fputs("Failed to get plot information\n", stderr);
      exit(EXIT_FAILURE);
   }

   maxx = info.widthInUnits;
   maxy = info.heightInUnits;
   
   switch (info.paperSize) {
   case ISO_A3:
      nx = 4;
      ny = 2;
      trueWidthmm = 420;
      trueHeightmm = 297;
      portrait = true;
      break;
   case ISO_A4:
      nx = 2;
      ny = 2;
      trueWidthmm = 297;
      trueHeightmm = 210;
      portrait = false;
      break;
   case ISO_A5:
      nx = 2;
      ny = 1;
      trueWidthmm = 210;
      trueHeightmm = 148;
      portrait = true;
      break;
   case ISO_A6:
      nx = 1;
      ny = 1;
      trueWidthmm = 184;
      trueHeightmm = 105;
      portrait = false;
      break;
   }
   
   borderx = (trueWidthmm - info.widthInmm) / 2.0;
   bordery = (trueHeightmm - info.heightInmm) / 2.0;
   
   dx = (trueWidthmm * info.unitsPermm) / (double)nx;
   dy = (trueHeightmm * info.unitsPermm) / (double)ny;
   
   for (i = 1; i < nx; i++) {
      x = (dx * (double)i) - (borderx * info.unitsPermm);
      moveto(x, 0);
      lineto(x, maxy);
   }
   
   for (i = 1; i < ny; i++) {
      y = (dy * (double)i) - (bordery * info.unitsPermm);
      moveto(0, y);
      lineto(maxx, y);
   }
   
   for (i = 0; i < nx; i++)
      for (j = 0; j < ny; j++) {
         x = (dx * (double)i) - (borderx * info.unitsPermm);
         y = (dy * (double)j) - (bordery * info.unitsPermm);

         if (portrait)
            postcard_port(x, y, dx, dy, maxx, maxy);
         else
            postcard_land(x, y, dx, dy, maxx, maxy);
      }
   
   plotend();
   
   return (0);
}


void postcard_port(const double x, const double y, const double wd, const double ht, const double maxx, const double maxy)
{
   bool right = true;
   bool bottom = true;
   double yc = y + (ht / 2.0);
   double x1 = x + (wd / 16.0);
   double x2 = x + wd - (wd / 16.0);
   double sx2 = x + wd - (5.0 * 40.0);
   double sy2 = y + (5.0 * 40.0);
   double sx1 = sx2 - (22.0 * 40.0);
   double sy1 = sy2 + (19.0 * 40.0);
   
   if (x1 < 0.0)
      x1 = 0.0;
      
   if (x2 > maxx)
      x2 = maxx;
      
   if (sx2 > maxx) {
      sx2 = maxx;
      right = false;
   }
   
   if (sy2 > maxy)
      sy2 = maxy;

   if (sy2 < 0.0) {
      sy2 = 0.0;
      bottom = false;
   }
   
   moveto(x1, yc);
   lineto(x2, yc);
   
   moveto(sx1, sy1);
   lineto(sx1, sy2); // Left side

   if (bottom)
      lineto(sx2, sy2); // Bottom
   else
      moveto(sx2, sy2);
      
   if (right)
      lineto(sx2, sy1); // Right side
   else
      moveto(sx2, sy1);
   
   lineto(sx1, sy1); // Top
}


void postcard_land(const double x, const double y, const double wd, const double ht, const double maxx, const double maxy)
{
}

