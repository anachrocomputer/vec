/* hpgllib --- HPGL graphics primitives library             2013-03-23 */
/* Copyright (c) 2013 John Honniball                                   */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include "hpgllib.h"

enum ptype {PLOT_UNKNOWN, PLOT_FILE, PLOT_SERIAL, PLOT_PARALLEL};

static int openPlotter (const char *port);
static int openPlotterPort (const char *port);

static double Minx, Miny;
static double Maxx, Maxy;
static double Scale;
static FILE *Plt = NULL;
static int NoInit = 0;
static char PenNum[16] = "1";
static char OutputFile[FILENAME_MAX] = "/dev/usblp0";
static char PaperSize[16] = "A3";
static char PlotterName[32] = "A3";
static char Title[128] = "";
static char Velocity[16] = "";
static int Penx, Peny;
static double PenUpDist, PenDownDist;
static int PlotType = PLOT_UNKNOWN;

int plotopt (int ch, const char *arg) 
{
   switch (ch) {
   case 'n':
      NoInit = 1;
      break;
   case 'p':
      strcpy (PenNum, arg);
      break;
   case 's':
      strcpy (PlotterName, arg);
      break;
   case 't':
      strcpy (Title, arg);
      break;
   case 'o':
      strcpy (OutputFile, arg);
      break;
   case 'v':
      strcpy (Velocity, arg);
      break;
   }
   
   return (0);
}


int plotbegin (int border)
{
   int v;
   int fd;
   int pen;
   
   if ((PlotterName[0] == 'a') || (PlotterName[0] == 'A')) {
      switch (PlotterName[1]) {
      case '1':
         Minx = -15970.0;
         Miny = -10870.0;
         Maxx = 15970.0;
         Maxy = 10870.0;
         Scale = 80.0;
         break;
      case '2':
         fputs ("A2 plotter not yet supported\n", stderr);
         exit (EXIT_FAILURE);
         break;
      case '3':
         Minx = 0.0;
         Miny = 0.0;
         Maxx = 15970.0;
         Maxy = 10870.0;
         Scale = 40.0;
         break;
      case '4':
         Minx = 0.0;
         Miny = 0.0;
         Maxx = 10870.0;
         Maxy = 7985.0;
         Scale = 28.28;
         break;
      case '5':
         Minx = 0.0;
         Miny = 0.0;
         Maxx = 7985.0;
         Maxy = 5435.0;
         Scale = 20.0;
         break;
      }
   }
   else {
      fprintf (stderr, "%s: unrecognised plotter size\n", PlotterName);
      exit (EXIT_FAILURE);
   }
   
   pen = atoi (PenNum);
   if ((pen < 1) || (pen > 8)) {
      fprintf (stderr, "invalid pen number\n");
      exit (EXIT_FAILURE);
   }

   fd = openPlotter (OutputFile);
   
   if (fd < 0) {
      exit (EXIT_FAILURE);
   }
   
   if (PlotType == PLOT_FILE) {
      if ((Plt = fdopen (fd, "w")) == NULL) {
         perror ("fdopen w");
         exit (EXIT_FAILURE);
         return (-1);
      } 
   }
   else {
      if ((Plt = fdopen (fd, "w+")) == NULL) {
         perror ("fdopen w+");
         exit (EXIT_FAILURE);
         return (-1);
      } 
   }
   
#ifdef DB
   switch (PlotType) {
   case PLOT_FILE:
      printf ("Plotter type file\n");
      break;
   case PLOT_SERIAL:
      printf ("Plotter type serial\n");
      break;
   case PLOT_PARALLEL:
      printf ("Plotter type parallel\n");
      break;
   case PLOT_UNKNOWN:
   default:
      printf ("Plotter type unknown\n");
      break;
   }
   
   if (isatty (fd)) {
      printf ("%s is a TTY\n", OutputFile);
   }
   else {
      printf ("%s is not a TTY\n", OutputFile);
   }
#endif
   
   Penx = Minx;
   Peny = Miny;
   PenUpDist = PenDownDist = 0.0;
   
   /* Initialise plotter */
   if (NoInit == 0)
      fprintf (Plt, "IN;\n");
   
   if (Velocity[0] != '\0') {
      v = atoi (Velocity);
      fprintf (Plt, "VS%d;\n", v);  /* Slow down for shite pens */
   }

   /* Select first pen */
   pencolr (pen - 1);

   /* Draw page title */
   if (Title[0] != '\0') {
      fprintf (Plt, "PU;PA%d,%d\n", (int)(Minx + (10.0 * 40.0)), (int)(Miny + (10.0 * 40.0)));
      fprintf (Plt, "DR0,1;\n");
      fprintf (Plt, "LB%s%c;\n", Title, 3);
   }

   /* Plot border of drawing area */
   if (border) {
      rectangle (0.0, 0.0, Maxx - Minx, Maxy - Miny);
   }
   
   return (0);
}


