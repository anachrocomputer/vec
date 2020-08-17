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

enum ptype {
   PLOT_UNKNOWN,  //!< Type of interface unknown
   PLOT_FILE,     //!< Writing to a file
   PLOT_SERIAL,   //!< Writing to a serial port
   PLOT_PARALLEL, //!< Writing to a parallel port
   PLOT_IEEE488,  //!< Writing to an IEEE-488 port
   PLOT_NETWORK   //!< Writing via the network
};

static int openPlotter(const char *const port);
static int openPlotterPort(const char *const port);

static double Minx;
static double Miny;
static double Maxx;
static double Maxy;
static double Scale;
static FILE *Plt = NULL;
static int NoInit = 0;
static char PenNum[16] = "1";
static char OutputFile[FILENAME_MAX] = "/dev/usb/lp0";
static char PaperSize[16] = "A3";
static char PlotterName[32] = "A3";
static char Title[128] = "";
static char Velocity[16] = "";
static int Penx;
static int Peny;
static double PenUpDist;
static double PenDownDist;
static int PlotType = PLOT_UNKNOWN;


/**
 * @brief handle a library command-line argument
 *
 * @param ch  The option character
 * @param arg The option string argument
 */
int plotopt(const int ch, const char *const arg)
{
   switch (ch) {
   case 'n':
      NoInit = 1;
      break;
   case 'p':
      strcpy(PenNum, arg);
      break;
   case 's':
      strcpy(PlotterName, arg);
      break;
   case 't':
      strcpy(Title, arg);
      break;
   case 'o':
      strcpy(OutputFile, arg);
      break;
   case 'v':
      strcpy(Velocity, arg);
      break;
   }
   
   return (0);
}


/**
 * @brief Initialise the HPGL plotter library
 *
 * @todo Support non-ISO paper sizes
 * @todo Support multiple pen numbers in a comma-separated list
 * @todo Take pen speed into account when calculating plot time
 *
 * @param border Non-zero if a border should be drawn
 *
 * @return Zero if OK, negative if error
 */
int plotbegin(const int border)
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
         fputs("A2 plotter not yet supported\n", stderr);
         return (-1);
         /* Values used experimentally with A2 paper in DPX-3300:
            Minx = -15970.0 - 200.0;
            Miny = -10870.0 - 600.0;
            Maxx = (10870.0 * 2.0) - (15970.0 + 200.0);
            Maxy = (7985.0 * 2.0) - (10870.0 + 600.0);
            Scale = 56.57;         */
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
      fprintf(stderr, "%s: unrecognised plotter size\n", PlotterName);
      return (-1);
   }
   
   pen = atoi(PenNum);
   if ((pen < 1) || (pen > 8)) {
      fprintf(stderr, "invalid pen number\n");
      return (-1);
   }

   fd = openPlotter(OutputFile);
   
   if (fd < 0) {
      perror(OutputFile);
      return (-1);
   }
   
   if (PlotType == PLOT_FILE) {
      if ((Plt = fdopen(fd, "w")) == NULL) {
         perror("fdopen w");
         return (-1);
      } 
   }
   else {
      if ((Plt = fdopen(fd, "w+")) == NULL) {
         perror("fdopen w+");
         return (-1);
      } 
   }
   
#ifdef DB
   switch (PlotType) {
   case PLOT_FILE:
      printf("Plotter type file\n");
      break;
   case PLOT_SERIAL:
      printf("Plotter type serial\n");
      break;
   case PLOT_PARALLEL:
      printf("Plotter type parallel\n");
      break;
   case PLOT_UNKNOWN:
   default:
      printf("Plotter type unknown\n");
      break;
   }
   
   if (isatty(fd)) {
      printf("%s is a TTY\n", OutputFile);
   }
   else {
      printf("%s is not a TTY\n", OutputFile);
   }
#endif
   
   Penx = Minx;
   Peny = Miny;
   PenUpDist = PenDownDist = 0.0;
   
   /* Initialise plotter */
   if (NoInit == 0)
      fprintf(Plt, "IN;\n");
   
   if (Velocity[0] != '\0') {
      v = atoi(Velocity);
      fprintf(Plt, "VS%d;\n", v);  /* Slow down for shite pens */
   }

   /* Select first pen */
   pencolr(pen - 1);

   /* Draw page title */
   if (Title[0] != '\0') {
      fprintf(Plt, "PU;PA%d,%d\n", (int)(Minx + (10.0 * 40.0)), (int)(Miny + (10.0 * 40.0)));
      fprintf(Plt, "DR0,1;\n");
      fprintf(Plt, "LB%s%c;\n", Title, 3);
   }

   /* Plot border of drawing area */
   if (border) {
      rectangle(0.0, 0.0, Maxx - Minx, Maxy - Miny);
   }
   
   return (0);
}


