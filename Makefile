# Makefile for the vector graphics programs                    2011-09-16
# Copyright (c) 2011 John Honniball

CC=gcc
CFLAGS=-c -Wall
LD=gcc

TITLE=-t "Bristol Hackspace"
BOLDPEN=-p 1
FINEPEN=-p 1

HPGL=flake.hpgl op.hpgl dala1.hpgl hconic2.hpgl tree.hpgl dome.hpgl \
     coords.hpgl hilb.hpgl hexagon.hpgl pappus.hpgl spiro.hpgl hyp.hpgl \
     lobe.hpgl ellipse.hpgl arches.hpgl dala2.hpgl piscis.hpgl rtree.hpgl \
     superellipse.hpgl lissajous.hpgl dala3.hpgl pin_and_cotton.hpgl \
     sqinsq.hpgl twist.hpgl pconic.hpgl zigzag.hpgl \
     spiralsq.hpgl dala2c.hpgl fraserspiral.hpgl \
     curve_stitching.hpgl cs_rosette.hpgl allover13.hpgl allover12.hpgl \
     qrplot.hpgl lotus.hpgl celticstep.hpgl op_moire.hpgl

all: $(HPGL) plottext

# Tarim's C++ code

spiro.hpgl: spiro
	./spiro >spiro.hpgl

spiro: spiro.cpp spiro.h
	g++ -o $@ spiro.cpp

# Direct HPGL programs

op.hpgl: op
	./op >op.hpgl

op: op.o
	$(LD) -o $@ op.o -lm

op.o: op.c
	$(CC) $(CFLAGS) -o $@ op.c

lobe.hpgl: lobe
	./lobe >lobe.hpgl

lobe: lobe.o
	$(LD) -o $@ lobe.o -lm

lobe.o: lobe.c
	$(CC) $(CFLAGS) -o $@ lobe.c

# HPGL library programs

piscis.hpgl: piscis Makefile
	./piscis $(TITLE) $(BOLDPEN) -o $@

piscis: piscis.o hpgllib.o
	$(LD) -o $@ piscis.o hpgllib.o -lm

piscis.o: piscis.c hpgllib.h
	$(CC) $(CFLAGS) -o $@ piscis.c

ellipse.hpgl: ellipse Makefile
	./ellipse $(TITLE) $(BOLDPEN) -o $@

ellipse: ellipse.o hpgllib.o
	$(LD) -o $@ ellipse.o hpgllib.o -lm

ellipse.o: ellipse.c hpgllib.h
	$(CC) $(CFLAGS) -o $@ ellipse.c

superellipse.hpgl: superellipse Makefile
	./superellipse $(TITLE) $(BOLDPEN) -o $@

superellipse: superellipse.o hpgllib.o
	$(LD) -o $@ superellipse.o hpgllib.o -lm

superellipse.o: superellipse.c hpgllib.h
	$(CC) $(CFLAGS) -o $@ superellipse.c

lissajous.hpgl: lissajous Makefile
	./lissajous $(TITLE) $(BOLDPEN) -o $@

lissajous: lissajous.o hpgllib.o
	$(LD) -o $@ lissajous.o hpgllib.o -lm

lissajous.o: lissajous.c hpgllib.h
	$(CC) $(CFLAGS) -o $@ lissajous.c

arches.hpgl: arches Makefile
	./arches $(TITLE) $(BOLDPEN) -o $@

arches: arches.o hpgllib.o
	$(LD) -o $@ arches.o hpgllib.o -lm

arches.o: arches.c hpgllib.h
	$(CC) $(CFLAGS) -o $@ arches.c

hyp.hpgl: hyp
	./hyp >hyp.hpgl

hyp: hyp.o
	$(LD) -o $@ hyp.o -lm

hyp.o: hyp.c
	$(CC) $(CFLAGS) -o $@ hyp.c

pappus.hpgl: pappus Makefile
	./pappus $(TITLE) $(BOLDPEN) -o $@

pappus: pappus.o hpgllib.o
	$(LD) -o $@ pappus.o hpgllib.o -lm

pappus.o: pappus.c
	$(CC) $(CFLAGS) -o $@ pappus.c

dala1.hpgl: dala1 Makefile
	./dala1 $(TITLE) $(BOLDPEN) -o $@

dala1: dala1.o hpgllib.o
	$(LD) -o $@ dala1.o hpgllib.o -lm

dala1.o: dala1.c hpgllib.h
	$(CC) $(CFLAGS) -o $@ dala1.c

dala2.hpgl: dala2
	./dala2 $(TITLE) $(BOLDPEN) -o $@

