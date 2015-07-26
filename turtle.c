/* turtle --- turtle graphics library for plotters          1996-12-15 */
/* Copyright (c) 1996 John Honniball, Froods Software Development      */

/* Modification:
 * 1996-12-15 JRH Initial coding
 * 2000-04-11 JRH Added HPGL support
 * 2009-11-12 JRH Added border and slow drawing flags
 * 2012-09-09 JRH Fixed text scaling and location in 'title()'
 * 2013-03-22 JRH Added flag FLG_RELS for relative scaling
 */
 
#include <stdio.h>
#include <string.h>
#include "turtle.h"
#include <math.h>

#define RADIANS   (0.017453292)
#define ETX (0x03)

static int Pltdev = DEV_NONE;
static double Minx, Miny;
static double Maxx, Maxy;
static double Homex, Homey;
static double Heading;
static double Scale;
static double Xvec, Yvec;
static double Xpos, Ypos;
static int Penstate, Pencol;
static int Main_flags;

int turtle (int dev, int siz, int ori, int flags)
{
   Pltdev = dev;
   Main_flags = flags;
   
   switch (dev) {
   case DEV_HPGL:
      switch (siz) {
      case SIZ_A1:
         Minx = -15970.0;
         Miny = -10870.0;
         Maxx = 15970.0;
         Maxy = 10870.0;
         Scale = 80.0;
         break;
      case SIZ_A3:
         /* A3 in HP7550A and DXY-990: */
         Minx = 0.0;
         Miny = 0.0;
         Maxx = 15970.0;
         Maxy = 10870.0;
         Scale = 40.0;
         break;
      case SIZ_A4:
         /* A4 in HP7550A: 10870x7600 */
         Minx = 0.0;
         Miny = 0.0;
         Maxx = 10870.0;
         Maxy = 7985.0;
         Scale = 28.28;
         break;
      case SIZ_A5:
         Minx = 0.0;
         Miny = 0.0;
         Maxx = 7985.0;
         Maxy = 5435.0;
         Scale = 20.0;
         break;
       default:
         fprintf (stderr, "paper size not supported\n");
         break;
      }
      
      if ((flags & FLG_RELS) == 0)
         Scale = 40.0;  /* Absolute scaling: 40 plotter units per mm */

      printf ("IN;SP%d;\n", BLACK);
      break;
   case DEV_BMC:
      /* A3 in GDU mode: */
      Maxx = 3800.0;
      Maxy = 2550.0;
      
      /* Convert to A4 */
      Maxx /= 1.4142;
      Maxy /= 1.4142;

      Scale = 10.0;  /* 10 plotter units per mm */

      printf ("IP1;PS%d;\n", BLACK);
      break;
   case DEV_PS:
      Scale = 1.0;   /* Plotter units are mm */
      break;
   case DEV_VGA:
      break;
   default:
      return (1);
   }
   
   Homex = (Minx + Maxx) / 2.0;
   Homey = (Miny + Maxy) / 2.0;
   
   Penstate = DOWN;
   Pencol = BLACK;
   Heading = 0.0;
   
   Xvec = cos (Heading * RADIANS);
   Yvec = sin (Heading * RADIANS);
   
   if (flags & FLG_SLOW) {
      switch (Pltdev) {
      case DEV_HPGL:
         printf ("VS10;\n");  /* Slow down for shite pens */
         break;
      case DEV_BMC:
         break;
      case DEV_PS:
         break;
      case DEV_VGA:
         break;
      default:
         fprintf (stderr, "Turtle graphics not initialised\n");
         break;
      }
   }

   if (flags & FLG_BORD) {
      switch (Pltdev) {
      case DEV_HPGL:
         printf ("PU;PA%d,%d;\n", (int)Minx, (int)Miny);
         printf ("PD;PA%d,%d;\n", (int)Maxx, (int)Miny);
         printf ("PD;PA%d,%d;\n", (int)Maxx, (int)Maxy);
         printf ("PD;PA%d,%d;\n", (int)Minx, (int)Maxy);
         printf ("PD;PA%d,%d;\n", (int)Minx, (int)Miny);
         break;
      case DEV_BMC:
         break;
      case DEV_PS:
         break;
      case DEV_VGA:
         break;
      default:
         fprintf (stderr, "Turtle graphics not initialised\n");
         break;
      }
   }

   switch (Pltdev) {
   case DEV_HPGL:
      printf ("PU;PA%d,%d;\n", (int)Homex, (int)Homey);
      break;
   case DEV_BMC:
      printf ("MA%d,%d\n", (int)Homex, (int)Homey);
      break;
   case DEV_PS:
      break;
   case DEV_VGA:
      break;
   default:
      fprintf (stderr, "Turtle graphics not initialised\n");
      break;
   }
   
   Xpos = Homex;
   Ypos = Homey;
   
   return (0);
}


/* show --- finish off the plot */

void show (void)
{
   switch (Pltdev) {
   case DEV_HPGL:
      printf ("PU;PA%d,%d;SP0;\n", (int)Minx, (int)Miny);
      break;
   case DEV_BMC:
      printf ("MA0,0;\n");
      break;
   case DEV_PS:
      break;
   case DEV_VGA:
      break;
   default:
      fprintf (stderr, "Turtle graphics not initialised\n");
      break;
   }
}


