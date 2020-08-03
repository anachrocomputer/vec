/* hyp --- draw hyperbolic geometry                         2001-04-24 */
/* Copyright (c) 2001 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <math.h>

#define A3

void infline(const double ctrang, const double incang, const int move);
void moveto(const double x, const double y);
void lineto(const double x, const double y);
void circle(const double x, const double y, const double r);
void arc(const double x, const double y, const double a);
void pencolr(int c);

#ifdef A3
#define MAXX (15970.0)
#define MAXY (10870.0)
#else
#define MAXX (10870.0)
#define MAXY (7600.0)
#endif   /* A3 */

#define DEG_TO_RAD  (M_PI / 180.0)

double X0, Y0;
double Rad;

int main(int argc, const char *argv[])
{
   int i;
   int level;
   int num;
   int first;
   double theta, phi;
   
   X0 = MAXX / 2.0;
   Y0 = MAXY / 2.0;
   Rad = Y0;
   
   printf("SP1;\n");
   /* printf("VS10;\n");   * Slow down for shite pens */

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

   printf("SP0;\n");
   
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


void moveto(const double x, const double y)
{
   printf("PU;PA%d,%d;\n", (int)x, (int)y);
}


void lineto(const double x, const double y)
{
   printf("PD;PA%d,%d;\n", (int)x, (int)y);
}


void circle(const double x, const double y, const double r)
{
   printf("PU;PA%d,%d;CI%d;\n", (int)x, (int)y, (int)r);
}


void arc(const double x, const double y, const double a)
{
   printf("PD;AA%d,%d,%2.1f;\n", (int)x, (int)y, a);
}


void pencolr(int c)
{
   c %= 4;
   
   printf("SP%d;", c + 1);
}
