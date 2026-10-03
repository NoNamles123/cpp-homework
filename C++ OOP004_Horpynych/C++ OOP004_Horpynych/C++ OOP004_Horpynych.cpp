#include <iostream>
#include <cstring>

using namespace std;

char* copyText(const char* text)
{
    if (text == nullptr)
        text = "";

    int length = (int)strlen(text);
    char* result = new char[length + 1];

    for (int i = 0; i <= length; i++)
        result[i] = text[i];

    return result;
}

class Date
{
private:
    int day;
    int month;
    int year;

public:
    Date(int day, int month, int year)
        : day(day), month(month), year(year)
    {
    }

    Date() : Date(1, 1, 2000)
    {
    }

    Date(Date& other)
        : day(other.day), month(other.month), year(other.year)
    {
    }

    void setDay(int day)
    {
        this->day = day;
    }

    void setMonth(int month)
    {
        this->month = month;
    }

    void setYear(int year)
    {
        this->year = year;
    }

    int getDay()
    {
        return day;
    }

    int getMonth()
    {
        return month;
    }

    int getYear()
    {
        return year;
    }

    void print()
    {
        if (day < 10)
            cout << '0';
        cout << day << '.';

        if (month < 10)
            cout << '0';
        cout << month << '.' << year;
    }
};

class Human
{
private:
    unsigned long long identificationNumber;
    char* surname;
    char* name;
    char* patronymic;
    Date birthDate;
    static int count;

    void replaceText(char*& destination, const char* source)
    {
        delete[] destination;
        destination = copyText(source);
    }

public:
    Human(unsigned long long identificationNumber,
        const char* surname,
        const char* name,
        const char* patronymic,
        int day,
        int month,
        int year)
        : identificationNumber(identificationNumber),
        surname(copyText(surname)),
        name(copyText(name)),
        patronymic(copyText(patronymic)),
        birthDate(day, month, year)
    {
        count++;
    }

    Human() : Human(0, "", "", "", 1, 1, 2000)
    {
    }

    Human(Human& other)
        : identificationNumber(other.identificationNumber),
        surname(copyText(other.surname)),
        name(copyText(other.name)),
        patronymic(copyText(other.patronymic)),
        birthDate(other.birthDate)
    {
        count++;
    }

    ~Human()
    {
        delete[] surname;
        delete[] name;
        delete[] patronymic;
        count--;
    }

    static int getCount()
    {
        return count;
    }

    void setIdentificationNumber(unsigned long long identificationNumber)
    {
        this->identificationNumber = identificationNumber;
    }

    void setSurname(const char* surname)
    {
        replaceText(this->surname, surname);
    }

    void setName(const char* name)
    {
        replaceText(this->name, name);
    }

    void setPatronymic(const char* patronymic)
    {
        replaceText(this->patronymic, patronymic);
    }

    void setBirthDate(int day, int month, int year)
    {
        birthDate.setDay(day);
        birthDate.setMonth(month);
        birthDate.setYear(year);
    }

    unsigned long long getIdentificationNumber()
    {
        return identificationNumber;
    }

    char* getSurname()
    {
        return surname;
    }

    char* getName()
    {
        return name;
    }

    char* getPatronymic()
    {
        return patronymic;
    }

    Date& getBirthDate()
    {
        return birthDate;
    }

    void copyFrom(Human& other)
    {
        identificationNumber = other.identificationNumber;
        setSurname(other.surname);
        setName(other.name);
        setPatronymic(other.patronymic);
        setBirthDate(other.birthDate.getDay(), other.birthDate.getMonth(), other.birthDate.getYear());
    }

    void print()
    {
        cout << "ID: " << identificationNumber << '\n';
        cout << "Surname: " << surname << '\n';
        cout << "Name: " << name << '\n';
        cout << "Patronymic: " << patronymic << '\n';
        cout << "Birth date: ";
        birthDate.print();
        cout << '\n';
    }
};

int Human::count = 0;

class String
{
private:
    char* text;
    int size;
    static int count;

public:
    String(int size)
        : size(size)
    {
        if (this->size < 1)
            this->size = 1;

        text = new char[this->size + 1];
        text[0] = '\0';
        count++;
    }

    String() : String(80)
    {
    }

