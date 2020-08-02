/* turtle --- header file for turtle graphics library       1996-12-15 */
/* Copyright (c) 1996 John Honniball, Froods Software Development      */

#define DEV_NONE  (0)   //!< No device selected
#define DEV_HPGL  (1)   //!< Hewlett-Packard Graphics Language
#define DEV_BMC   (2)   //!< BMC plotter commands
#define DEV_PS    (3)   //!< Postscript language
#define DEV_VGA   (4)   //!< VGA graphics @todo May be obsolete, remove

#define SIZ_A1    (1)   //!< ISO A1 paper size
#define SIZ_A2    (2)   //!< ISO A2 paper size
#define SIZ_A3    (3)   //!< ISO A3 paper size
#define SIZ_A4    (4)   //!< ISO A4 paper size
#define SIZ_A5    (5)   //!< ISO A5 paper size

#define ORI_LAND  (0)   //!< Drawing orientation landscape @todo Implement this feature
#define ORI_PORT  (1)   //!< Drawing orientation portrait @todo Implement this feature

#define FLG_NONE  (0)   //!< No flags
#define FLG_BORD  (1)   //!< Flag to draw a border around the plot
#define FLG_SLOW  (2)   //!< Flag to slow down the drawing to allow for poor pens
#define FLG_RELS  (4)   //!< Relative size flag

#define ITALIC    (1)   //!< Use an italic font (or just a slant)

#define UP        (0)   //!< Pen up, turtle moves will not draw anything
#define DOWN      (1)   //!< Pen down, turtle moves will draw a line

#define TOP       (1)   //!< Position of title text: top of page
#define MID       (2)   //!< Position of title text: middle of page
#define BOT       (3)   //!< Position of title text: bottom of page
#define LEFT      (4)   //!< Position of title text: left of page
#define CENTRE    (8)   //!< Position of title text: centre of page
#define RIGHT     (12)  //!< Position of title text: right of page

#define BLACK     (1)   //!< Pen colour black (only truly correct for BMC plotter)
#define RED       (2)   //!< Pen colour red (only truly correct for BMC plotter)
#define GREEN     (3)   //!< Pen colour green (only truly correct for BMC plotter)
#define BLUE      (4)   //!< Pen colour blue (only truly correct for BMC plotter)

int turtle(const int dev, const int siz, const int ori, const int flags);
void show(void);
void title(const char str[], const double size, const int posn, const int flags);
void bottom_left(void);
void set_heading(const double deg);
void forward(const double mm);
void turn(const double deg);
void pen(const int flag);
void colour(const int c);
double page_width(void);
double page_height(void);
