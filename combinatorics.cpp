#include <cstdint>
#include <vector>

using namespace std;

const int64_t MOD = 1'000'000'007L;

int64_t dpow(int64_t x, int64_t y) {
  if (y == 0) {
    return 1;
  }
  if (y % 2 == 0) {
    return dpow(x * x % MOD, y / 2);
  } else {
    return x * dpow(x, y - 1) % MOD;
  }
}

int64_t inv(int64_t x) { return dpow(x, MOD - 2); }

int64_t comb(int64_t n, int64_t k) {
  int64_t a = 1, b = 1;
  for (int64_t i = 0; i < k; i++) {
    a *= (n - i) % MOD;
    a %= MOD;
    b *= (i + 1) % MOD;
    b %= MOD;
  }
  return a * inv(b) % MOD;
}

int64_t perm(int64_t n, int64_t k) {
  int64_t r = 1;
  for (int64_t i = 0; i < k; i++) {
    r *= (n - i) % MOD;
    r %= MOD;
  }
  return r;
}

/// lookup table of k! for k in {1, ..., n}
vector<int64_t> factorials(size_t n, size_t MOD) {
  vector<int64_t> result(n + 1, 1);
  for (size_t i = 1; i <= n; i++) {
    result[i] = result[i - 1] * i % MOD;
  }
  return result;
}

/**
 *  lookup table of stirling number s(i,j) for i in {1, ..., n} and j in {1,
 *  ..., m}
 * s(i, j) == the number of partition of n items into m nonempty groups
 * */
vector<vector<int64_t>> stirling2(size_t n, size_t m, size_t MOD) {
  vector<vector<int64_t>> result(n + 1, vector<int64_t>(m + 1, 0));
  result[0][0] = 1;
  for (size_t i = 1; i <= n; i++)
    for (size_t j = 1; j <= i; j++) {
      result[i][j] = result[i - 1][j - 1] + j * result[i - 1][j] % MOD;
      result[i][j] %= MOD;
    }
  return result;
}
