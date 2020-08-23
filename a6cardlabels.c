/* a6cardlabels --- plot labelling for A6 postcards         2020-08-23 */
/* Copyright (c) 2020 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include "hpgllib.h"

void postcard_port(const double x, const double y, const double wd, const double ht);
void postcard_land(const double x, const double y, const double wd, const double ht);

int main(int argc, char * const argv[])
{
   int opt;
   int i, j;
   bool portrait;
   double x, y;
   double dx, dy;
   int nx, ny;
   double maxx, maxy;
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

   if (plotbegin(1) < 0) {
      fputs("Failed to initialise HPGL library\n", stderr);
      exit(EXIT_FAILURE);
   }

   if (getplotinfo(&info, sizeof (info)) < 0) {
      fputs("Failed to get plot information\n", stderr);
      exit(EXIT_FAILURE);
   }

   getplotsize(&maxx, &maxy);
   
   switch (info.paperSize) {
   case ISO_A3:
      nx = 4;
      ny = 2;
      portrait = true;
      break;
   case ISO_A4:
      nx = 2;
      ny = 2;
      portrait = false;
      break;
   case ISO_A5:
      nx = 2;
      ny = 1;
      portrait = true;
      break;
   case ISO_A6:
      nx = 1;
      ny = 1;
      portrait = false;
      break;
   }
   
   dx = maxx / (double)nx;
   dy = maxy / (double)ny;
   
   for (i = 1; i < nx; i++) {
      x = dx * (double)i;
      moveto(x, 0);
      lineto(x, maxy);
   }
   
   for (i = 1; i < ny; i++) {
      y = dy * (double)i;
      moveto(0, y);
      lineto(maxx, y);
   }
   
   for (i = 0; i < nx; i++)
      for (j = 0; j < ny; j++) {
         x = dx * (double)i;
         y = dy * (double)j;

         if (portrait)
            postcard_port(x, y, dx, dy);
         else
            postcard_land(x, y, dx, dy);
      }
   
   plotend();
   
   return (0);
}


void postcard_port(const double x, const double y, const double wd, const double ht)
{
   double yc = y + (ht / 2.0);
   double x1 = x + (wd / 16.0);
   double x2 = x + wd - (wd / 16.0);
   double sx2 = x + wd - (5.0 * 40.0);
   double sy2 = y + (5.0 * 40.0);
   double sx1 = sx2 - (22.0 * 40.0);
   double sy1 = sy2 + (19.0 * 40.0);
   
   moveto(x1, yc);
   lineto(x2, yc);
   
   rectangle(sx1, sy1, sx2, sy2);
}


void postcard_land(const double x, const double y, const double wd, const double ht)
{
}

