#pragma once
#include <iostream>
#include <thread>
#include <windows.h>
#include "worldmap.h"
#include "random.h"
#include <exception>
#include "random.h"
using namespace std;

int main(void){
    int x = 70;
    int y = 35;
    int resident_amount = 4;
    worldmap m;
    Random r;



    while( true ){
            resident_amount =  r.range(2,20); // random resident amount
        try{
            m = worldmap(x,y);
            m.set_debug( true );
            m.setstate( true ); // Show Map

/*      The 4 Kingdoms Test
            dot b(m._x_min,m._y_max-1, '*');

            dot c(m._x_max-1, 0, '@' );

            dot a(0,0,'.');

            dot d(m._x_max-1, m._y_max-1, '+' );

            m.add( a );
            m.add( b );
            m.add( c );
            m.add( d );
*/

            m.auto_add_residents( resident_amount );
            m.display_newmap(); // print the entire base map

            m.display();
        }
        catch ( ... ){
            cout << "Current: " << m.generations << ". Restarting..." << endl;
            getchar();
        }
    }
    return 0;
}
