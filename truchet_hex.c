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

struct XY {
   double x;
   double y;
};

struct HexRec {
   struct XY face[6];
   struct XY vert[6];
   struct XY apex[6];
};

void drawHexagon(const double x, const double y, const struct HexRec *const hex, const int first, const int last);
void drawtile(const double x, const double y, const struct HexRec *const hex, const int tile_type);
void drawTileB(const double x, const double y, const struct HexRec *const hex, const int i);
void drawTileC(const double x, const double y, const struct HexRec *const hex, const int i);
void drawTileD(const double x, const double y, const struct HexRec *const hex, const int i);
void drawTileE(const double x, const double y, const struct HexRec *const hex, const int i);
void lineFace(const double x, const double y, const struct HexRec *const hex, const int f);
void arcApex(const double x, const double y, const struct HexRec *const hex, const int a);
void arcVertex(const double x, const double y, const struct HexRec *const hex, const int v);


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
   double rowht;
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
   rowht = 2.0 * r * sin(delta);
   
   // Generate the base hexagon
   for (i = 0; i < 6; i++) {
      const double theta = delta * (double)i;
      
      hex.face[i].x = r * cos(theta);
      hex.face[i].y = r * sin(theta);

      hex.vert[i].x = r * cos(theta + (delta / 2.0)) / sin(delta);
      hex.vert[i].y = r * sin(theta + (delta / 2.0)) / sin(delta);

      hex.apex[i].x = r * cos(theta) * 2.0;
      hex.apex[i].y = r * sin(theta) * 2.0;
   }
   
   // Generate the grid
   for (i = 0; i < ngridy; i++) {
      for (j = 0; j < ngridx; j++) {
//       tile_type = (i + j) % 15;
         tile_type = rand() / (RAND_MAX / 15);
         grid[i][j] = tile_type;
      }
   }

   pencolr(1);
   
   // Draw the hexagons
   for (i = 0; i < ngridy; i++) {
      y = rowht + (i * rowht);

      if (i & 1) {
         for (j = 0; j < (ngridx - 1) ; j++) {
            x = r + gridx0 + (j * r * 2.0);
//          circle(x, y, r);
            if (j == (ngridx - 2))
               drawHexagon(x, y, &hex, 0, 6);
            else
               drawHexagon(x, y, &hex, 0, 5);
         }
      }
      else {

         for (j = 0; j < ngridx; j++) {
            x = gridx0 + (j * r * 2.0);
//          circle(x, y, r);
            if (i == 0) {
               if (j == 0)
                  drawHexagon(x, y, &hex, 1, 5);
               else if (j == (ngridx - 1))
                  drawHexagon(x, y, &hex, 2, 7);
               else
                  drawHexagon(x, y, &hex, 2, 5);
            }
            else {
               if (j == 0)
                  drawHexagon(x, y, &hex, 1, 4);
               else if (j == (ngridx - 1)) {
                  drawHexagon(x, y, &hex, 2, 3);
                  drawHexagon(x, y, &hex, 4, 7);
               }
               else
                  drawHexagon(x, y, &hex, 2, 3);
            }
         }
      }
   }

   pencolr(0);
   
   // Draw the main grid
   for (i = 0; i < ngridy; i++) {
      y = rowht + (i * rowht);

      if (i & 1)
         for (j = 0; j < (ngridx - 1) ; j++) {
            x = r + gridx0 + (j * r * 2.0);
            drawtile(x, y, &hex, grid[i][j]);
         }
      else
         for (j = 0; j < ngridx; j++) {
            x = gridx0 + (j * r * 2.0);
            drawtile(x, y, &hex, grid[i][j]);
         }
   }
   
   plotend();
   
   return (0);
}


void drawHexagon(const double x, const double y, const struct HexRec *const hex, const int first, const int last)
{
   int i;
   
   moveto(x + hex->vert[first].x, y + hex->vert[first].y);
   
   for (i = first + 1; i < last + 1; i++)
      lineto(x + hex->vert[i % 6].x, y + hex->vert[i % 6].y);
}


void drawtile(const double x, const double y, const struct HexRec *const hex, const int tile_type)
{
   switch (tile_type) {
   case 0:  // Tile A: only one kind
      lineFace(x, y, hex, 0);
      lineFace(x, y, hex, 1);
      lineFace(x, y, hex, 2);
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
   lineFace(x, y, hex, i);

   arcApex(x, y, hex, i);

   arcApex(x, y, hex, i + 3);
}


void drawTileC(const double x, const double y, const struct HexRec *const hex, const int i)
{
   lineFace(x, y, hex, i);

   arcVertex(x, y, hex, i + 1);

   arcVertex(x, y, hex, i + 4);
}


void drawTileD(const double x, const double y, const struct HexRec *const hex, const int i)
{
   arcVertex(x, y, hex, i + 1);

   arcVertex(x, y, hex, i + 3);

   arcVertex(x, y, hex, i + 5);
}


void drawTileE(const double x, const double y, const struct HexRec *const hex, const int i)
{
   arcVertex(x, y, hex, i);

   arcApex(x, y, hex, i + 3);

   arcApex(x, y, hex, i + 4);
}


void lineFace(const double x, const double y, const struct HexRec *const hex, const int f)
{
   const int f1 = f % 6;
   const int f2 = (f + 3) % 6;
   
   moveto(x + hex->face[f1].x, y + hex->face[f1].y);
   lineto(x + hex->face[f2].x, y + hex->face[f2].y);
}


void arcApex(const double x, const double y, const struct HexRec *const hex, const int a)
{
   const int f = (a + 1) % 6;

   moveto(x + hex->face[f].x, y + hex->face[f].y);
   arc(x + hex->apex[a % 6].x, y + hex->apex[a % 6].y, 60.0);
}


void arcVertex(const double x, const double y, const struct HexRec *const hex, const int v)
{
   const int f = (v + 1) % 6;
   
   moveto(x + hex->face[f].x, y + hex->face[f].y);
   arc(x + hex->vert[v % 6].x, y + hex->vert[v % 6].y, 120.0);
}
