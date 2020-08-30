/* truchet2 --- draw Truchet tiles with arcs on them        2020-08-30 */
/* Copyright (c) 2020 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "hpgllib.h"

struct Tile {
   bool top;
   bool bottom;
   bool left;
   bool right;
};

void drawtile(const double x, const double y, const double wd, const double ht, const int tile_type);


int main(int argc, char * const argv[])
{
   /* Inspired by a tweet on #plottertwitter */
   int opt;
   int i, j;
   int ngridx = 12;
   int ngridy = 12;
   int tile_type;
   int grid[32][32];
   double x, y;
   double xc;
   double maxx, maxy;
   double height;
   double gridw, gridh;
   double gridx0;
   static struct Tile connect[6] = {
      {false,  true,  true, false}, // 0
      {false,  true, false,  true}, // 1
      {true,  false,  true, false}, // 2
      {true,  false, false,  true}, // 3
      {true,   true, false, false}, // 4
      {false, false,  true,  true}  // 5
   };

   while ((opt = getopt(argc, argv, "no:p:s:t:v:")) != -1) {
      switch (opt) {
      case 's':
         if (strchr(optarg, '1')) {
            ngridx = ngridy = 32;
         }
         else if (strchr(optarg, '2')) {
            ngridx = ngridy = 14; // If A1 is 32, A2 should be 22, but that's too slow
         }
         else if (strchr(optarg, '4')) {
            ngridx = ngridy = 8;
         }
         else if (strchr(optarg, '5')) {
            ngridx = ngridy = 6;
         }
         else if (strchr(optarg, '6')) {
            ngridx = ngridy = 4;
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
   
   for (i = 0; i < ngridy; i++) {
      for (j = 0; j < ngridx; j++) {
//       tile_type = (i + j) % 4;
         tile_type = rand() / (RAND_MAX / 4);
         grid[i][j] = tile_type;
      }
   }

   for (i = 0; i < ngridx; i++) {
      x = gridx0 + (i * gridw);
      
      switch (grid[ngridy - 1][i]) {
      case 2:
      case 3:
      case 4:
      case 5:
         moveto(x, maxy);
         lineto(x + gridw, maxy);
      }
   }
   
   for (i = 0; i < ngridy; i++) {
      y = i * gridh;
      
      switch (grid[i][0]) {
      case 0:
      case 2:
      case 4:
      case 5:
         moveto(gridx0, y);
         lineto(gridx0, y + gridh);
      }
   }
   
   for (i = 0; i < ngridx; i++) {
      x = gridx0 + (i * gridw);
      
      switch (grid[0][i]) {
      case 0:
      case 1:
      case 4:
      case 5:
         moveto(x, 0.0);
         lineto(x + gridw, 0.0);
      }
   }
   
   // Draw right-hand edge
   for (i = 0; i < ngridy; i++) {
      y = i * gridh;
      
      switch (grid[i][ngridx - 1]) {
      case 1:
      case 3:
      case 4:
      case 5:
         moveto(gridx0 + maxy, y);
         lineto(gridx0 + maxy, y + gridh);
      }
   }

   // Draw the main grid
   for (i = 0; i < ngridy; i++) {
      for (j = 0; j < ngridx; j++) {
         x = gridx0 + (j * gridw);
         y = i * gridh;
         drawtile(x, y, gridw, gridh, grid[i][j]);
      }
   }
   
   // Draw horizontal lines
   for (i = 0; i < (ngridy - 1); i++) {
      for (j = 0; j < ngridx; j++) {
         const int lower = grid[i][j];
         const int upper = grid[i + 1][j];
         
         if (connect[lower].top == connect[upper].bottom)
            ;
         else {
            x = gridx0 + (j * gridw);
            y = (i + 1) * gridh;

            moveto(x, y);
            lineto(x + gridw, y);
         }
      }
   }
   
   // Draw vertical lines
   for (i = 0; i < ngridy; i++) {
      for (j = 0; j < (ngridx - 1); j++) {
         const int left  = grid[i][j];
         const int right = grid[i][j + 1];
         
         if (connect[left].right == connect[right].left)
            ;
         else {
            x = gridx0 + ((j + 1) * gridw);
            y = i * gridh;

            moveto(x, y);
            lineto(x, y + gridh);
         }
      }
   }
   
   plotend();
   
   return (0);
}


void drawtile(const double x, const double y, const double wd, const double ht, const int tile_type)
{
   int i;
   const double dr = (wd / 5.0);
   
   switch (tile_type) {
   case 0:
      for (i = 0; i < 5; i++) {
         const double x1 = x + (dr * (double)(i + 1));
         
         moveto(x1, y);
         arc(x, y, 90.0);
      }
      break;
   case 1:
      for (i = 0; i < 5; i++) {
         const double x1 = x + wd - (dr * (double)(i + 1));
         
         moveto(x1, y);
         arc(x + wd, y, -90.0);
      }
      break;
   case 2:
      for (i = 0; i < 5; i++) {
         const double y1 = y + ht - (dr * (double)(i + 1));
         
         moveto(x, y1);
         arc(x, y + ht, 90.0);
      }
      break;
   case 3:
      for (i = 0; i < 5; i++) {
         const double x1 = x + wd - (dr * (double)(i + 1));

         moveto(x1, y + ht);
         arc(x + wd, y + ht, 90.0);
      }
      break;
   case 4:
      for (i = 1; i < 5; i++) {
         const double x1 = x + (dr * (double)i);
         
         moveto(x1, y);
         lineto(x1, y + ht);
      }
      break;
   case 5:
      for (i = 1; i < 5; i++) {
         const double y1 = y + (dr * (double)i);
         
         moveto(x, y1);
         lineto(x + wd, y1);
      }
      break;
   }
}
