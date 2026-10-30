/*
Horpynych Illia
Home Work - 007

Завдання 2
1. Створити клас Date, який буде містити інформацію
про дату (день, місяць, рік). За допомогою механізму
перевантаження операторів, визначте операцію різниці двох
дат (результат у вигляді кількості днів між датами), а також
операцію збільшення дати на певну кількість днів.
2. Створити клас Time, в якому реалізовані операції
складання, віднімання, порівняння, введення та виведення
на екран.
Можливість конвертації часу з американського формату am
(pm): 10:00 pm = 22:00, 12:00 pm = 00:00.
(Можна зробити 2 конструктора з різними параметрами години і хв
та години хвилина та pm/am і відповідні сетера за рахунок
перегрузки оператора “()” дані тримати в форматі HH:MM - 22:00
m_hour, m_min, а для сетерів в американському форматі
використовувати функцію для конвертації)
Також зробити все наступні методи:
● Додавати/віднімати хвилини через ++i, i++, --i, i–, +, - .
● Перегрузити методи - (), new, delete (пункт на
кристалик книга 004 (UA) книга 003 (RU))
● Привести до типу int хвилин (hours*60 + min).
(пункт на кристалик)
(книга 004 (UA) сторінка 111
(книга 003 (RU) Class::operator type (void); урок 3,
сторінка 17)
3. Зробити класи Date та Time дружніми.
● Додати конструктор та сетер, щоб можна було
передати вказівники клас Date в класі Time.
● Доробити функції в класі часу +, - (можна через
приватний метод Add(Time &time) Minus(Time
&time)) та при потребі використати один з методів
в іншому щоб не дублювати код
○ додати загальний валідатор в одній функції,
якщо щось виходить за межі змінити
відповідні члени класу.
Наприклад: якщо реаліщовуванти не через
дні і ха відповідно, а додавати/віднімати
відповідні члени класу Date - day, month, year,
і дата більше реальної, наприкткад 35, тоді
віднімаємо 31(макс для місяця) та
збільшуємо місяць на 1…
○ основне що треба зробити - якщо година
більше 24 год в класі Date додати день (чи
кілька днів) і зменшити час навпаки, якщо
година менше 0 при відніманні - відняти
потрібну кількість днів
■ !!! але перевірити якщо вказівник на
клас Дата NULL то відповідно нічого не
робити з датою !!!
● ++, -- (в ++ і -- додаються/віднімаються тільки
хвилини та дні у відповідних класах)
*/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <stdexcept>
#include <cstddef>
#include <new>
#include <cctype>
using namespace std;

class Time;

class Date {
    int day;
    int month;
    int year;

    friend class Time;

    static bool isLeapYear(int y) {
        return y % 400 == 0 || (y % 4 == 0 && y % 100 != 0);
    }

    static int daysInMonth(int m, int y) {
        if (m == 2) return isLeapYear(y) ? 29 : 28;
        if (m == 4 || m == 6 || m == 9 || m == 11) return 30;
        return 31;
    }

    static long long daysBeforeYear(int y) {
        long long previous = y - 1;
        return previous * 365 + previous / 4 - previous / 100 + previous / 400;
    }

    long long serialDays() const {
        long long result = daysBeforeYear(year);
        for (int m = 1; m < month; ++m) {
            result += daysInMonth(m, year);
        }
        return result + day - 1;
    }

    void moveDays(long long amount) {
        long long serial = serialDays() + amount;
        long long maximum = daysBeforeYear(10000) - 1;

        if (serial < 0 || serial > maximum) {
            throw out_of_range("Date is outside years 1..9999");
        }

        int low = 1;
        int high = 9999;
        while (low < high) {
            int middle = low + (high - low + 1) / 2;
            if (daysBeforeYear(middle) <= serial) low = middle;
            else high = middle - 1;
        }
        year = low;
        int daysLeft = static_cast<int>(serial - daysBeforeYear(year));
        month = 1;
        while (daysLeft >= daysInMonth(month, year)) {
            daysLeft -= daysInMonth(month, year);
            ++month;
        }
        day = daysLeft + 1;
    }

public:
    Date(int d, int m, int y) : day(d), month(m), year(y) {
        if (y < 1 || y > 9999 || m < 1 || m > 12 ||
            d < 1 || d > daysInMonth(m, y)) {
            throw invalid_argument("Invalid date");
        }
    }

    Date() : Date(1, 1, 2000) {}
    Date(const Date& other) = default;

    void setDate(int d, int m, int y) {
        Date validated(d, m, y);
        day = validated.day;
        month = validated.month;
        year = validated.year;
    }

