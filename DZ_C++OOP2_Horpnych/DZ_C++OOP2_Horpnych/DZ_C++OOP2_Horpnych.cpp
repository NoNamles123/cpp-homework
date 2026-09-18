/*
	Full NS: ILLIA HORPNYCH
    Home Work - 02

    Homework text:
    Домашнє завдання
Завдання 1
Вивчити теорію з навчальних матеріалів книжки 001
(посилання також в майстаті і в кінці презентації).
Завдання 2 (на 7-9 балів)
Реалізуйте клас «Точка» - Point. Необхідно зберігати
координати x, y, z в приватних змінних-членах класу.
Реалізуйте функції-члени класу для введення даних,
виведення даних, реалізуйте аксесор для доступу до змінних-членів.
Створіть 2 об'єкта класу «Точка». В одному об'єкті задайте
значення.
Виведіть значення на екран.
Створіть конструктор без параметрів та присвойте 0 для x, y, z
Виконайте програму та виведіть значення на екран.
Модифікуйте конструктор, так щоб він приймав параметри x, y,
z та задавав їх змінним членам класу.
Виконайте програму та виведіть значення на екран.
Завдання 3 (на 10-12 балів)
Завдання доробити програму для розрахунку газу на базі
попередньо домашнього завдання:
Додати вдосконалення згідно вивченого матеріалу:
1. Винести реалізацію великих функцій за межі класу.
Приклад: void Gas::PrintForMonth(int monthNum)
можна все зробити в одному файлі там де розміщений main або
винести в окремі файли gas.h i gas.cpp
2. Створити мінімум 2 конструктори (дефолтний/пустий та для
задання імені абоненту)
3. Член класу ім'я абонента оголосити в класі як вказівник на
масив чарів та ініціалізувати методом new з відповідним
розміром імені, що переданий в функці сетер параметром
(setName).
a. створити метод для очищення пам'яті для імені, якщо
вона була виділена, та занулення вказівника. Даний
метод викликати в деструкторі.
4. В файлі main.cpp оголосити 2 об'єкти/змінні даного класу з
двома видами конструкторів.
a. задати дані через сетер використаного газу для даних
абонентів
i. для першого абонента
1. використати пустий конструктор
2. задати ім'я через сетер - “Petro”
3. дані по використаному газу помісячно задати в
циклі. Дані записати в масив.
ii. для другого абонента запросити ввести ім'я в ручну
з клавіатури перед оголошенням класу і передати в
конструктор параметром
1. дані по використаному газу також запросити
ввести вручну в циклі
b. вивести для двох абонентів наступну інформацію:
i. ім'я абонента
ii. розрахунки по оплаті помісячно (номер місяця,
використано газу, сума за місяць )
iii. сума загалом за рік
5. Додаткова задачаа на кристалик - створити
динамічний масив абонентів
a. для початку запитати кількість абонентів
для створити масиву об'єктів.
b. внести дані для всіх абонентів
c. зробити розрахунок для всіх абонентів та вивести
результати
*/

#include <iostream>
#include <cstring>
#include <iomanip>

using namespace std;

class Point {
private:
    int m_x;
    int m_y;
    int m_z;

public:
    Point();
    Point(int x, int y, int z);

    void input();
    void print();

    void setX(int x);
    void setY(int y);
    void setZ(int z);

    int getX();
    int getY();
    int getZ();
};

Point::Point() {
    m_x = 0;
    m_y = 0;
    m_z = 0;
}

Point::Point(int x, int y, int z) {
    m_x = x;
    m_y = y;
    m_z = z;
}

void Point::input() {
    cout << "Enter x: ";
    cin >> m_x;

    cout << "Enter y: ";
    cin >> m_y;

    cout << "Enter z: ";
    cin >> m_z;
}

void Point::print() {
    cout << "x = " << m_x
        << ", y = " << m_y
        << ", z = " << m_z
        << endl;
}

void Point::setX(int x) {
    m_x = x;
}

void Point::setY(int y) {
    m_y = y;
}

void Point::setZ(int z) {
    m_z = z;
}

