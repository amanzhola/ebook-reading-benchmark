#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <chrono>
#include <bits/stdc++.h>

#ifdef ENABLE_BENCHMARK
#include "log_duration.h"
#endif

using namespace std;

// Naive solution based on the course hint:
//  - at_least_[m] = how many readers have read at least page m
class ReadingManagerNaive {
private:
    static constexpr int kMaxUser = 100000;
    static constexpr int kMaxPage = 1000;

public:
    ReadingManagerNaive()
        : user_page_(kMaxUser + 1, 0)
        , at_least_(kMaxPage + 1, 0)
        , total_readers_(0) {}

    void Read(int user, int page) {
        const int prev_page = user_page_[user];

        if (prev_page == 0) {
            ++total_readers_;
        } else {
            for (int m = 1; m <= prev_page; ++m) {
                --at_least_[m];
            }
        }

        user_page_[user] = page;

        for (int m = 1; m <= page; ++m) {
            ++at_least_[m];
        }
    }

    double Cheer(int user) const {
        const int page = user_page_[user];

        if (page == 0) {
            return 0.0;
        }
        if (total_readers_ == 1) {
            return 1.0;
        }

        const int readers_with_less_pages = total_readers_ - at_least_[page];
        return static_cast<double>(readers_with_less_pages) / (total_readers_ - 1);
    }

private:
    vector<int> user_page_;
    vector<int> at_least_;
    int total_readers_;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ReadingManagerNaive manager;

    int query_count = 0;
    cin >> query_count;

    cout << setprecision(6);

#ifdef ENABLE_BENCHMARK
    using Clock = std::chrono::steady_clock;
    long long input_ns = 0;
    long long algo_ns = 0;
    LogDuration total("Naive TOTAL");
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
