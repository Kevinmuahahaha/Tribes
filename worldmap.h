#include <iostream>
#include <windows.h>
#include <vector>
#include <algorithm>
#include <set>
#include <thread>
#include <cstdio>
#include "components.h"
#include "random.h"
#include "utilities.h"

const char death = ' ';
class worldmap{
    friend class talker;
    public:
        talker talk;
        worldmap( int x, int y );
        worldmap(){};
        void display_newmap();
        void display();
        void add( dot& d );
        void kill( dot& d );
        void setstate( bool state );
        bool getstate();

        int _x_min;
        int _y_min;
        int _x_max;
        int _y_max;
        int generations=0;
        int max_generations= 50;

        void spread_residents();
        void auto_add_residents( int amount = 4 );
        inline void set_debug( bool d ){ _debug = d; }
        dot& findlast( const dot& d_new );
        bool blocked( dot& d, int direction );

    private:
        int max_age = 45;
        bool _debug;
        char** _world;
        int _x; // columns
        int _y; // rows
        int refresh_rate = 0;
        bool enabled;
        std::vector<dot> residents;
        void move_residents();
        char _empty = ' ';
};

void worldmap::display_newmap(){
    if( enabled ){
        wipescreen();
        for( int i=0 ; i<_y ; i++ ){
            for( int j=0 ; j<_x ; j++ ){
                _world[i][j] >> talk;
                std::cout << talk;
            }
            "\n" >> talk;
            std::cout << talk;
        }
    }
}
dot& worldmap::findlast( const dot& d_new ){
    for( unsigned i=0; i<residents.size() ; ++i ){
        if( residents[i] == d_new ) return residents[i];
    }
}
void worldmap::kill( dot& d ){
    d.flag = death;
    printat( d.x, d.y , death);           // For Graphics
    old: _world[ d.y ][ d.x ] = death;    // For Logic
    residents.erase( std::find( residents.begin(),residents.end(), d ) );
}
void worldmap::auto_add_residents( int amount ){
    std::vector<dot> auto_dots;
    Random r;
    r.seed();
    char flag = '!';
    dot tmp;
    bool flag_exist = false;

    for( int i=0 ; i<amount ; i++, flag++ ){
        flag = (char)r.range((int)'!',(int)'~');
        if( flag == _empty || flag == death ){
            --i; continue;
        }

        flag_exist = false;
        for( int i=0; i<auto_dots.size() ; ++i){
            if( auto_dots[i].flag == flag ){
                flag_exist = true;
            }
        }
        if( flag_exist ){
            --i; continue;
        }

        tmp = dot( r.range( _x_min, _x_max -1 ), r.range( _y_min, _y_max -1 ), flag );
        for( unsigned index=0 ; index<auto_dots.size() ; index++ ){
            while( tmp == auto_dots[index] ){
                tmp = dot( r.range( _x_min, _x_max -1 ), r.range( _y_min, _y_max -1 ), flag );
            }
        }

        "Adding Resident: " >> talk;
        tmp >> talk;
        "\n" >>talk;
        std::cout<< talk;

        auto_dots.push_back( tmp );
    }
    for( unsigned index=0 ; index<auto_dots.size() ; index++ ){
        add( auto_dots[index] );
    }
}
void worldmap::add( dot& d ){

    bool overlay = false;
    if( d.x < _x_max && d.x >= _x_min && d.y < _y_max && d.y >= _y_min ){
           if( _world[ d.y ][ d.x ] != _empty && _world[ d.y ][ d.x ] != death ){
             overlay = true;
             if( _world[ d.y ][ d.x ] != d.flag ){
                dot tmp = findlast( d );
                if( tmp.strength > d.strength ){

                    printat( d.x, d.y, tmp.flag );
                    _world[ d.y ][ d.x ] = tmp.flag;
                    // it's not "Kill d"
                    // it's "don't add d"
                }
                else if( tmp.strength < d.strength ){
                    kill( tmp );

                    printat( d.x, d.y, d.flag );
                    _world[ d.y ][ d.x ] = d.flag;
                    d.strength-=30;
                    residents.push_back(d);
                }
                else{
                    kill( tmp );

                    printat( d.x, d.y, death);
                    _world[ d.y ][ d.x ] = death;
                }
             }
           }
           else{
            residents.push_back( d );

            printat( d.x, d.y, d.flag);
            _world[ d.y ][ d.x ] = d.flag;
           }
    }
}
void worldmap::setstate( bool state ){
    enabled = state;
    talk.enabled = enabled;
}
bool worldmap::getstate(){
    return enabled;
}
void worldmap::display(){
    int i=0;
    int j=0;
    while( enabled ){
        ++generations;
        if( generations >= max_generations ){
            break;
        }
        if( _debug ) getchar();

        for( unsigned i=0 ; i<residents.size() ; ++i){
            residents[i].age++;
            residents[i].is_newborn = false;
        }
        for( unsigned i=0 ; i<residents.size() ; ++i){
            if( residents[i].age > max_age ) kill( residents[i] );
        }

        spread_residents();

        gotoxy( 0, _y_max + 2 );

        "\n\nGeneration: " >> talk;
        generations >> talk;
        "\n" >>talk;
        std::cout<< talk;

        Sleep( refresh_rate );
    }
}
bool worldmap::blocked( dot& d, int direction ){
    if( direction == 1 ){ // left
        if( d.x == 0 ) return true;
        if( _world[ d.y ][ d.x-1 ] == d.flag ) return true;
        return false;
    }
    if( direction == 2 ){ // right
        if( d.x >= _x_max-1 ) return true;
        if( _world[ d.y ][ d.x+1 ] == d.flag ) return true;
        return false;
    }
    if( direction == 3 ){ // up
        if( d.y == 0 ) return true;
        if( _world[ d.y-1 ][ d.x ] == d.flag ) return true;
        return false;
    }
    if( direction == 4 ){ // down
        if( d.y >= _y_max-1 ) return true;
        if( _world[ d.y+1 ][ d.x ] == d.flag ) return true;
        return false;
    }
}
void worldmap::spread_residents(){
    Random r;
    //r.seed();
    int direction;
    dot tmp;
    for( unsigned i=0 ; i<residents.size() ; i++){
        direction = r.range(1,4);

        if( residents[i].flag == death ) continue;
        if( blocked( residents[i], direction  ) ) continue;
        if( residents[i].is_newborn ) continue;



        if( direction == 1 ){ // left
            tmp = dot ( residents[i].x -1 , residents[i].y, residents[i].flag );
        }
        else if( direction == 2 ){ // right
            tmp = dot ( residents[i].x +1 , residents[i].y, residents[i].flag );
        }
        else if( direction == 3 ){ // up
            tmp = dot ( residents[i].x , residents[i].y - 1, residents[i].flag );
        }
        else if( direction == 4 ){ // down
            tmp = dot ( residents[i].x , residents[i].y + 1, residents[i].flag );
        }
        add( tmp );
    }

    // iterate through residents
        // skip dead residents
        // generate new dot ( using direction )
            // ( keep flag, random strength )
        // add the new dot

    // detect collision
        // start killing
}


worldmap::worldmap( int x, int y )
: _x(x), _y(y), enabled( true )
{

    talk.enabled = enabled;
    _x_min = 0;
    _y_min = 0;
    _x_max = x;
    _y_max = y;


    _world = new char*[y];
    for( int i=0 ; i<y ; i++ ){
        _world[i] = new char[x];
    }
    for( int i=0 ; i<y ; i++){
        for( int j=0 ; j<x ; j++ ){
            _world[i][j] = _empty;
        }
    }
}