/**
 * @brief Finalise the plot
 */
void plotend(void)
{
   double sec;
   
   /* Park pen-holder */
   moveto(0.0, 0.0);

   /* Deselect pen */
   pencolr(-1);
   
   if (PlotType == PLOT_SERIAL) {
      printf("Draining serial...");
      fflush(stdout);
      tcdrain(fileno(Plt));
      printf("done\n");
      fflush(stdout);
   }

   fclose(Plt);
   
   printf("PenUpDist = %.1f (%.1fmm)\n", PenUpDist, PenUpDist / 40.0);
   printf("PenDownDist = %.1f (%.1fmm)\n", PenDownDist, PenDownDist / 40.0);
   
   /* TODO: take pen speed into account when calculating plot time */
   sec = ((PenUpDist + PenDownDist) / 40.0) / 250.0;
   
   printf("Estimated time to plot: %.1fs\n", sec);
}


/**
 * @brief Cancel a plot and flush any buffered output
 */
void plotcancel(void)
{
   int fd;
   
   fd = fileno(Plt);
   
   printf("Calling tcflush...");
   fflush(stdout);
   if (tcflush(fd, TCOFLUSH) < 0)
      perror("tcflush");
      
   printf("done\n");
   fflush(stdout);

   write(fd, "\033.K;;;", 6);
   printf("ESC.K\n");
   fflush(stdout);

   write(fd, "PU0,0;SP0;\n", 11);
   printf("PU/SP\n");
   fflush(stdout);
   
   printf("Calling close...");
   fflush(stdout);
   close(fd);
   printf("done\n");
   fflush(stdout);

   fclose(Plt);
}


/**
 * @brief Return information about the plot
 *
 * @details This function fills in the fields of a structure
 *  that give information about the plot in progress. The information
 *  will not change throughout the plot (i.e. it will not include
 *  pen position or pen number).
 *
 * @param p    Pointer to struct PlotInfo, to be filled in
 * @param size Size of structure pointed to by \p p.
 *
 * @return 0 if successful, negative otherwise.
 */
int getplotinfo(struct PlotInfo *const p, unsigned int size)
{
   if ((p != NULL) && (size == sizeof (struct PlotInfo))) {
      p->versionMajor = 1;
      p->versionMinor = 1;
      
      p->plotterName = PlotterName;
      p->portName = OutputFile;
      p->paperName = PaperSize;
      p->plotTitle = Title;
      p->paperCapacityISO = "A3";
      p->paperCapacityANSI = "B";
      
      p->nPenStalls = 8;   // HP 7470 = 2, 7475 = 6, 7550 = 8
      p->nPens = 1;        // One, until we parse -p correctly
      
      p->unitsPermm = 40.0;
      p->unitsPerInch = 40.0 * 25.4;
      
      p->widthInUnits = Maxx - Minx;
      p->heightInUnits = Maxy - Miny;
      
      p->widthInmm = p->widthInUnits * p->unitsPermm;
      p->heightInmm = p->heightInUnits * p->unitsPermm;
   
      p->widthInInches = p->widthInUnits * p->unitsPerInch;
      p->heightInInches = p->heightInUnits * p->unitsPerInch;
   
      return (0);
   }
   else
      return (-1);
}


/**
 * @brief Get the maximum X and Y co-ordinates
 *
 * @param xp Pointer to return X dimension
 * @param yp Pointer to return Y dimension
 */
void getplotsize(double *const xp, double *const yp)
{
   if (xp != NULL)
      *xp = Maxx - Minx;
   
   if (yp != NULL)
      *yp = Maxy - Miny;
}


/**
 * @brief Convert an X co-ordinate to device units
 *
 * @param x X co-ordinate to be converted
 *
 * @return X co-ordinate scaled and offset to device units
 */
int getdevx(const double x)
{
   return ((int)(x + Minx));
}


/**
 * @brief Convert a Y co-ordinate to device units
 *
 * @param y Y co-ordinate to be converted
 *
 * @return Y co-ordinate scaled and offset to device units
 */
int getdevy(const double y)
{
   return ((int)(y + Miny));
}


/**
 * @brief Convert a radius to device units
 *
 * @param r Radius to be converted
 *
 * @return Radius scaled to device units
 */
