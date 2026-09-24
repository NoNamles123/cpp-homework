#pragma once

class String
{
private:
    char* m_pStr;
    int m_nSize;
    static int m_nCount;

public:
    String();
    String(int size);
    String(const char* str);
    ~String();

    void input();
    void print();

    static int getCount();
};
