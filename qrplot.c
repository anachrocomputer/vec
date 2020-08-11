/* qrplot --- draw a QR code via a PBM file                 2013-07-18 */
/* Copyright (c) 2013 John Honniball, Froods Software Development      */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include "hpgllib.h"


int plotPBM(char *fname, double x0, double y0, double pix);


int main(int argc, char * const argv[])
{
   int opt;
   
   while ((opt = getopt(argc, argv, "no:p:s:t:v:")) != -1) {
      switch (opt) {
      case 'n':
      case 'o':
      case 'p':
      case 's':
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

   plotPBM("qr_sample.pbm", 50.0, 0.0, 2.0);
   
   plotend();
   
   return (0);
}


int plotPBM(char *fname, double x0, double y0, double pix)
{
   FILE *fp;
   char lin[256];
   int ch;
   int prev_ch;
   int xsize, ysize;
   int x, y;
   double x1, y1;
   double x2;
   double psiz;
   double ytop;
   
   x0 *= 40.0;
   y0 *= 40.0;
   pix *= 40.0;
   psiz = pix - (0.1 * 40.0);

   if ((fp = fopen(fname, "r")) == NULL) {
      perror(fname);
      return (0);
   }

   /* Read PBM header */
   fgets(lin, sizeof (lin), fp);

   if (lin[0] != 'P') {
      fprintf(stderr, "Image file '%s' is not a PBM file\n", fname);
      fclose(fp);
      return (0);
   }
   
   if (lin[1] != '1') {
      fprintf(stderr, "Image file '%s' not ASCII PBM file\n", fname);
      fclose(fp);
      return (0);
   }

   /* Skip PBM comment */
   fgets(lin, sizeof (lin), fp);
   
   /* Read PBM X and Y size */
   fscanf(fp, "%d %d", &xsize, &ysize);

   ytop = y0 + (ysize * pix);
   
   for (y = 0; y < ysize; y++) {
      prev_ch = '0';
      y1 = ytop - (y * pix);

      for (x = 0; x < xsize; x++) {
         do {        /* Read image pixels from file */
            ch = fgetc(fp);
         } while (!(ch == '1' || ch == '0'));
         
         if (ch == '1' && prev_ch == '0') {
            x1 = x * pix;
         }
         else if (ch == '0' && prev_ch == '1') {
            x2 = (x - 1) * pix;
            fillrectangle(x0 + x1, y0 + y1, x0 + x2 + psiz, y0 + y1 + psiz);
         }
         
         prev_ch = ch;
      }
   }
   
   rectangle(x0, y0, x0 + (xsize * pix), y0 + ((ysize + 1) * pix));
   
   fclose(fp);
   
   return (1);
}
