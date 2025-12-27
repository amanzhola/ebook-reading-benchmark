#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

// Fenwick tree (Binary Indexed Tree):
// stores count[1..n] and supports:
//  - Update(index, delta): count[index] += delta
//  - PrefixSum(index): sum(count[1..index])
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

// ReadingManager invariants:
//  - user_page_[user] = last read page (0 if no READ yet)
//  - total_readers_ = number of users with at least one READ
//  - page_readers_ stores count[page] = number of readers currently on that page
class ReadingManager {
private:
    static constexpr int kMaxUser = 100000;
    static constexpr int kMaxPage = 1000;

public:
    ReadingManager()
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

    ReadingManager manager;

    int query_count = 0;
    cin >> query_count;

    // As in the provided course skeleton: setprecision(6) (no fixed).
    cout << setprecision(6);

    for (int query_index = 0; query_index < query_count; ++query_index) {
        string command;
        int user = 0;
        cin >> command >> user;

        if (command == "READ") {
            int page = 0;
            cin >> page;
            manager.Read(user, page);
        } else if (command == "CHEER") {
            cout << manager.Cheer(user) << "\n";
        }
    }

    return 0;
}
