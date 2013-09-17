// Spiro.h

// Spirograph library for the Bristol Dorkbot Draw project
//
// Copyright Tarim 2010
// Relased as open source under WTFPL

#ifndef _TEST_APP
#define _TEST_APP

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// swap these for a plotter type device
// #define OFSCREEN
#define PLOTTER

#define A1

#ifdef A1
#define MINX (-15970)
#define MINY (-10870)
#define MAXX (15970)
#define MAXY (10870)
#define MM 80  // 40 plotter units per millimetre
#else
#define MINX (0)
#define MINY (0)
#define MAXX (15970)
#define MAXY (10870)
#define MM 40  // 40 plotter units per millimetre
#endif /* A1 */

#define PI (3.14156535)



// all real values are same type so easily changed for more precision
typedef float real_t;

// tools to implement scaling, drawto, moveto etc
// moveTo_ and drawTo_ are the "device drivers" replace these as you like

class drawTool {
public:
    // these will be set before moveTo_ and drawTo_ are called
    int previousX, previousY;        // previous position
    int currentX, currentY;            // current position

    // define your own DRAWTO and MOVETO routines for a PLOTTER
    #ifdef PLOTTER
        void moveTo_() {
            printf ("PU;PA%d,%d;\n", currentX + MINX, currentY + MINY);
        };

        void drawTo_() {
            printf ("PD;PA%d,%d;\n", currentX + MINX, currentY + MINY);
        };
    #endif

    #ifdef OFSCREEN
        // don't need a moveTo in Open Frameworks
        void moveTo_() { };

        // because drawTo uses the previous position
        void drawTo_() {
            ofLine( previousX, previousY, currentX, currentY );
        };
    #endif


    int scaleX, scaleY;            // size of unit square in pixels
    int centreX, centreY;        // centre offset in pixels

    // set the scaling factors
    void setScale( const int cX, const int cY, const int sX, const int sY ) {
        scaleX = sX;
        scaleY = sY;
        centreX = cX;
        centreY = cY;

        // Draw unit circle
        //moveTo (0.0, 0.0);
        //printf ("CI%d;\n", scaleX);
        
        // Draw box around current spiro
        //moveTo (-1.0, -1.0);
        //drawTo ( 1.0, -1.0);
        //drawTo ( 1.0,  1.0);
        //drawTo (-1.0,  1.0);
        //drawTo (-1.0, -1.0);
    };

    // convert real_t co-ords to scaled pixel positions
    void setCurrentXY( const real_t x, const real_t y ) {
        currentX = (int)(x * scaleX + centreX);
        currentY = (int)(y * scaleY + centreY);
    };

    // copy current position to previous position
    void setPreviousXY() {
        previousX = currentX;
        previousY = currentY;
    };

    // general moveTo which copes with plotters or screens
    void moveTo( const real_t x, const real_t y ) {
        setCurrentXY( x, y );
        moveTo_();
        setPreviousXY();
    };

    // general drawTo which copes with plotters or screens
    void drawTo( const real_t x, const real_t y ) {
        setCurrentXY( x, y );
        drawTo_();
        setPreviousXY();
    };
    
    void setPen( const int pen ) {
        printf ("SP%d;\n", pen);
    };
};

class spirograph: public drawTool {
public:
    // ratio of the radii of wheels as integers so 2:1 is same as 4:2
    // (though 4:2 will draw it twice)
    real_t innerRadius;
    real_t outerRadius;

    // ratio of distance of pen from centre of inner wheel (between 0 and 1)
    real_t penDistance;

    // increment for angle of inner wheel about centre of outer wheel
    // in radians
    real_t delta;

    // set the wheel ratios, pen distance and step increment
    void setWheels( const real_t rI, const real_t rO, const real_t pD, const real_t d ) {
        innerRadius = rI;
        outerRadius = rO;
        penDistance = pD;
        delta = d;
    };

    // draw a spirograph
    void drawSpiro() {
        real_t limit;
        const real_t outerF = ( outerRadius - innerRadius ) / outerRadius;
        const real_t innerF = innerRadius * penDistance / outerRadius;
        const real_t thetaF = ( outerRadius - innerRadius ) / innerRadius;
        
        if (outerRadius < innerRadius)
            limit = 2*PI * outerRadius + delta;
        else
            limit = 2*PI * innerRadius + delta;

        moveTo( 0, outerF + innerF );
        for( real_t theta = 0; theta < limit; theta += delta ) {
            drawTo(
                outerF * sin(theta) - innerF * sin( thetaF * theta ),
                outerF * cos(theta) + innerF * cos( thetaF * theta )
            );
        }
    };

    // draw a circle
    void drawCircle() {
        moveTo( 0, 1 );
        for( real_t theta = 0; theta <= 2 * PI; theta += delta ) {
            drawTo( sin(theta), cos(theta) );
        };
    };
};


// openframeworks stuff
class Spiro {

    public:

        void setup();
        void draw();
        void setPen();
        void close ();

        spirograph spiro;
};

#endif
