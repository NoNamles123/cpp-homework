/*
Horpynych Illia
Home Work - 006

Завдання 2 (на 7-9 балів)
Розробити клас Worker.
Необхідно зберігати дані:
• ПІБ
• посада
• рік вступу на роботу
• зарплата
Створити масив об’єктів.
Вивести список працівників:
• стаж роботи яких на цьому підприємстві перевершує задане
число років;
• зарплата яких перевищує задану;
• які займають задану посаду.
Використовуйте explicit-конструктор і константні
функції-члени (наприклад, для відображення даних про працівника
і т.д.).

Завдання 3 (на 10-12 балів)
Створіть клас «Дріб» (або використовуйте вже раніше
створений вами).
Використовуючи перевантаження операторів
реалізуйте для нього арифметичні операції для роботи з
дробом (операції +, -, *, /, =, ==, !=, >, <, >=, <=, !, ++ та --
(пркфіксну та постфіксну)).
Приклад як реалізовувати в mian()
Drib d1(15, 13);
Drib d2(58, 33);
Drib d3, d4;
d4 = d3 = d1 + d2;
d3.print(); // виводити дріб наприклад d1 як 15/13
d4.print();
d3 = d1 - d2;
d3.print();
d3 = d1 * d2;
d3.print();
d3 = d1 / d2;
d3.print();
d4 = Drib(0 , 0);
d3 = d1 / d4; //передбачити ділення на 0
d3.print();
і так всі операції, логічні та інші
if (d1 > d2) d1.print(“Більше”, d2);
else d2.print(“Більще”, d1);
…// Додати перегрузку прінту з параметрами щоб виводити
одаткову стрічку результат на екрані наприклад
“15/13 Більше 8/13”
if (d1 == d2) d1.print(“Рівне”, d2);
else d2.print(“не рівне”, d1);
// “15/13 не рівне 8/13”
d4 = ++d1;
d4.print();
d4 = d1--;
d4.print();
…
і так всі операції, логічні та інші…

Завдання 3 (додаткове на кристалики)
Створити клас Flat (квартира).
Реалізувати перевантажені оператори:
1. Перевірка на рівність площ квартир (операція «==»).
2. Операцію присвоювання одного об’єкта іншому
(операція «=»).
3. Порівняння двох квартир за ціною (операція «>»)
*/

#include <iostream>
#include <string>
using namespace std;

class Worker {
    string fullName;
    string position;
    int startYear;
    double salary;

public:
    Worker(string fullName, string position, int startYear, double salary)
        : fullName{ fullName }, position{ position }, startYear{ startYear }, salary{ salary } {
    }

    explicit Worker(string fullName)
        : Worker(fullName, "Unknown", 2026, 0) {
    }

    Worker() : Worker("Unknown") {
    }

    string getFullName() const { return fullName; }
    string getPosition() const { return position; }
    int getStartYear() const { return startYear; }
    double getSalary() const { return salary; }
    int getExperience(int currentYear) const { return currentYear - startYear; }

    void setFullName(string value) { fullName = value; }
    void setPosition(string value) { position = value; }
    void setStartYear(int value) { startYear = value; }
    void setSalary(double value) { salary = value; }

    void print() const {
        cout << "Name: " << fullName
            << ", Position: " << position
            << ", Start year: " << startYear
            << ", Salary: " << salary << endl;
    }
};

class Drib {
    long long numerator;
    long long denominator;

    void simplify() {
        if (denominator == 0) {
            return;
        }
        if (denominator < 0) {
            numerator = -numerator;
            denominator = -denominator;
        }
        long long a = numerator < 0 ? -numerator : numerator;
        long long b = denominator;
        while (b != 0) {
            long long rest = a % b;
            a = b;
            b = rest;
        }
        if (a != 0) {
            numerator /= a;
            denominator /= a;
        }
    }

    void printValue() const {
        if (denominator == 0) {
            cout << "Undefined";
        }
        else {
            cout << numerator << "/" << denominator;
        }
    }

public:
    Drib(long long numerator, long long denominator)
        : numerator{ numerator }, denominator{ denominator } {
        simplify();
    }

