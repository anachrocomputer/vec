/* sutpent --- plot a Sutcliffe pentagon                    2016-10-31 */
/* Copyright (c) 2016 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include "hpgllib.h"

/* Inspired by Section 8.4 of "Generative Art" by Matt Pearson, ISBN 1935182625 */

struct Vertex {
   double x;
   double y;
};

void sutcliffepolygon(const double xc, const double yc, const double radius, const int nsides, const int levels, const double ratio);
void sutpoly(const int nsides, const struct Vertex vertex[], const int level, const double ratio);
void drawpoly(const int nsides, const struct Vertex vertex[]);


int main(int argc, char * const argv[])
{
   int opt;
   double w4, h4;
   double xc;
   double maxx, maxy;
   double radius;
   
   while ((opt = getopt(argc, argv, "no:p:s:t:v:")) != -1) {
      switch (opt) {
      case 's':
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

   plotbegin(1);

   getplotsize(&maxx, &maxy);
   
   w4 = maxx / 4.0;
   h4 = maxy / 4.0;
   
   radius = maxy / 4.0;
   xc = sqrt(((2.0 * radius) * (2.0 * radius)) - (h4 * h4));
   xc = (2.0 * w4) - xc;
   
   sutcliffepolygon(maxx - xc, h4 * 3.0, radius, 5, 6, 0.15);
   sutcliffepolygon(xc,        h4 * 3.0, radius, 6, 7, 0.15);
   sutcliffepolygon(maxx - xc, h4,       radius, 7, 7, 0.15);
   sutcliffepolygon(xc,        h4,       radius, 8, 7, 0.15);
   sutcliffepolygon(w4 * 2.0,  h4 * 2.0, radius, 9, 7, 0.15);

   plotend();
   
   return (0);
}

   
void sutcliffepolygon(const double xc, const double yc, const double radius, const int nsides, const int levels, const double ratio)
{
   const double delta = (2.0 * M_PI) / (double)nsides;
   double theta;
   struct Vertex vertex[32];
   int i;
   
   for (i = 0; i < nsides; i++) {
      theta = (double)i * delta;
      vertex[i].x = xc + (cos(theta) * radius);
      vertex[i].y = yc + (sin(theta) * radius);
   }
   
   sutpoly(nsides, vertex, levels, ratio);
}


void sutpoly(const int nsides, const struct Vertex vertex[], const int level, const double ratio)
{
   int i;
   int i1;
   int opp, opp1;
   double dx, dy;
   double x1, y1;
   double x2, y2;
   struct Vertex newv[32];
   
   drawpoly(nsides, vertex);
   
   for (i = 0; i < nsides; i++) {
      i1 = (i + 1) % nsides;
      
      /* Midpoint of the current side of the polygon */
      x1 = (vertex[i].x + vertex[i1].x) / 2.0;
      y1 = (vertex[i].y + vertex[i1].y) / 2.0;
      
      /* Locate the point that we will draw towards */
      if (nsides & 1) {
         opp = (i + (nsides / 2) + 1) % nsides;
         x2 = vertex[opp].x;
         y2 = vertex[opp].y;
      }
      else {
         opp = (i + (nsides / 2)) % nsides;
         opp1 = (opp + 1) % nsides;
         x2 = (vertex[opp].x + vertex[opp1].x) / 2.0;
         y2 = (vertex[opp].y + vertex[opp1].y) / 2.0;
      }
      
      /* Shorten the line (x1,y1)-(x2,y2) by the ratio */
      dx = (x2 - x1) * ratio;
      dy = (y2 - y1) * ratio;
      
      x2 = x1 + dx;
      y2 = y1 + dy;
      
      moveto(x1, y1);
      lineto(x2, y2);
      
      newv[i].x = x2;
      newv[i].y = y2;
   }
   
   if (level > 0)
      sutpoly(nsides, newv, level - 1, ratio);
   else
      drawpoly(nsides, newv);
}


void drawpoly(const int nsides, const struct Vertex vertex[])
{
   int i;
   
   openlinesequence(vertex[0].x, vertex[0].y);

   for (i = 1; i < nsides; i++)
      linesegmentto(vertex[i].x, vertex[i].y);
   
   closelinesequence(1);
}
