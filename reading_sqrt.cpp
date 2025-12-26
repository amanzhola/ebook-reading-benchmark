#include <bits/stdc++.h>
#include "log_duration.h"
using namespace std;

class ReadingSqrt {
public:
    static const int MAX_USER = 100000;
    static const int MAX_PAGE = 1000;

    ReadingSqrt()
        : user_page_(MAX_USER + 1, 0),
          cnt_(MAX_PAGE + 1, 0),
          readers_(0) {
        block_ = sqrt(MAX_PAGE) + 1;
        blocks_.assign((MAX_PAGE + block_) / block_, 0);
    }

    void Read(int user, int page) {
        int old = user_page_[user];
        if (old == 0) {
            ++readers_;
        } else {
            --cnt_[old];
            --blocks_[old / block_];
        }
        user_page_[user] = page;
        ++cnt_[page];
        ++blocks_[page / block_];
    }

    double Cheer(int user) const {
        int p = user_page_[user];
        if (p == 0) return 0.0;
        if (readers_ == 1) return 1.0;

        int less = 0;
        for (int b = 0; b < p / block_; ++b)
            less += blocks_[b];
        for (int i = (p / block_) * block_; i < p; ++i)
            less += cnt_[i];

        return double(less) / (readers_ - 1);
    }

private:
    vector<int> user_page_, cnt_, blocks_;
    int readers_, block_;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ReadingSqrt rm;
    int Q;
    cin >> Q;

    using Clock = std::chrono::steady_clock;
    long long read_ns = 0;
    long long work_ns = 0;

    double sink = 0;

    {
        LogDuration total("Sqrt TOTAL");

        for (int qi = 0; qi < Q; ++qi) {
            // ---------- замер ВВОДА ----------
            auto t1 = Clock::now();

            string cmd;
            int user;
            cin >> cmd >> user;

            int page = 0;
            if (cmd == "READ") {
                cin >> page;
            }

            auto t2 = Clock::now();
            read_ns += std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();

            // ---------- замер АЛГОРИТМА ----------
            auto t3 = Clock::now();

            if (cmd == "READ") {
                rm.Read(user, page);
            } else { // CHEER
                sink += rm.Cheer(user);
            }

            auto t4 = Clock::now();
            work_ns += std::chrono::duration_cast<std::chrono::nanoseconds>(t4 - t3).count();
        }
    }

    cerr << "sink=" << sink << "\n";
    cerr << "Input: " << (read_ns / 1'000'000) << " ms\n";
    cerr << "Algo:  " << (work_ns / 1'000'000) << " ms\n";
}
