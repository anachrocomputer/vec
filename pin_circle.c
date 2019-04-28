/* pin_circle --- draw circular pin-and-cotton              2018-05-03 */
/* Copyright (c) 2018 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "hpgllib.h"


#define MAXPINS   (100)

int draw_pin_circle(const double xc, const double yc, const double r, const int n1, const int n2);

int main(int argc, char * const argv[])
{
/* https://twitter.com/janjarfalk/status/991291613338132480 */
   int n1, n2;
   int nlines;
   double r;
   int opt;
   double xc, yc;
   double maxx, maxy;
   
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
         fprintf(stderr, "Usage: %s [-p pen] [-s <size>] [-t title]\n", argv[0]);
         fprintf(stderr, "       <size> ::= A1 | A2 | A3 | A4 | A5\n");
         exit(EXIT_FAILURE);
      }
   }

   plotbegin(0);

   getplotsize(&maxx, &maxy);
   
   xc = maxx / 2.0;
   yc = maxy / 2.0;
   
   /* Draw square border */
   rectangle(xc - (maxy / 2.0), 0.0, xc + (maxy / 2.0), maxy);
   
   n1 = 24;
   n2 = 5;
   r = (maxy / 2.0) * 0.98;
   
   nlines = draw_pin_circle(xc, yc, r, n1, n2);
   
   plotend();
   
   printf("%d+%d pins, %d lines drawn\n", n1, n2, nlines);

   return (0);
}


int draw_pin_circle(const double xc, const double yc, const double r, const int n1, const int n2)
{
   struct coord {
      double x;
      double y;
   };
   struct coord pin[MAXPINS];
   struct cotton {
      int p1;
      int p2;
      int drawn;
   };
   struct cotton line[1024];
   int i, j;
   int nlines = 0;
   int pin1, pin2;
   const double delta = (M_PI * 2.0) / (double)n1;
   int needmove;
   const int n3 = n2 + 1;
   const int n4 = n1 + n2;
   
   /* Pre-compute coordinates of pins */
   for (i = 0; i < n1; i++) {
      const double theta = (double)i * delta;
      
      pin[i].x = xc + (cos(theta) * r);
      pin[i].y = yc + (sin(theta) * r);
   }
   
   for (i = 0; i < n2; i++) {
      const double theta = ((double)i * delta) + (delta / 2.0);
      
      pin[i + n1].x = xc + (cos(theta) * r);
      pin[i + n1].y = yc + (sin(theta) * r);
   }

   /* Draw lines between pins */
   for (pin1 = 0; pin1 < n3; pin1++) {
      for (i = 0; i < (n1 - n3); i++) {
         pin2 = i + 6;
         
         line[nlines].p1 = pin1;
         line[nlines].p2 = pin2;
         line[nlines].drawn = 0;
         nlines++;
      }
   }
   
   for (pin1 = 24; pin1 < n4; pin1++) {
      for (i = 0; i < (n1 - n3); i++) {
         pin2 = i + n3;
         
         line[nlines].p1 = pin1;
         line[nlines].p2 = pin2;
         line[nlines].drawn = 0;
         nlines++;
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
            
//          printf("Drawing to pin %d.\n", pin2);
         }
         else {
            pin1 = (pin1 + 1) % (n1 * 4);
            needmove = 1;
//          printf("Moving to pin %d\n", pin1);
         }
      } while (j >= nlines);
      
      if (needmove)
         moveto(pin[pin1].x, pin[pin1].y);
         
      lineto(pin[pin2].x, pin[pin2].y);
      line[j].drawn = 1;
      pin1 = pin2;
      needmove = 0;
   }
   
   return (nlines);
}