int getdevr(const double r)
{
   return ((int)r);
}


/**
 * @brief Insert a snippet of HPGL into the output stream
 *
 * @todo Eliminate this function because it breaks the abstraction that the library creates
 *
 * @param buf Pointer to buffer containing HPGL command(s)
 *
 * @return Always zero
 */
int hpglout(const char *const buf)
{
   fputs(buf, Plt);
   
   return (0);
}


/**
 * @brief Move the pen to the given coordinates without drawing a line
 *
 * @param x X co-ordinate to move to
 * @param y Y co-ordinate to move to
 */
void moveto(const double x, const double y)
{
   const int ix = (int)(x + Minx);
   const int iy = (int)(y + Miny);
   double dx, dy;

   fprintf(Plt, "PU;PA%d,%d;\n", ix, iy);
   
   dx = ix - Penx;
   dy = iy - Peny;
   
   PenUpDist += sqrt((dx * dx) + (dy * dy));
   
   Penx = ix;
   Peny = iy;
}


/**
 * @brief Draw a line from the current pen position to the new position
 *
 * @todo Optimise HPGL by maintaining a state variable and eliminating
 *  unnecesary 'PD;' and 'PA' commands.
 *
 * @param x X co-ordinate to draw to
 * @param y Y co-ordinate to draw to
 */
void lineto(const double x, const double y)
{
   const int ix = (int)(x + Minx);
   const int iy = (int)(y + Miny);
   double dx, dy;

   fprintf(Plt, "PD;PA%d,%d;\n", ix, iy);
   
   dx = ix - Penx;
   dy = iy - Peny;
   
   PenDownDist += sqrt((dx * dx) + (dy * dy));
   
   Penx = ix;
   Peny = iy;
}


static int FirstSeg = 0;
static int SeqStartx = 0;
static int SeqStarty = 0;

void openlinesequence(const double x, const double y)
{
   const int ix = (int)(x + Minx);
   const int iy = (int)(y + Miny);
   double dx, dy;
   
   FirstSeg = 1;

   fprintf(Plt, "PU;PA%d,%d;", ix, iy);
   
   dx = ix - Penx;
   dy = iy - Peny;
   
   PenUpDist += sqrt((dx * dx) + (dy * dy));
   
   Penx = ix;
   Peny = iy;
   
   SeqStartx = ix;
   SeqStarty = iy;
}


void linesegmentto(const double x, const double y)
{
   const int ix = (int)(x + Minx);
   const int iy = (int)(y + Miny);
   double dx, dy;

   if (FirstSeg) {
      fprintf(Plt, "PD;PA%d,%d", ix, iy);
      FirstSeg = 0;
   }
   else
      fprintf(Plt, ",%d,%d", ix, iy);
   
   dx = ix - Penx;
   dy = iy - Peny;
   
   PenDownDist += sqrt((dx * dx) + (dy * dy));
   
   Penx = ix;
   Peny = iy;
}


void closelinesequence(const int closePoly)
{
   double dx, dy;

   if (closePoly) {
      fprintf(Plt, ",%d,%d", SeqStartx, SeqStarty);

      dx = SeqStartx - Penx;
      dy = SeqStarty - Peny;
      
      PenDownDist += sqrt((dx * dx) + (dy * dy));
      
      Penx = SeqStartx;
      Peny = SeqStarty;
   }

   fprintf(Plt, ";\n");
}


/**
 * @brief Draw a rectangle aligned with the X and Y axes
 *
 * @details Use 'PA' here instead of 'EA' (available in HPGL-2) because
 * some conversion programs (notably HPGL-to-PostScript) fail
 * to recognise 'EA'.
 *
 * @param x1 X co-ordinate of first corner
 * @param y1 Y co-ordinate of first corner
 * @param x2 X co-ordinate of second corner
 * @param y2 Y co-ordinate of second corner
 */
void rectangle(const double x1, const double y1, const double x2, const double y2)
{
   const int ix1 = (int)(x1 + Minx);
   const int iy1 = (int)(y1 + Miny);
   const int ix2 = (int)(x2 + Minx);
   const int iy2 = (int)(y2 + Miny);

   fprintf(Plt, "PU%d,%d;PD;PA%d,%d,%d,%d,%d,%d,%d,%d;\n", ix1, iy1,
                ix1, iy2, ix2, iy2, ix2, iy1, ix1, iy1);
}


