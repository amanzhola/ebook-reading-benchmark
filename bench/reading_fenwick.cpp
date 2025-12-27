#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <chrono>
#include <bits/stdc++.h>

using namespace std;

#ifdef ENABLE_BENCHMARK
#include "log_duration.h"
#endif

// Fenwick tree (Binary Indexed Tree):
// stores count[1..n] and supports:
//  - Update(index, delta)
//  - PrefixSum(index)
class FenwickTree {
public:
    explicit FenwickTree(int size)
        : size_(size)
        , tree_(size + 1, 0) {}

    void Update(int index, int delta) {
        for (; index <= size_; index += index & -index) {
            tree_[index] += delta;
        }
    }

    int PrefixSum(int index) const {
        int result = 0;
        for (; index > 0; index -= index & -index) {
            result += tree_[index];
        }
        return result;
    }

private:
    int size_;
    vector<int> tree_;
};

class ReadingManagerFenwick {
private:
    static constexpr int kMaxUser = 100000;
    static constexpr int kMaxPage = 1000;

public:
    ReadingManagerFenwick()
        : user_page_(kMaxUser + 1, 0)
        , page_readers_(kMaxPage)
        , total_readers_(0) {}

    void Read(int user, int page) {
        const int prev_page = user_page_[user];

        if (prev_page == 0) {
            ++total_readers_;
        } else {
            page_readers_.Update(prev_page, -1);
        }

        user_page_[user] = page;
        page_readers_.Update(page, +1);
    }

    double Cheer(int user) const {
        const int page = user_page_[user];

        if (page == 0) {
            return 0.0;
        }
        if (total_readers_ == 1) {
            return 1.0;
        }

        const int readers_with_less_pages = page_readers_.PrefixSum(page - 1);
        return static_cast<double>(readers_with_less_pages) / (total_readers_ - 1);
    }

private:
    vector<int> user_page_;
    FenwickTree page_readers_;
    int total_readers_;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ReadingManagerFenwick manager;

    int query_count = 0;
    cin >> query_count;

    cout << setprecision(6);

#ifdef ENABLE_BENCHMARK
    using Clock = std::chrono::steady_clock;
    long long input_ns = 0;
    long long algo_ns = 0;
    LogDuration total("Fenwick TOTAL");
#endif

    for (int query_index = 0; query_index < query_count; ++query_index) {
#ifdef ENABLE_BENCHMARK
        const auto t_in_1 = Clock::now();
#endif

        string command;
        int user = 0;
        cin >> command >> user;

        int page = 0;
        if (command == "READ") {
            cin >> page;
        }

#ifdef ENABLE_BENCHMARK
        const auto t_in_2 = Clock::now();
        input_ns += std::chrono::duration_cast<std::chrono::nanoseconds>(t_in_2 - t_in_1).count();

        const auto t_alg_1 = Clock::now();
#endif

        if (command == "READ") {
            manager.Read(user, page);
        } else if (command == "CHEER") {
            cout << manager.Cheer(user) << "\n";
        }

#ifdef ENABLE_BENCHMARK
        const auto t_alg_2 = Clock::now();
        algo_ns += std::chrono::duration_cast<std::chrono::nanoseconds>(t_alg_2 - t_alg_1).count();
#endif
    }

#ifdef ENABLE_BENCHMARK
    cerr << "Input: " << (input_ns / 1'000'000) << " ms\n";
    cerr << "Algo:  " << (algo_ns / 1'000'000) << " ms\n";
#endif

    return 0;
}
