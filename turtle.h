/* turtle --- header file for turtle graphics library       1996-12-15 */
/* Copyright (c) 1996 John Honniball, Froods Software Development      */

#define DEV_NONE  (0)
#define DEV_HPGL  (1)
#define DEV_BMC   (2)
#define DEV_PS    (3)
#define DEV_VGA   (4)

#define SIZ_A1    (1)
#define SIZ_A2    (2)
#define SIZ_A3    (3)
#define SIZ_A4    (4)
#define SIZ_A5    (5)

#define ORI_LAND  (0)
#define ORI_PORT  (1)

#define FLG_NONE  (0)
#define FLG_BORD  (1)
#define FLG_SLOW  (2)
#define FLG_RELS  (4)

#define ITALIC    (1)

#define UP        (0)
#define DOWN      (1)

#define TOP       (1)
#define MID       (2)
#define BOT       (3)
#define LEFT      (4)
#define CENTRE    (8)
#define RIGHT     (12)

#define BLACK     (1)
#define RED       (2)
#define GREEN     (3)
#define BLUE      (4)

int turtle (int dev, int siz, int ori, int flags);
void show (void);
void title (const char str[], double size, int posn, int flags);
void bottom_left (void);
void set_heading (double deg);
void forward (double len);
void turn (double deg);
void pen (int flag);
void colour (int c);
double page_width (void);
double page_height (void);
