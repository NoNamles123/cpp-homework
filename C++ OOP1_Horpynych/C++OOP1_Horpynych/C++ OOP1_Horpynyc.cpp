/*
ПІБ: Horpynych Illia Volodymyrovych
Home Work - 001

Завдання 1
Вивчити теорію з навчальних матеріалів книжки 001
(посилання також в майстаті і в кінці презентації).

Завдання 2 (на 7-9 балів)
1. Створити - Прямокутник
a. додати приватні змінні-члени класу:
i. ширина
ii. вистота
b. додати публічний методи-члени класу
i. для розрахунку площі
2. Створити - Точка
a. додати публіні змінні-члени класу:
i. x
ii. y
iii. z
3. Створити - Машина, змінні і функції придумати самим.

Завдання 3 (на 10-12 балів)
Завдання написати програму розрахунку вартості газу за рік:
1. Створити клас Газ (Gas) - поля:
a. ім'я абонента,
b. кількість спожитого газу в місяці
c. інші поля при потребі.
2. Формула для розрахунку вартості газу:
a. 6 грн з 3-10 місяці
b. 8 грн в 1-2 та 11-12 місяці
3. Внести інформацію в клас по кожному місяцю за цілий рік з
клавіатури.
a. для цього в класі створити відповідні сетери і гетери
4. Порахувати суму оплати помісячно та за рік по абоненту Іван
Іванович та вивести в консоль
a. Суму по кожному місяцю в році
b. Загальну суму до оплати за рік
*/

#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

class Rectangle {
private:
    double m_width;
    double m_height;

public:
    void setWidth(double width) {
        m_width = width;
    }

    void setHeight(double height) {
        m_height = height;
    }

    double calculateArea() {
        return m_width * m_height;
    }
};

class Point {
public:
    double x;
    double y;
    double z;
};

class Car {
private:
    string m_brand;
    string m_model;
    int m_year;

public:
    void setBrand(string brand) {
        m_brand = brand;
    }

    void setModel(string model) {
        m_model = model;
    }

    void setYear(int year) {
        m_year = year;
    }

    string getBrand() {
        return m_brand;
    }

    string getModel() {
        return m_model;
    }

    int getYear() {
        return m_year;
    }

    void printInfo() {
        cout << "Brand: " << m_brand << endl;
        cout << "Model: " << m_model << endl;
        cout << "Year: " << m_year << endl;
    }
};

class Gas {
private:
    string m_name;
    double m_consumption[12];

public:
    void setName(string name) {
        m_name = name;
    }

    string getName() {
        return m_name;
    }

    void setConsumption(int month, double consumption) {
        m_consumption[month - 1] = consumption;
    }

    double getConsumption(int month) {
        return m_consumption[month - 1];
    }

    double getPrice(int month) {
        if (month == 1 || month == 2 || month == 11 || month == 12) {
            return 8;
        }

        return 6;
    }

    double calculateMonthPrice(int month) {
        return getConsumption(month) * getPrice(month);
    }

    double calculateYearPrice() {
        double total = 0;

        for (int month = 1; month <= 12; month++) {
            total += calculateMonthPrice(month);
        }

        return total;
    }
};

int main() {
    Rectangle rectangle;

    rectangle.setWidth(5);
    rectangle.setHeight(10);

    cout << "Rectangle area: "
        << rectangle.calculateArea()
        << endl;

    Point point;

    point.x = 10;
    point.y = 20;
    point.z = 30;

    cout << endl;
    cout << "Point:" << endl;
    cout << "X: " << point.x << endl;
    cout << "Y: " << point.y << endl;
    cout << "Z: " << point.z << endl;

    Car car;

    car.setBrand("BMW");
    car.setModel("M5");
    car.setYear(2020);

    cout << endl;
    cout << "Car:" << endl;
    car.printInfo();

    Gas gas;

    gas.setName("Ivan Ivanovych");

    cout << endl;
    cout << "Gas consumption for " << gas.getName() << endl;

    for (int month = 1; month <= 12; month++) {
        double consumption;

        cout << "Month " << month << ": ";
        cin >> consumption;

        gas.setConsumption(month, consumption);
    }

    cout << fixed << setprecision(2);

    cout << endl;
    cout << "Payment:" << endl;

    for (int month = 1; month <= 12; month++) {
        cout << "Month " << month
            << ": " << gas.calculateMonthPrice(month)
            << " UAH"
            << endl;
    }

    cout << endl;
    cout << "Total payment for year: "
        << gas.calculateYearPrice()
        << " UAH"
        << endl;

    return 0;
}