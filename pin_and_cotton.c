/* pin_and_cotton --- draw pin-and-cotton                   2013-01-24 */
/* Copyright (c) 2013 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "hpgllib.h"


#define MAXPINS   (400)
#define MAXFIBS   (30)
#define BOTTOM    1
#define RIGHT     2
#define TOP       4
#define LEFT      8


int main (int argc, char * const argv[])
{
/* High-Resolution Computer Graphics Using C, by Ian O. Angell, 1989.
   Exercise 1.10, page 21. */
   struct coord {
      double x;
      double y;
      int side;
   };
   struct coord pin[MAXPINS];
   struct cotton {
      int p1;
      int p2;
      int drawn;
   };
   struct cotton line[1024];
   int i, j;
   int n;
   int pin1, pin2;
   int needmove;
   int nfib;
   int fibonacci[MAXFIBS];
   int nlines;
   double d;
   double xoff;
   int opt;
   double xc, yc;
   double maxx, maxy;
   
   while ((opt = getopt (argc, argv, "no:p:s:t:v:")) != -1) {
      switch (opt) {
      case 'n':
      case 'o':
      case 'p':
      case 's':
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

   plotbegin (0);

   getplotsize (&maxx, &maxy);
   
   xc = maxx / 2.0;
   yc = maxy / 2.0;
   
   /* Draw square border */
   rectangle (xc - (maxy / 2.0), 0.0, xc + (maxy / 2.0), maxy);
   
   xoff = (maxx - maxy) / 2.0;
   
   /* Pre-compute coordinates of pins */
   /* n = 13; */
   n = 12;
   for (i = 0; i < n; i++) {
      d = (double)i * (maxy / (double)n);
      
      pin[i].x = d + xoff;
      pin[i].y = 0.0;
      pin[i].side = BOTTOM;

      pin[i + n].x = maxy + xoff;
      pin[i + n].y = d;
      pin[i + n].side = RIGHT;

      pin[i + (n * 2)].x = (maxy - d) + xoff;
      pin[i + (n * 2)].y = maxy;
      pin[i + (n * 2)].side = TOP;

      pin[i + (n * 3)].x = xoff;
      pin[i + (n * 3)].y = maxy - d;
      pin[i + (n * 3)].side = LEFT;
   }
   
   pin[0].side |= LEFT;
   pin[n].side |= BOTTOM;
   pin[n * 2].side |= RIGHT;
   pin[n * 3].side |= TOP;

   /* Fill in array of Fibonacci numbers */
   fibonacci[0] = 1;
   fibonacci[1] = 2;
   
   for (i = 2; i < MAXFIBS; i++) {
      fibonacci[i] = fibonacci[i - 1] + fibonacci[i - 2];
      
      if (fibonacci[i] < (4 * n))
         nfib = i + 1;
   }

   /* Draw lines between pins */
   nlines = 0;
   
   for (pin1 = 0; pin1 < (n * 4); pin1++) {
      for (i = 0; i < nfib; i++) {
         pin2 = (pin1 + fibonacci[i]) % (n * 4);
         
         if ((pin[pin1].side & pin[pin2].side) == 0) {
            line[nlines].p1 = pin1;
            line[nlines].p2 = pin2;
            line[nlines].drawn = 0;
            nlines++;
         }
      }
   }
   
   pin1 = 0;
   needmove = 1;

   for (i = 0; i < nlines; i++) {
      do {
         for (j = 0; j < nlines; j++) {
            if (line[j].drawn == 0 && (line[j].p1 == pin1 || line[j].p2 == pin1))
               break;
         }
         
         if (j < nlines) {
            if (line[j].p1 == pin1)
               pin2 = line[j].p2;
            else
               pin2 = line[j].p1;
            
//          printf ("Drawing to pin %d.\n", pin2);
         }
         else {
            pin1 = (pin1 + 1) % (n * 4);
            needmove = 1;
//          printf ("Moving to pin %d\n", pin1);
         }
      } while (j >= nlines);
      
      if (needmove)
         moveto (pin[pin1].x, pin[pin1].y);
         
      lineto (pin[pin2].x, pin[pin2].y);
      line[j].drawn = 1;
      pin1 = pin2;
      needmove = 0;
   }
   
   
   plotend ();
   
   printf ("%d pins, %d lines drawn\n", n * 4, nlines);

   return (0);
}
