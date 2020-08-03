/* lobe --- plot circular lobes on the plotter              1997-04-27 */
/* Copyright (c) 1997 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <math.h>

#define RADIANS  (M_PI / 180.0)

#define BLACK     (1)
#define RED       (2)
#define GREEN     (3)
#define BLUE      (4)

#define DEV_HPGL  (1)
#define DEV_BMC   (2)

static double Maxx, Maxy;
static double Homex, Homey;
static int Pencol;
static int Pltdev = DEV_HPGL;

void drawlobes(const double a, const double b, const double l);
void moveto(const double x, const double y);
void drawto(const double x, const double y);
void setpen(const int pen);

int main(int argc, const char *argv[])
{
   double a, b, l;
   int i;
   
   if (Pltdev == DEV_HPGL) {
      Maxx = 10870;
      Maxy = 7600;
      
      Maxx = 15970;
      Maxy = 10870;
   }
   else {
      Maxx = 3800.0;
      Maxy = 2550.0;
   
      Maxx /= 1.4142;
      Maxy /= 1.4142;
   }
   
   Homex = Maxx / 2.0;
   Homey = Maxy / 2.0;
   
   Pencol = BLACK;
   
   if (Pltdev == DEV_HPGL)
      printf("SP%d;\n", Pencol);
   else
      printf("IP1;PS%d;\n", Pencol);
   
   a = Maxy / 4.0;
   b = a / 3.0;
   
   for (i = 0; i < 9; i++) {
      l = (double)i;
      switch (i % 4) {
      case 0:
         setpen(BLACK);
         break;
      case 1:
         setpen(RED);
         break;
      case 2:
         setpen(GREEN);
         break;
      case 3:
         setpen(BLUE);
         break;
      }

      drawlobes(a, b, l);
   }

   if (Pltdev == DEV_HPGL)
      printf("SP0;\n");
   else
      printf("MA0,0;CH;\n");
      
   return (0);
}


void drawlobes(const double a, const double b, const double l)
{
   double r, theta;
   double x, y;
   int i;

   moveto(Homex + a + b, Homey);
   
   for (i = 1; i <= 360; i++) {
      theta = i * RADIANS;
      r = a + (b * cos(l * theta));
      
      x = r * cos(theta);
      y = r * sin(theta);
      
      drawto(Homex + x, Homey + y);
   }
}


void moveto(const double x, const double y)
{
   if (Pltdev == DEV_HPGL)
      printf("PU;PA%d,%d\n", (int)x, (int)y);
   else
      printf("MA%d,%d\n", (int)x, (int)y);
}


void drawto(const double x, const double y)
{
   if (Pltdev == DEV_HPGL)
      printf("PD;PA%d,%d\n", (int)x, (int)y);
   else
      printf("DA%d,%d\n", (int)x, (int)y);
}


void setpen(const int pen)
{
   if (Pltdev == DEV_HPGL)
      printf("SP%d;\n", pen);
   else
      printf("PS%d\n", pen);
}
