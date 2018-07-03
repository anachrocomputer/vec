/* hpgllib --- header file for HPGL library                 2013-03-23 */
/* Copyright (c) 2013 John Honniball                                   */

int plotopt (int ch, const char *arg);
int plotbegin (int border);
void plotend (void);
void plotcancel (void);
void getplotsize (double *xp, double *yp);
int getdevx (double x);
int getdevy (double y);
int getdevr (double r);
int hpglout (const char *buf);
void moveto (double x, double y);
void lineto (double x, double y);
void openlinesequence (double x, double y);
void linesegmentto (double x, double y);
void closelinesequence (int closePoly);
void rectangle (const double x1, const double y1, const double x2, const double y2);
void fillrectangle (const double x1, const double y1, const double x2, const double y2);
void circle (double x, double y, double r);
void circle2 (double x, double y, double r, double tol);
void arc (const double x, const double y, const double a);
void ellipse(const double x0, const double y0, const double a, const double b, const double theta);
void roundrect (const double x1, const double y1, const double x2, const double y2, const double radius);
void pencolr (int c);
void vlabel (double x, double y, double siz, const char *str);
void hlabel (double x, double y, double siz, const char *str);
