#include <iostream>
void gotoxy(int x, int y)
{
  COORD coord;
  coord.X = x;
  coord.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void printat(int x, int y, char c){
    gotoxy(x,y);
    printf( "%c", c );
}

void wipescreen(){
    system( "cls" );
}

class talker{
    public:
        std::string out;
        bool enabled;
};
std::ostream& operator<<( std::ostream& os, talker &rhs ){
    if( rhs.enabled ){
        os << rhs.out;
        rhs.out = "";
        return os;
    }
    else{
        return os;
    }
}
void operator>>( const std::string& input, talker &rhs ){
    if( rhs.enabled ){
        rhs.out += input;
    }
}
void operator>>( const char* input, talker &rhs ){
    if ( rhs.enabled ){
        rhs.out += std::string( input );
    }
}
void operator>>( const char input, talker &rhs ){
    if( rhs.enabled ){
        rhs.out += std::string(1, input);
    }
}