    Drib() : Drib(0, 1) {
    }

    Drib(const Drib& other)
        : numerator{ other.numerator }, denominator{ other.denominator } {
    }

    Drib& operator=(const Drib& other) {
        if (this != &other) {
            numerator = other.numerator;
            denominator = other.denominator;
        }
        return *this;
    }

    Drib operator+(const Drib& other) const {
        if (denominator == 0 || other.denominator == 0) return Drib(0, 0);
        return Drib(numerator * other.denominator + other.numerator * denominator,
            denominator * other.denominator);
    }

    Drib operator-(const Drib& other) const {
        if (denominator == 0 || other.denominator == 0) return Drib(0, 0);
        return Drib(numerator * other.denominator - other.numerator * denominator,
            denominator * other.denominator);
    }

    Drib operator*(const Drib& other) const {
        if (denominator == 0 || other.denominator == 0) return Drib(0, 0);
        return Drib(numerator * other.numerator, denominator * other.denominator);
    }

    Drib operator/(const Drib& other) const {
        if (denominator == 0 || other.denominator == 0 || other.numerator == 0) {
            cout << "Division by zero!" << endl;
            return Drib(0, 0);
        }
        return Drib(numerator * other.denominator, denominator * other.numerator);
    }

    bool operator==(const Drib& other) const {
        if (denominator == 0 || other.denominator == 0) return false;
        return numerator * other.denominator == other.numerator * denominator;
    }

    bool operator!=(const Drib& other) const {
        return !(*this == other);
    }

    bool operator>(const Drib& other) const {
        if (denominator == 0 || other.denominator == 0) return false;
        return numerator * other.denominator > other.numerator * denominator;
    }

    bool operator<(const Drib& other) const {
        if (denominator == 0 || other.denominator == 0) return false;
        return numerator * other.denominator < other.numerator * denominator;
    }

    bool operator>=(const Drib& other) const {
        if (denominator == 0 || other.denominator == 0) return false;
        return *this > other || *this == other;
    }

    bool operator<=(const Drib& other) const {
        if (denominator == 0 || other.denominator == 0) return false;
        return *this < other || *this == other;
    }

    bool operator!() const {
        return denominator != 0 && numerator == 0;
    }

    Drib& operator++() {
        if (denominator != 0) numerator += denominator;
        return *this;
    }

    Drib operator++(int) {
        Drib copy(*this);
        ++(*this);
        return copy;
    }

    Drib& operator--() {
        if (denominator != 0) numerator -= denominator;
        return *this;
    }

    Drib operator--(int) {
        Drib copy(*this);
        --(*this);
        return copy;
    }

    void print() const {
        printValue();
        cout << endl;
    }

    void print(const string& message, const Drib& other) const {
        printValue();
        cout << " " << message << " ";
        other.printValue();
        cout << endl;
    }
};

class Flat {
    string address;
    double area;
    double price;

public:
    Flat(string address, double area, double price)
        : address{ address }, area{ area }, price{ price } {
    }

    Flat() : Flat("Unknown", 0, 0) {
    }

    bool operator==(const Flat& other) const {
        return area == other.area;
    }

    Flat& operator=(const Flat& other) {
        if (this != &other) {
            address = other.address;
            area = other.area;
            price = other.price;
        }
        return *this;
    }

    bool operator>(const Flat& other) const {
        return price > other.price;
    }

    void print() const {
        cout << "Address: " << address
            << ", Area: " << area
            << ", Price: " << price << endl;
    }
};

