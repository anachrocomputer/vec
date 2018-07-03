/* sqinsq -- draw square-in-square pattern                  2013-01-26 */
/* Copyright (c) 2013 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "hpgllib.h"


int main(int argc, char * const argv[])
{
/* High-Resolution Computer Graphics Using C, by Ian O. Angell, 1989.
   Example 3.1, page 50. */
   int opt;
   double maxx, maxy;
   struct vector2 {
      double x;
      double y;
   };
   struct vector2 ptd[4];
   struct vector2 pt[4];
   int n = 80;
   int t = 7;
   double q;
   double mu, um;
   double xoff;
   int i, j, nextj;
   
   while ((opt = getopt(argc, argv, "no:p:s:t:v:")) != -1) {
      switch (opt) {
      case 'n':
      case 'o':
      case 'p':
      case 's':
      case 't':
      case 'v':
         plotopt(opt, optarg);
         break;
      default: /* '?' */
         fprintf(stderr, "Usage: %s [-p pen] [-s <size>] [-t title]\n",
                  argv[0]);
         fprintf(stderr, "       <size> ::= A1 | A2 | A3 | A4 | A5\n");
         exit(EXIT_FAILURE);
      }
   }

   plotbegin(0);

   getplotsize(&maxx, &maxy);
   
   pt[0].x = maxy;
   pt[0].y = maxy;
   pt[1].x = maxy;
   pt[1].y = 0.0;
   pt[2].x = 0.0;
   pt[2].y = 0.0;
   pt[3].x = 0.0;
   pt[3].y = maxy;

   xoff = (maxx - maxy) / 2.0;
   
   q = tan (t * M_PI / (4.0 * n));
   mu = q / (q + 1.0);
   um = 1.0 - mu;

   /* Draw inner squares */
   for (i = 0; i <= n; i++) {
      openlinesequence(pt[0].x + xoff, pt[0].y);
      
      for (j = 1; j < 4; j++)
         linesegmentto(pt[j].x + xoff, pt[j].y);
      
      closelinesequence(1);

      for (j = 0; j < 4; j++) { 
         nextj = (j + 1) % 4;
         ptd[j].x = (um * pt[j].x) + (mu * pt[nextj].x);
         ptd[j].y = (um * pt[j].y) + (mu * pt[nextj].y);
      }
      
      /* Reset square coordinates */
      for (j = 0; j < 4; j++) {
         pt[j].x = ptd[j].x;
         pt[j].y = ptd[j].y;
      }
   }
   
   plotend();
   
   return (0);
}
