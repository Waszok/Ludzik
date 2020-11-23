#ifndef POINT3D_H
#define POINT3D_H

class Point3D
{
public:
    Point3D();
    Point3D(int, int, int);
    ~Point3D();

    int x;
    int y;
    int z;

    bool operator ==(const Point3D); // przeciazenie operatora ==
    Point3D operator +(const Point3D); // przeciazenie operatora +
};

#endif // POINT3D_H