int main() {
    cout << boolalpha;

    cout << "TASK 2 - WORKER\n\n";
    const int currentYear = 2026;
    const int minExperience = 5;
    const double minSalary = 30000;
    const string neededPosition = "Programmer";

    Worker workers[] = {
        Worker("Illia Horpynych", "Programmer", 2025, 35000),
        Worker("Oleh Petrenko", "Manager", 2017, 48000),
        Worker("Anna Shevchenko", "Programmer", 2019, 55000),
        Worker("Ivan Kovalenko", "Designer", 2022, 24000)
    };
    const int workersCount = sizeof(workers) / sizeof(workers[0]);

    cout << "All workers:\n";
    for (int i = 0; i < workersCount; i++) workers[i].print();

    cout << "\nExperience greater than " << minExperience << " years:\n";
    for (int i = 0; i < workersCount; i++) {
        if (workers[i].getExperience(currentYear) > minExperience) workers[i].print();
    }

    cout << "\nSalary greater than " << minSalary << ":\n";
    for (int i = 0; i < workersCount; i++) {
        if (workers[i].getSalary() > minSalary) workers[i].print();
    }

    cout << "\nPosition: " << neededPosition << ":\n";
    for (int i = 0; i < workersCount; i++) {
        if (workers[i].getPosition() == neededPosition) workers[i].print();
    }

    cout << "\nTASK 3 - DRIB\n\n";
    Drib d1(15, 13);
    Drib d2(58, 33);
    Drib d3, d4;

    cout << "d1: "; d1.print();
    cout << "d2: "; d2.print();

    d4 = d3 = d1 + d2;
    cout << "d3 = d1 + d2: "; d3.print();
    cout << "d4 = d3: "; d4.print();

    d3 = d1 - d2;
    cout << "d1 - d2: "; d3.print();

    d3 = d1 * d2;
    cout << "d1 * d2: "; d3.print();

    d3 = d1 / d2;
    cout << "d1 / d2: "; d3.print();

    d4 = Drib(0, 0);
    cout << "d1 / invalid fraction: ";
    d3 = d1 / d4;
    d3.print();

    cout << "d1 / 0/1: ";
    d3 = d1 / Drib(0, 1);
    d3.print();

    cout << "\nComparison operators:\n";
    cout << "d1 == d2: " << (d1 == d2) << endl;
    cout << "d1 != d2: " << (d1 != d2) << endl;
    cout << "d1 > d2: " << (d1 > d2) << endl;
    cout << "d1 < d2: " << (d1 < d2) << endl;
    cout << "d1 >= d2: " << (d1 >= d2) << endl;
    cout << "d1 <= d2: " << (d1 <= d2) << endl;
    cout << "!d1: " << !d1 << endl;
    cout << "!Drib(0, 1): " << !Drib(0, 1) << endl;

    if (d1 > d2) d1.print("Greater than", d2);
    else d2.print("Greater than", d1);

    if (d1 == d2) d1.print("Equal to", d2);
    else d1.print("Not equal to", d2);

    cout << "\nIncrement and decrement:\n";
    cout << "d1 before: "; d1.print();
    d4 = ++d1;
    cout << "d4 = ++d1: "; d4.print();
    cout << "d1 now: "; d1.print();

    d4 = d1++;
    cout << "d4 = d1++: "; d4.print();
    cout << "d1 now: "; d1.print();

    d4 = --d1;
    cout << "d4 = --d1: "; d4.print();
    cout << "d1 now: "; d1.print();

    d4 = d1--;
    cout << "d4 = d1--: "; d4.print();
    cout << "d1 now: "; d1.print();

    cout << "\nEXTRA TASK - FLAT\n\n";
    Flat flat1("Ternopil, Apartment 1", 65, 65000);
    Flat flat2("Ternopil, Apartment 2", 65, 73000);
    Flat flat3;

    cout << "Flat 1: "; flat1.print();
    cout << "Flat 2: "; flat2.print();
    cout << "Same area (flat1 == flat2): " << (flat1 == flat2) << endl;
    cout << "Flat 1 costs more (flat1 > flat2): " << (flat1 > flat2) << endl;
    cout << "Flat 2 costs more (flat2 > flat1): " << (flat2 > flat1) << endl;

    flat3 = flat1;
    cout << "Flat 3 after flat3 = flat1: "; flat3.print();

    return 0;
}
