#define _USE_MATH_DEFINES
#include <iostream>
#include <Windows.h>
#include <iomanip>
#include <cmath>
#include <conio.h>
#include <ctime>

using namespace std;

int main() {
    srand(time(NULL));

	// завдання 1
    const int ROWS_1 = 5;
    const int COLS_1 = 5;
    int M[ROWS_1][COLS_1];

    for (auto& row : M) {
        for (auto& elem : row) {
            elem = 10 + rand() % 90;
        }
    }

    int maxVal = M[0][0];
    int maxRow = 0;
    int maxCol = 0;

    int r = 0;
    for (auto& row : M) {
        int c = 0;
        for (auto elem : row) {
            cout << setw(4) << elem;
            if (elem > maxVal) {
                maxVal = elem;
                maxRow = r;
                maxCol = c;
            }
            c++;
        }
        cout << endl;
        r++;
    }

    cout << "\nMax value: " << maxVal << endl;
    cout << "Index of max value: [" << maxRow << "][" << maxCol << "]" << endl;
    cout << endl;

	// завдання 2
    const int SIZE_2 = 7;
    int matrix[SIZE_2][SIZE_2];

    for (auto& row : matrix) {
        for (auto& elem : row) {
            elem = 10 + rand() % 90;
        }
    }

    cout << "Original matrix:" << endl;
    for (auto& row : matrix) {
        for (auto elem : row) {
            cout << setw(4) << elem;
        }
        cout << endl;
    }

    for (int i = 0; i < SIZE_2; ++i) {
        for (int j = i + 1; j < SIZE_2; ++j) {
            int temp = matrix[i][j];
            matrix[i][j] = matrix[j][i];
            matrix[j][i] = temp;
        }
    }

    cout << "\nMirrored matrix:" << endl;
    for (auto& row : matrix) {
        for (auto elem : row) {
            cout << setw(4) << elem;
        }
        cout << endl;
    }
    cout << endl;


	// завдання 3
    char board[3][3];
    for (auto& row : board) {
        for (auto& cell : row) {
            cell = ' ';
        }
    }

    bool gameOver = false;
    char winner = ' ';
    int moves = 0;

    while (!gameOver && moves < 9) {
        cout << "\n    Turn " << moves + 1 << endl;
        cout << "  0 1 2" << endl;
        int rIdx = 0;
        for (auto& row : board) {
            cout << rIdx++ << " ";
            for (auto cell : row) {
                cout << (cell == ' ' ? '.' : cell) << " ";
            }
            cout << endl;
        }

        if (moves % 2 == 0) {
            int input;
            int playerRow, playerCol;
            while (true) {
                cout << "\nYour turn. Enter coordinates (11, 02 ...): ";
                cin >> input;
                playerRow = input / 10;
                playerCol = input % 10;

                if (playerRow >= 0 && playerRow < 3 && playerCol >= 0 && playerCol < 3 && board[playerRow][playerCol] == ' ') {
                    board[playerRow][playerCol] = 'X';
                    break;
                }
                cout << "Invalid move. Try again." << endl;
            }
        }
        else {
            cout << "\nComputer's turn " << endl;
            while (true) {
                int compRow = rand() % 3;
                int compCol = rand() % 3;
                if (board[compRow][compCol] == ' ') {
                    board[compRow][compCol] = 'O';
                    break;
                }
            }
        }

        moves++;

        for (int i = 0; i < 3; ++i) {
            if (board[i][0] != ' ' && board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
                winner = board[i][0];
                gameOver = true;
            }
            if (board[0][i] != ' ' && board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
                winner = board[0][i];
                gameOver = true;
            }
        }
        if (board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
            winner = board[0][0];
            gameOver = true;
        }
        if (board[0][2] != ' ' && board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
            winner = board[0][2];
            gameOver = true;
        }

        if (!gameOver && moves == 9) {
            gameOver = true;
            winner = 'D';
        }
    }

    cout << "\nFinal Board:" << endl;
    cout << "  0 1 2" << endl;
    int rIdx = 0;
    for (auto& row : board) {
        cout << rIdx++ << " ";
        for (auto cell : row) {
            cout << (cell == ' ' ? '.' : cell) << " ";
        }
        cout << endl;
    }

    if (winner == 'D') {
        cout << "It's a draw!" << endl;
    }

     else if (winner == 'X') {
     cout << "Winner: YOU!" << endl;
    }

    else  {
     cout << "Winner: COMPUTER!" << endl;
    }

}