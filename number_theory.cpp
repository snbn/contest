#include <sstream>
#include <tuple>
#include <utility>
#include <vector>

using namespace std;

bool is_prime(size_t n) {
  for (size_t p = 2; p * p <= n; p++) {
    if (n % p == 0) {
      return false;
    }
  }

  return true;
}

vector<size_t> factors(size_t n) {
  vector<size_t> result;
  for (size_t d = 1; d * d <= n; d++) {
    if (n % d == 0) {
      result.push_back(d);
      if (n / d != d) {
        result.push_back(n / d);
      }
    }
  }
  return result;
}

vector<pair<size_t, size_t>> factorize(size_t n) {
  vector<pair<size_t, size_t>> result;
  for (size_t p = 2; p * p <= n; p++) {
    if (is_prime(p)) {
      size_t cnt = 0;
      while (n % p == 0) {
        n /= p;
        cnt++;
      }
      if (cnt > 0) {
        result.push_back(make_pair(p, cnt));
      }
    }
  }
  if (n != 1) {
    result.push_back(make_pair(n, 1));
  }
  return move(result);
}

vector<bool> eratosthenes_sieve(size_t size) {
  vector<bool> is_prime(size + 1, true);
  for (size_t i = 0; i <= size; i++) {
    if (i < 2) {
      is_prime[i] = false;
    } else if (is_prime[i]) {
      for (size_t j = 2 * i; j <= size; j += i) {
        is_prime[j] = false;
      }
    }
  }
  return move(is_prime);
}

size_t totient(const size_t n) {
  vector<pair<size_t, size_t>> fs = factorize(n);
  size_t result = n;

  for (auto pa : fs) {
    size_t p;
    tie(p, ignore) = pa;
    result /= p;
    result *= p - 1;
  }
  return result;
}

tuple<int64_t, int64_t, int64_t> xgcd(int64_t a, int64_t b) {
  if (b == 0) {
    return make_tuple(1, 0, a);
  }
  int64_t z, w, d;
  tie(z, w, d) = xgcd(b, a % b);
  return make_tuple(w, z - a / b * w, d);
}

tuple<int64_t, int64_t> chinese_rem(int64_t a1, int64_t a2, int64_t m1,
                                    int64_t m2) {
  int64_t x, y, d;
  tie(x, y, d) = xgcd(m1, m2);

  const int64_t c = a2 - a1;
  if (c % d != 0) {
    return make_tuple(0, 0);
  }
  const int64_t tmp = c / d * x % (m2 / d);
  const int64_t sln = a1 + tmp * m1;
  const int64_t M = m1 / d * m2;

  return make_tuple((sln % M + M) % M, M);
}

tuple<int64_t, int64_t> chinese_rem(const vector<int64_t>& a,
                                    const vector<int64_t>& MOD) {
  if (a.size() != MOD.size()) {
    stringstream msg;
    msg << "a.size() is " << a.size() << ", but MOD.size()" << MOD.size()
        << "\n";
    throw msg.str();
  }
  int64_t sln = 0, M = 1;
  for (size_t i = 0; i < a.size(); i++) {
    tie(sln, M) = chinese_rem(sln, a[i], M, MOD[i]);
    if (M == 0) {
      return make_tuple(0, 0);
    }
  }

  return make_tuple(sln, M);
}