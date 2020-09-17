/* truchet_hex --- draw hexagonal Truchet tiles             2020-09-16 */
/* Copyright (c) 2020 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "hpgllib.h"


#define DEG_TO_RAD  (M_PI / 180.0)

struct HexRec {
   double xface[6];
   double yface[6];
   double xvert[6];
   double yvert[6];
   double xapex[6];
   double yapex[6];
};

void drawtile(const double x, const double y, const struct HexRec *const hex, const int tile_type);
void drawTileD(const double x, const double y, const struct HexRec *const hex, const int i);
void drawTileE(const double x, const double y, const struct HexRec *const hex, const int i);


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
   double r;
   double gridx0;
   const double delta = (2.0 * M_PI) / 6.0;
   struct HexRec hex;

   while ((opt = getopt(argc, argv, "no:p:s:t:v:")) != -1) {
      switch (opt) {
      case 's':
         if (strchr(optarg, '1')) {
            ngridx = ngridy = 24;
         }
         else if (strchr(optarg, '2')) {
            ngridx = ngridy = 16;
         }
         else if (strchr(optarg, '3')) {
            ngridx = ngridy = 12;
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

   if (plotbegin(1) < 0) {
      fputs("Failed to initialise HPGL library\n", stderr);
      exit(EXIT_FAILURE);
   }

   getplotsize(&maxx, &maxy);
   
   xc = maxx / 2.0;
   
   height = maxy;

   gridx0 = xc - (height / 2.0);
   
   gridw = maxy / ngridx;
   gridh = maxy / ngridy;
   
   r = gridw / 2.0;
   
   // Generate the base hexagon
   for (i = 0; i < 6; i++) {
      const double theta = delta * (double)i;
      
      hex.xface[i] = r * cos(theta);
      hex.yface[i] = r * sin(theta);

      hex.xvert[i] = r * cos(theta + (delta / 2.0)) / sin(delta);
      hex.yvert[i] = r * sin(theta + (delta / 2.0)) / sin(delta);

      hex.xapex[i] = r * cos(theta) * 2.0;
      hex.yapex[i] = r * sin(theta) * 2.0;
   }
   
   // Generate the grid
   for (i = 0; i < ngridy; i++) {
      for (j = 0; j < ngridx; j++) {
//       tile_type = (i + j) % 15;
         tile_type = rand() / (RAND_MAX / 15);
         grid[i][j] = tile_type;
      }
   }

   // Draw the main grid
   for (i = 0; i < ngridy; i++) {
      if (i & 1)
         for (j = 0; j < (ngridx - 1) ; j++) {
            x = r + gridx0 + (j * r * 2.0);
            y = r + (i * (2.0 * r * sin(delta)));
//          circle(x, y, r);
            drawtile(x, y, &hex, grid[i][j]);
         }
      else
         for (j = 0; j < ngridx; j++) {
            x = gridx0 + (j * r * 2.0);
            y = r + (i * (2.0 * r * sin(delta)));
//          circle(x, y, r);
            drawtile(x, y, &hex, grid[i][j]);
         }
   }
   
   
   plotend();
   
   return (0);
}


void drawtile(const double x, const double y, const struct HexRec *const hex, const int tile_type)
{
   switch (tile_type) {
   case 0:  // Tile A: only one kind
      moveto(x + hex->xface[0], y + hex->yface[0]);
      lineto(x + hex->xface[3], y + hex->yface[3]);
      moveto(x + hex->xface[1], y + hex->yface[1]);
      lineto(x + hex->xface[4], y + hex->yface[4]);
      moveto(x + hex->xface[2], y + hex->yface[2]);
      lineto(x + hex->xface[5], y + hex->yface[5]);
      break;
   case 1:  // Tile B: three orientations
      moveto(x + hex->xface[0], y + hex->yface[0]);
      lineto(x + hex->xface[3], y + hex->yface[3]);
      moveto(x + hex->xface[1], y + hex->yface[1]);
      arc(x + hex->xapex[0], y + hex->yapex[0], 60.0);
      moveto(x + hex->xface[4], y + hex->yface[4]);
      arc(x + hex->xapex[3], y + hex->yapex[3], 60.0);
      break;
   case 2:
      moveto(x + hex->xface[1], y + hex->yface[1]);
      lineto(x + hex->xface[4], y + hex->yface[4]);
      moveto(x + hex->xface[2], y + hex->yface[2]);
      arc(x + hex->xapex[1], y + hex->yapex[1], 60.0);
      moveto(x + hex->xface[5], y + hex->yface[5]);
      arc(x + hex->xapex[4], y + hex->yapex[4], 60.0);
      break;
   case 3:
      moveto(x + hex->xface[2], y + hex->yface[2]);
      lineto(x + hex->xface[5], y + hex->yface[5]);
      moveto(x + hex->xface[3], y + hex->yface[3]);
      arc(x + hex->xapex[2], y + hex->yapex[2], 60.0);
      moveto(x + hex->xface[0], y + hex->yface[0]);
      arc(x + hex->xapex[5], y + hex->yapex[5], 60.0);
      break;
   case 4:  // Tile C: three orientations
      moveto(x + hex->xface[0], y + hex->yface[0]);
      lineto(x + hex->xface[3], y + hex->yface[3]);
      moveto(x + hex->xface[2], y + hex->yface[2]);
      arc(x + hex->xvert[1], y + hex->yvert[1], 120.0);
      moveto(x + hex->xface[5], y + hex->yface[5]);
      arc(x + hex->xvert[4], y + hex->yvert[4], 120.0);
      break;
   case 5:
      moveto(x + hex->xface[1], y + hex->yface[1]);
      lineto(x + hex->xface[4], y + hex->yface[4]);
      moveto(x + hex->xface[3], y + hex->yface[3]);
      arc(x + hex->xvert[2], y + hex->yvert[2], 120.0);
      moveto(x + hex->xface[0], y + hex->yface[0]);
      arc(x + hex->xvert[5], y + hex->yvert[5], 120.0);
      break;
   case 6:
      moveto(x + hex->xface[2], y + hex->yface[2]);
      lineto(x + hex->xface[5], y + hex->yface[5]);
      moveto(x + hex->xface[4], y + hex->yface[4]);
      arc(x + hex->xvert[3], y + hex->yvert[3], 120.0);
      moveto(x + hex->xface[1], y + hex->yface[1]);
      arc(x + hex->xvert[0], y + hex->yvert[0], 120.0);
      break;
   case 7:
      drawTileD(x, y, hex, 0);
      break;
   case 8:
      drawTileD(x, y, hex, 1);
      break;
   case 9:
      drawTileE(x, y, hex, 0);
      break;
   case 10:
      drawTileE(x, y, hex, 1);
      break;
   case 11:
      drawTileE(x, y, hex, 2);
      break;
   case 12:
      drawTileE(x, y, hex, 3);
      break;
   case 13:
      drawTileE(x, y, hex, 4);
      break;
   case 14:
      drawTileE(x, y, hex, 5);
      break;
   }
}


void drawTileD(const double x, const double y, const struct HexRec *const hex, const int i)
{
   moveto(x + hex->xface[(i + 0) % 6], y + hex->yface[(i + 0) % 6]);
   arc(x + hex->xvert[(i + 5) % 6], y + hex->yvert[(i + 5) % 6], 120.0);

   moveto(x + hex->xface[(i + 4) % 6], y + hex->yface[(i + 4) % 6]);
   arc(x + hex->xvert[(i + 3) % 6], y + hex->yvert[(i + 3) % 6], 120.0);

   moveto(x + hex->xface[(i + 2) % 6], y + hex->yface[(i + 2) % 6]);
   arc(x + hex->xvert[(i + 1) % 6], y + hex->yvert[(i + 1) % 6], 120.0);
}


void drawTileE(const double x, const double y, const struct HexRec *const hex, const int i)
{
   moveto(x + hex->xface[(i + 1) % 6], y + hex->yface[(i + 1) % 6]);
   arc(x + hex->xvert[(i + 0) % 6], y + hex->yvert[(i + 0) % 6], 120.0);

   moveto(x + hex->xface[(i + 5) % 6], y + hex->yface[(i + 5) % 6]);
   arc(x + hex->xapex[(i + 4) % 6], y + hex->yapex[(i + 4) % 6], 60.0);

   moveto(x + hex->xface[(i + 4) % 6], y + hex->yface[(i + 4) % 6]);
   arc(x + hex->xapex[(i + 3) % 6], y + hex->yapex[(i + 3) % 6], 60.0);
}
