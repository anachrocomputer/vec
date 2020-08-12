/* linetiles --- draw randomised tiles made up of lines     2020-08-12 */
/* Copyright (c) 2020 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "hpgllib.h"


void drawtile(const double x, const double y, const double wd, const double ht, const int tile_type);


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

   /* Draw square border */
   rectangle(xc - (height / 2.0), 0.0, xc + (height / 2.0), maxy);
   
   gridx0 = xc - (height / 2.0);
   
   gridw = maxy / ngridx;
   gridh = maxy / ngridy;
   
   for (i = 0; i < ngridy; i++) {
      for (j = 0; j < ngridx; j++) {
         x = gridx0 + (j * gridw);
         y = i * gridh;
//       tile_type = (i + j) % 6;
         tile_type = rand() / (RAND_MAX / 6);
         drawtile(x, y, gridw, gridh, tile_type);
      }
   }
   
   plotend();
   
   return (0);
}


void drawtile(const double x, const double y, const double wd, const double ht, const int tile_type)
{
   int i;
   const double step = wd / 8.0;
   
   switch (tile_type) {
   case 0:
      for (i = 0; i < 9; i++) {
         moveto(x, y + (step * i));
         lineto(x + wd, y + (step * i));
      }
      break;
   case 1:
      for (i = 0; i < 9; i++) {
         moveto(x + (step * i), y);
         lineto(x + (step * i), y + ht);
      }
      break;
   case 2:
      for (i = 1; i < 9; i++) {
         moveto(x, y + (step * i));
         lineto(x + (step * i), y + (step * i));
         lineto(x + (step * i), y);
      }
      break;
   case 3:
      for (i = 1; i < 9; i++) {
         moveto(x, y + ht - (step * i));
         lineto(x + (step * i), y + ht - (step * i));
         lineto(x + (step * i), y + ht);
      }
      break;
   case 4:
      for (i = 1; i < 9; i++) {
         moveto(x + wd, y + (step * i));
         lineto(x + wd - (step * i), y + (step * i));
         lineto(x + wd - (step * i), y);
      }
      break;
   case 5:
      for (i = 1; i < 9; i++) {
         moveto(x + wd, y + ht - (step * i));
         lineto(x + wd - (step * i), y + ht - (step * i));
         lineto(x + wd - (step * i), y + ht);
      }
      break;
   }
}
