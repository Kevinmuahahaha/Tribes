#include "random.h"
#include <iostream>
class dot{
    public:
        dot();
        dot( int _x, int _y=0, char _flag='+' );
        int x;
        int y;
        int age=0;
        char flag; // flag '+' means dead resident
        int strength;
        bool is_newborn = true;

        bool operator==( const dot &rhs );
//        std::ostream& operator<<( std::ostream& os, const dot &rhs );

    private:
        char _empty = ' ';
        char _death = _empty;
        int direction;
};
bool dot::operator==( const dot &rhs ){
    return this->x == rhs.x && this->y == rhs.y;
}
std::ostream& operator<<( std::ostream& os, const dot &rhs ){
    os << rhs.x << "," << rhs.y << "|" << rhs.flag;
    return os;
}
dot::dot()
: x(0), y(0), flag('+')
{
    Random r; r.seed(); strength = r.range(0,100);
}
dot::dot( int _x, int _y, char _flag )
: x(_x), y(_y), flag(_flag)
{
    Random r; r.seed(); strength = r.range(0,100);
}
