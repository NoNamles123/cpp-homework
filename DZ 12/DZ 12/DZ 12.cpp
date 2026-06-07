#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

// завдання 1
void printRange(int a, int b) {
    cout << a << " ";
    if (a == b) {
        return;
    }
    if (a < b) {
        printRange(a + 1, b);
    }
    else {
        printRange(a - 1, b);
    }
}

// завдання 2
int getMin(int a, int b) {
    if (a < b) {
        return a;
    }
    else {
        return b;
    }
}

int getMin(int arr[], int size) {
    if (size == 1) {
        return arr[0];
    }
    return getMin(arr[size - 1], getMin(arr, size - 1));
}

int main() {
    srand(time(0));

    // завдання 1
    int a, b;
    cout << "Enter A and B: ";
    cin >> a >> b;
    cout << "Sequence: ";
    printRange(a, b);
    cout << endl << endl;

    // завдання 2
    const int SIZE = 20;
    int arr[SIZE];
    cout << "Array elements: ";
    for (int i = 0; i < SIZE; i++) {
        arr[i] = rand() % 90 + 10;
        cout << arr[i] << " ";
    }
    cout << endl;
    cout << "Minimum element: " << getMin(arr, SIZE) << endl << endl;

    // завдання 3
    int n;
    cout << "Enter N (10 to 50): ";
    cin >> n;

    if (n >= 10 && n <= 50) {
        cout << "Sequence: ";
        int currentNumber = 1;
        int count = 0;

        for (int i = 0; i < n; i++) {
            cout << currentNumber << " ";
            count++;
            if (count == currentNumber) {
                currentNumber++;
                count = 0;
            }
        }
        cout << endl;
    }
    else {
        cout << "Error: N is out of range!" << endl;
    }

    return 0;
}