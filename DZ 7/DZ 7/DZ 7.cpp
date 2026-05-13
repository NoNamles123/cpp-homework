#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

int main() {
    //  завдання 1
    int n;
    cout << "Enter starting number N: ";
    cin >> n;

    if (n % 2 != 0) {
        n++;
    }

    int task1_arr[25];
    for (int i = 0; i < 25; i++) {
        task1_arr[i] = n;
        n += 2;
    }

    cout << "Resulting array (column):" << endl;
    for (int i = 0; i < 25; i++) {
        cout << task1_arr[i] << endl;
    }

    cout  << endl;


	// завдання 2
    srand(time(0));
    int task2_arr[100];

    cout << "Generated Array (100 elements):" << endl;
    for (int i = 0; i < 100; i++) {
        task2_arr[i] = rand() % 201 - 100;
        cout << task2_arr[i] << " ";
        if ((i + 1) % 10 == 0) cout << endl;
    }

    int x;
    cout << "\nEnter number X to search: ";
    cin >> x;

    int count = 0;
    for (int i = 0; i < 100; i++) {
        if (task2_arr[i] == x) {
            count++;
        }
    }

    if (count > 0) {
        cout << "Number " << x << " found! Occurrences: " << count << endl;
    }
    else {
        cout << "Number " << x << " was not found." << endl;
    }

    cout  << endl;


	// завдання 3
    long long fibanachi[20];
    fibanachi[0] = 0;
    fibanachi[1] = 1;

    for (int i = 2; i < 20; i++) {
        fibanachi[i] = fibanachi[i - 1] + fibanachi[i - 2];
    }

    cout << "First 20 Fibonacci numbers:" << endl;
    for (int i = 0; i < 20; i++) {
        cout << fibanachi[i] << " ";
    }
    cout << endl;

    return 0;
}