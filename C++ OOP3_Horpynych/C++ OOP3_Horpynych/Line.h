#pragma once
#include "Point.h"

class Line
{
private:
    Point m_point1;
    Point m_point2;

public:
    Line();
    Line(Point p1, Point p2);
    Line(int x1, int y1, int x2, int y2);
    Line(int x1, int y1, int z1, int x2, int y2, int z2);

    void print();
};
