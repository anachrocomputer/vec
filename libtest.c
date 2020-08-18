/* libtest --- test the HPGL library                        2020-08-10 */
/* Copyright (c) 2020 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include "hpgllib.h"

struct Point {
   double x;
   double y;
};

void lineTest(const double xc, const double yc, const double wd, const double ht, const PlotInfo_t *const info);
void circleTest(const double xc, const double yc, const double wd, const double ht, const PlotInfo_t *const info);
void arcTest(const double xc, const double yc, const double wd, const double ht, const PlotInfo_t *const info);
void textTest(const double xc, const double yc, const double wd, const double ht, const PlotInfo_t *const info);

int main(int argc, char * const argv[])
{
   int opt;
   double xc, yc;
   double h4, w4;
   double maxx, maxy;
   PlotInfo_t info;
   
   while ((opt = getopt(argc, argv, "no:p:q:s:t:v:")) != -1) {
      switch (opt) {
      case 's':
      case 'n':
      case 'o':
      case 'p':
      case 'q':
      case 't':
      case 'v':
         plotopt(opt, optarg);
         break;
      default: /* '?' */
         fprintf(stderr, "Usage: %s [-p pen] [-q plotter] [-s <size>] [-t title]\n", argv[0]);
         fprintf(stderr, "       <size> ::= A1 | A2 | A3 | A4 | A5\n");
         exit(EXIT_FAILURE);
      }
   }

   if (plotbegin(1) < 0) {
      fputs("Failed to initialise HPGL library\n", stderr);
      exit(EXIT_FAILURE);
   }

   if (getplotinfo(&info, sizeof (info)) < 0) {
      fputs("Failed to get plot information\n", stderr);
      exit(EXIT_FAILURE);
   }

   getplotsize(&maxx, &maxy);
   
   xc = maxx / 2.0;
   yc = maxy / 2.0;
   
   w4 = maxx / 4.0;
   h4 = maxy / 4.0;
   
   moveto(0.0, yc);
   lineto(maxx, yc);
   
   moveto(xc, 0.0);
   lineto(xc, maxy);
   
   lineTest(w4, yc + h4, w4, h4, &info);
   circleTest(w4, h4, w4, h4, &info);
   arcTest(xc + w4, yc + h4, w4, h4, &info);
   textTest(xc + w4, h4, w4, h4, &info);
   
   plotend();
   
   return (0);
}


void lineTest(const double xc, const double yc, const double wd, const double ht, const PlotInfo_t *const info)
{
   const double len = wd / 16.0;
   
   moveto(xc - len, yc);
   lineto(xc + len, yc);
   
   moveto(xc, yc - len);
   lineto(xc, yc + len);
   
   hlabel(xc - (wd / 2.0), yc - (ht / 2.0), 5.0, info->plotterName);
   hlabel(xc + (wd / 2.0), yc - (ht / 2.0), 5.0, info->paperName);
}


void circleTest(const double xc, const double yc, const double wd, const double ht, const PlotInfo_t *const info)
{
   const double major = wd / 2.0;
   const double minor = ht / 2.0;
   
   circle(xc, yc, minor);
   ellipse(xc, yc, major, minor, 0.0);
   ellipse(xc, yc, major, minor, (M_PI / 4.0));
   ellipse(xc, yc, major, minor, (M_PI / 2.0));
   circle(xc, yc, major);
}


void arcTest(const double xc, const double yc, const double wd, const double ht, const PlotInfo_t *const info)
{
   double radius = wd / 2.0;
   const double delta = M_PI / 4.0;
   double angle;
   int i;
   
   moveto(xc - (wd / 2.0), yc);
   lineto(xc, yc);
   lineto(xc, yc + (wd / 2.0));
   
   for (i = 0; i < 7; i++) {
      radius = (wd / 16.0) * (i + 1);
      angle = 45.0 * (i + 1);
      
      moveto(xc - radius, yc);
      arc(xc, yc, angle);
   }
}


void textTest(const double xc, const double yc, const double wd, const double ht, const PlotInfo_t *const info)
{
   const double len = wd / 16.0;
   
   moveto(xc - len, yc);
   lineto(xc + len, yc);
   
   moveto(xc, yc - len);
   lineto(xc, yc + len);
   
   hlabel(xc, yc, 10.0, "ABCDEF");
// hlabel(xc, yc + (len * 2.0), 10.0, "\x99\x9F\xEC");
}
