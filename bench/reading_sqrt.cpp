#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <bits/stdc++.h>

#ifdef ENABLE_BENCHMARK
#include "log_duration.h"
#endif

using namespace std;

// Sqrt decomposition for counts by page:
//  - page_count_[page] = readers on that page
//  - block_sum_[b] = sum of page_count_ in block b
class ReadingManagerSqrt {
private:
    static constexpr int kMaxUser = 100000;
    static constexpr int kMaxPage = 1000;

public:
    ReadingManagerSqrt()
        : user_page_(kMaxUser + 1, 0)
        , page_count_(kMaxPage + 1, 0)
        , total_readers_(0)
        , block_size_(static_cast<int>(sqrt(kMaxPage)) + 1)
        , block_sum_((kMaxPage + block_size_) / block_size_, 0) {}

    void Read(int user, int page) {
        const int prev_page = user_page_[user];

        if (prev_page == 0) {
            ++total_readers_;
        } else {
            RemoveFromPage(prev_page);
        }

        user_page_[user] = page;
        AddToPage(page);
    }

    double Cheer(int user) const {
        const int page = user_page_[user];

        if (page == 0) {
            return 0.0;
        }
        if (total_readers_ == 1) {
            return 1.0;
        }

        const int readers_with_less_pages = CountReadersWithLessPages(page);
        return static_cast<double>(readers_with_less_pages) / (total_readers_ - 1);
    }

private:
    void AddToPage(int page) {
        ++page_count_[page];
        ++block_sum_[page / block_size_];
    }

    void RemoveFromPage(int page) {
        --page_count_[page];
        --block_sum_[page / block_size_];
    }

    int CountReadersWithLessPages(int page) const {
        const int right = page - 1;
        if (right <= 0) {
            return 0;
        }

        int result = 0;

        const int full_blocks = right / block_size_;
        for (int b = 0; b < full_blocks; ++b) {
            result += block_sum_[b];
        }

        const int start = full_blocks * block_size_;
        for (int i = max(1, start); i <= right; ++i) {
            result += page_count_[i];
        }

        return result;
    }

private:
    vector<int> user_page_;
    vector<int> page_count_;
    int total_readers_;

    int block_size_;
    vector<int> block_sum_;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ReadingManagerSqrt manager;

    int query_count = 0;
    cin >> query_count;

    cout << setprecision(6);

#ifdef ENABLE_BENCHMARK
    using Clock = std::chrono::steady_clock;
    long long input_ns = 0;
    long long algo_ns = 0;
    LogDuration total("Sqrt TOTAL");
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
