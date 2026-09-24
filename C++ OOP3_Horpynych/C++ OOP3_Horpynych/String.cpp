#include "String.h"
#include <iostream>
#include <cstring>

using namespace std;

int String::m_nCount{ 0 };

String::String() : String{ 80 }
{
}

String::String(int size)
    : m_pStr{ new char[size + 1] }, m_nSize{ size }
{
    m_pStr[0] = '\0';
    m_nCount++;
}

String::String(const char* str)
    : String{ (int)strlen(str) }
{
    strcpy_s(m_pStr, m_nSize + 1, str);
}

String::~String()
{
    if (m_pStr != nullptr)
    {
        delete[] m_pStr;
        m_pStr = nullptr;
    }
    m_nCount--;
}

void String::input()
{
    char buffer[1000];
    cout << "Enter string: ";
    cin.getline(buffer, 1000);

    if (m_pStr != nullptr)
        delete[] m_pStr;

    m_nSize = (int)strlen(buffer);
    m_pStr = new char[m_nSize + 1];
    strcpy_s(m_pStr, m_nSize + 1, buffer);
}

void String::print()
{
    cout << m_pStr << endl;
}

int String::getCount()
{
    return m_nCount;
}
