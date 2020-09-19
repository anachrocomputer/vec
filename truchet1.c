/* truchet1 --- draw Truchet tiles with arcs on them        2020-08-26 */
/* Copyright (c) 2020 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "hpgllib.h"


void drawTile(const double x, const double y, const double wd, const double ht, const int tile_type);


int main(int argc, char * const argv[])
{
   /* Inspired by a tweet on #plottertwitter */
   int opt;
   int i, j;
   int ngridx = 12;
   int ngridy = 12;
   int tile_type;
   double x, y;
   double xc;
   double maxx, maxy;
   double height;
   double gridw, gridh;
   double gridx0;

   while ((opt = getopt(argc, argv, "no:p:s:t:v:")) != -1) {
      switch (opt) {
      case 's':
         if (strchr(optarg, '1')) {
            ngridx = ngridy = 32;
         }
         else if (strchr(optarg, '2')) {
            ngridx = ngridy = 14; // If A1 is 32, A2 should be 22, but that's too slow
         }
            
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

   srand((unsigned int)time(NULL));

   if (plotbegin(0) < 0) {
      fputs("Failed to initialise HPGL library\n", stderr);
      exit(EXIT_FAILURE);
   }

   getplotsize(&maxx, &maxy);
   
   xc = maxx / 2.0;
   
   height = maxy;

   gridx0 = xc - (height / 2.0);
   
   gridw = maxy / ngridx;
   gridh = maxy / ngridy;
   
   const double bigr = gridw / sqrt(2.0);
   const double smallr = gridw - bigr;

   for (i = 0; i < ngridx; i++) {
      x = gridx0 + (i * gridw);
      
      moveto(x + smallr, maxy);
      lineto(x + bigr, maxy);
   }
   
   for (i = 0; i < ngridy; i++) {
      y = i * gridh;
      
      moveto(gridx0, y + smallr);
      lineto(gridx0, y + bigr);
   }
   
   for (i = 0; i < ngridx; i++) {
      x = gridx0 + (i * gridw);
      
      moveto(x + smallr, 0.0);
      lineto(x + bigr, 0.0);
   }
   
   for (i = 0; i < ngridy; i++) {
      y = i * gridh;
      
      moveto(gridx0 + maxy, y + smallr);
      lineto(gridx0 + maxy, y + bigr);
   }

   /* Draw the main grid */
   for (i = 0; i < ngridy; i++) {
      for (j = 0; j < ngridx; j++) {
         x = gridx0 + (j * gridw);
         y = i * gridh;
//       tile_type = (i + j) % 2;
         tile_type = rand() / (RAND_MAX / 2);
         drawTile(x, y, gridw, gridh, tile_type);
      }
   }
   
   plotend();
   
   return (0);
}


/* drawTile --- draw a single tile at (x, y) */

void drawTile(const double x, const double y, const double wd, const double ht, const int tile_type)
{
   const double bigr = wd / sqrt(2.0);
   const double smallr = wd - bigr;
   
   switch (tile_type) {
   case 0:
      moveto(x + wd, y + smallr);
      arc(x + wd, y, 90.0);
      moveto(x + smallr, y);
      arc(x + wd, y, -90.0);
      moveto(x + bigr, y + ht);
      arc(x, y + ht, -90.0);
      moveto(x, y + bigr);
      arc(x, y + ht, 90.0);
      break;
   case 1:
      moveto(x, y + smallr);
      arc(x, y, -90.0);
      moveto(x + bigr, y);
      arc(x, y, 90.0);
      moveto(x + smallr, y + ht);
      arc(x + wd, y + ht, 90.0);
      moveto(x + wd, y + bigr);
      arc(x + wd, y + ht, -90.0);
      break;
   }
}
