#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
    srand(time(NULL));
    // завдання 1
    const int SIZE = 50;
    int originalArray[SIZE];
    int negativeArray[SIZE];
    int positiveArray[SIZE];
    int negCount = 0;
    int posCount = 0;

    cout << "Original array:\n";
    for (int i = 0; i < SIZE; i++) {
        originalArray[i] = rand() % 201 - 100;
        cout << originalArray[i] << " ";
        if (originalArray[i] < 0) {
            negativeArray[negCount++] = originalArray[i];
        }
        else {
            positiveArray[posCount++] = originalArray[i];
        }
    }
    cout << endl << endl;
    for (int i = 0; i < negCount - 1; i++) {
        for (int j = 0; j < negCount - i - 1; j++) {
            if (negativeArray[j] < negativeArray[j + 1]) {
                int temp = negativeArray[j];
                negativeArray[j] = negativeArray[j + 1];
                negativeArray[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < posCount - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < posCount; j++) {
            if (positiveArray[j] < positiveArray[minIdx]) {
                minIdx = j;
            }
        }
        int temp = positiveArray[i];
        positiveArray[i] = positiveArray[minIdx];
        positiveArray[minIdx] = temp;
    }

    cout << "Negative array (descending):\n";
    for (int i = 0; i < negCount; i++) {
        cout << negativeArray[i] << " ";
    }
    cout << endl << endl;

    cout << "Positive array (ascending):\n";
    for (int i = 0; i < posCount; i++) {
        cout << positiveArray[i] << " ";
    }
    cout << endl << endl;

    // завдання 2

    const int N = 7;
    int M[N][N];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            M[i][j] = rand() % 90 + 10;
        }
    }

    cout << "Matrix BEFORE sorting:\n";
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << M[i][j] << "\t";
        }
        cout << endl;
    }
    cout << endl;

    for (int j = 0; j < N; j++) {
        int index = 0;
        while (index < N) {
            if (index == 0) {
                index++;
            }
            if (M[index][j] <= M[index - 1][j]) {
                index++;
            }
            else {
                int temp = M[index][j];
                M[index][j] = M[index - 1][j];
                M[index - 1][j] = temp;
                index--;
            }
        }
    }

    cout << "Matrix AFTER sorting columns (Gnome Sort):\n";
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << M[i][j] << "\t";
        }
        cout << endl;
    }
    cout << endl;

    // завдання 3
    const int N3 = 7;
    int M3[N3][N3];
    int result[N3][N3];

    for (int i = 0; i < N3; i++) {
        for (int j = 0; j < N3; j++) {
            M3[i][j] = rand() % 90 + 10;
        }
    }

    cout << "Initial Matrix:\n";
    for (int i = 0; i < N3; i++) {
        for (int j = 0; j < N3; j++) {
            cout << M3[i][j] << "\t";
        }
        cout << "\n";
    }
    cout << "\n";

    int colIndices[N3];
    for (int i = 0; i < N3; i++) {
        colIndices[i] = i;
    }

    for (int i = 0; i < N3 - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < N3; j++) {
            if (M3[i][colIndices[j]] < M3[i][colIndices[minIdx]]) {
                minIdx = j;
            }
        }
        int temp = colIndices[i];
        colIndices[i] = colIndices[minIdx];
        colIndices[minIdx] = temp;
    }
    bool possible = true;
    for (int i = 0; i < N3 - 1; i++) {
        if (M3[i][colIndices[i]] > M3[i + 1][colIndices[i + 1]]) {
            possible = false;
            break;
        }
    }

    if (possible) {
        for (int j = 0; j < N3; j++) {
            int sourceCol = colIndices[j];
            for (int i = 0; i < N3; i++) {
                result[i][j] = M3[i][sourceCol];
            }
        }

        cout << "Sorted Matrix (Main diagonal is ascending):\n";
        for (int i = 0; i < N3; i++) {
            for (int j = 0; j < N3; j++) {
                if (i == j) {
                    cout << result[i][j] << "*\t";
                }
                else {
                    cout << result[i][j] << "\t";
                }
            }
            cout << "\n";
        }
    }
    else {
        cout << "It is impossible to rearrange columns to sort the main diagonal.\n";
    }

}