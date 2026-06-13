#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

void sortThree(int& A, int& B, int& C) {
    if (A > B) {
        int temp = A;
        A = B;
        B = temp;
    }
    if (B > C) {
        int temp = B;
        B = C;
        C = temp;
    }
    if (A > B) {
        int temp = A;
        A = B;
        B = temp;
    }
}

void analyzeArray(const int arr[], int size, int& min_val, int& max_val) {
    min_val = arr[0];
    max_val = arr[0];

    for (int i = 1; i < size; i++) {
        if (arr[i] < min_val) {
            min_val = arr[i];
        }
        if (arr[i] > max_val) {
            max_val = arr[i];
        }
    }
}

void checkRows(int M[7][7], int T) {
    int greaterThanT = 0;
    int lessThanT = 0;
    int equalToT = 0;

    for (int i = 0; i < 7; i++) {
        int rowSum = 0;
        for (int j = 0; j < 7; j++) {
            rowSum += M[i][j];
        }

        if (rowSum > T) {
            greaterThanT++;
        }
        else if (rowSum < T) {
            lessThanT++;
        }
        else {
            equalToT++;
        }
    }

    cout << "\nAnalysis results:\n";
    cout << "Rows with sum > T: " << greaterThanT << endl;
    cout << "Rows with sum < T: " << lessThanT << endl;
    cout << "Rows with sum == T: " << equalToT << endl;
}

int main() {
    srand(time(NULL));

    // завдання 1
    int numA, numB, numC;

    cout << "Enter A: ";
    cin >> numA;
    cout << "Enter B: ";
    cin >> numB;
    cout << "Enter C: ";
    cin >> numC;

    sortThree(numA, numB, numC);

    cout << "\nResult:" << endl;
    cout << "A (min) = " << numA << endl;
    cout << "B (mid) = " << numB << endl;
    cout << "C (max) = " << numC << endl;
    cout << endl;

    // завдання 2
    const int SIZE = 50;
    int array[SIZE];
    int min_array = 0;
    int max_array = 0;

    cout << "Generated Array:\n";
    for (int i = 0; i < SIZE; i++) {
        array[i] = rand() % 90 + 10;
        cout << array[i] << " ";
    }
    cout << "\n\n";

    analyzeArray(array, SIZE, min_array, max_array);

    cout << "Result:\n";
    cout << "min_array = " << min_array << endl;
    cout << "max_array = " << max_array << endl;
    cout << endl;

    // завдання 3
    int M[7][7];
    int T;

    cout << "Matrix M[7][7]:\n";
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            M[i][j] = rand() % 201 - 100;
            cout << M[i][j] << "\t";
        }
        cout << endl;
    }

    cout << "\nEnter value T: ";
    cin >> T;

    checkRows(M, T);

    return 0;
}