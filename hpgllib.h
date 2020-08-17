/* hpgllib --- header file for HPGL library                 2013-03-23 */
/* Copyright (c) 2013 John Honniball                                   */

struct PlotInfo {
   int versionMajor;          //!< Major version number of HPGL library
   int versionMinor;          //!< Minor version number of HPGL library
   char *plotterName;         //!< Model name of plotter
   char *paperName;           //!< Name of paper size (ISO or ANSI)
   char *portName;            //!< Name of port connectd to plotter
   char *plotTitle;           //!< Title of plot, if set via command-line
   char *paperCapacityISO;    //!< Biggest ISO paper size that will fit on the plotter
   char *paperCapacityANSI;   //!< Biggest ANSI paper size that will fit on the plotter
   int plotterInterface;      //!< Type of interface
   int plotterLanguage;       //!< Plotter command language
   int orientation;           //!< Orientation of plot, landscape or portrait
   int nPenStalls;            //!< Number of pen stalls in the plotter
   int nPens;                 //!< Number of pens to be used in this plot
   double unitsPermm;         //!< Number of plotter units per millimetre
   double unitsPerInch;       //!< Number of plotter units per inch
   double widthInmm;          //!< Width of plottable area in millimetres
   double heightInmm;         //!< Height of plottable area in millimetres
   double widthInInches;      //!< Width of plottable area in inches
   double heightInInches;     //!< Height of plottable area in inches
   double widthInUnits;       //!< Width of plottable area in plotter units
   double heightInUnits;      //!< Height of plottable area in plotter units
};

typedef struct PlotInfo PlotInfo_t;

int plotopt(const int ch, const char *const arg);
int plotbegin(const int border);
void plotend(void);
void plotcancel(void);
int getplotinfo(struct PlotInfo *const p, unsigned int size);
void getplotsize(double *const xp, double *const yp);
int getdevx(const double x);
int getdevy(const double y);
int getdevr(const double r);
int hpglout(const char *const buf);
void moveto(const double x, const double y);
void lineto(const double x, const double y);
void openlinesequence(const double x, const double y);
void linesegmentto(const double x, const double y);
void closelinesequence(const int closePoly);
void rectangle(const double x1, const double y1, const double x2, const double y2);
void fillrectangle(const double x1, const double y1, const double x2, const double y2);
void circle(const double x, const double y, const double r);
void circle2(const double x, const double y, const double r, const double tol);
void arc(const double x, const double y, const double a);
void ellipse(const double x0, const double y0, const double a, const double b, const double theta);
void roundrect(const double x1, const double y1, const double x2, const double y2, const double radius);
void pencolr(int c);
void vlabel(const double x, const double y, const double siz, const char *const str);
void hlabel(const double x, const double y, const double siz, const char *const str);
