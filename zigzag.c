/* zigzag --- plot an image in greyscale using zigzags      2013-03-25 */
/* Copyright (c) 2013 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "hpgllib.h"


int main (int argc, char * const argv[])
{
/* Inspired by polargraph plotter at Bristol Mini Maker Faire 2013 */
   int opt;
   double x, y;
   double off;
   int i, j;
   int pixel;
   char lin[128];
   FILE *fp;
   double maxx, maxy;
   
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
         fprintf (stderr, "Usage: %s [-p pen] [-s <size>] [-t title]\n",
                  argv[0]);
         fprintf (stderr, "       <size> ::= A1 | A2 | A3 | A4 | A5\n");
         exit (EXIT_FAILURE);
      }
   }

   if ((fp = fopen ("zigzag.pgm", "r")) == NULL) {
      perror ("zigzag.pgm");
      exit (EXIT_FAILURE);
   }
   
   fgets (lin, 128, fp);
   fgets (lin, 128, fp);
   fgets (lin, 128, fp);

   plotbegin (1);

   getplotsize (&maxx, &maxy);
   
   for (i = 0; i < 40; i++) {
      x = 10.0 * (double)i;
      
      moveto (x * 40.0, 0.0);
      
      for (j = 0; j < 108; j++) {
         fscanf (fp, "%d", &pixel);
         
         y = 2.5 * (double)j;
         
         off = ((255 - pixel) * 5) / 255.0;

         if (j & 1)
            off = -off;

         lineto ((x + off) * 40.0, y * 40.0);
      }
   }
   
   plotend ();
   
   fclose (fp);
   
   return (0);
}
