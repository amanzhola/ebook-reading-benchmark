#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int Q = 1'000'000;
    const int MAX_USER = 100000;
    const int MAX_PAGE = 1000;

    mt19937 rng(123456);

    uniform_int_distribution<int> user_dist(1, MAX_USER);
    uniform_int_distribution<int> coin(1, 100);

    // Новому пользователю сразу даём крупную страницу,
    // чтобы "наив" делал много операций (1..page)
    uniform_int_distribution<int> first_page_dist(400, 700);

    // Дальше — большие прыжки, чтобы часто обновлять много страниц
    uniform_int_distribution<int> add_dist(50, 250);

    vector<int> page(MAX_USER + 1, 0);

    cout << Q << "\n";

    for (int i = 0; i < Q; ++i) {
        int user = user_dist(rng);

        if (coin(rng) <= 90) {  // 90% READ -> больше нагрузки на обновления
            if (page[user] == 0) {
                page[user] = first_page_dist(rng);
            } else if (page[user] < MAX_PAGE) {
                int add = add_dist(rng);
                int np = page[user] + add;
                page[user] = (np > MAX_PAGE ? MAX_PAGE : np);
            }
            cout << "READ " << user << " " << page[user] << "\n";
        } else {
            cout << "CHEER " << user << "\n";
        }
    }
}
