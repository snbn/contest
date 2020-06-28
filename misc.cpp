#include <cstdint>

using namespace std;

template <typename F>
void forComb(unsigned int n, unsigned int k, F f) {
  if (k == 0) {
    f(0);
    return;
  }
  int comb = (1 << k) - 1;
  while (comb < (1 << n)) {
    f(comb);

    int x = comb & -comb;
    int y = comb + x;
    comb = ((comb & ~y) / x >> 1) | y;
  }
}

template <typename F>
void forSubset(unsigned int s, F f) {
  unsigned int sub = s;
  do {
    f(sub);

    unsigned int lsb = ~(s ^ -s);
    sub -= lsb;
    sub = (sub & s);
  } while (sub != s);
}

int64_t fill_ones(int64_t a) {
  a |= a >> 1;
  a |= a >> 2;
  a |= a >> 4;
  a |= a >> 8;
  a |= a >> 16;
  a |= a >> 32;
  return a;
}

int64_t dceil(int64_t, int64_t);
int64_t dfloor(int64_t, int64_t);

int64_t dceil(int64_t x, int64_t y) {
  if (y < 0) {
    return dceil(-x, -y);
  }
  if (x < 0) {
    return -dfloor(-x, y);
  }
  return (x + y - 1) / y;
}

int64_t dfloor(int64_t x, int64_t y) {
  if (y < 0) {
    return dfloor(-x, -y);
  }
  if (x < 0) {
    return -dceil(-x, y);
  }
  return x / y;
}

template <typename F>
void forNeighbor4(int x, int y, F f) {
  for (int dx = -1; dx <= 1; dx++) {
    for (int dy = -1; dy <= 1; dy++) {
      if (abs(dx) + abs(dy) == 1) {
        f(x + dx, y + dy);
      }
    }
  }
}

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