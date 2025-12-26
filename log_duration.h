#pragma once
#include <chrono>
#include <iostream>
#include <string>

class LogDuration {
public:
    explicit LogDuration(const std::string& msg)
        : msg_(msg)
        , start_(std::chrono::steady_clock::now()) {}

    ~LogDuration() {
        using namespace std::chrono;
        auto end = steady_clock::now();
        std::cerr << msg_ << ": "
                  << duration_cast<milliseconds>(end - start_).count()
                  << " ms\n";
    }

private:
    std::string msg_;
    std::chrono::steady_clock::time_point start_;
};
