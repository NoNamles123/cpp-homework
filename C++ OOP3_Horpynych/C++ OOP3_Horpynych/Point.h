#pragma once

class Point
{
private:
    int m_nX;
    int m_nY;
    int m_nZ;

public:
    Point();
    Point(int x, int y);
    Point(int x, int y, int z);

    void setX(int x);
    void setY(int y);
    void setZ(int z);

    int getX();
    int getY();
    int getZ();

    void print();
};