    String(const char* userText) : String((int)strlen(userText))
    {
        for (int i = 0; i <= size; i++)
            text[i] = userText[i];
    }

    String(String& other)
        : size(other.size)
    {
        text = new char[size + 1];

        for (int i = 0; i <= size; i++)
            text[i] = other.text[i];

        count++;
    }

    ~String()
    {
        delete[] text;
        count--;
    }

    static int getCount()
    {
        return count;
    }

    void input()
    {
        char buffer[1000];
        cin.getline(buffer, 1000);

        int length = (int)strlen(buffer);

        if (length > size)
        {
            delete[] text;
            size = length;
            text = new char[size + 1];
        }

        for (int i = 0; i <= length; i++)
            text[i] = buffer[i];
    }

    void print()
    {
        cout << text << '\n';
    }
};

int String::count = 0;

class Apartment
{
private:
    Human* people;
    int peopleCount;

public:
    Apartment(int peopleCount)
        : peopleCount(peopleCount)
    {
        if (this->peopleCount < 0)
            this->peopleCount = 0;

        if (this->peopleCount == 0)
            people = nullptr;
        else
            people = new Human[this->peopleCount];
    }

    Apartment() : Apartment(0)
    {
    }

    Apartment(Apartment& other)
        : peopleCount(other.peopleCount)
    {
        if (peopleCount == 0)
        {
            people = nullptr;
        }
        else
        {
            people = new Human[peopleCount];

            for (int i = 0; i < peopleCount; i++)
                people[i].copyFrom(other.people[i]);
        }
    }

    ~Apartment()
    {
        delete[] people;
    }

    int getPeopleCount()
    {
        return peopleCount;
    }

    Human& getHuman(int index)
    {
        return people[index];
    }

    void setHuman(int index,
        unsigned long long identificationNumber,
        const char* surname,
        const char* name,
        const char* patronymic,
        int day,
        int month,
        int year)
    {
        if (index < 0 || index >= peopleCount)
            return;

        people[index].setIdentificationNumber(identificationNumber);
        people[index].setSurname(surname);
        people[index].setName(name);
        people[index].setPatronymic(patronymic);
        people[index].setBirthDate(day, month, year);
    }

    void copyFrom(Apartment& other)
    {
        delete[] people;

        peopleCount = other.peopleCount;

        if (peopleCount == 0)
        {
            people = nullptr;
            return;
        }

        people = new Human[peopleCount];

        for (int i = 0; i < peopleCount; i++)
            people[i].copyFrom(other.people[i]);
    }

    void print()
    {
        cout << "People in apartment: " << peopleCount << '\n';

        for (int i = 0; i < peopleCount; i++)
        {
            cout << "Person " << i + 1 << ":\n";
            people[i].print();
            cout << '\n';
        }
    }
};

class House
{
private:
    Apartment* apartments;
    int apartmentCount;

public:
    House(int apartmentCount)
        : apartmentCount(apartmentCount)
    {
        if (this->apartmentCount < 0)
            this->apartmentCount = 0;

        if (this->apartmentCount == 0)
            apartments = nullptr;
        else
            apartments = new Apartment[this->apartmentCount];
    }

    House() : House(0)
    {
    }

    House(House& other)
        : apartmentCount(other.apartmentCount)
    {
        if (apartmentCount == 0)
        {
            apartments = nullptr;
        }
        else
        {
            apartments = new Apartment[apartmentCount];

            for (int i = 0; i < apartmentCount; i++)
                apartments[i].copyFrom(other.apartments[i]);
        }
    }

    ~House()
    {
        delete[] apartments;
    }

    void setApartment(int index, Apartment& apartment)
    {
        if (index < 0 || index >= apartmentCount)
            return;

        apartments[index].copyFrom(apartment);
    }

    int getApartmentCount()
    {
        return apartmentCount;
    }

    void print()
    {
        cout << "Apartments in house: " << apartmentCount << "\n\n";

        for (int i = 0; i < apartmentCount; i++)
        {
            cout << "Apartment " << i + 1 << ":\n";
            apartments[i].print();
        }
    }
};

class Array
{
private:
    int* data;
    int size;

public:
    Array(int size)
        : size(size)
    {
        if (this->size < 0)
            this->size = 0;

        if (this->size == 0)
            data = nullptr;
        else
            data = new int[this->size];

        for (int i = 0; i < this->size; i++)
            data[i] = 0;
    }

