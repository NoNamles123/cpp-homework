#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

int main() {
    srand(time(NULL));
    // завдання 1
    int v_a1, v_b2, v_c3;
    cout << "Enter A, B and C: ";
    cin >> v_a1 >> v_b2 >> v_c3;

    for (int j_idx = 1; j_idx <= 100000; j_idx++) {
        if (j_idx % v_a1 == 0 && j_idx % v_b2 == 0 && j_idx % v_c3 == 0) {
            cout << j_idx << " ";
        }
    }
    cout << endl << endl;

    // завдання 2
    int t_dim;
    cout << "Enter T: ";
    cin >> t_dim;

    for (int i_r = 0; i_r < t_dim; i_r++) {
        for (int j_c = 0; j_c < t_dim; j_c++) {
            int row_val = (i_r <= t_dim / 2) ? i_r : t_dim - 1 - i_r;
            int col_val = (j_c <= t_dim / 2) ? j_c : t_dim - 1 - j_c;
            int base_start = (t_dim == 9) ? 5 : 1;
            cout << base_start + row_val + col_val;
        }
        cout << endl;
    }
    cout << endl;

    // завдання 3
    int n_games;
    cout << "Enter rounds (N): ";
    cin >> n_games;

    int u_win_count = 0;
    int r_stats = 0, p_stats = 0, s_stats = 0;

    for (int step = 0; step < n_games; step++) {
        int cpu_rnd = 1 + rand() % 3;
        int user_p;
        cout << "Round " << step + 1 << " [1-Rock, 2-Paper, 3-Scissors]: ";
        cin >> user_p;

        if (user_p == 1) r_stats++;
        else if (user_p == 2) p_stats++;
        else if (user_p == 3) s_stats++;

        if (user_p == cpu_rnd) {
            cout << "Draw!" << endl;
        }
        else if ((user_p == 1 && cpu_rnd == 3) || (user_p == 2 && cpu_rnd == 1) || (user_p == 3 && cpu_rnd == 2)) {
            cout << "You win!" << endl;
            u_win_count++;
        }
        else {
            cout << "AI wins!" << endl;
        }
    }
    cout << "Total rounds: " << n_games << endl;
    cout << "Your moves -> Rock: " << r_stats << ", Paper: " << p_stats << ", Scissors: " << s_stats << endl;
    int most_frequent;
    if (r_stats >= p_stats && r_stats >= s_stats) {
        cout << "Your most frequent move was Rock. AI should have picked Paper more often." << endl;
    }
    else if (p_stats >= r_stats && p_stats >= s_stats) {
        cout << "Your most frequent move was Paper. AI should have picked Scissors more often." << endl;
    }
    else {
        cout << "Your most frequent move was Scissors. AI should have picked Rock more often." << endl;
    }

    cout << "User final victories: " << u_win_count << endl;
}