dala2: dala2.o hpgllib.o
	$(LD) -o $@ dala2.o hpgllib.o -lm

dala2.o: dala2.c hpgllib.h
	$(CC) $(CFLAGS) -o $@ dala2.c

dala2c.hpgl: dala2c
	./dala2c $(TITLE) $(BOLDPEN) -o $@

dala2c: dala2c.o hpgllib.o
	$(LD) -o $@ dala2c.o hpgllib.o -lm

dala2c.o: dala2c.c hpgllib.h
	$(CC) $(CFLAGS) -o $@ dala2c.c

dala3.hpgl: dala3 Makefile
	./dala3 $(TITLE) $(BOLDPEN) -o $@

dala3: dala3.o hpgllib.o
	$(LD) -o $@ dala3.o hpgllib.o -lm

dala3.o: dala3.c hpgllib.h
	$(CC) $(CFLAGS) -o $@ dala3.c

twist.hpgl: twist Makefile
	./twist $(TITLE) $(BOLDPEN) -o $@

twist: twist.o hpgllib.o
	$(LD) -o $@ twist.o hpgllib.o -lm

twist.o: twist.c hpgllib.h
	$(CC) $(CFLAGS) -o $@ twist.c

lotus.hpgl: lotus Makefile
	./lotus $(TITLE) $(BOLDPEN) -o $@

lotus: lotus.o hpgllib.o
	$(LD) -o $@ lotus.o hpgllib.o -lm

lotus.o: lotus.c hpgllib.h
	$(CC) $(CFLAGS) -o $@ lotus.c

celticstep.hpgl: celticstep Makefile
	./celticstep $(TITLE) $(BOLDPEN) -o $@

celticstep: celticstep.o hpgllib.o
	$(LD) -o $@ celticstep.o hpgllib.o -lm

celticstep.o: celticstep.c hpgllib.h
	$(CC) $(CFLAGS) -o $@ celticstep.c

op_moire.hpgl: op_moire Makefile
	./op_moire $(TITLE) $(BOLDPEN) -o $@

op_moire: op_moire.o hpgllib.o
	$(LD) -o $@ op_moire.o hpgllib.o -lm

op_moire.o: op_moire.c hpgllib.h
	$(CC) $(CFLAGS) -o $@ op_moire.c

qrplot.hpgl: qrplot Makefile
	./qrplot $(TITLE) $(BOLDPEN) -o $@

qrplot: qrplot.o hpgllib.o
	$(LD) -o $@ qrplot.o hpgllib.o -lm

qrplot.o: qrplot.c hpgllib.h
	$(CC) $(CFLAGS) -o $@ qrplot.c

pconic.hpgl: pconic Makefile
	./pconic $(TITLE) $(BOLDPEN) -o $@

pconic: pconic.o hpgllib.o
	$(LD) -o $@ pconic.o hpgllib.o -lm

pconic.o: pconic.c hpgllib.h
	$(CC) $(CFLAGS) -o $@ pconic.c

sqinsq.hpgl: sqinsq Makefile
	./sqinsq $(TITLE) $(BOLDPEN) -o $@

sqinsq: sqinsq.o hpgllib.o
	$(LD) -o $@ sqinsq.o hpgllib.o -lm

sqinsq.o: sqinsq.c hpgllib.h
	$(CC) $(CFLAGS) -o $@ sqinsq.c

spiralsq.hpgl: spiralsq Makefile
	./spiralsq $(TITLE) $(BOLDPEN) -o $@

spiralsq: spiralsq.o hpgllib.o
	$(LD) -o $@ spiralsq.o hpgllib.o -lm

spiralsq.o: spiralsq.c hpgllib.h
	$(CC) $(CFLAGS) -o $@ spiralsq.c

fraserspiral.hpgl: fraserspiral Makefile
	./fraserspiral $(TITLE) $(BOLDPEN) -o $@

fraserspiral: fraserspiral.o hpgllib.o
	$(LD) -o $@ fraserspiral.o hpgllib.o -lm

fraserspiral.o: fraserspiral.c hpgllib.h
	$(CC) $(CFLAGS) -o $@ fraserspiral.c

curve_stitching.hpgl: curve_stitching Makefile
	./curve_stitching $(TITLE) $(BOLDPEN) -o $@

curve_stitching: curve_stitching.o hpgllib.o
	$(LD) -o $@ curve_stitching.o hpgllib.o -lm

curve_stitching.o: curve_stitching.c hpgllib.h
	$(CC) $(CFLAGS) -o $@ curve_stitching.c

