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
void drawTileB(const double x, const double y, const struct HexRec *const hex, const int i);
void drawTileC(const double x, const double y, const struct HexRec *const hex, const int i);
void drawTileD(const double x, const double y, const struct HexRec *const hex, const int i);
void drawTileE(const double x, const double y, const struct HexRec *const hex, const int i);
void lineFaceFace(const double x, const double y, const struct HexRec *const hex, const int f1, const int f2);
void arcFaceApex(const double x, const double y, const struct HexRec *const hex, const int f, const int a);
void arcFaceVertex(const double x, const double y, const struct HexRec *const hex, const int f, const int v);


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
      lineFaceFace(x, y, hex, 0, 3);
      lineFaceFace(x, y, hex, 1, 4);
      lineFaceFace(x, y, hex, 2, 5);
      break;
   case 1:  // Tile B: three orientations
      drawTileB(x, y, hex, 0);
      break;
   case 2:
      drawTileB(x, y, hex, 1);
      break;
   case 3:
      drawTileB(x, y, hex, 2);
      break;
   case 4:  // Tile C: three orientations
      drawTileC(x, y, hex, 0);
      break;
   case 5:
      drawTileC(x, y, hex, 1);
      break;
   case 6:
      drawTileC(x, y, hex, 2);
      break;
   case 7:  // Tile D: two orientations
      drawTileD(x, y, hex, 0);
      break;
   case 8:
      drawTileD(x, y, hex, 1);
      break;
   case 9:  // Tile E: six orientations
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


void drawTileB(const double x, const double y, const struct HexRec *const hex, const int i)
{
   lineFaceFace(x, y, hex, i + 0, i + 3);

   arcFaceApex(x, y, hex, i + 1, i + 0);

   arcFaceApex(x, y, hex, i + 4, i + 3);
}


void drawTileC(const double x, const double y, const struct HexRec *const hex, const int i)
{
   lineFaceFace(x, y, hex, i + 0, i + 3);

   arcFaceVertex(x, y, hex, i + 2, i + 1);

   arcFaceVertex(x, y, hex, i + 5, i + 4);
}


void drawTileD(const double x, const double y, const struct HexRec *const hex, const int i)
{
   arcFaceVertex(x, y, hex, i + 0, i + 5);

   arcFaceVertex(x, y, hex, i + 4, i + 3);

   arcFaceVertex(x, y, hex, i + 2, i + 1);
}


void drawTileE(const double x, const double y, const struct HexRec *const hex, const int i)
{
   arcFaceVertex(x, y, hex, i + 1, i + 0);

   arcFaceApex(x, y, hex, i + 5, i + 4);

   arcFaceApex(x, y, hex, i + 4, i + 3);
}


void lineFaceFace(const double x, const double y, const struct HexRec *const hex, const int f1, const int f2)
{
   moveto(x + hex->xface[f1 % 6], y + hex->yface[f1 % 6]);
   lineto(x + hex->xface[f2 % 6], y + hex->yface[f2 % 6]);
}


void arcFaceApex(const double x, const double y, const struct HexRec *const hex, const int f, const int a)
{
   moveto(x + hex->xface[f % 6], y + hex->yface[f % 6]);
   arc(x + hex->xapex[a % 6], y + hex->yapex[a % 6], 60.0);
}


void arcFaceVertex(const double x, const double y, const struct HexRec *const hex, const int f, const int v)
{
   moveto(x + hex->xface[f % 6], y + hex->yface[f % 6]);
   arc(x + hex->xvert[v % 6], y + hex->yvert[v % 6], 120.0);
}
