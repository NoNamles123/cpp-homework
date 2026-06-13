#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
    srand(time(NULL));
    //завдання 1
    const int SIZE = 30;
    int arrayA[SIZE];
    int arrayB[SIZE];
    int arrayC[SIZE * 2];
    int sizeC = 0;
    for (int i = 0; i < SIZE; i++) {
        *(arrayA + i) = rand() % 90 + 10;
        *(arrayB + i) = rand() % 90 + 10;
    }
    for (int i = 0; i < SIZE; i++) {
        int current = *(arrayA + i);
        bool inB = false;
        bool inC = false;

        for (int j = 0; j < SIZE; j++) {
            if (*(arrayB + j) == current) {
                inB = true;
                break;
            }
        }

        for (int j = 0; j < sizeC; j++) {
            if (*(arrayC + j) == current) {
                inC = true;
                break;
            }
        }

        if (!inB && !inC) {
            *(arrayC + sizeC) = current;
            sizeC++;
        }
    }
    for (int i = 0; i < SIZE; i++) {
        int current = *(arrayB + i);
        bool inA = false;
        bool inC = false;

        for (int j = 0; j < SIZE; j++) {
            if (*(arrayA + j) == current) {
                inA = true;
                break;
            }
        }

        for (int j = 0; j < sizeC; j++) {
            if (*(arrayC + j) == current) {
                inC = true;
                break;
            }
        }

        if (!inA && !inC) {
            *(arrayC + sizeC) = current;
            sizeC++;
        }
    }
    cout << "array A: ";
    for (int i = 0; i < SIZE; i++) cout << *(arrayA + i) << " ";

    cout << "\n\narray B: ";
    for (int i = 0; i < SIZE; i++) cout << *(arrayB + i) << " ";

    cout << "\n\narray C: ";
    for (int i = 0; i < sizeC; i++) cout << *(arrayC + i) << " ";
    cout << endl;
	//завдання 2
    const int SIZE2 = 50;
    int A[SIZE2];
    int K;
    int foundIndex = -1;

    cout << "Enter number K: ";
    cin >> K;

    cout << "\nArray A:\n";
    for (int i = 0; i < SIZE2; i++) {
        *(A + i) = rand() % 90 + 10;
        cout << *(A + i) << " ";
    }
    cout << "\n\n";

    for (int i = 0; i < SIZE2; i++) {
        if (*(A + i) > K) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex != -1) {
        cout << "First number greater than " << K << " has index: " << foundIndex
            << " (this is the number " << *(A + foundIndex) << ")" << endl;
    }
    else {
        cout << "There are no numbers greater than " << K << " in the array. Result: " << foundIndex << endl;
    }

    return 0;
}