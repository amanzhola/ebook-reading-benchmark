CXX := g++
CXXFLAGS := -O2 -std=c++17 -Wall -Wextra -Wpedantic

# -------------------- Binaries --------------------
SOLUTION_BIN := solution.exe

GEN_BIN := gen.exe
NAIVE_BIN := naive.exe
SQRT_BIN := sqrt.exe
FENWICK_BIN := fenwick.exe

NAIVE_BENCH_BIN := naive_bench.exe
SQRT_BENCH_BIN := sqrt_bench.exe
FENWICK_BENCH_BIN := fenwick_bench.exe

# -------------------- I/O files --------------------
INPUT := input.txt
OUT_NAIVE := naive.txt
OUT_SQRT := sqrt.txt
OUT_FENWICK := fenwick.txt

# -------------------- Phony targets --------------------
.PHONY: all solution gen bench diff bench_time bench_all_time clean

all: solution

# 1) Build submission solution
solution: $(SOLUTION_BIN)

$(SOLUTION_BIN): solution.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

# 2) Generator + input
gen: $(INPUT)

$(GEN_BIN): bench/generator.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(INPUT): $(GEN_BIN)
	$(GEN_BIN) > $(INPUT)

# 3) Build bench implementations (no timings)
bench: $(NAIVE_BIN) $(SQRT_BIN) $(FENWICK_BIN)

$(NAIVE_BIN): bench/reading_naive.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(SQRT_BIN): bench/reading_sqrt.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(FENWICK_BIN): bench/reading_fenwick.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

# 4) Compare outputs
diff: gen bench
	$(NAIVE_BIN) < $(INPUT) > $(OUT_NAIVE)
	$(SQRT_BIN) < $(INPUT) > $(OUT_SQRT)
	$(FENWICK_BIN) < $(INPUT) > $(OUT_FENWICK)
	fc $(OUT_NAIVE) $(OUT_FENWICK)
	fc $(OUT_SQRT) $(OUT_FENWICK)
	@echo OK: outputs match

# 5) Bench time: Fenwick only
bench_time: gen
	$(CXX) $(CXXFLAGS) -DENABLE_BENCHMARK bench/reading_fenwick.cpp -o $(FENWICK_BENCH_BIN)
	@echo === FENWICK ===
	$(FENWICK_BENCH_BIN) < $(INPUT) > NUL

# 6) Bench time: all 3
bench_all_time: gen
	$(CXX) $(CXXFLAGS) -DENABLE_BENCHMARK bench/reading_naive.cpp   -o $(NAIVE_BENCH_BIN)
	$(CXX) $(CXXFLAGS) -DENABLE_BENCHMARK bench/reading_sqrt.cpp    -o $(SQRT_BENCH_BIN)
	$(CXX) $(CXXFLAGS) -DENABLE_BENCHMARK bench/reading_fenwick.cpp -o $(FENWICK_BENCH_BIN)
	@echo === NAIVE ===
	$(NAIVE_BENCH_BIN) < $(INPUT) > NUL
	@echo === SQRT ===
	$(SQRT_BENCH_BIN) < $(INPUT) > NUL
	@echo === FENWICK ===
	$(FENWICK_BENCH_BIN) < $(INPUT) > NUL

# 7) Cleanup
clean:
	del /Q $(SOLUTION_BIN) $(GEN_BIN) $(NAIVE_BIN) $(SQRT_BIN) $(FENWICK_BIN) 2>NUL
	del /Q $(NAIVE_BENCH_BIN) $(SQRT_BENCH_BIN) $(FENWICK_BENCH_BIN) 2>NUL
	del /Q $(INPUT) $(OUT_NAIVE) $(OUT_SQRT) $(OUT_FENWICK) 2>NUL
