#pragma once
#include <time.h>
#include <cstdlib>
class Random{
    public:
        int range( int min, int max );
        void seed();
};
void Random::seed(){
    srand(time(NULL));
}
int Random::range( int min, int max ){
    int ret;

    ret = (int)( rand()%( max+1 ) );
    if( ret < min ) return min;
    else return ret;
}
