#include <iostream>
#include <vector>
#include <random>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int query_count = 1'000'000;
    const int max_user = 100000;
    const int max_page = 1000;

    mt19937 rng(123456);

    uniform_int_distribution<int> user_dist(1, max_user);
    uniform_int_distribution<int> coin_dist(1, 100);

    // New users start with a relatively large page to stress updates in naive solution.
    uniform_int_distribution<int> first_page_dist(400, 700);

    // Later reads increase by large jumps to cause frequent page changes.
    uniform_int_distribution<int> add_pages_dist(50, 250);

    vector<int> user_page(max_user + 1, 0);

    cout << query_count << "\n";

    for (int query_index = 0; query_index < query_count; ++query_index) {
        const int user = user_dist(rng);

        if (coin_dist(rng) <= 90) {  // 90% READ
            if (user_page[user] == 0) {
                user_page[user] = first_page_dist(rng);
            } else if (user_page[user] < max_page) {
                const int delta = add_pages_dist(rng);
                const int next_page = user_page[user] + delta;
                user_page[user] = (next_page > max_page ? max_page : next_page);
            }

            cout << "READ " << user << " " << user_page[user] << "\n";
        } else {
            cout << "CHEER " << user << "\n";
        }
    }

    return 0;
}
