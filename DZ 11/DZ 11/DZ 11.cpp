#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

// завдання 1
int countDigits(int number) {
    if (number == 0) return 1;
    number = abs(number);
    int count = 0;
    while (number > 0) {
        count++;
        number /= 10;
    }
    return count;
}

// завдання 2
char encodeChar(char c, int shift) {
    shift = (shift % 26 + 26) % 26;
    if (c >= 'A' && c <= 'Z') return (c - 'A' + shift) % 26 + 'A';
    if (c >= 'a' && c <= 'z') return (c - 'a' + shift) % 26 + 'a';
    return c;
}

// завдання 3
float add(float a, float b) { return a + b; }
float subtract(float a, float b) { return a - b; }
float multiply(float a, float b) { return a * b; }
float divide(float a, float b) {
    if (b == 0) {
        cout << "Error: Division by zero!" << endl;
        return 0;
    }
    return a / b;
}

void printResult(float num1, float num2, char op, float result) {
    cout << fixed << setprecision(2);
    cout << num1 << " " << op << " " << num2 << " = " << result << endl;
}

int main() {
    int t;
    cout << "Enter integer T: ";
    cin >> t;
    cout << "Digits count: " << countDigits(t) << endl;

    cout << "Test encoding 'A' with 2: " << encodeChar('A', 2) << endl;
    cout << "Test encoding 'n' with -3: " << encodeChar('n', -3) << endl;

    float num1, num2;
    char op;
    cout << "Enter first number: ";
    cin >> num1;
    cout << "Enter operation (+, -, *, /): ";
    cin >> op;
    cout << "Enter second number: ";
    cin >> num2;

    float result = 0;
    bool validOperation = true;

    switch (op) {
    case '+': result = add(num1, num2); break;
    case '-': result = subtract(num1, num2); break;
    case '*': result = multiply(num1, num2); break;
    case '/':
        if (num2 == 0) {
            divide(num1, num2);
            validOperation = false;
        }
        else {
            result = divide(num1, num2);
        }
        break;
    default:
        cout << "Error: Invalid operation!" << endl;
        validOperation = false;
        break;
    }

    if (validOperation) {
        printResult(num1, num2, op, result);
    }

    return 0;
}