void plotend (void)
{
   double sec;
   
   /* Park pen-holder */
   moveto (0.0, 0.0);

   /* Deselect pen */
   pencolr (-1);
   
   if (PlotType == PLOT_SERIAL) {
      printf ("Draining serial...");
      fflush (stdout);
      tcdrain (fileno (Plt));
      printf ("done\n");
      fflush (stdout);
   }

   fclose (Plt);
   
   printf ("PenUpDist = %.1f (%.1fmm)\n", PenUpDist, PenUpDist / 40.0);
   printf ("PenDownDist = %.1f (%.1fmm)\n", PenDownDist, PenDownDist / 40.0);
   
   sec = ((PenUpDist + PenDownDist) / 40.0) / 250.0;
   
   printf ("Estimated time to plot: %.1fs\n", sec);
}


void plotcancel (void)
{
   int fd;
   
   fd = fileno (Plt);
   
   printf ("Calling tcflush...");
   fflush (stdout);
   if (tcflush (fd, TCOFLUSH) < 0)
      perror ("tcflush");
      
   printf ("done\n");
   fflush (stdout);

   write (fd, "\033.K;;;", 6);
   printf ("ESC.K\n");
   fflush (stdout);

   write (fd, "PU0,0;SP0;\n", 11);
   printf ("PU/SP\n");
   fflush (stdout);
   
   printf ("Calling close...");
   fflush (stdout);
   close (fd);
   printf ("done\n");
   fflush (stdout);

   fclose (Plt);
}


void getplotsize (double *xp, double *yp)
{
   if (xp != NULL)
      *xp = Maxx - Minx;
   
   if (yp != NULL)
      *yp = Maxy - Miny;
}


int getdevx (double x)
{
   return ((int)(x + Minx));
}


int getdevy (double y)
{
   return ((int)(y + Miny));
}


int getdevr (double r)
{
   return ((int)r);
}


int hpglout (const char *buf)
{
   fputs (buf, Plt);
   
   return (0);
}


void moveto (double x, double y)
{
   int ix, iy;
   double dx, dy;
   
   ix = (int)(x + Minx);
   iy = (int)(y + Miny);

   fprintf (Plt, "PU;PA%d,%d;\n", ix, iy);
   
   dx = ix - Penx;
   dy = iy - Peny;
   
   PenUpDist += sqrt ((dx * dx) + (dy * dy));
   
   Penx = ix;
   Peny = iy;
}


void lineto (double x, double y)
{
   int ix, iy;
   double dx, dy;
   
   ix = (int)(x + Minx);
   iy = (int)(y + Miny);

   fprintf (Plt, "PD;PA%d,%d;\n", ix, iy);
   
   dx = ix - Penx;
   dy = iy - Peny;
   
   PenDownDist += sqrt ((dx * dx) + (dy * dy));
   
   Penx = ix;
   Peny = iy;
}


void rectangle (double x1, double y1, double x2, double y2)
{
   int ix1, iy1, ix2, iy2;
   
   ix1 = (int)(x1 + Minx);
   iy1 = (int)(y1 + Miny);
   ix2 = (int)(x2 + Minx);
   iy2 = (int)(y2 + Miny);

  fprintf (Plt, "PU%d,%d;EA%d,%d;\n", ix1, iy1, ix2, iy2);
}


void fillrectangle (double x1, double y1, double x2, double y2)
{
   int ix1, iy1, ix2, iy2;
   
   ix1 = (int)(x1 + Minx);
   iy1 = (int)(y1 + Miny);
   ix2 = (int)(x2 + Minx);
   iy2 = (int)(y2 + Miny);

  fprintf (Plt, "PU%d,%d;RA%d,%d;\n", ix1, iy1, ix2, iy2);
}


void circle (double x, double y, double r)
{
   int ix, iy, ir;
   
   ix = (int)(x + Minx);
   iy = (int)(y + Miny);
   ir = r;

   fprintf (Plt, "PU;PA%d,%d;CI%d;\n", ix, iy, ir);
}


void circle2 (double x, double y, double r, double tol)
{
   int ix, iy, ir, itol;
   
   ix = (int)(x + Minx);
   iy = (int)(y + Miny);
   ir = r;
   itol = tol;
   
   fprintf (Plt, "PU;PA%d,%d;CI%d,%d;\n", ix, iy, ir, itol);
}


