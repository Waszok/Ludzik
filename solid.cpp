#include "solid.h"

Solid::Solid()
{
}

Solid::Solid(Point3D a, Point3D b, Point3D c, Point3D d, Point3D e, Point3D f, Point3D g, Point3D h) // konstruktor
{
    solidVertices[0] = a; solidVertices[1] = b; solidVertices[2] = c; solidVertices[3] = d;
    solidVertices[4] = e; solidVertices[5] = f; solidVertices[6] = g; solidVertices[7] = h;
}

Solid::~Solid(){ // destruktor
}

Solid Solid::operator +(const Point3D other) // przesuwamy bryle (jej kazdy wierzcholek) o podany wektor w przestrzeni 3d
{
    Solid tmp;
    for ( int i = 0; i < 8; i++ ){
        tmp.solidVertices[i].x = solidVertices[i].x + other.x;
        tmp.solidVertices[i].y = solidVertices[i].y + other.y;
        tmp.solidVertices[i].z = solidVertices[i].z + other.z;
    }
    return tmp;
}
