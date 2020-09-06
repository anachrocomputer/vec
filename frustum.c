/* frustum --- plot the net of a frustum as a container     2020-09-06 */
/* Copyright (c) 2020 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include "hpgllib.h"

struct Trapezoid {
   double angle;
   double x1, y1;
   double x2, y2;
   double x3, y3;
   double x4, y4;
   double xc, yc;    // Centre of decoration
   double xt, yt, r; // Centre and radius of arc to form lid
   double sx1, sy1;  // Edge of slot
   double sx2, sy2;  // Centre of slot
};

void decorate(const int side, const double xc, const double yc, const double wd, const double ht, const double angle);
void rotate(double *x, double *y, const double st, const double ct);
void half_ellipse(const double x0, const double y0, const double a, const double b, const double theta);
void half_superellipse(const double x0, const double y0, const double a, const double b, const double theta, const double d);

int main(int argc, char * const argv[])
{
   int opt;
   int i;
   double xc, yc;
   double maxx, maxy;
   int nsides;
   double delta;
   struct Trapezoid t[32];
   double base;
   double height;
   double top;
   
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

   if (plotbegin(1) < 0) {
      fputs("Failed to initialise HPGL library\n", stderr);
      exit(EXIT_FAILURE);
   }

   getplotsize(&maxx, &maxy);
   
   xc = maxx / 2.0;
   yc = maxy / 2.0;
   
   nsides = 4;
   base = maxy / 6.0;
   height = maxy / 4.0;
   top = base * 1.5;
   
   delta = (2.0 * M_PI) / (double)nsides;
   
   for (i = 0; i < nsides; i++) {
      const double theta = delta * (double)i;
      const double st = sin(theta);
      const double ct = cos(theta);
      
      t[i].angle = theta;
      
      // Locate four corners of trapezoid
      t[i].x1 = ((base / 2.0) / tan(delta / 2.0));
      t[i].y1 = -(base / 2.0);
      
      t[i].x2 = t[i].x1 + height;
      t[i].y2 = -(top / 2.0);
      
      t[i].x3 = t[i].x2;
      t[i].y3 = top / 2.0;
      
      t[i].x4 = t[i].x1;
      t[i].y4 = base / 2.0;
      
      // Locate the middle of the top
      t[i].xt = t[i].x2;
      t[i].yt = 0.0;
      t[i].r = top / 2.0;
      
      // Locate the centre of the trapezoid, for decoration
      t[i].xc = t[i].x1 + (height / 2.0);
      t[i].yc = 0.0;
      
      // Locate the interlocking slots to hold the lid closed
      t[i].sx1 = t[i].x2 + (top / 2.0);
      t[i].sy1 = t[i].y3;
      
      t[i].sx2 = t[i].sx1;
      t[i].sy2 = 0.0;
      
      // Rotate all the points about the origin
      rotate(&t[i].x1, &t[i].y1, st, ct);
      rotate(&t[i].x2, &t[i].y2, st, ct);
      rotate(&t[i].x3, &t[i].y3, st, ct);
      rotate(&t[i].x4, &t[i].y4, st, ct);
      rotate(&t[i].xc, &t[i].yc, st, ct);
      rotate(&t[i].xt, &t[i].yt, st, ct);
      rotate(&t[i].sx1, &t[i].sy1, st, ct);
      rotate(&t[i].sx2, &t[i].sy2, st, ct);
      
      // Translate entire trapezoid to centre of page
      t[i].x1 += xc;
      t[i].y1 += yc;
      
      t[i].x2 += xc;
      t[i].y2 += yc;
      
      t[i].x3 += xc;
      t[i].y3 += yc;
      
      t[i].x4 += xc;
      t[i].y4 += yc;
      
      t[i].xt += xc;
      t[i].yt += yc;
      
      t[i].xc += xc;
      t[i].yc += yc;
      
      t[i].sx1 += xc;
      t[i].sy1 += yc;
      
      t[i].sx2 += xc;
      t[i].sy2 += yc;
   }
   
   // Draw trapezoids
   for (i = 0; i < nsides; i++) {
      moveto(t[i].x1, t[i].y1);
      lineto(t[i].x2, t[i].y2);
      lineto(t[i].x3, t[i].y3);
      lineto(t[i].x4, t[i].y4);
      lineto(t[i].x1, t[i].y1);
      
      // Draw lid flaps
      if ((i == 0) || (i == nsides / 2)) {
         half_superellipse(t[i].xt, t[i].yt, t[i].r, t[i].r * 1.5, t[i].angle - (M_PI / 2.0), 2.8);
         moveto(t[i].sx1, t[i].sy1);   // Draw slot
         lineto(t[i].sx2, t[i].sy2);
      }
      else {
         half_superellipse(t[i].xt, t[i].yt, t[i].r, t[i].r, t[i].angle - (M_PI / 2.0), 2.8);
         //moveto(t[i].x2, t[i].y2);
         //arc(t[i].xt, t[i].yt, 180.0);
      }
      
      decorate(i, t[i].xc, t[i].yc, base / 1.6, base / 2.0, t[i].angle);
   }
   
   // Draw fold lines
   for (i = 0; i < nsides; i++) {
      const int j = (i + 1) % nsides;
      const double midx = (t[i].x3 + t[j].x2) / 2.0;
      const double midy = (t[i].y3 + t[j].y2) / 2.0;
      
      moveto(t[i].x3, t[i].y3);
      lineto(t[j].x2, t[j].y2);
      
      moveto(t[i].x4, t[i].y4);
      lineto(midx, midy);
   }

   plotend();
   
   return (0);
}


void decorate(const int side, const double xc, const double yc, const double wd, const double ht, const double angle)
{
   if ((side % 2) == 0)
      ellipse(xc, yc, wd, ht, angle);
   else
      circle(xc, yc, ht);
}


void rotate(double *x, double *y, const double st, const double ct)
{
   const double newx = (*x * ct) - (*y * st);
   const double newy = (*x * st) + (*y * ct);
   
   *x = newx;
   *y = newy;
}


void half_ellipse(const double x0, const double y0, const double a, const double b, const double theta)
{
   const int npts = 36;
   const double delta = M_PI / (double)npts;
   const double sintheta = sin(theta);
   const double costheta = cos(theta);
   int i;
   
   for (i = 0; i <= npts; i++) {
      const double t = (double)i * delta;
      
      const double x = (a * cos(t) * costheta) - (b * sin(t) * sintheta);
      const double y = (a * cos(t) * sintheta) + (b * sin(t) * costheta);
      
      if (i == 0)
         moveto(x0 + x, y0 + y);
      else
         lineto(x0 + x, y0 + y);
   }
}


void half_superellipse(const double x0, const double y0, const double a, const double b, const double theta, const double d)
{
   const int npts = 36;
   const double delta = M_PI / (double)npts;
   const double sintheta = sin(theta);
   const double costheta = cos(theta);
   int i;

   for (i = 0; i <= npts; i++) {
      double sinpt, cospt;
      const double t = (double)i * delta;

      const double st = sin(t);
      const double ct = cos(t);

      if (st < 0.0)
         sinpt = -pow(-st, 2.0 / d);
      else
         sinpt = pow(st, 2.0 / d);

      if (ct < 0.0)
         cospt = -pow(-ct, 2.0 / d);
      else
         cospt = pow(ct, 2.0 / d);

      const double x = (a * cospt * costheta) - (b * sinpt * sintheta);
      const double y = (a * cospt * sintheta) + (b * sinpt * costheta);

      if (i == 0)
         moveto(x0 + x, y0 + y);
      else
         lineto(x0 + x, y0 + y);
   }
}
