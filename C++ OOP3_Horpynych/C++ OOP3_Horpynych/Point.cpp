#include "Point.h"
#include <iostream>

using namespace std;

Point::Point() : Point{ 0, 0, 0 }
{
}

Point::Point(int x, int y) : Point{ x, y, 0 }
{
}

Point::Point(int x, int y, int z)
    : m_nX{ x }, m_nY{ y }, m_nZ{ z }
{
}

void Point::setX(int x)
{
    m_nX = x;
}

void Point::setY(int y)
{
    m_nY = y;
}

void Point::setZ(int z)
{
    m_nZ = z;
}

int Point::getX()
{
    return m_nX;
}

int Point::getY()
{
    return m_nY;
}

int Point::getZ()
{
    return m_nZ;
}

void Point::print()
{
    cout << "X: " << m_nX
         << " Y: " << m_nY
         << " Z: " << m_nZ << endl;
}
