// testApp.cc

// Spirograph library for the Bristol Dorkbot Draw project
//
// Copyright Tarim 2010
// Released as open source under WTFPL

#include "spiro.h"

//--------------------------------------------------------------
void Spiro::setup()
{
    printf ("IN;\n");
    printf ("SP1;\n");
//  printf ("VS10;\n");
    printf ("PU;PA%d,%d;\n", MINX + (10 * 40), MINY + (10 * 40));
    printf ("DR0,1;\n");
//  printf ("LBhttp://www.dorkbot.org/dorkbotbristol%c;\n", 3);
    printf ("LBBristol Hackspace%c;\n", 3);
    printf ("PU%d,%d;EA%d,%d\n", MINX, MINY, MAXX, MAXY);
    printf ("PU%d,%d;PD;PA%d,%d,%d,%d,%d,%d,%d,%d\n", MINX, MINY,
             MINX, MAXY, MAXX, MAXY, MAXX, MINY, MINX, MINY);
}

void Spiro::close()
{
    printf ("PU;\n");
    printf ("PA%d,%d;\n", MINX, MINY);
    printf ("SP0;\n");
}

//--------------------------------------------------------------
void Spiro::draw()
{
    const real_t step = 0.06;

    // ratio 1:2 with different pen positions
    spiro.setScale( 65 * MM, 75 * MM, 75 * MM, 75 * MM );
    spiro.setPen(1);
    for( real_t p = 0; p <= 1; p += 0.2 ) {
        spiro.setWheels( 1, 2, p, step );
        spiro.drawSpiro();
    }

    // ratio 1:3 with different pen positions
    spiro.setScale( 190 * MM, 75 * MM, 75 * MM, 75 * MM );
    spiro.setPen (2);
    for( real_t p = 0; p <= 1; p += 0.2 ) {
        spiro.setWheels( 1, 3, p, step );
        spiro.drawSpiro();
    }

    // ratio 2:3 (inner wheel more than half outer wheel) - gives loops
    spiro.setScale( 330 * MM, 75 * MM, 75 * MM, 75 * MM );
    spiro.setPen (3);
//  spiro.setWheels( 2, 3, 0.6, step );
    spiro.setWheels( 9, 14, 0.6, step );
    spiro.drawSpiro();

    // ratio 23:37 lots of loops
    spiro.setScale( 125 * MM, 200 * MM, 75 * MM, 75 * MM );
    spiro.setPen (4);
    spiro.setWheels( 23, 37, 0.6, step );
    spiro.drawSpiro();

    // ratio 5:37 creates a band of lines - at different scales
    spiro.setPen (1);
    spiro.setWheels( 5, 37, 0.5, step );
    for( int j = 12; j <= 60; j += 12 ) {
        spiro.setScale( 260 * MM, 200 * MM, j * MM, j * MM );
        spiro.drawSpiro();
    }
}

int main (void)
{
   Spiro sp;
   
   sp.setup ();
   sp.draw ();
   sp.close ();
}