cs_rosette.hpgl: cs_rosette Makefile
	./cs_rosette $(TITLE) $(BOLDPEN) -o $@

cs_rosette: cs_rosette.o hpgllib.o
	$(LD) -o $@ cs_rosette.o hpgllib.o -lm

cs_rosette.o: cs_rosette.c hpgllib.h
	$(CC) $(CFLAGS) -o $@ cs_rosette.c

allover12.hpgl: allover12 Makefile
	./allover12 $(TITLE) $(BOLDPEN) -o $@

allover12: allover12.o hpgllib.o
	$(LD) -o $@ allover12.o hpgllib.o -lm

allover12.o: allover12.c hpgllib.h
	$(CC) $(CFLAGS) -o $@ allover12.c

allover13.hpgl: allover13 Makefile
	./allover13 $(TITLE) $(BOLDPEN) -o $@

allover13: allover13.o hpgllib.o
	$(LD) -o $@ allover13.o hpgllib.o -lm

allover13.o: allover13.c hpgllib.h
	$(CC) $(CFLAGS) -o $@ allover13.c

zigzag.hpgl: zigzag Makefile
	./zigzag $(TITLE) $(BOLDPEN) -o $@

zigzag: zigzag.o hpgllib.o
	$(LD) -o $@ zigzag.o hpgllib.o -lm

zigzag.o: zigzag.c hpgllib.h
	$(CC) $(CFLAGS) -o $@ zigzag.c

pin_and_cotton.hpgl: pin_and_cotton Makefile
	./pin_and_cotton $(TITLE) $(BOLDPEN) -o $@

pin_and_cotton: pin_and_cotton.o hpgllib.o
	$(LD) -o $@ pin_and_cotton.o hpgllib.o -lm

pin_and_cotton.o: pin_and_cotton.c hpgllib.h
	$(CC) $(CFLAGS) -o $@ pin_and_cotton.c

hconic2.hpgl: hconic2
	./hconic2 >hconic2.hpgl

hconic2: hconic2.o
	$(LD) -o $@ hconic2.o -lm

hconic2.o: hconic2.c
	$(CC) $(CFLAGS) -o $@ hconic2.c

coords.hpgl: coords Makefile
	./coords $(BOLDPEN) -o $@

coords: coords.o hpgllib.o
	$(LD) -o $@ coords.o hpgllib.o -lm

coords.o: coords.c hpgllib.h
	$(CC) $(CFLAGS) -o $@ coords.c

# Turtle graphics programs

hexagon.hpgl: hexagon
	./hexagon >hexagon.hpgl

hexagon: hexagon.o turtle.o
	$(LD) -o $@ hexagon.o turtle.o -lm

hexagon.o: hexagon.c turtle.h
	$(CC) $(CFLAGS) -o $@ hexagon.c

plottext: plottext.o turtle.o
	$(LD) -o $@ plottext.o turtle.o -lm

plottext.o: plottext.c turtle.h
	$(CC) $(CFLAGS) -o $@ plottext.c

flake.hpgl: flake
	./flake >flake.hpgl

flake: flake.o turtle.o
	$(LD) -o $@ flake.o turtle.o -lm

flake.o: flake.c turtle.h
	$(CC) $(CFLAGS) -o $@ flake.c

hilb.hpgl: hilb
	./hilb >hilb.hpgl

hilb: hilb.o turtle.o
	$(LD) -o $@ hilb.o turtle.o -lm

hilb.o: hilb.c turtle.h
	$(CC) $(CFLAGS) -o $@ hilb.c

tree.hpgl: tree
	./tree >tree.hpgl

tree: tree.o turtle.o
	$(LD) -o $@ tree.o turtle.o -lm

tree.o: tree.c turtle.h
	$(CC) $(CFLAGS) -o $@ tree.c

rtree.hpgl: rtree
	./rtree >rtree.hpgl

rtree: rtree.o turtle.o
	$(LD) -o $@ rtree.o turtle.o -lm

rtree.o: rtree.c turtle.h
	$(CC) $(CFLAGS) -o $@ rtree.c

dome.hpgl: dome
	./dome >dome.hpgl

dome: dome.o turtle.o
	$(LD) -o $@ dome.o turtle.o -lm

dome.o: dome.c turtle.h
	$(CC) $(CFLAGS) -o $@ dome.c

# Build the HPGL library
hpgllib.o: hpgllib.c hpgllib.h
	$(CC) $(CFLAGS) -o $@ hpgllib.c

# Build the turtle library
turtle.o: turtle.c turtle.h
	$(CC) $(CFLAGS) -o $@ turtle.c
