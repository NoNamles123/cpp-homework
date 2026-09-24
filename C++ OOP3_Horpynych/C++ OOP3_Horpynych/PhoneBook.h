#pragma once
#include "Contact.h"

class PhoneBook
{
private:
    Contact* m_pContacts[100];
    int m_nCount;

public:
    PhoneBook();
    ~PhoneBook();

    void addContact();
    void deleteContact();
    void searchContact();
    void showAll();
    void saveToFile();
    void loadFromFile();
    void menu();
};
