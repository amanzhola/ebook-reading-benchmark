#include <bits/stdc++.h>
#include "log_duration.h"
using namespace std;

class Fenwick {
public:
    Fenwick(int n) : n_(n), f_(n + 1) {}
    void Add(int i, int v) {
        for (; i <= n_; i += i & -i) f_[i] += v;
    }
    int Sum(int i) const {
        int s = 0;
        for (; i > 0; i -= i & -i) s += f_[i];
        return s;
    }
private:
    int n_;
    vector<int> f_;
};

class ReadingFenwick {
public:
    static const int MAX_USER = 100000;
    static const int MAX_PAGE = 1000;

    ReadingFenwick()
        : user_page_(MAX_USER + 1, 0),
          fw_(MAX_PAGE),
          readers_(0) {}

    void Read(int user, int page) {
        int old = user_page_[user];
        if (old == 0) ++readers_;
        else fw_.Add(old, -1);

        user_page_[user] = page;
        fw_.Add(page, +1);
    }

    double Cheer(int user) const {
        int p = user_page_[user];
        if (p == 0) return 0.0;
        if (readers_ == 1) return 1.0;
        int less = fw_.Sum(p - 1);
        return double(less) / (readers_ - 1);
    }

private:
    vector<int> user_page_;
    Fenwick fw_;
    int readers_;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ReadingFenwick rm;
    int Q;
    cin >> Q;

    using Clock = std::chrono::steady_clock;
    long long read_ns = 0;
    long long work_ns = 0;

    double sink = 0;

    {
        LogDuration total("Fenwick TOTAL");

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
