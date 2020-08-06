/* poly_oval --- plot polygon of ovals                      2020-08-05 */
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

void multi_oval(struct Point *p1, struct Point *p2, const double inner, const double outer, const int n);
void draw_oval(const struct Point *const p1, const struct Point *const p2, const struct Point *const vec90);

int main(int argc, char * const argv[])
{
   int opt;
   int i, j;
   double xc, yc;
   double maxx, maxy;
   double delta;
   int nsides = 5;
   double outer = 40.0;
   double inner = 15.0;
   struct Point pt[16];
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
   
   xc = maxx / 2.0;
   yc = maxy / 2.0;
   
   delta = (2.0 * M_PI) / (double)nsides;
   radius = (maxy / 2.0) - (outer * 40.0);

   for (i = 0; i < nsides; i++) {
      const double theta = (double)i * delta;

      pt[i].x = xc + (radius * cos(theta));
      pt[i].y = yc + (radius * sin(theta));
   }
   
   for (i = 0; i < nsides; i++) {
      j = (i + 1) % nsides;
      
      multi_oval(&pt[i], &pt[j], inner * 40.0, outer * 40.0, 12);
   }
   
   plotend();
   
   return (0);
}


void multi_oval(struct Point *p1, struct Point *p2, const double inner, const double outer, const int n)
{
   int i;
   struct Point vec;
   struct Point vec90;
   const double dx = p1->x - p2->x;
   const double dy = p1->y - p2->y;
   const double mag = sqrt((dx * dx) + (dy * dy));
   const double delta_r = (outer - inner) / (double)(n - 1);
   
   vec.x = dx / mag;
   vec.y = dy / mag;
   
   for (i = 0; i < n; i++) {
      const double r = inner + (i * delta_r);

      vec90.x = vec.y * r;
      vec90.y = -vec.x * r;
      
      draw_oval(p1, p2, &vec90);
   }
}


void draw_oval(const struct Point *const p1, const struct Point *const p2, const struct Point *const vec90)
{
   moveto(p1->x + vec90->x, p1->y + vec90->y);
   arc(p1->x, p1->y, 180.0);
   lineto(p2->x - vec90->x, p2->y - vec90->y);
   arc(p2->x, p2->y, 180.0);
   lineto(p1->x + vec90->x, p1->y + vec90->y);
}
