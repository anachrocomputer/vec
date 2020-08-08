/* celticstep --- draw Celtic step pattern                  2013-11-16 */
/* Copyright (c) 2013 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "hpgllib.h"


void drawstep(const double x0, const double y0, const double w, const double h, const int flipx, const int flipy);
void draw_saltire_border(const double x0, const double y0, const int nx, const int ny, const double w, const double h);


int main(int argc, char * const argv[])
{
/* Celtic Design: A Beginner's Manual, Aidan Meehan, 1991
   Chapter 1 */
   int opt;
   int i, j;
   int ngridx = 16;
   int ngridy = 16;
   double x, y;
   double xc;
   const double scale = 40.0; /* Plotter units per mm */
   double maxx, maxy;
   double height;
   double gridw, gridh;
   double gridx0, gridy0;
   double gridxmax, gridymax;
   double sqsize = 16.0;      // Size of cell in mm
   
   while ((opt = getopt(argc, argv, "no:p:s:t:v:")) != -1) {
      switch (opt) {
      case 's':
         if (strchr(optarg, '1')) {
            ngridx = ngridy = 32;
            sqsize = 16.0;
         }
         else if (strchr(optarg, '2')) {
            ngridx = ngridy = 14; // If A1 is 32, A2 should be 22, but that's too slow
            sqsize = 28.0;
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

   plotbegin(0);

   getplotsize(&maxx, &maxy);
   
   xc = maxx / 2.0;
   
   height = maxy;

   /* Draw square border */
   rectangle(xc - (height / 2.0), 0.0, xc + (height / 2.0), maxy);
   
   gridw = sqsize * scale;
   gridh = sqsize * scale;
   
   gridx0 = (maxx - (ngridx * gridw)) / 2.0;
   gridy0 = (maxy - (ngridy * gridh)) / 2.0;
   
   gridxmax = gridx0 + (ngridx * gridw);
   gridymax = gridy0 + (ngridy * gridh);
   
   /* Draw vertical grid lines */
   for (x = gridx0, i = 0; i <= ngridx; x += gridw, i++) {
      if (i & 1) {
         moveto(x, gridy0);
         lineto(x, gridymax);
      }
      else {
         moveto(x, gridymax);
         lineto(x, gridy0);
      }
   }
   
   /* Draw horizontal grid lines */
   for (y = gridy0, i = 0; i <= ngridy; y += gridh, i++) {
      if (i & 1) {
         moveto(gridx0, y);
         lineto(gridxmax, y);
      }
      else {
         moveto(gridxmax, y);
         lineto(gridx0, y);
      }
   }
   
   /* Draw step pattern within one-square border */
   for (i = 1; i < (ngridy - 1); i++) { 
      for (j = 1; j < (ngridx - 1); j++) {
         x = gridx0 + (j * gridw);
         y = gridy0 + (i * gridh);
         drawstep(x, y, gridw, gridh, j & 1, i & 1);
      }
   }
   
   draw_saltire_border(gridx0, gridy0, ngridx, ngridy, gridw, gridh);
   
   plotend();
   
   return (0);
}


/* drawstep --- draw a single block of Celtic step pattern */

void drawstep(const double x0, const double y0, const double w, const double h, const int flipx, const int flipy)
{
   struct coord {
      double x, y;
   };
   struct Grid {
      double x, y;
      struct coord ne, se, nw, sw;
   };
   double xoff = w / 20.0;
   double yoff = h / 20.0;
   struct Grid grid[4][4];
   int x, y;
   int i, j;
   
   if (flipx)
      xoff = -xoff;
   
   if (flipy)
      yoff = -yoff;
   
   for (i = 0; i < 4; i++)
      for (j = 0; j < 4; j++) {
         if (flipx)
            x = 3 - j;
         else
            x = j;
            
         if (flipy)
            y = 3 - i;
         else
            y = i;
            
         grid[y][x].x = x0 + (j * (w / 3.0));
         grid[y][x].y = y0 + (i * (h / 3.0));
         grid[y][x].ne.x = grid[y][x].se.x = grid[y][x].x + xoff;
         grid[y][x].nw.x = grid[y][x].sw.x = grid[y][x].x - xoff;
         grid[y][x].ne.y = grid[y][x].nw.y = grid[y][x].y + yoff;
         grid[y][x].se.y = grid[y][x].sw.y = grid[y][x].y - yoff;
      }
   
#ifdef DB
   for (y = 0; y < 3; y++)
      for (x = 0; x < 3; x++) {
         moveto(grid[y][x].ne.x, grid[y][x].ne.y);
         lineto(grid[y+1][x+1].sw.x, grid[y+1][x+1].sw.y);
         moveto(grid[y][x+1].nw.x, grid[y][x+1].nw.y);
         lineto(grid[y+1][x].se.x, grid[y+1][x].se.y);
      }
#endif
      
   /* Box */
   moveto(grid[1][0].x, grid[1][0].y);
   lineto(grid[1][1].x, grid[1][1].y);
   lineto(grid[0][1].x, grid[0][1].y);
   
   /* Zig-zag */
   moveto(grid[1][3].x, grid[1][3].y);
   lineto(grid[1][2].x, grid[1][2].y);
   lineto(grid[2][2].x, grid[2][2].y);
   lineto(grid[2][1].x, grid[2][1].y);
   lineto(grid[3][1].x, grid[3][1].y);
   
   /* Inner box */
   rectangle(grid[0][0].ne.x, grid[0][0].ne.y, grid[1][1].sw.x, grid[1][1].sw.y);
   
   /* Inner zig-zag */
   moveto(grid[0][1].ne.x, grid[0][1].ne.y);
   lineto(grid[0][3].nw.x, grid[0][3].nw.y);
   lineto(grid[1][3].sw.x, grid[1][3].sw.y);
   lineto(grid[1][2].sw.x, grid[1][2].sw.y);
   lineto(grid[2][2].sw.x, grid[2][2].sw.y);
   lineto(grid[2][1].sw.x, grid[2][1].sw.y);
   lineto(grid[3][1].sw.x, grid[3][1].sw.y);
   lineto(grid[3][0].se.x, grid[3][0].se.y);
   lineto(grid[1][0].ne.x, grid[1][0].ne.y);
   lineto(grid[1][1].ne.x, grid[1][1].ne.y);
   lineto(grid[0][1].ne.x, grid[0][1].ne.y);
   
   /* Inner L */
   moveto(grid[1][2].ne.x, grid[1][2].ne.y);
   lineto(grid[1][3].nw.x, grid[1][3].nw.y);
   lineto(grid[3][3].sw.x, grid[3][3].sw.y);
   lineto(grid[3][1].se.x, grid[3][1].se.y);
   lineto(grid[2][1].ne.x, grid[2][1].ne.y);
   lineto(grid[2][2].ne.x, grid[2][2].ne.y);
   lineto(grid[1][2].ne.x, grid[1][2].ne.y);
}


/* draw_saltire_border --- draw border squares as saltires */

void draw_saltire_border(const double x0, const double y0, const int nx, const int ny, const double w, const double h)
{
   /* nx and ny must be even, but need not be equal */
   const double xmax = x0 + (nx * w);
   const double ymax = y0 + (ny * h);
   int i;
   
   moveto(x0, y0);
   lineto(x0 + w, y0 + h);
   
   moveto(xmax, y0);
   lineto(xmax - w, y0 + h);
   
   moveto(x0, ymax);
   lineto(x0 + w, ymax - h);

   moveto(xmax, ymax);
   lineto(xmax - w, ymax - h);
   
   /* Across bottom */
   moveto(x0, y0 + (2.0 * h));
   lineto(x0 + (2.0 * w), y0);
   
   for (i = 3; i < (nx - 1); i += 2) {
      lineto(x0 + (i * w), y0 + h);
      lineto(x0 + ((i + 1) * w), y0);
   }

   /* Up RHS */
   lineto(xmax, y0 + (2.0 * h));
   
   for (i = 3; i < (ny - 1); i += 2) {
      lineto(xmax - w, y0 + (i * h));
      lineto(xmax, y0 + ((i + 1) * h));
   }
   
   /* Across top, R to L */
   lineto(xmax - (2.0 * w), ymax);
   
   for (i = nx - 3; i > 1; i -= 2) {
      lineto(x0 + (i * w), ymax - h);
      lineto(x0 + ((i - 1) * w), ymax);
   }
   
   /* Down LHS */
   lineto(x0, ymax - (2.0 * h));
   
   for (i = ny - 3; i > 1; i -= 2) {
      lineto(x0 + w, y0 + (i * h));
      lineto(x0, y0 + ((i - 1) * h));
   }
   
   /* Across bottom again, L to R */
   moveto(x0, y0 + h);
   lineto(x0 + w, y0);
   
   for (i = 2; i < nx; i += 2) {
      lineto(x0 + (i * w), y0 + h);
      lineto(x0 + ((i + 1) * w), y0);
   }
   
   /* Up RHS */
   lineto(xmax, y0 + h);

   for (i = 2; i < ny; i += 2) {
      lineto(xmax - w, y0 + (i * h));
      lineto(xmax, y0 + ((i + 1) * h));
   }
   
   /* Across top, R to L */
   lineto(xmax - w, ymax);
   
   for (i = nx - 2; i > 1; i -= 2) {
      lineto(x0 + (i * w), ymax - h);
      lineto(x0 + ((i - 1) * w), ymax);
   }
   
   /* Down LHS */
   lineto(x0, ymax - h);
   
   for (i = ny - 2; i > 1; i -= 2) {
      lineto(x0 + w, y0 + (i * h));
      lineto(x0, y0 + ((i - 1) * h));
   }
}
