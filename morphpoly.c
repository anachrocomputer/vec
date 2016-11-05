/* morphpoly --- plot a morphed polygon                     2016-11-03 */
/* Copyright (c) 2016 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include "hpgllib.h"

/* Inspired by Manfred Mohr's "Lady Quark" of 1972, seen in */
/* figure 1.3 of "Generative Art" by Matt Pearson, ISBN 1935182625 */

// #define LADY_QUARK

struct Vertex {
   double x;
   double y;
};

int generate2polygons (const double radius, const double angle, const int nsides, const int nsubdiv, struct Vertex outer[], struct Vertex inner[]);
void splitside (const struct Vertex v1, const struct Vertex v2, const int nsubdiv, struct Vertex *result);
void morphpolygon (const double xc, const double yc, const int nsides, const struct Vertex inner[], const struct Vertex outer[], const int nsteps);
void drawpoly (const int nsides, const double xc, const double yc, const struct Vertex vertex[]);


int main (int argc, char * const argv[])
{
   int opt;
   int nsides;
   double radius;
#ifdef LADY_QUARK
   int x, y;
   double xc, yc;
   double w3, h3;
#else
   double w4, h4;
#endif
   double maxx, maxy;
   struct Vertex inner[64], outer[64];
   
   while ((opt = getopt (argc, argv, "no:p:s:t:v:")) != -1) {
      switch (opt) {
      case 's':
      case 'n':
      case 'o':
      case 'p':
      case 't':
      case 'v':
         plotopt (opt, optarg);
         break;
      default: /* '?' */
         fprintf (stderr, "Usage: %s [-p pen] [-s <size>] [-t title]\n",
                  argv[0]);
         fprintf (stderr, "       <size> ::= A1 | A2 | A3 | A4 | A5\n");
         exit (EXIT_FAILURE);
      }
   }

   srand ((unsigned int)time (NULL));

   plotbegin (1);

   getplotsize (&maxx, &maxy);
   
#ifdef LADY_QUARK
   radius = (maxy / 3.0) / sqrt (2.0);

   w3 = maxx / 3.0;
   h3 = maxy / 3.0;

   for (y = 0; y < 3; y++) {
      yc = (h3 / 2.0) + (h3 * y);
      
      for (x = 0; x < 3; x++) {
         xc = (h3 / 2.0) + (h3 * x) + ((maxx - maxy) / 2.0);

         nsides = generate2polygons (radius, M_PI / 4.0, 4, 4, outer, inner);
         morphpolygon (xc, yc, nsides, outer, inner, 20);
      }
   }

#else
   radius = maxy / 4.0;

   w4 = maxx / 4.0;
   h4 = maxy / 4.0;
   
   nsides = generate2polygons (radius, 0.0, 4, 4, outer, inner);
   morphpolygon (w4 * 3.0, h4 * 3.0, nsides, outer, inner, 12);

   nsides = generate2polygons (radius, 0.0, 5, 4, outer, inner);
   morphpolygon (w4,       h4 * 3.0, nsides, outer, inner, 12);

   nsides = generate2polygons (radius, 0.0, 6, 4, outer, inner);
   morphpolygon (w4 * 3.0, h4,       nsides, outer, inner, 12);

   nsides = generate2polygons (radius, 0.0, 7, 4, outer, inner);
   morphpolygon (w4,       h4,       nsides, outer, inner, 12);
#endif

   plotend ();
   
   return (0);
}

   
int generate2polygons (const double radius, const double angle,  const int nsides,
                       const int nsubdiv, struct Vertex outer[],
                       struct Vertex inner[])
{
   const double rad = radius * 0.2;
   int i, i1;
   const double delta = (2.0 * M_PI) / (double)nsides;
   double theta;
   struct Vertex vertex[64];
   struct Vertex jiggle;
   
   for (i = 0; i < nsides; i++) {
      theta = (double)i * delta;
      vertex[i].x = cos (theta + angle) * radius;
      vertex[i].y = sin (theta + angle) * radius;
   }
   
   for (i = 0; i < nsides; i++) {
      i1 = (i + 1) % nsides;
      
      splitside (vertex[i1], vertex[i], nsubdiv, &outer[i * (nsubdiv + 1)]);
   }

   for (i = 0; i < (nsides * (nsubdiv + 1)); i++) {
      jiggle.x = (((double)rand () / (double)RAND_MAX) * rad) - (rad / 2.0);
      jiggle.y = (((double)rand () / (double)RAND_MAX) * rad) - (rad / 2.0);
      inner[i].x = (outer[i].x / 3.0) + jiggle.x;
      inner[i].y = (outer[i].y / 3.0) + jiggle.y;
   }
   
   return (nsides * (nsubdiv + 1));
}


void splitside (const struct Vertex v1, const struct Vertex v2, const int nsubdiv, struct Vertex *result)
{

   int i;
   
   for (i = 0; i <= nsubdiv; i++) {
      const double p1 = i / (double)nsubdiv;
      const double p2 = 1.0 - p1;

      result[i].x = (v1.x * p1) + (v2.x * p2);
      result[i].y = (v1.y * p1) + (v2.y * p2);
   }
}


void morphpolygon (const double xc, const double yc, const int nsides, const struct Vertex outer[], const struct Vertex inner[], const int nsteps)
{
   int i, j;
   const double delta = 1.0 / nsteps;
   struct Vertex vertex[64];
   
   drawpoly (nsides, xc, yc, outer);
   
   for (i = 1; i < nsteps; i++) {
      const double innerportion = delta * i;
      const double outerportion = delta * (nsteps - i);
      
      for (j = 0; j < nsides; j++) {
         vertex[j].x = (outer[j].x * outerportion) + (inner[j].x * innerportion);
         vertex[j].y = (outer[j].y * outerportion) + (inner[j].y * innerportion);
      }

      drawpoly (nsides, xc, yc, vertex);
   }

   drawpoly (nsides, xc, yc, inner);
}


void drawpoly (const int nsides, const double xc, const double yc, const struct Vertex vertex[])
{
   int i;
   
   openlinesequence (xc + vertex[0].x, yc + vertex[0].y);

   for (i = 1; i < nsides; i++) {
      linesegmentto (xc + vertex[i].x, yc + vertex[i].y);
//    circle (xc + vertex[i].x, yc + vertex[i].y, 20.0);
   }

   closelinesequence (1);
}
