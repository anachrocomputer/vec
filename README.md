# vec

Vector graphics demo software to run on an HPGL pen plotter.

The programs are in C and may be compiled with 'gcc' on Linux
(MacOS X may also work).
They generate HPGL and send it to a plotter via the parallel port
(/dev/usb/lp0),
the serial port (/dev/ttyUSB0),
or to a file.
Most of the programs accept command-line arguments to scale the vector
drawings up or down (default size is suitable for ISO A3 paper).

## HPGL

HPGL stands for Hewlett-Packard Graphics Language.
It is a simple text-based language to encode vector graphics and
drive pen plotters.
Most pen plotters will accept it, not just those made by HP.

The basic resolution of HPGL is 40 plotter units per millimetre.
HPGL drawing primitives are straight lines, arcs, and text.

## Useful Tools

To preview the HPGL files on a raster-scan display,
use a package called 'hp2xx'.
Install it with a command-line such as:

sudo apt-get install hp2xx

The Makefile will use 'hp2xx' to generate low-resolution preview images
in PNG format.
It also converts the HPGL to SVG for previewing in a browser or for
plotting on newer types of plotter such as the AxiDraw.

## The Plotters

Most of the programs here have been tested on a Roland DXY-990 A3
flat-bed pen plotter.
Some have also been used at A1 size on the much bigger Roland DPX-3300.
A few have been tested on the fast HP 7550A A3 grit-wheel plotter.

## The Pens

I use Pilot V-Ball pens on the Roland plotters, in modified pen-holders.
For thicker lines, I use Staedtler Triplus pens, again with pen-holders
that fit the Roland plotters.
Other types of pen will no doubt work, and few of the plots here rely
on special line width or other pen properties.

Most of the plots require only a single pen.

None of the drawings are intended for use with vinyl cutters, but there's
plenty of scope to try that sort of thing.

