/* ternary_arcs --- plot arcs or lines to form 81 squares   2019-04-22 */
/* Copyright (c) 2019 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include "hpgllib.h"

void draw_arcs(const double x, const double y, const double r, const char str[]);

int main(int argc, char * const argv[])
{
   /* Inspired by a #plottertwitter tweet:
    * https://twitter.com/arjanvandermeij/status/1112840850059481088
    */
   int opt;
   int i, j;
   double height;
   double xc, yc;
   double x0, y0;
   double maxx, maxy;
   double left, right;
   double gridsz;
   double radius;
   char str[8];
   
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

   if (plotbegin(0) < 0) {
      fputs("Failed to initialise HPGL library\n", stderr);
      exit(EXIT_FAILURE);
   }

   getplotsize(&maxx, &maxy);
   
   xc = maxx / 2.0;
   yc = maxy / 2.0;
   height = maxy;
   gridsz = height / 9.0;
   radius = gridsz * 0.45;
   left = xc - (height / 2.0);
   right = xc + (height / 2.0);
   
   /* Draw square border */
   rectangle(left, 0.0, right, maxy);

   for (i = 0; i < 9; i++) {
      for (j = 0; j < 9; j++) {
         const int n = (i * 9) + j;
         
         str[0] = ((n / 27) % 3) + '0';
         str[1] = ((n / 9) % 3) + '0';
         str[2] = ((n / 3) % 3) + '0';
         str[3] = (n % 3) + '0';
         str[4] = '\0';
         
         printf("%2d %s ", n, str);
         
         x0 = left + (j * gridsz) + (gridsz / 2.0);
         y0 = (i * gridsz) + (gridsz / 2.0);
         draw_arcs(x0, y0, radius, str);
      }
      
      printf("\n");
   }
   
   plotend();
   
   return (0);
}


void draw_arcs(const double x, const double y, const double r, const char str[])
{
   moveto(x, y + r);

   if (str[0] == '0') {
      arc(x, y, 90.0);
   }
   else if (str[0] == '1') {
      lineto(x - r, y);
   }
   else {
      arc(x - r, y + r, -90.0);
   }
   
   if (str[1] == '0') {
      arc(x, y, 90.0);
   }
   else if (str[1] == '1') {
      lineto(x, y - r);
   }
   else {
      arc(x - r, y - r, -90.0);
   }
   
   if (str[2] == '0') {
      arc(x, y, 90.0);
   }
   else if (str[2] == '1') {
      lineto(x + r, y);
   }
   else {
      arc(x + r, y - r, -90.0);
   }
   
   if (str[3] == '0') {
      arc(x, y, 90.0);
   }
   else if (str[3] == '1') {
      lineto(x, y + r);
   }
   else {
      arc(x + r, y + r, -90.0);
   }
}
