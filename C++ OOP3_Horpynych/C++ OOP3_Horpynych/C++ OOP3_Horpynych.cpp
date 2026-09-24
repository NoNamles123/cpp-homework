/*
PIB: Illia Horpynych
Home Work - 003

Домашнє завдання

Завдання 1
Вивчити теорію з навчальних матеріалів книжки 002.

Завдання 2 (на 7-9 балів)
До вже реалізованого класу Point додати механізм ініціалізації членів класу.
Реалізувати 3 конструктори: за замовчуванням; з параметрами x, y; з параметрами x, y, z.
Реалізувати метод print().
Створити клас Line з двома членами типу Point - p1 і p2.
Реалізувати 4 конструктори: за замовчуванням; p1, p2; x1, y1, x2, y2; x1, y1, z1, x2, y2, z2.
Реалізувати метод print().

Завдання 3 (на 10-12 балів)
Розробити клас String.
Конструктор за замовчуванням створює рядок довжиною 80 символів.
Конструктор довільного розміру.
Конструктор, який ініціалізує рядок рядком користувача.
Деструктор. Делегування конструкторів, якщо це можливо.
Методи введення і виведення рядка.
Статична функція-член для отримання кількості створених об'єктів.

Завдання 4 (додаткове на кристалики)
Створити додаток Phone Book. Зберігати ПІБ, домашній, робочий, мобільний телефони та додаткову інформацію.
ПІБ зберігати в динамічно виділеній пам'яті. Реалізувати додавання, видалення, пошук, показ усіх, збереження та завантаження з файлу.

Завдання 5 (додаткове на 3 кристалики)
Створити клас GameSettings за патерном Singleton. Зберігати гучність, складність, роздільну здатність та мову.
*/

#include <iostream>
#include "Point.h"
#include "Line.h"
#include "String.h"
#include "PhoneBook.h"
#include "GameSettings.h"

using namespace std;

int main()
{
    cout << "===== TASK 2 =====" << endl << endl;

    Point point1;
    Point point2{ 10, 20 };
    Point point3{ 10, 20, 30 };

    cout << "Point 1: "; point1.print();
    cout << "Point 2: "; point2.print();
    cout << "Point 3: "; point3.print();

    cout << endl << "===== LINE =====" << endl << endl;

    Line line1;
    Line line2{ point2, point3 };
    Line line3{ 1, 2, 10, 20 };
    Line line4{ 1, 2, 3, 10, 20, 30 };

    cout << "Line 1:" << endl; line1.print(); cout << endl;
    cout << "Line 2:" << endl; line2.print(); cout << endl;
    cout << "Line 3:" << endl; line3.print(); cout << endl;
    cout << "Line 4:" << endl; line4.print();

    cout << endl << "===== TASK 3 =====" << endl << endl;

    String string1;
    String string2{ 30 };
    String string3{ "Hello OOP!" };

    cout << "String 3: ";
    string3.print();
    cout << "Created String objects: " << String::getCount() << endl;

    cout << endl;
    string2.input();
    cout << "String 2: ";
    string2.print();

    cout << endl << "===== TASK 5 =====" << endl << endl;

    GameSettings& settings = GameSettings::getInstance();
    settings.setVolume(75);
    settings.setDifficulty("Hard");
    settings.setResolution(1920, 1080);
    settings.setLanguage("Ukrainian");
    settings.print();

    cout << endl << "===== PHONE BOOK =====" << endl;
    PhoneBook phoneBook;
    phoneBook.menu();

    return 0;
}
