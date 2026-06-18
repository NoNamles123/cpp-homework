#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int findMax(int** matrix, int N, int M) {
    int maxVal = matrix[0][0];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (matrix[i][j] > maxVal) {
                maxVal = matrix[i][j];
            }
        }
    }
    return maxVal;
}

int findMin(int** matrix, int N, int M) {
    int minVal = matrix[0][0];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (matrix[i][j] < minVal) {
                minVal = matrix[i][j];
            }
        }
    }
    return minVal;
}

void insertRow(int**& matrix, int& N, int M, int position) {
    if (position < 0 || position > N) {
        cout << "Invalid position!\n";
        return;
    }
    int** temp = new int* [N + 1];
    for (int i = 0; i < position; i++) {
        temp[i] = matrix[i];
    }
    temp[position] = new int[M];
    for (int j = 0; j < M; j++) {
        temp[position][j] = rand() % 90 + 10;
    }
    for (int i = position; i < N; i++) {
        temp[i + 1] = matrix[i];
    }
    delete[] matrix;
    matrix = temp;
    N++;
}

int main() {
    srand(time(NULL));

    // завдання 1
    int N1, M1;
    cout << "Enter N and M for matrix 1: ";
    cin >> N1 >> M1;

    int** matrix1 = new int* [N1];
    for (int i = 0; i < N1; i++) {
        matrix1[i] = new int[M1];
        for (int j = 0; j < M1; j++) {
            int val = rand() % 90 + 10;
            if (rand() % 2 == 0) {
                val = -val;
            }
            matrix1[i][j] = val;
        }
    }

    cout << "Matrix 1:\n";
    for (int i = 0; i < N1; i++) {
        for (int j = 0; j < M1; j++) {
            cout << matrix1[i][j] << "\t";
        }
        cout << "\n";
    }

    cout << "Maximum value: " << findMax(matrix1, N1, M1) << "\n";
    cout << "Minimum value: " << findMin(matrix1, N1, M1) << "\n";

    for (int i = 0; i < N1; i++) {
        delete[] matrix1[i];
    }
    delete[] matrix1;

    // завдання 2
    int N2, M2;
    cout << "\nEnter N and M for matrix 2: ";
    cin >> N2 >> M2;

    int** matrix2 = new int* [N2];
    for (int i = 0; i < N2; i++) {
        matrix2[i] = new int[M2];
        for (int j = 0; j < M2; j++) {
            matrix2[i][j] = rand() % 90 + 10;
        }
    }

    cout << "Matrix 2 before insertion:\n";
    for (int i = 0; i < N2; i++) {
        for (int j = 0; j < M2; j++) {
            cout << matrix2[i][j] << "\t";
        }
        cout << "\n";
    }

    int pos;
    cout << "Enter position to insert new row (0 to " << N2 << "): ";
    cin >> pos;

    insertRow(matrix2, N2, M2, pos);

    cout << "Matrix 2 after insertion:\n";
    for (int i = 0; i < N2; i++) {
        for (int j = 0; j < M2; j++) {
            cout << matrix2[i][j] << "\t";
        }
        cout << "\n";
    }

    for (int i = 0; i < N2; i++) {
        delete[] matrix2[i];
    }
    delete[] matrix2;

    // завдання 3
    int M3, N3, K3;
    cout << "\nEnter M, N, K for 3D matrix: ";
    cin >> M3 >> N3 >> K3;

    int*** matrix3 = new int** [M3];
    for (int i = 0; i < M3; i++) {
        matrix3[i] = new int* [N3];
        for (int j = 0; j < N3; j++) {
            matrix3[i][j] = new int[K3];
            for (int k = 0; k < K3; k++) {
                matrix3[i][j][k] = rand() % 90 + 10;
            }
        }
    }

    cout << "3D Matrix layers:\n";
    for (int i = 0; i < M3; i++) {
        cout << "Layer " << i << ":\n";
        for (int j = 0; j < N3; j++) {
            for (int k = 0; k < K3; k++) {
                cout << matrix3[i][j][k] << "\t";
            }
            cout << "\n";
        }
    }

    int idxM, idxN, idxK, newVal;
    while (true) {
        cout << "Enter 3 indices (m n k) to modify element: ";
        cin >> idxM >> idxN >> idxK;
        if (idxM >= 0 && idxM < M3 && idxN >= 0 && idxN < N3 && idxK >= 0 && idxK < K3) {
            break;
        }
        cout << "Index out of bounds! Please try again.\n";
    }

    cout << "Enter new value: ";
    cin >> newVal;

    matrix3[idxM][idxN][idxK] = newVal;

    cout << "Updated Layer " << idxM << ":\n";
    for (int j = 0; j < N3; j++) {
        for (int k = 0; k < K3; k++) {
            cout << matrix3[idxM][j][k] << "\t";
        }
        cout << "\n";
    }

    for (int i = 0; i < M3; i++) {
        for (int j = 0; j < N3; j++) {
            delete[] matrix3[i][j];
        }
        delete[] matrix3[i];
    }
    delete[] matrix3;

    return 0;
}