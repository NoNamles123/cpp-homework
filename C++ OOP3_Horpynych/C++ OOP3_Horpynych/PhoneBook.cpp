#include "PhoneBook.h"
#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

PhoneBook::PhoneBook() : m_nCount{ 0 }
{
    for (int i = 0; i < 100; i++)
        m_pContacts[i] = nullptr;
}

PhoneBook::~PhoneBook()
{
    for (int i = 0; i < m_nCount; i++)
        delete m_pContacts[i];
}

void PhoneBook::addContact()
{
    if (m_nCount >= 100)
    {
        cout << "Phone book is full!" << endl;
        return;
    }

    m_pContacts[m_nCount] = new Contact;
    m_pContacts[m_nCount]->input();
    m_nCount++;
    cout << "Contact added." << endl;
}

void PhoneBook::deleteContact()
{
    char fullName[100];
    cout << "Enter full name: ";
    cin.getline(fullName, 100);

    for (int i = 0; i < m_nCount; i++)
    {
        if (strcmp(m_pContacts[i]->getFullName(), fullName) == 0)
        {
            delete m_pContacts[i];
            for (int j = i; j < m_nCount - 1; j++)
                m_pContacts[j] = m_pContacts[j + 1];

            m_pContacts[m_nCount - 1] = nullptr;
            m_nCount--;
            cout << "Contact deleted." << endl;
            return;
        }
    }

    cout << "Contact not found." << endl;
}

void PhoneBook::searchContact()
{
    char fullName[100];
    cout << "Enter full name: ";
    cin.getline(fullName, 100);

    for (int i = 0; i < m_nCount; i++)
    {
        if (strcmp(m_pContacts[i]->getFullName(), fullName) == 0)
        {
            m_pContacts[i]->print();
            return;
        }
    }

    cout << "Contact not found." << endl;
}

void PhoneBook::showAll()
{
    if (m_nCount == 0)
    {
        cout << "Phone book is empty." << endl;
        return;
    }

    for (int i = 0; i < m_nCount; i++)
    {
        cout << endl << "Contact " << i + 1 << endl;
        m_pContacts[i]->print();
    }
}

void PhoneBook::saveToFile()
{
    ofstream file("PhoneBook.txt");

    for (int i = 0; i < m_nCount; i++)
    {
        file << m_pContacts[i]->getFullName() << "|"
             << m_pContacts[i]->getHomePhone() << "|"
             << m_pContacts[i]->getWorkPhone() << "|"
             << m_pContacts[i]->getMobilePhone() << "|"
             << m_pContacts[i]->getInfo() << endl;
    }

    file.close();
    cout << "Phone book saved." << endl;
}

void PhoneBook::loadFromFile()
{
    ifstream file("PhoneBook.txt");
    if (!file)
    {
        cout << "File not found." << endl;
        return;
    }

    for (int i = 0; i < m_nCount; i++)
    {
        delete m_pContacts[i];
        m_pContacts[i] = nullptr;
    }
    m_nCount = 0;

    char fullName[100];
    char homePhone[20];
    char workPhone[20];
    char mobilePhone[20];
    char info[100];

    while (m_nCount < 100 && file.getline(fullName, 100, '|'))
    {
        file.getline(homePhone, 20, '|');
        file.getline(workPhone, 20, '|');
        file.getline(mobilePhone, 20, '|');
        file.getline(info, 100);

        m_pContacts[m_nCount] = new Contact{
            fullName, homePhone, workPhone, mobilePhone, info
        };
        m_nCount++;
    }

    file.close();
    cout << "Phone book loaded." << endl;
}

void PhoneBook::menu()
{
    int choice;
    do
    {
        cout << endl;
        cout << "===== PHONE BOOK =====" << endl;
        cout << "1. Add contact" << endl;
        cout << "2. Delete contact" << endl;
        cout << "3. Search contact" << endl;
        cout << "4. Show all" << endl;
        cout << "5. Save to file" << endl;
        cout << "6. Load from file" << endl;
        cout << "0. Exit" << endl;
        cout << "Choose: ";
        cin >> choice;
        cin.ignore(1000, '\n');

        switch (choice)
        {
        case 1: addContact(); break;
        case 2: deleteContact(); break;
        case 3: searchContact(); break;
        case 4: showAll(); break;
        case 5: saveToFile(); break;
        case 6: loadFromFile(); break;
        case 0: break;
        default: cout << "Incorrect choice." << endl;
        }
    } while (choice != 0);
}
