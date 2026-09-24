#include "Contact.h"
#include <iostream>
#include <cstring>

using namespace std;

Contact::Contact() : Contact{ "", "", "", "", "" }
{
}

Contact::Contact(const char* fullName, const char* homePhone, const char* workPhone,
                 const char* mobilePhone, const char* info)
    : m_pFullName{ nullptr }
{
    setFullName(fullName);
    strcpy_s(m_homePhone, 20, homePhone);
    strcpy_s(m_workPhone, 20, workPhone);
    strcpy_s(m_mobilePhone, 20, mobilePhone);
    strcpy_s(m_info, 100, info);
}

Contact::~Contact()
{
    if (m_pFullName != nullptr)
    {
        delete[] m_pFullName;
        m_pFullName = nullptr;
    }
}

void Contact::setFullName(const char* fullName)
{
    if (m_pFullName != nullptr)
        delete[] m_pFullName;

    int size = (int)strlen(fullName);
    m_pFullName = new char[size + 1];
    strcpy_s(m_pFullName, size + 1, fullName);
}

void Contact::input()
{
    char fullName[100];

    cout << "Full name: ";
    cin.getline(fullName, 100);
    setFullName(fullName);

    cout << "Home phone: ";
    cin.getline(m_homePhone, 20);
    cout << "Work phone: ";
    cin.getline(m_workPhone, 20);
    cout << "Mobile phone: ";
    cin.getline(m_mobilePhone, 20);
    cout << "Additional info: ";
    cin.getline(m_info, 100);
}

void Contact::print()
{
    cout << "Full name: " << m_pFullName << endl;
    cout << "Home phone: " << m_homePhone << endl;
    cout << "Work phone: " << m_workPhone << endl;
    cout << "Mobile phone: " << m_mobilePhone << endl;
    cout << "Additional info: " << m_info << endl;
}
