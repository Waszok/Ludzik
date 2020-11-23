#ifndef POINT_H
#define POINT_H

class Point
{
public:
    Point();
    Point(int, int);
    ~Point();

    int x;
    int y;

    bool operator ==(const Point); // przeciazenie operatora ==
    void swapPoint(Point &); // zamiana punktow
};

#endif // POINT_H