void arc (double x, double y, double a)
{
   int ix, iy;
   
   ix = (int)(x + Minx);
   iy = (int)(y + Miny);

   fprintf (Plt, "AA%d,%d,%2.1f;\n", ix, iy, a);
}


void pencolr (int c)
{
   if (c < 0) {
      fprintf (Plt, "SP0;\n");
   }
   else {
      c %= 8;
   
      fprintf (Plt, "SP%d;\n", c + 1);
   }
}


void vlabel (double x, double y, double siz, const char *str)
{       
   int ix, iy;
   
   ix = (int)(x + Minx);
   iy = (int)(y + Miny);

   fprintf (Plt, "PU;PA%d,%d;SI%1.2f,%1.2f;DI0,1;LB%s%c;", ix, iy, siz/10.0, siz/10.0, str, 0x03);
}


void hlabel (double x, double y, double siz, const char *str)
{       
   int ix, iy;
   
   ix = (int)(x + Minx);
   iy = (int)(y + Miny);

   fprintf (Plt, "PU;PA%d,%d;SI%1.2f,%1.2f;DI1,0;LB%s%c;", ix, iy, siz/10.0, siz/10.0, str, 0x03);
}


static int openPlotter (const char *port)
{
   struct stat buf;
   int fd = -1;
   
   if ((stat (port, &buf) < 0) && (errno != ENOENT)) {
      perror (port);
      return (-1);
   }
   
   if ((S_ISREG (buf.st_mode)) || (errno == ENOENT)) {
      fd = creat (port, 0666);
      PlotType = PLOT_FILE;
   }
   else if (S_ISCHR (buf.st_mode)) {
      fd = openPlotterPort (port);
   }
   else {
      fprintf (stderr, "%s: unknown file type\n", port);
      return (-1);
   }
   
   return (fd);
}


static int openPlotterPort (const char *port)
{
   int fd = -1;
// int mbits;
   struct termios tbuf;
   long int fdflags;

   fd = open (port, O_RDWR | O_NOCTTY | O_NDELAY);
   
   if (fd < 0) {
      perror (port);
      return (-1);
   }
   
   if ((fdflags = fcntl (fd, F_GETFL, NULL)) < 0) {
      perror ("fcntl GETFL");
      return (-1);
   }
   
   fdflags &= ~O_NDELAY;
   
   if (fcntl (fd, F_SETFL, fdflags) < 0) {
      perror ("fcntl SETFL");
      return (-1);
   }

   // If it's not a TTY, we assume it's parallel and return here
   if (!isatty (fd)) {
      PlotType = PLOT_PARALLEL;
      return (fd);
   }

   // It's a TTY (serial), so we set up baud rates
   if (tcgetattr (fd, &tbuf) < 0) {
      perror ("tcgetattr");
      return (-1);
   }
   
   cfsetospeed (&tbuf, B9600);
   cfsetispeed (&tbuf, B9600);
   cfmakeraw (&tbuf);

   tbuf.c_cflag |= CLOCAL;
   tbuf.c_cflag &= ~CSIZE;
   tbuf.c_cflag |= CS8 | CREAD | CRTSCTS;
   tbuf.c_cflag &= ~PARENB;
   
   if (tcsetattr (fd, TCSAFLUSH, &tbuf) < 0) {
      perror ("tcsetattr");
      return (-1);
   }
   
#if 0
   if (ioctl (fd, TIOCMGET, &mbits) < 0) {
      perror ("ioctl get");
      return (-1);
   }
   
   if (mbits & TIOCM_DTR)
      printf ("DTR ");
   else
      printf ("dtr ");
   
   if (mbits & TIOCM_DSR)
      printf ("DSR ");
   else
      printf ("dsr ");
   
   if (mbits & TIOCM_RTS)
      printf ("RTS ");
   else
      printf ("rts ");
   
   if (mbits & TIOCM_CTS)
      printf ("CTS ");
   else
      printf ("cts ");
   
   printf ("\n");

// mbits &= ~(TIOCM_DTR | TIOCM_DSR | TIOCM_RTS | TIOCM_CTS);

// if (ioctl (fd, TIOCMSET, &mbits) < 0) {
//    perror ("ioctl set");
//    return (-1);
// }
#endif
   
   PlotType = PLOT_SERIAL;
// printf ("Returning SERIAL\n");
   return (fd);
}


void catcher (int sig)
{
   printf ("SIGINT!\n");
   
// fclose (Plt);
   plotcancel ();

   exit (EXIT_FAILURE);
}