    Array() : Array(0)
    {
    }

    Array(int size, int value) : Array(size)
    {
        fill(value);
    }

    Array(Array& other)
        : size(other.size)
    {
        if (size == 0)
            data = nullptr;
        else
            data = new int[size];

        for (int i = 0; i < size; i++)
            data[i] = other.data[i];
    }

    ~Array()
    {
        delete[] data;
    }

    int getSize()
    {
        return size;
    }

    void fill(int value)
    {
        for (int i = 0; i < size; i++)
            data[i] = value;
    }

    void fillFromKeyboard()
    {
        for (int i = 0; i < size; i++)
        {
            cout << "Element [" << i << "]: ";
            cin >> data[i];
        }
    }

    void print()
    {
        for (int i = 0; i < size; i++)
            cout << data[i] << ' ';

        cout << '\n';
    }

    void resize(int newSize)
    {
        if (newSize < 0)
            newSize = 0;

        int* newData;

        if (newSize == 0)
            newData = nullptr;
        else
            newData = new int[newSize];

        int limit = size < newSize ? size : newSize;

        for (int i = 0; i < limit; i++)
            newData[i] = data[i];

        for (int i = limit; i < newSize; i++)
            newData[i] = 0;

        delete[] data;
        data = newData;
        size = newSize;
    }

    void setValue(int index, int value)
    {
        if (index < 0 || index >= size)
            return;

        data[index] = value;
    }

    void sort()
    {
        for (int i = 0; i < size - 1; i++)
        {
            for (int j = 0; j < size - i - 1; j++)
            {
                if (data[j] > data[j + 1])
                {
                    int temp = data[j];
                    data[j] = data[j + 1];
                    data[j + 1] = temp;
                }
            }
        }
    }

    int getMin()
    {
        if (size == 0)
            return 0;

        int minimum = data[0];

        for (int i = 1; i < size; i++)
        {
            if (data[i] < minimum)
                minimum = data[i];
        }

        return minimum;
    }

    int getMax()
    {
        if (size == 0)
            return 0;

        int maximum = data[0];

        for (int i = 1; i < size; i++)
        {
            if (data[i] > maximum)
                maximum = data[i];
        }

        return maximum;
    }
};

int main()
{
    cout << "TASK 2\n\n";

    Human human1(1234567890, "Horpynych", "Illia", "", 28, 9, 2010);
    Human human2(human1);

    human1.print();
    cout << "Human objects: " << Human::getCount() << "\n\n";

    cout << "TASK 3\n\n";

    String string1;
    String string2(150);
    String string3("C++ OOP homework 004");
    String string4(string3);

    cout << "Initialized string: ";
    string3.print();
    cout << "Copied string: ";
    string4.print();
    cout << "String objects: " << String::getCount() << "\n\n";

    cout << "Enter your string: ";
    string2.input();
    cout << "Your string: ";
    string2.print();
    cout << '\n';

    cout << "TASK 4\n\n";

    Apartment apartment1(2);
    apartment1.setHuman(0, 1111111111, "Horpynych", "Illia", "", 28, 9, 2010);
    apartment1.setHuman(1, 2222222222, "Petrenko", "Oleh", "Ivanovych", 15, 5, 1990);

    Apartment apartment2(1);
    apartment2.setHuman(0, 3333333333, "Shevchenko", "Anna", "Petrivna", 7, 12, 1995);

    House house1(2);
    house1.setApartment(0, apartment1);
    house1.setApartment(1, apartment2);

    House house2(house1);
    house2.print();

    cout << "TASK 5\n\n";

    Array array(5);
    array.setValue(0, 8);
    array.setValue(1, 3);
    array.setValue(2, 12);
    array.setValue(3, -4);
    array.setValue(4, 7);

    cout << "Array: ";
    array.print();

    cout << "Min: " << array.getMin() << '\n';
    cout << "Max: " << array.getMax() << '\n';

    array.sort();
    cout << "Sorted: ";
    array.print();

    array.resize(8);
    cout << "After resize: ";
    array.print();

    Array arrayCopy(array);
    cout << "Copy: ";
    arrayCopy.print();

    return 0;
}