int Point::getX() {
    return m_x;
}

int Point::getY() {
    return m_y;
}

int Point::getZ() {
    return m_z;
}

class Gas {
private:
    char* m_name;
    double m_gas[12];

public:
    Gas();
    Gas(const char* name);
    ~Gas();

    void setName(const char* name);
    const char* getName();

    void clearName();

    void setGas(int month, double gas);
    double getGas(int month);

    double getMonthSum(int month);
    double getYearSum();

    void printForMonth(int month);
    void printAll();
};

Gas::Gas() {
    m_name = nullptr;

    for (int i = 0; i < 12; i++) {
        m_gas[i] = 0;
    }
}

Gas::Gas(const char* name) {
    m_name = nullptr;

    for (int i = 0; i < 12; i++) {
        m_gas[i] = 0;
    }

    setName(name);
}

Gas::~Gas() {
    clearName();
}

void Gas::clearName() {
    if (m_name != nullptr) {
        delete[] m_name;
        m_name = nullptr;
    }
}

void Gas::setName(const char* name) {
    clearName();

    int size = strlen(name);

    m_name = new char[size + 1];

    for (int i = 0; i <= size; i++) {
        m_name[i] = name[i];
    }
}

const char* Gas::getName() {
    return m_name;
}

void Gas::setGas(int month, double gas) {
    if (month >= 0 && month < 12) {
        m_gas[month] = gas;
    }
}

double Gas::getGas(int month) {
    if (month >= 0 && month < 12) {
        return m_gas[month];
    }

    return 0;
}

double Gas::getMonthSum(int month) {
    double price;

    if (month == 0 || month == 1 || month == 10 || month == 11) {
        price = 8;
    }
    else {
        price = 6;
    }

    return m_gas[month] * price;
}

double Gas::getYearSum() {
    double sum = 0;

    for (int i = 0; i < 12; i++) {
        sum += getMonthSum(i);
    }

    return sum;
}

void Gas::printForMonth(int month) {
    cout << "Month: " << month + 1 << endl;

    cout << "Gas used: "
        << m_gas[month]
        << endl;

    cout << "Sum: "
        << fixed << setprecision(2)
        << getMonthSum(month)
        << " UAH"
        << endl;
}

void Gas::printAll() {
    cout << "\nSubscriber: " << m_name << endl;
    cout << endl << " \n";

    for (int i = 0; i < 12; i++) {
        cout << "Month " << i + 1<< " | Gas: " << m_gas[i] << " | Sum: " << getMonthSum(i)<< " UAH"<< endl;
    }

    cout << endl << " \n";

    cout << "Total for year: "
        << fixed << setprecision(2)
        << getYearSum()
        << " UAH"
        << endl;
}

int main() {
    cout << "           POINT           \n\n";

    Point point1;

    cout << "Point 1:\n";
    point1.print();

    point1.setX(10);
    point1.setY(20);
    point1.setZ(30);

    cout << "\nPoint 1 after setters:\n";
    point1.print();

    Point point2(100, 200, 300);

    cout << "\nPoint 2:\n";
    point2.print();

    cout << "\n\n        GAS        \n";

    Gas firstSubscriber;

    firstSubscriber.setName("Petro");

    double petroGas[12] = {
        100, 110, 90, 80,
        70, 60, 50, 40,
        60, 80, 100, 120
    };

    for (int i = 0; i < 12; i++) {
        firstSubscriber.setGas(i, petroGas[i]);
    }

    char secondName[100];

    cout << "\nEnter second subscriber name: ";
    cin >> secondName;

    Gas secondSubscriber(secondName);

    for (int i = 0; i < 12; i++) {
        double gas;

        cout << "Enter gas for month "
            << i + 1
            << ": ";

        cin >> gas;

        secondSubscriber.setGas(i, gas);
    }

    cout << "\n\n FIRST SUBSCRIBER \n";
    firstSubscriber.printAll();

    cout << "\n\n SECOND SUBSCRIBER \n";
    secondSubscriber.printAll();

    return 0;
}
