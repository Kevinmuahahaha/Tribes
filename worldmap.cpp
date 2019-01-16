#include "worldmap.h"
void worldmap::display_newmap(){
    if( enabled ){
        wipescreen();
        for( int i=0 ; i<_y ; i++ ){
            for( int j=0 ; j<_x ; j++ ){
                std::cout << _world[i][j];
            }
            std::cout << "\n";
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

        std::cout<< "Adding Resident: " << tmp << std::endl;
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

        // Print Changed Dots

        /*
        Replace The Commented Codes with a Better Implementation
        system("cls");
        for( i=0 ; i<_y ; i++ ){
            for( j=0 ; j<_x ; j++ ){
                std::cout << _world[i][j];
            }
            std::cout << "\n";
        }
        */
        spread_residents();

        gotoxy( 0, _y_max + 2 );
        std::cout<< "\n\nGeneration: " << generations << std::endl;
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