    int getDay() const { return day; }
    int getMonth() const { return month; }
    int getYear() const { return year; }

    long long operator-(const Date& other) const {
        return serialDays() - other.serialDays();
    }

    Date operator+(int days) const {
        Date result(*this);
        result.moveDays(days);
        return result;
    }

    Date operator-(int days) const {
        Date result(*this);
        result.moveDays(-static_cast<long long>(days));
        return result;
    }

    Date& operator+=(int days) {
        moveDays(days);
        return *this;
    }

    Date& operator-=(int days) {
        moveDays(-static_cast<long long>(days));
        return *this;
    }

    Date& operator++() {
        moveDays(1);
        return *this;
    }

    Date operator++(int) {
        Date old(*this);
        ++(*this);
        return old;
    }

    Date& operator--() {
        moveDays(-1);
        return *this;
    }

    Date operator--(int) {
        Date old(*this);
        --(*this);
        return old;
    }

    void print() const {
        cout << setfill('0') << setw(2) << day << '.' << setw(2) << month
            << '.' << setw(4) << year << setfill(' ');
    }

    friend ostream& operator<<(ostream& out, const Date& date) {
        out << setfill('0') << setw(2) << date.day << '.'
            << setw(2) << date.month << '.' << setw(4) << date.year
            << setfill(' ');
        return out;
    }
};

class Time {
    int hour;
    int minute;
    Date* date;

    friend class Date;

    static int fromUS(int h, int m, const string& period) {
        string amPm = period;
        for (char& symbol : amPm) {
            symbol = static_cast<char>(tolower(static_cast<unsigned char>(symbol)));
        }
        if (h < 1 || h > 12 || m < 0 || m > 59 ||
            (amPm != "am" && amPm != "pm")) {
            throw invalid_argument("Invalid AM/PM time");
        }
        return (h % 12) + (amPm == "pm" ? 12 : 0);
    }

    int totalMinutes() const {
        return hour * 60 + minute;
    }

    void normalize(long long minutes) {
        long long changeDays = minutes / 1440;
        long long rest = minutes % 1440;
        if (rest < 0) {
            rest += 1440;
            --changeDays;
        }

        if (date != nullptr && changeDays != 0) {
            date->moveDays(changeDays);
        }
        hour = static_cast<int>(rest / 60);
        minute = static_cast<int>(rest % 60);
    }

    void addMinutes(long long amount) {
        normalize(static_cast<long long>(totalMinutes()) + amount);
    }

public:
    Time(int h, int m, Date* datePointer = nullptr)
        : hour(0), minute(0), date(datePointer) {
        if (h < 0 || h > 23 || m < 0 || m > 59) {
            throw invalid_argument("Invalid 24-hour time");
        }
        hour = h;
        minute = m;
    }

    Time() : Time(0, 0) {}

    explicit Time(Date* datePointer) : Time(0, 0, datePointer) {}

    Time(int h, int m, const string& period, Date* datePointer = nullptr)
        : Time(fromUS(h, m, period), m, datePointer) {
    }

    void setTime(int h, int m) {
        if (h < 0 || h > 23 || m < 0 || m > 59) {
            throw invalid_argument("Invalid 24-hour time");
        }
        hour = h;
        minute = m;
    }

    void setTime(int h, int m, const string& period) {
        setTime(fromUS(h, m, period), m);
    }

    void setDate(Date* newDate) { date = newDate; }
    Date* getDate() const { return date; }
    int getHour() const { return hour; }
    int getMinute() const { return minute; }

    void operator()(int h, int m) { setTime(h, m); }
    void operator()(int h, int m, const string& period) { setTime(h, m, period); }

    Time operator+(const Time& other) const {
        Time result(*this);
        result.addMinutes(other.totalMinutes());
        return result;
    }

    Time operator-(const Time& other) const {
        Time result(*this);
        result.addMinutes(-static_cast<long long>(other.totalMinutes()));
        return result;
    }

    Time operator+(int minutes) const {
        Time result(*this);
        result.addMinutes(minutes);
        return result;
    }

    Time operator-(int minutes) const {
        Time result(*this);
        result.addMinutes(-static_cast<long long>(minutes));
        return result;
    }

    Time& operator+=(int minutes) {
        addMinutes(minutes);
        return *this;
    }

    Time& operator-=(int minutes) {
        addMinutes(-static_cast<long long>(minutes));
        return *this;
    }

    Time& operator++() {
        addMinutes(1);
        return *this;
    }

    Time operator++(int) {
        Time old(*this);
        ++(*this);
        return old;
    }

