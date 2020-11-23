#ifndef SOLID_H
#define SOLID_H

#include<point3d.h>

class Solid
{
public:
    Solid();
    Solid(Point3D, Point3D, Point3D, Point3D,
          Point3D, Point3D, Point3D, Point3D);
    ~Solid();

    Point3D solidVertices[8]; // tablica wierzcholkow wyznaczajacych dana bryle
    Solid operator +(const Point3D); // przeciazenie operatora +
};

#endif // SOLID_H
