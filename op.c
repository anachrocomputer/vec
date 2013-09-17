/* op --- Op-art generator                                 1999-08-04 */
/* Copyright (c) 1999 BJ, Froods Software Development                  */

/* Inspired by an article in Personal Computer World magazine,
 * volume 5 number 4 April 1982, page 134. An article called
 * "Patterns" by Alan Sutcliffe. Originally in FORTRAN, the
 * program was converted to BASIC for the magazine.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
/* #include <graph.h> */

/* Takes about 28 minutes to plot on DPX-3300 at A1 */
#define A3

#define YES    (1)
#define NO     (0)

int Devxmin, Devymin;
int Devxmax, Devymax;

static enum {GR_PS, GR_HPGL, GR_BMC} GraphDev = GR_HPGL;

struct pt {
   int x;
   int y;
};

struct CoOrd {
   double x;
   double y;
};

void drawbox (int row, int col, struct CoOrd param[5], double zd, double width);
void drawtri (int row, int col, int ineg, struct CoOrd nbox[5], double zd);
void drawpoly (struct CoOrd *box, int nsides);
int shrink (struct CoOrd *box, struct CoOrd *nbox, double zd, int nsides, int *ineg);
double rnd (void);

int OpenGraph (void);
void CloseGraph (void);


int main (int argc, const char *argv[])
{
#ifdef A1
   int nrows = 12;
   int ncols = 18;
#else
   int nrows =  6;   /* A4: 9, A3: 12 */
   int ncols =  8;   /* A4: 9, A3: 18 */
#endif
   double step = 2.0;
   int maxx = ncols + 1;
   int maxy = nrows + 1;
   int size = 40;    /* Size of boxes, in mm */
   struct CoOrd grid[30][30];
   struct CoOrd box[4];
   double x, y;
   double r;
   int row, col;
   int ix, iy;
   
   srand ((unsigned int)time (NULL));
   
   if (OpenGraph () == NO) {
      fprintf (stderr, "Could not start up graphics\n");
      exit (1);
   }
   
   printf ("SP1;PU;PA%d,%d\n", Devxmin + (10 * 40), Devymin + (10 * 40));
   printf ("DR0,1;\n");
// printf ("LBhttp://www.dorkbot.org/dorkbotbristol%c;\n", 3);
   printf ("LBBristol Hackspace%c;\n", 3);

   /* Build the regular grid */
   for (ix = 0; ix < maxx; ix++) {
      for (iy = 0; iy < 2; iy++) {
         x = (double)(ix + 1);
         y = (double)iy;

         grid[ix][iy].x = x * size;
         grid[ix][iy].y = y * size;
      }
   }

   /* Build the randomised grid */
   for (ix = 0; ix < maxx; ix++) {
      for (iy = 2; iy < maxy; iy++) {
         x = (double)(ix + 1);
         y = (double)iy;
         r = (y / (double)maxy) * size;

         grid[ix][iy].x = (x * size) - ((rnd () - 0.5) * r);
         grid[ix][iy].y = (y * size) - ((rnd () - 0.5) * r);
      }
   }
   
   /* Loop over the grid, drawing the boxes */
   for (row = 0; row < nrows; row++) {
      for (col = 0; col < ncols; col++) {
         box[0] = grid[col][row];
         box[1] = grid[col][row+1];
         box[2] = grid[col+1][row+1];
         box[3] = grid[col+1][row];
         drawbox (row, col, box, step, 2.0 - ((double)row / (double)nrows));
      }
   }
   
   CloseGraph ();

   return (0);
}


/* drawbox --- draw a single four-sided box */

void drawbox (int row, int col, struct CoOrd param[4], double step, double width)
{
   int i;
   struct CoOrd box[6], nbox[5];
   int nneg;
   int ineg;
   double zd;

   if (GraphDev == GR_PS)
      printf ("%f setlinewidth\n", width);

   for (i = 0; i < 4; i++) {
      box[i] = param[i];
   }

   box[4] = box[0];
   box[5] = box[1];

   drawpoly (box, 4);      /* Draw boxes */
      
   for (i = 0; i < 20; i++) {
      zd = (i + 1) * step;
      
      nneg = shrink (box, nbox, zd, 4, &ineg);
      
      if (nneg > 0)
         break;

      drawpoly (nbox, 4);     /* Draw smaller boxes */
   }

   if (nneg == 1)
      drawtri (row, col, ineg, nbox, step);
}


/* drawtri --- draw boxes when reduced to three sides */