/**
 * @brief Fill a rectangle aligned with the X and Y axes
 *
 * @details Note that 'RA' is an HPGL-2 command and may not be recognised
 * by all plotters and all HPGL conversion programs
 *
 * @todo Detect plotters that do not support 'RA' and return an error code, or work around it
 *
 * @param x1 X co-ordinate of first corner
 * @param y1 Y co-ordinate of first corner
 * @param x2 X co-ordinate of second corner
 * @param y2 Y co-ordinate of second corner
 */
void fillrectangle(const double x1, const double y1, const double x2, const double y2)
{
   const int ix1 = (int)(x1 + Minx);
   const int iy1 = (int)(y1 + Miny);
   const int ix2 = (int)(x2 + Minx);
   const int iy2 = (int)(y2 + Miny);

   fprintf(Plt, "PU%d,%d;RA%d,%d;\n", ix1, iy1, ix2, iy2);
}


/**
 * @brief Draw a circle
 *
 * @todo Would it be more efficient to plot all circles as 360
 *  degree arcs?
 *
 * @param x X co-ordinate of centre of circle
 * @param y Y co-ordinate of centre of circle
 * @param r Radius of circle
 */
void circle(const double x, const double y, const double r)
{
   const int ix = (int)(x + Minx);
   const int iy = (int)(y + Miny);
   const int ir = r;

   fprintf(Plt, "PU;PA%d,%d;CI%d;\n", ix, iy, ir);
}


/**
 * @brief Draw a circle with a specified arc tolerance
 *
 * @todo Design a more general method to specify arc tolerance.
 *
 * @param x   X co-ordinate of centre of circle
 * @param y   Y co-ordinate of centre of circle
 * @param r   Radius of circle
 * @param tol Arc tolerance
 */
void circle2(const double x, const double y, const double r, const double tol)
{
   const int ix = (int)(x + Minx);
   const int iy = (int)(y + Miny);
   const int ir = r;
   const int itol = tol;
   
   fprintf(Plt, "PU;PA%d,%d;CI%d,%d;\n", ix, iy, ir, itol);
}


/**
 * @brief Draw an arc
 *
 * @param x X co-ordinate of centre of arc
 * @param y Y co-ordinate of centre of arc
 * @param a Angle of arc in degrees
 */
void arc(const double x, const double y, const double a)
{
   const int ix = (int)(x + Minx);
   const int iy = (int)(y + Miny);

   fprintf(Plt, "PD;AA%d,%d,%2.1f;\n", ix, iy, a);
}


/**
 * @brief Draw an ellipse, given major and minor axes and angle
 *
 * @param x0    X co-ordinate of centre of ellipse
 * @param y0    Y co-ordinate of centre of ellipse
 * @param a     Length of major axis of ellipse
 * @param b     Length of minor axis of ellipse
 * @param theta Angle from X axis to major axis of ellipse, in radians
 */
