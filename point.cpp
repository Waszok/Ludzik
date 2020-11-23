#include "point.h"

Point::Point()
{
}

Point::Point(int x, int y){ // kontruktor
    this->x = x;
    this->y = y;
}

Point::~Point(){ //destruktor
}

bool Point::operator ==(const Point other){ // sprawdzamy czy punkty sa rowne, czyli czy (x1,y1)==(x2,y2)
    if ( x == other.x && y == other.y )
        return true;
    else
        return false;
}

void Point::swapPoint(Point &other){ // zamieniamy ze soba dwa punkty (swap)
    int tmp;

    tmp = this->x;
    this->x = other.x;
    other.x = tmp;

    tmp = this->y;
    this->y = other.y;
    other.y = tmp;
}