void drawtri (int row, int col, int ineg, struct CoOrd nbox[5], double zd)
{
   int i, j;
   double m1, c1, m2, c2;
   struct CoOrd box[6];
   int nneg, junk;
   
   /* Copy co-ords of "good" side */
   for (i = 0; i < 5; i++) {
      j = (ineg + 2 + i) % 4;
      
      box[i] = nbox[j];
   }
   
   /* Avoid division-by-zero */
   if (box[1].y == box[2].y) {
      box[1].y += 0.001;
#ifdef DB
      fputs ("Tilt 1\n", stderr);
#endif
   }
      
   /* Find equation of line [2] - [1] */
   m1 = (box[2].x - box[1].x) / (box[1].y - box[2].y);
   c1 = -box[1].x - (m1 * box[1].y);

   /* Avoid division-by-zero */
   if (box[0].y == box[3].y) {
      box[0].y += 0.001;
#ifdef DB
      fputs ("Tilt 2\n", stderr);
#endif
   }
      
   /* Find equation of line [0] - [3] */
   m2 = (box[0].x - box[3].x) / (box[3].y - box[0].y);
   c2 = -box[0].x - (m2 * box[0].y);
   
   /* If gradients are equal, lines are parallel */
   if (fabs (m2 - m1) < 0.00001)
      return;
      
   /* Find point of intersection */
   nbox[2].y = (c1 - c2) / (m2 - m1);
   nbox[2].x = (-m1 * nbox[2].y) - c1;
   
   nbox[0] = box[0];
   nbox[1] = box[1];
   
#ifdef DB
   fprintf (stderr, "(%d,%d): tri%d: (%.1f,%.1f) (%.1f,%.1f) (%.1f,%.1f)\n", row, col, ineg, nbox[0].x, nbox[0].y, nbox[1].x, nbox[1].y, nbox[2].x, nbox[2].y);
   
   printf ("SP2;PU;PA%d,%d;PD;PA%d,%d,%d,%d,%d,%d;SP1;\n", (int)(nbox[0].x * 40.0), (int)(nbox[0].y * 40.0), (int)(nbox[1].x * 40.0), (int)(nbox[1].y * 40.0), (int)(nbox[2].x * 40.0), (int)(nbox[2].y * 40.0), (int)(nbox[0].x * 40.0), (int)(nbox[0].y * 40.0));
#endif

   for (i = 0; i < 3; i++) {
      box[i] = nbox[i];
   }

   for (i = 0; i < 20; i++) {
      box[3] = box[0];
      box[4] = box[1];

      drawpoly (box, 3);       /* Draw triangles */
      
      nneg = shrink (box, nbox, zd, 3, &junk);
      
      if (nneg > 0) {
#ifdef DB
         fprintf (stderr, "tri: row %d, col %d, %d neg sides\n", row, col, nneg);
#endif
         return;
      }

      for (j = 0; j < 3; j++) {
         box[j] = nbox[j];
      }
   }
}


/* drawpoly --- draw a polygon, given corner co-ords */

void drawpoly (struct CoOrd *box, int nsides)
{
   int i;
   int ix, iy;

   switch (GraphDev) {
   case GR_PS:
      printf ("newpath %.2f %.2f moveto\n", box[0].x, box[0].y);

      for (i = 1; i < nsides; i++)
         printf ("%.2f %.2f lineto\n", box[i].x, box[i].y);

      printf ("closepath stroke\n");
   
      break;
   case GR_HPGL:
      ix = Devxmin + (int)(box[0].x * 40.0);
      iy = Devymin + (int)(box[0].y * 40.0);
      printf ("PU;PA%d,%d;PD;PA", ix, iy);
      
      for (i = 1; i <= nsides; i++) {
         ix = Devxmin + (int)(box[i].x * 40.0);
         iy = Devymin + (int)(box[i].y * 40.0);
         printf ("%d,%d", ix, iy);
         if (i < nsides)
            printf (",");
      }
         
      printf (";\n");
      
      break;
   case GR_BMC:
      break;
   }
}


/* shrink --- make a smaller polygon inside a larger one */

