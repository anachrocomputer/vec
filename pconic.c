/* pconic --- plot Bezier curves */

/* Note: this program does not yet work properly with the HPGL
 * library. It was originally written for a non-HPGL plotter.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "hpgllib.h"

#define NPTS   (16)

struct Weights {
   double w0;
   double w1;
   double w2;
};

struct Point {
   double x, y;
};

double quadfunc (struct Weights *wp, double x0, double x1, double x2,
                 double u);
void genconic ();
void gencurve (struct Weights *wp, double x0, double x1, double x2,
               double x[], int n);
void showcurve (int colr, double xpts[], double ypts[], double wpts[], int n);


int main (int argc, char * const argv[])
{
   int opt;
   double maxx, maxy;
   static struct Point p0 = {
      180.0 * 40.0, 250.0 * 40.0
   };
   static struct Point p1 = {
      0.0,          250.0 * 40.0
   };           
   static struct Point p2 = {
      0.0,          125.0 * 40.0
   };
   static struct Point p3 = {
      0.0,          0.0
   };
   static struct Point p4 = {
      180.0 * 40.0, 0.0
   };
   static struct Point p5 = {
      360.0 * 40.0, 0.0
   };
   static struct Point p6 = {
      360.0 * 40.0, 125.0 * 40.0 
   };
   static struct Point p7 = {
      360.0 * 40.0, 250.0 * 40.0
   };
   double xpts[NPTS + 1], ypts[NPTS + 1], wpts[NPTS + 1];
   struct Weights wt;
   int i;

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
         fprintf (stderr, "Usage: %s [-p pen] [-s <size>] [-t title]\n", argv[0]);
         fprintf (stderr, "       <size> ::= A1 | A2 | A3 | A4 | A5\n");
         exit (EXIT_FAILURE);
      }
   }

   /* Select first pen and draw border */
   plotbegin (1);
// printf ("IP1;PS3;\n");

   getplotsize (&maxx, &maxy);
   
   wt.w0 = 1.0;
   wt.w1 = 3.0;
   wt.w2 = 1.0;
   
   for (i = 0; i <  10; i++) {
      p1.x = (8.0 * 40.0) * i;
      p2.x = p1.x;
      p3.x = p1.x;
      p5.x = (360.0 * 40.0) - ((8.0 * 40.0) * i);
      p6.x = p5.x;
      p7.x = p5.x;
      p0.y = (250.0 * 40.0) - ((8.0 * 40.0) * i);
      p1.y = p0.y;
      p7.y = p0.y;
      
      genconic (p0, p1, p2, &wt, xpts, ypts, wpts, NPTS);
      showcurve (1, xpts, ypts, wpts, NPTS);

      genconic (p2, p3, p4, &wt, xpts, ypts, wpts, NPTS);
      showcurve (1, xpts, ypts, wpts, NPTS);
      
      genconic (p4, p5, p6, &wt, xpts, ypts, wpts, NPTS);
      showcurve (1, xpts, ypts, wpts, NPTS);

      genconic (p6, p7, p0, &wt, xpts, ypts, wpts, NPTS);
      showcurve (1, xpts, ypts, wpts, NPTS);
   }
   
// printf ("MA0,0;CH\n");
   plotend ();
   
   return (0);
}


/* quadfunc --- rational quadratic function */

double quadfunc (struct Weights *wp, double x0, double x1, double x2,
                 double u)
{
   double t0, t1, t2;
   
   t0 = 1.0 - u;
   t0 *= t0;
   t0 *= x0 * wp->w0;
   
   t1 = u * (1.0 - u);
   t1 *= 2.0;
   t1 *= x1 * wp->w1;
   
   t2 = u * u;
   t2 *= x2 * wp->w2;
   
   return (t0 + t1 + t2);
}


/* gencurve --- generate 'n + 1' points on the curve */

void gencurve (struct Weights *wp, double x0, double x1, double x2,
               double x[], int n)
{
   double t;      /* The parameter, 0..1 */
   double step;   /* The increment in 't' for each step */
   int i;
   
   step = 1.0 / (double)n;
   
   for (i = 0; i <= n; i++) {
      t = i * step;
      x[i] = quadfunc (wp, x0, x1, x2, t);
   }
}


/* genconic --- generate a conic section from 'p0' to 'p2' */

void genconic (p0, p1, p2, wt, x, y, w, n)
struct Point p0, p1, p2;
struct Weights *wt;
double x[], y[], w[];
int n;
{
   gencurve (wt, p0.x, p1.x, p2.x, x, n);
   gencurve (wt, p0.y, p1.y, p2.y, y, n);
   gencurve (wt, 1.0,  1.0,  1.0,  w, n);
}


void showcurve (colr, xpts, ypts, wpts, n)
int colr;
double xpts[], ypts[], wpts[];
int n;
{
   int i;
   int x, y;
   
   x = (xpts[0] / wpts[0]) + 0.5;
   y = (ypts[0] / wpts[0]) + 0.5;
// printf ("MA%d,%d\n", y, x);
   printf ("PU;PA%d,%d;\n", x, y);
   
// printf ("DA");
   printf ("PD;PA");
   for (i = 1; i <= n; i++) {
      x = (xpts[i] / wpts[i]) + 0.5;
      y = (ypts[i] / wpts[i]) + 0.5;
      printf ("%05d,%05d", x, y);
      if (i < n)
         printf (",");
   }
// printf ("\n");
   printf (";\n");
}