    Time& operator--() {
        addMinutes(-1);
        return *this;
    }

    Time operator--(int) {
        Time old(*this);
        --(*this);
        return old;
    }

    bool operator==(const Time& other) const { return totalMinutes() == other.totalMinutes(); }
    bool operator!=(const Time& other) const { return !(*this == other); }
    bool operator<(const Time& other) const { return totalMinutes() < other.totalMinutes(); }
    bool operator>(const Time& other) const { return other < *this; }
    bool operator<=(const Time& other) const { return !(*this > other); }
    bool operator>=(const Time& other) const { return !(*this < other); }

    explicit operator int() const { return totalMinutes(); }

    void print() const { cout << *this; }

    static void* operator new(size_t size) {
        return ::operator new(size);
    }

    static void operator delete(void* pointer) noexcept {
        ::operator delete(pointer);
    }

    friend ostream& operator<<(ostream& out, const Time& value) {
        out << setfill('0') << setw(2) << value.hour << ':'
            << setw(2) << value.minute << setfill(' ');
        return out;
    }

    friend istream& operator>>(istream& in, Time& value) {
        int hours = 0;
        int minutes = 0;
        char separator = '\0';
        if (!(in >> hours >> separator >> minutes)) return in;
        if (separator != ':' || hours < 0 || hours > 23 ||
            minutes < 0 || minutes > 59) {
            in.setstate(ios::failbit);
            return in;
        }
        value.setTime(hours, minutes);
        return in;
    }
};

int main() {
    try {
        cout << "TASK 2.1 - DATE\n\n";
        Date first(28, 9, 2010);
        Date second(30, 10, 2026);
        cout << "First date: " << first << '\n';
        cout << "Second date: " << second << '\n';
        cout << "Days between dates: " << second - first << '\n';
        cout << "First date + 10 days: " << first + 10 << '\n';

        Date leap(28, 2, 2024);
        cout << "Leap year: " << leap << " -> ";
        ++leap;
        cout << leap << " -> ";
        leap++;
        cout << leap << " -> ";
        --leap;
        cout << leap << "\n\n";

        cout << "TASK 2.2 - TIME\n\n";
        Time american(10, 0, "pm");
        cout << "10:00 pm = " << american << '\n';
        cout << "12:00 am = " << Time(12, 0, "am") << '\n';
        cout << "12:00 pm = " << Time(12, 0, "pm") << '\n';

        Time a(14, 40);
        Time b(2, 30);
        cout << "a = " << a << ", b = " << b << '\n';
        cout << "a + b = " << a + b << '\n';
        cout << "a - b = " << a - b << '\n';
        cout << "a > b: " << (a > b) << '\n';
        cout << "a as minutes: " << static_cast<int>(a) << '\n';
        cout << "a + 80 minutes = " << a + 80 << '\n';
        cout << "a - 100 minutes = " << a - 100 << '\n';

        cout << "Before ++a: " << a << '\n';
        cout << "After ++a: " << ++a << '\n';
        cout << "Postfix a++ returns: " << a++ << ", now a: " << a << '\n';
        cout << "After --a: " << --a << '\n';
        cout << "Postfix a-- returns: " << a-- << ", now a: " << a << '\n';

        a(9, 15);
        cout << "After a(9, 15): " << a << '\n';
        a(11, 45, "pm");
        cout << "After a(11, 45, pm): " << a << '\n';

        istringstream sampleInput("07:05");
        Time entered;
        sampleInput >> entered;
        cout << "Read from stream 07:05 -> " << entered << '\n';

        cout << "\nTASK 2.3 - FRIEND DATE + TIME\n\n";
        Date attachedDate(31, 12, 2025);
        Time clock(23, 59, &attachedDate);
        cout << "Before midnight: " << attachedDate << ' ' << clock << '\n';
        ++clock;
        cout << "After ++clock:  " << attachedDate << ' ' << clock << '\n';
        clock -= 2;
        cout << "After -2 min:   " << attachedDate << ' ' << clock << '\n';
        Time result = clock + Time(2, 15);
        cout << "Clock + 02:15: " << attachedDate << ' ' << result << '\n';

        Time noDate(23, 50);
        noDate += 30;
        cout << "Without Date*: " << noDate << " (only clock wraps)\n";

        Time* dynamicTime = new Time(8, 30);
        cout << "new Time: " << *dynamicTime << '\n';
        delete dynamicTime;
        cout << "delete Time: OK\n";
    }
    catch (const exception& error) {
        cerr << "Error: " << error.what() << '\n';
        return 1;
    }
    return 0;
}