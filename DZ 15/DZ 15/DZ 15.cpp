#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

void copyArrays(int* A, int sizeA, int*& B, int& sizeB) {
    if (sizeB < sizeA) {
        cout << "Array A contains more elements than array B\n";
    }
    else {
        for (int i = 0; i < sizeA; i++) {
            B[i] = A[i];
        }
        int* temp = new int[sizeA];
        for (int i = 0; i < sizeA; i++) {
            temp[i] = B[i];
        }
        delete[] B;
        B = temp;
        sizeB = sizeA;

        cout << "Copied array B: ";
        for (int i = 0; i < sizeB; i++) {
            cout << B[i] << " ";
        }
        cout << "\n";
    }
}

void addElement(int*& arr, int& size, int value) {
    int* temp = new int[size + 1];
    for (int i = 0; i < size; i++) {
        temp[i] = arr[i];
    }
    temp[size] = value;
    delete[] arr;
    arr = temp;
    size++;
}

void bubbleSort(int* arr, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main() {
    srand(time(NULL));

    // завдання 1
    cout << "Enter the size of array A: ";
    int sizeA;
    cin >> sizeA;
    int* A = new int[sizeA];
    for (int i = 0; i < sizeA; i++) {
        A[i] = rand() % 90 + 10;
    }

    int sizeB = rand() % 100 + 1;
    int* B = new int[sizeB];
    for (int i = 0; i < sizeB; i++) {
        B[i] = 0;
    }

    copyArrays(A, sizeA, B, sizeB);

    delete[] A;
    delete[] B;
    cout << endl;

    // завдання 2
    cout << "\nEnter numbers for array (enter a negative number to stop):\n";
    int* arr2 = nullptr;
    int size2 = 0;
    int val;

    while (true) {
        cin >> val;
        if (val < 0) {
            break;
        }
        addElement(arr2, size2, val);
    }

    cout << "Your array: ";
    for (int i = 0; i < size2; i++) {
        cout << arr2[i] << " ";
    }
    cout << endl;
    delete[] arr2;

    // завдання 3
    int size3;
    do {
        cout << "\nEnter the size of array for sorting (min 30): ";
        cin >> size3;
    } while (size3 < 30);

    int* arr3 = new int[size3];
    cout << "Array before sorting: ";
    for (int i = 0; i < size3; i++) {
        arr3[i] = rand() % 100;
        cout << arr3[i] << " ";
    }
    cout << endl;

    bubbleSort(arr3, size3);

    cout << "Array after sorting: ";
    for (int i = 0; i < size3; i++) {
        cout << arr3[i] << " ";
    }
    cout << endl;
    delete[] arr3;

    return 0;
}