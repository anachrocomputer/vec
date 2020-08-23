/* hpgllib --- header file for HPGL library                 2013-03-23 */
/* Copyright (c) 2013 John Honniball                                   */

enum PaperSizeCode {
   ISO_A0,  //!< 841 x 1189 mm
   ISO_A1,  //!< 594 x 841 mm
   ISO_A2,  //!< 420 x 594 mm
   ISO_A3,  //!< 297 x 420 mm
   ISO_A4,  //!< 210 x 297 mm
   ISO_A5,  //!< 148 x 210 mm
   ISO_A6,  //!< 105 x 148 mm
   ANSI_E,  //!< 34 x 44 inches
   ANSI_D,  //!< 22 x 34 inches
   ANSI_C,  //!< 17 x 22 inches
   ANSI_B,  //!< 11 x 17 inches
   ANSI_A   //!< 8.5 x 11 inches
};

enum PlotterModelCode {
   HP_7470A,         //!< HP 7470A
   HP_7475A,         //!< HP 7475A
   HP_7550A,         //!< HP 7550A
   HP_7585A,         //!< HP 7585A or 7585B
   ROLAND_DPX_3300,  //!< Roland DPX-3300
   ROLAND_DXY_990,   //!< Roland DXY-990
   GENERIC_HPGL      //!< Unspecified, default to generic HPGL
};

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
   int paperSize;             //!< Paper size code
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
void __attribute__ ((deprecated("use moveto/lineto instead."))) openlinesequence(const double x, const double y);
void __attribute__ ((deprecated("use moveto/lineto instead."))) linesegmentto(const double x, const double y);
void __attribute__ ((deprecated("use moveto/lineto instead."))) closelinesequence(const int closePoly);
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
int writedisplay(const char *const str);
