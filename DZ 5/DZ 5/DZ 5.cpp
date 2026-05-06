#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;


int main() {
    srand(time(NULL));
    // завдання 1
    int alpha_99, beta_v2;
    cout << "Enter A: ";
    cin >> alpha_99;
    cout << "Enter B: ";
    cin >> beta_v2;

    int omega_sum = 0;
    int k_33 = alpha_99;

    while (k_33 <= beta_v2) {
        if (k_33 % 3 == 0) {
            omega_sum += k_33;
        }
        k_33++;
    }
    cout << "Suma = " << omega_sum << endl << endl;

    // завдання 2
    long delta_xr = 100 + rand() % (1000000 - 100 + 1);
    cout << "Generated number: " << delta_xr << endl;

    int pixel_count = 0;
    long zeta_tmp = delta_xr;

    do {
        int m_val = zeta_tmp % 10;
        if (m_val > 3 && m_val < 8) {
            pixel_count++;
        }
        zeta_tmp /= 10;
    } while (zeta_tmp > 0);

    if (pixel_count > 0) {
        cout << "Result = " << pixel_count << endl;
    }
    else {
        cout << "No digits between 3 and 8 found." << endl;
    }
    cout << endl;

    // завдання 3
    cout << "Enter Roman number: ";

    char p_char;
    int gamma_res = 0;
    int last_q = 0;

    while (cin.get(p_char) && p_char != '\n') {
        int current_val = 0;

        if (p_char == 'X' || p_char == 'x') current_val = 10;
        else if (p_char == 'V' || p_char == 'v') current_val = 5;
        else if (p_char == 'I' || p_char == 'i') current_val = 1;

        if (current_val > 0) {
            if (last_q < current_val && last_q != 0) {
                gamma_res += (current_val - 2 * last_q);
            }
            else {
                gamma_res += current_val;
            }
            last_q = current_val;
        }
    }

    cout << "Arabic number: " << gamma_res << endl;

    return 0;
}