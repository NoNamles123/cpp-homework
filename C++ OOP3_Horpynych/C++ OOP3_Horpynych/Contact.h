#pragma once

class Contact
{
private:
    char* m_pFullName;
    char m_homePhone[20];
    char m_workPhone[20];
    char m_mobilePhone[20];
    char m_info[100];

public:
    Contact();
    Contact(const char* fullName, const char* homePhone, const char* workPhone,
            const char* mobilePhone, const char* info);
    ~Contact();

    void setFullName(const char* fullName);
    void input();
    void print();

    inline char* getFullName() { return m_pFullName; }
    inline char* getHomePhone() { return m_homePhone; }
    inline char* getWorkPhone() { return m_workPhone; }
    inline char* getMobilePhone() { return m_mobilePhone; }
    inline char* getInfo() { return m_info; }
};
