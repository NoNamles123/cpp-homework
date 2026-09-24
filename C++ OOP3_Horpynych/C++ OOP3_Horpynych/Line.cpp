#include "Line.h"
#include <iostream>

using namespace std;

Line::Line() : Line{ 0, 0, 0, 0, 0, 0 }
{
}

Line::Line(Point p1, Point p2)
    : m_point1{ p1 }, m_point2{ p2 }
{
}

Line::Line(int x1, int y1, int x2, int y2)
    : Line{ x1, y1, 0, x2, y2, 0 }
{
}

Line::Line(int x1, int y1, int z1, int x2, int y2, int z2)
    : m_point1{ x1, y1, z1 }, m_point2{ x2, y2, z2 }
{
}

void Line::print()
{
    cout << "Point 1: ";
    m_point1.print();
    cout << "Point 2: ";
    m_point2.print();
}
