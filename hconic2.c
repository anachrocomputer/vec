/* hconic --- HPGL conic section generator                  13/11/2009 */
/* Copyright (c) 2009 BJ, Froods Software Development                  */

/* See "Computational Geometry for Design and Manufacture", by
   I.D.Faux and M.J.Pratt, p34. They refer to "Practical
   Analytical Geometry with Applications to Aircraft", by
   R.A.Liming (1944).

   http://www.jstor.org/pss/3029785
   http://technology.calumet.purdue.edu/met/gneff/Publications/ilin97/ilin97.html
   http://books.google.com/books?id=0SV5G8fgxLoC&pg=PA2&lpg=PA2&dq=liming+analytical+geometry+aircraft&source=bl&ots=hDmpcoe7UF&sig=bHNlmwCWhjfTUoLi1_8kEvUOqOU&hl=en&ei=0J38SsvpFNWB4Qa67Lz3Bg&sa=X&oi=book_result&ct=result&resnum=9&ved=0CCMQ6AEwCA#v=onepage&q=liming%20analytical%20geometry%20aircraft&f=false
 */

#include <stdio.h>
#include <math.h>

#define NPTS   (16)

struct Weights {
   double w0;
   double w1;
   double w2;
};

struct Point {
   double x, y;
};

void drawbox (double x0, double y0, double width, double height);
void straightbox (double x0, double y0, double x1, double y1);
double quadfunc (struct Weights *wp, double x0, double x1, double x2, double u);
void genconic ();
void showcurve (int colr, double xpts[], double ypts[], double wpts[], int n, int move);

int main (int argc, const char *argv[])
{
   int x, y;
   
   printf ("IN;SP1;\n");

   printf ("SP1;PU;PA%d,%d\n", 10 * 40, 10 * 40);
   printf ("DR0,1;\n");
// printf ("LBhttp://www.dorkbot.org/dorkbotbristol%c;\n", 3);
   printf ("LBBristol Hackspace%c;\n", 3);

   for (y = 0; y < 9600; y += 4800)
      for (x = 0; x < 14400; x += 4800)
         drawbox ((double)(x + (20 * 40)), (double)y, 4800.0, 4800.0);
   
   printf ("PU;PA0,0;SP0;\n");
   
   return (0);
}


void drawbox (double x0, double y0, double width, double height)
{
   double xpts[NPTS + 1], ypts[NPTS + 1], wpts[NPTS + 1];
   struct Point p0, p1, p2, p3, p4, p5, p6, p7;
   double w0 = 1.0;
   double w1 = 3.0;
   double w2 = 1.0;
   double xmin, ymin;
   double xmax, ymax;
   double xstep = 240.0;
   double ystep = 240.0;
   int i;
   static double controls[8] = {
      0.0,
      32.0, 16.0, 8.0, 4.0, 2.0, 1.0, 0.5
   };
   
   /* p1    p0    p7
    *
    * p2          p6
    *
    * p3    p4    p5
    */
    
   straightbox (x0, y0, x0 + width, y0 + height);
    
   for (i = 1; i <= 6; i++) {
      xmin = x0 + (i * xstep);
      xmax = (x0 + width) - (i * xstep);
      
      ymin = y0 + (i * ystep);
      ymax = (y0 + height) - (i * ystep);
      
      /* Eight control points */
      p0.x = x0 + (width / 2.0);
      p0.y = ymax;
      
      p1.x = xmin;
      p1.y = ymax;
      
      p2.x = xmin;
      p2.y = y0 + (height / 2.0);
      
      p3.x = xmin;
      p3.y = ymin;

      p4.x = x0 + (width / 2.0);
      p4.y = ymin;
      
      p5.x = xmax;
      p5.y = ymin;
      
      p6.x = xmax;
      p6.y = y0 + (height / 2.0);
      
      p7.x = xmax;
      p7.y = ymax;
      
      w1 = controls[i];
      
      genconic (p0, p1, p2, w0, w1, w2, xpts, ypts, wpts, NPTS);
      showcurve (1, xpts, ypts, wpts, NPTS, 1);

      genconic (p2, p3, p4, w0, w1, w2, xpts, ypts, wpts, NPTS);
      showcurve (1, xpts, ypts, wpts, NPTS, 0); 
                   
      genconic (p4, p5, p6, w0, w1, w2, xpts, ypts, wpts, NPTS);
      showcurve (1, xpts, ypts, wpts, NPTS, 0);
                   
      genconic (p6, p7, p0, w0, w1, w2, xpts, ypts, wpts, NPTS);
      showcurve (1, xpts, ypts, wpts, NPTS, 0); 
   }
   
   printf ("PU;PA%d,%d;CI%d;\n", (int)p4.x, (int)p6.y, (int)((width - (14.0 * xstep)) / 2.0));
}


void straightbox (double x0, double y0, double x1, double y1)
{
   int ix0, iy0, ix1, iy1;
   
   ix0 = (int)(x0 + 0.5);
   iy0 = (int)(y0 + 0.5);

   ix1 = (int)(x1 + 0.5);
   iy1 = (int)(y1 + 0.5);

   printf ("PU;PA%d,%d;", ix0, iy0);
   printf ("PD;PA%d,%d,%d,%d,%d,%d,%d,%d\n", ix1, iy0, ix1, iy1, ix0, iy1, ix0, iy0);
}


/* quadfunc --- rational quadratic function */

double quadfunc (struct Weights *wp, double x0, double x1, double x2, double u)
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

void gencurve (wp, x0, x1, x2, x, n)
struct Weights *wp;
double x0, x1, x2;
double x[];
int n;
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

void genconic (p0, p1, p2, w0, w1, w2, x, y, w, n)
struct Point p0, p1, p2;
double w0, w1, w2;
double x[], y[], w[];
int n;
{
   struct Weights wt;
   
   wt.w0 = w0;
   wt.w1 = w1;
   wt.w2 = w2;
   
   gencurve (&wt, p0.x, p1.x, p2.x, x, n);
   gencurve (&wt, p0.y, p1.y, p2.y, y, n);
   gencurve (&wt, 1.0,  1.0,  1.0,  w, n);
}


/* showcurve --- generate HPGL commands to transfer the curve to the plotter */

void showcurve (colr, xpts, ypts, wpts, n, move)
int colr;
double xpts[], ypts[], wpts[];
int n;
int move;
{
   int i;
   int x, y;
   
#ifdef A4
   wpts[0] *= sqrt (2.0);
#endif

   x = (xpts[0] / wpts[0]) + 0.5;
   y = (ypts[0] / wpts[0]) + 0.5;


   /* Move to starting position */
   if (move)
      printf ("PU;PA%d,%d;\n", x, y);
   
   printf ("PD;PA");

   for (i = 1; i <= n; i++) {
#ifdef A4
      wpts[i] *= sqrt (2.0);
#endif
      x = (xpts[i] / wpts[i]) + 0.5;
      y = (ypts[i] / wpts[i]) + 0.5;

      printf ("%d,%d", x, y);

      if (i < n)
         printf (",");
   }

   printf (";\n");
}
