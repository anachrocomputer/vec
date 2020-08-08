/* hyp --- draw hyperbolic geometry                         2001-04-24 */
/* Copyright (c) 2001 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "hpgllib.h"


void infline(const double ctrang, const double incang, const int move);


#define DEG_TO_RAD  (M_PI / 180.0)

double X0, Y0;
double Rad;

int main(int argc, char *const argv[])
{
   int opt;
   int i;
   int level;
   int num;
   int first;
   double theta, phi;
   
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

   plotbegin(0);
   
   getplotsize(&X0, &Y0);

   X0 /= 2.0;
   Y0 /= 2.0;
   Rad = Y0;

   /* Plot centre of main circle */
   moveto(X0 + 200.0, Y0);
   lineto(X0 - 200.0, Y0);
   
   moveto(X0, Y0 + 200.0);
   lineto(X0, Y0 - 200.0);
   
   /* Plot main circle */
   circle(X0, Y0, Rad);
   
   num = 3;
   phi = 120.0;
   
   for (level = 0; level < 6; level++) {
      /* pencolr(level); */
      
      theta = phi / 2.0;

      first = 1;
      for (i = 0; i < num; i++) {
         infline(theta, phi, first);
         first = 0;
      
         theta -= phi;
      }
      
      phi /= 2.0;
      num *= 2;
   }

   plotend();
   
   return (0);
}


/* infline --- draw a single, infinitely long line in hyperbolic space */

void infline(const double ctrang, const double incang, const int move)
{
   double a; /* Angle of start of arc */
   double b; /* Angle of centre of arc, in radians */
   double c; /* Angle included in arc, in degrees */
   double d; /* Distance of centre of arc from origin */
   double x, y;
   
   /* Move pen to start of arc */
   a = (ctrang + (incang / 2.0)) * DEG_TO_RAD;
   if (move)
      moveto(X0 + (Rad * cos(a)), Y0 + (Rad * sin(a)));

   b = ctrang * DEG_TO_RAD;
   c = 180.0 - incang;

   d = Rad / cos((incang / 2.0) * DEG_TO_RAD);
   
   x = d * cos(b);
   y = d * sin(b);
   
   arc(X0 + x, Y0 + y, c);
}
