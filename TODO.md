# Things To Do

## HPGL library
* Add support for non-ISO paper sizes (i.e. U.S. paper sizes)
* Add support for plot scaling. We should be able to plot directly
in millimetres (and possibly in inches) as well as in device
coordinates and maybe in other units.
* Separate paper size and plotter type so that we can draw on (say)
A4 paper in an A0 plotter. The big plotters have the origin in the
centre of the bed, whereas the smaller ones have the origin in the
lower left corner.
* Fully implement and test serial plotter support.
* Fully implement and test signal-handling and clean-up after a
SIGINT has stopped a plot.
* Move 'drawline()' into the library. It's a generalised line-drawing
function that will reverse a line end-for-end to speed up drawing. That function
is useful in many situations and would eliminate explicit logic to plot
lines in reverse directions.
* Move 'spiral()' into the library. Another useful general-purpose
drawing function. Make sure it can start with a non-zero radius, and draw
both clockwise and anti-clockwise.
* Introduce a proper strategy for handling arc tolerance. Several
functions have a hard-coded '72' in them to represent the default HPGL
arc tolerance of 72 chords. Make this a settable parameter somehow.
* Add multi-pen support. Probably make the '-p' argument accept a
comma-separated list of pens. Plot programs could then call for the
next (or previous) pen and make simple multi-colour plots.
Plots that might benefit from that: dala2, morphpoly, sutpent.
* Eliminate 'hpglout()' if at all possible. It breaks the abstraction
that the library tries to maintain.
* Support non-HPGL output. Maybe PostScript, BMC plotter commands,
G-code or SVG. This would be a big change to the library and would
introduce a lot of new code. It would also be a big testing issue.
* Add circles and arcs to the PenDownDistance and plot time calculations.
* Make plot time take into account pen speed if '-v' is used.
* Add Doxygen comments to the library source.

## Turtle library
* Implement run-time option setting in all Turtle plots, and in the
Turtle library. This would allow a consistent user interface across
direct HPGL programs and Turtle programs.
* Make Turtle library drive the plotter directly, as the HPGL library
does. This would eliminate the need to redirect the output to the
plotter (using shell I/O redirection) when plotting Turtle programs.
* Add Doxygen comments to the library source.

## Plotting programs
* Add new drawings! Especially colouring-in drawings, all-over patterns,
3D perspective drawings, maps and even slow-to-draw drawings!
* Add colour (pen changes) to drawings.
* Convert some of the older drawings to fully use the HPGL library.
E.g. op, pconic, hconic, lobe, hyp.
* Finish off some of the incomplete plots. E.g. hexagon, fraserspiral,
flake, arches, lissajous, piscis, qrplot, spiralsq, superellipse,
twist, zigzag.
* Clean up the various 'dala' drawings. Eliminate the 'dala2c' variant
by adding colour to 'dala2.c'.
* Add run-time variation to 'dala2.c'.
* Consider using Doxygen to document the programs.