int shrink (struct CoOrd *box, struct CoOrd *nbox, double zd, int nsides, int *ineg)
{
   struct CoOrd d[5];
   double len[5];
   double tst, xyd;
   double t1, t2;
   int nneg = 0;
   int i;
   
   /* Calculate side lengths */
   for (i = 0; i < nsides; i++) {
      d[i].x = box[i].x - box[i+1].x;
      d[i].y = box[i].y - box[i+1].y;
      len[i] = sqrt ((d[i].x * d[i].x) + (d[i].y * d[i].y));
   }

   /* Allow for wrap-around */
   d[nsides] = d[0];
   len[nsides] = len[0];
   
   for (i = 0; i < nsides; i++) {
      do {
         tst = (d[i].x * d[i+1].y) - (d[i+1].x * d[i].y);

         if (tst == 0.0) {
            d[i].x += 0.001;
            d[i].y += 0.001;
            d[i+1].x += 0.001;
            d[i+1].y += 0.001;
#ifdef DB
            fprintf (stderr, "(%d,%d): tst = %f, d[%d].x=%f, d[i+1].y=%f, d[i].y=%f\n", row, col, tst, i, d[i].x, d[i+1].y, d[i].y);
#endif
         }
      } while (tst == 0.0);

      xyd = zd / tst;
      
      nbox[i].x = box[i+1].x -
                  (d[i].x * len[i+1] - d[i+1].x * len[i]) * xyd;
      nbox[i].y = box[i+1].y -
                  (d[i].y * len[i+1] - d[i+1].y * len[i]) * xyd;
   }
   
   /* Allow for wrap-around again */
   nbox[nsides] = nbox[0];

   /* Count negative sides */
   for (i = 0; i < nsides; i++) {
      t1 = (nbox[i+1].x - nbox[i].x);
      t2 = (box[i+2].x - box[i+1].x);
      
      if ((fabs (t1) < 0.00001) || (fabs (t2) < 0.00001)) {
         t1 = (nbox[i+1].y - nbox[i].y);
         t2 = (box[i+2].y - box[i+1].y);
      }
      
      tst = t1 / t2;

      if (tst < 0.0) {
#ifdef DB
         fprintf (stdout, "t1=%f, t2=%f, tst=%f\n", t1, t2, tst);
#endif
         nneg++;
         *ineg = i;
      }
   }
   
#ifdef DB
   if (nneg > 0)
      fprintf (stdout, "%d neg sides\n", nneg);
#endif

   return (nneg);
}


/* rnd --- simulate BASIC's rnd */

double rnd (void)
{
   return ((double)rand () / (double)RAND_MAX);
}


int OpenGraph (void)
{
   switch (GraphDev) {
   case GR_PS:
      Devxmin = 0;
      Devymin = 0;
      Devxmax = 639;
      Devymax = 479;

      printf ("%%!PS-Adobe2.0\n");
      printf ("%%%%Creator: op\n");
      printf ("%%%%EndComments\n");
      printf ("%%%%EndProlog\n");
      printf ("%% begin page\n");
      printf ("0 setgray\n");
      break;
   case GR_HPGL:
#ifdef A1
      Devxmin = -15970; /* A1 in DPX-3300 */
      Devymin = -10870;
      Devxmax = 15970;
      Devymax = 10870;
#else
      Devxmin = 0;
      Devymin = 0;
/*    Devxmax = 10870;  * A4 in HP7550A */
/*    Devymax = 7600; */
      Devxmax = 15970; /* A3 in HP7550A */
      Devymax = 10870;
#endif

      printf ("IN;\n");
      printf ("SP1;\n");
/*    printf ("VS10;\n"); */
      printf ("PU;PA%d,%d;PD;\n", Devxmin, Devymin);
      printf ("PA%d,%d;\n", Devxmin, Devymax);
      printf ("PA%d,%d;\n", Devxmax, Devymax);
      printf ("PA%d,%d;\n", Devxmax, Devymin);
      printf ("PA%d,%d;\n", Devxmin, Devymin);
      break;
   case GR_BMC:
      Devxmin = 0;
      Devymin = 0;
      Devxmax = 3799 / 2;
      Devymax = 2549 / 2;

      printf ("IP1;\n");   /* GDU mode */
      printf ("MA%d,%d;PS1;\n", Devxmin, Devymin);
      printf ("DA%d,%d;\n", Devxmin, Devymax);
      printf ("DA%d,%d;\n", Devxmax, Devymax);
      printf ("DA%d,%d;\n", Devxmax, Devymin);
      printf ("DA%d,%d;\n", Devxmin, Devymin);
      break;
   }
   
   return (YES);
}


void CloseGraph (void)
{
   switch (GraphDev) {
   case GR_PS:
      printf ("showpage\n");
      printf ("%%%%Trailer\n");
      break;
   case GR_HPGL:
      printf ("PU;PA%d,%d;\n", Devxmin, Devymin);
      printf ("SP0;\n");
      break;
   case GR_BMC:
      printf ("MA%d,%d;CH;\n", Devxmin, Devymin);
      break;
   }
}