/* title --- draw a title on the plot */

void title (const char str[], double size, int posn, int flags)
{
   int x, y;
   double xc = 0.0;
   double yc = 0.0;
   int len = strlen (str);
   
   switch (Pltdev) {
   case DEV_HPGL:
      switch (posn & 0x03) {
      case 0:
      case TOP:
         y = (int)Maxy;
         yc = -1.0;
         break;
      case MID:
         y = (int)((Miny + Maxy) / 2.0);
         yc = -0.25;
         break;
      case BOT:
         y = (int)Miny;
         yc = 0.25;  // Allow room for descenders
         break;
      }

      switch (posn & 0x0c) {
      case 0:
      case LEFT:
         x = (int)Minx;
         xc = 0.0;
         
         if (Main_flags & FLG_BORD)
           xc += 0.5;
           
         break;
      case CENTRE:
         x = (int)((Minx + Maxx) / 2.0);
         xc = -((double)len) / 2.0;
         break;
      case RIGHT:
         x = (int)Maxx;
         xc = -((double)len);

         if (Main_flags & FLG_BORD)
           xc -= 0.5;
           
         break;
      }

      if (flags & ITALIC)
         printf ("SL.36;\n");
      else
         printf ("SL0;\n");
      
      printf ("SI%1.2f,%1.2f;\n", size / 13.1579, size / 10.0);
      printf ("PU;PA%d,%d;CP%1.2f,%1.2f;LB%s%c;\n", x, y, xc, yc, str, ETX);
      printf ("PU;PA%d,%d;\n", (int)Xpos, (int)Ypos);
      break;
   case DEV_BMC:
      break;
   case DEV_PS:
      break;
   case DEV_VGA:
      break;
   default:
      fprintf (stderr, "Turtle graphics not initialised\n");
      break;
   }
}


/* bottom_left --- move the turtle to the bottom left corner */

void bottom_left (void)
{
   Xpos = Minx;
   Ypos = Miny;
   
   switch (Pltdev) {
   case DEV_HPGL:
      printf ("PU;PA%d,%d;\n", (int)Xpos, (int)Ypos);
      break;
   case DEV_BMC:
      printf ("MA%d,%d\n", (int)Xpos, (int)Ypos);
      break;
   case DEV_PS:
      break;
   case DEV_VGA:
      break;
   default:
      fprintf (stderr, "Turtle graphics not initialised\n");
      break;
   }
}


/* set_heading --- set the turtle heading to an absolute angle */

void set_heading (double deg)
{
   Heading = deg;
   
   Xvec = cos (Heading * RADIANS);
   Yvec = sin (Heading * RADIANS);
}


/* forward --- move the turtle forward by a given number of millimetres */

void forward (double len)
{
   double newx, newy;
   
   len *= Scale;
   
   newx = Xpos + (Xvec * len);
   newy = Ypos + (Yvec * len);
   
   switch (Pltdev) {
   case DEV_HPGL:
      if (Penstate == DOWN)
         printf ("PD;PA%d,%d;\n", (int)newx, (int)newy);
      else
         printf ("PU;PA%d,%d;\n", (int)newx, (int)newy);

      break;
   case DEV_BMC:
      if (Penstate == DOWN)
         printf ("DA%d,%d\n", (int)newx, (int)newy);
      else
         printf ("MA%d,%d\n", (int)newx, (int)newy);

      break;
   case DEV_PS:
      break;
   case DEV_VGA:
      break;
   default:
      fprintf (stderr, "Turtle graphics not initialised\n");
      break;
   }
   
   Xpos = newx;
   Ypos = newy;
}


/* turn --- turn the turtle by a given number of degrees */

void turn (double deg)
{
   Heading += deg;
   
   if (Heading > 360.0)
      Heading -= 360.0;
      
   if (Heading < 0.0)
      Heading += 360.0;
   
   Xvec = cos (Heading * RADIANS);
   Yvec = sin (Heading * RADIANS);
#ifdef DB
   printf ("turn (%f): %f (%f,%f)\n", deg, Heading, Xvec, Yvec);
#endif
}


/* pen --- raise or lower the pen; only draws when pen down */

void pen (int flag)
{
   Penstate = flag;
}


/* colour --- set the pen colour */

void colour (int c)
{
   if (Pencol == c)
      return;
      
   switch (Pltdev) {
   case DEV_HPGL:
      printf ("SP%d;\n", c);
      break;
   case DEV_BMC:
      printf ("PS%d;\n", c);
      break;
   case DEV_PS:
      break;
   case DEV_VGA:
      break;
   default:
      fprintf (stderr, "Turtle graphics not initialised\n");
      break;
   }

   Pencol = c;
}

/* page_width --- return width of page in millimetres */

double page_width (void)
{
   if (Pltdev != DEV_NONE)
      return ((Maxx - Minx) / Scale);
   else {
      fprintf (stderr, "Turtle graphics not initialised\n");
      return (0.0);
   }
}


/* page_height --- return height of page in millimetres */

double page_height (void)
{
   if (Pltdev != DEV_NONE)
      return ((Maxy - Miny) / Scale);
   else {
      fprintf (stderr, "Turtle graphics not initialised\n");
      return (0.0);
   }
}