void ellipse(const double x0, const double y0, const double a, const double b, const double theta)
{
   const int npts = 72;
   const double delta = (2.0 * M_PI) / (double)npts;
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


/**
 * @brief Draw a rounded rectangle
 *
 * @param x1     X co-ordinate of first corner
 * @param y1     Y co-ordinate of first corner
 * @param x2     X co-ordinate of second corner
 * @param y2     Y co-ordinate of second corner
 * @param radius Radius of rounded corners
 */
void roundrect(const double x1, const double y1, const double x2, const double y2, const double radius)
{
   moveto(x1 + radius, y1);
   lineto(x2 - radius, y1);
   
   arc(x2 - radius, y1 + radius, 90.0);
   
   lineto(x2, y2 - radius);
   
   arc(x2 - radius, y2 - radius, 90.0);
   
   lineto(x1 + radius, y2);
   
   arc(x1 + radius, y2 - radius, 90.0);
   
   lineto(x1, y1 + radius);
   
   arc(x1 + radius, y1 + radius, 90.0);
}


/**
 * @brief Select a pen
 *
 * @todo Improve support for multiple-pen plotting
 *
 * @param c Logical pen number
 */
void pencolr(int c)
{
   if (c < 0) {
      fprintf(Plt, "SP0;\n");
   }
   else {
      c %= 8;
   
      fprintf(Plt, "SP%d;\n", c + 1);
   }
}


/**
 * @brief Draw a text label vertically
 *
 * @param x   X co-ordinate of start of text
 * @param y   Y co-ordinate of start of text
 * @param siz Size of text
 * @param str String containing text to be plotted
 */
void vlabel(const double x, const double y, const double siz, const char *const str)
{       
   const int ix = (int)(x + Minx);
   const int iy = (int)(y + Miny);

   fprintf(Plt, "PU;PA%d,%d;SI%1.2f,%1.2f;DI0,1;LB%s%c;", ix, iy, siz/10.0, siz/10.0, str, 0x03);
}


/**
 * @brief Draw a text label horizontally
 *
 * @param x   X co-ordinate of start of text
 * @param y   Y co-ordinate of start of text
 * @param siz Size of text
 * @param str String containing text to be plotted
 */
void hlabel(const double x, const double y, const double siz, const char *const str)
{       
   const int ix = (int)(x + Minx);
   const int iy = (int)(y + Miny);

   fprintf(Plt, "PU;PA%d,%d;SI%1.2f,%1.2f;DI1,0;LB%s%c;", ix, iy, siz/10.0, siz/10.0, str, 0x03);
}


/**
 * @brief Open the port connected to the plotter
 *
 * @param port Name of port
 *
 * @return File descriptor if successful or -1 on failure
 */
static int openPlotter(const char *const port)
{
   struct stat buf;
   int fd = -1;
   
   if ((stat(port, &buf) < 0) && (errno != ENOENT)) {
      perror(port);
      return (-1);
   }
   
   if ((S_ISREG(buf.st_mode)) || (errno == ENOENT)) {
      fd = creat(port, 0666);
      PlotType = PLOT_FILE;
   }
   else if (S_ISCHR(buf.st_mode)) {
      fd = openPlotterPort(port);
   }
   else {
      fprintf(stderr, "%s: unknown file type\n", port);
      return (-1);
   }
   
   return (fd);
}


/**
 * @brief Open and configure the device file for the plotter
 *
 * @param port Name of port
 *
 * @return File descriptor if successful or -1 on failure
 */
static int openPlotterPort(const char *const port)
{
   int fd = -1;
// int mbits;
   struct termios tbuf;
   long int fdflags;

   fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
   
   if (fd < 0) {
      perror(port);
      return (-1);
   }
   
   if ((fdflags = fcntl(fd, F_GETFL, NULL)) < 0) {
      perror("fcntl GETFL");
      return (-1);
   }
   
   fdflags &= ~O_NDELAY;
   
   if (fcntl(fd, F_SETFL, fdflags) < 0) {
      perror("fcntl SETFL");
      return (-1);
   }

   // If it's not a TTY, we assume it's parallel and return here
   if (!isatty(fd)) {
      PlotType = PLOT_PARALLEL;
      return (fd);
   }

   // It's a TTY (serial), so we set up baud rates
   if (tcgetattr(fd, &tbuf) < 0) {
      perror("tcgetattr");
      return (-1);
   }
   
   cfsetospeed(&tbuf, B9600);
   cfsetispeed(&tbuf, B9600);
   cfmakeraw(&tbuf);

   tbuf.c_cflag |= CLOCAL;
   tbuf.c_cflag &= ~CSIZE;
   tbuf.c_cflag |= CS8 | CREAD | CRTSCTS;
   tbuf.c_cflag &= ~PARENB;
   
   if (tcsetattr (fd, TCSAFLUSH, &tbuf) < 0) {
      perror("tcsetattr");
      return (-1);
   }
   
#if 0
   if (ioctl(fd, TIOCMGET, &mbits) < 0) {
      perror("ioctl get");
      return (-1);
   }
   
   if (mbits & TIOCM_DTR)
      printf("DTR ");
   else
      printf("dtr ");
   
   if (mbits & TIOCM_DSR)
      printf("DSR ");
   else
      printf("dsr ");
   
   if (mbits & TIOCM_RTS)
      printf("RTS ");
   else
      printf("rts ");
   
   if (mbits & TIOCM_CTS)
      printf("CTS ");
   else
      printf("cts ");
   
   printf("\n");

// mbits &= ~(TIOCM_DTR | TIOCM_DSR | TIOCM_RTS | TIOCM_CTS);

// if (ioctl(fd, TIOCMSET, &mbits) < 0) {
//    perror("ioctl set");
//    return (-1);
// }
#endif
   
   PlotType = PLOT_SERIAL;
// printf("Returning SERIAL\n");
   return (fd);
}


/**
 * @brief Signal handler to stop plotting and close port
 *
 * @param sig Signal number
 */
void catcher(const int sig)
{
   printf("SIGINT!\n");
   
// fclose(Plt);
   plotcancel();

   exit(EXIT_FAILURE);
}
