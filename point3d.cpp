#include "point3d.h"

Point3D::Point3D()
{
}

Point3D::Point3D(int x, int y, int z) // konstruktor
{
    this -> x = x;
    this -> y = y;
    this -> z = z;
}

Point3D::~Point3D(){ // destruktor
}

bool Point3D::operator ==(const Point3D other )
{
    if ( x == other.x && y == other.y && z == other.z) //sprawdzamy czy punkty sa rowne, czyli czy (x1,y1,z1)==(x2,y2,z2)
        return true;
    else
        return false;
}

Point3D Point3D::operator +(const Point3D other) // przesuwamy punkt o podany wektor w przestrzeni 3d
{
    Point3D tmp;
    tmp.x = x + other.x;
    tmp.y = y + other.y;
    tmp.z = z + other.z;
    return tmp;
}

